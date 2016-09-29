//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL console.c
 *
 * @brief Console commands
 * @author Mindspeed Technologies
 * @version $Revision: 1.261 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "4gmx.h"
#include "console.h"
#include "stream.h"
#include <stdio.h>
#include "appprintf.h"
#include "schtaskdrv.h"
#include "systimers.h"
#include "supervisor.h"
#include "initcpu.h"
#include "syscorecfg.h"
#include <string.h>
#include "fpdrv.h"
#include <stdlib.h>
#include "sirqassign.h"
#include "version.h"
#include "gemdrv.h"
#include "DSPdrv.h"
#include "mdmadrv.h"
#include "ncnbdata.h"
#include "spidrv.h"
#include "pciedrv.h"
#include "evmfpga.h"
#include "dejitter.h"
#include "ad9361radio.h"
#include "logmgr.h"
#include "iqphone.h"
#include "timerdrv.h"
#include "dataint.h"
#include "genlog.h"
#include "mlog.h"
#include "uartdrv.h"
#include "cpridrv.h"
#ifdef DIAG_SPACC_ENABLED
#include "spaccdrv.h"
#endif
#ifdef SRIO_DRV_ENABLED
#include "sriodrv.h"
#endif

#include "rex.h"
#include "FECdrv.h"
#include "network/network.h"
#include "network/sctp/sctp.h"
#ifdef USE_IPERF
#include "network/iperf/iperf.h"
#endif
#include "network/utils.h"
#include "network/dhcp/dhcp.h"
#include "icdrv.h"
#include "globalstoragearrays.h"
#include "lcorecfg.h"
#include "sysddrctrl.h"
#ifdef PTM_ENABLED
#include "coresight.h"
#endif
#include "rexdiag.h"
#ifdef PM_ENABLED
#include "pm.h"
#endif // #ifdef PM_ENABLED

#ifdef UNIDIAG_ENABLED
#include "unified_diag.h"
#endif

#include "tracer.h"
#include "cevamdmadrv.h"
#include "rex_serv.h"
#include "ceva_proc.h"
#include "rootinfo.h"
#include "dbgfunctab.h"

#include "conicpu.h"

extern ICORE_PART_CFG ICorePartListCfg[SVSR_ICORE_NUM];

#ifdef DIAG_SPACC_ENABLED
extern unsigned char spaccCmdText[];
#endif
UINT32 CLineErrCmd (CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineDump (CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCheckObjCRC(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineMemGet (CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineMemSet (CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 UartCmdHelp (CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdCheck (CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdCheck2 (CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdCheck3 (CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdCheck4 (CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdCheck5 (CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdArmTest (CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdTicks (CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdDelay (CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdCheckSys (CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdCheckDCache (CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdDbg (CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdIram (CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdFill (CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineTestIramAlloc(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineTestMemCpy(CMgrCmdCtx * a, CMgrCmdParamCtx * b);



#if defined(_MX_DEBUG) && defined(MX_CREATE_OBJ_LOG_ENABLED)
UINT32 CLineCmdSysObj (CMgrCmdCtx * a, CMgrCmdParamCtx * b);
#endif

UINT32 CLineCmdDumpMask (CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdCheckApi (CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdCheckHeap (CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdDrvRealloc (CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdInjectThr (CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdDspDump(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdShutdown(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdL2Check(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdDumpICObj(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdMsgTest(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdRegSubApiHandle(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdRegSubApiHandleTst(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdPrintVersion(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdHeapsInfo(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdGemDump(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdHeapLC(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdRadMdma(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdSysMdma(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdFreq(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdCheckIMsg(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdGemInit(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdGemSend(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdGenDAbort(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdPartAlloc(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdTestSpi(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdTestPcie(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdReadPcie(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdWritePcie(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdDebugPcie(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdTestApiLogger(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdTestICPU(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdTestICore(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdTestEvmFpga(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdTestAd9361(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdStopEvmFpga(CMgrCmdCtx * a, CMgrCmdParamCtx * b);

#ifdef MLOG_ENABLED
UINT32 CLineCmdMLogInfo(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdMLogSetMask(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdMLogReset(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdMLogFlush(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdMLogClose(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdMLogFinalize(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
#endif


UINT32 CLineCmdApiLogInfo(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdApiLogSetMask(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdApiLogSetMaskNmm(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdApiLogReset(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdApiLogReload(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdApiLogFlush(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);

UINT32 CLineCmdApiInvMem(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdApiCleanMem(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdApiLogCtrl(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);

UINT32 CLineCmdCheckHeapAlloc(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdCheckMxTicks(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineTestDDR1(CMgrCmdCtx * pCtx, CMgrCmdParamCtx * pParams);
UINT32 CLineCmdLogMgr(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdIQPhoneInit(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdIQPhoneStart(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdHeapAPICheck(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdCheckTmr(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdCheckTmrWDTimer(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdCheckTmrWDWatchdog(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdCheckTmrWDWatchdog2(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdDi(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdCpriGpio(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdGenLog(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdTestMmu(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdRDump(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdMMUL2Test(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdTestAppHeap(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdSetAP(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
#ifdef DIAG_SPACC_ENABLED
UINT32 CLineCmdTestSPAcc(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
#endif
#ifdef SRIO_DRV_ENABLED
UINT32 CLineCmdSRIO(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
#endif
UINT32 CLineCmdHoldPrint(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdPrintData(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdReleasePrint(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdPrintSomeData(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdRePrint(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdReExclTest(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdFecLogTest(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdNetTest(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdRvds(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdCpriConfig(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdThreadMsg(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdSCTPTest(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdSFP(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdCRC(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdGetInfo(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdUT(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdDHCP(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdIRQL(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdICpuStat(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdICoreStat(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdSetFreq(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdTestCeva(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdPackHeap(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCmdSysInfo(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineDDRProtectionTest(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineSMPTest(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
#ifdef PTM_ENABLED
UINT32 CLineCmdPTMTraceTest(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
#endif
UINT32 CLineRexTest(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineSchedMeasurement(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineRSchedMeasurement(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineThrSwMeasurement(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineQPMeasurement(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
#ifdef PM_ENABLED
UINT32 CLinePMCacheTest(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
#endif // #ifdef PM_ENABLED
UINT32 CLineSigTests(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineICpuChkTests(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineCheckPM(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);

UINT32 CLineCmd(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);

#ifdef UNIDIAG_ENABLED
UINT32 CLineUDRedirectUDP(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineUDEnable(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineUDAPIDiagConfig(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
#endif
UINT32 CLineCpuInfo(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineGenApiTest(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineIntInfo(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineDILog(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
#ifdef SYS_TRACER_ENABLED
UINT32 CLineTrace(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 CLineSvsrTraceEnable(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
#endif
UINT32 CLineCmdSWDiagStart(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdSWDiagPrint(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdSWDiagClose(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdAd9361GetRSSI(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdPtb(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdTestCevaMDMA(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdIrqPrior(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdMmuInfo(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdStackParce(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdCICPUTest(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdThreads(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdTimers(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
UINT32 CLineCmdClear(CMgrCmdCtx * a, CMgrCmdParamCtx * b);

// Mainly 180 element for static and 20 for the dynamic registrations
// to allow PHY to register new elements in this table by calling: CMgrRegisterHandler

CMgrMenuElm Menu  [200] =
{
    {CM_OBJ_CMD, "help",        NULL,   CLineCmdHelp,        "List of commands",   "USAGE: ""help",                      NULL},
    {CM_OBJ_CMD, "?",           NULL,   CLineCmdHelp,        "List of commands",   "USAGE: ""help",                      NULL},
    {CM_OBJ_CMD, "clear",       NULL,   CLineCmdClear,       "Clear screen",       "USAGE: ""clear",                     NULL},

    {CM_OBJ_CMD, "dump",        NULL,   CLineDump,           "Dump of memory",     "USAGE: ""dump <addr> <nbytes> [<1-invalidate cache first>]",      NULL},
    {CM_OBJ_CMD, "dlcrc",       NULL,   CLineCheckObjCRC,    "To check the DL crc","USAGE: ""dlcrc <sf-num>",            NULL},
    {CM_OBJ_CMD, "smem",        NULL,   CLineMemSet,         "Set 32 bit memory",  "USAGE: ""smem <addr> <val> <range>", NULL},
    {CM_OBJ_CMD, "rmem",        NULL,   CLineMemGet,         "Read 32 bit memory", "USAGE: ""rmem <addr>",               NULL},
    {CM_OBJ_CMD, "test",        NULL,   CLineCmdCheck,       "",                   "USAGE: ""test",                      NULL},
    {CM_OBJ_CMD, "test2",       NULL,   CLineCmdCheck2,      "",                   "USAGE: ""test",                      NULL},
    {CM_OBJ_CMD, "test3",       NULL,   CLineCmdCheck3,      "",                   "USAGE: ""test",                      NULL},

    #ifdef MX_MTHREAD_ENABLED
    {CM_OBJ_CMD, "test4",       NULL,   CLineCmdCheck4,      "m-threads testing",  "USAGE: ""test4",                     NULL},
    {CM_OBJ_CMD, "test5",       NULL,   CLineCmdCheck5,      "frac-timer test",  "USAGE: ""test5",     NULL},
    #endif

    {CM_OBJ_CMD, "armt",        NULL,   CLineCmdArmTest,     "",                   "USAGE: ""armt",                      NULL},
    {CM_OBJ_CMD, "tick",        NULL,   CLineCmdTicks,       "",                   "USAGE: ""tick",                      NULL},
    {CM_OBJ_CMD, "delay",       NULL,   CLineCmdDelay,       "",                   "USAGE: ""delay",                     NULL},
    {CM_OBJ_CMD, "chksys",      NULL,   CLineCmdCheckSys,    "",                   "USAGE: ""chksys",                    NULL},

#if defined(_MX_DEBUG) && defined(MX_CREATE_OBJ_LOG_ENABLED)
    {CM_OBJ_CMD, "sysobj",      NULL,   CLineCmdSysObj,      "",                   "USAGE: ""sysobj",                    NULL},
#endif
    {CM_OBJ_CMD, "apidump",     NULL,	CLineCmdDumpMask,    "",                    "USAGE: ""apidump <mask: 1-rx, 2-tx, 3-tx+rx>",                 NULL},
    {CM_OBJ_CMD, "genapi",      NULL,	CLineGenApiTest,    "",                    "USAGE: ""genapi <tti> <num> <listlen> <test type>| genapi off",            NULL},
    {CM_OBJ_CMD, "chkd",        NULL,	CLineCmdCheckDCache, "",                    "USAGE: ""chkd",                 NULL},
    {CM_OBJ_CMD, "dbg",         NULL,	CLineCmdDbg, "",                    "USAGE: ""test",                  NULL},
    {CM_OBJ_CMD, "iram",        NULL,	CLineCmdIram, "",                    "USAGE: ""iram",                 NULL},
    {CM_OBJ_CMD, "fill",        NULL,   CLineCmdFill, "",                    "USAGE: ""iram",                 NULL},
    {CM_OBJ_CMD, "api",         NULL,   CLineCmdCheckApi, "",                    "USAGE: ""api [cpu]",        NULL},
    {CM_OBJ_CMD, "iram_alloc",  NULL,   CLineTestIramAlloc, "",             "USAGE: ""iram",                 NULL},
    {CM_OBJ_CMD, "memcpytest",  NULL,   CLineTestMemCpy, "",             "USAGE: ""memcpytest",                 NULL},
    {CM_OBJ_CMD, "dcfg",        NULL,   CLineCmdDrvRealloc, "",             "USAGE: ""dcfg",                 NULL},
    {CM_OBJ_CMD, "inj",         NULL,   CLineCmdInjectThr, "",             "USAGE: ""inj",                 NULL},
    {CM_OBJ_CMD, "dsp_dump",    NULL,   CLineCmdDspDump, "",               "USAGE: ""dsp_dump [DRAM size] [dspID]]",                 NULL},
    {CM_OBJ_CMD, "sd",          NULL,   CLineCmdShutdown, "System shutdown",             "USAGE: ""sd",                 NULL},
    {CM_OBJ_CMD, "l2",          NULL,   CLineCmdL2Check, "L2 cach testing",             "USAGE: ""l2 [opt]",                 NULL},
    {CM_OBJ_CMD, "dumpic",      NULL,   CLineCmdDumpICObj, "Dump IC objects",             "USAGE: ""dumpic",       NULL},
    {CM_OBJ_CMD, "msg",         NULL,   CLineCmdMsgTest,  "To test supervisor messages",     "USAGE: ""msg [cpu_id]", NULL},
    {CM_OBJ_CMD, "rapih",       NULL,   CLineCmdRegSubApiHandle,  "To register API handler on remote core",     "USAGE: ""rapih [cpu_id]", NULL},
    {CM_OBJ_CMD, "rapitst",     NULL,   CLineCmdRegSubApiHandleTst,  "To register API handler on remote core",     "USAGE: ""rapitst [cpu_id]", NULL},
    {CM_OBJ_CMD, "version",     NULL,   CLineCmdPrintVersion,  "To print branch & version info",     "USAGE: ""version", NULL},
    {CM_OBJ_CMD, "gem_init",    NULL,   CLineCmdGemInit,  "GEM INIT",     "USAGE: ""gem_init", NULL},
    {CM_OBJ_CMD, "gem_tx",      NULL,   CLineCmdGemSend,  "GEM TX",     "USAGE: ""gem_tx [frm num] [frm size]", NULL},
    {CM_OBJ_CMD, "gem_rx",      NULL,   CLineCmdGemDump,  "GEM RX",     "USAGE: ""gem_rx [0-stop, 1-start]", NULL},
    {CM_OBJ_CMD, "heapinfo",    NULL,   CLineCmdHeapLC,  "heapinfo",     "USAGE: ""heapinfo [app, ncnb, dsp, iram, ...]", NULL},
    {CM_OBJ_CMD, "radmdma",     NULL,   CLineCmdRadMdma, "The test of RW operation of radio MDMA", "USAGE: ""RadMDMA", NULL},
    {CM_OBJ_CMD, "sysmdma",     NULL,   CLineCmdSysMdma, "The test of RW operation of SYSMDMA", "USAGE: ""SYSMDMA", NULL},
    {CM_OBJ_CMD, "freq",        NULL,   CLineCmdFreq,    "System frequency", "USAGE: ""freq", NULL},
    {CM_OBJ_CMD, "setfreq",     NULL,   CLineCmdSetFreq,    "To set system frequency", "USAGE: ""setfreq <dev> <freq in Mhz> [map to pll]", NULL},
    {CM_OBJ_CMD, "imsg",        NULL,   CLineCmdCheckIMsg, "To test I-MSG", "USAGE: ""imsg [num]", NULL},
    {CM_OBJ_CMD, "dabort",      NULL,   CLineCmdGenDAbort, "To generate test D-ABORT", "USAGE: ""dabort", NULL},
    {CM_OBJ_CMD, "palloc",      NULL,   CLineCmdPartAlloc, "Check partition alloc functions", "USAGE: ""palloc", NULL},
    {CM_OBJ_CMD, "testspi",     NULL,   CLineCmdTestSpi, "To test SPI interface", "USAGE: ""testspi", NULL},
    {CM_OBJ_CMD, "testpcie",    NULL,   CLineCmdTestPcie, "To test PCIe interface", "USAGE: ""testpcie <EP (0)/RC (1)> <SerDes #> <Num Lanes>", NULL},
    {CM_OBJ_CMD, "rpcie",       NULL,   CLineCmdReadPcie, "Read from PCIe interface", "USAGE: ""rpcie <addr>", NULL},
    {CM_OBJ_CMD, "wpcie",       NULL,   CLineCmdWritePcie, "Write to PCIe interface", "USAGE: ""wpcie <addr> <val>", NULL},
    {CM_OBJ_CMD, "checkpcieif", NULL,   CLineCmdDebugPcie, "Check PCIe interface", "USAGE: ""checkpcieif <EP(0)/RC(1)>", NULL},
    {CM_OBJ_CMD, "apilog",      NULL,   CLineCmdTestApiLogger, "Get logger info", "USAGE: ""apilog", NULL},
    {CM_OBJ_CMD, "icpu",        NULL,   CLineCmdTestICPU, "To test icpu", "USAGE: ""icpu", NULL},
    {CM_OBJ_CMD, "icore",       NULL,   CLineCmdTestICore, "To see free icore memory partitions", "USAGE: ""icore", NULL},
    {CM_OBJ_CMD, "evm",         NULL,   CLineCmdTestEvmFpga, "To test evmfpga", "USAGE: ""evm", NULL},
    {CM_OBJ_CMD, "adi",         NULL,   CLineCmdTestAd9361, "To initialize ADI9361", "USAGE: ""adi [profile]", NULL},
    {CM_OBJ_CMD, "aditest",     NULL,   CLineCmdAd9361GetRSSI, "ADI9361 test and debug stuff", NULL},
    {CM_OBJ_CMD, "evmstop",     NULL,   CLineCmdStopEvmFpga, "To stop ADI9361 antenna", "USAGE: ""evmstop", NULL},

    {CM_OBJ_CMD, "heapalloc",   NULL,   CLineCmdCheckHeapAlloc, "To check heap allocation", "USAGE: ""heapalloc [size]", NULL},
    {CM_OBJ_CMD, "checkheap",   NULL,   CLineCmdCheckHeap, "",                    "USAGE: ""checkheap",       NULL},
    {CM_OBJ_CMD, "heaps",       NULL,   CLineCmdHeapsInfo,  "Heap information",     "USAGE: ""heaps", NULL},

    {CM_OBJ_CMD, "clean",       NULL,   CLineCmdApiCleanMem, "Memory range cleaning (cache control)", "USAGE: ""clean <mem> <size>", NULL},
    {CM_OBJ_CMD, "inv",         NULL,   CLineCmdApiInvMem, "Memory range invalidating (cache control)", "USAGE: ""inv <mem> <size>", NULL},

    {CM_OBJ_CMD, "delayticks",  NULL,   CLineCmdCheckMxTicks, "To check MxDelayTick", "USAGE: ""delayticks <ticks>", NULL},
    {CM_OBJ_CMD, "testddr1",    NULL,   CLineTestDDR1, "To verify DDR1 heap", "USAGE: ""testddr1", NULL},
    {CM_OBJ_CMD, "iqinit",      NULL,   CLineCmdIQPhoneInit, "To initialize IQ player/recorder", "USAGE: ""iqinit", NULL},
    {CM_OBJ_CMD, "iq",          NULL,   CLineCmdIQPhoneStart, "To start IQ player/recorder", "USAGE: ""iq <bit0-tx,bit1-rx>", NULL},
    {CM_OBJ_CMD, "heapat",      NULL,   CLineCmdHeapAPICheck, "To check new heap API (allocat and allocinrange)", "USAGE: ""heapat", NULL},
    {CM_OBJ_CMD, "tmr",         NULL,   CLineCmdCheckTmr, "To check new timer interface", "USAGE: ""tmr", NULL},
    {CM_OBJ_CMD, "tmrwd1",      NULL,   CLineCmdCheckTmrWDTimer, "To check watchdog timer interface (timer mode)", "USAGE: ""tmrwd1 [period]", NULL},
    {CM_OBJ_CMD, "tmrwd2",      NULL,   CLineCmdCheckTmrWDWatchdog, "To check watchdog timer interface (watchdog mode)", "USAGE: ""tmrwd2", NULL},
    {CM_OBJ_CMD, "tmrwd3",      NULL,   CLineCmdCheckTmrWDWatchdog2, "To check watchdog timer interface (watchdog mode)", "USAGE: ""tmrwd2", NULL},
    {CM_OBJ_CMD, "di",          NULL,   CLineCmdDi, "To check HAL for data interface", "USAGE: ""di [mode]", NULL},
    {CM_OBJ_CMD, "cprigpio",    NULL,   CLineCmdCpriGpio, "To toggle GPIO by CPRI Tx and Rx IRQs", "USAGE: ""cprigpio <'txIRQ' ena> ['txIRQ' port] ['rxIRQ' ena] ['rxIRQ' port]", NULL},
#ifdef DIAG_SPACC_ENABLED
    {CM_OBJ_CMD, "spacc",       NULL,   CLineCmdTestSPAcc, "SPAcc sanity check", (char*)spaccCmdText, NULL},
#endif

    // Loggers
    //-------
    {CM_OBJ_DELIMITER, "",      NULL,   NULL,               "[Logger commands]", NULL},
    {CM_OBJ_CMD, "logmgr",      NULL,   CLineCmdLogMgr,     "To test log manager", "USAGE: ""logmgr", NULL},
    {CM_OBJ_CMD, "logctrl",     NULL,   CLineCmdApiLogCtrl, "Save API log/Restore/Free tmp storage", "USAGE: ""logctrl <0-save, 1-restore, 2-free>", NULL},

    {CM_OBJ_DELIMITER, "",      NULL,   NULL,               "[API logger commands]", NULL},
    {CM_OBJ_CMD, "svsrloginfo", NULL,   CLineCmdApiLogInfo,  "To show the log information", "USAGE: ""svsrloginfo", NULL},
    {CM_OBJ_CMD, "svsrlogmask", NULL,   CLineCmdApiLogSetMask, "To set API log mask", "USAGE: ""svsrlogmask <mask>", NULL},
    {CM_OBJ_CMD, "svsrlogmasknmm",NULL, CLineCmdApiLogSetMaskNmm, "To set NMM API log mask", "USAGE: ""svsrlogmasknmm <mask>", NULL},
    {CM_OBJ_CMD, "svsrlogreset",NULL,   CLineCmdApiLogReset, "To reset the API log and erase data with zero", "USAGE: ""svsrlogreset", NULL},
    {CM_OBJ_CMD, "svsrlogreload",NULL,  CLineCmdApiLogReload, "To reload the log from preloaded data", "USAGE: ""svsrlogreload [1-reload cache (for Linux)]", NULL},
    {CM_OBJ_CMD, "svsrlogflush",NULL,   CLineCmdApiLogFlush, "To flush the log storage to the DDR", "USAGE: ""svsrlogflush", NULL},
#ifdef MLOG_ENABLED
    {CM_OBJ_DELIMITER, "",      NULL,   NULL,               "[MLOG commands]", NULL},
    {CM_OBJ_CMD, "mloginfo",    NULL,   CLineCmdMLogInfo,  "To show MLog information", "USAGE: ""mloginfo", NULL},
    {CM_OBJ_CMD, "mlogmask",    NULL,   CLineCmdMLogSetMask, "To set MLog mask", "USAGE: ""mlogmask <mask>", NULL},
    {CM_OBJ_CMD, "mlogreset",   NULL,   CLineCmdMLogReset, "To reset / open the MLog buffers", "USAGE: ""mlogreset", NULL},
    {CM_OBJ_CMD, "mlogflush",   NULL,   CLineCmdMLogFlush, "To flush MLog storage to the DDR", "USAGE: ""mlogflush", NULL},
    {CM_OBJ_CMD, "mlogclose",   NULL,   CLineCmdMLogClose, "To close MLog and clear all buffers", "USAGE: ""mlogclose", NULL},
    {CM_OBJ_CMD, "mlogfinalize",NULL,   CLineCmdMLogFinalize, "To finalize MLog storage (merge all MLOG Buffers into 1)", "USAGE: ""mlogfinalize", NULL},
#endif
    {CM_OBJ_DELIMITER, "",      NULL,   NULL,               NULL, NULL},
    {CM_OBJ_CMD, "genlog",      NULL,   CLineCmdGenLog, "To test general log functionality", "USAGE: ""genlog", NULL},
    {CM_OBJ_CMD, "mmu",         NULL,   CLineCmdTestMmu, "Check mmu functionality", "USAGE: ""mmu [opt]", NULL},
    {CM_OBJ_CMD, "rdump",       NULL,   CLineCmdRDump, "R-DUMP: dump of memory from remote ARM", "USAGE: ""rdump <cpu id> <addr> <size>", NULL},
    {CM_OBJ_CMD, "mmu64",       NULL,   CLineCmdMMUL2Test, "To test MMU page mapping", "USAGE: ""mmu64", NULL},
    {CM_OBJ_CMD, "test4",       NULL,   CLineCmdTestAppHeap, "", "USAGE: ""test4", NULL},
    {CM_OBJ_CMD, "ap",          NULL,   CLineCmdSetAP, "", "USAGE: ""ap", NULL},
#ifdef SRIO_DRV_ENABLED
    {CM_OBJ_CMD, "srio",        NULL,   CLineCmdSRIO, "", "USAGE: ""srio", NULL},
#endif
    {CM_OBJ_CMD, "irqpr",       NULL,   CLineCmdIrqPrior, "IRQ priority test", "USAGE: ""irqpr", NULL},

    {CM_OBJ_CMD, "excl",        NULL,   CLineCmdReExclTest, "It tests exclusive set/inc/dec 4GMX API", "USAGE: ""excl", NULL},
    {CM_OBJ_CMD, "feclog",      NULL,   CLineCmdFecLogTest, "It tests FEC logging", "USAGE: ""feclog", NULL},
    {CM_OBJ_CMD, "net",        	NULL,   CLineCmdNetTest, "It tests net", "USAGE: ""net cmd", NULL},
    {CM_OBJ_CMD, "rvds",        NULL,   CLineCmdRvds, "", "USAGE: ""rvds", NULL},
    {CM_OBJ_CMD, "cpriconf",    NULL,   CLineCmdCpriConfig, "", "USAGE: ""rvds", NULL},
    {CM_OBJ_CMD, "thrmsg",      NULL,   CLineCmdThreadMsg, "To check 4GMX thread messages", "USAGE: ""thrmsg", NULL},
    {CM_OBJ_CMD, "sctp",       	NULL,   CLineCmdSCTPTest, "It tests sctp", "USAGE: ""sctp cmd", NULL},
    {CM_OBJ_CMD, "sfp",       	NULL,   CLineCmdSFP, "It enables SFP (for CPRI)", "USAGE: ""sfp [srds id = 1] [mask = 3]", NULL},
    {CM_OBJ_CMD, "crc",       	NULL,   CLineCmdCRC, "To calculate CRC", "USAGE: ""crc <addr> <size>", NULL},
    {CM_OBJ_CMD, "icpuinfo",  	NULL,   CLineCmdGetInfo, "To print i-cpu info", "USAGE: ""icpuinfo", NULL},
    {CM_OBJ_CMD, "irql",  	NULL,   CLineCmdIRQL, "irq-l", "USAGE: ""irq [cpuid]", NULL},
    {CM_OBJ_CMD, "icpustat",  	NULL,   CLineCmdICpuStat, "I-CPU statistics", "USAGE: ""icpustat", NULL},
    {CM_OBJ_CMD, "icorestat",  	NULL,   CLineCmdICoreStat, "I-CORE statistics", "USAGE: ""icorestat", NULL},    

    {CM_OBJ_CMD, "ut",          NULL,   CLineCmdUT, "UT-unit test", "USAGE: ""ut", NULL},
    {CM_OBJ_CMD, "dhcp",        NULL,   CLineCmdDHCP, "DHCP test", "USAGE: ""dhcp", NULL},
    {CM_OBJ_CMD, "cevatst",     NULL,   CLineCmdTestCeva, "To test ceva pool", "USAGE: ""ceva", NULL},
    {CM_OBJ_CMD, "packheap",    NULL,   CLineCmdPackHeap, "To pack the heap", "USAGE: ""packheap <heap name>", NULL},
    {CM_OBJ_CMD, "info",        NULL,   CLineCmdSysInfo, "To provide the system information", "USAGE: ""info", NULL},
    {CM_OBJ_CMD, "cpuinfo",     NULL,   CLineCpuInfo, "To provide current state of ARM core", "USAGE: ""cpuinfo <cpu_idx>", NULL},
    {CM_OBJ_CMD, "intinfo",     NULL,   CLineIntInfo, "To show information about interrupts", "USAGE: ""infoinfo", NULL},
    {CM_OBJ_CMD, "threadinfo",  NULL,   CLineCmdThreads, "To show information about system threads", "USAGE: ""threadinfo [cpu-id]", NULL},
    {CM_OBJ_CMD, "timerinfo",   NULL,   CLineCmdTimers, "To show information about system timers", "USAGE: ""timerinfo [cpu-id]", NULL},

    {CM_OBJ_CMD, "ddrprotect",  NULL,   CLineDDRProtectionTest, "DDR protection test", "USAGE: ddrprotect <0|1> <lower addr> <upper addr>", NULL},
    {CM_OBJ_CMD, "smp",         NULL,   CLineSMPTest, "SMP tests", "USAGE: smp", NULL},
#ifdef PTM_ENABLED
    {CM_OBJ_CMD, "trace",       NULL,   CLineCmdPTMTraceTest, "coresight ptm trace", "USAGE: ""ptm trace", NULL},
#endif
    {CM_OBJ_CMD, "rex",         NULL,   CLineRexTest, "REX tests", "USAGE: rex [0-TCB priors]", NULL},
    {CM_OBJ_CMD, "schedm",      NULL,   CLineSchedMeasurement, "schedm", "USAGE: schedm", NULL},
    {CM_OBJ_CMD, "rschedm",     NULL,   CLineRSchedMeasurement, "rschedm", "USAGE: rschedm", NULL},
    {CM_OBJ_CMD, "swthrm",      NULL,   CLineThrSwMeasurement, "swthr", "USAGE: swthr", NULL},
    {CM_OBJ_CMD, "qpm",         NULL,   CLineQPMeasurement, "qpm", "USAGE: qpm", NULL},
#ifdef PM_ENABLED
    {CM_OBJ_CMD, "pmcachetest", NULL,   CLinePMCacheTest, "PM: testing data cache miss/hit", "USAGE: pmcachetest", NULL},
#endif //#ifdef PM_ENABLED

    {CM_OBJ_CMD, "sigtest",     NULL,   CLineSigTests, "sigtest - to test I-SIGNALLING", "USAGE: sigtest", NULL},
    {CM_OBJ_CMD, "icpuchk",     NULL,   CLineICpuChkTests, "icpuchk - to test new I-CPU by emulate RX", "USAGE: icpuchk", NULL},
    {CM_OBJ_CMD, "pm",          NULL,   CLineCheckPM, "pm - to test PM service", "USAGE: pm", NULL},

    {CM_OBJ_DELIMITER, "",      NULL,   NULL,     "[LTE PHY Commands]", NULL},
    {CM_OBJ_CMD, "createphy",   NULL,   NULL,     "Create Phy instance along with testmac", "USAGE: createphy 2", NULL},
    {CM_OBJ_CMD, "phystart",    NULL,   NULL,     "Start the Phy that was created",  "USAGE: phystart <mode> <period> <count>", NULL},
    {CM_OBJ_CMD, "phystop",     NULL,   NULL,     "Stop / Pause Phy", "USAGE: phystop", NULL},
    {CM_OBJ_CMD, "physhutdown", NULL,   NULL,     "Shutdown Phy", "USAGE: physhutdown", NULL},
    {CM_OBJ_CMD, "phystats",    NULL,   NULL,     "Display PhyStats", "USAGE: phystats", NULL},
    {CM_OBJ_CMD, "phymemstat",  NULL,   NULL,     "Display Memory Status allcoation of PHY buffers", "USAGE: phymemstat", NULL},
    {CM_OBJ_CMD, "nmmstart",    NULL,   NULL,     "Start NMM function", "USAGE: nmmstart", NULL},
    {CM_OBJ_CMD, "nmmstop",     NULL,   NULL,     "Stop NMM functio", "USAGE: nmmstop", NULL},

    {CM_OBJ_CMD, "phypause",    NULL,   NULL,     "Pause the Phy",  "", NULL},
    {CM_OBJ_CMD, "phyresume",   NULL,   NULL,     "Resume the Phy",  "", NULL},
    {CM_OBJ_CMD, "phylogs",     NULL,   NULL,     "Enable / Disable PhyLogs",  "USAGE: type phylogs without any arguments for usage details", NULL},

    {CM_OBJ_DELIMITER, "",      NULL,   NULL,     "[LTE TestMAC Commands]", NULL},
    {CM_OBJ_CMD, "createmac",   NULL,   NULL,     "Create TestMac instance",                "USAGE: createmac <testcase_id>", NULL},
    {CM_OBJ_CMD, "logplay",     NULL,   NULL,     "Play an API Log that is present in DDR", "USAGE: logplay <StartSF> <SFNtoPlay> <tti:0-PHYstart is used> <DI: 0xFF-default> <1-to log RX sdu> <1-pattern>", NULL},

    {CM_OBJ_CMD, "cmd",         NULL,   CLineCmd, "Issue command", "USAGE: cmd <control> <type> <srcInstID> <dstInstID> <msgID> <paramID> <paramLen> <params> [[<paramID> <paramLen> <params>)] ... ]", NULL},
    {CM_OBJ_CMD, "dilog",       NULL,   CLineDILog, "DI set log mask", "USAGE: dilog [rx|tx|rx0|rx1|tx0|tx1|rxrr|txrr|all|rr|dis|off]", NULL},
#ifdef SYS_TRACER_ENABLED
    {CM_OBJ_CMD, "trace",       NULL,   CLineTrace, "To show the trace of partition/queue", "USAGE: trace <traceID>", NULL},
    {CM_OBJ_CMD, "svsrtrace",   NULL,   CLineSvsrTraceEnable, "To enable svsr trace", "USAGE: svsrtrace <traceID>", NULL},

#endif

#ifdef UNIDIAG_ENABLED
    {CM_OBJ_CMD, "udredirectudp",NULL,  CLineUDRedirectUDP, "redirect UD to udp", "USAGE: udredirectudp <dest ip:a.b.c.d> <dest port> [<dest mac:aabbccddeeff>] ", NULL},
    {CM_OBJ_CMD, "udapiopt",    NULL,   CLineUDAPIDiagConfig, "API diag option", "USAGE: udapiopt <API ud option: 1:tx 2:rx 3:tx/rx > ", NULL},
    {CM_OBJ_CMD, "udenable",    NULL,   CLineUDEnable, "UD enable mask", "USAGE: udenable <stream enable mask: bit 0- API, other bits - reserved>", NULL},

#endif

    {CM_OBJ_CMD, "swdiagstart", NULL,   CLineCmdSWDiagStart, "SWDiag start", "USAGE: swdiagstart", NULL},
    {CM_OBJ_CMD, "swdiagprn",   NULL,   CLineCmdSWDiagPrint, "SWDiag print", "USAGE: swdiagprn", NULL},
    {CM_OBJ_CMD, "swdiagclose", NULL,   CLineCmdSWDiagClose, "SWDiag close", "USAGE: swdiagclose", NULL},
    {CM_OBJ_CMD, "ptb",         NULL,   CLineCmdPtb, "Print Trace Buffer", "USAGE: ptb", NULL},
    {CM_OBJ_CMD, "cevamdma",    NULL,   CLineCmdTestCevaMDMA, "To run-test for CEVA MDMA driver", "USAGE: cevamdma [0/1/2]", NULL},
    {CM_OBJ_CMD, "mmuinfo",     NULL,   CLineCmdMmuInfo,  "To get information about mmu table", "USAGE: mmuinfo", NULL},
    {CM_OBJ_CMD, "stacktest",   NULL,   CLineCmdStackParce,  "To get information about mmu table", "USAGE: mmuinfo", NULL},
    {CM_OBJ_CMD, "cicputest",   NULL,   CLineCmdCICPUTest,  "test conn icpu", "USAGE: cicputest on|off|send text to send ", NULL},
    

    // End of the list
    {CM_OBJ_CMD, NULL,          NULL,   CLineErrCmd, NULL,                 NULL,                         NULL}
};

UINT32 CLineCmdHelp (CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    return CMgrShowTableInfo((CMgrCtx *)a->ConsoleCtx, (CMgrMenuElm*)a->MenuTable);
}

UINT32 CLineCmdClear(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    UINT32 i;
    for (i = 0; i < 128;i++)
    {
        uart_printf ("\r\n");
        MxDelayTicks (US_TO_TICKS(10));
    }

    return 0;
}

UINT32 CLineErrCmd (CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    uart_printf ("Unknown command\r\n");

    return RC_CM_OK;
}

void DumpProc (UINT32 nAddr, UINT32 nSize)
{
    UINT8 * pAddr;
    UINT32  nNum, i, nCol = 0;
    PUINT8 pBeginLine= NULL;
    //MMUPAGE hPage;

    pAddr = (UINT8 *)nAddr;

#if 0

    if (MxGetCpuID () == 3)
    {
        UINT32 a = 0x09310000;

        hPage = MxMmuGetPage (MxMmuGetMasterTable(), a);

        _ASSERT_PTR(hPage);

        uart_printf ("Protect PAGE64: %x\r\n", a);

        MxMmuSetPageParam (hPage, a,
                         MMU_PAGE_AP(AP_NO_ACCESS)|TEXCB_PAGE64_OI_WB_WA_S);
                         //MMU_PAGE_AP(AP_FULL_ACCESS)|TEXCB_PAGE64_OI_WB_WA_S);
    }
#endif

    if (FAILED(MxSafeRead32 ((PUINT32)((UINT32)pAddr & ~3), &nNum)))
    {
        uart_printf ("\r\nAddress error (0x%08x), the memory cannot be read", pAddr);
        return;
    }

    nNum  = nSize;

    for (i = 0; i < nNum; i++, nCol ++)
    {
        if (( i % 16)== 0)
        {
		    uart_printf("\r\n0x%08x:", (UINT32)(pAddr + i));
            nCol = 0;

            pBeginLine = (pAddr + i);
        }

        if ((nCol % 8) == 0 && nCol)
        {
		    uart_printf (" |");
        }

        uart_printf(" %02x", (UINT32)(pAddr[i]));

        if (( (i+1) % 16)== 0)
        {
            uart_printf ("  ");
            while (pBeginLine != (pAddr + i))
            {
                if (*pBeginLine >= 32 && *pBeginLine <= 126)
                    uart_printf ("%c", *pBeginLine);
                else
                    uart_printf (".");
                pBeginLine ++;
            }

            pBeginLine = NULL;
        }
    }

    if (pBeginLine != NULL)
    {
		nCol %= 16;

		if (nCol >= 8)
			nCol = 0;

        while ((i % 16) != 0)
        {
            uart_printf("   ");

			if (nCol)
				nCol++;

			if ((nCol % 8) == 0 && nCol != 0)
			{
                uart_printf(" |");
				nCol = 0;
			}

			i++;
        }

        uart_printf ("  ");

		while (pBeginLine != (pAddr + nNum))
		{
			if (*pBeginLine >= 32 && *pBeginLine <= 126)
                    uart_printf ("%c", *pBeginLine);
                else
                    uart_printf (".");
                pBeginLine ++;
		}
    }
}

extern int _strcmp (const char * pSrc, char * pDst);

UINT32 DumpGetObjAddr (char * pObjName)
{
    if (pObjName == NULL)
        return NULL;

    if (((pObjName[0] == 'd' && pObjName[1]=='l') || (pObjName[0] == 'D' && pObjName[1]=='L')) && pObjName[2]=='-')
    {
        UINT32 id;
        id = StrToNum (pObjName+3);

        return (UINT32)gDlIqBuf + id * 61440;
    }

    if (((pObjName[0] == 'u' && pObjName[1]=='l') || (pObjName[0] == 'U' && pObjName[1]=='L')) && pObjName[2]=='-')
    {
        UINT32 id;
        id = StrToNum (pObjName+3);

        return (UINT32)gUlIqBuf + id * 61440;
    }

    if (!_strcmp(pObjName, "dl") || !_strcmp(pObjName, "DL"))
        return (UINT32)gDlIqBuf;

    if (!_strcmp(pObjName, "ul") || !_strcmp(pObjName, "UL"))
        return (UINT32)gUlIqBuf;

    if (!_strcmp(pObjName, "api") || !_strcmp(pObjName, "API"))
        return (UINT32)gSvsrLoggerBuf;

    if (!_strcmp(pObjName, "mlog") || !_strcmp(pObjName, "MLOG"))
        return (UINT32)gMlogBuf;

    return StrToNum(pObjName);
}

UINT32 CLineDump (CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    UINT32 Addr;
    UINT32 NumBytes = 4;
    UINT32 bCacheInv = 0;

    if (b->ParamCount < 1)
        return RC_CM_PARAM_ERROR;

    Addr = DumpGetObjAddr(b->ParamList [0]);

    if (b->ParamCount > 1)
    {
        NumBytes = StrToNum(b->ParamList [1]);

        if (b->ParamCount > 2)
            bCacheInv = StrToNum(b->ParamList [2]);
    }

    if(bCacheInv)
        MxCacheInvalidate((LPVOID)Addr, NumBytes);

    DumpProc (Addr, NumBytes);

    return RC_CM_OK;
}

UINT32 dl_crc_table [400] = {
  0xdcd9722c, 0x3b221907, 0x2768a4f9, 0x55cd2b51, 0x81037192, 0xd6e3b2a9, 0x839a2572, 0x0ed68190, 0xfed5076b, 0x5ae66e26, 0xaf68e148, 0x3b221907, 0x2768a4f9, 0x55cd2b51, 0x81037192, 0xd6e3b2a9
, 0x839a2572, 0x0ed68190, 0xfed5076b, 0x5ae66e26, 0x541a053d, 0x3b221907, 0x2768a4f9, 0x55cd2b51, 0x81037192, 0xd6e3b2a9, 0x839a2572, 0x0ed68190, 0xfed5076b, 0x5ae66e26, 0x85277188, 0x3b221907
, 0x2768a4f9, 0x55cd2b51, 0x81037192, 0xd6e3b2a9, 0x839a2572, 0x0ed68190, 0xfed5076b, 0x5ae66e26, 0x2e981e3b, 0x3b221907, 0x2768a4f9, 0x55cd2b51, 0x81037192, 0xd6e3b2a9, 0x839a2572, 0x0ed68190
, 0xfed5076b, 0x5ae66e26, 0x9a54d5d2, 0x3b221907, 0x2768a4f9, 0x55cd2b51, 0x81037192, 0xd6e3b2a9, 0x839a2572, 0x0ed68190, 0xfed5076b, 0x5ae66e26, 0x074a77fc, 0x3b221907, 0x2768a4f9, 0x55cd2b51
, 0x81037192, 0xd6e3b2a9, 0x839a2572, 0x0ed68190, 0xfed5076b, 0x5ae66e26, 0x68ddc6f7, 0x3b221907, 0x2768a4f9, 0x55cd2b51, 0x81037192, 0xd6e3b2a9, 0x839a2572, 0x0ed68190, 0xfed5076b, 0x5ae66e26
, 0x8993f189, 0x3b221907, 0x2768a4f9, 0x55cd2b51, 0x81037192, 0xd6e3b2a9, 0x839a2572, 0x0ed68190, 0xfed5076b, 0x5ae66e26, 0x8a0d778b, 0x3b221907, 0x2768a4f9, 0x55cd2b51, 0x81037192, 0xd6e3b2a9
, 0x839a2572, 0x0ed68190, 0xfed5076b, 0x5ae66e26, 0x67c7355d, 0x3b221907, 0x2768a4f9, 0x55cd2b51, 0x81037192, 0xd6e3b2a9, 0x839a2572, 0x0ed68190, 0xfed5076b, 0x5ae66e26, 0xd6e4004a, 0x3b221907
, 0x2768a4f9, 0x55cd2b51, 0x81037192, 0xd6e3b2a9, 0x839a2572, 0x0ed68190, 0xfed5076b, 0x5ae66e26, 0x7772bac5, 0x3b221907, 0x2768a4f9, 0x55cd2b51, 0x81037192, 0xd6e3b2a9, 0x839a2572, 0x0ed68190
, 0xfed5076b, 0x5ae66e26, 0x5e542152, 0x3b221907, 0x2768a4f9, 0x55cd2b51, 0x81037192, 0xd6e3b2a9, 0x839a2572, 0x0ed68190, 0xfed5076b, 0x5ae66e26, 0xf9abc686, 0x3b221907, 0x2768a4f9, 0x55cd2b51
, 0x81037192, 0xd6e3b2a9, 0x839a2572, 0x0ed68190, 0xfed5076b, 0x5ae66e26, 0x1ca344fd, 0x3b221907, 0x2768a4f9, 0x55cd2b51, 0x81037192, 0xd6e3b2a9, 0x839a2572, 0x0ed68190, 0xfed5076b, 0x5ae66e26
, 0x4fc91d49, 0x3b221907, 0x2768a4f9, 0x55cd2b51, 0x81037192, 0xd6e3b2a9, 0x839a2572, 0x0ed68190, 0xfed5076b, 0x5ae66e26, 0x928a64a7, 0x3b221907, 0x2768a4f9, 0x55cd2b51, 0x81037192, 0xd6e3b2a9
, 0x839a2572, 0x0ed68190, 0xfed5076b, 0x5ae66e26, 0x99fc8fd3, 0x3b221907, 0x2768a4f9, 0x55cd2b51, 0x81037192, 0xd6e3b2a9, 0x839a2572, 0x0ed68190, 0xfed5076b, 0x5ae66e26, 0xc859e536, 0x3b221907
, 0x2768a4f9, 0x55cd2b51, 0x81037192, 0xd6e3b2a9, 0x839a2572, 0x0ed68190, 0xfed5076b, 0x5ae66e26, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5
, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5
, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5
, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5
, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5
, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5
, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5
, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5
, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5
, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5
, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5
, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5
, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5, 0xb18c7ca5

};

UINT32 CLineCheckObjCRC (CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    UINT32 nAddr = (UINT32)gDlIqBuf;

    UINT32 i, num = 400;
    UINT32 crc, rem, err=0;

    if (b->ParamCount < 1)
    {
        uart_printf ("Param error:\r\n - dlcrc gen <num> - to generate CRC\r\n - dlcrc <num> - to check the CRC");
        return 0;
    }

    if (!_strcmp (b->ParamList[0], "gen"))
    {
        num = CMgrGetParam(b, 1, num);

        uart_printf ("\r\nUINT32 dl_crc_table [%d] = {\r\n", num);

        for (i = 0; i < num; i++)
        {
            rem = 0;
            crc = MdmaProcCRC (&rem, 0x864CFB, 32, (PUINT8)(nAddr + i * 61440), 61440);

            if (i != 0)
                uart_printf (", 0x%08x", crc);
            else
                uart_printf ("  0x%08x", crc);

            MxDelayTicks(150*100);

            if (((i + 1) % 16) == 0)
            {
                uart_printf ("\r\n");
                MxDelayTicks(150*100);
            }
        }

        uart_printf ("\r\n};\r\n");
    }
    else
    {
        num = StrToNum(b->ParamList [0]);
        num = MIN(sizeof (dl_crc_table) / sizeof (dl_crc_table[0]), num);

        uart_printf ("Checking CRC for %d SF ... ", num);

        for (i = 0; i < num; i++)
        {
            rem = 0;
            crc = MdmaProcCRC (&rem, 0x864CFB, 32, (PUINT8)(nAddr + i * 61440), 61440);

            if (crc != dl_crc_table[i])
            {
                uart_printf ("\r\nSF#%d - crc error [0x%08x] != [0x%08x]", i, crc, dl_crc_table[i]);
                MxDelayTicks(150*100);
                err ++;
            }
        }

        if (err == 0)
            uart_printf ("OK\r\n");
        else
            uart_printf ("\r\nCompleted, %d error(s) are found\r\n", err);
    }

    return RC_CM_OK;
}


UINT32 CLineMemSet (CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    UINT32   addr;
    UINT32   val;
    UINT32   numbytes = sizeof(UINT32);
    MXRC rc = RC_CM_PARAM_ERROR; // Not OK

    if (b->ParamCount < 2)
        return RC_CM_PARAM_ERROR;

    addr = StrToNum(b->ParamList [0]) & ~3;
    val  = StrToNum(b->ParamList [1]);

    if (b->ParamCount > 2)
    {
        numbytes = StrToNum(b->ParamList [2]) & ~3;
        if (numbytes == 0)
        {
            printf("Invalid Number of bytes %d\n", numbytes);
            return RC_CM_PARAM_ERROR;
        }
    }

    while(numbytes)
    {
        printf ("[0x%08x] <= 0x%x(%d)\r\n", addr, val, val);

        rc = MxSafeWrite32 ((PUINT32)addr, val);
        if (rc != MX_OK)
            break;
        addr += sizeof(UINT32);
        numbytes -= sizeof(UINT32);
    }

    if (SUCCESSFUL(rc))
    {
        printf ("Writing OK\r\n");
    }
    else
    {
        printf ("Writing error\r\n");
    }

    return RC_CM_OK;
}

UINT32 CLineMemGet (CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    UINT32   addr;
    MXRC rc, val;

    if (b->ParamCount < 1)
        return RC_CM_PARAM_ERROR;

    addr = StrToNum(b->ParamList [0]) & ~3;

    rc = MxSafeRead32 ((PUINT32)addr, &val);

    if (FAILED(rc))
    {
        printf ("Reading error\r\n");
        return 0;
    }

    printf ("[0x%08x] => 0x%x (%d)\r\n", addr, val, val);

    return RC_CM_OK;
}

VUINT32 gTestVar2 = 0;
VUINT32 * gTestVar = (VUINT32 *)&gTestVar2;

#define TEST_COUNT    0x100000

UINT32 MyThread1 (LPVOID p)
{
    int a;

    uart_printf ("My Thread , cpu=%d\r\n", MxGetCpuID ());

    *gTestVar = 0;

    for (a = 0; a < TEST_COUNT; a++)
    {
        //uart_printf ("Iter# %d\r\n", (*gTestVar));

        (*gTestVar) = (*gTestVar) + 1;

        //MxCacheClean ((LPVOID)gTestVar, sizeof (gTestVar));

        CPU_IDLE();
    }

    uart_printf ("END My Thread, cpu=%d\r\n", MxGetCpuID ());

    return 0;
}

UINT32 CLineCmdCheck (CMgrCmdCtx * a1, CMgrCmdParamCtx * b)
{
   HANDLE hThr;
   MXRC rc = 0;
   int a;
   UINT32 ncpu = 1;

   if (b->ParamCount > 0)
    ncpu = StrToNum (b->ParamList [0]);

   ncpu = MIN(ncpu, SysGetCpuNum () - 1);

   uart_printf ("Test for R-CPU-%d\r\n", ncpu);

   gTestVar = (VUINT32 *)&gTestVar2;
   *gTestVar = 0;
   //MxCacheClean ((LPVOID)gTestVar, sizeof (*gTestVar));

   //MxRaiseInterCpu (0, 1);

   uart_printf ("Begin create exthread (var addr = 0x%08x) \r\n", gTestVar);

   rc = MxCreateThreadEx (ncpu, MyThread1, 0, 0, 0, &hThr);
   _ASSERT_RC(rc);

   printf ("end create exthread \r\n");

   rc = MxScheduleThread (hThr);
   _ASSERT_RC(rc);

   a = -1;

   while ((*gTestVar) != TEST_COUNT)
   {
       //MxCacheInvalidate ((LPVOID)gTestVar, sizeof (gTestVar));

       if (a != *gTestVar)
       {
          a = *gTestVar;
          printf ("Val = %d\r\n", *gTestVar);
       }
   }

   return rc;
}

UINT32 CLineCmdCheck2 (CMgrCmdCtx * a1, CMgrCmdParamCtx * b)
{
#ifdef REX_GLOBAL_PROF
    extern UINT32 AvgTicks;
    extern UINT32 NumStart;
    extern UINT32 NumExec;
    extern UINT32 MaxExec[];
    extern UINT32 MinExec[];
    extern UINT32 AvgExec[];
    extern UINT32 TLTime [];
    extern UINT32 TLHandle [];
    extern UINT32 TLNumTcb [];
    REXPROF * pProf;

    UINT32 val, avg_num=1;

    UINT32 TicksPerList = AvgTicks;

    if (NumStart)
    {
        TicksPerList /= NumStart;
    }

    if (NumExec)
        avg_num = NumExec;

    uart_printf ("(exec=%d)List num=%d, avg=%d, sum=%d\r\n", NumExec, NumStart, TicksPerList, AvgTicks);

    uart_printf (" *MaxLock[0]=%d, MinLock[0]=%d, Avg[0]=%d\r\n", MaxExec[0], MinExec[0], AvgExec[0] / avg_num);
    uart_printf (" *MaxLock[1]=%d, MinLock[1]=%d, Avg[1]=%d\r\n", MaxExec[1], MinExec[1], AvgExec[1] / avg_num);

    for (val = 0; val < NumStart; val++)
    {
        uart_printf (" *[%d] List[%x], tcb_num-%d = %d\r\n", val, TLHandle[val], TLNumTcb[val], TLTime[val]);
        MxDelayTicks (TICKS_TO_MS(20));
    }
    pProf = RexGetProfInfo (REX_PROF_CEVA_RUN);
    uart_printf ("CEVA-PROF-RUN: max=%d avg=%d min=%d num=%d\r\n", pProf->MaxTime, pProf->TotalTime / pProf->TotalNum, pProf->MinTime, pProf->TotalNum);

    pProf = RexGetProfInfo (REX_PROF_CEVA_DONE);
    uart_printf ("CEVA-PROF-DONE: max=%d avg=%d min=%d num=%d\r\n", pProf->MaxTime, pProf->TotalTime / pProf->TotalNum, pProf->MinTime, pProf->TotalNum);

    pProf = RexGetProfInfo (REX_PROF_KERNEL);
    uart_printf ("KERNEL-PROF: max=%d avg=%d min=%d num=%d\r\n", pProf->MaxTime, pProf->TotalTime / pProf->TotalNum, pProf->MinTime, pProf->TotalNum);

    pProf = RexGetProfInfo (REX_PROF_PROC_LIST);
    uart_printf ("PROC-LIST-PROF: max=%d avg=%d min=%d num=%d\r\n", pProf->MaxTime, pProf->TotalTime / pProf->TotalNum, pProf->MinTime, pProf->TotalNum);

    pProf = RexGetProfInfo (REX_PROF_FP_RUN);
    uart_printf ("FP-RUN-PROF: max=%d avg=%d min=%d num=%d\r\n", pProf->MaxTime, pProf->TotalTime / pProf->TotalNum, pProf->MinTime, pProf->TotalNum);

    pProf = RexGetProfInfo (REX_PROF_FP_DONE);
    uart_printf ("FP-DONE-PROF: max=%d avg=%d min=%d num=%d\r\n", pProf->MaxTime, pProf->TotalTime / pProf->TotalNum, pProf->MinTime, pProf->TotalNum);

    pProf = RexGetProfInfo (REX_PROF_EXEC_LIST);
    uart_printf ("EXEC-PROF: max=%d avg=%d min=%d num=%d\r\n", pProf->MaxTime, pProf->TotalTime / pProf->TotalNum, pProf->MinTime, pProf->TotalNum);

    pProf = RexGetProfInfo (REX_PROF_REINIT_LIST);
    uart_printf ("REINIT-PROF: max=%d avg=%d min=%d num=%d\r\n", pProf->MaxTime, pProf->TotalTime / pProf->TotalNum, pProf->MinTime, pProf->TotalNum);

    pProf = RexGetProfInfo (REX_PROF_CALL_CB);
    uart_printf ("CBDONE-PROF: max=%d avg=%d min=%d num=%d\r\n", pProf->MaxTime, pProf->TotalTime / pProf->TotalNum, pProf->MinTime, pProf->TotalNum);

#endif
    return 0;
}

VUINT32 MyLock = 0;
VUINT32 MyLock2 = 0;
VUINT32 nStop = 0;
VUINT32 nCount = 0;
VUINT32 nThrEnabled[CPU_NUM] = {0};
#define SLEEP_NUM 7
UINT32  Sleeps [SLEEP_NUM] = {500, 1030, 2310, 4500, 220, 800, 125};
VUINT32 LockTestRes;

UINT32 MyThread3 (LPVOID p)
{
    VUINT32 i, idx = 2, sleep = 0, subcount = 0, mytstval = 0;
    //IRQTYPE irq;

    uart_printf ("\r\nThread# is started, cpu=%d, count=%d\r\n", MxGetCpuID (), nCount);

    nThrEnabled[MxGetCpuID ()] = 1;

    while (!nStop)
    {
        //irq = ARM_INT_disable ();
        MxGetLock (&MyLock);

        nCount = 0;
        mytstval = 0;

        subcount = 0;

        while (subcount++ < 1000)
        {
            nCount += 3217 * (MxGetCpuID () + 1);
            mytstval += 3217 * (MxGetCpuID () + 1);
        }

        if (mytstval != nCount)
        {
            uart_printf ("\r\n\r\nThe problem is detected, cpu=%d !!!\r\n\r\n", MxGetCpuID ());
            nStop = 1;
            LockTestRes = MxGetCpuID ();
        }

        MxReleaseLock (&MyLock);
        //ARM_INT_restore (irq);
    }

    uart_printf ("\r\nThread# is stopped, cpu=%d, count=%d\r\n", MxGetCpuID (), nCount);

    return nCount;
}

UINT32 CLineCmdCheck3 (CMgrCmdCtx * a1, CMgrCmdParamCtx * b)
{
    UINT32 i = 0;
    volatile int dummy =0;
    UINT32 sleep, idx = 0, tmp;
    //IRQTYPE irq;

    MXRC rc = 0;
    HANDLE hMyThr[4];

    nCount = 0;
    nStop = 0;
    LockTestRes = (UINT32)-1;

    nThrEnabled[0] = 0;
    nThrEnabled[1] = 0;
    nThrEnabled[2] = 0;
    nThrEnabled[3] = 0;

    uart_printf ("Begin test ...\r\n");

    rc = MxCreateThreadEx (1, MyThread3, 0, 0, 0, &hMyThr[1]);
    _ASSERT_RC(rc);

    rc = MxCreateThreadEx (2, MyThread3, 0, 0, 0, &hMyThr[2]);
    _ASSERT_RC(rc);

    rc = MxCreateThreadEx (3, MyThread3, 0, 0, 0, &hMyThr[3]);
    _ASSERT_RC(rc);

    nCount = 1;

    rc = MxScheduleThread (hMyThr[1]);
    rc = MxScheduleThread (hMyThr[2]);
    rc = MxScheduleThread (hMyThr[3]);

    uart_printf ("Waiting ... ");

    while (nThrEnabled[1] == 0 || nThrEnabled[2] == 0 || nThrEnabled[3] == 0);

    uart_printf ("OK\r\n");

    while (i++ < 100000)
    {
        //irq = ARM_INT_disable ();
        MxGetLock (&MyLock);

        tmp = nCount;

        sleep = Sleeps [idx];
        idx = (idx + 1) % SLEEP_NUM;

        dummy=sleep;
        while (dummy--);

        nCount += 255;

        sleep = Sleeps [idx];
        idx = (idx + 1) % SLEEP_NUM;

        dummy=sleep;
        while (dummy--);

        nCount *= 10;

        sleep = Sleeps [idx];
        idx = (idx + 1) % SLEEP_NUM;

        dummy=sleep;
        while (dummy--);

        if (((tmp + 255) * 10) != nCount)
        {
            nStop = 1;
            uart_printf ("FAILED, tmp=%d, count=%d\r\n", tmp, nCount);

            break;
        }
        else
        {
            if ((i % 5000) == 0)
                uart_printf ("OK\r\n");
        }

        MxReleaseLock (&MyLock);
        //ARM_INT_restore (irq);

    }

    nStop = 1;

    i = 0;
    while (i ++ < 200);

    MxRemoveThread (hMyThr[1]);
    MxRemoveThread (hMyThr[2]);
    MxRemoveThread (hMyThr[3]);

    if (LockTestRes != (UINT32)-1)
        uart_printf ("Test is FAILED, cpu-%d\r\n", LockTestRes);
    else
        uart_printf ("Test is PASSED\r\n");

    return rc;
}

#ifdef MX_MTHREAD_ENABLED

UINT32 MyGVar = 0;
UINT32 MyGVar1= 0;
HANDLE hMThr, hMThr1;
HANDLE hSema;
HANDLE hMTimer;

UINT32 Proc(void * p)
{
    //HANDLE h;
    UINT32 a = 1;
    uart_printf ("\r\nproc#0, param=0x%x, cpu=%d\r\n", p, MxGetCpuID());

    while (a)
    {
        MxWaitForSingleObject (hSema, 0);
        MyGVar ++;
        //h = MxGetCurrentMThread();
        //uart_printf ("[proc0], param=%x, val=%d, h=%x\r\n", p, MyGVar, HANDLE_TOINT(h));
    }

    return 0;
}

UINT32 Proc1(void * p)
{
    UINT32 a = 1;
    //HANDLE h;
    uart_printf ("\r\nproc#1, param=0x%x, cpu=%d\r\n", p, MxGetCpuID());

    while (a)
    {
        MxWaitForSingleObject (hSema, 0);
        MyGVar1 ++;
        //h = MxGetCurrentMThread();
        //uart_printf ("[proc1], param=%x, val=%d, h=%x\r\n", p, MyGVar1, HANDLE_TOINT(h));
    }

    return 0;
}

void MThrSemaGenerator (void * p)
{
    MxSetSema (hSema, 1);
}

UINT32 CLineCmdCheck4 (CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    MXRC rc;
    UINT32 dst_cpu=0;

    dst_cpu = CMgrGetParam(b, 0, 0);

    uart_printf ("M-Threads testing for cpu-%d... ", dst_cpu);

    if (HANDLE_NULL(hMThr) && HANDLE_NULL(hMThr1))
    {
        rc = MxCreateSemaEx (dst_cpu, 0, NULL, &hSema);
        uart_printf ("\r\nSema rc=0x%x, h = %x\r\n", rc, HANDLE_TOINT(hSema));
        MxDelay(2);

        rc = MxMCreateThreadEx (dst_cpu, Proc, NULL, (void*)  9,  9, 0, 0, 0, &hMThr);
        uart_printf ("\r\nrc=%x, h = %x\r\n", rc, HANDLE_TOINT(hMThr));
        MxDelay(2);

        rc = MxMCreateThreadEx (dst_cpu, Proc1, NULL, (void*)10, 10, 0, 0, 0, &hMThr1);
        uart_printf ("\r\nrc=%x, h = %x\r\n", rc, HANDLE_TOINT(hMThr1));
        MxDelay(2);

        rc = MxCreateTimer(TIMER_MODE_PERIOD, 1, NULL, HNULL, &hMTimer);
        uart_printf ("Timer rc=%x, h = %x\r\n", rc, HANDLE_TOINT(hMTimer));

        rc = MxSetTimerProc(hMTimer, MThrSemaGenerator, 0);
        uart_printf ("Timer-proc rc=%x, h = %x\r\n", rc, HANDLE_TOINT(hMTimer));

        //MxStartTimer(hMTimer);
        MxStartFracTimer(hMTimer);
    }
    else
    {
        //MxSetSema (hSema, 1);
        uart_printf ("\r\nMyGVar=%d\r\nMyGVar1=%d\r\n", MyGVar, MyGVar1);
    }

    return 0;
}

HANDLE hFracTimer;
HANDLE hTest5Event;
HANDLE hTest5Thread;
UINT32 Test5Num = 0;
UINT32 Test5Begin = 0;

UINT32 ThreadTest5(void * p)
{
    Test5Num ++;

    if (Test5Num >= MX_TIMER_FRACTION * 100)
    {
        Test5Begin = MxGetTicks () - Test5Begin;
        MxStopTimer(hFracTimer);

        MxRemoveEvent (hTest5Event);
        MxRemoveThread(hTest5Thread);
        MxRemoveTimer(hFracTimer);

        uart_printf ("\r\nFraction timer test, num-%d, time ticks = %d (where 1000 is 1ms)\r\n", Test5Num, Test5Begin / 150);
    }

    return 0;
}

UINT32 CLineCmdCheck5 (CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    MXRC rc;

    Test5Num = 0;

    rc = MxCreateEvent (0, NULL, &hTest5Event);
    _ASSERT_RC(rc);

    rc = MxCreateTimer(TIMER_MODE_PERIOD, 1, NULL, hTest5Event, &hFracTimer);
    _ASSERT_RC(rc);

    rc = MxCreateThread(ThreadTest5, NULL, NULL, 0, &hTest5Thread);
    _ASSERT_RC(rc);

    MxScheduleThreadByEvent(hTest5Thread, hTest5Event, THREAD_EVENT_AUTO_RESET);

    Test5Begin = MxGetTicks();
    MxStartFracTimer(hFracTimer);
    //MxStartTimer(hFracTimer);

    return rc;
}


#endif

#define COUNT_NUM 20

TCB my_tcb [COUNT_NUM];
VUINT32 gArmComplete = 0;

void ArmTestTCBFunction(TCB * p)
{
    // Its empty task

    //uart_printf ("TCB handler, cpu is %d, mode is 0x%x\r\n", MxGetCpuID(), MxGetCpuMode ());
}

void ArmTestNotifier(LPVOID pDrvCtx, LPVOID p)
{
    TCB * pTcb;

    //uart_printf ("Notifier: cpu is %d, mode is 0x%x\r\n", MxGetCpuID (), MxGetCpuMode ());

    while ((pTcb = STDGetReadyTcb (pDrvCtx)) != NULL)
    {
         pTcb->ResourceID = (SysTimerGetTick () - pTcb->ResourceID);

         gArmComplete ++;
    }

}

UINT32 CLineCmdArmTest (CMgrCmdCtx * a1, CMgrCmdParamCtx * b)
{
    MXRC rc;
    UINT32 i = 0, nAvrTime;
    UINT32 count = COUNT_NUM;
    //UINT32 nExecTime [] = {10000, 12000, 20000, 400000, 50000, 15000, 200000};

    UINT32 nMaxTime=0, nCurTime = 0;

    STDrvRegNotifier (ArmTestNotifier, (LPVOID)0x123);

    uart_printf ("Allocate table ... ");

    rc = STDrvAllocTable (count, FALSE);

    if (FAILED(rc))
    {
        printf ("Alloc table error\r\n");
        return 0;
    }

    printf ("OK\r\n");

    for (i = 0; i < count; i++)
    {
        rc = STDrvRegARMTask (i, ArmTestTCBFunction);

        if (FAILED(rc))
        {
            printf ("Reg task error\r\n");
            return 0;
        }
    }

    printf ("Reg task OK\r\n");

    gArmComplete = 0;

    for (i = 0; i < count; i++)
    {
        my_tcb[i].Status = 0;
        my_tcb[i].TaskID = i; // TASK ID
        my_tcb[i].ResourceID = SysTimerGetTick ();

        nCurTime = SysTimerGetTick ();

        rc = STDrvRunTask (1, &my_tcb[i]);

        nCurTime = SysTimerGetTick () - nCurTime;

        while (my_tcb[i].Status == 0);

        //nMaxTime = MAX(nMaxTime, nCurTime);
        nMaxTime += nCurTime;

        if (FAILED(rc))
        {
            printf ("Run task (%d) error\r\n", i);
            return 0;
        }

        //printf ("Run task (%d) OK\r\n", i);
    }

    while (gArmComplete != count);

    nAvrTime = 0;

    for (i = 0; i < count; i++)
    {
        nAvrTime += my_tcb [i].ResourceID;
    }

    printf ("AVG SCH TICK IS : %d\r\n", nMaxTime / count);
    printf ("AVG EXEC TICK IS: %d\r\n", nAvrTime / count);

    return 0;
}

UINT32 CLineCmdTicks (CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    uart_printf ("Ticks: %d\r\n", MxGetTime ());

    return 0;
}
UINT32 CLineCmdDelay (CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    UINT32 nTicks = 1000;

    if (b->ParamCount>0)
        nTicks = StrToNum (b->ParamList[0]);

    uart_printf ("Begin wait (%d) ... ", nTicks);

    MxDelay (nTicks);

    uart_printf ("End \r\n");

    return 0;
}

UINT32 CLineCmdCheckSys (CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    MXRC rc;
    UINT32 nParam = 1;

    uart_printf ("To check the system resources\r\n");

#if 0
    uart_printf ("To make memory leak ... ");

    MxHeapAlloc (AppHeap,  100);
    MxHeapAlloc (NcnbHeap, 200);

    uart_printf ("OK\r\n");
#endif

    if (b->ParamCount != 0)
    {
        nParam = StrToNum (b->ParamList[0]);

        uart_printf ("With parameter:%d\r\n", nParam);
    }

    rc = SvsrCheckSysObjs (nParam, NULL);

    if (FAILED(rc))
    {
        uart_printf ("Problems are detected, rc = %08x\r\n", rc);
    }
    else
    {
        uart_printf ("The system is OK\r\n");
    }

    return 0;
}

#if defined(_MX_DEBUG) && defined(MX_CREATE_OBJ_LOG_ENABLED)

extern UINT32 CreateObjectIndex;
extern UINT32 CreateObjectLogA [];
extern UINT32 CreateObjectLogH [];

UINT32 CLineCmdSysObj (CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    UINT32 i = 0;

    while (CreateObjectLogH [i] != 0)
    {
        uart_printf ("C-LOG: addr=0x%08x, h = 0x%08x\r\n", CreateObjectLogA [i], CreateObjectLogH [i]);

        i++;
    }

    return 0;
}

#endif

UINT32 CLineCmdDumpMask (CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    UINT32 nParam = 0;

    if (b->ParamCount < 1)
        return RC_CM_PARAM_ERROR;

    nParam = StrToNum (b->ParamList[0]);

    return SvsrSetDumpMask (nParam);
}

UINT32 CLineCmdCheckDCache (CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    UINT32 nAddr;

    if (b->ParamCount < 1)
        return RC_CM_PARAM_ERROR;

    nAddr = StrToNum (b->ParamList[0]);

    if (SysCoreIsDCached (nAddr))
    {
        printf ("[0x%08x .. 0x%08x) - DCACHE IS ENABLED\r\n", (nAddr & ~0xFFFFF), (nAddr + 0xFFFFF) & ~0xFFFFF);
    }
    else
    {
        printf ("[0x%08x .. 0x%08x) - DCACHE IS DISABLED\r\n", (nAddr & ~0xFFFFF), (nAddr + 0xFFFFF) & ~0xFFFFF);
    }

    return 0;
}

UINT32 CLineCmdDbg (CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    LPVOID pSrc, pDst;
    UINT32 n, size = 16*1024;
    IRQTYPE irq;

    size = CMgrGetParam (b, 0, 16*1024);

    pSrc = MxHeapAlloc (NcnbHeap, size);

    if (pSrc == NULL)
    {
        printf ("SRC allocation error\r\n");
        return 0;
    }

    pDst = MxHeapAlloc (NcnbHeap, size);

    if (pDst == NULL)
    {
        MxHeapFree (NcnbHeap, pSrc);

        printf ("DST allocation error\r\n");
        return 0;
    }

    uart_printf ("DDR (%x,  %x) \r\n", pSrc, pDst);
    uart_printf ("memset ...");

    irq = ARM_INT_disable ();

    n = SysTimerGetTick ();

    memset (pSrc, 1, size);

    n = SysTimerGetTick () - n;

    ARM_INT_restore (irq);

    uart_printf ("%d\r\n", n);


    uart_printf ("memcpy ...");

    irq = ARM_INT_disable ();

    n = SysTimerGetTick ();

    memcpy (pDst, pSrc, size);

    n = SysTimerGetTick () - n;

    ARM_INT_restore (irq);

    uart_printf ("%d\r\n", n);


    uart_printf ("_memcpy ...");

    irq = ARM_INT_disable ();

    n = SysTimerGetTick ();

    _memcpy (pDst, pSrc, size);

    n = SysTimerGetTick () - n;

    ARM_INT_restore (irq);

    uart_printf ("%d\r\n", n);

    MxHeapFree (NcnbHeap, pSrc);
    MxHeapFree (NcnbHeap, pDst);



    // CRAM

    pSrc = MxHeapAlloc (DspHeap, size);

    if (pSrc == NULL)
    {
        printf ("SRC allocation error\r\n");
        return 0;
    }

    pDst = MxHeapAlloc (DspHeap, size);

    if (pDst == NULL)
    {
        MxHeapFree (DspHeap, pSrc);

        printf ("DST allocation error\r\n");
        return 0;
    }

    uart_printf ("CRAM (%x,  %x) \r\n", pSrc, pDst);
    uart_printf ("memset ...");

    irq = ARM_INT_disable ();

    n = SysTimerGetTick ();

    memset (pSrc, 1, size);

    n = SysTimerGetTick () - n;

    ARM_INT_restore (irq);

    uart_printf ("%d\r\n", n);


    uart_printf ("memcpy ...");

    irq = ARM_INT_disable ();

    n = SysTimerGetTick ();

    memcpy (pDst, pSrc, size);

    n = SysTimerGetTick () - n;

    ARM_INT_restore (irq);

    uart_printf ("%d\r\n", n);


    uart_printf ("_memcpy ...");

    irq = ARM_INT_disable ();

    n = SysTimerGetTick ();

    _memcpy (pDst, pSrc, size);

    n = SysTimerGetTick () - n;

    ARM_INT_restore (irq);

    uart_printf ("%d\r\n", n);

    MxHeapFree (DspHeap, pSrc);
    MxHeapFree (DspHeap, pDst);

    return 0;
}

UINT32 CLineCmdIram (CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{

    PUINT8 p;
    UINT32 nSize = 1024, i = 0;

    if (HANDLE_NULL(IramHeap))
    {
        printf ("The IRAM heap is not initialized\r\n");
        return 0;
    }

    if (b->ParamCount != 0)
    {
        nSize = StrToNum (b->ParamList [i]);
    }

    p = (PUINT8)MxHeapAlloc (IramHeap, nSize);

    if (p == NULL)
    {
        printf ("IRAM allocation error, the pointer is NULL");
        return 0;
    }

    printf ("Alloc - OK\r\n");

    printf ("Start setting ... ");

    for (i = 0; i < nSize; i++)
    {
        p [i] = (i & 0xFF);
    }

    printf ("OK\r\n");

    printf ("Begin checking ... ");

    for (i = 0; i < nSize; i++)
    {
        if (p [i] != (i & 0xFF))
        {
            printf ("Error, offs = %d, exp: %d, taken: %d\r\n", i, i & 0xFF, p [i]);

            MxHeapFree (IramHeap, p);

            return 0;
        }
    }

    printf ("OK\r\n");

    MxHeapFree (IramHeap, p);

    return 0;
}

UINT32 CLineCmdFill (CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    UINT32 nAddr, nSize, nData;
    PUINT8 p;

    if (b->ParamCount != 3)
        return RC_CM_PARAM_ERROR;

    nAddr = StrToNum (b->ParamList [0]);
    nSize = StrToNum (b->ParamList [1]);
    nData = StrToNum (b->ParamList [2]);

    p = (PUINT8)nAddr;

    printf ("filling ... ");


	if(p)
	    while (nSize --)
    	{
        	*p++ = (UINT8)(nData & 0xFF);
	    }

    printf ("OK\r\n");

    return 0;
}


HANDLE hTestApiThr = {0};
VUINT32 nFinish = 0;
UINT32  gApiNumSend;
UINT32  gApiAllocSum, gApiAllocMax, gApiAllocMin;
UINT32  gApiSendSum, gApiSendMax, gApiSendMin;
UINT32  gApiCopySum, gApiCopyMax, gApiCopyMin;

#define SVSR_ARM1_NEW_API

UINT32 TestApiThread (LPVOID p )
{
    ApiHeader * pApi;
    MXRC rc = MX_OK;
    UINT32 t, irq, i;

    gApiAllocSum = 0;
    gApiAllocMax = 0;
    gApiAllocMin = ~0;

    gApiSendSum = 0;
    gApiSendMax = 0;
    gApiSendMin = ~0;

    gApiCopySum = 0;
    gApiCopyMax = 0;
    gApiCopyMin = ~0;

    uart_printf ("(ARM-%d) Sending ... ", MxGetCpuID ());

    irq = ARM_INT_disable();

    for (i = 0; i < gApiNumSend; i++)
    {
        t = MxGetTicks();

        #ifdef SVSR_ARM1_NEW_API
        pApi = SvsrAllocICApi ();
        #else
        pApi = (ApiHeader *)malloc (1024);
        #endif

        t = MxGetTicks() - t;

        gApiAllocSum += t;
        gApiAllocMax = MAX(gApiAllocMax, t);
        gApiAllocMin = MIN(gApiAllocMin, t);

        if (pApi == NULL)
        {
            uart_printf ("Allocation error ... \r\n");
            nFinish = 1;
            break;
        }

        pApi = ApiCreate(pApi,
                         API_DEF_FLAGS,
                         MAKE_LARM_INST(SVSR_INSTANCE_ID),
                         UARM_INSTID_HOST,
                         API_TYPE_IND,
                         SVSR_READY);

        t = MxGetTicks();
        ApiAddParam(pApi, NULL, 2, &gApiAllocSum, 1536);
        t = MxGetTicks() - t;
        gApiCopySum += t;
        gApiCopyMax = MAX(gApiCopyMax, t);
        gApiCopyMin = MIN(gApiCopyMin, t);

        t = MxGetTicks ();

        #ifdef SVSR_ARM1_NEW_API
        rc = SvsrSendICApi (pApi);
        #else
        rc = SvsrSendAPI(pApi);
        free (pApi);
        #endif

        t = MxGetTicks() - t;
        gApiSendSum += t;
        gApiSendMax = MAX(gApiSendMax, t);
        gApiSendMin = MIN(gApiSendMin, t);

        if (FAILED(rc))
        {
            uart_printf ("Sending error\r\n");
            nFinish = 1;
            break;
        }
    }

    ARM_INT_restore(irq);

    if (nFinish == 0)
    {
        gApiCopySum /= i;
        gApiAllocSum /= i;
        gApiSendSum /= i;

        uart_printf ("TOTAL: avg: %d, min: %d, max: %d\r\n",

                gApiCopySum + gApiAllocSum + gApiSendSum,
                gApiSendMin + gApiAllocMin + gApiCopyMin,
                gApiSendMax + gApiAllocMax + gApiCopyMax
        );

        uart_printf ("ALLOC: avg: %d, min: %d, max: %d\r\n",
                gApiAllocSum,
                gApiAllocMin,
                gApiAllocMax
        );

        uart_printf ("COPY: avg: %d, min: %d, max: %d\r\n",
                gApiCopySum,
                gApiCopyMin,
                gApiCopyMax
        );

        uart_printf ("SEND: avg: %d, min: %d, max: %d\r\n",
                gApiSendSum,
                gApiSendMin,
                gApiSendMax
        );

        uart_printf ("OK\r\n");
        nFinish = 2;
    }

    return 0;
}

UINT32 CLineCmdCheckApi (CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    UINT32 nDst = 0;
    HANDLE h;
    MXRC rc = MX_OK;

    nDst = CMgrGetParam(b, 0, MxGetCpuID());
    gApiNumSend = CMgrGetParam(b, 1, 1);


    if (nDst == 0)
    {
        TestApiThread (NULL);
    }
    else
    {
        rc = MxCreateThreadEx (nDst, TestApiThread, NULL, NULL, 0, &h);

        if (FAILED(rc))
        {
            uart_printf ("Create thread error \r\n");
            return 0;
        }

        nFinish = 0;

        rc = MxScheduleThread (h);

        if (FAILED(rc))
        {
            uart_printf ("Schedule thread error\r\n");
            return 0;
        }

        while (nFinish == 0)
        {
            CPU_IDLE();
        }

        if (nFinish != 2)
        {
            uart_printf ("test finished with error\r\n");
        }
        else
        {
            uart_printf ("test finished OK\r\n");
        }

        MxRemoveThread (h);
    }

    return 0;
}

UINT32 CLineCmdCheckHeap (CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
#ifdef MX_HEAP_INTEGRITY

    MXRC   rc;
    LPVOID pBlock = NULL;
    LPVOID pData [10];
    UINT32 nSizes [10] = {22, 47, 110, 220, 400, 80, 321, 442, 500, 43};
    SINT32 i;

    uart_printf ("Begin checking integrity (AppHeap), alloc ...\r\n");

    for (i = 0; i < 10; i++)
    {
        // it should allocate something like 64
        pData[i] = MxHeapAlloc (AppHeap, nSizes[i]);

        //memset (pData, 'A', 65);

        uart_printf ("Data = %x, size=%d\r\n", pData[i], MxHeapGetBlockSize (AppHeap, pData[i]));

        rc = MxHeapCheckIntegrity (AppHeap, &pBlock);

        if (FAILED(rc))
        {
            uart_printf ("The problem is detected, rc=%x, block=%x\r\n", rc, pBlock);
        }
    }

    if (i >= 10)
        uart_printf ("OK\r\n");


    uart_printf ("Begin checking integrity (AppHeap), free ... ");

    for (i = 9; i >= 0; i--)
    {
        rc = MxHeapFree (AppHeap, pData[i]);

        if (FAILED(rc))
        {
            uart_printf ("Free block error, id=%d, addr=%x\r\n", i, pData[i]);
        }

        rc = MxHeapCheckIntegrity (AppHeap, &pBlock);

        if (FAILED(rc))
        {
            uart_printf ("The problem is detected, rc=%x, block=%x\r\n", rc, pBlock);
        }
    }

    if (i < 0)
        uart_printf ("OK\r\n");

#else
    uart_printf ("Integrity is not supported in this build\r\n");
#endif

    return 0;
}

UINT32 CLineTestMemCpy(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    volatile unsigned int t = 0;
    UINT32 *pSrc = NULL;
    UINT32 *pDst = NULL;
    HANDLE hCachHeap  = AppHeap;
    HANDLE hNCachHeap = NcnbHeap;

    IRQTYPE r = ARM_INT_disable();

    UINT32 i, j;
    #define BUFF_SZ_MAX 20
    #define C_T_C
    //#define C_T_NC
    //#define NC_T_C
    //#define NC_T_NC

    if (CMgrGetParam(b, 0, 0) != 0)
    {
        hCachHeap = PRV_CORE_HEAP;
        uart_printf ("PRV_HEAP IS USED\r\n");
    }

#ifdef C_T_C
    uart_printf("Op1: Cacheable DDR0 to  Cacheable DDR0 memcpy\r\n");

    for ( i = 7; i < BUFF_SZ_MAX ; i++)
    {
        pSrc = MxHeapAlloc(hCachHeap, 1 << i);
        pDst = MxHeapAlloc(hCachHeap, 1 << i);
        if (pSrc == NULL || pDst == NULL)
        {
            ARM_INT_restore(r);
            uart_printf("Allocation error\r\n");
            return 0;
        }

        MxCacheInvalidate(pDst, 1 << i);
        MxCacheInvalidate(pSrc, 1 << i);

        if ((1 << i) >= 40*3*4 && 0)
        {
            t = MxGetTicks();

            for (j = 0; j < 40; j++)
            {
                pSrc [j * 2] += pDst [j * 3];
            }

            t = MxGetTicks() - t;

            uart_printf (" >>>>  t = %d ticksr\n", t);
        }

        MxCacheInvalidate(pDst, 1 << i);
        MxCacheInvalidate(pSrc, 1 << i);

        memset(pDst, 0, 1 << i);
        memset(pSrc, 0, 1 << i);

        t = MxGetTicks();
        memcpy(pDst, pSrc, 1 << i);
        uart_printf("size, %6d, [bytes] time, %6d, [ticks]\r\n", 1 << i, (MxGetTicks() - t));
        MxHeapFree(hCachHeap, pSrc);
        MxHeapFree(hCachHeap, pDst);
    }

#endif

#ifdef C_T_NC
    uart_printf("Op2: Cacheable DDR0 to Non-Cacheable DDR0 memcpy\r\n");

    for ( i = 7; i < BUFF_SZ_MAX; i++)
    {
        pSrc = MxHeapAlloc(hCachHeap, 1 << i);
        pDst = MxHeapAlloc(hNCachHeap, 1 << i);
        if (pSrc == NULL || pDst == NULL)
        {
            ARM_INT_restore(r);
            uart_printf("Allocation error\r\n");
            return 0;
        }
        // refresh cache
        memset(pSrc, 0, 1 << i);

        t = MxGetTicks();
        memcpy(pDst, pSrc, 1 << i);
        uart_printf("size, %6d, [bytes] time, %6d, [ticks]\r\n", 1 << i, (MxGetTicks() - t));
        MxHeapFree(hCachHeap, pSrc);
        MxHeapFree(hNCachHeap, pDst);
    }
#endif

#ifdef NC_T_C
    uart_printf("Op3:  Non-Cacheable DDR0 to Cacheable DDR0 memcpy\r\n");

    for ( i = 7; i < BUFF_SZ_MAX; i++)
    {
        pSrc = MxHeapAlloc(hNCachHeap, 1 << i);
        pDst = MxHeapAlloc(hCachHeap, 1 << i);
        if (pSrc == NULL || pDst == NULL)
        {
            ARM_INT_restore(r);
            uart_printf("Allocation error\r\n");
            return 0;
        }

        // refresh cache
        memset(pDst, 0, 1 << i);

        t = MxGetTicks();
        memcpy(pDst, pSrc, 1 << i);
        uart_printf("size, %6d, [bytes] time, %6d, [ticks]\r\n", 1 << i, (MxGetTicks() - t));
        MxHeapFree(hNCachHeap, pSrc);
        MxHeapFree(hCachHeap, pDst);
    }
#endif

#ifdef NC_T_NC
    uart_printf("Op4:  Non-Cacheable DDR0 to Non-Cacheable DDR0 memcpy\r\n");

    for ( i = 7; i < BUFF_SZ_MAX; i++)
    {
        pSrc = MxHeapAlloc(hNCachHeap, 1 << i);
        pDst = MxHeapAlloc(hNCachHeap, 1 << i);
        if (pSrc == NULL || pDst == NULL)
        {
            ARM_INT_restore(r);
            uart_printf("Allocation error\r\n");
            return 0;
        }

        t = MxGetTicks();
        memcpy(pDst, pSrc, 1 << i);
        uart_printf("size, %6d, [bytes] time, %6d, [ticks]\r\n", 1 << i, (MxGetTicks() - t));
        MxHeapFree(hNCachHeap, pSrc);
        MxHeapFree(hNCachHeap, pDst);
    }
#endif

#ifdef C_T_C
    uart_printf("Op5: Cacheable DDR0 to  Cacheable DDR0 _memcpy\r\n");

    for ( i = 7; i < BUFF_SZ_MAX ; i++)
    {
        pSrc = MxHeapAlloc(hCachHeap, 1 << i);
        pDst = MxHeapAlloc(hCachHeap, 1 << i);
        if (pSrc == NULL || pDst == NULL)
        {
            ARM_INT_restore(r);
            uart_printf("Allocation error\r\n");
            return 0;
        }

        memset(pDst, 0, 1 << i);
        memset(pSrc, 1, 1 << i);

        uart_printf("SRC: 0x%x (%d)\r\n", pSrc, 1 << i);

        t = MxGetTicks();
        _memcpy(pDst, pSrc, 1 << i);
        uart_printf("size, %6d, [bytes] time, %6d, [ticks]\r\n", 1 << i, (MxGetTicks() - t));
        MxHeapFree(hCachHeap, pSrc);
        MxHeapFree(hCachHeap, pDst);
    }
#endif

#ifdef C_T_NC
    uart_printf("Op6: Cacheable DDR0 to Non-Cacheable DDR0 _memcpy\r\n");

    for ( i = 7; i < BUFF_SZ_MAX; i++)
    {
        pSrc = MxHeapAlloc(hCachHeap, 1 << i);
        pDst = MxHeapAlloc(hNCachHeap, 1 << i);
        if (pSrc == NULL || pDst == NULL)
        {
            ARM_INT_restore(r);
            uart_printf("Allocation error\r\n");
            return 0;
        }

        memset(pSrc, 1, 1 << i);

        t = MxGetTicks();
        _memcpy(pDst, pSrc, 1 << i);
        uart_printf("size, %6d, [bytes] time, %6d, [ticks]\r\n", 1 << i, (MxGetTicks() - t));
        MxHeapFree(hCachHeap, pSrc);
        MxHeapFree(hNCachHeap, pDst);
    }
#endif

#ifdef NC_T_C
    uart_printf("Op7:  Non-Cacheable DDR0 to Cacheable DDR0 _memcpy\r\n");

    for ( i = 7; i < BUFF_SZ_MAX; i++)
    {
        pSrc = MxHeapAlloc(hNCachHeap, 1 << i);
        pDst = MxHeapAlloc(hCachHeap, 1 << i);
        if (pSrc == NULL || pDst == NULL)
        {
            ARM_INT_restore(r);
            uart_printf("Allocation error\r\n");
            return 0;
        }

        memset(pDst, 0, 1 << i);

        t = MxGetTicks();
        _memcpy(pDst, pSrc, 1 << i);
        uart_printf("size, %6d, [bytes] time, %6d, [ticks]\r\n", 1 << i, (MxGetTicks() - t));
        MxHeapFree(hNCachHeap, pSrc);
        MxHeapFree(hCachHeap, pDst);
    }
#endif

#ifdef NC_T_NC
    uart_printf("Op8:  Non-Cacheable DDR0 to Non-Cacheable DDR0 _memcpy\r\n");

    for ( i = 7; i < BUFF_SZ_MAX; i++)
    {
        pSrc = MxHeapAlloc(hNCachHeap, 1 << i);
        pDst = MxHeapAlloc(hNCachHeap, 1 << i);
        if (pSrc == NULL || pDst == NULL)
        {
            ARM_INT_restore(r);
            uart_printf("Allocation error\r\n");
            return 0;
        }

        t = MxGetTicks();
        _memcpy(pDst, pSrc, 1 << i);
        uart_printf("size, %6d, [bytes] time, %6d, [ticks]\r\n", 1 << i, (MxGetTicks() - t));
        MxHeapFree(hNCachHeap, pSrc);
        MxHeapFree(hNCachHeap, pDst);
    }
#endif

    ARM_INT_restore(r);

    return 0;
}
UINT32 CLineTestIramAlloc(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    unsigned int* p_block1;
    unsigned int* p_block2;
    unsigned int* p_block3;
    unsigned int* p_next;
    unsigned int counter = 0;
    unsigned int wrong = 0;
    unsigned int heap_size = 64*1024-32;
    HANDLE Heaps = IramHeap;

    printf ("starting IRAM memory alloc...\n");
    printf ("allocating and filling the arrays in the memory...\n");
    printf ("Base IRAM heap descriptor: 0x%X\n", *((unsigned int*)(&Heaps)));

    p_block1 = (unsigned int*)MxHeapAlloc(Heaps, heap_size);
    p_next = p_block1;
    printf("Block 1 allocated. Address: 0x%X\n", (unsigned int)p_block1);
    while((unsigned int)p_next < ((unsigned int)p_block1 + heap_size))
    {
        if (!(counter % 64)) {printf ("written value %d to address 0x%X\n", counter, (unsigned int)p_next); MxDelay(100);}
        *p_next = counter;
        counter += 1;
        p_next += 1;
    }
//    p_block1 = (unsigned int*)((unsigned int)p_block1 - 64*1024);
    printf("Block 1 filled.\n");

    p_block2 = (unsigned int*)MxHeapAlloc(Heaps, heap_size);
    p_next = p_block2;
    printf("Block 2 allocated. Address: 0x%X\n", (unsigned int)p_block2);
    while((unsigned int)p_next < ((unsigned int)p_block2 + heap_size))
    {
        if (!(counter % 64)) {printf ("written value %d to address 0x%X\n", counter, (unsigned int)p_next); MxDelay(100);}
        *p_next = counter;
        counter += 1;
        p_next += 1;
    }
//    p_block2 = (unsigned int*)((unsigned int)p_block2 - 64*1024);
    printf("Block 2 filled.\n");

    p_block3 = (unsigned int*)MxHeapAlloc(Heaps, heap_size);
    p_next = p_block3;
    printf("Block 3 allocated. Address: 0x%X\n", (unsigned int)p_block3);
    while((unsigned int)p_next < ((unsigned int)p_block3 + heap_size))
    {
        if (!(counter % 64)) {printf ("written value %d to address 0x%X\n", counter, (unsigned int)p_next); MxDelay(100);}
        *p_next = counter;
        counter += 1;
        p_next += 1;
    }
//    p_block3 = (unsigned int*)((unsigned int)p_block3 - 64*1024);
    printf("Block 3 filled.\n");

    printf ("checking results...\n");

    counter = 0;

    p_next = p_block1;
    while((unsigned int)p_next < ((unsigned int)p_block1 + heap_size))
    {
        if (!(counter % 64)) {printf ("read value %d from address 0x%X\n", *p_next, (unsigned int)p_next); MxDelay(100);}
        if (*p_next != counter)
            {wrong = 1;}
        counter += 1;
        p_next += 1;
    }
    if (wrong != 0)
        {printf("wrong result in block1\n"); wrong = 0;}
    printf("Block 1 checked.\n");
    MxDelay(100);

    p_next = p_block2;
    while((unsigned int)p_next < ((unsigned int)p_block2 + heap_size))
    {
        if (!(counter % 64)) {printf ("read value %d from address 0x%X\n", *p_next, (unsigned int)p_next); MxDelay(100);}
        if (*p_next != counter)
            {wrong = 1;}
        counter += 1;
        p_next += 1;
    }
    if (wrong != 0)
        {printf("wrong result in block2\n"); wrong = 0;}
    printf("Block 2 checked.\n");
    MxDelay(100);

    p_next = p_block3;
    while((unsigned int)p_next < ((unsigned int)p_block3 + heap_size))
    {
        if (!(counter % 64)) {printf ("read value %d from address 0x%X\n", *p_next, (unsigned int)p_next); MxDelay(100);}
        if (*p_next != counter)
            {wrong = 1;}
        counter += 1;
        p_next += 1;
    }
    if (wrong != 0)
        {printf("wrong result in block2\n"); wrong = 0;}
    printf("Block 3 checked.\n");
    MxDelay(100);

    MxHeapFree(Heaps, p_block1);
    MxHeapFree(Heaps, p_block2);
    MxHeapFree(Heaps, p_block3);

    printf("IRAM test completed.\n");

    return 0;
}

UINT32 CLineCmdDrvRealloc (CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    SYSCORECFG cfg;
    MXRC rc = MX_OK;
    SCHEDSTRC sched;

    /*cfg.CpuDrivers [0] = SYSCORE_FEC_DL_DRV   | SYSCORE_FEC_UL_DRV;
    cfg.CpuDrivers [1] = SYSCORE_SYS_MDMA_DRV | SYSCORE_SPU_MDMA_DRV | SYSCORE_CEVA_DRV | SYSCORE_FP_DRV | SYSCORE_SCHEDULER;*/

    cfg.CpuDrivers [0] = SYSCORE_FEC_DL_DRV | SYSCORE_FEC_UL_DRV | SYSCORE_SYS_MDMA_DRV | SYSCORE_SPU_MDMA_DRV | SYSCORE_CEVA_DRV | SYSCORE_FP_DRV | SYSCORE_SCHEDULER;
    cfg.CpuDrivers [1] = 0;

    cfg.FpMode = FPMODE_FFT1024;
    cfg.PtrSchedCfg = &sched;

    sched.FECMode = 0;
    sched.CEVAPool= 8;
    sched.FPPool= 1;
    sched.SerialExe= 0;
    sched.DebugLevel = TRX_DBG_NODATA;
    sched.SchedPolicy = TRX_POLICY_FALI;
    sched.BlockingAPI = 1;

    {
       UINT32 i;
       for (i = 0; i <sched.CEVAPool; i++)
            sched.CEVAIDS[i]=i+1; //set ID of cevas to be used, the rest are unused
    }

    rc = SysCoreConfig (&cfg);

    printf ("finish (error = 0x%08x)\r\n", rc);

    return 0;
}

VUINT32 InjStatus = 0;
VUINT32 InjBgStatus = 0;
VUINT32 InjTime = 0;
VUINT32 InjApiTime = 0;

UINT32 InjThrCpu0 (LPVOID ptr)
{
    InjTime = GetTIMETICK () - InjTime;

    uart_printf ("(cpu-%d)Injected threadA, cpu_mode=%x, ptr=%x\r\n", MxGetCpuID (), MxGetCpuMode (), (UINT32)ptr);

    InjStatus = 1;

    return 0;
}

UINT32 InjThrCpu1 (LPVOID ptr)
{
    InjTime = GetTIMETICK () - InjTime;

    uart_printf ("(cpu-%d)Injected threadB, cpu_mode=%x, ptr=%x\r\n", MxGetCpuID (), MxGetCpuMode (), (UINT32)ptr);

    InjStatus = 1;

    return 0;
}

UINT32 InjThrCpuBg (LPVOID ptr)
{
    uart_printf ("(cpu-%d)BG thread, cpu_mode=%x, ptr=%x\r\n", MxGetCpuID (), MxGetCpuMode (), (UINT32)ptr);

    uart_printf ("BG thread, waiting for injecting ... ");

    InjBgStatus = 1;

    while (InjStatus == 0);

    uart_printf ("OK\r\n");

    InjBgStatus = 2;

    return 0;
}

UINT32 CLineCmdInjectThr (CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    HANDLE h, hbg;
    MXRC rc;

    uart_printf ("Create local thread ... ");

    rc = MxCreateThread (InjThrCpu0, NULL, (LPVOID)0x123, 0, &h);

    if (FAILED(rc))
    {
        uart_printf ("FAILED, rc=%x\n", rc);
        return 0;
    }

    uart_printf ("OK\n");
    uart_printf ("Inject to cpu-1 ... \n");

    InjStatus = 0;
    InjApiTime = GetTIMETICK ();
    InjTime = GetTIMETICK ();

    rc = MxInjectThread (1, h);

    InjApiTime = GetTIMETICK () - InjApiTime;

    if (FAILED(rc))
    {
        uart_printf ("FAILED, rc=%x\n", rc);

        MxRemoveThread (h);
        return 0;
    }

    while (InjStatus == 0);

    uart_printf ("OK, inj ticks=%d, inj api=%d\n", InjTime, InjApiTime);

    MxRemoveThread (h);

    // Test remotely created thread

    uart_printf ("Create remotely thread ... ");

    rc = MxCreateThreadEx (1, InjThrCpu1, NULL, (LPVOID)0x321, 0, &h);

    if (FAILED(rc))
    {
        uart_printf ("FAILED, rc=%x\n", rc);
        return 0;
    }

    uart_printf ("OK\n");
    uart_printf ("Inject to cpu-1 ... \n");

    InjStatus = 0;
    InjTime = GetTIMETICK ();

    rc = MxInjectThread (1, h);

    if (FAILED(rc))
    {
        uart_printf ("FAILED, rc=%x\n", rc);

        MxRemoveThread (h);
        return 0;
    }

    while (InjStatus == 0);

    uart_printf ("OK, inj ticks=%d\n", InjTime);

    MxRemoveThread (h);

    // ========================================
    // To test injection with backgroung thread
    // ========================================

    InjStatus = 0;

    uart_printf ("Create bg thread ... ");

    rc = MxCreateThreadEx (1, InjThrCpuBg, NULL, (LPVOID)0x111, 0, &hbg);

    if (FAILED(rc))
    {
        uart_printf ("FAILED, rc=%x\n", rc);
        return 0;
    }

    uart_printf ("OK\n");

    InjBgStatus = 0;

    uart_printf ("Run bg thread  ... \n");

    rc = MxScheduleThread (hbg);

    if (FAILED(rc))
    {
        uart_printf ("Run BG thread error, rc=%x\n", rc);
        MxRemoveThread (hbg);

        return rc;
    }

    while (InjBgStatus == 0);

    uart_printf ("OK(run bg)\r\n");

    uart_printf ("Create inj-thread ... ");

    rc = MxCreateThread (InjThrCpu0, NULL, (LPVOID)0x222, 0, &h);

    if (FAILED(rc))
    {
        uart_printf ("FAILED, rc=%x\n", rc);
        return 0;
    }

    uart_printf ("OK\n");
    uart_printf ("Inject to cpu-1 ... \n");

    InjStatus = 0;
    InjTime = GetTIMETICK ();

    rc = MxInjectThread (1, h);

    if (FAILED(rc))
    {
        uart_printf ("FAILED, rc=%x\n", rc);

        MxRemoveThread (h);
        return 0;
    }

    while (InjBgStatus != 2);

    uart_printf ("Test passed, inj ticks=%d\n", InjTime);

    MxRemoveThread (hbg);
    MxRemoveThread (h);

    return 0;
}

MXRC DspDrvDump (UINT32 nCevaID, PUINT8 pOutBuf, PUINT32 pnSize);

UINT32 CLineCmdDspDump(CMgrCmdCtx * pCtx, CMgrCmdParamCtx * pParams)
{
    PUINT8 pDspDump;
    UINT32 dspdump_size = 128 * 1024 + HW_TCB_SIZE;
    UINT32 dspId = 1;

    if(pParams->ParamCount)
    {
        dspId = StrToNum (pParams->ParamList[0]);
        if(pParams->ParamCount > 1)
            dspdump_size = StrToNum (pParams->ParamList[1]);
    }

    pDspDump = (unsigned char*)MxHeapAlloc (NcnbHeap, dspdump_size);
    printf (" Dsp Dump start address: 0x%08X \n", (unsigned int)pDspDump);

    if (DspDrvDump (dspId, pDspDump, &dspdump_size) != MX_OK)
    {
        MxDelay(1000);
        printf (" Time for DSP operation was out. \n");
        MxHeapFree(NcnbHeap, pDspDump);
        return 0;
    }

    MxDelay(1000);
    printf (" DSP dump size: %d bytes (%d U32 words)\n", dspdump_size, dspdump_size / 4);

    MxHeapFree(NcnbHeap, pDspDump);

    return 0;
}

UINT32 CLineCmdShutdown(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    MxRaiseInterCpu (0, SOFT_ICPU_SYS_SHDWN);

    return 0;
}

#define L2_BLOCK_SIZE   (64*1024)

//extern void TestICProc(void);

UINT32 CLineCmdL2Check(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    UINT32 nMask = 0xFFFF;
    LPVOID pDst, pSrc;
    UINT32 t, i, tmp;

    if (b->ParamCount > 0)
    {
        nMask = StrToNum (b->ParamList [0]);
    }

    uart_printf ("Mask = %x\n", nMask);

    // CRAM <=> NCNB memory
    if (nMask & (1 << 0))
    {
        uart_printf ("Check memcpy NCNB to CRAM ...\n");

        if ((pSrc = MxHeapAlloc (NcnbHeap, L2_BLOCK_SIZE)) == NULL)
        {
            uart_printf ("NCNB alloc error\n");
            return 0;
        }

        if ((pDst = MxHeapAlloc (DspHeap, L2_BLOCK_SIZE)) == NULL)
        {
            MxHeapFree (NcnbHeap, pSrc);
            uart_printf ("CRAM alloc error\n");
            return 0;
        }

        uart_printf ("NCNB mem = %x\n", pSrc);
        uart_printf ("CRAM mem = %x\n", pDst);

        t = GetTIMETICK();
        memcpy (pDst, pSrc, L2_BLOCK_SIZE);
        t = GetTIMETICK() - t;

        uart_printf ("NCNB to CRAM ticks = %d\n", t);

        t = GetTIMETICK();
        memcpy (pSrc, pDst, L2_BLOCK_SIZE);
        t = GetTIMETICK() - t;

        uart_printf ("CRAM to NCNB ticks = %d\n", t);

        MxHeapFree (NcnbHeap, pSrc);
        MxHeapFree (DspHeap, pDst);
    }


    // CACHABLE <=> NCNB memory
    if (nMask & (1 << 1))
    {
        uart_printf ("Check memcpy NCNB to APP ...\n");

        if ((pSrc = MxHeapAlloc (NcnbHeap, L2_BLOCK_SIZE)) == NULL)
        {
            uart_printf ("NCNB alloc error\n");
            return 0;
        }

        if ((pDst = MxHeapAlloc (AppHeap, L2_BLOCK_SIZE)) == NULL)
        {
            MxHeapFree (NcnbHeap, pSrc);
            uart_printf ("APP alloc error\n");
            return 0;
        }

        uart_printf ("NCNB mem = %x\n", pSrc);
        uart_printf ("APP  mem = %x\n", pDst);

        t = GetTIMETICK();
        memcpy (pDst, pSrc, L2_BLOCK_SIZE);
        t = GetTIMETICK() - t;

        uart_printf ("NCNB to APP ticks = %d\n", t);

        t = GetTIMETICK();
        memcpy (pSrc, pDst, L2_BLOCK_SIZE);
        t = GetTIMETICK() - t;

        uart_printf ("APP to NCNB ticks = %d\n", t);

        MxHeapFree (NcnbHeap, pSrc);
        MxHeapFree (AppHeap, pDst);
    }


    // CACHABLE <=> CACHABLE memory
    if (nMask & (1 << 2))
    {
        uart_printf ("Check memcpy APP to APP ...\n");

        if ((pSrc = MxHeapAlloc (AppHeap, L2_BLOCK_SIZE)) == NULL)
        {
            uart_printf ("APP alloc error\n");
            return 0;
        }

        if ((pDst = MxHeapAlloc (AppHeap, L2_BLOCK_SIZE)) == NULL)
        {
            MxHeapFree (AppHeap, pSrc);
            uart_printf ("APP alloc error\n");
            return 0;
        }

        uart_printf ("APP mem = %x\n", pSrc);
        uart_printf ("APP mem = %x\n", pDst);

        t = GetTIMETICK();
        memcpy (pDst, pSrc, L2_BLOCK_SIZE);
        t = GetTIMETICK() - t;

        uart_printf ("APP#1 to APP#2 ticks = %d\n", t);

        t = GetTIMETICK();
        memcpy (pSrc, pDst, L2_BLOCK_SIZE);
        t = GetTIMETICK() - t;

        uart_printf ("APP#2 to APP#1 ticks = %d\n", t);

        MxHeapFree (AppHeap, pSrc);
        MxHeapFree (AppHeap, pDst);
    }

    // CACHE CLEAN
    if (nMask & (1 << 3))
    {
        uart_printf ("Cache clean (APP heap)...\n");

        if ((pSrc = MxHeapAlloc (AppHeap, L2_BLOCK_SIZE)) == NULL)
        {
            uart_printf ("APP alloc error\n");
            return 0;
        }

        // to read and write data (load buffer to cache)

        for (i = 0; i < L2_BLOCK_SIZE / 4; i++)
        {
            ((PUINT32)pSrc)[i] = 0;

            tmp = ((PUINT32)pSrc)[i];

            // tmp = 0

            ((PUINT32)pSrc)[i] = (tmp + i);
        }

        t = GetTIMETICK();
        MxCacheClean (pSrc, L2_BLOCK_SIZE);
        //l2x0_clean (pSrc, L2_BLOCK_SIZE);
        t = GetTIMETICK() - t;

        uart_printf ("clean ticks = %d\n", t);

        t = GetTIMETICK();
        MxCacheInvalidate(pSrc, L2_BLOCK_SIZE);
        //l2x0_invalidate(pSrc, L2_BLOCK_SIZE);
        t = GetTIMETICK() - t;

        uart_printf ("invalidate ticks = %d\n", t);

        // to check the data, it should be error, due to the cache async

        for (i = 0; i < L2_BLOCK_SIZE / 4; i++)
        {
            if (((PUINT32)pSrc)[i] != i)
            {
                uart_printf ("Compare error, off=%d, data=%d\n", i, ((PUINT32)pSrc)[i]);
                break;
            }
        }

        if (i >= L2_BLOCK_SIZE / 4)
            uart_printf ("Buffers are equal!!!\n");

        t = GetTIMETICK();
        //l2x0_clean (pSrc, L2_BLOCK_SIZE);
        t = GetTIMETICK() - t;

        uart_printf ("ticks = %d\n", t);

        MxHeapFree (AppHeap, pSrc);
    }

    if (nMask & (1 << 4))
    {
        IRQTYPE r = ARM_INT_disable ();
        UINT32 tcommon = 0, nNum = 100, i;

        uart_printf ("Begin IProc...");

        for (i = 0; i < nNum; i++)
        {
            t = GetTIMETICK();
            //TestICProc ();
            t = GetTIMETICK() - t;

            tcommon += t;
        }

        ARM_INT_restore (r);

        uart_printf ("ticks = %d\n", tcommon / nNum);
    }

    uart_printf ("tests are finished\n");

    return 0;
}

extern MXICCTX   MxICCtx [];    // The inter-cpu contextes of the 4GMX systems
extern MXICCTX * MxICTable [];  // The table of the pointers to the IC contextes

void DumpICObj (UINT32 nID )
{
    MXICCTX * pObj = &MxICCtx [nID];
    UINT32 i;

    uart_printf ("\n=======================\n");
    uart_printf ("Obj ID=%d\n", nID);

    uart_printf ("MxICTable[%d]=%x\n", nID, MxICTable[nID]);

    uart_printf (".Lock=%x\n", pObj->Lock);
    uart_printf (".CF  =%x\n", pObj->CF);
    uart_printf (".Irq =%x\n", pObj->Irq);

    for (i = 0; i < MX_MAX_INTERCPU; i++)
    {
        uart_printf ("[%d].CmdPool.StoragePtr =%x\n", i, pObj->CmdPool[i].StoragePtr);
        uart_printf ("[%d].CmdPool.BlockNum   =%d\n", i, pObj->CmdPool[i].BlockNum);
        uart_printf ("[%d].CmdPool.BlockSize  =%d\n", i, pObj->CmdPool[i].BlockSize);
        uart_printf ("[%d].CmdPool.FreePtr    =%x\n", i, pObj->CmdPool[i].FreePtr);
        uart_printf ("[%d].CmdPool.FreePut    =%d\n", i, pObj->CmdPool[i].FreePut);
        uart_printf ("[%d].CmdPool.FreeGet    =%d\n", i, pObj->CmdPool[i].FreeGet);
        uart_printf ("[%d].CmdPool.UsedPtr    =%x\n", i, pObj->CmdPool[i].UsedPtr);
        uart_printf ("[%d].CmdPool.UsedPut    =%d\n", i, pObj->CmdPool[i].UsedPut);
        uart_printf ("[%d].CmdPool.UsedGet    =%d\n", i, pObj->CmdPool[i].UsedGet);
    }

    uart_printf (".ShrPart.storage =%x\n", pObj->ShrPart.storage);
    uart_printf (".ShrPart.freeblk =%x\n", pObj->ShrPart.freeblk);
    uart_printf (".ShrPart.blksize =%x\n", pObj->ShrPart.blksize);
    uart_printf (".ShrPart.blkcnt  =%x\n", pObj->ShrPart.blkcnt);
    uart_printf (".ShrPart.AllocCnt=%x\n", pObj->ShrPart.AllocCnt);

    uart_printf (".CoreCPUID=%x\n", pObj->CoreCPUID);
    uart_printf ("=======================\n");

}

UINT32 CLineCmdDumpICObj(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    DumpICObj (0);
    DumpICObj (1);

    return 0;
}

UINT32 RegComplete = 0;
VUINT32 MsgProc = 0;
VUINT32 MsgSeng = 0;

MXRC TstMsgHandler(LPVOID pClientCtx, LPVOID pMsg)
{
    uart_printf ("MsgHandler, cpu-%d, ctx=%x, msg=%x\r\n", MxGetCpuID (), pClientCtx, pMsg);

    MsgProc ++;

    return 0;
}

UINT32 RegThread (void * p)
{
    SvsrRegMsgHandler (1, TstMsgHandler, (LPVOID)0x1234);

    RegComplete = 1;
    return 0;
}

UINT32 CLineCmdMsgTest(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    UINT32 nCore = 1;
    HANDLE hThrReg;
    MXRC rc, i;

    UINT32 alloc_avg = 0, alloc_max = 0, alloc = 0,
           send_avg = 0, send_max = 0, send = 0;

    if (b->ParamCount > 0)
        nCore = StrToNum (b->ParamList [0]);

    uart_printf ("Test svsr-msg for core-%d\r\n", nCore);

    RegComplete = 0;

    rc = MxCreateThreadEx (nCore, RegThread, NULL, 0, 0, &hThrReg);

    if (FAILED(rc))
    {
        uart_printf ("Create thread error, rc=%x\r\n", rc);
        return 0;
    }
    else
        uart_printf ("Create thread OK\r\n");

    rc = MxScheduleThread (hThrReg);

    if (FAILED(rc))
    {
        MxRemoveThread (hThrReg);
        uart_printf ("Schedule thread error, rc=%x\r\n", rc);
        return 0;
    }

    uart_printf ("Waiting ... ");
    while (RegComplete == 0);
    uart_printf ("OK\r\n");

    MxRemoveThread (hThrReg);

    MsgProc = 0;
    MsgSeng = 0;

    i = 0;
    while (i < SVSR_MSG_NUM)
    {
        //uart_printf ("Send msg-%d...");

        ApiHeader * pApi;

        alloc = GetTIMETICK();
        pApi = SvsrAllocMsg ();
        alloc = GetTIMETICK() - alloc;

        alloc_max = MAX(alloc_max, alloc);

        if (alloc_avg)
            alloc_avg = (alloc_avg + alloc) >> 1;
        else
            alloc_avg = (alloc);

        MsgSeng ++;

        send = GetTIMETICK();
        rc = SvsrSendMsg (nCore, 1, (LPVOID)pApi, SVSR_MSG_OPT_DEALLOC);
        send = GetTIMETICK() - send;

        send_max = MAX(send_max, send);

        if (send_avg)
            send_avg = (send_avg + send) >> 1;
        else
            send_avg = (send);

        if (rc)
            uart_printf ("error, rc=%x\r\n", rc);
        /*else
            uart_printf ("OK\r\n", rc);*/

        if (FAILED(rc))
            break;

        i++;
    }

    if (SUCCESSFUL(rc))
    {
        while (MsgSeng != MsgProc) {__nop();}
    }

    uart_printf ("Sending is completed:\r\n");
    uart_printf (" - Alloc: avg:%d  max: %d\r\n", alloc_avg, alloc_max);
    uart_printf (" - Send:  avg:%d  max: %d\r\n", send_avg, send_max);

    return 0;
}

VUINT32 ICEndTime;
VUINT32 ICAvgTime;
VUINT32 ICCount = 0;
VUINT32 ICMaxCount = 0;

APIHEC SubApiHandler (LPVOID pCtx, ApiHeader * pApi, ApiHeader * pResp)
{
    ICEndTime = GetTIMETICK () - pApi->Length;

    if (ICAvgTime == 0)
        ICAvgTime = (ICEndTime);
    else
        ICAvgTime = (ICAvgTime + ICEndTime) >> 1;

    ICCount++;

    if (ICCount >= ICMaxCount)
    {
        uart_printf ("A(cpu-%d) Api handler, ctx=%x, pApi=%x, pResp=%x\r\n", MxGetCpuID (), pCtx, pApi, pResp);
        uart_printf ("ICCount:%d, avgtime=%d\r\n", ICCount, ICAvgTime);
    }

    return APIHEC_OK;
}

UINT32 ApiRegReady = 0;

UINT32 RegApiHThread (LPVOID p)
{
    SvsrRegClient (IID_EXT_APP, SubApiHandler, (LPVOID)0x1234);
    ApiRegReady = 1;
    return 0;
}

UINT32 CLineCmdRegSubApiHandle(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    UINT32 nCore = 1;
    HANDLE hThrReg;
    MXRC rc;

    ICMaxCount  = 100;
    ICCount = 0;
    ApiRegReady = 0;
    ICAvgTime = 0;

    if (b->ParamCount > 0)
        nCore = StrToNum (b->ParamList [0]);

    if (b->ParamCount > 1)
        ICMaxCount = StrToNum (b->ParamList [1]);

    uart_printf ("Test svsr-msg for core-%d, tst count=%d\r\n", nCore, ICMaxCount);

    if (nCore == MxGetCpuID ())
    {
        SvsrRegClient (IID_EXT_APP, SubApiHandler, (LPVOID)0x1234);
    }
    else
    {
        rc = MxCreateThreadEx (nCore, RegApiHThread, NULL, 0, 0, &hThrReg);

        if (FAILED(rc))
        {
            uart_printf ("Create thread error, rc=%x\r\n", rc);
            return 0;
        }
        else
            uart_printf ("Create thread OK\r\n");

        rc = MxScheduleThread (hThrReg);

        if (FAILED(rc))
        {
            MxRemoveThread (hThrReg);
            uart_printf ("Schedule thread error, rc=%x\r\n", rc);
            return 0;
        }

        uart_printf ("Waiting ... ");
        while (ApiRegReady == 0);
        uart_printf ("OK\r\n");

        MxRemoveThread (hThrReg);
    }

    uart_printf ("Ready to proc API on core-%d\r\n", nCore);

    return 0;
}


extern APIHEC LTEAPIHandlerStub(LPVOID pClientCtx, ApiHeader *pApi, ApiHeader *pResp);
UINT32 TstApiRegReady = 0;

UINT32 TstRegApiHThread (LPVOID p)
{
    SvsrRegClient (IID_EXT_APP, LTEAPIHandlerStub, (LPVOID)NULL);
    TstApiRegReady = 1;
    return 0;
}

UINT32 CLineCmdRegSubApiHandleTst(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    UINT32 nCore = 0;
    HANDLE hThrReg;
    MXRC rc;

    if (b->ParamCount > 0)
        nCore = StrToNum (b->ParamList [0]);

    uart_printf ("Test svsr-msg for core-%d, id=%d\r\n", nCore, IID_EXT_APP);

    if (nCore == MxGetCpuID ())
    {
        SvsrRegClient (IID_EXT_APP, LTEAPIHandlerStub, (LPVOID)NULL);
    }
    else
    {
        TstApiRegReady = 0;

        rc = MxCreateThreadEx (nCore, TstRegApiHThread, NULL, 0, 0, &hThrReg);

        if (FAILED(rc))
        {
            uart_printf ("Create thread error, rc=%x\r\n", rc);
            return 0;
        }
        else
            uart_printf ("Create thread OK\r\n");

        rc = MxScheduleThread (hThrReg);

        if (FAILED(rc))
        {
            MxRemoveThread (hThrReg);
            uart_printf ("Schedule thread error, rc=%x\r\n", rc);
            return 0;
        }

        uart_printf ("Waiting ... ");
        while (TstApiRegReady == 0);
        uart_printf ("OK\r\n");

        MxRemoveThread (hThrReg);
    }

    uart_printf ("Ready to proc API for id=%d on core-%d\r\n", IID_EXT_APP, nCore);

    return 0;
}



UINT32 CLineCmdPrintVersion(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    PrintSysVersion ();
    return 0;
}

UINT32 CLineCmdHeapsInfo(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    UINT32 nHeapNum, i, nSize, nOrgSize, id;
    UINT64 Val;
    HANDLE h;
    char * pName;
    HEAPINFO info;

    nHeapNum = MxGetObjCount (OBJ_TYPE_HEAP);

    uart_printf ("\r\n");

    for (i = 0; i < nHeapNum; i++)
    {
        h = MxGetObjByIndex (OBJ_TYPE_HEAP, i);

        if (HANDLE_TOINT(h) == HANDLE_TOINT(AppHeap))
        {
            id = ID_HEAP_APP;
            pName = "AppHeap     ";
        }
        else if (HANDLE_TOINT(h) == HANDLE_TOINT(DspHeap))
        {
            id = ID_HEAP_DSP;
            pName = "DspHeap     ";
        }
        else if (HANDLE_TOINT(h) == HANDLE_TOINT(NcnbHeap))
        {
            id = ID_HEAP_NCNB;
            pName = "NcnbHeap    ";
        }
        else if (HANDLE_TOINT(h) == HANDLE_TOINT(IramHeap))
        {
            id = ID_HEAP_IRAM;
            pName = "IramHeap    ";
        }
        else if (HANDLE_TOINT(h) == HANDLE_TOINT(DDR1Heap))
        {
            id = ID_HEAP_DDR1;
            pName = "DDR1Heap    ";
        }
        else if (HANDLE_TOINT(h) == HANDLE_TOINT(DDR1NcnbHeap))
        {
            id = ID_HEAP_DDR1_NCNB;
            pName = "DDR1NcnbHeap";
        }
        else if (HANDLE_TOINT(h) == HANDLE_TOINT(PRV_CORE_HEAP))
        {
            id = ID_HEAP_PRV_CORE0;
            pName = "PrvCore0Heap";
        }
        else
        {
            id = ~0;
            pName = "DEFCORE-HEAP";
        }

        nOrgSize = MxHeapGetTotalSize(h);
        nSize    = MxHeapGetFreeSpace (h);

        GetHeapInfo (id, &info);

        Val = nSize;
        Val *= 100;
        Val /= nOrgSize;

        uart_printf (" - %s [h=0x%08x]{0x%08x..0x%08x}, size = %d / %d (%d%% free)\r\n",
            pName,
            HANDLE_TOINT(h),
            info.BaseAddr,
            info.BaseAddr + info.Size,
            nSize,
            nOrgSize,
            (UINT32)Val);
    }

    return 0;
}

#ifdef PHY_GEM_ENABLED

UINT32 CLineCmdGemInit(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    MXRC rc;
    GemCfg    cfg;
    GemDMACfg dmacfg;
    LPVOID    pGemDrv;

    GemDrvShutdown (1);

    rc = GemDrvInit (GEMAC1_BASEADDR);

    if (FAILED(rc))
    {
        uart_printf ("GEM init failed, rc=%x\r\n", rc);
        return 0;
    }

    uart_printf ("GEM init OK\r\n");

    pGemDrv = GemDrvGetCtx (1);

    _ASSERT_PTR (pGemDrv);

    cfg.Phy     = RGMII;
    cfg.Speed   = MAC_SPEED_1000Mbps;

     // 00-11-22-33-44-55 - temporary MAC address

    cfg.MacAddr[0] = 0x00;
    cfg.MacAddr[1] = 0x11;
    cfg.MacAddr[2] = 0x22;
    cfg.MacAddr[3] = 0x33;
    cfg.MacAddr[4] = 0x44;
    cfg.MacAddr[5] = 0x55;

    if (FAILED(rc = GemDrvCfg(pGemDrv, &cfg)))
    {
        uart_printf ("GEM config failed, rc=%x\r\n", rc);
        return 0;
    }

    uart_printf ("GEM config OK\r\n");

    // GEM DMA configuration

    dmacfg.RxDescrCount = ETH_NUM_RX_BUFS;
    dmacfg.TxDescrCount = ETH_NUM_TX_BUFS;

    dmacfg.RxDescr   = (HGRxFrm)gem_rx_frm;
    dmacfg.TxDescr   = (HGTxFrm)gem_tx_frm;
    dmacfg.RxBuffers = gem_rx_buffers;

    dmacfg.DMAConfig = GEM_DMA_DEF_RX_SIZE(2048) | GEM_DMA_BURST_INCR4;
    //dmacfg.DMAConfig = GEM_DMA_DEF_RX_SIZE(GEM_0_RX_BUF_SIZE) | GEM_DMA_BURST_INCR16;
    dmacfg.RxBufOpt    = GEM_DMA_RX_IRQ_ENB;

    if (FAILED(rc = GemDrvConfigDMA(pGemDrv, &dmacfg)))
    {
        uart_printf ("GEM config DMA failed, rc=%x\r\n", rc);
        return 0;
    }

    uart_printf ("GEM config DMA OK\r\n");

    GemDrvDisableRX(pGemDrv);
    GemDrvEnableTX(pGemDrv);

    if (FAILED(rc = GemDrvCopyAll (pGemDrv, 1)))
    {
        uart_printf ("GEM set copyall failed, rc=%x\r\n", rc);
        return 0;
    }

    uart_printf ("GEM set copyall OK\r\n");

    if (FAILED(rc = GemDrvEnBroadcast (pGemDrv, 1)))
    {
        uart_printf ("GEM set broadcast failed, rc=%x\r\n", rc);
        return 0;
    }

    uart_printf ("GEM set broadcast OK\r\n");

    return 0;
}

void GemRXCb (LPVOID pCBCtx, LPVOID pGemDrvCtx, UINT32 nIrqStatus)
{
    HGRxFrm rx_frm;
    UINT32 BufNum = 0;
    UINT32 FrmSize = 0;
    PUINT8 pRxBuf = 0;

    uart_printf ("GEM IRQ %x \r\n", nIrqStatus);

	if(!(nIrqStatus & GEM_INT_RX_COMPLETE || nIrqStatus & GEM_INT_TX_COMPLETE))
		return;

    while ((GemDrvRxFrm (pGemDrvCtx, &rx_frm, &BufNum, &FrmSize)) == MX_OK)
    {
        pRxBuf = GemDrvGetRxFrmBuf (pGemDrvCtx, rx_frm, 0);

        if (pRxBuf != NULL)
        {
            uart_printf ("GEM-RX:{%02x-%02x-%02x-%02x-%02x-%02x}, size=%d\r\n",
                pRxBuf [0],
                pRxBuf [1],
                pRxBuf [2],
                pRxBuf [3],
                pRxBuf [4],
                pRxBuf [5],
                GEM_DMA_RX_GET_LEN (rx_frm)
            );
        }

        GemDrvFreeRxFrm (pGemDrvCtx, rx_frm);

        BufNum = 0;
        FrmSize= 0;
    }
}

UINT32 CLineCmdGemDump(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    LPVOID    pGemDrv;
    UINT32 nStart = 1;

    pGemDrv = GemDrvGetCtx (1);

    if (pGemDrv == NULL)
    {
        uart_printf ("GEM is not inited\r\n");
        return 0;
    }

    if (b->ParamCount > 0)
        nStart = StrToNum (b->ParamList[0]);

    GemDrvDisableRX(pGemDrv);

    if (nStart == 0)
    {
        uart_printf ("GEM-RX was disabled\r\n");
        return 0;
    }

    GemDrvSetIrqProc (pGemDrv, GemRXCb, NULL, GEM_INT_RX_COMPLETE|GEM_INT_TX_COMPLETE);

    GemDrvEnableRX(pGemDrv);

    uart_printf ("GEM-RX is enabled\r\n");

    return 0;
}

__align (128) UINT8 NCNBVAR gem_tx_buf [1024];

UINT32 CLineCmdGemSend(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    LPVOID pGemDrv;
    MXRC   rc     = MX_OK;
    UINT32 nCount = 10;
    UINT32 nSize  = 200;
    UINT32 i;

    pGemDrv = GemDrvGetCtx (1);

    if (pGemDrv == NULL)
    {
        uart_printf ("GEM is not inited\r\n");
        return 0;
    }

    if (b->ParamCount > 0)
        nCount = StrToNum (b->ParamList[0]);

    if (b->ParamCount > 1)
        nSize = StrToNum (b->ParamList[1]);

    nSize = MAX(nSize, 14);
    nSize = MIN(nSize, sizeof (gem_tx_buf));

    uart_printf ("TX num=%d, size=%d\r\n", nCount, nSize);

    // 90-E6-BA-44-3B-DE

    for (i = 0; i < nCount; i++)
    {
        gem_tx_buf [0] = 0xff;
        gem_tx_buf [1] = 0xff;
        gem_tx_buf [2] = 0xff;
        gem_tx_buf [3] = 0xff;
        gem_tx_buf [4] = 0xff;
        gem_tx_buf [5] = 0xff;

        gem_tx_buf [6] = 0x00;
        gem_tx_buf [7] = 0x11;
        gem_tx_buf [8] = 0x22;
        gem_tx_buf [9] = 0x33;
        gem_tx_buf [10]= 0x44;
        gem_tx_buf [11]= 0x55;

        gem_tx_buf [12] = 0x11;
        gem_tx_buf [13] = 0x22;

        memset (&gem_tx_buf [14], i, nSize - 14);

        uart_printf ("%02d. Send packet (size=%d) ... ", i, nSize);
        //rc = GemDrvTxFrm (pGemDrv, GEM_DMA_TX_CRC, gem_tx_buf, nSize);
        rc = GemDrvTxFrm (pGemDrv, GEM_DMA_TX_CRC, gem_tx_buf, nSize, gem_tx_buf, nSize);

        if (FAILED(rc))
            uart_printf ("error, rc=%x\r\n", rc);
        else
            uart_printf ("OK\r\n");
    }

    return 0;
}

#endif // #ifdef PHY_GEM_ENABLED

#ifdef MX_HEAP_LEAKAGE_CONTROL

int _strcmp (const char * pSrc, char * pDst);

UINT32 CLineCmdHeapLC(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    HANDLE hHeap = AppHeap;
    MEM_ALLOC_BLOCK* pBlock;
    HEAPINFO heapInfo;
    char * pName = "AppHeap";
    UINT32 nCrcID;
    char * pCrc;
    char * pProcName;
    PRootInfo pInfo;

    uart_printf ("\r\n");

    GetHeapInfo(ID_HEAP_APP, &heapInfo);

    if (b->ParamCount > 0)
    {
        pName = b->ParamList [0];

        if (_strcmp (b->ParamList [0], "app") == 0)
        {
            hHeap = AppHeap;
            GetHeapInfo(ID_HEAP_APP, &heapInfo);
        }
        else if (_strcmp (b->ParamList [0], "ncnb") == 0)
        {
            hHeap = NcnbHeap;
            GetHeapInfo(ID_HEAP_NCNB, &heapInfo);
        }
        else if (_strcmp (b->ParamList [0], "dsp") == 0)
        {
            hHeap = DspHeap;
            GetHeapInfo(ID_HEAP_DSP, &heapInfo);
        }
        else if (_strcmp (b->ParamList [0], "iram") == 0)
        {
            hHeap = IramHeap;
            GetHeapInfo(ID_HEAP_IRAM, &heapInfo);
        }
        else if (_strcmp (b->ParamList [0], "ddr1") == 0)
        {
            hHeap = DDR1Heap;
            GetHeapInfo(ID_HEAP_DDR1, &heapInfo);
        }
        else if (_strcmp (b->ParamList [0], "ddr1ncnb") == 0)
        {
            hHeap = DDR1NcnbHeap;
            GetHeapInfo(ID_HEAP_DDR1_NCNB, &heapInfo);
        }
        else if (_strcmp (b->ParamList [0], "core0") == 0)
        {
            hHeap = PRV_CORE_HEAP;
            GetHeapInfo(ID_HEAP_PRV_CORE0, &heapInfo);
        }
        else
        {
            uart_printf ("Unknown heap: %s\r\n", b->ParamList [0]);
            return 0;
        }
    }
    else
    {
        uart_printf ("heapinfo dsp\r\n");
        uart_printf ("heapinfo app\r\n");
        uart_printf ("heapinfo iram\r\n");
        uart_printf ("heapinfo ncnb\r\n");
        uart_printf ("heapinfo ddr1\r\n");
        uart_printf ("heapinfo ddr1ncnb\r\n");
        uart_printf ("heapinfo core0\r\n");

        return 0;
    }

    pBlock = MxHeapGetAllocChain (hHeap);

    if (pBlock == NULL)
    {
        uart_printf ("No allocation is detected\r\n");
    }

    uart_printf ("  HEAP INFO [%s]\r\n", pName);
    uart_printf ("  [ ARM code ] [CPU] [    PTR   ]   [  SIZE   ]  [ ID ]  [CRC] [    PROC    ]\r\n");
    uart_printf ("  ===========================================================================\r\n");
    while (pBlock != NULL)
    {
        nCrcID = MxHeapBlockIntegrity(hHeap, MxHeapGetAppBlockPtr(hHeap, pBlock));

        if (nCrcID == MX_HEAP_INTEGRITY_OK)
        {
            pCrc = "OK";
        }
        else if (nCrcID == MX_HEAP_INTEGRITY_BAD)
        {
            pCrc = "BAD";
        }
        else
        {
            pCrc = "---";
        }

        pInfo = DbgGetObjInfo(pBlock->AllocAddr);

        if (pInfo != NULL)
        {
            pProcName = pInfo->ObjName;
        }
        else
        {
            pProcName = NULL;
        }

        uart_printf ("   0x%08x    %d    0x%08x     % 9d    %04d    %s    %s\r\n",
            pBlock->AllocAddr,
            pBlock->AllocCpuID,
            MxHeapGetAppBlockPtr(hHeap, pBlock),
            MxHeapGetAppBlockSize(hHeap, pBlock),
            pBlock->MarkID,
            pCrc,
            pProcName
            );

        MxDelayTicks (150*1000);

        pBlock = pBlock->NextAllocated;
    }
    uart_printf ("  ===========================================================================\r\n");
    uart_printf("\r\n  Free Space in Heap:     %d / %d bytes (%2d%% free)\r\n", MxHeapGetFreeSpace(hHeap), heapInfo.Size, (UINT32)(((UINT64)MxHeapGetFreeSpace(hHeap)*100)/(UINT64)heapInfo.Size));

    #ifdef MX_HEAP_INTEGRITY
    pBlock = NULL;
    pCrc = (MxHeapCheckIntegrity(hHeap, (void**)&pBlock) == MX_OK) ? "OK" : "BAD";

    if (pBlock == NULL)
        uart_printf("  Heap integrity is %s\r\n", pCrc);
    else
        uart_printf("  Heap integrity is %s, bad block is [0x%08x]\r\n", pCrc, pBlock);
    #endif

    return 0;
}
#else
UINT32 CLineCmdHeapLC(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    uart_printf ("Heap leakage control is turned off\r\n");
    return 0;
}
#endif

UINT32 CLineCmdRadMdma(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    unsigned int i;
    unsigned int datasize = 512;
    unsigned char* pSrcBuf;
    unsigned char* pDstBuf;
    PMDMA_FDESC pFDesc;
    MDMAHWCTX HWContext;

    // source data buffer
    pSrcBuf = MxHeapAlloc(NcnbHeap, datasize);
    if (pSrcBuf == NULL)
    {
        printf("Cannot allocate the buffer.\n");
        return RC_CM_PARAM_ERROR;
    }
    memset(pSrcBuf, 0, datasize);

    // destination data buffer
    pDstBuf = MxHeapAlloc(NcnbHeap, datasize);
    if (pDstBuf == NULL)
    {
        printf("Cannot allocate the buffer.\n");
        MxHeapFree(NcnbHeap, pSrcBuf);
        return RC_CM_PARAM_ERROR;
    }
    memset(pDstBuf, 0, datasize);

    // initialization of source buffer;
    for (i = 0; i < datasize; i++)
    {
        if (i > 255)
        {
            *(pSrcBuf + i) = i % 256;
            continue;
        }
        *(pSrcBuf + i) = i;
    }

    // initialization of radio MDMA driver
    if (MdmaRadInit() != MX_OK)
    {
        printf ("Cannot initialize radio MDMA driver.\n");
        return RC_CM_PARAM_ERROR;
    }

    // creating frame descriptor
    pFDesc = MdmaAllocDesc(MDMA_HW_RAD, 1);
    if (pFDesc == NULL)
    {
        printf ("Cannot allocate memory for frame descriptors.\n");
        return RC_CM_PARAM_ERROR;
    }

    // creating context
    memset (&HWContext, 0, sizeof(MDMAHWCTX));
    MdmaSetHwCtx(pFDesc, HWContext);

    // filling buffer descriptors in frame descriptor
    MdmaSetData (pFDesc, pSrcBuf, datasize, pDstBuf, datasize);

    // Turn on RAD MDMA IRQ
    MdmaSetLastDesc(pFDesc, NULL, NULL);

    // start MDMA
//    *(PUINT32)(MDMA_RAD_CTRL_REGS_BASE + 4) = (UINT32)pFDesc;
    printf ("Frame descriptor address: 0x%08X\n", (UINT32)pFDesc);
    printf ("Source buffer address: 0x%08X\n", (UINT32)pSrcBuf);
    printf ("Destination buffer address: 0x%08X\n", (UINT32)pDstBuf);
    printf ("radio MDMA register's status:\n");
    printf ("0x%08X\n", *(PUINT32)MDMA_RAD_CTRL_REGS_BASE);
    printf ("0x%08X\n", *(PUINT32)(MDMA_RAD_CTRL_REGS_BASE + 4));
    printf ("0x%08X\n", *(PUINT32)(MDMA_RAD_CTRL_REGS_BASE + 8));
    printf ("0x%08X\n", *(PUINT32)(MDMA_RAD_CTRL_REGS_BASE + 12));
    printf ("0x%08X\n", *(PUINT32)(MDMA_RAD_CTRL_REGS_BASE + 16));
    printf ("0x%08X\n", *(PUINT32)(MDMA_RAD_CTRL_REGS_BASE + 20));
    printf ("Frame Descriptor:\n");
    printf ("0x%08X\n", *(PUINT32)pFDesc);
    printf ("0x%08X\n", *(PUINT32)((UINT32)pFDesc + 4));
    printf ("0x%08X\n", *(PUINT32)((UINT32)pFDesc + 8));
    printf ("0x%08X\n", *(PUINT32)((UINT32)pFDesc + 12));
    printf ("0x%08X\n", *(PUINT32)((UINT32)pFDesc + 16));
    printf ("0x%08X\n", *(PUINT32)((UINT32)pFDesc + 24));
    printf ("start radio MDMA...\n");
//    MdmaRadRun (pFDesc);
    MdmaRunEx(MDMA_HW_RAD, pFDesc, NULL, NULL);
//    printf ("0x%08X\n", *(PUINT32)MDMA_RAD_CTRL_REGS_BASE);
    printf (" Head pointer register: 0x%08X\n", *(PUINT32)(MDMA_RAD_CTRL_REGS_BASE + 4));


    printf ("input data:\n");
    for (i = 0; i < datasize; i++)
    {
        if (!(i % 8))
        {
            printf ("%d, ", *(pSrcBuf + i));
            MxDelay (200);
        }
    }

    printf ("\n output data:\n");
    for (i = 0; i < datasize; i++)
    {
        if (!(i % 8))
        {
            printf ("%d, ", *(pDstBuf + i));
            MxDelay (200);
        }
    }

    // empty the memory
//    MdmaRadFreeDesc (pFDesc);
    MxHeapFree (NcnbHeap, pSrcBuf);
    MxHeapFree (NcnbHeap, pDstBuf);

    MxDelay(500);
    printf ("\nNumber of ticks: %d\n", MdmaGetExecTicks(MDMA_HW_RAD));
    printf ("Master CPU is: %d\n", MdmaRadGetMasterCpu());

    printf ("\n the test ended.\n");

    return 0;
}

UINT32 CLineCmdSysMdma(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    unsigned int i;
    unsigned int datasize = 512;
    unsigned int logsize  = 1280;
    unsigned char* pSrcBuf;
    unsigned char* pDstBuf;
    unsigned char* pLogBuf;
    PMDMA_FDESC pFDesc;
    MDMAHWCTX HWContext;

    // source data buffer
    pSrcBuf = MxHeapAlloc(NcnbHeap, datasize);
    if (pSrcBuf == NULL)
    {
        printf("Cannot allocate the buffer.\n");
        return RC_CM_PARAM_ERROR;
    }
    memset(pSrcBuf, 0, datasize);

    // destination data buffer
    pDstBuf = MxHeapAlloc(NcnbHeap, datasize);
    if (pDstBuf == NULL)
    {
        printf("Cannot allocate the buffer.\n");
        MxHeapFree(NcnbHeap, pSrcBuf);
        return RC_CM_PARAM_ERROR;
    }
    memset(pDstBuf, 0, datasize);

    // buffer for logger
    pLogBuf = MxHeapAlloc(NcnbHeap, logsize);
    if (pLogBuf == NULL)
    {
    	printf("Cannot allocate the buffer.\n");
    	MxHeapFree(NcnbHeap, pSrcBuf);
    	MxHeapFree(NcnbHeap, pDstBuf);
    	return RC_CM_PARAM_ERROR;
    }
    memset(pLogBuf, 0, logsize);

    // initialization of source buffer;
    for (i = 0; i < datasize; i++)
    {
        if (i > 255)
        {
            *(pSrcBuf + i) = i % 256;
            continue;
        }
        *(pSrcBuf + i) = i;
    }

    // initialization of Sys MDMA driver
    if (MdmaSysInit() != MX_OK)
    {
        printf ("Cannot initialize sys MDMA driver.\n");
        MxHeapFree(NcnbHeap, pSrcBuf);
        MxHeapFree(NcnbHeap, pDstBuf);
        MxHeapFree(NcnbHeap, pLogBuf);
        return RC_CM_PARAM_ERROR;
    }

    // creating frame descriptor
    pFDesc = MdmaAllocDesc(MDMA_HW_SYS, 1);
    if (pFDesc == NULL)
    {
        printf ("Cannot allocate memory for frame descriptors.\n");
        MxHeapFree(NcnbHeap, pSrcBuf);
        MxHeapFree(NcnbHeap, pDstBuf);
        MxHeapFree(NcnbHeap, pLogBuf);
        return RC_CM_PARAM_ERROR;
    }

    // creating context
    memset (&HWContext, 0, sizeof(MDMAHWCTX));
    MdmaSetHwCtx(pFDesc, HWContext);

    // filling buffer descriptors in frame descriptor
    MdmaSetData (pFDesc, pSrcBuf, datasize, pDstBuf, datasize);

    // enable logging
    if (MdmaSysSetLog(MDMA_OP_COPY, 0x3, pLogBuf, logsize) != MX_OK)
    {
    	printf ("Cannot enable logging.\n");
    	MxHeapFree(NcnbHeap, pSrcBuf);
    	MxHeapFree(NcnbHeap, pDstBuf);
    	MxHeapFree(NcnbHeap, pLogBuf);
    	return RC_CM_PARAM_ERROR;
    }

    // start Sys MDMA
//    *(PUINT32)(MDMA_RAD_CTRL_REGS_BASE + 4) = (UINT32)pFDesc;
    printf ("Frame descriptor address: 0x%08X\n", (UINT32)pFDesc);
    printf ("Source buffer address: 0x%08X\n", (UINT32)pSrcBuf);
    printf ("Destination buffer address: 0x%08X\n", (UINT32)pDstBuf);
    printf ("Sys MDMA register's status:\n");
    printf ("0x%08X\n", *(PUINT32)MDMA_SYS_CTRL_REGS_BASE);
    printf ("0x%08X\n", *(PUINT32)(MDMA_SYS_CTRL_REGS_BASE + 4));
    printf ("0x%08X\n", *(PUINT32)(MDMA_SYS_CTRL_REGS_BASE + 8));
    printf ("0x%08X\n", *(PUINT32)(MDMA_SYS_CTRL_REGS_BASE + 12));
    printf ("0x%08X\n", *(PUINT32)(MDMA_SYS_CTRL_REGS_BASE + 16));
    printf ("0x%08X\n", *(PUINT32)(MDMA_SYS_CTRL_REGS_BASE + 20));
    printf ("Frame Descriptor:\n");
    printf ("0x%08X\n", *(PUINT32)pFDesc);
    printf ("0x%08X\n", *(PUINT32)((UINT32)pFDesc + 4));
    printf ("0x%08X\n", *(PUINT32)((UINT32)pFDesc + 8));
    printf ("0x%08X\n", *(PUINT32)((UINT32)pFDesc + 12));
    printf ("0x%08X\n", *(PUINT32)((UINT32)pFDesc + 16));
    printf ("0x%08X\n", *(PUINT32)((UINT32)pFDesc + 24));
    printf ("start Sys MDMA...\n");
//    MdmaSysRun (pFDesc);
    MdmaRunEx (MDMA_HW_SYS, pFDesc, NULL, NULL);

    printf ("input data:\n");
    for (i = 0; i < datasize; i++)
    {
        if (!(i % 8))
        {
            printf ("%d, ", *(pSrcBuf + i));
            MxDelay (200);
        }
    }

    printf ("\n output data:\n");
    for (i = 0; i < datasize; i++)
    {
        if (!(i % 8))
        {
            printf ("%d, ", *(pDstBuf + i));
            MxDelay (200);
        }
    }

    uart_printf ("\ndata from log buffer:\n");
    for (i = 0; i < logsize; i++)
    {
    	if (!(i % 8))
    	{
    		uart_printf ("%d, ", *(pLogBuf + i));
    		MxDelay (200);
    	}
    }

    // disable logging
    if (MdmaSysCloseLog() != MX_OK)
    	printf ("cannot disable logging.\n");

    // empty the memory
//    MdmaSysFreeDesc (pFDesc);
    MxHeapFree (NcnbHeap, pSrcBuf);
    MxHeapFree (NcnbHeap, pDstBuf);
    MxHeapFree (NcnbHeap, pLogBuf);

    printf ("\n the test ended.\n");

    return 0;
}

UINT32 CLineCmdFreq(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    uart_printf ("==============================\r\n");

    uart_printf ("  PLL-0 frequency is %d Mhz\r\n", ClkRstGetFreq (CR_PLL_0) / 1000000);
    uart_printf ("  PLL-1 frequency is %d Mhz\r\n", ClkRstGetFreq (CR_PLL_1) / 1000000);
    uart_printf ("  PLL-2 frequency is %d Mhz\r\n", ClkRstGetFreq (CR_PLL_2) / 1000000);
    uart_printf ("  PLL-3 frequency is %d Mhz\r\n", ClkRstGetFreq (CR_PLL_3) / 1000000);
    uart_printf ("  PLL-4 frequency is %d Mhz\r\n", ClkRstGetFreq (CR_PLL_4) / 1000000);
    uart_printf ("  PLL-5 frequency is %d Mhz\r\n", ClkRstGetFreq (CR_PLL_5) / 1000000);
    uart_printf ("\r\n");

    uart_printf ("  DDR-0 frequency is %d Mhz (PLL-%d)\r\n", SysDDRGetFreq(0, ClkRstGetFreq (CR_DDR0)) / 1000000, ClkGetSrcPll (CR_DDR0));
    uart_printf ("  DDR-1 frequency is %d Mhz (PLL-%d)\r\n", SysDDRGetFreq(1, ClkRstGetFreq (CR_DDR0)) / 1000000, ClkGetSrcPll (CR_DDR0));
    uart_printf ("  ARMx4 frequency is %d Mhz (PLL-%d)\r\n", ClkRstGetFreq (ARMx4_CORE0) / 1000000, ClkGetSrcPll (ARMx4_CORE0));
    uart_printf ("  ARMx2 frequency is %d Mhz (PLL-%d)\r\n", ClkRstGetFreq (ARMx2_CORE0) / 1000000, ClkGetSrcPll (ARMx2_CORE0));
    uart_printf ("   CEVA frequency is %d Mhz (PLL-%d)\r\n", ClkRstGetFreq (CR_DEV_CEVA_0) / 1000000,ClkGetSrcPll (CR_DEV_CEVA_0));
    uart_printf ("     FP frequency is %d Mhz (PLL-%d)\r\n", ClkRstGetFreq (CR_DEV_FP_0) / 1000000, ClkGetSrcPll (CR_DEV_FP_0));
    uart_printf (" FEC-DL frequency is %d Mhz (PLL-%d)\r\n", ClkRstGetFreq (CR_DEV_FEC_DL) / 1000000,ClkGetSrcPll (CR_DEV_FEC_DL));
    uart_printf (" FEC-UL frequency is %d Mhz (PLL-%d)\r\n", ClkRstGetFreq (CR_DEV_FEC_UL) / 1000000, ClkGetSrcPll (CR_DEV_FEC_UL));
    uart_printf ("   PCIe frequency is %d Mhz (PLL-%d)\r\n", ClkRstGetFreq (CR_PCI) / 1000000, ClkGetSrcPll (CR_PCI));
    uart_printf ("\r\n");

    uart_printf ("SYS-AXI frequency is %d Mhz (PLL-%d)\r\n", ClkRstGetFreq (SYS_AXI) / 1000000, ClkGetSrcPll (SYS_AXI));
    uart_printf ("SPU-AXI frequency is %d Mhz (PLL-%d)\r\n", ClkRstGetFreq (SPU_AXI) / 1000000, ClkGetSrcPll (SPU_AXI));
    uart_printf ("RAD-AXI frequency is %d Mhz (PLL-%d)\r\n", ClkRstGetFreq (RAD_AXI) / 1000000, ClkGetSrcPll (RAD_AXI));
    uart_printf (" XP-AXI frequency is %d Mhz (PLL-%d)\r\n", ClkRstGetFreq (XP_AXI ) / 1000000, ClkGetSrcPll (XP_AXI ));
    uart_printf ("==============================\r\n");

    return 0;
}

UINT32 GetDevIDByName (char * pName)
{
    if (!strcmp (pName, "pll0"))
    {
        return CR_PLL_0;
    }
    else if (!strcmp (pName, "pll1"))
    {
        return CR_PLL_1;
    }
    else if (!strcmp (pName, "pll2"))
    {
        return CR_PLL_2;
    }
    else if (!strcmp (pName, "pll3"))
    {
        return CR_PLL_3;
    }
    else if (!strcmp (pName, "pll4"))
    {
        return CR_PLL_4;
    }
    else if (!strcmp (pName, "pll5"))
    {
        return CR_PLL_5;
    }
    else if (!strcmp (pName, "qarm"))
    {
        return ARMx4_CORE0;
    }
    else if (!strcmp (pName, "darm"))
    {
        return ARMx2_CORE0;
    }
    else if (!strcmp (pName, "fp"))
    {
        return CR_DEV_FP_0;
    }
    else if (!strcmp (pName, "ceva"))
    {
        return CR_DEV_CEVA_0;
    }
    else if (!strcmp (pName, "fecdl"))
    {
        return CR_DEV_FEC_DL;
    }
    else if (!strcmp (pName, "fecul"))
    {
        return CR_DEV_FEC_UL;
    }
    else if (!strcmp (pName, "sys"))
    {
        return SYS_AXI;
    }
    else if (!strcmp (pName, "spu"))
    {
        return SPU_AXI;
    }
    else if (!strcmp (pName, "rad"))
    {
        return RAD_AXI;
    }
    else if (!strcmp (pName, "xp"))
    {
        return XP_AXI;
    }

    return 0xFF;
}

UINT32 CLineCmdSetFreq(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 rc = 1;
    IRQTYPE irq;
    UINT32 nXPAxi, Armx4;
    UINT32 nDevID, nDevFreq;
    UINT32 nMapPllID = 0xFF;
    //UINT32 nXpPllID;

    if (pParams->ParamCount < 2)
    {
        uart_printf ("Usage: <devid> <freq in Mhz> [pll-id map to]\r\n");
        return 0;
    }

    nDevID = GetDevIDByName (pParams->ParamList[0]);
    nDevFreq = StrToNum (pParams->ParamList[1]);

    if (pParams->ParamCount >= 3)
        nMapPllID = GetDevIDByName (pParams->ParamList[2]);

    uart_printf ("\r\nSetting [%s] (devid=%d) to %dMhz (mapdev=0x%02x) ... ", pParams->ParamList[0], nDevID, nDevFreq, nMapPllID);

    nXPAxi = ClkRstGetFreq (XP_AXI);
    Armx4  = ClkRstGetFreq (ARMx4_CORE0);

    irq = ARM_INT_disable();

    if (nDevID >= CR_PLL_0 && nDevID <= CR_PLL_5)
    {
        //nXpPllID = 0;

        if (ClkGetSrcPll (XP_AXI) == (nDevID-CR_PLL_0))
        {
            uart_printf ("\r\nXP-AXI clock is prohibited to be changed\r\n");
            ARM_INT_restore(irq);
            return 0;
        }

        rc = ClkRstSetPll(nDevID, nDevFreq);
    }
    else
    {
        if (XP_AXI == nDevID)
        {
            uart_printf ("\r\nXP-AXI clock is prohibited to be changed\r\n");
            ARM_INT_restore(irq);
            return 0;
        }

        rc = ClkRstSetDevFreq(nDevID, nDevFreq, nMapPllID);
    }

    if (nXPAxi != ClkRstGetFreq (XP_AXI))
    {
        //UartDrvReinitFreq ();
    }

    if (Armx4  != ClkRstGetFreq (ARMx4_CORE0))
    {
        uart_printf ("Reconfiguring ARM timer, ARMx4(%dMhz) ...", ClkRstGetFreq (ARMx4_CORE0) / 1000000);

        {
            HANDLE hTimer;
            extern const char LTmrName[];

            rc = MxGetObjByName(CUR_CPU, LTmrName, &hTimer);

            _ASSERT_RC (rc);

            MxConfigDrv(hTimer, TIMER_CMD_SET_PRESCALE, (LPVOID) 0, 0);
            MxConfigDrv(hTimer, TIMER_CMD_SET_PERIOD,(LPVOID) (ClkRstGetFreq (ARMx4_CORE0) / 2000),0);

            if (MxGetARMID () == 0)
            {

    	        MxConfigDrv(hTimer, TIMER_CMD_SET_CTRL,
    	            (LPVOID)(TIMER_CTRL_ENABLE | TIMER_CTRL_AUTO |
    	            TIMER_CTRL_INT_ENABLE), 0);

    		    uart_printf ("LOCAL IRQ");
            }
            else
            {
            	MxConfigDrv(hTimer, TIMER_CMD_SET_CTRL,
            		            (LPVOID)(TIMER_CTRL_ENABLE | TIMER_CTRL_AUTO), 0);
            }
        }

        uart_printf ("OK\r\n");
    }

    ARM_INT_restore(irq);

    if (FAILED(rc))
    {
        uart_printf ("Failed\r\n");
    }
    else
    {
        uart_printf ("OK\r\n");
    }

    return 0;
}

VUINT32 IMsgCount     = 0;
VUINT32 IMsgregStatus = 0;
VUINT32 IMsgCpu       = 0;
VUINT32 IMsgID        = 0;
VUINT32 IMsgSubNum    = 0;
VUINT32 IMsgSize      = 32;
VUINT32 IMsgTestStopped = 0;
HANDLE  IMsgThr;

VUINT32 IMsgTotalTime[CPU_NUM];
VUINT32 IMsgMaxTime[CPU_NUM];
VUINT32 IMsgMinTime[CPU_NUM];
VUINT32 IMsgTimeNum[CPU_NUM];

#define IMSG_LIMIT      10000

void IMsgRegTime (UINT32 t)
{
    UINT32 cpu = MxGetCpuID();
    IMsgTimeNum[cpu]++;

    IMsgTotalTime[cpu] += t;

    IMsgMaxTime[cpu] = MAX(IMsgMaxTime[cpu], t);
    IMsgMinTime[cpu] = MIN(IMsgMinTime[cpu], t);
}

MXRC TestMsgHandler (LPVOID pClientCtx, LPVOID pMsg)
{
    // Pseudo messages
    PUINT8 pTx, pRx, pSdu, pInd;
    MXRC rc = MX_OK;
    UINT32 t;

    if (pMsg != NULL)
        memset (pMsg, MxGetCpuID () ? 0xBB : 0xAA, IMsgSize);

    //uart_printf ("IMsg handler, cpu=%d\r\n", MxGetCpuID ());

    // cpu0 = phy
    if (MxGetCpuID () == 0)
    {
        if (IMsgCount == 0)
        {
            UINT32 i;
            if (IMsgTestStopped == 0)
            {
                uart_printf ("Test stopped\r\n");

                for (i = 0; i < CPU_NUM; i++)
                {
                    if (IMsgTimeNum[i])
                        uart_printf ("CPU-%d, max:%d  avg:%d  min:%d, reg_num=%d\r\n",
                            i, IMsgMaxTime[i], IMsgTotalTime[i] / IMsgTimeNum[i], IMsgMinTime[i], IMsgTimeNum[i]);
                }
            }

            IMsgTestStopped = 1;

            return 0;
        }

        // to generate the request for the first message
        if (IMsgSubNum == 0)
        {
            pInd = (PUINT8)SvsrAllocMsg ();

            if (pInd == NULL)
            {
                uart_printf ("Alloc IMsg error,cpu=%d, Ind=%x\r\n",
                        MxGetCpuID (), pInd);

                IMsgCount = 0;
                return 0;
            }

            memset (pInd, 0xFF, IMsgSize);

            t = GetTIMETICK();
            rc = SvsrSendMsg (IMsgCpu, IMsgID, pInd, SVSR_MSG_OPT_DEALLOC);
            IMsgRegTime (GetTIMETICK() - t);

            if (FAILED(rc))
            {
                uart_printf ("CPU-%d, SvsrSendMsg-error, rc=%x\r\n", MxGetCpuID (), rc);
                _ASSERT_RC (rc);

                IMsgCount = 0;
                return 0;
            }

            IMsgCount--;
        }

        IMsgSubNum ++;
        if (IMsgSubNum >= 3)
            IMsgSubNum = 0;

    }
    // CPU=1 = MAC
    else
    {
        pTx = (PUINT8)SvsrAllocMsg ();

        if (pTx == NULL)
        {
            uart_printf ("Alloc IMsg error,cpu=%d, tx=%x\r\n",
                    MxGetCpuID (), pTx);

            IMsgCount = 0;
            return 0;
        }

        memset (pTx,  0x11, IMsgSize);

        t = GetTIMETICK();
        rc = SvsrSendMsg (0, IMsgID, pTx,  SVSR_MSG_OPT_DEALLOC);
        t = GetTIMETICK() - t;
        IMsgRegTime (t);

        if (t > IMSG_LIMIT)
        {
            uart_printf ("PISEZ#1 (%d)\r\n", t);
            while (1);
        }

        if (FAILED(rc))
        {
            uart_printf ("CPU-%d, SvsrSendMsg tx-error, rc=%x\r\n", MxGetCpuID (), rc);
            _ASSERT_RC (rc);

            IMsgCount = 0;
            return 0;
        }

        pRx = (PUINT8)SvsrAllocMsg ();
        pSdu = (PUINT8)SvsrAllocMsg ();

        if (pTx == NULL || pRx == NULL || pSdu == NULL)
        {
            uart_printf ("Alloc IMsg error,cpu=%d, tx=%x, rx=%x, sdu=%x\r\n",
                    MxGetCpuID (), pTx, pRx, pSdu);

            IMsgCount = 0;
            return 0;
        }

        memset (pRx,  0x22, IMsgSize);
        memset (pSdu, 0x33, IMsgSize);

        t = GetTIMETICK();
        rc = SvsrSendMsg (0, IMsgID, pRx,  SVSR_MSG_OPT_DEALLOC);
        t = GetTIMETICK() - t;
        IMsgRegTime (t);

        if (t > IMSG_LIMIT)
        {
            uart_printf ("PISEZ#2 (%d)\r\n", t);
            while (1);
        }

        if (FAILED(rc))
        {
            uart_printf ("CPU-%d, SvsrSendMsg rx-error, rc=%x\r\n", MxGetCpuID (), rc);
            _ASSERT_RC (rc);

            IMsgCount = 0;
            return 0;
        }

        t = GetTIMETICK();
        rc = SvsrSendMsg (0, IMsgID, pSdu, SVSR_MSG_OPT_DEALLOC);
        t = GetTIMETICK() - t;
        IMsgRegTime (t);

        if (t > IMSG_LIMIT)
        {
            uart_printf ("PISEZ#3 (%d)\r\n", t);
            while (1);
        }

        if (FAILED(rc))
        {
            uart_printf ("CPU-%d, SvsrSendMsg sdu-error, rc=%x\r\n", MxGetCpuID (), rc);
            _ASSERT_RC (rc);

            IMsgCount = 0;
            return 0;
        }
    }

    return 0;
}

void IMsgSendTestMsg (UINT32 nCpu)
{
    PUINT8 pNewMsg = (PUINT8)SvsrAllocMsg ();
    MXRC rc;
    UINT32 t;

    if (pNewMsg == NULL)
    {
        uart_printf ("Send test message error, ptr = NULL\r\n");
        return;
    }

    memset (pNewMsg, 0x33, IMsgSize);

    t = GetTIMETICK();
    rc = SvsrSendMsg (nCpu, IMsgID, pNewMsg, SVSR_MSG_OPT_DEALLOC);
    t = GetTIMETICK() - t;

    IMsgRegTime (t);

    if (rc != MX_OK)
        uart_printf ("Failed to send I-CORE msg, cpu-%d, rc=%x\r\n", MxGetCpuID (), rc);

    //uart_printf ("Send First message, rc=%x\r\n", rc);
}

UINT32 IMsgRegHandler (LPVOID p)
{
    UINT32 rc;

    uart_printf ("Registering IMsg on CPU-%d ...", MxGetCpuID ());
    rc = SvsrRegMsgHandler (IMsgID, TestMsgHandler, NULL);
    uart_printf ("rc=%x\r\n", rc);
    IMsgregStatus = 1;
    return 0;
}

UINT32 ScheduleIMsgTest (LPVOID p)
{
    IMsgSendTestMsg ((UINT32)p);
    return 0;
}

UINT32 CLineCmdCheckIMsg(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    MXRC rc;
    UINT32 nCpu = 2, i;

    IMsgSubNum = 0;
    IMsgCpu = nCpu;
    IMsgregStatus = 0;
    IMsgID = SVSR_CLIENT_NUM - 1;

    for (i=0; i < CPU_NUM; i++)
    {
        IMsgTotalTime[i] = 0;
        IMsgMaxTime[i] = 0;
        IMsgMinTime[i]= (UINT32)-1;
        IMsgTimeNum[i]= 0;
    }

    IMsgTestStopped = 0;
    IMsgCount = 1000000;

    if (FAILED(rc = MxInjectCode (nCpu, IMsgRegHandler, NULL)))
    {
        uart_printf ("Reg client on cpu=%d error\r\n", nCpu);
        return 0;
    }

    uart_printf ("Reg client on CPU-%d is ... ", MxGetCpuID ());
    while (IMsgregStatus == 0);
    uart_printf ("OK\r\n");

    if (FAILED(rc = SvsrRegMsgHandler (IMsgID, TestMsgHandler, NULL)))
    {
        uart_printf ("Reg client on CPU-0 is FAILED, rc=%x\r\n", rc);
    }
    else
    {
        uart_printf ("Reg client on CPU-0 is OK\r\n");
    }

    uart_printf ("Run test\r\n");

    // this code is needed to start test without
    // influence of console manager and to get the 'clean' time

    if (HANDLE_NULL(IMsgThr))
    {
        MxCreateThread (ScheduleIMsgTest, NULL, (LPVOID)(nCpu), 0, &IMsgThr);
    }

    MxScheduleThread (IMsgThr);

    return 0;
}


ASMPROC void GenDAbort (void)
{
    MOV r0, #0x10
    MOV r1, #0x11
    MOV r2, #0x12
    MOV r3, #0x13
    MOV r4, #0x14
    MOV r5, #0x15
    MOV r6, #0x16
    MOV r7, #0x17
    MOV r8, #0x18
    MOV r9, #0x19
    MOV r10,#0x1A
    MOV r11,#0x1B
    MOV r12,#0x1C
    MOV r13,#0x1D
    MOV r14,#0x1E

    STR r0, [r1]

LOOP B LOOP

}

UINT32 CLineCmdGenDAbort(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    GenDAbort ();

    return 0;
}

LPVOID mypartptr[CPU_NUM] = {NULL};

LPVOID PAlloc (HANDLE h)
{
    UINT32 cpu = MxGetCpuID ();


	if(mypartptr[cpu] == NULL)
        mypartptr[cpu] = MxAllocBlock (h);

    return mypartptr[cpu];
}

UINT32 CLineCmdPartAlloc(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    LPVOID pMem, ptr;
    HANDLE hPart;
    UINT32 nSize = 100, i;
    UINT32 nNum = 10;
    UINT32 nTestNum = 2000;
    MXRC rc;
    UINT32 t = 0, avg_allc = 0, max_allc = 0;
    UINT32 avg_free = 0, max_free = 0;

    pMem = MxHeapAlloc (DspHeap, nSize * nNum);

    if (pMem == NULL)
    {
        uart_printf ("Heap alloc - error\r\n");
        return 0;
    }

    rc = MxCreatePart (pMem, nNum, nSize, NULL, HNULL, HNULL, &hPart);

    if (FAILED(rc))
    {
        uart_printf ("Creare partition error\r\n");
        MxHeapFree (DspHeap, pMem);
        return 0;
    }

    rc = MxShareObject(hPart, OBJ_SHARE_ON);

    if (FAILED(rc))
    {
        uart_printf ("Share partition error\r\n");
        MxRemovePart (hPart);
        MxHeapFree (DspHeap, pMem);
        return 0;
    }

    for (i = 0; i < nTestNum; i++)
    {
        t = GetTIMETICK ();
        ptr = MxAllocBlock (hPart);
        //ptr = PAlloc(hPart);
        t = GetTIMETICK () - t;

        if (avg_allc == 0)
            avg_allc = t;
        else
            avg_allc = (t + avg_allc) / 2;

        max_allc = MAX (max_allc, t);

        t = GetTIMETICK ();
        MxFreeBlock (hPart, ptr);
        t = GetTIMETICK () - t;

        if (avg_free == 0)
            avg_free = t;
        else
            avg_free = (t + avg_free) / 2;

        avg_free = MAX (avg_free, t);
    }

    MxRemovePart (hPart);
    MxHeapFree (DspHeap, pMem);

    uart_printf ("Alloc avg: %d, alloc max : %d\r\n", avg_allc, max_allc);
    uart_printf ("Free  avg: %d, free  max : %d\r\n", avg_free, max_free);

    return 0;
}

#if SPI_TEST_EPROM
void Eprom_ReadID(AT25ID * IdInfo)
{
    EPROM_CS_LOW;
    SPISendByte(cmd_ID);

    SPISendReadByte(DUMMY_DATA, &IdInfo->ManufacturerID);
    SPISendReadByte(DUMMY_DATA, &IdInfo->DeviceID1);
    SPISendReadByte(DUMMY_DATA, &IdInfo->DeviceID2);
    EPROM_CS_HIGH;
}

void Eprom_ReadStatus(UINT8 * status)
{
    EPROM_CS_LOW;
    SPISendByte(cmd_RD_STATUS);
    SPISendReadByte(DUMMY_DATA, status);
    EPROM_CS_HIGH;
}

void Eprom_Wait(void)
{
    UINT8 status;
    do
    {
        Eprom_ReadStatus(&status);
    }
    while(status & BUSY);
}

void Eprom_WriteEnable(void)
{
    EPROM_CS_LOW;
    SPISendByte(cmd_WR_ENABLE);
    EPROM_CS_HIGH;

    Eprom_Wait();
}

void Eprom_WriteStatus(UINT8 Data)
{
    Eprom_WriteEnable();

    EPROM_CS_LOW;
    SPISendByte(cmd_WD_STATUS);
    SPISendByte(Data);
    EPROM_CS_HIGH;

    Eprom_Wait();
}

void Eprom_ReadData(UINT8 *buf,UINT32 StartAdd ,UINT32 Length)
{
    UINT32 i;
    UINT8 Addr0,Addr1,Addr2;

    Addr0 = (UINT8)StartAdd;
    Addr1 = (UINT8)(StartAdd >> 8);
    Addr2 = (UINT8)(StartAdd >> 16);

    Eprom_Wait();

    EPROM_CS_LOW;
    SPISendByte(cmd_RD_DATA);

    SPISendByte(Addr2);                    //send address MSB first
    SPISendByte(Addr1);
    SPISendByte(Addr0);

    for (i=0;i < Length; i++)            //read data
        SPISendReadByte(DUMMY_DATA, &buf[i]);

    EPROM_CS_HIGH;
}

void Eprom_PageProgram(UINT8 *buf, UINT32 StartAddr, UINT16 Length)
{
    UINT32 i;
    UINT8 Addr0,Addr1,Addr2;

    Addr0 = (UINT8)StartAddr;
    Addr1 = (UINT8)(StartAddr >> 8);
    Addr2 = (UINT8)(StartAddr >> 16);

    Eprom_WriteEnable();

    EPROM_CS_LOW;
    SPISendByte(cmd_PAGE_PROGRAM);
    SPISendByte(Addr2);
    SPISendByte(Addr1);
    SPISendByte(Addr0);                   //if Addr0 != 0x0, special circustance apply

    for (i=0;i < Length; i++)            //if length > 256, only the last 256 bytes will latch into internal buffer
        SPISendByte(buf[i]);

    EPROM_CS_HIGH;

    Eprom_Wait();

}

UINT32 Eprom_SectorErase(UINT8 SectorNum)
{
    UINT32 rc = MX_OK;
    UINT32 StartAddr = SectorNum * AT25DF161_SECTOR_SIZE;
    UINT8 Addr0,Addr1,Addr2;

    Addr0 = (UINT8)StartAddr;
    Addr1 = (UINT8)(StartAddr >> 8);
    Addr2 = (UINT8)(StartAddr >> 16);

    Eprom_WriteEnable();

    EPROM_CS_LOW;
    SPISendByte(cmd_SECTOR_ERASE);

    SPISendByte(Addr2);
    SPISendByte(Addr1);
    SPISendByte(Addr0);
    EPROM_CS_HIGH;

    Eprom_Wait();

    return rc;
}

void Eprom_GlobalUnProtect(void)
{
    UINT8 status;
    Eprom_ReadStatus(&status);
    Eprom_WriteStatus(status & (~PROTECT_BITS));
}
#endif

UINT32 CLineCmdTestSpi(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    SPICONFIG * pSpiConfig;
    MXRC rc = MX_OK;

    // Init/Reinit the driver for the 10 devices

    if (!SPIIsInit())
    {
        if (FAILED(rc = SPIInit(10)))
        {
            uart_printf ("SPI drv init error, rc=%x\r\n", rc);
            return 0;
        }
    }

    pSpiConfig = SPICreateDevCtx ();

    if (pSpiConfig == NULL)
    {
        uart_printf ("Cannot allocate the device context\r\n");
        return 0;
    }

    pSpiConfig->frameSize = SPI_FRAME_SIZE_8BITS;
    pSpiConfig->frameFormat = SPI_MOTOROLA_FORMAT;
    pSpiConfig->clkMode = SPI_CLOCK_MODE3;
    pSpiConfig->transferMode = SPI_RX_TX_ENABLE;
    pSpiConfig->frqDivider = SPI_DIVIDER_SPEED_1M;

    pSpiConfig->DevID = SPI_DEV_EPROM;
    pSpiConfig->CS = SPI_EPROM_CS;

    //allowing signals transmit through buffer
    REG32(GPIO_PIN_SELECT_REG) |= (1<<9);
    REG32(GPIO_OE_REG) |= (1<<9);
    REG32(GPIO_OUTPUT_REG) &= ~(1<<9);

    SPISelectDevice(pSpiConfig);

#if SPI_TEST_EPROM
    //use GPIO to control device due to the cs timing requirement
    REG32(GPIO_PIN_SELECT_REG) |= (1<<EEPROM_CS_GPIO);
    REG32(GPIO_OE_REG) |= (1<<EEPROM_CS_GPIO);
    REG32(GPIO_OUTPUT_REG) |= (1<<EEPROM_CS_GPIO);

    {
        UINT8  buf[64];
        UINT16 i;
        AT25ID IdInfo;

        uart_printf("\r\nSPI Eprom Test...");

        Eprom_ReadID(&IdInfo);
        if ((IdInfo.ManufacturerID != ST25P_MANUFACTURER_ID)
            || (IdInfo.DeviceID1 != ST25P16_TYPE_ID)
            || (IdInfo.DeviceID2 != ST25P16_CAPACITY_ID))
        {
            uart_printf("\r\nUnknown SPI Eprom ID: man=%X Dev1=%X Dev2=%X\r\n",
                IdInfo.ManufacturerID, IdInfo.DeviceID1, IdInfo.DeviceID2);
        }
        Eprom_GlobalUnProtect();
        Eprom_SectorErase(0);
        memset(buf,0x5A,64);
        Eprom_PageProgram(buf,0x0,64);

        memset(buf,0x0,64);
        Eprom_ReadData(buf,0x0,64);

        for (i = 0;i < 64;i ++)
        {
            if (buf[i] != 0x5A)
            {
                uart_printf(" FAILED !\r\n");
                return 1;
            }
        }

        uart_printf(" PASSED !\r\n");
    }
#endif

    SPISelectDevice(NULL);
    SPIRemoveDevCtx(pSpiConfig);

    return 0;
}

UINT32 CLineCmdTestPcie(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    PCIECFG cfg;
    UINT32 rc, val;
    PCIEREGS cfg_regs;
    PCIELOCREGS lm_regs;
    PCIERCREGS rc_regs;

    memset (&cfg, 0, sizeof(cfg));

    cfg.BaseMem = CRAM_BASEADDR;
    cfg.Mode    = PCIE_DRV_OPT_RC;
    cfg.Lanes   = 1; // x1
    cfg.IrqCB   = NULL;
    cfg.pIrqData= NULL;

    if (b->ParamCount > 0)
        cfg.Mode = StrToNum (b->ParamList[0]);

    if (cfg.Mode != PCIE_DRV_OPT_EP)
        uart_printf ("\nPCIe (RC) init ... ");
    else
        uart_printf ("\nPCIe (EP) init ... ");

    if (b->ParamCount > 1)
    {
        cfg.SerDesID = StrToNum (b->ParamList[1]);
        if((cfg.SerDesID != 0) && (cfg.SerDesID != 2))
        {
            uart_printf ("\nInvalid SerDes config %d\n", cfg.SerDesID);
            return 0;
        }
        else
        {
            uart_printf ("SerDes %d ... ", cfg.SerDesID);
        }
    }

    if (b->ParamCount > 2)
    {
        cfg.Lanes = StrToNum (b->ParamList[2]);
        if((cfg.Lanes != 1) && (cfg.Lanes != 2) && (cfg.Lanes != 4))
        {
            uart_printf ("\nInvalid Lanes config %d\n", cfg.Lanes);
            return 0;
        }
        else
        {
            uart_printf ("%d lanes ... ", cfg.Lanes);
        }
    }

    rc = PCIeDrvInit (&cfg);

    if (FAILED(rc))
    {
        uart_printf ("failed, rc=%x\r\n", rc);
        return 0;
    }
    else
    {
        uart_printf ("OK\r\n");
    }

    if (cfg.Mode != PCIE_DRV_OPT_EP)
    {
        uart_printf ("Connecting ... ");

        rc = PCIeDrvConnect (PCIE_DRV_CONNECT_TIMEOUT);

        // Just for debug purpose
        //PCIeDrvReadLocMgtRegs (&lm_regs);
        //PCIeDrvReadRCConfRegs (&rc_regs);
        uart_printf ("Link Status = 0x%x\n", rc_regs.LinkCtrlStatReg);

        if (FAILED(rc))
        {
            uart_printf ("failed, rc=%x\r\n", rc);
            return 0;
        }
        else
        {
            uart_printf ("OK\r\n");
        }

        // Just for debug purpose
        val = REG32(RAD_CFG_PCIE_STATUS);

        if ((val & 3) == PCIE_LINK_NOT_DETECTED)
            uart_printf ("Link status: No receivers detected\r\n");
        if ((val & 3) == PCIE_LINK_TRAINING)
            uart_printf ("Link status: Link training in progress\r\n");
        if ((val & 3) == PCIE_LINK_DL_INIT)
            uart_printf ("Link status: Link UP, DL initialization ... \r\n");
        if ((val & 3) == PCIE_LINK_DL_COMPLETE)
            uart_printf ("Link status: Link UP, DL initialization is OK\r\n");

        if (val & (1 << 2))
            uart_printf ("Speed 5.0 GB\r\n");
        else
            uart_printf ("Speed 2.5 GB\r\n");

        if (val & (1 << 3))
            uart_printf ("IO space Enabled\r\n");

        if (val & (1 << 4))
            uart_printf ("MemSpace Enabled\r\n");

        if (val & (1 << 5))
            uart_printf ("Bus Master Enabled\r\n");

        if (val & (1 << 6))
            uart_printf ("INTx Disabled\r\n");

        // More debug
        val = lm_regs.PhyLayer0;

        switch((val >> 1) & 7)
        {
                case 0:
                        uart_printf ("Lane count negotiated: x1\r\n");
                        break;
                case 1:
                        uart_printf ("Lane count negotiated: x2\r\n");
                        break;
                case 2:
                        uart_printf ("Lane count negotiated: x4\r\n");
                        break;
                case 3:
                        uart_printf ("Lane count negotiated: x8\r\n");
                        break;
                case 4:
                        uart_printf ("Lane count negotiated: x16\r\n");
                        break;
        }

        if(b->ParamCount > 3)
        {// Run enumeration
            //PCIeDrvScan();
        }
    }

    return RC_CM_OK;
}

UINT32 CLineCmdReadPcie (CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
	UINT32   addr, pcieaddr, val, count = 1;

	if ((b->ParamCount == 0) || (b->ParamCount > 2))
		return RC_CM_PARAM_ERROR;
	if (b->ParamCount > 0)
		addr = StrToNum(b->ParamList [0]);
	if (b->ParamCount > 1)
		count = StrToNum(b->ParamList [1]);

	while(count--)
	{
		pcieaddr = PCIeDrvReadMemory(addr, &val, 1);
		printf ("Reading...[0x%08x / 0x%08x] = 0x%x\r\n", addr, pcieaddr, val);
		addr+=sizeof(UINT32);
	}

	return RC_CM_OK;
}

UINT32 CLineCmdWritePcie (CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
	UINT32   addr, pcieaddr, val;

	if (b->ParamCount != 2)
		return RC_CM_PARAM_ERROR;

	addr = StrToNum(b->ParamList [0]) & ~3;
	val  = StrToNum(b->ParamList [1]);

	pcieaddr = PCIeDrvWriteMemory(addr, &val, 1);

	printf ("Writing...[0x%08x / 0x%08x] = 0x%x\r\n", addr, pcieaddr, val);

	return RC_CM_OK;
}

UINT32 CLineCmdDebugPcie (CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
	UINT32   val = 0;

	if (b->ParamCount > 0)
		val  = StrToNum(b->ParamList [0]);

	PCIeDrvDebug(val);

	return RC_CM_OK;
}

void PrintLogHeader(UINT32 index, API_LOG_INFO * pHeader, UINT32 dt)
{
    uart_printf ("========= Index-%d (dt=%d)=========\r\n", index, dt);

    uart_printf (" - PayloadLen : %d\r\n", pHeader->PayloadLen);
    uart_printf (" - Timestamp  : %d\r\n", pHeader->Timestamp);
    uart_printf (" - Message    : %d\r\n", pHeader->Message);
    uart_printf (" - Source     : %d\r\n", pHeader->Source);
    uart_printf (" - Destination: %d\r\n", pHeader->Destination);
    uart_printf (" - PHY_ID     : %d\r\n", pHeader->PHY_ID);
    uart_printf (" - MAC_ID     : %d\r\n", pHeader->MAC_ID);
}

UINT32 CLineCmdTestApiLogger(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    UINT32 i, nNum = SvsrLoggerGetMsgNum ();
    API_LOG_INFO * pHeader;
    UINT32 nMax = 0, begin = 0;
    UINT32 ts = 0;

    if (b->ParamCount > 0)
        begin  = StrToNum (b->ParamList[0]);

    if (b->ParamCount > 1)
        nMax  = StrToNum (b->ParamList[1]);

    uart_printf ("Msg num is: %d\r\n", nNum);
    uart_printf ("  Flags is: 0x%08x\r\n", SvsrLoggerGetFlag());
    uart_printf ("      Mask: 0x%08x\r\n", logger_ctx.EnabledMask);

    if (nMax != 0)
        nNum = nMax;

    for (i = begin; i < nNum; i++)
    {
        pHeader = SvsrLoggerGetMsg (i);

        if (pHeader != NULL)
        {
            PrintLogHeader (i, pHeader, pHeader->Timestamp - ts);

            ts = pHeader->Timestamp;
        }
    }

    return 0;
}

static HANDLE hICPUThr;
static UINT32 systicknum = 0;        //number of system ticks which should be reached for to send API command
static UINT32 divider;               //the divider for setting the frequency of API command sending
static UINT32 apicmdnum = 0;         //number of API commands which should be sent to U-ARM
static UINT32 apicntr = 0;           //counter of API commands
static UINT32 prevtick = 0;          //number of previous system ticks
ApiHeader * TestApi = NULL;          // API message
PUINT8 TestBuf [128];
#define TEST_ICPU_INST   40             // !!!!!!!!!!!  MAC has to use this ID in DST INSTANCE (so, it = 0x8028,  0x28 =40!!! )
UINT32 IcpuRecvApiNum;               // counter of received commands

static APIHEC ICpuProcHandler (LPVOID pCtx,  ApiHeader * pApi, ApiHeader * pResp)
{
    UINT32 nParamNum = 0;
    ApiParam * pParam = NULL;

     IcpuRecvApiNum ++;

     if ((IcpuRecvApiNum%100) == 0)
         uart_printf("Message received: %d.\n", IcpuRecvApiNum);

    // Just the read the API command
    // and calculate number of API parameters
     while ((pParam = ApiGetNextParam(pApi, pParam)) != NULL)
     {
        nParamNum ++;
     }

    // Just for test , to add one temporary parameer and send responce to the MAC
    // like one additional API command
     ApiAddParam(pResp, NULL, 2, &nParamNum, sizeof (nParamNum));

    return APIHEC_OK;
}

static UINT32 APICmdThread (LPVOID p)
{
    UINT32 interval;
    MXRC rc;

//    uart_printf("Thread is called first time.\n");
    if (prevtick == 0)
    {
//        uart_printf("Thread is called first time.\n");
        prevtick = SysTimerGetTick();
        rc = MxScheduleThread (MxGetCurrentThread ());
        if (FAILED(rc))
            printf("Cannot create next thread.\n");
        return 0;
    }
//    uart_printf("Thread is called repeatedly.\n");

    interval = SysTimerGetTick() - prevtick;
    prevtick = SysTimerGetTick();

    if (SysTimerGetTick() >= systicknum)
    {
        systicknum = SysTimerGetTick() + divider*interval;
        rc = SvsrSendAPI(TestApi);
        if (FAILED(rc))
        {
            uart_printf ("Send API error, rc-%x, already sent=%d/%d\r\n", rc, apicntr, apicmdnum);
        }
        apicntr += 1;
//        printf("API message sent: %d.\n", apicntr);
    }

    if (apicntr < apicmdnum)
    {
        rc = MxScheduleThread (MxGetCurrentThread ());
        if (FAILED(rc))
            printf("Cannot create next thread.\n");
        return 0;
    }

    MxRemoveThread (hICPUThr);
    uart_printf("The test ended. Sent API commands: %d.\n", apicntr);

    return 0;
}

UINT32 CLineCmdTestICore(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    UINT32 nCore = 0;

    uart_printf("\r\n");
    for (nCore = 0; nCore < SVSR_ICORE_NUM; nCore++)
         uart_printf("Part[%d]: %d Free parts\r\n", nCore, MxGetFreePartBlocks(ICorePartListCfg[nCore].PartHandle));

    return 0;
}

UINT32 CLineCmdTestICPU(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    UINT32 nCore = 0;
    MXRC rc;
    UINT32 tmp1;

    divider = (UINT32)StrToNum(b->ParamList[0]);
    if (divider == 0)
        divider = 1;
    apicmdnum = (UINT32)StrToNum(b->ParamList[1]);
    prevtick  = 0;
    systicknum = 0;
    apicntr = 0;
//    SvsrRegClient (IID_API_ICPUTST, APIIcpuParser, NULL);
    IcpuRecvApiNum = 0;

    memset (TestBuf, 0, sizeof (TestBuf));

    TestApi = ApiCreate (TestBuf, API_DEF_FLAGS,
                         TEST_ICPU_INST,
                         1,                        // The supervisor ID
                         API_TYPE_CFG_PARAM,
                         0xF90A);                    // Message ID = 0xF90A,  change it if needed

    // Just add couple temporary parameters (just for test)
    tmp1 = 100;
    ApiAddParam (TestApi, NULL, 2, &tmp1, sizeof (tmp1));

    tmp1 = 200;
    ApiAddParam (TestApi, NULL, 3, &tmp1, sizeof (tmp1));

//    rc = MxCreateThread (APICmdThread, NULL, NULL, 0, &hThr);
    rc = MxCreateThreadEx (nCore, APICmdThread, NULL, 0, 0, &hICPUThr);
    if (FAILED(rc))
    {
        uart_printf("Cannot create thread.\n");
        return 0;
    }

    rc = MxScheduleThread(hICPUThr);
    if (FAILED(rc))
    {
        MxRemoveThread(hICPUThr);
        uart_printf("Cannot schedule thread.\n");
        return 0;
    }

    SvsrRegClient (TEST_ICPU_INST, ICpuProcHandler, NULL);

    printf("Parameters: %d, %d.\n", divider, apicmdnum);

//    while (apicntr < apicmdnum);

//    MxRemoveThread(hICPUThr);

    return 0;
}

void EvmRxCb (void * pCtx, UINT32 nChID, PUINT8 pRxData, UINT32 nSize)
{
    //uart_printf ("Rx:%x, %x\r\n", pCtx, pRxData);
}

void EvmTxCb (void * pCtx, UINT32 nChID, PUINT8 pTxData, UINT32 nSize)
{
    //uart_printf ("Tx:%x, %x\r\n", pCtx, pTxData);
}

//__align (128) UINT32 NCNBVAR radio_rx_log [61440];

UINT32 CLineCmdTestEvmFpga(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    EvmCfg cfg;
    MXRC rc;

    memset (&cfg, 0, sizeof (cfg));

    cfg.pRxDMACallBack  = EvmRxCb;
    cfg.pTxDMACallBack  = EvmTxCb;

    cfg.CtrlMask        |= EVM_FPGA_CTRL_AUTO_RUN;
    //cfg.CtrlMask      |= EVM_FPGA_CTRL_CHECK;
    //cfg.CtrlMask      |= EVM_FPGA_CTRL_TX_LOG;
    //cfg.CtrlMask      |= EVM_FPGA_CTRL_RX_LOG;
    //cfg.CtrlMask      |= EVM_FPGA_CTRL_RX_LOG_RR;
    //cfg.CtrlMask      |= EVM_FPGA_CTRL_TX_LOG_RR;

    //cfg.pTxLog      = (PUINT8)tx_log_iq_samples;
    //cfg.TxLogSize   = iq_samples_size * sizeof (tx_log_iq_samples[0]);

    //cfg.pRxLog      = (PUINT8)radio_rx_log;
    //cfg.RxLogSize   = sizeof (radio_rx_log);

    //memset (radio_rx_log, 0xC5, sizeof (radio_rx_log));

    if (b->ParamCount)
        cfg.CtrlMask = StrToNum(b->ParamList[0]);

    uart_printf ("EvmFpga initialization with[0x%x] ", cfg.CtrlMask);

    rc = EvmFpgaInit(&cfg);

    if (FAILED(rc))
    {
        uart_printf ("failed, rc=0x%x\r\n", rc);
        return 0;
    }

    uart_printf ("OK\r\n");

    return 0;
}

UINT32 CLineCmdStopEvmFpga(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    EvmFpgaStopRadio();
    uart_printf ("It's stopped\r\n");
    return 0;
}


UINT32 CLineCmdAd9361GetRSSI(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
	MXRC rc;
	UINT32 ch = 0;

	if(b->ParamCount == 0)
	{
		uart_printf("need to set command: rssi, freq, reinit\r\n");
		return 0;
	}

	if(_strcmp(b->ParamList[0], "rssi") == 0)
		uart_printf("Ad9361 RSSI for channal %d is -%d ", ch, Ad9361GetRSSI(CMgrGetParam(b, 1, 0)));
	else if(_strcmp(b->ParamList[0], "freq") == 0)
	{
		if ((rc = Ad9361SetFreq((double)CMgrGetParam(b, 1, 0), (double)CMgrGetParam(b, 2, 0))) != MX_OK)
        	uart_printf ("error,rc=%x\r\n", rc);
		else
			uart_printf ("Set frequency rx freq %d, tx freq %d", CMgrGetParam(b, 1, 0), CMgrGetParam(b, 2, 0));
	}
	else if(_strcmp(b->ParamList[0], "reinit") == 0)
		if ((rc = Ad9361RadioInit (7, 1, 1, 0, (double)CMgrGetParam(b, 1, 0), (double)CMgrGetParam(b, 2, 0))) != MX_OK)
        	uart_printf ("error,rc=%x\r\n", rc);
		else
			uart_printf ("reinit with rx freq %d, tx freq %d", CMgrGetParam(b, 1, 0), CMgrGetParam(b, 2, 0));
	else
		uart_printf("unknown command %s\r\n",  b->ParamList[1]);

	return 0;
}


UINT32 CLineCmdTestAd9361(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    MXRC rc;
    UINT32 profID = 0;
    UINT32 tx_ann = 1;
    UINT32 rx_ann = 1;
    UINT32 dejitt = 0;
    UINT32 loopb  = 0;
	UINT32 rxfreq = 0;
	UINT32 txfreq = 0;

    if (b->ParamCount == 0)
    {
        uart_printf ("\r\nUsage: adi <band-id> [tx-antenna-num:1] [rx-antenna-num:1] [loopback:0] [dejitter-init:0] [rxfreq:profdef] [txfreq:profdef]");
        return MX_OK;
    }

    profID = CMgrGetParam(b, 0, 7);
    tx_ann = CMgrGetParam(b, 1, 1);
    rx_ann = CMgrGetParam(b, 2, 1);
    loopb  = CMgrGetParam(b, 3, 0);
    dejitt = CMgrGetParam(b, 4, 0);
    rxfreq = CMgrGetParam(b, 5, 0);
    txfreq = CMgrGetParam(b, 6, 0);

    if (dejitt != 0)
    {
        if (FAILED(rc = DejitterInit(DEJITTER_INIT_RADIO, AD9548_CLK_SRC_REFD)))
        {
            uart_printf ("error,rc=%x\r\n", rc);
        }
        else
        {
            uart_printf ("OK\r\n");
        }
    }

    uart_printf ("\r\nAD9361 initialization: band=%d, tx-ann=%d, rx-ann=%d [%s]...\r\n", profID, tx_ann, rx_ann, loopb ? "loopback" : "no loopback");

    if (FAILED(rc = Ad9361RadioInit (profID, tx_ann, rx_ann, loopb, rxfreq, txfreq)))
    {
        uart_printf ("error,rc=%x\r\n", rc);
    }
    else
    {
        uart_printf ("OK\r\n");
    }

    return 0;
}

UINT32 CLineCmdApiLogReset(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    uart_printf ("Reset API logger storage (set with zerro) ...");
    SvsrLoggerResetStorage(1);
    uart_printf ("OK\r\n");

    return 0;
}

UINT32 CLineCmdApiLogInfo(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    SvsrLoggerPrintInfo();
    return 0;
}


UINT32 CLineCmdApiLogReload(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 nCache = 0;
    MXRC rc;

    if (pParams->ParamCount > 0)
        nCache = StrToNum(pParams->ParamList[0]);

    if (nCache)
        uart_printf ("Invalidate & reload API logger storage (for Linux)...");
    else
        uart_printf ("Reload API logger storage (for HBI) ...");

    rc = SvsrLoggerRestore(nCache);

    if (FAILED(rc))
        uart_printf ("FAIL, rc=%x\r\n", rc);
    else
        uart_printf ("OK\r\n");

    return 0;
}

UINT32 CLineCmdApiLogSetMask(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{

    UINT32 nMask = 0;

    if (pParams->ParamCount > 0)
    {
        nMask = StrToNum(pParams->ParamList[0]);

        uart_printf ("LOG mask = 0x%08x\r\n", nMask);
        SvsrLoggerSetMask(nMask);
    }
    else
    {
        uart_printf("Mask Bits are as folows:\r\n");
        uart_printf("PHY_TXSTART_REQ:   1\r\n");
        uart_printf("PHY_TXSTART_CONF:  2\r\n");
        uart_printf("PHY_TXSTART_IND:   3\r\n");
        uart_printf("PHY_TXSDU_REQ:     4\r\n");
        uart_printf("PHY_TXSDU_CONF:    5\r\n");
        uart_printf("PHY_TXEND_IND:     6\r\n");
        uart_printf("PHY_RXSTART_REQ:   7\r\n");
        uart_printf("PHY_RXSTART_CONF:  8\r\n");
        uart_printf("PHY_RXSTART_IND:   9\r\n");
        uart_printf("PHY_RXSDU_IND:     10\r\n");
        uart_printf("PHY_RXEND_IND:     11\r\n");
        uart_printf("PHY_INIT_REQ:      12\r\n");
        uart_printf("PHY_INIT_IND:      13\r\n");
        uart_printf("PHY_RXSTATUS_IND:  14\r\n");
        uart_printf("PHY_RECONFIG_REQ:  15\r\n");
        uart_printf("PHY_RECONFIG_CNF:  16\r\n");
        uart_printf("PHY_START_REQ:     17\r\n");
        uart_printf("PHY_START_CONF:    18\r\n");
        uart_printf("PHY_STOP_REQ:      19\r\n");
        uart_printf("PHY_STOP_CONF:     20\r\n");
        uart_printf("PHY_STOP_IND:      21\r\n");
        uart_printf("PHY_TXHIADCIUL_REQ:  22\r\n");
        uart_printf("PHY_TXHISDU_REQ: 23\r\n");
        uart_printf("PHY_TXDCIULSDU_REQ: 24\r\n");
        uart_printf("\r\n");
        uart_printf("Current Mask Value: 0x%08x\r\n\n", SvsrLoggerGetMask());
        uart_printf("Use %X value to log all useful TX/RX messages\r\n", SVSR_API_LOGGER_DEF_MASK_ENA);
    }

    return 0;
}

UINT32 CLineCmdApiLogSetMaskNmm(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{

    UINT32 nMask = 0;

    if (pParams->ParamCount > 0)
    {
        nMask = StrToNum(pParams->ParamList[0]);

        uart_printf ("NMM LOG mask = 0x%08x\r\n", nMask);
        SvsrLoggerSetMaskNmm(nMask);
    }
    else
    {
        uart_printf("Mask Bits are as folows:\n");
        uart_printf("NMM_START:                   0\n");
        uart_printf("NMM_START_RESP:              0\n");
        uart_printf("NMM_STOP:                    1\n");
        uart_printf("NMM_STOP_RESP:               1\n");
        uart_printf("NMM_RSSI_MEAS_REQ:           2\n");
        uart_printf("NMM_RSSI_MEAS_REQ_RESP:      2\n");
        uart_printf("NMM_RSSI_MEAS_IND:           3\n");
        uart_printf("NMM_RSSI_MEAS_CNF:           4\n");
        uart_printf("NMM_STOP_RSSI_MEAS_REQ:      5\n");
        uart_printf("NMM_STOP_RSSI_MEAS_REQ_RESP: 5\n");
        uart_printf("NMM_CELL_SEARCH_REQ:         6\n");
        uart_printf("NMM_CELL_SEARCH_REQ_RESP:    6\n");
        uart_printf("NMM_CELL_SEARCH_IND:         7\n");
        uart_printf("NMM_CELL_SEARCH_CNF:         8\n");
        uart_printf("NMM_STOP_CELL_SEARCH_REQ:    9\n");
        uart_printf("NMM_STOP_CELL_SEARCH_REQ_RESP: 9\n");
        uart_printf("NMM_PBCH_CONFIG_REQ:         10\n");
        uart_printf("NMM_PBCH_CONFIG_CNF:         11\n");
        uart_printf("NMM_PBCH_DATA_IND:           12\n");
        uart_printf("NMM_STOP_PBCH_REQ:           13\n");
        uart_printf("NMM_STOP_PBCH_REQ_RESP:      13\n");
        uart_printf("NMM_SIB1_CONFIG_REQ:         14\n");
        uart_printf("NMM_SIB1_CONFIG_CNF:         15\n");
        uart_printf("NMM_BCCH_CONFIG_REQ:         16\n");
        uart_printf("NMM_BCCH_CONFIG_CNF:         17\n");
        uart_printf("NMM_BCCH_DATA_IND:           18\n");
        uart_printf("NMM_STOP_BCCH_REQ:           19\n");
        uart_printf("\n");
        uart_printf("Current Mask Value: 0x%08x\r\n\n", SvsrLoggerGetMaskNmm());
    }

    return 0;
}


UINT32 CLineCmdApiLogFlush(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    MXRC rc;

    uart_printf ("LOG flushing ... ");

    if (FAILED(rc = SvsrLoggerFlush()))
    {
        uart_printf ("Failed, rc=%x\r\n", rc);
    }
    else
    {
        uart_printf ("OK\r\n");
    }

    return 0;
}



#ifdef MLOG_ENABLED
// Mlog Commands
//--------------


UINT32 CLineCmdMLogReset(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    MLOGCTX *info;

    uart_printf ("Getting MLog info ...");
    info = MLogGetCtx();
    if (info == NULL)
    {
        uart_printf ("Failed\r\n");
        return 0;
    }
    uart_printf ("OK\r\n");


    if (info->Opened)
    {
        uart_printf("MLog Opened. Closing... ");
        if (MLogClose() != MLOG_TRUE)
        {
            uart_printf("Error!!!\r\n");
            return 0;
        }
        else
            uart_printf("OK\r\n");
    }
    uart_printf("MLog Closed. Opening... ");

    if (MLogOpen() != MLOG_TRUE)
    {
        uart_printf("Error!!!\r\n");
        return 0;
    }
    else
        uart_printf("OK\r\n");

    CLineCmdMLogInfo(NULL, NULL);

    return 0;
}

UINT32 CLineCmdMLogInfo(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    MLOGCTX *info;

    uart_printf ("Getting MLog info ...");

    info = MLogGetCtx();

    if (info == NULL)
    {
        uart_printf ("Failed\r\n");
        return 0;
    }

    uart_printf ("OK\r\n");

    uart_printf ("              MLOG opened: %s\r\n",    (info->Opened ? "YES" : "NO"));

    if (info->Opened)
    {
        uart_printf ("           MLOG finalized: %s\r\n",    (info->Finalized ? "YES" : "NO"));
        uart_printf ("                MLOG Mask: 0x%08x\r\n", info->EnabledMask);
        uart_printf ("     MLOG Logging Enabled: %s\r\n",     (info->Started ? "YES" : "NO"));
        uart_printf ("             MLOG storage: 0x%08x\r\n", info->StoragePtr->ptr);
        uart_printf ("        MLOG storage size: %d\r\n",     info->StorageSize);
        uart_printf ("   MLOG Number of buffers: %d\r\n",     MLOG_BUFFERS_COUNT);
    }
    else
        uart_printf ("Use \"mlogreset\" to open the Log\r\n");

    return 0;
}

UINT32 CLineCmdMLogSetMask(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 nMask = 0;

    if (pParams->ParamCount > 0)
    {
        nMask = StrToNum(pParams->ParamList[0]);

        uart_printf ("MLog mask = 0x%08x\r\n", nMask);
        MLogSetMask(nMask);
        MLogSetMaskProg(nMask);
    }
    else
    {
        uart_printf("Mask Bits are as folows:\n");
        uart_printf(" RESOURCE_MAP        2\n");
        uart_printf(" RESOURCE_CEVA       3\n");

        uart_printf(" RESOURCE_FECUL      7\n");
        uart_printf(" RESOURCE_FECDL      8\n");

        uart_printf(" RESOURCE_XP_AXI     9\n");
        uart_printf(" RESOURCE_SYS_AXI    10\n");
        uart_printf(" RESOURCE_SPU_AXI    11\n");
        uart_printf(" RESOURCE_RAD_AXI    12\n");

        uart_printf(" RESOURCE_LARM0      1\n");
        uart_printf(" RESOURCE_LARM1      13\n");
        uart_printf(" RESOURCE_UARM0      14\n");
        uart_printf(" RESOURCE_UARM1      15\n");
        uart_printf(" RESOURCE_UARM2      16\n");
        uart_printf(" RESOURCE_UARM3      17\n");
        uart_printf("\n");
        uart_printf("Current Mask Value: 0x%08x\r\n\n", MLogGetMask());
    }

    return 0;
}

UINT32 CLineCmdMLogFlush(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    MXRC rc = 0;

    uart_printf ("MLOG flushing ... \r\n");

    if (MLogFlush() != MLOG_TRUE)
        uart_printf ("Failed, rc=%x\r\n", rc);
    else
        uart_printf ("OK\r\n");

    return 0;
}

UINT32 CLineCmdMLogClose(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    MLOGCTX *info;

    uart_printf ("Getting MLog info ...");
    info = MLogGetCtx();
    if (info == NULL)
    {
        uart_printf ("Failed\r\n");
        return 0;
    }
    uart_printf ("OK\r\n");


    if (info->Opened)
    {
        uart_printf("MLog Opened. Closing... ");
        if (MLogClose() != MLOG_TRUE)
        {
            uart_printf("Error!!!\r\n");
            return 0;
        }
        else
            uart_printf("OK\r\n");
    }
    else
        uart_printf("MLog Already Closed\r\n");

    return 0;
}


UINT32 CLineCmdMLogFinalize(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    MLOGCTX *info;

    uart_printf ("Getting MLog info ...");
    info = MLogGetCtx();
    if (info == NULL)
    {
        uart_printf ("Failed\r\n");
        return 0;
    }
    uart_printf ("OK\r\n");


    if (info->Finalized == MLOG_FALSE && info->Opened == MLOG_TRUE)
    {
        uart_printf("MLog Opened. Finalizing... \r\n");
        if (MLogPrint() != MLOG_TRUE)
        {
            uart_printf("Error!!!\r\n");
            return 0;
        }
        else
            uart_printf("OK\r\n");
    }
    else
        uart_printf("Nothing to Finalize\r\n");

    return 0;

}


#endif











UINT32 CLineCmdApiInvMem(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 nMem, nSize;
    IRQTYPE irq;

    if (pParams->ParamCount < 2)
    {
        uart_printf ("Params error, please use <addr> <size>\r\n");
        return 0;
    }

    nMem = StrToNum (pParams->ParamList[0]);
    nSize= StrToNum (pParams->ParamList[1]);

    irq = ARM_INT_disable();

    uart_printf ("Invalidate memory [%x] of [%d] ... ", nMem, nSize);
    MxCacheInvalidate((LPVOID)nMem, nSize);
    uart_printf ("OK\r\n");

    ARM_INT_restore(irq);

    return 0;
}

UINT32 CLineCmdApiCleanMem(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 nMem, nSize;
    IRQTYPE irq;

    if (pParams->ParamCount < 2)
    {
        uart_printf ("Params error, please use <addr> <size>\r\n");
        return 0;
    }

    nMem = StrToNum (pParams->ParamList[0]);
    nSize= StrToNum (pParams->ParamList[1]);

    irq = ARM_INT_disable();

    uart_printf ("Cleaning memory [%x] of [%d] ... ", nMem, nSize);
    MxCacheClean((LPVOID)nMem, nSize);
    uart_printf ("OK\r\n");

    ARM_INT_restore(irq);

    return 0;
}

LPVOID ApiLogTmpStor = NULL;
UINT32 ApiLogTmpSize = 0;

UINT32 CLineCmdApiLogCtrl(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 nCtrl = 0;

    if (pParams->ParamCount > 0)
        nCtrl = StrToNum (pParams->ParamList[0]);

    if (nCtrl == 0)
    {
        if (ApiLogTmpStor == NULL)
        {
            ApiLogTmpStor = (LPVOID)MxHeapAlloc (NcnbHeap, logger_ctx.StorageSize);

            if (ApiLogTmpStor == NULL)
            {
                uart_printf ("No memory in heap to presave API log\r\n");
                return 0;
            }

            ApiLogTmpSize = logger_ctx.StorageSize;
        }

        uart_printf ("Saving the log [%x] [%d] ... ", ApiLogTmpStor, ApiLogTmpSize);
        memcpy (ApiLogTmpStor, logger_ctx.StoragePtr->hdr, ApiLogTmpSize);
        uart_printf ("OK\r\n");
    }
    else if (nCtrl == 1)
    {
        if (ApiLogTmpStor == NULL)
        {
            uart_printf ("Log is not presaved\r\n");
        }
        else
        {
            uart_printf ("Restoring the log [%x] [%d] ... ", logger_ctx.StoragePtr->hdr, ApiLogTmpSize);
            memcpy (logger_ctx.StoragePtr->hdr, ApiLogTmpStor, ApiLogTmpSize);
            uart_printf ("OK\r\n");
        }
    }
    else if (nCtrl == 2)
    {
        if (ApiLogTmpStor != NULL)
            MxHeapFree (NcnbHeap, ApiLogTmpStor);

        ApiLogTmpStor = NULL;
        ApiLogTmpSize = 0;

        uart_printf ("Presaved log storage is free\r\n");
    }
    else
        uart_printf ("Unknown parameters\r\n");

    return 0;
}

UINT32 CLineCmdCheckHeapAlloc(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 nHeapNum, i, nSize = 128;
    HANDLE h;
    char * pName;

    nHeapNum = MxGetObjCount (OBJ_TYPE_HEAP);

    if (pParams->ParamCount > 0)
        nSize = StrToNum (pParams->ParamList[0]);

    uart_printf ("Tested size is %d bytes\r\n", nSize);

    for (i = 0; i < nHeapNum; i++)
    {
        h = MxGetObjByIndex (OBJ_TYPE_HEAP, i);

        if (HANDLE_TOINT(h) == HANDLE_TOINT(AppHeap) && HANDLE_NOT_NULL(h))
        {
            pName = "AppHeap";
        }
        else if (HANDLE_TOINT(h) == HANDLE_TOINT(DspHeap) && HANDLE_NOT_NULL(h))
        {
            pName = "DspHeap";
        }
        else if (HANDLE_TOINT(h) == HANDLE_TOINT(NcnbHeap) && HANDLE_NOT_NULL(h))
        {
            pName = "NcnbHeap";
        }
        else if (HANDLE_TOINT(h) == HANDLE_TOINT(IramHeap) && HANDLE_NOT_NULL(h))
        {
            pName = "IramHeap";
        }
        else if (HANDLE_TOINT(h) == HANDLE_TOINT(DDR1Heap) && HANDLE_NOT_NULL(h))
        {
            pName = "DDR1Heap";
        }
        else if (HANDLE_TOINT(h) == HANDLE_TOINT(DDR1NcnbHeap) && HANDLE_NOT_NULL(h))
        {
            pName = "DDR1NcnbHeap";
        }
        else
        {
            pName = "DEFCORE-HEAP";
        }

        uart_printf (" - %s [h=0x%08x]: ", pName, HANDLE_TOINT(h));

        {
            LPVOID p = MxHeapAlloc (h, nSize);

            if (p != NULL)
            {
                uart_printf ("Allocation is: 0x%08x\r\n", p);
                MxHeapFree (h, p);
            }
            else
            {
                uart_printf ("No free space\r\n");
            }
        }
    }


    return 0;
}

UINT32 CLineCmdCheckMxTicks(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 nTicks = (150000*1000 * 5); // 5 seconds
    UINT32 nBegin;

    if (pParams->ParamCount > 0)
        nTicks = StrToNum (pParams->ParamList[0]);

    uart_printf ("Delay for %d ... ", nTicks);

    nBegin = GetTIMETICK();

    MxDelayTicks(nTicks);

    nBegin = GetTIMETICK() - nBegin;

    uart_printf ("OK, time=%d\r\n", nBegin);

    return 0;
}

UINT32 CLineTestDDR1(CMgrCmdCtx * pCtx, CMgrCmdParamCtx * pParams)
{
    unsigned int* p_block1;
    unsigned int* p_block2;
    unsigned int* p_block3;
    unsigned int* p_next;
    unsigned int counter = 0;
    unsigned int wrong = 0;
    unsigned int block_size = 512*1024 -32;
    HANDLE Heaps = DDR1Heap;

    uart_printf ("starting DDR1 memory alloc...\n");
    uart_printf ("allocating and filling the arrays in the memory...\n");
    uart_printf ("Base DDR1 heap descriptor: 0x%X\n", *((unsigned int*)(&Heaps)));

    p_block1 = (unsigned int*)MxHeapAlloc(Heaps, block_size);
    if(p_block1 == NULL)
    {
        uart_printf("There is no free space (%d) in DDR1 heap or heap isn't created\r\n", block_size);
        return 0;
    }
    p_next = p_block1;
    uart_printf("Block 1 allocated. Address: 0x%X\n", (unsigned int)p_block1);
    while((unsigned int)p_next < ((unsigned int)p_block1 + block_size))
    {
        if (!(counter % 2048))
        {
            uart_printf ("written value %d to address 0x%X\n", counter, (unsigned int)p_next);
            MxDelay(100);
        }
        *p_next = counter;
        counter += 1;
        p_next += 1;
    }
    uart_printf("Block 1 filled.\n");

    p_block2 = (unsigned int*)MxHeapAlloc(Heaps, block_size);
    if(p_block2 == NULL)
    {
        uart_printf("There is no free space (%d) in DDR1 heap\r\n", block_size);
        return 0;
    }
    p_next = p_block2;
    uart_printf("Block 2 allocated. Address: 0x%X\n", (unsigned int)p_block2);
    while((unsigned int)p_next < ((unsigned int)p_block2 + block_size))
    {
        if (!(counter % 2048))
        {
            uart_printf ("written value %d to address 0x%X\n", counter, (unsigned int)p_next);
            MxDelay(100);
        }
        *p_next = counter;
        counter += 1;
        p_next += 1;
    }

    uart_printf("Block 2 filled.\n");

    p_block3 = (unsigned int*)MxHeapAlloc(Heaps, block_size);
    if(p_block3 == NULL)
    {
        uart_printf("There is no free space (%d) in DDR1 heap\r\n", block_size);
        return 0;
    }
    p_next = p_block3;
    uart_printf("Block 3 allocated. Address: 0x%X\n", (unsigned int)p_block3);
    while((unsigned int)p_next < ((unsigned int)p_block3 + block_size))
    {
        if (!(counter % 2048))
        {
            uart_printf ("written value %d to address 0x%X\n", counter, (unsigned int)p_next);
            MxDelay(100);
        }
        *p_next = counter;
        counter += 1;
        p_next += 1;
    }
    //    p_block3 = (unsigned int*)((unsigned int)p_block3 - 64*1024);
    uart_printf("Block 3 filled.\n");

    uart_printf ("checking results...\n");

    counter = 0;

    p_next = p_block1;
    while((unsigned int)p_next < ((unsigned int)p_block1 + block_size))
    {
        if (!(counter % 2048))
        {
            uart_printf ("read value %d from address 0x%X\n", *p_next, (unsigned int)p_next);
            MxDelay(100);
        }
        if (*p_next != counter)
        {
            wrong = 1;
            break;
        }
        counter += 1;
        p_next += 1;
    }
    if (wrong != 0)
    {
        uart_printf("wrong result in block1 (Addr=%X: %d != %d ref)\n", p_next, *p_next, counter);
        wrong = 0;
    }
    uart_printf("Block 1 checked.\n");
    MxDelay(100);
    counter = block_size / 4;

    p_next = p_block2;
    while((unsigned int)p_next < ((unsigned int)p_block2 + block_size))
    {
        if (!(counter % 2048))
        {
            uart_printf ("read value %d from address 0x%X\n", *p_next, (unsigned int)p_next);
            MxDelay(100);
        }
        if (*p_next != counter)
        {
            wrong = 1;
            break;
        }
        counter += 1;
        p_next += 1;
    }
    if (wrong != 0)
    {
        uart_printf("wrong result in block2 (Addr=%X: %d != %d ref)\n", p_next, *p_next, counter);
        wrong = 0;
    }
    uart_printf("Block 2 checked.\n");
    MxDelay(100);
    counter = 2 * (block_size / 4);

    p_next = p_block3;
    while((unsigned int)p_next < ((unsigned int)p_block3 + block_size))
    {
        if (!(counter % 2048))
        {
            uart_printf ("read value %d from address 0x%X\n", *p_next, (unsigned int)p_next);
            MxDelay(100);
        }
        if (*p_next != counter)
        {
            wrong = 1;
            break;
        }
        counter += 1;
        p_next += 1;
    }
    if (wrong != 0)
    {
        uart_printf("wrong result in block3 (Addr=%X: %d != %d ref)\n", p_next, *p_next, counter);
        wrong = 0;
    }
    uart_printf("Block 3 checked.\n");
    MxDelay(100);
    counter = 3 * (block_size / 4);

    MxHeapFree(Heaps, p_block1);
    MxHeapFree(Heaps, p_block2);
    MxHeapFree(Heaps, p_block3);

    printf("DDR1 test completed.\n");

    return 0;
}

extern PUINT8 MemCmp(PUINT8 p1, PUINT8 p2, UINT32 length);

UINT32 CLineCmdLogMgr(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    LPVOID pRef;
    LOGHANDLE hLog;
    UINT32 nSize = 10*1024, i;
    MXRC rc;
    UINT32 st1, st2;
    PUINT8 p;

    pRef = MxHeapAlloc (NcnbHeap, nSize);

    if (pRef == NULL)
    {
        uart_printf ("LOG storage ref allocation error\r\n");
        return 0;
    }

    MdmaSysInit ();

    LogMgrInit ();

    uart_printf ("Cur LOGs num - %d\r\n", LogMgrGetLogNum());

    rc = LogMgrCreateLog (LOG_CTRL_SYS_MDMA, NcnbHeap, nSize, &hLog);

    if (FAILED(rc))
    {
        MxHeapFree (NcnbHeap, pRef);
        uart_printf ("Create LOG error, rc-0x%08x\r\n", rc);
        return 0;
    }

    uart_printf ("Create LOG OK\r\n");
    uart_printf ("LOGs num - %d\r\n", LogMgrGetLogNum());
    uart_printf ("Ref filling ...");

    for (i = 0; i < nSize; i++)
    {
        ((PUINT8)pRef)[i] = (UINT8)(i & 0xFF);
    }

    uart_printf ("OK\r\n");

    st1 = GetTIMETICK();
    rc = LogMgrWriteData (hLog, pRef, nSize / 2);
    st1 = GetTIMETICK() - st1;

    if (FAILED(rc))
    {
        uart_printf ("LOG: write data error, rc=%x\r\n", rc);

        MxHeapFree (NcnbHeap, pRef);
        LogMgrCloseLog (hLog);
        return 0;
    }

    st2 = GetTIMETICK();
    rc = LogMgrWriteData (hLog, (PUINT8)pRef + nSize / 2, nSize / 2);
    st2 = GetTIMETICK() - st2;

    if (FAILED(rc))
    {
        uart_printf ("LOG: write data error, rc=%x\r\n", rc);

        MxHeapFree (NcnbHeap, pRef);
        LogMgrCloseLog (hLog);
        return 0;
    }

    uart_printf ("Avg1 - %d ticks\r\n", st1);
    uart_printf ("Avg2 - %d ticks\r\n", st2);

    uart_printf ("Start testing ... ");

    p = MemCmp (pRef, LogMgrGetLogStorage (hLog), nSize);

    if (p != NULL)
    {
        UINT32 nOffs = (UINT32)p - (UINT32)pRef;
        uart_printf ("error, offs = %d, [0x%02x] != [0x%02x]\r\n", nOffs, *p, ((PUINT8)LogMgrGetLogStorage (hLog))[nOffs]);
    }
    else
    {
        uart_printf ("OK\r\n");
    }

    uart_printf ("Free ref storage ... ");
    MxHeapFree (NcnbHeap, pRef);
    uart_printf ("OK\r\nFree LOG ...");
    LogMgrCloseLog (hLog);
    uart_printf ("OK\r\n");

    return 0;
}

VUINT32 Irq1PrRes = 0;
VUINT32 Irq2PrRes = 0;

static void TestPrior0IRQ(volatile UINT32 * pAckReg, UINT32 nIrqID, LPVOID pParam)
{
    Irq1PrRes += Irq2PrRes + 1;
    uart_printf ("IRQ ID %d.\n", nIrqID);
    *pAckReg = nIrqID;
}

static void TestPrior1IRQ(volatile UINT32 * pAckReg, UINT32 nIrqID, LPVOID pParam)
{
    Irq2PrRes += Irq1PrRes + 2;
    uart_printf ("IRQ ID %d.\n", nIrqID);
    *pAckReg = nIrqID;
}

UINT32 CLineCmdIrqPrior(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    UINT32 CpuID = MxGetCpuID();
    IRQTYPE irq;
    UINT32 irqID1 = 6;
    UINT32 irqPrior1 = (1);
    UINT32 irqID2 = 7;
    UINT32 irqPrior2 = (2);

    Irq1PrRes = 0;
    Irq2PrRes = 0;

    irq = ARM_INT_disable();

    MxSetIntHandler(irqID1, INT_TYPE_IRQ, irqPrior1, TestPrior0IRQ, NULL);
    MxSetIntHandler(irqID2, INT_TYPE_IRQ, irqPrior2, TestPrior1IRQ, NULL);

    MxEnableInt(irqID2);
    MxEnableInt(irqID1);

    ICDrvRaiseInterCpu(CpuID, irqID1);
    ICDrvRaiseInterCpu(CpuID, irqID2);

    uart_printf ("Raised IRQs and to check results\r\n");

    ARM_INT_restore(irq);

    if (Irq1PrRes == 1 && Irq2PrRes == 3)
    {
        uart_printf ("Results is OK\r\n");
    }
    else
    {
        uart_printf ("Results is FAILED (%d %d)\r\n", Irq1PrRes, Irq2PrRes);
    }

    return 0;
}

UINT32 CLineCmdIQPhoneInit(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    IQPhoneInitData cfg;
    MXRC rc;

    memset (&cfg, 0, sizeof (cfg));

    cfg.nMode    = IQPHONE_CTRL_TX_ENABLE | IQPHONE_CTRL_RX_ENABLE;
    cfg.RxLogLim = 0xffffffff;
    cfg.TxLogLim = 0xffffffff;
    cfg.RxLogSize= 61440 * 300; // 18Mb for RX log in DDR1
    cfg.TxLogSize= 61440 * 300; // 18Mb for TX log in DDR1

    uart_printf ("IQ player/recorder initialization ...");

    rc = IQPhoneInitCpri (&cfg);

    if (FAILED(rc))
    {
        uart_printf ("FAILED, rc=0x%x\r\n", rc);
    }
    else
    {
        uart_printf ("OK\r\n");
    }

    return 0;
}

UINT32 CLineCmdIQPhoneStart(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    IQPhoneInitData cfg;
    UINT32 nMode=IQPHONE_CTRL_TX_ENABLE | IQPHONE_CTRL_RX_ENABLE;
    MXRC rc;

    memset (&cfg, 0, sizeof (cfg));

    if (pParams->ParamCount > 0)
    {
        nMode = StrToNum (pParams->ParamList[0]);

        nMode = ((nMode & 1) ? IQPHONE_CTRL_TX_ENABLE:0)|((nMode & 2) ? IQPHONE_CTRL_RX_ENABLE:0);
    }

    uart_printf ("LOG-Mode: TX-%d, RX-%d\r\n", nMode & IQPHONE_CTRL_TX_ENABLE ? 1:0, nMode & IQPHONE_CTRL_RX_ENABLE ? 1:0);

    cfg.nMode    = nMode;
    cfg.RxLogLim = 80;
    cfg.TxLogLim = 80;

    uart_printf ("Starting IQ player/recorder ...");

    rc = IQPhoneStart(IQPHONE_COM_CPRI, &cfg);

    if (FAILED(rc))
    {
        uart_printf ("FAILED, rc=0x%x\r\n", rc);
    }
    else
    {
        uart_printf ("OK\r\n");
    }

    return 0;
}

void ScanHeap (HANDLE hHeap)
{
    PMEMBLOCK pBlock;
    UINT32 bFirst = 1;

    uart_printf ("\r\n======= HEAP scanning =========\r\n");

    while (SUCCESSFUL(MxHeapScan (hHeap, bFirst, &pBlock)))
    {
        bFirst = 0;
        uart_printf (" - Block: [0x%08x], size: [%d]\r\n", (UINT32)pBlock, pBlock->Size);
    }

    uart_printf ("\r\n===============================\r\n");
}

UINT32 CLineCmdHeapAPICheck(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    // We are using new heap, to prevent demaging of already used

    HANDLE hHeap;

    LPVOID pStor;
    UINT32 nStorSize = 1024;
    LPVOID pAt, pRange, pRange1;

    MXRC rc;

    pStor = MxHeapAlloc (AppHeap, nStorSize);

    uart_printf ("Heap storage : 0x%08x\r\n", pStor);

    if (pStor == NULL)
    {
        uart_printf ("Storage is NULL\r\n");
        return 0;
    }

    rc = MxCreateHeap (pStor, nStorSize, 32, NULL, &hHeap);

    if (FAILED(rc))
    {
        uart_printf ("Create heap - error, rc=%xr\n", rc);
        MxHeapFree(AppHeap, pStor);
        return 0;
    }

    uart_printf ("Create heap - ok\r\n");

    ScanHeap (hHeap);

    uart_printf ("allocate at(0x%08x) ... ", (UINT32)pStor + 32);
    pAt    = MxHeapAllocAt (hHeap, (UINT32)pStor + 32, 100);
    uart_printf ("OK\r\n");

    ScanHeap (hHeap);

    uart_printf ("allocate in range (0x%08x ... 0x%08x) ... ", (UINT32)pStor + 512, (UINT32)pStor + 1024);
    pRange = MxHeapAllocInRange(hHeap, (UINT32)pStor + 512, (UINT32)pStor + 1024, 100);
    uart_printf ("OK\r\n");

    ScanHeap (hHeap);

    uart_printf ("allocate in range (0x%08x ... 0x%08x) ... ", (UINT32)pStor + 512, (UINT32)pStor + 1024);
    pRange1 = MxHeapAllocInRange(hHeap, (UINT32)pStor + 512, (UINT32)pStor + 1024, 100);
    uart_printf ("OK\r\n");

    uart_printf ("pAt = 0x%08x,  pRange=0x%08x,  pRange1=0x%08x\r\n", pAt, pRange, pRange1);

    ScanHeap (hHeap);

    MxHeapFree (hHeap, pAt);
    MxHeapFree (hHeap, pRange);
    MxHeapFree (hHeap, pRange1);

    MxRemoveHeap(hHeap);
    MxHeapFree (AppHeap, pStor);
    pStor = NULL;

    uart_printf ("The test is completed\r\n");

    return 0;
}


UINT32 TestTmrCount = 0;

void TestTmrDrvProc (void * pdata)
{
    TestTmrCount ++;
    uart_printf (" - (0x%02x) tick, data=%x\r\n", MxGetCpuMode(), (UINT32)pdata);
}

UINT32 CLineCmdCheckTmr(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    TestTmrCount = 0;

    uart_printf ("Begin test ... ");

    TimerDrvSetProc (1000, 0, TestTmrDrvProc, (LPVOID)0x12345678);

    // to wait 3 seconds
    MxDelayTicks (3 * 150 * 1000000);

    TimerDrvSetProc (0, 0, NULL, NULL);

    if (TestTmrCount == 0)
        uart_printf ("failed\r\n");
    else
        uart_printf ("passed (%d)\r\n", TestTmrCount);

    return 0;
}

extern const char LTmrName[];
UINT32 CLineCmdCheckTmrWDTimer(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    HANDLE hTimer;
    MXRC rc = MX_OK;
    UINT32 nArmFrq;
    UINT32 nPer = 500;
    //UINT32 ResetCpuId;

    if (pParams->ParamCount > 0)
    {
        nPer = StrToNum (pParams->ParamList[0]);
    }

    TestTmrCount = 0;

    rc = MxGetObjByName(CUR_CPU, LTmrName, &hTimer);

    if (FAILED(rc))
    {
        uart_printf ("MxGetObjByName failed, rc=%x\r\n", rc);
    }

    _ASSERT_RC (rc);

    uart_printf("1. Check watchdog working as usual timer\r\n");

    if (MxGetARMID () == 0)
        nArmFrq = ClkRstGetFreq (ARMx4_CORE0);
    else
        nArmFrq = ClkRstGetFreq (ARMx2_CORE0);

    MxConfigDrv(hTimer, TIMER_WD_CMD_SET_PRESCALE, (LPVOID) 0, 0);
    MxConfigDrv(hTimer, TIMER_WD_CMD_SET_PERIOD,(LPVOID) (nArmFrq / 1000) ,0); // actually 2ms per tick

    if (MxGetARMID () == 0)
    {

        MxConfigDrv(hTimer, TIMER_WD_CMD_SET_CTRL,
            (LPVOID)(TIMER_CTRL_ENABLE | TIMER_CTRL_AUTO | TIMER_CTRL_INT_ENABLE), 0);
    }
    else
    {
        MxConfigDrv(hTimer, TIMER_WD_CMD_SET_CTRL,
            (LPVOID)(TIMER_CTRL_ENABLE | TIMER_CTRL_AUTO), 0);
    }

    uart_printf ("Begin test ... ");

    TimerDrvWDSetProc (nPer, TestTmrDrvProc, (LPVOID)0x12345678);

    // to wait 5 seconds
    MxDelayTicks (5 * 150 * 1000000);

    TimerDrvWDSetProc (0, NULL, NULL);

    if (TestTmrCount == 0)
        uart_printf ("failed\r\n");
    else
        uart_printf ("passed (%d)\r\n", TestTmrCount);

    MxConfigDrv(hTimer, TIMER_WD_CMD_SET_CTRL, (LPVOID) 0, 0);

    return 0;
}

HANDLE hWDThread;


UINT32 wdproc(LPVOID data)
{
	HANDLE hTimer;
	UINT32 nArmFrq;
	MXRC rc = MX_OK;
	UINT32 ResetCpuId;

	ResetCpuId = MxGetCpuID();//(MxGetARMID() == 0)? 3 : 1;

	rc = MxGetObjByName(ResetCpuId, LTmrName, &hTimer);

    if (FAILED(rc))
    {
        uart_printf ("MxGetObjByName is failed, rc=%x\r\n", rc);
    }
//	uart_printf ("sheduling... %d\r\n", ResetCpuId);
	//MxConfigDrv(hTimer, TIMER_WD_CMD_RESTART, (LPVOID) 0, 0);

	if (MxGetARMID () == 0)
		nArmFrq = ClkRstGetFreq (ARMx4_CORE0);
	else
		nArmFrq = ClkRstGetFreq (ARMx2_CORE0);

    MxConfigDrv(hTimer, TIMER_WD_CMD_SET_PRESCALE, (LPVOID) 0, 0);
	MxConfigDrv(hTimer, TIMER_WD_CMD_SET_PERIOD,(LPVOID) (nArmFrq) ,0);

    MxConfigDrv(hTimer, TIMER_WD_CMD_SET_CTRL,
            (LPVOID)(TIMER_CTRL_ENABLE | TIMER_CTRL_AUTO | TIMER_WD_CTRL_WDMODE), 0);
	MxScheduleThread(hWDThread);

	return 0;
}


UINT32 CLineCmdCheckTmrWDWatchdog(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    HANDLE hTimer;
	//HANDLE cb_evnt;
	//HANDLE cb_timer;
    MXRC rc = MX_OK;
    UINT32 nArmFrq;
    UINT32 ResetCpuId;
    //VUINT32 reg = 0;


	MxCreateThread(wdproc,NULL,NULL,0,&hWDThread);
	MxScheduleThread(hWDThread);

    if (MxGetARMID () == 0)
        nArmFrq = ClkRstGetFreq (ARMx4_CORE0);
    else
        nArmFrq = ClkRstGetFreq (ARMx2_CORE0);


    ResetCpuId = MxGetCpuID();//(MxGetARMID() == 0)? 3 : 1;

    rc = MxGetObjByName(ResetCpuId, LTmrName, &hTimer);

    if (FAILED(rc))
    {
        uart_printf ("MxGetObjByName is failed, rc=%x\r\n", rc);
    }

    _ASSERT_RC (rc);

    uart_printf("2. Check watchdog working as watchdog timer (our cpuID=%d)\r\n", MxGetCpuID());

    MxConfigDrv(hTimer, TIMER_WD_CMD_SET_PRESCALE, (LPVOID) 0, 0);
    MxConfigDrv(hTimer, TIMER_WD_CMD_SET_PERIOD,(LPVOID) (nArmFrq) ,0);

    MxConfigDrv(hTimer, TIMER_WD_CMD_SET_CTRL,
            (LPVOID)(TIMER_CTRL_ENABLE | TIMER_CTRL_AUTO | TIMER_WD_CTRL_WDMODE), 0);

    uart_printf ("Begin test...\r\n");
    uart_printf ("[generate WDRESETREQ by cpuID %d in 1 second]...\r\n", ResetCpuId);

    // this is unreachable actually
    uart_printf ("Done\r\n");

    return 0;
}

UINT32 CLineCmdCheckTmrWDWatchdog2(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    HANDLE hTimer;
    MXRC rc = MX_OK;
    UINT32 ResetCpuId;
	UINT32 reset_reg;


    ResetCpuId = MxGetCpuID();//(MxGetARMID() == 0)? 3 : 1;

    rc = MxGetObjByName(ResetCpuId, LTmrName, &hTimer);

    if (FAILED(rc))
    {
        uart_printf ("MxGetObjByName is failed, rc=%x\r\n", rc);
    }

    _ASSERT_RC (rc);

	reset_reg = TimerDrvWDGetWDReset();
	uart_printf("reset_reg %d\r\n", reset_reg);

	TimerDrvWDSetWDReset(1);

	reset_reg = TimerDrvWDGetWDReset();
	uart_printf("reset_reg %d\r\n", reset_reg);

	TimerDrvWDSetWDReset(0);

	reset_reg = TimerDrvWDGetWDReset();
	uart_printf("reset_reg %d\r\n", reset_reg);

	SysTimersShutdown();


//	MxConfigDrv(hTimer, TIMER_WD_CMD_CLEAR_WDRESET, (LPVOID) 0, 0);


    return 0;
}



UINT32 CLineCmdDi(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 nTestMode = 0;

    if (pParams->ParamCount > 0)
    {
        nTestMode = StrToNum (pParams->ParamList[0]);
    }

    DiTest (nTestMode);

    return 0;
}

UINT32 CLineCmdCpriGpio(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    if (pParams->ParamCount > 0)
    {
        CPRICONFIG CpriCfg;

        memset(&CpriCfg, 0, sizeof(CpriCfg));

        CpriCfg.TxIrqGpioEna = StrToNum (pParams->ParamList[0]);

        if(pParams->ParamCount > 2)
            CpriCfg.RxIrqGpioEna = StrToNum (pParams->ParamList[2]);

        if(CpriCfg.TxIrqGpioEna)
        {
            if(pParams->ParamCount > 1)
               CpriCfg.TxIrqGpioPort = StrToNum (pParams->ParamList[1]);

            uart_printf("CPRIGPIO: Enable on TxIrq - EnaMask=%d, Port=%d\r\n",
                CpriCfg.TxIrqGpioEna, CpriCfg.TxIrqGpioPort);
        }
        else
        {
            uart_printf("CPRIGPIO: Disable on TxIrq\r\n");
        }

        if(CpriCfg.RxIrqGpioEna)
        {
            if(pParams->ParamCount > 3)
                CpriCfg.RxIrqGpioPort = StrToNum (pParams->ParamList[3]);
            uart_printf("CPRIGPIO: Enable on RxIrq - EnaMask=%d, Port=%d\r\n",
                CpriCfg.RxIrqGpioEna, CpriCfg.RxIrqGpioPort);
        }
        else
        {
            uart_printf("CPRIGPIO: Disable on RxIrq\r\n");
        }

        CPRIDrvCfgProc(&CpriCfg);
    }
    else
    {
        uart_printf("USAGE: ""cprigpio <'txIRQ' ena> ['txIRQ' port] ['rxIRQ' ena] ['rxIRQ' port]");
    }
    return 0;
}

#ifdef DIAG_SPACC_ENABLED

volatile static UINT32 spaccRunTesting = 1;
static          UINT32 s_mode = 0, s_nR = 1, s_len = 0, s_isAsynchMode = 0, s_cpuMask = 1, s_isFullTest = 0;
HANDLE hThreadCpu[CPU_NUM];

static UINT32 SpaccRunTestThread(LPVOID pData)
{
    MXRC rc = 0;
    UINT32 avrSpeed = 0, maxSpeed = 0, ticks = 0, len = s_len;
    char cmd[64], rslt[64];

    while (spaccRunTesting);

    WaitPrint();

        uart_printf("%s\r\n", (s_isAsynchMode == 1)?"\r\nAsynchrone mode is enabled\r\n" : "");

        if (s_isFullTest)
        {
            int k,j;
            extern UINT32 SPAccDiagGetNumTestSets(UINT32 mode);
            extern UINT32 SPAccDiagGetNumModes(void);


            for (k = 0; k < SPAccDiagGetNumModes(); ++k)
                for (j = 0; j < SPAccDiagGetNumTestSets(k); ++j)
                {
                    ticks = 0;
                    len = j;
                    rc = SPAccDiagTest(k, s_nR, &len, &avrSpeed, &maxSpeed, &ticks, cmd, s_isAsynchMode, rslt);
                    uart_printf("%16s %5d %5d %5d %5d %11d %3d %5s\r\n",cmd,avrSpeed, maxSpeed,s_nR, len, ticks,MxGetCpuID(), rslt);
                }

        }else
        {

            rc = SPAccDiagTest(s_mode, s_nR, &len, &avrSpeed, &maxSpeed, &ticks, cmd, s_isAsynchMode, rslt);
            uart_printf("%16s %5d %5d %5d %5d %11d %3d %5s\r\n",cmd,avrSpeed, maxSpeed, s_nR, len, ticks, MxGetCpuID(), rslt);
         }

        MxRemoveThread(hThreadCpu[MxGetCpuID()]);

   return rc;
}


UINT32 CLineCmdTestSPAcc(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *b)
{
    MXRC rc = 0;
    UINT32 *pValue, tVal;
    UINT32 paramCount = 0, i =0;

    pValue = &tVal;

    s_nR = 1;
    s_cpuMask = 1;
    s_isAsynchMode = 0;
    s_isFullTest = 0;
    s_mode = 0;
    s_len = 0;

    paramCount = b->ParamCount;

    if (paramCount > 0)
    {
        for (i = 0; i < paramCount; ++i)
        {
            if (b->ParamList[i][0] == '-')
            {
                switch (b->ParamList[i][1])
                {
                     case 'h':
                        SPAccDiagPrintHelp();
                        return 0;

                     case 'm':
                        pValue = &s_mode;
                        break;

                     case 'r':
                         pValue = &s_nR;
                         break;

                     case 's':
                         pValue = &s_len;
                         break;

                      case 'a':
                        s_isAsynchMode = 1;
                        break;

                      case 'f':
                        s_isFullTest = 1;
                        break;

                      case 'c':
                        pValue = &s_cpuMask;
                        break;

                    default:
                        uart_printf("\nError format, please read help\n");
                        SPAccDiagPrintHelp();
                    return 0;
                }
            }
            else
            {
                *pValue = StrToNum (b->ParamList [i]);
                pValue = &tVal;
            }

        }
    }
    else
    {
        SPAccDiagPrintHelp();
        return 0;
    }

    for (i = 0; i < CPU_NUM; i++)
        if ( (1 << i) & s_cpuMask)
    {
            rc = MxCreateThreadEx(i,SpaccRunTestThread, NULL,NULL,0,&hThreadCpu[i]);
            _ASSERT_RC(rc);

            rc = MxScheduleThread(hThreadCpu[i]);
            _ASSERT_RC(rc);
     }

   // Run testing
   WaitPrint();
   uart_printf("Spacc Driver Version: %s\r\n",SPACC_DRIVER_VERSION);
   uart_printf("\r\n%13c cmd %1c avg %1c max %1c rep %1c len %4c ticks cpu result\r\n", ' ', ' ', ' ',' ', ' ', ' ');
   spaccRunTesting = 0;

    return rc;
}
#endif

UINT32 CLineCmdGenLog(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    MXRC rc;
    UINT32 nPat = 1, i;
    PUINT8 pData;
    PGenLogRecType pRec;
    UINT32 nPos = 0;
    UINT32 nMax = 0;
    UINT32 nTick = 0, nAvg = 0;
    HANDLE hHeap = NcnbHeap;
    UINT32 nTestSize = 1024;

    rc = MdmaRadInit ();

    if (FAILED(rc))
    {
        uart_printf ("RAD MDMA init error, rc=0x%x\r\n", rc);
        return 0;
    }

    uart_printf ("RAD MDMA init OK\r\n");
    uart_printf ("Resetting log ... ");
    GenLogReset ();
    uart_printf ("OK\r\n");
    uart_printf ("Enable mask (1) ... ");

    GenLogEnableMsg (1, 1);
    uart_printf ("OK\r\n");

    pData = (PUINT8)MxHeapAlloc (hHeap, nTestSize);

    if (pData == NULL)
    {
        uart_printf ("Alloc test buffer error\r\n");
        return 0;
    }

    uart_printf ("Alloc buffer OK\r\n");

    for (i = 0; i < 20; i++)
    {
        memset (pData, nPat + i, nTestSize);

        nTick = GetTIMETICK();
        rc = GenLogAddData (1, 0xFA, pData, nTestSize);
        nTick = GetTIMETICK() - nTick;

        if (FAILED(rc))
        {
            uart_printf ("GenLogAddData - error, rc=0x%x\r\n", rc);
            MxHeapFree (hHeap, pData);
            return 0;
        }

        nMax = MAX(nTick, nMax);

        if (nAvg == 0)
            nAvg = nTick;
        else
            nAvg = (nAvg + nTick) >> 1;
    }

    uart_printf ("Put data is ok, max=%d, avg=%d\r\n", nMax, nAvg);

    // To wait some time to let MDMA finish
    MxDelayTicks (TICKS_TO_MS(100));

    // testing
    for (i = 0; i < 20; i++)
    {
        memset (pData, nPat + i, nTestSize);

        rc = GenLogScan(&nPos, &pRec);

        if (FAILED(rc))
        {
            uart_printf ("GenLogScan - error, rc=0x%x\r\n", rc);
            MxHeapFree (hHeap, pData);
            return 0;
        }

        //((PUINT8)pData) [10] = 6;

        rc = memcmp (pData, pRec + 1, nTestSize);

        if (rc != 0)
        {
            uart_printf ("Data compare error, i=%d, rc=%d\r\n", i, rc);
            MxHeapFree (hHeap, pData);
            return 0;
        }

        if (pRec->nSize != nTestSize)
        {
            uart_printf ("pRec->nSize: %d != (block size=%d), i=%d\r\n", nTestSize, pRec->nSize, i);
            MxHeapFree (hHeap, pData);
            return 0;
        }

        if (pRec->ID != 1)
        {
            uart_printf ("pRec->ID != 1, id=%d, i=%d\r\n", pRec->ID, i);
            MxHeapFree (hHeap, pData);
            return 0;
        }

        if (pRec->Descr != 0xFA)
        {
            uart_printf ("pRec->Descr != 0xFA, id=%d, i=%d\r\n", pRec->Descr, i);
            MxHeapFree (hHeap, pData);
            return 0;
        }
    }

    uart_printf ("Comparing is OK\r\n");

    if (GenLogGetMsgNum () != 20)
        uart_printf ("Num of messages error, %d != 20\r\n", GenLogGetMsgNum ());

    MxHeapFree (hHeap, pData);

    GenLogReset ();

    return 0;
}

UINT32 CLineCmdTestMmu(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 nOpt = 0;
    UINT32 nAddr= 0;

    nOpt = CMgrGetParam (pParams, 0, 0);
    nAddr= CMgrGetParam (pParams, 1, 0);

    if (nOpt == 0)
    {
        uart_printf ("VA [0x%08x] => PH [0x%08x]\r\n", nAddr, MxMmuVToP (nAddr));
    }

    return 0;
}

typedef struct
{
    VUINT32 Cpu;
    VUINT32 Addr;
    VUINT32 Size;
    VUINT32 nStatus;
}RDUMP;

volatile RDUMP rdump;

UINT32 RDumpCode (RDUMP * pData)
{
    uart_printf ("R-DUMP(CPU-%d)\r\n", MxGetCpuID ());

    uart_printf ("------------------------------------------------------------------------------", MxGetCpuID ());
    DumpProc (pData->Addr, pData->Size);
    uart_printf ("\r\n------------------------------------------------------------------------------\r\n", MxGetCpuID ());

    rdump.nStatus = 1;

    return 0;
}

UINT32 CLineCmdRDump(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    MXRC rc = MX_OK;

    rdump.Cpu     = CMgrGetParam (pParams, 0, 0);
    rdump.nStatus = 0;
    rdump.Addr    = CMgrGetParam (pParams, 1, 0);
    rdump.Size    = CMgrGetParam (pParams, 2, 0);

    uart_printf ("R-DUMP(cpu-%d, [0x%x...%x])\r\n", rdump.Cpu, rdump.Addr, rdump.Addr+rdump.Size);

    if (rdump.Cpu != 0)
    {
        rdump.nStatus = 0;
        rc = MxInjectCode (rdump.Cpu, (TENTRY)RDumpCode, (void*)&rdump);
    }
    else
    {
        DumpProc (rdump.Addr, rdump.Size);
        rdump.nStatus = 1;
    }

    if (FAILED(rc))
    {
        uart_printf ("Failed to run R-DUMP on CPU-%d\r\n", rdump.Cpu);
        return 0;
    }

    while (1)
    {
        if (rdump.nStatus != 0)
            break;
    }

    return 0;
}


UINT32 CLineCmdMMUL2Test(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    PUINT32 p;
    UINT32 i, num = 32768;
    MASTERTABLE hTable;
    MMUSECTION hSect;
    //MMUPAGE hPage;

    // Pointer to NCNB DDR1
    p = (PUINT32)(0x20000000 + 128*1024*1024);

    uart_printf ("Filling DDR1 (%x)... ", (UINT32)p);

    // 128KB
    for (i = 0; i < num; i++)
    {
        p[i] = (UINT32)(p + i);
    }

    uart_printf ("OK\r\n");

    // Preparing MMU

    uart_printf ("MMU cfg ... ");

    hTable = MxMmuGetMasterTable ();
    _ASSERT_PTR(hTable);

    hSect = MxMmuGetSection (hTable, (UINT32)p);
    _ASSERT_PTR(hSect);

    MxMmuSetSectionType (hSect, MMU_SECTION_TYPE_PAGED_64, MMU_MASTER_DOMAIN(0), MMU_PAGE_AP(AP_FULL_ACCESS)|MMU_PAGE64_TEX(TEXCB_STRONGLY_ORDERED));

    uart_printf ("OK\r\n");

    uart_printf ("Checking DDR1 (%x)... ", (UINT32)p);

    // 128KB
    for (i = 0; i < num; i++)
    {
        if (p[i] != (UINT32)(p + i))
        {
            uart_printf ("ERROR: i=%d, (%x)!=(%x)\r\n", i, p[i], (UINT32)(p + i));
            break;
        }
    }

    if (i >= num)
        uart_printf ("OK\r\n");

    return 0;
}

UINT32 CLineCmdTestAppHeap(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 nCode;
    UINT32 i, *p;
    SECTINFO info;
    UINT32 nTick;

    nCode = CMgrGetParam (pParams, 0, 0);

    SysCoreGetSectInfo (SECTION_APP_HEAP, &info);

    if (nCode == 0)
    {
        uart_printf ("Filling memory %x ... ", info.Base);

        nTick = GetTIMETICK();

        p = (PUINT32)info.Base;

        for (i = 0; i < 32768 * 2; i++)
        {
            p[i] = (UINT32)(p+i);
        }

        uart_printf ("OK (tick=%d)\r\n", GetTIMETICK() - nTick);
    }

    if (nCode == 1)
    {
        uart_printf ("Checking memory %x ... ", info.Base);

        nTick = GetTIMETICK();

        p = (PUINT32)info.Base;

        for (i = 0; i < 32768 * 2; i++)
        {
            if (p[i] != (UINT32)(p+i))
            {
                uart_printf ("ERROR: %x, (%x)!=(%x)\r\n", (UINT32)(i+p), p[i], (UINT32)(p+i));
                return 0;
            }
        }

        uart_printf ("OK (tick=%d)\r\n", GetTIMETICK() - nTick);
    }

    if (nCode == 2)
    {
        uart_printf ("Filling memory %x with A5... ", info.Base);

        nTick = GetTIMETICK();

        p = (PUINT32)info.Base;

        for (i = 0; i < 32768 * 2; i++)
        {
            p[i] = (UINT32)(0xA5A5A5A5);
        }

        uart_printf ("OK (tick=%d)\r\n", GetTIMETICK() - nTick);
    }

    return 0;
}

UINT32 CLineCmdSetAP(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 nAddr, nCpuID;
    UINT32 nSize;
    UINT32 nAP;
    MXRC rc;

    nCpuID= CMgrGetParam (pParams, 0, 1<<0);
    nAddr = CMgrGetParam (pParams, 1, 0);
    nSize = CMgrGetParam (pParams, 2, 0);
    nAP   = CMgrGetParam (pParams, 3, AP_NO_ACCESS);

    uart_printf ("CPU[0x%02x][0x%x ... 0x%x] AP is [0x%x] ... ", nCpuID, nAddr, nAddr+nSize, nAP);

    rc = SysCoreMemAccess (nCpuID, nAddr, nSize, nAP);

    if (FAILED(rc))
        uart_printf ("FAILED, rc=%x\r\n", rc);
    else
        uart_printf ("OK\r\n");

    return 0;
}

#ifdef SRIO_DRV_ENABLED
UINT32 CLineCmdSRIO(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    SRIODRVCFG cfg;
    UINT32 rc;

    memset (&cfg, 0, sizeof (cfg));

    cfg.nID = 0;

    cfg.RxBufNum = 2;
    cfg.RxBufSize= 512;

    cfg.TxBufNum = 2;
    cfg.TxBufSize= 512;

    cfg.LBMode = SRIO_LB_PCS;
    cfg.Lanes  = SRIO_LANES_X4;

    uart_printf ("Enabling SRIO device (id=%d, LB=%d, lanes=%d) ... ", cfg.nID, cfg.LBMode, cfg.Lanes);
    rc = SrioDrvEnabledDevice (&cfg);
    uart_printf ("finished, rc=0x%x\r\n", rc);

    SrioDrvShutdown();

    return 0;
}
#endif

static unsigned char* pPrintBufData = NULL;

UINT32 CLineCmdHoldPrint(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
	UINT32 bufsize;

	bufsize = (UINT32)StrToNum(pParams->ParamList[0]);
	pPrintBufData = MxHeapAlloc(NcnbHeap, bufsize);
	if (pPrintBufData == NULL)
	{
		uart_printf("Cannot allocate the memory.\n");
		return 0;
	}
	UartDrvStartHoldPrinting(pPrintBufData, bufsize);
	printf("Bufferization turned on.\n");
	return 0;
}

UINT32 CLineCmdPrintData(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
	if (UartDrvPrintBuf() != MX_OK);
	{
//		printf ("Printing error.\n");
	}
	return 0;
}

UINT32 CLineCmdReleasePrint(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
	UartDrvStopHoldPrinting();
	uart_printf("\nBufferization turned off.\n");
	MxHeapFree(NcnbHeap, pPrintBufData);
	return 0;
}

UINT32 CLineCmdPrintSomeData(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
	unsigned int i;

	printf("Start test...\n");
	for (i = 0; i < 100; i++)
	{
		uart_printf("%d", i);
	}
	printf("printing delayed.\n");
	MxDelay(10000);
	printf("End delay.\n");
	return 0;
}

UINT32 CLineCmdRePrint(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
	unsigned char* pData = NULL;
	unsigned int i;
	unsigned int bufsize = (UINT32)StrToNum(pParams->ParamList[0]);

	pData = MxHeapAlloc(NcnbHeap, bufsize);
	if (pData == NULL)
	{
		printf("The memory cannot be allocated.\n");
		return 0;
	}

	printf("Start test...\n");
	UartDrvStartHoldPrinting(pData, bufsize);
	for (i = 0; i < 100; i++)
	{
		uart_printf("%d", i);
	}
	printf("printing delayed.\n");
	MxDelay(10000);
	if (UartDrvPrintBuf() != MX_OK)
		printf("Printing error.\n");
	UartDrvStopHoldPrinting();
	uart_printf("\n\n The test ended.\n");
	MxHeapFree(NcnbHeap, pData);
	return 0;
}

UINT32 CLineCmdReExclTest(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 nTestVar = 0;
    UINT32 nRetVal;

    nRetVal = MxExcSet(&nTestVar, 1);

    if (nRetVal == nTestVar && nTestVar == 1)
    {
        uart_printf ("MxExcSet - OK\r\n");
    }
    else
    {
        uart_printf ("MxExcSet - error (expected:%d, taken:%d)\r\n", 1, nRetVal);
    }

    nRetVal = MxExcInc(&nTestVar);

    if (nRetVal == nTestVar && nTestVar == 2)
    {
        uart_printf ("MxExcInc - OK\r\n");
    }
    else
    {
        uart_printf ("MxExcInc - error (expected:%d, taken:%d)\r\n", 2, nRetVal);
    }

    nRetVal = MxExcDec(&nTestVar);

    if (nRetVal == nTestVar && nTestVar == 1)
    {
        uart_printf ("MxExcDec - OK\r\n");
    }
    else
    {
        uart_printf ("MxExcDec - error (expected:%d, taken:%d)\r\n", 1, nRetVal);
    }

    return 0;
}

UINT32 CLineCmdFecLogTest(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
	unsigned int i;
	unsigned int datasize = 512;
	unsigned int logsize  = 2048;
	unsigned char* pSrcBuf;
	unsigned char* pDstBuf;
	unsigned char* pLogBuf;
	PFEC_DL_FDESC pFDesc;
	DLHWCTX HwContext;
	FEC_CONFIG_REGS FecConfigRegs;

	// source data buffer
	pSrcBuf = MxHeapAlloc(NcnbHeap, datasize);
	if (pSrcBuf == NULL)
	{
	    printf("Cannot allocate the buffer.\n");
	    return RC_CM_PARAM_ERROR;
	}
	memset(pSrcBuf, 0, datasize);

	// destination data buffer
	pDstBuf = MxHeapAlloc(NcnbHeap, datasize);
	if (pDstBuf == NULL)
	{
	    printf("Cannot allocate the buffer.\n");
	    MxHeapFree(NcnbHeap, pSrcBuf);
	    return RC_CM_PARAM_ERROR;
	}
	memset(pDstBuf, 0, datasize);

	// buffer for logger
	pLogBuf = MxHeapAlloc(NcnbHeap, logsize);
	if (pLogBuf == NULL)
	{
		printf("Cannot allocate the buffer.\n");
	   	MxHeapFree(NcnbHeap, pSrcBuf);
	   	MxHeapFree(NcnbHeap, pDstBuf);
	   	return RC_CM_PARAM_ERROR;
	}
	memset(pLogBuf, 0, logsize);

    // initialization of source buffer;
    for (i = 0; i < datasize; i++)
    {
        if (i > 255)
        {
            *(pSrcBuf + i) = i % 256;
            continue;
        }
        *(pSrcBuf + i) = i;
    }

    // initialization of DL FEC
    memset (&FecConfigRegs, 0, sizeof(FEC_CONFIG_REGS));
    if (FecDlInit(&FecConfigRegs) != MX_OK)
    {
    	uart_printf ("Cannot initialize DL FEC.\n");
    	MxHeapFree(NcnbHeap, pSrcBuf);
    	MxHeapFree(NcnbHeap, pDstBuf);
    	MxHeapFree(NcnbHeap, pLogBuf);
    	return RC_CM_PARAM_ERROR;
    }

    // allocates Frame Descriptor
    pFDesc = FecDlAllocDesc(1);
    if (pFDesc == NULL)
    {
    	uart_printf ("Cannot allocate FEC FDesc.\n");
    	MxHeapFree(NcnbHeap, pSrcBuf);
    	MxHeapFree(NcnbHeap, pDstBuf);
    	MxHeapFree(NcnbHeap, pLogBuf);
    	return RC_CM_PARAM_ERROR;
    }

    // sets HW context
    memset (&HwContext, 0, sizeof(DLHWCTX));
    if (FecDlSetHwCtx(pFDesc, HwContext) != MX_OK)
    {
    	uart_printf ("Cannot set HW context.\n");
    	MxHeapFree(NcnbHeap, pSrcBuf);
    	MxHeapFree(NcnbHeap, pDstBuf);
    	MxHeapFree(NcnbHeap, pLogBuf);
    	return RC_CM_PARAM_ERROR;
    }

    // sets data
    if (FecDlSetData(pFDesc, pSrcBuf, datasize, pDstBuf, datasize) != MX_OK)
    {
    	uart_printf ("Cannot set data.\n");
    	MxHeapFree(NcnbHeap, pSrcBuf);
    	MxHeapFree(NcnbHeap, pDstBuf);
    	MxHeapFree(NcnbHeap, pLogBuf);
    	return RC_CM_PARAM_ERROR;
    }

    // sets last desc
    if (FecDlSetLastDesc(pFDesc, NULL, NULL) != MX_OK)
    {
    	uart_printf ("Cannot set last desc.\n");
    	MxHeapFree(NcnbHeap, pSrcBuf);
    	MxHeapFree(NcnbHeap, pDstBuf);
    	MxHeapFree(NcnbHeap, pLogBuf);
    	return RC_CM_PARAM_ERROR;
    }

    // enables logging
    if (FecDlSetLog(FEC_LOGFDESCMODE | FEC_LOGDATAMODE, pLogBuf, logsize) != MX_OK)
    {
    	uart_printf ("Cannot enable logging.\n");
    	MxHeapFree(NcnbHeap, pSrcBuf);
    	MxHeapFree(NcnbHeap, pDstBuf);
    	MxHeapFree(NcnbHeap, pLogBuf);
    	return RC_CM_PARAM_ERROR;
    }

    // launches DL FEC
    if (FecDlRun(pFDesc) != MX_OK)
    {
    	uart_printf ("Cannot launch FEC DL.\n");
    	MxHeapFree(NcnbHeap, pSrcBuf);
    	MxHeapFree(NcnbHeap, pDstBuf);
    	MxHeapFree(NcnbHeap, pLogBuf);
    	return RC_CM_PARAM_ERROR;
    }

    // printing results
    printf ("input data:\n");
    for (i = 0; i < datasize; i++)
    {
        if (!(i % 8))
        {
            printf ("%d, ", *(pSrcBuf + i));
            MxDelay (200);
        }
    }

    printf ("\n output data:\n");
    for (i = 0; i < datasize; i++)
    {
        if (!(i % 8))
        {
            printf ("%d, ", *(pDstBuf + i));
            MxDelay (200);
        }
    }

    uart_printf ("\ndata from log buffer:\n");
    for (i = 0; i < logsize; i++)
    {
    	if (!(i % 8))
    	{
    		uart_printf ("%d, ", *(pLogBuf + i));
    		MxDelay (200);
    	}
    }

    // disable logging
    if (FecDlCloseLog() != MX_OK)
    	printf ("cannot disable logging.\n");

    // empty the memory
    MxHeapFree (NcnbHeap, pSrcBuf);
    MxHeapFree (NcnbHeap, pDstBuf);
    MxHeapFree (NcnbHeap, pLogBuf);

    printf ("\n the test ended.\n");

	return 0;
}

UINT32 CLineCmdRvds(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    FILE * pFile = NULL;

    fprintf (stderr, "Hello from: fprintf\r\n");
    pFile = fopen  ("123.txt", "ro");
	if(pFile)
	    fclose (pFile);
    fprintf (stderr, "Bye: fprintf\r\n");

    return 0;
}

ApiHeader* pApiCpriCfg = NULL;

UINT32 CLineCmdCpriConfig(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
	MXRC rc;
	UINT32 cpri_data1 = 1;
	UINT32 cpri_data2 = 2;

	// Allocates the memory for API message
	pApiCpriCfg = SvsrAllocICApiEx(SVSR_I_CPU_RX);
	if (pApiCpriCfg ==  NULL)
	{
		uart_printf ("CPRI config test: cannot allocate API message.\n");
		return 0;
	}

	// Creates API message
//	ApiCreate(pApiCpriCfg, API_DEF_FLAGS, 1, 1, API_TYPE_CFG_PARAM, PHY_START_REQ_MSG);
	pApiCpriCfg = ApiCreate(pApiCpriCfg, API_DEF_FLAGS, 1, IID_LTE, API_TYPE_CFG_PARAM, PHY_CPRI_CONFIG);

	// Adds the parameter
	ApiAddParam (pApiCpriCfg, NULL, PAR_CPRI_CONF_NUM_CPRI_DEV, &cpri_data1, sizeof (cpri_data1));
	ApiAddParam (pApiCpriCfg, NULL, PAR_CPRI_CONF_SAMPLING_RATE, &cpri_data2, sizeof (cpri_data2));

	// Sends API message
//	rc = SvsrSendAPI (pApiCpriCfg); // sends a message to Linux
	rc = SvsrPutICpuApiToRx(pApiCpriCfg); // sends a message to L-ARM
	if (FAILED(rc))
	{
		uart_printf("CPRI config test: cannot send API message.\n");
	}

	return 0;
}

__align (128) byte NCNBVAR __test_msg[2000];


UINT32 CLineCmdNetTest(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
	char *cmd;
	MXRC rc;

	if(pParams->ParamCount == 0)
	{
		uart_printf("Need set command \r\n");
	}

	cmd = pParams->ParamList[0];
	uart_printf("Command : %s\r\n", cmd);

	if(strcmp(cmd, "init") == 0)
	{
		rc = net_init();

		uart_printf("init result %d\r\n", rc);
	}
	else if (strcmp(cmd, "test1") == 0)
	{
		net_socket s;
		net_sockaddr addr;
		byte test_msg[10] = {1,2,3,4,5,6,7,8,9,10};
		byte recv_buff[100];
		int free_mem_before, free_mem_after;

		free_mem_before = MxHeapGetFreeSpace(AppHeap);

		s = net_connection(NET_SOCKTYPE_DGRAM, NET_PROTO_UDP);

		addr.addr.ipv4addr.addr = hton(0xC0A80001);
		addr.addr.ipv4addr.port = htons(1234);

		net_sendto(s, &addr, test_msg, sizeof(test_msg), 0);

		net_recv_from(s, recv_buff, sizeof(recv_buff), &addr, 0);

		net_close(s);

		free_mem_after = MxHeapGetFreeSpace(AppHeap);

		if(free_mem_before != free_mem_after)
		{
			uart_printf("Free before : %d, free after : %d\r\n", free_mem_before, free_mem_after);
		}
		else
			uart_printf("Memcheck ok\r\n");

	}
	else if (strcmp(cmd, "test2") == 0)
	{
		net_socket s;
		net_sockaddr addr;
		int i;

		memset(__test_msg, 0, sizeof(__test_msg));

		s = net_connection(NET_SOCKTYPE_DGRAM, NET_PROTO_UDP);

		addr.addr.ipv4addr.addr = hton(0xC0A80001);
		addr.addr.ipv4addr.port = htons(2000);

		for(i = 0; i < 100; i++)
		{
			sprintf((char*)__test_msg, "test #%d", i);
			net_sendto(s, &addr, __test_msg, 100, 0);
		}

		net_close(s);
	}
	else if (strcmp(cmd, "test3") == 0)
	{
		net_socket s;
		net_sockaddr to;
		net_sockaddr from;
		byte test_msg[10];
		byte recv_buff[100];
		int i;
		int recv_size;
		int free_mem_before, free_mem_after;

		free_mem_before = MxHeapGetFreeSpace(AppHeap);

		memset(test_msg, 0, sizeof(test_msg));
		memset(recv_buff, 0, sizeof(recv_buff));

		s = net_connection(NET_SOCKTYPE_DGRAM, NET_PROTO_UDP);

		to.addr.ipv4addr.addr = hton(0xC0A80001);
		to.addr.ipv4addr.port = htons(3000);

		for(i = 0; i < 100; i++)
		{
			sprintf((char*)test_msg, "test #%d", i);
			net_sendto(s, &to, test_msg, sizeof(test_msg), 0);
			recv_size = net_recv_from(s, recv_buff, sizeof(recv_buff), &from, 0);
			uart_printf("recv  %d bytes, data is\r\n", recv_size);
			net_dump_data(recv_buff, recv_size);
			if(memcmp(test_msg, recv_buff, recv_size) != 0)
			{
				uart_printf("failed %d \r\n", i);
				break;
			}
			else
				uart_printf("pass %d \r\n", i);

		}

		net_close(s);

		free_mem_after = MxHeapGetFreeSpace(AppHeap);

		if(free_mem_before != free_mem_after)
		{
			uart_printf("Free before : %d, free after : %d\r\n", free_mem_before, free_mem_after);
		}
		else
			uart_printf("Memcheck ok\r\n");

	}
	else if (strcmp(cmd, "test4") == 0)
		{
			net_socket s;
			net_sockaddr to;
			byte test_msg[10];
			byte recv_buff[100];
			int i;
			int recv_size;
			int free_mem_before, free_mem_after;

			free_mem_before = MxHeapGetFreeSpace(AppHeap);

			memset(test_msg, 0, sizeof(test_msg));
			memset(recv_buff, 0, sizeof(recv_buff));

			s = net_connection(NET_SOCKTYPE_DGRAM, NET_PROTO_UDP);

			to.addr.ipv4addr.addr = hton(0xC0A80001);
			to.addr.ipv4addr.port = htons(3000);

			net_connect(s, &to);

			for(i = 0; i < 100; i++)
			{
				sprintf((char*)test_msg, "test #%d", i);
				net_send(s, test_msg, sizeof(test_msg), 0);

                recv_size = net_recv(s, recv_buff, sizeof(recv_buff), 0);

				if(memcmp(test_msg, recv_buff, recv_size) != 0)
					break;
				else
					uart_printf("pass %d\r\n", i);
			}

			net_close(s);

			free_mem_after = MxHeapGetFreeSpace(AppHeap);

			if(free_mem_before != free_mem_after)
			{
				uart_printf("Free before : %d, free after : %d\r\n", free_mem_before, free_mem_after);
			}
			else
				uart_printf("Memcheck ok\r\n");
		}
		else if (strcmp(cmd, "test5") == 0)
		{
			net_socket s;
			net_sockaddr to;
			byte test_msg[10];
			byte recv_buff[100];
			int i;

			memset(test_msg, 0, sizeof(test_msg));
			memset(recv_buff, 0, sizeof(recv_buff));

			s = net_connection(NET_SOCKTYPE_DGRAM, NET_PROTO_UDP);

			to.addr.ipv4addr.addr = hton(0xC0A80001);
			to.addr.ipv4addr.port = htons(3000);

			net_connect(s, &to);

			for(i = 0; i < 100; i++)
			{
				sprintf((char*)test_msg, "test #%d", i);
				net_send(s, test_msg, sizeof(test_msg), 0);
			}

			net_close(s);
		}
#ifdef USE_IPERF
		else if (strcmp(cmd, "iperf") == 0)
		{
			//char static_args[] = "-c 192.168.0.1 -u -t 600 -p 5002";
			//char static_args[] = "-s -u -l 200 -i 1 -p 5002";
			//char static_args[] = "-c 192.168.39.1 -l 500 -t 120 -p 5002";

			char *iperf_args[20];
			int iperf_args_count;
			int free_mem_before, free_mem_after;

			free_mem_before = MxHeapGetFreeSpace(AppHeap);

			memset(iperf_args, 0, sizeof(iperf_args));

			iperf_args_count = pParams->ParamCount - 1;

			_memcpy(&iperf_args[1], &(pParams->ParamList[1]), iperf_args_count * sizeof(char *));

			uart_printf("run iperf\r\n");

			iperf(iperf_args_count + 1, iperf_args);

			free_mem_after = MxHeapGetFreeSpace(AppHeap);

			if(free_mem_before != free_mem_after)
			{
				uart_printf("Free before : %d, free after : %d\r\n", free_mem_before, free_mem_after);

			}
			else
				uart_printf("Memcheck ok\r\n");
		}
#endif
		else if (strcmp(cmd, "test6") == 0)
		{
			net_socket s;
			net_sockaddr addr;
			//byte recv_buff[100];
			int i;
			UINT32 t;

			MLogOpen();


			MLogSetMask(0xFFFFFFFF);
			MLogDevInfo(DevInfoGetHWID());

			MLogFREQ(RESOURCE_UARM0, ClkRstGetFreq(ARMx4_CORE0));
			MLogFREQ(RESOURCE_UARM1, ClkRstGetFreq(ARMx4_CORE1));
			MLogFREQ(RESOURCE_UARM2, ClkRstGetFreq(ARMx4_CORE2));
			MLogFREQ(RESOURCE_UARM3, ClkRstGetFreq(ARMx4_CORE3));


			MLogFREQ(RESOURCE_LARM0, ClkRstGetFreq(ARMx2_CORE0));
			MLogFREQ(RESOURCE_LARM1, ClkRstGetFreq(ARMx2_CORE1));

			MLogFREQ(RESOURCE_CEVA, ClkRstGetFreq(CR_DEV_CEVA_0));
			MLogFREQ(RESOURCE_FLTP, ClkRstGetFreq(CR_DEV_FP_0));

			MLogFREQ(RESOURCE_FECUL, ClkRstGetFreq(CR_DEV_FEC_UL));
			MLogFREQ(RESOURCE_FECDL, ClkRstGetFreq(CR_DEV_FEC_DL));

			MLogFREQ(RESOURCE_XP_AXI, ClkRstGetFreq(XP_AXI));
			MLogFREQ(RESOURCE_SYS_AXI, ClkRstGetFreq(SYS_AXI));
			MLogFREQ(RESOURCE_SPU_AXI, ClkRstGetFreq(SPU_AXI));
			MLogFREQ(RESOURCE_RAD_AXI, ClkRstGetFreq(RAD_AXI));

			MLogEnableRC(1);
			MLogResourceCondition (MLOG_MODE_SUPERVISOR, MxGetCpuID () + RESOURCE_UARM0, 0);

			memset(__test_msg, 0, sizeof(__test_msg));

			s = net_connection(NET_SOCKTYPE_DGRAM, NET_PROTO_UDP);

			addr.addr.ipv4addr.addr = hton(0xC0A80001);
			addr.addr.ipv4addr.port = htons(2000);

			net_connect(s, &addr);

			net_send(s, __test_msg, 100, 0);
			//net_sendto(s, &addr, __test_msg, 100, 0);

			t = ARM_INT_disable();

			for(i = 0; i < 1000; i++)
			{
				UINT32 t;
				sprintf((char*)__test_msg, "test #%d", i);
				t = MxGetTicks();
				net_send(s,  __test_msg, 1024, 0);
				//net_sendto(s, &addr,  __test_msg, 100, 0);
				MLogTask(10, RESOURCE_LARM, t, MxGetTicks());
			}

			ARM_INT_restore(t);
			net_close(s);

			MLogClose();
			MLogFlush();

			//for(i = 0; i < NET_PROF_SIZE; i++)
			//	uart_printf("prof[%d] = %u\r\n", i, net_prof[i]);
	}

    return 0;
}

HANDLE hMsgThrSrc, hMsgThrDst;

UINT32 MsgThreadCpuSrc(LPVOID p)
{
    UINT32 i, nMsgNum = 10;

    //                      0  1  2  3  4  5  6  7  8  9
    UINT32 msg_prior [] = { 4, 5, 6, 0, 1, 2, 8, 0, 1, 1 };

    // => 3, 7, 4, 8, 9, 5, 0, 1, 2, 6

    UINT32 t;
    UINT32 t_min = (UINT32)(-1);
    UINT32 t_max = 0;
    UINT32 t_avg = 0;

    for (i = 0; i < nMsgNum; i++)
    {
        t = GetTIMETICK();
        MxThreadSendMsgEx (hMsgThrDst, (LPVOID) (i), msg_prior[i], /*SCHED_SMP | */SCHED_AT_BEGIN);
        t = GetTIMETICK() - t;

        t_min = MIN(t_min, t);
        t_max = MAX(t_max, t);
        t_avg+= t;
    }

    t_avg /= nMsgNum;

    uart_printf ("Src thread is completed, ticks are (min:%d avg:%d max:%d)\r\n", t_min, t_avg, t_max);

    return 0;
}

UINT32 MsgThreadCpuDst(LPVOID p)
{
    HANDLE h = MxGetCurrentThread ();
    LPVOID pMsg;

    UINT32 msg_list [] = { 3, 7, 4, 8, 9, 5, 0, 1, 2, 6};
    UINT32 i = 0;

    uart_printf ("Dst thread, cpu=%d\r\n", MxGetCpuID ());

    while (SUCCESSFUL (MxThreadRecvMsg (h, &pMsg)))
    {
        if ((UINT32)pMsg == msg_list[i])
            uart_printf ("DST THR, message: %d (PASS)\r\n", (UINT32)pMsg);
        else
            uart_printf ("DST THR, message: %d != %d (FAIL!!!)\r\n", (UINT32)pMsg, msg_list[i]);

        i++;
    }

    uart_printf ("\r\n%d messages are received\r\n", i);

    return 0;
}

UINT32 CLineCmdThreadMsg(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 nSrcCpu = 0;
    UINT32 nDstCpu = 1;
    MXRC   rc = 0;

    rc = MxCreateThreadEx (nSrcCpu, MsgThreadCpuSrc, NULL, 0, 0, &hMsgThrSrc);

    if (FAILED(rc))
    {
        uart_printf ("Create src thread (cpu-%d) error, rc=%x\r\n", nSrcCpu, rc);
        return 0;
    }

    rc = MxCreateThreadEx (nDstCpu, MsgThreadCpuDst, NULL, 0, 0, &hMsgThrDst);

    if (FAILED(rc))
    {
        MxRemoveThread (hMsgThrSrc);
        uart_printf ("Create dst thread (cpu-%d) error, rc=%x\r\n", nSrcCpu, rc);
        return 0;
    }

    uart_printf ("To start source thread ... \r\n");

    MxScheduleThread (hMsgThrSrc);

    return 0;
}

void __sctp_rcv_cb(void)
{
	uart_printf("!!!!! CB !!!!!!\r\n");
	sctp_getEvents();
}

UINT32 CLineCmdSCTPTest(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
#define MAXIMUM_NUMBER_OF_IN_STREAMS          1
#define MAXIMUM_NUMBER_OF_OUT_STREAMS         1

	char *cmd;
	int rc;
	static unsigned int	sctpInstance;

	if(pParams->ParamCount == 0)
	{
		uart_printf("Need set command \r\n");
	}

	cmd = pParams->ParamList[0];
	uart_printf("Command : %s\r\n", cmd);

	if(strcmp(cmd, "init") == 0)
	{
		SCTP_ulpCallbacks Ulp;
		unsigned char	localAddressList[1][SCTP_MAX_IP_LEN];

		rc = net_init();
		rc = sctp_initLibrary();
		uart_printf("init result %d\r\n", rc);

		memset(&Ulp, 0, sizeof(Ulp));
		strcpy((char *)localAddressList[0], "192.168.0.5");


		sctpInstance = sctp_registerInstance(32,	MAXIMUM_NUMBER_OF_IN_STREAMS,
											MAXIMUM_NUMBER_OF_OUT_STREAMS,
											1, localAddressList,
											Ulp);

		uart_printf("isctp_registerInstance %d\r\n", sctpInstance);
	}

	if(strcmp(cmd, "test1") == 0)
	{
		unsigned short assocID;
		byte test_msg[10] = {1,2,3,4,5,6,7,8,9,10};
		byte test_msg_rcv[10];
		unsigned int test_msg_rcv_size = 10;
		int res;
		unsigned short streamSN;
		unsigned int tsn;

		memset(test_msg_rcv, 0, sizeof(test_msg_rcv));

		assocID = sctp_associate( sctpInstance,
													MAXIMUM_NUMBER_OF_OUT_STREAMS,
													"192.168.0.1",
													32, NULL );

		uart_printf("sctp_associate %d\r\n", assocID);

		if(assocID <= 0)
		{
			uart_printf("error sctp_associate \r\n");
			return 0;
		}

		res = sctp_send(	assocID ,
								0,
								test_msg,
								sizeof(test_msg),
								SCTP_GENERIC_PAYLOAD_PROTOCOL_ID,
								SCTP_USE_PRIMARY,
								SCTP_NO_CONTEXT,
								SCTP_INFINITE_LIFETIME,
								SCTP_ORDERED_DELIVERY,
								SCTP_BUNDLING_DISABLED );

		uart_printf("sctp_send %d\r\n", res);

		res = sctp_receive(assocID, 0, test_msg_rcv, &test_msg_rcv_size, &streamSN, &tsn, SCTP_MSG_DEFAULT);

		if(res == SCTP_SUCCESS)
		{
			if(memcmp(test_msg, test_msg_rcv, test_msg_rcv_size) == 0)
				uart_printf("\r\ncompare success \r\n");
			else
				uart_printf("\r\ncompare UNsuccess \r\n");

		}
		else
			uart_printf("\r\n unsuccess recv %d\r\n", res);

		sctp_shutdown(assocID);
		sctp_deleteAssociation(assocID);

	}
	else if(strcmp(cmd, "test2") == 0)
	{
		unsigned short assocID;
		char test_msg[10];
		char test_msg_rcv[10];
		unsigned int test_msg_rcv_size = 10;
		int res;
		unsigned short streamSN;
		unsigned int tsn;
		int i = 0;
		int len;

		memset(test_msg, 0, sizeof(test_msg_rcv));
		memset(test_msg_rcv, 0, sizeof(test_msg_rcv));

		assocID = sctp_associate( sctpInstance,
													MAXIMUM_NUMBER_OF_OUT_STREAMS,
													"192.168.0.1",
													32, NULL );

		uart_printf("sctp_associate %d\r\n", assocID);

		if(assocID <= 0)
		{
			uart_printf("error sctp_associate \r\n");
			return 0;
		}

		for(i = 0; i < 100; i++)
		{
			sprintf(test_msg, "test %d", i);

			uart_printf("\r\ntest %d:\r\n", i);

			len = strlen(test_msg);

			res = sctp_send(	assocID ,
								0,
								(unsigned char *)test_msg,
								len,
								SCTP_GENERIC_PAYLOAD_PROTOCOL_ID,
								SCTP_USE_PRIMARY,
								SCTP_NO_CONTEXT,
								SCTP_INFINITE_LIFETIME,
								SCTP_ORDERED_DELIVERY,
								SCTP_BUNDLING_DISABLED );

			uart_printf("sctp_send %d\r\n", res);

			memset(test_msg_rcv, 0, test_msg_rcv_size);

			test_msg_rcv_size = len;
			res = sctp_receive(assocID, 0, (unsigned char *)test_msg_rcv, &test_msg_rcv_size, &streamSN, &tsn, SCTP_MSG_DEFAULT);

			if(res == SCTP_SUCCESS)
			{
				if(memcmp(test_msg, test_msg_rcv, test_msg_rcv_size) == 0)
					uart_printf("\r\n%d - compare success \r\n", i);
				else
				{
					uart_printf("\r\n%d - compare UNsuccess: send %s, recived %s (%d) \r\n", i, test_msg, test_msg_rcv, test_msg_rcv_size);
					break;
				}
			}
			else
			{
				uart_printf("\r\n%d unsuccess recv %d\r\n", i, res);
				break;
			}

		}

		sctp_shutdown(assocID);
		sctp_deleteAssociation(assocID);

	}
	else if(strcmp(cmd, "free") == 0)
	{
		rc = sctp_freeLibrary();

		uart_printf("free result %d\r\n", rc);
	}
	else if(strcmp(cmd, "client") == 0)
	{
		char *sctp_host;
		char *sctp_port;

		unsigned int sctp_port_int;

		sctp_host = pParams->ParamList[1];
		sctp_port = pParams->ParamList[2];

		uart_printf("Host is %s, port is %s \r\n", sctp_host, sctp_port);

		sctp_port_int = atoi(sctp_port);

		SvsrChangeTransport(COMM_TYPE_DIRECT_SCTP, sctp_host, sctp_port_int);
	}


	return 0;
}

UINT32 SrdsSfpCtrl(UINT32 SrdsId, UINT32 SfpEnableMask);
UINT32 CLineCmdSFP(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 id, mask, rc;

    id   = CMgrGetParam (pParams, 0, 1);
    mask = CMgrGetParam (pParams, 0, 3);

    uart_printf ("SFP initialization (id=%d, mask=%d) ... ", id, mask);

    if (FAILED(rc = SrdsSfpCtrl(id, mask)))
    {
        uart_printf ("SFP control is failed ... rc=0x%08x\r\n", rc);
        return rc;
    }

    uart_printf ("OK\r\n");

    return 0;
}

UINT32 CLineCmdCRC(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 nSize, rem = 0;
    PUINT8 pBuf;
    UINT32 nCrc;

    //0, 0x864CFB, 1

    if (pParams->ParamCount < 2)
    {
        uart_printf ("crc <addr> <size>\r\n");
        return 0;
    }

    pBuf  = (PUINT8)CMgrGetParam (pParams, 0, 0);
    nSize = CMgrGetParam (pParams, 1, 0);

    uart_printf ("CRC [0x%08x / %d] is ", (UINT32)pBuf, nSize);

    nCrc = MdmaProcCRC (&rem, 0x864CFB, 32, pBuf, nSize);

    uart_printf ("0x%08x\r\n", nCrc);

    return 0;
}

void ICpuQPrint(ICPUQ *pQ, UINT32 index)
{
    const char * pDescr [ICPU_QUEUE_NUM + 1] =
    {
        "ICPU_TX_FREE_QUEUE",
        "ICPU_TX_HI_QUEUE",
        "ICPU_TX_REG_QUEUE",
        "ICPU_RX_FREE_QUEUE",
        "ICPU_RX_HI_QUEUE",
        "ICPU_RX_REG_QUEUE",
        NULL
    };

    uart_printf ("Q-index:%d(%s) ", index, pDescr[index]);
    uart_printf ("~~~~~~~~~~~~~~\r\n", index);
    uart_printf (" Ptr:0x%08x, Num:%d, Put:%d, Get:%d, PutStat:%d, GetStat:%d\r\n", pQ->Ptr, pQ->Num, pQ->Put, pQ->Get, pQ->StatPut, pQ->StatGet);
}

UINT32 CLineCmdGetInfo(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    MXRC rc;
    SICMemMap    info;
    ICPU_OBJ_CTX info_ex;
    UINT32 i;

    rc = GetICMemObjects (&info);

    if (SUCCESSFUL(rc))
    {
        uart_printf ("===========RX(from U-ARM)============\r\n");

        uart_printf ("RX-part.AllocNum=%d\r\n", info.FromUpArmCtrlObj.part.AllocNum);
        uart_printf ("RX-part.BlkCount=%d\r\n", info.FromUpArmCtrlObj.part.BlkCount);
        uart_printf ("RX-part.BlkSize=%d\r\n", info.FromUpArmCtrlObj.part.BlkSize);
        uart_printf ("RX-part.FreeBlock=0x%08x\r\n", info.FromUpArmCtrlObj.part.FreeBlock);
        uart_printf ("RX-part.Storage=0x%08x\r\n", info.FromUpArmCtrlObj.part.Storage);
        uart_printf ("RX-part.Sync=0x%08x\r\n", info.FromUpArmCtrlObj.part.Sync);

        uart_printf ("\r\n");

        uart_printf ("RX-Q.ElmSize=%d\r\n", info.FromUpArmCtrlObj.queue.ElmSize);
        uart_printf ("RX-Q.GetIndex=%d\r\n", info.FromUpArmCtrlObj.queue.GetIndex);
        uart_printf ("RX-Q.PutIndex=%d\r\n", info.FromUpArmCtrlObj.queue.PutIndex);
        uart_printf ("RX-Q.QueueSize=%d\r\n", info.FromUpArmCtrlObj.queue.QueueSize);
        uart_printf ("RX-Q.Storage=0x%08x\r\n", info.FromUpArmCtrlObj.queue.Storage);
        uart_printf ("RX-Q.Sync=0x%08x\r\n", info.FromUpArmCtrlObj.queue.Sync);

        uart_printf ("===========TX(to U-ARM)============\r\n");

        uart_printf ("TX-part.AllocNum=%d\r\n", info.ToUpArmCtrlObj.part.AllocNum);
        uart_printf ("TX-part.BlkCount=%d\r\n", info.ToUpArmCtrlObj.part.BlkCount);
        uart_printf ("TX-part.BlkSize=%d\r\n", info.ToUpArmCtrlObj.part.BlkSize);
        uart_printf ("TX-part.FreeBlock=0x%08x\r\n", info.ToUpArmCtrlObj.part.FreeBlock);
        uart_printf ("TX-part.Storage=0x%08x\r\n", info.ToUpArmCtrlObj.part.Storage);
        uart_printf ("TX-part.Sync=0x%08x\r\n", info.ToUpArmCtrlObj.part.Sync);

        uart_printf ("\r\n");

        uart_printf ("TX-Q.ElmSize=%d\r\n", info.ToUpArmCtrlObj.queue.ElmSize);
        uart_printf ("TX-Q.GetIndex=%d\r\n", info.ToUpArmCtrlObj.queue.GetIndex);
        uart_printf ("TX-Q.PutIndex=%d\r\n", info.ToUpArmCtrlObj.queue.PutIndex);
        uart_printf ("TX-Q.QueueSize=%d\r\n", info.ToUpArmCtrlObj.queue.QueueSize);
        uart_printf ("TX-Q.Storage=0x%08x\r\n", info.ToUpArmCtrlObj.queue.Storage);
        uart_printf ("TX-Q.Sync=0x%08x\r\n", info.ToUpArmCtrlObj.queue.Sync);
    }
    else if (SUCCESSFUL(rc = GetExtICMemObjects (&info_ex)))
    {
        uart_printf ("Extended I-CPU is selected\r\n\r\n");
        uart_printf ("    I-CPU version: %04x\r\n", info_ex.Version);
        uart_printf ("  I-CPU tx-irq-id:%d\r\n", info_ex.TxIrqID);
        uart_printf ("  I-CPU rx-irq-id:%d\r\n", info_ex.RxIrqID);
        uart_printf ("       TxArrayPtr:0x%08x\r\n", info_ex.TxMsgStoragePtr);
        uart_printf ("       RxArrayPtr:0x%08x\r\n", info_ex.RxMsgStoragePtr);
        uart_printf ("      TxBlocksNum:%d\r\n", info_ex.TxBlocksNum);
        uart_printf ("      RxBlocksNum:%d\r\n", info_ex.RxBlocksNum);
        uart_printf ("      TxBlockSize:%d\r\n", info_ex.TxBlockSize);
        uart_printf ("      RxBlockSize:%d\r\n", info_ex.RxBlockSize);
        uart_printf ("           TxQNum:%d\r\n", info_ex.TxQNum);
        uart_printf ("           RxQNum:%d\r\n", info_ex.RxQNum);
        uart_printf ("\r\n");

        for (i = 0; i < sizeof (info_ex.Queues)/sizeof (info_ex.Queues[0]); i++)
        {
            ICpuQPrint (&info_ex.Queues[i], i);
        }
    }
    else
    {
        uart_printf ("Error to get the I-CPU object information\r\n");
    }

    return 0;
}

#define IRQ_L_ID      5
#define IRQ_L_NUM     100000
VUINT32 IrqLStart    = 0;
VUINT32 IrqLTotal    = 0;
VUINT32 IrqLComplete = 0;
VUINT32 IrqLCanStart = 0;
VUINT32 IrqLNum      = 0;
VUINT32 IrqLMin      = 0;
VUINT32 IrqLMax      = 0;

void IrqLHandler(volatile UINT32 *pIntClearReg, UINT32 nIrqID, LPVOID pParam)
{
    UINT32 nTick = Get24MHzTick();

    IrqLTotal += (nTick - IrqLStart);

    IrqLMin = MIN(IrqLMin, (nTick - IrqLStart));
    IrqLMax = MAX(IrqLMax, (nTick - IrqLStart));

    IrqLComplete = 1;

    *pIntClearReg = nIrqID;
}

UINT32 IrqLThread (LPVOID p)
{
    uart_printf ("IrqLThread (cpu=%d) is started (for num=%d)\r\n", MxGetCpuID (), IrqLNum);

    while (IrqLCanStart == 0);

    while (IrqLNum != 0)
    {
        IrqLComplete = 0;

        // to generate IRQ to CPU-0
        IrqLStart = Get24MHzTick();
        //((GicDistRegs*)GIC_DISTR_BASE_ADDR)->SoftGenInt = ((((1 << 0) & 0xFF) << 16) | (IRQ_L_ID));
        //MxRaiseInterCpu (0, IRQ_L_ID);
        ICDrvRaiseInterCpu (0, IRQ_L_ID);

        // to wait processing
        while (IrqLComplete == 0)
        {
            _NOP();
        }

        // to wait 1 us
        MxDelayTicks(150);

        IrqLNum--;
    }

    return 0;
}

UINT32 CLineCmdIRQL(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 remcpu;
    HANDLE h;
    MXRC rc;
    UINT32 irq = IRQ_L_ID;

    remcpu = CMgrGetParam (pParams, 0, 1);

    uart_printf ("IRQ latency test [cpu:0] <-> [cpu:%d]\r\n", remcpu);

    rc = MxSetIntHandler (irq, INT_TYPE_IRQ, 0, IrqLHandler, NULL);

    if (FAILED(rc))
    {
        uart_printf ("Install IRQ handle error, rc=%xr\n", rc);
        return 0;
    }

    rc = MxCreateThreadEx (remcpu, IrqLThread, NULL, NULL, 0, &h);

    if (FAILED(rc))
    {
        uart_printf ("Create IRQ-L thread error, rc=%xr\n", rc);
        return 0;
    }

    IrqLStart    = 0;
    IrqLTotal    = 0;
    IrqLComplete = 0;
    IrqLCanStart = 0;
    IrqLNum      = IRQ_L_NUM;
    IrqLMin      = (UINT32)-1;
    IrqLMax      = (UINT32)0;

    rc = MxScheduleThread(h);

    if (FAILED(rc))
    {
        uart_printf ("Schedule IRQ-L thread error, rc=%xr\n", rc);
        return 0;
    }

    MxDelayTicks (150*100);

    uart_printf ("The test is started ... ");

    rc = MxEnableInt (irq);

    if (FAILED(rc))
    {
        uart_printf ("Enable IRQ-L is failed, rc=%xr\n", rc);
        return 0;
    }

    IrqLCanStart = 1;

    while (IrqLNum != 0)
    {
        _NOP();
        _NOP();
    }

    MxDisableInt (irq);

    uart_printf ("OK\r\n");
    uart_printf ("Results: irq_num = %d, max=%d, avg=%d, min=%d\r\n", IRQ_L_NUM, IrqLMax, IrqLTotal / IRQ_L_NUM, IrqLMin);

    MxRemoveThread (h);

    return 0;
}

UINT32 CLineCmdICpuStat(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 nCpuNum = SysGetCpuNum ();
    SvsrApiStat Stat;
    MXRC rc;
    UINT32 i, rx = 0, tx = 0;

    for (i = 0; i < nCpuNum; i++)
    {
        rc = SvsrGetICpuStatistics (i, &Stat);

        if (FAILED(rc))
        {
            printf ("Error to get svsr statistics (cpu-%d), rc=%x\r\n", i, rc);
            continue;
        }

        if (Stat.TxMsgNum != 0 || Stat.RxMsgNum != 0)
        {
            printf ("================ CPU-%d ==============\r\n", i);
        }

        if (Stat.TxMsgNum != 0)
        {
            printf ("TX-NUM=%d, Time=0x%08x ", Stat.TxMsgNum, Stat.TxTime);
            //MxDelayTicks (150*2000);

            printf ("[last TX-API]: Ctrl: 0x%08x Src: 0x%04x Dst: 0x%04x ID: 0x%04x Type: 0x%04x Len: %04d\r\n",
            Stat.TxICpuApi.Control,
            Stat.TxICpuApi.SrcInstID,
            Stat.TxICpuApi.DstInstID,
            Stat.TxICpuApi.MessageID,
            Stat.TxICpuApi.Type,
            Stat.TxICpuApi.Length);
            //MxDelayTicks (150*2000);

            tx += Stat.TxMsgNum;
        }

        if (Stat.RxMsgNum != 0)
        {
            printf ("RX-NUM=%d, Time=0x%08x ", Stat.RxMsgNum, Stat.RxTime);
            //MxDelayTicks (150*2000);

            printf ("[last RX-API]: Ctrl: 0x%08x Src: 0x%04x Dst: 0x%04x ID: 0x%04x Type: 0x%04x Len: %04d\r\n",
            Stat.RxICpuApi.Control,
            Stat.RxICpuApi.SrcInstID,
            Stat.RxICpuApi.DstInstID,
            Stat.RxICpuApi.MessageID,
            Stat.RxICpuApi.Type,
            Stat.RxICpuApi.Length);
            //MxDelayTicks (150*2000);

            rx += Stat.RxMsgNum;
        }

    }

    printf ("-----------------\r\n");
    //MxDelayTicks (150*2000);
    printf ("Total ICPU-TX : %d\r\n", tx);
    printf ("Total ICPU-RX : %d\r\n", rx);

    return 0;
}

UINT32 CLineCmdICoreStat(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 nCpuNum = SysGetCpuNum ();
    SvsrMsgStat Stat;
    MXRC rc;
    UINT32 i, rx = 0, tx = 0;

    for (i = 0; i < nCpuNum; i++)
    {
        rc = SvsrGetICoreStatistics (i, &Stat);

        if (FAILED(rc))
        {
            printf ("Error to get svsr statistics (cpu-%d), rc=%x\r\n", i, rc);
            continue;
        }

        if (Stat.TxMsgNum != 0 || Stat.RxMsgNum != 0)
        {
            printf ("================ CPU-%d ==============\r\n", i);
        }

        if (Stat.TxMsgNum != 0)
        {
            printf ("TX-NUM = %d\r\n", Stat.TxMsgNum);
            tx += Stat.TxMsgNum;
        }

        if (Stat.RxMsgNum != 0)
        {
            printf ("RX-NUM = %d\r\n", Stat.RxMsgNum);
            rx += Stat.RxMsgNum;
        }

    }

    printf ("-----------------\r\n");
    printf ("Total ICORE-TX : %d\r\n", tx);
    printf ("Total ICORE-RX : %d\r\n", rx);

    return 0;

}


UINT32 CondThr (void * p)
{
    PUINT32 pVal = (PUINT32)p;

    (*pVal) ++;

    return 0;
}

UINT32 CLineCmdCheckThrCond(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    HANDLE h;
    UINT32 val = 0, i = 0, num = 100;

    MxCreateThreadEx (1, CondThr, NULL, &val, 0, &h);

    while (i < num)
    {
        MxScheduleThreadEx (h, 1, SCHED_BY_CONDITION);
        MxDelayTicks (150 * 100);

        i++;
    }

    uart_printf ("test#1: Num = %d, val = %d\r\n", num, val);

    i = 0;
    val = 0;
    while (i < num)
    {
        MxScheduleThreadEx (h, 2, SCHED_BY_CONDITION);
        MxDelayTicks (150 * 100);

        i++;
    }

    uart_printf ("test#2: Num = %d, val = %d\r\n", num, val);

    i = 0;
    val = 0;
    while (i < num)
    {
        MxScheduleThreadEx (h, 3, SCHED_BY_CONDITION);
        MxDelayTicks (150 * 100);

        i++;
    }

    uart_printf ("test#3: Num = %d, val = %d\r\n", num, val);

    i = 0;
    val = 0;
    while (i < num)
    {
        MxScheduleThreadEx (h, 3, SCHED_BY_CONDITION | SCHED_SMP);
        MxDelayTicks (150 * 100);

        i++;
    }

    uart_printf ("test#4(SMP): Num = %d, val = %d\r\n", num, val);

    return 0;
}

UINT32 CLineCmdCheckSection(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    uart_printf ("Ticks:%d ... ", MxGetTime());
    MxDelayTicks (150*3000);
    uart_printf ("%d ... ", MxGetTime());
    MxDelayTicks (150*3000);
    uart_printf ("%d ...\r\n", MxGetTime());

    uart_printf ("Enter to safe section ... ");

    MxOpenSafeSection (1);

    uart_printf ("Ticks:%d ... ", MxGetTime());
    MxDelayTicks (150*3000);
    uart_printf ("%d ... ", MxGetTime());
    MxDelayTicks (150*3000);
    uart_printf ("%d ... ", MxGetTime());

    MxCloseSafeSection();

    uart_printf ("OK\r\n");

    uart_printf ("Ticks:%d ... ", MxGetTime());
    MxDelayTicks (150*3000);
    uart_printf ("%d ... ", MxGetTime());
    MxDelayTicks (150*3000);
    uart_printf ("%d ...\r\n", MxGetTime());

    return 0;
}


UINT32 CLineCmdUT(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
	UINT32 olddev = uart_printf_get_dev();
	uart_printf_select_dev(PRINT_DEV_CICPU, 0, 0);
	uart_printf("test \r\n");
	uart_printf_select_dev(olddev, 0, 0);
	uart_printf("test complete\r\n");
	return 0;
}

UINT32 CLineCmdDHCP(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
	char *cmd;

	if(pParams->ParamCount == 0)
	{
		uart_printf("Need set command \r\n");
	}

	cmd = pParams->ParamList[0];
	uart_printf("Command : %s\r\n", cmd);

	if(strcmp(cmd, "get") == 0)
	{
		net_conf_t conf;
		net_init();
		dhcp_request(&conf);
	}

	return 0;
}

UINT32 CLineCmdTestCeva(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    uart_printf ("\r\n");
    DspDrvCevaPoolBist();
    return 0;
}

#ifdef MX_HEAP_LEAKAGE_CONTROL
UINT32 CLineCmdPackHeap(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 nSize = 0, rc;
    HANDLE h;
    char * pName = NULL;
    UINT32 id;

    if (pParams->ParamCount == 0)
    {
        uart_printf ("packheap <app>\r\n");
        uart_printf ("packheap <dsp>\r\n");
        uart_printf ("packheap <ncnb>\r\n");
        uart_printf ("packheap <iram>\r\n");
        uart_printf ("packheap <ddr1>\r\n");
        uart_printf ("packheap <ddr1ncnb>\r\n");

        return 0;
    }

    if (!_strcmp (pParams->ParamList[0], "app"))
    {
        id = 0;
        h = AppHeap;
        pName = "AppHeap";
    }
    else if (!_strcmp (pParams->ParamList[0], "dsp"))
    {
        id = 1;
        h = DspHeap;
        pName = "DspHeap";
    }
    else if (!_strcmp (pParams->ParamList[0], "ncnb"))
    {
        id = 2;
        h = NcnbHeap;
        pName = "NcnbHeap";
    }
    else if (!_strcmp (pParams->ParamList[0], "iram"))
    {
        id = 3;
        h = IramHeap;
        pName = "IramHeap";
    }
    else if (!_strcmp (pParams->ParamList[0], "ddr1"))
    {
        id = 4;
        h = DDR1Heap;
        pName = "DDR1Heap";
    }
    else if (!_strcmp (pParams->ParamList[0], "ddr1ncnb"))
    {
        id = 5;
        h = DDR1NcnbHeap;
        pName = "DDR1NcnbHeap";
    }
    else
    {
        id = 6;
        h = HNULL;
        pName = "HNULL heap";
    }

    nSize = MxHeapGetPacketSize(h);

    if (nSize > PACK_HEAP_INFO_BUF)
    {
        printf ("Due to memory size, it's impossible to save [%s] heap state, it's neded [%d] bytes\r\n", pName, nSize);
    }
    else
    {
        uart_printf ("Packing [%s] heap, pack size [%d] bytes ... ", pName, nSize);

        rc = SvsrPackHeap (h, id, PackHeapPtr, PACK_HEAP_INFO_BUF);

        if (rc == MX_OK)
        {
            uart_printf ("OK\r\nStorage:[0x%08x], packed size:[%d] bytes\r\n", PackHeapPtr, nSize);
        }
        else
        {
            uart_printf ("Error, rc=0x%08x\r\n", rc);
        }
    }

    return 0;
}

#else
UINT32 CLineCmdPackHeap(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    uart_printf ("Leakage control is not supported\r\n");
    return 0;
}
#endif

UINT32 CLineCmdSysInfo(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    extern UINT8 Image$$ROOT_RO$$Base [];
    extern UINT8 Image$$ROOT_RWZI$$Base [];
    extern UINT8 Image$$NCNB_DATA$$Base [];

    extern UINT8 Image$$ROOT_RO$$Length [];
    extern UINT8 Image$$ROOT_RWZI$$Length [];
    extern UINT8 Image$$NCNB_DATA$$Length[];
    extern UINT32 ExtCodeDataLen;
    extern const unsigned cevabincode_size;
    extern const LCoreCfg CoreCfg;

    HEAPINFO app, dsp, ncnb, ddr1, ddr1ncnb;
    UINT8 mx_hver = 0, mx_lver = 0;
    UINT32 arm_cc_ver = 0, numPart, totalSize, h, m, s, tmp;
    PUINT8 storageLoc;
    char * pCompMode = "HBI";

    #ifdef __CC_ARM
    arm_cc_ver = __ARMCC_VERSION;
    #endif

    tmp = MxGetTime() / 1000;

    h = (tmp / (60*60)) % 24;
    m = (tmp / (60   )) % 60;
    s = (tmp          ) % 60;

    MxGetVersion (&mx_hver, &mx_lver);

    GetHeapInfo(ID_HEAP_APP, &app);
    GetHeapInfo(ID_HEAP_DSP, &dsp);
    GetHeapInfo(ID_HEAP_NCNB, &ncnb);
    GetHeapInfo(ID_HEAP_DDR1, &ddr1);
    GetHeapInfo(ID_HEAP_DDR1_NCNB, &ddr1ncnb);

    uart_printf ("\r\n[L-ARM system information]\r\n");
    uart_printf ("--------------------------\r\n");

    if (DevInfoIsT4Kx4 ())
    {
        uart_printf ("        Device : T4K x4\r\n");
    }
    else if (DevInfoIsT4Kx6())
    {
        uart_printf ("        Device : T4K x6\r\n");
    }
    else if (DevInfoIsT4Kx7())
    {
        uart_printf ("        Device : T4K x7\r\n");
    }
    else
    {
        uart_printf ("        Device : T3K\r\n");
    }

    if ((UINT32)Image$$ROOT_RO$$Base == 0x8000000)
    {
        pCompMode = "128m";
    }
    else if ((UINT32)Image$$ROOT_RO$$Base == 0x10000000)
    {
        pCompMode = "256m";
    }
    else if ((UINT32)Image$$ROOT_RO$$Base == 0x20000000)
    {
        pCompMode = "512m";
    }
    else if ((UINT32)Image$$ROOT_RO$$Base == 0x28000000)
    {
        pCompMode = "640m";
    }

    uart_printf ("          4GMX : %d.%02d\r\n", mx_hver, mx_lver);
    uart_printf ("       Syscore : %d.%02d\r\n", APP_H_VER, APP_L_VER);
    #ifdef _MX_DEBUG
    uart_printf ("   Compilation : debug\r\n");
    #else
    uart_printf ("   Compilation : release\r\n");
    #endif
    uart_printf ("     Comp mode : %s\r\n", pCompMode);
    uart_printf ("    Build date : %s\r\n", __DATE__);
    uart_printf ("    Build time : %s\r\n", __TIME__);
    #ifdef __CC_ARM
    uart_printf ("RVDS comp. ver : %d.%02d build %d\r\n", (arm_cc_ver / 100000), (arm_cc_ver / 1000) % 100, (arm_cc_ver % 1000));
    #endif
    uart_printf ("ARM cluster id : %d\r\n", MxGetARMID());
    uart_printf ("   ARM core id : %d\r\n", MxGetCpuID());
    uart_printf (" ARM cores num : %d\r\n", SysGetCpuNum());
    uart_printf ("ARM L1 i-cache : %s\r\n", (sys_read_control() & CR_I) ? "on" : "off");
    uart_printf ("ARM L1 d-cache : %s\r\n", (sys_read_control() & CR_C) ? "on" : "off");
    uart_printf ("  ARM L2 cache : %s\r\n", (REG32(MX_ARM_L2_BASE + L2X0_CTRL) & 1) ? "on" : "off");
    uart_printf ("      Time(ms) : %d\r\n", MxGetTime());
    uart_printf (" Frac-time(ms) : %d\r\n", MxGetFracTime());
    uart_printf ("          Time : %02d:%02d:%02d\r\n", h, m, s);
    uart_printf (" HW-DDR-0 base : 0x%x\r\n", GetDDR0Base ());
    uart_printf (" HW-DDR-0 size : %dMB\r\n", GetDDR0Size () / (1024*1024));

#ifdef NO_USE_DDR1
    uart_printf ("vrt DDR-1 base : 0x%x\r\n", GetDDR1Base ());
    uart_printf ("vrt DDR-1 size : %dMB\r\n", GetDDR1Size () / (1024*1024));
#else
    uart_printf (" HW-DDR-1 base : 0x%x\r\n", (REG32(CFG_SYS_DDR_LL1) << 20));
    uart_printf (" HW-DDR-1 size : %dMB\r\n", ((REG32(CFG_SYS_DDR_HL1) << 20) - (REG32(CFG_SYS_DDR_LL1) << 20)) / (1024*1024));
#endif

    uart_printf ("  RO code base : 0x%x\r\n", (UINT32)Image$$ROOT_RO$$Base);
    uart_printf ("  RO code size : %d\r\n", (UINT32)Image$$ROOT_RO$$Length);
    uart_printf ("     RWZI base : 0x%x\r\n", (UINT32)Image$$ROOT_RWZI$$Base);
    uart_printf ("     RWZI size : %d\r\n", (UINT32)Image$$ROOT_RWZI$$Length);
    uart_printf ("     NCNB base : 0x%x\r\n", (UINT32)Image$$NCNB_DATA$$Base);
    uart_printf ("     NCNB size : %d\r\n", (UINT32)Image$$NCNB_DATA$$Length);
    uart_printf ("CEVA file size : %d\r\n", (UINT32)cevabincode_size);
    uart_printf ("CEVA pass mask : 0x%x\r\n", DspDrvGetDevMask());
    uart_printf (" CEVA dump ptr : 0x%x\r\n", DspDrvGetDumpAddr(0));
    uart_printf ("    I-CPU base : 0x%x\r\n", CoreCfg.ICPUBase);
    for (numPart = 0; numPart < SVSR_ICORE_NUM; numPart++)
    {
        totalSize = ICorePartListCfg[numPart].BlockNum * ICorePartListCfg[numPart].BlockSize;
        storageLoc = (PUINT8)ICorePartListCfg[numPart].StorPtr;
        uart_printf ("I-CORE[%d] info : Base[%x - %x], Num:[%d] Size:[%d] T-Size:[%d]\r\n",
                       numPart, storageLoc, storageLoc + totalSize - 1,
                       ICorePartListCfg[numPart].BlockNum, ICorePartListCfg[numPart].BlockSize,
                       totalSize);
    }

    if (CoreCfg.HostIntID == IC_CHAN_SVSR)
        uart_printf ("    I-CPU type : I-CPU v1\r\n");
    else if (CoreCfg.HostIntID == IC_CHAN_SVSR_EX_ICPU)
        uart_printf ("    I-CPU type : I-CPU v2\r\n");
    else
        uart_printf ("    I-CPU type : HBI\r\n");

    if (app.bInited)
    {
        uart_printf (" APP heap base : 0x%x\r\n", app.BaseAddr);
        uart_printf (" APP heap size : %dMb\r\n", app.Size/ (1024*1024));
    }

    if (dsp.bInited)
    {
        uart_printf (" DSP heap base : 0x%x\r\n", dsp.BaseAddr);
        uart_printf (" DSP heap size : %dKb\r\n", dsp.Size/ (1024));
    }

    if (ncnb.bInited)
    {
        uart_printf ("NCNB heap base : 0x%x\r\n", ncnb.BaseAddr);
        uart_printf ("NCNB heap size : %dMb\r\n", ncnb.Size/ (1024*1024));
    }

    if (ddr1.bInited)
    {
        uart_printf ("DDR1 heap base : 0x%x\r\n", ddr1.BaseAddr);
        uart_printf ("DDR1 heap size : %dMb\r\n", ddr1.Size / (1024*1024));
    }

    if (ddr1ncnb.bInited)
    {
        uart_printf (" DDR1ncnb base : 0x%x\r\n", ddr1ncnb.BaseAddr);
        uart_printf (" DDR1ncnb size : %dMb\r\n", ddr1ncnb.Size/ (1024*1024));
    }


    return 0;
}

UINT32 CLineDDRProtectionTest(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 ddrId = 0;
    UINT32 laddr = 0x000000;
    UINT32 uaddr = 0x000000;

    unsigned int i;
    unsigned int datasize = 512;
    unsigned char* pSrcBuf;
    unsigned char* pDstBuf;
    PMDMA_FDESC pFDesc;
    MDMAHWCTX HWContext;

    if (pParams->ParamCount < 3)
    {
        uart_printf ("USAGE: ddrprotect <0|1> <lower addr> <upper addr>\r\n");
        return RC_CM_PARAM_ERROR;
    }

    ddrId = CMgrGetParam (pParams, 0, 0);
    laddr = CMgrGetParam (pParams, 1, 0);
    uaddr = CMgrGetParam (pParams, 2, 0);

    SysDDREnableWriteProtect(ddrId, 0, laddr, uaddr);

    uart_printf("DDR controller protection 0x%08x - 0x%08x\r\n", laddr, uaddr);

    // source data buffer
    pSrcBuf = MxHeapAlloc(NcnbHeap, datasize);
    if (pSrcBuf == NULL)
    {
        printf("Cannot allocate the buffer.\n");
        return RC_CM_PARAM_ERROR;
    }
    memset(pSrcBuf, 0, datasize);

    // destination data buffer
    pDstBuf = (UINT8*) ROUND((uaddr - laddr)/2, 32);

    if (pDstBuf == NULL)
    {
        printf("Cannot allocate the buffer.\n");
        MxHeapFree(NcnbHeap, pSrcBuf);
        return RC_CM_PARAM_ERROR;
    }
//    memset(pDstBuf, 0, datasize);

    // initialization of source buffer;
    for (i = 0; i < datasize; i++)
    {
        if (i > 255)
        {
            *(pSrcBuf + i) = i % 256;
            continue;
        }
        *(pSrcBuf + i) = i;
    }

    // initialization of radio MDMA driver
    if (MdmaRadInit() != MX_OK)
    {
        printf ("Cannot initialize radio MDMA driver.\n");
        return RC_CM_PARAM_ERROR;
    }

    // creating frame descriptor
    pFDesc = MdmaAllocDesc(MDMA_HW_RAD, 1);
    if (pFDesc == NULL)
    {
        printf ("Cannot allocate memory for frame descriptors.\n");
        return RC_CM_PARAM_ERROR;
    }

    // creating context
    memset (&HWContext, 0, sizeof(MDMAHWCTX));
    MdmaSetHwCtx(pFDesc, HWContext);

    // filling buffer descriptors in frame descriptor
    MdmaSetData (pFDesc, pSrcBuf, datasize, pDstBuf, datasize);

    // Turn on RAD MDMA IRQ
    MdmaSetLastDesc(pFDesc, NULL, NULL);

    // start MDMA
//    *(PUINT32)(MDMA_RAD_CTRL_REGS_BASE + 4) = (UINT32)pFDesc;
    printf ("Frame descriptor address: 0x%08X\n", (UINT32)pFDesc);
    printf ("Source buffer address: 0x%08X\n", (UINT32)pSrcBuf);
    printf ("Destination buffer address: 0x%08X\n", (UINT32)pDstBuf);
    printf ("radio MDMA register's status:\n");
    printf ("0x%08X\n", *(PUINT32)MDMA_RAD_CTRL_REGS_BASE);
    printf ("0x%08X\n", *(PUINT32)(MDMA_RAD_CTRL_REGS_BASE + 4));
    printf ("0x%08X\n", *(PUINT32)(MDMA_RAD_CTRL_REGS_BASE + 8));
    printf ("0x%08X\n", *(PUINT32)(MDMA_RAD_CTRL_REGS_BASE + 12));
    printf ("0x%08X\n", *(PUINT32)(MDMA_RAD_CTRL_REGS_BASE + 16));
    printf ("0x%08X\n", *(PUINT32)(MDMA_RAD_CTRL_REGS_BASE + 20));
    printf ("Frame Descriptor:\n");
    printf ("0x%08X\n", *(PUINT32)pFDesc);
    printf ("0x%08X\n", *(PUINT32)((UINT32)pFDesc + 4));
    printf ("0x%08X\n", *(PUINT32)((UINT32)pFDesc + 8));
    printf ("0x%08X\n", *(PUINT32)((UINT32)pFDesc + 12));
    printf ("0x%08X\n", *(PUINT32)((UINT32)pFDesc + 16));
    printf ("0x%08X\n", *(PUINT32)((UINT32)pFDesc + 24));
    printf ("start radio MDMA...\n");
//    MdmaRun (MDMA_HW_RAD, pFDesc);
    MdmaRunEx(MDMA_HW_RAD, pFDesc, NULL, NULL);
//    printf ("0x%08X\n", *(PUINT32)MDMA_RAD_CTRL_REGS_BASE);
    printf (" Head pointer register: 0x%08X\n", *(PUINT32)(MDMA_RAD_CTRL_REGS_BASE + 4));


    printf ("input data:\n");
    for (i = 0; i < datasize; i++)
    {
        if (!(i % 8))
        {
            printf ("%d, ", *(pSrcBuf + i));
            MxDelay (200);
        }
    }

    printf ("\n output data:\n");
    for (i = 0; i < datasize; i++)
    {
        int data;
        if (!(i % 8))
        {
            MxSafeRead32 ((PUINT32)((UINT32)(pDstBuf + i) & ~3), (PUINT32)&data);
            printf ("%d, ", data);
            MxDelay (200);
        }
    }

    // empty the memory
    //MdmaFreeDesc (MDMA_HW_RAD, pFDesc);
    MxHeapFree (NcnbHeap, pSrcBuf);
//    MxHeapFree (NcnbHeap, pDstBuf);

    MxDelay(500);
    printf ("\nNumber of ticks: %d\n", MdmaGetExecTicks(MDMA_HW_RAD));
    printf ("Master CPU is: %d\n", MdmaRadGetMasterCpu());

    printf ("\n the test ended.\n");

    return 0;
}

HANDLE hMySMPThread = {0};
HANDLE hMySMPDiv = {0};
VUINT32 MySMPThreadNum = 0;
VUINT32 MySMPLock = 0;

UINT32 MySMP_Cpu2Diverter(void *p)
{
    MxDelayTicks(150);

    return 0;
}

UINT32 MySMPThread (void * p)
{
    HANDLE h = MxGetCurrentThread ();
    MXRC rc = MX_OK;

    if (MySMPThreadNum == 0)
        return 0;

    MySMPThreadNum --;

    if (MySMPLock != 0)
    {
        uart_printf ("SMP double scheduling is detected!!! cpu=%d\r\n", MxGetCpuID());
        return 0;
    }

    rc = MxScheduleThreadEx (hMySMPDiv, 0, 0);
    _ASSERT_RC(rc);

    MySMPLock = 1;

    MxScheduleThreadEx (hMySMPThread, 0, SCHED_SMP);

    uart_printf ("Started on CPU=%d, reg-h=%x, loc-h=%x\r\n", MxGetCpuID (), HANDLE_TOINT(hMySMPThread), HANDLE_TOINT(h));

    MySMPLock = 0;

    return rc;
}

UINT32 CLineSMPTest(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    MXRC rc = MX_OK;

    MySMPThreadNum = 10;
    MySMPLock = 0;

    if (HANDLE_NULL(hMySMPThread))
    {
        rc = MxCreateThread (MySMPThread, NULL, NULL, 0, &hMySMPThread);

        if (FAILED(rc))
        {
            uart_printf ("Create SMP thread error! rc=%x\r\n", rc);
            return rc;
        }

        rc = MxCreateThreadEx (2, MySMP_Cpu2Diverter, NULL, NULL, 0, &hMySMPDiv);

        if (FAILED(rc))
        {
            uart_printf ("Create SMP diverted thread error! rc=%x\r\n", rc);
            return rc;
        }
    }

    uart_printf ("\r\n");

    rc = MxScheduleThreadEx (hMySMPDiv, 0, 0);
    _ASSERT_RC(rc);

    rc = MxScheduleThreadEx (hMySMPThread, 0, SCHED_SMP);
    _ASSERT_RC(rc);

    return MX_OK;
}
#ifdef PTM_ENABLED

char *DumpBuf = NULL;
UINT32 CLineCmdPTMTraceTest(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    char *cmd;

    uart_printf (" call CLineCmdPTMTraceTest \r\n");

    if(pParams->ParamCount == 0)
	{
		uart_printf("Need set command \r\n");
	}

    cmd = pParams->ParamList[0];
	if(strcmp(cmd, "init") == 0)
	{
		trace_debug(DEBUG_FUNC_INIT, 0, NULL);
	}
	else if(strcmp(cmd, "cfg") == 0)
	{
	    UINT8      PtmID;
	    PtmConfig  PtmCfg;

		if(pParams->ParamCount < 9)
	    {
		    uart_printf("Need more than 9 parameters for config \r\n");
	    }

		PtmID = StrToNum(pParams->ParamList[1]);

        memset(&PtmCfg, 0, sizeof(PtmConfig));
		PtmCfg.BranchOutput      = StrToNum(pParams->ParamList[2]);
		PtmCfg.CycleAccurate     = StrToNum(pParams->ParamList[3]);
		PtmCfg.TimestampEnable   = StrToNum(pParams->ParamList[4]);
		PtmCfg.TraceEnableActive = StrToNum(pParams->ParamList[5]);
		PtmCfg.TraceRangeExclude = StrToNum(pParams->ParamList[6]);
		PtmCfg.TraceRange        = StrToNum(pParams->ParamList[7]);
		PtmCfg.TraceEnableEvt    = StrToNum(pParams->ParamList[8]);
	    trace_debug(DEBUG_FUNC_CFG, PtmID, &PtmCfg);
	}
	else if(strcmp(cmd, "ctrl") == 0)
	{
	    UINT8      PtmID;
	    UINT32     enable;

		if(pParams->ParamCount < 2)
	    {
		    uart_printf("Need more than 2 parameters for config \r\n");
	    }

		PtmID  = StrToNum(pParams->ParamList[1]);
		enable = StrToNum(pParams->ParamList[2]);

	    trace_debug(DEBUG_FUNC_TRACE, PtmID, &enable);
	}
	else if(strcmp(cmd, "access") == 0)
	{
	    DataInfo   di;
		int  i;

        if(DumpBuf == NULL)
        {
		    DumpBuf = MxHeapAlloc(DDR1NcnbHeap, 4* 0x800);
        }

		di.addr = (void *)DumpBuf;
		di.len  = 4 * 0x800;

		uart_printf("trace buf, addr =0x%x, length %d \r\n", di.addr, di.len);
		trace_debug(DEBUG_FUNC_ACCESS, 0, &di);

	}
	else if(strcmp(cmd, "dump") == 0)
	{
		trace_debug(DEBUG_FUNC_DUMP, 0, NULL);
	}
	else if(strcmp(cmd, "rd") == 0)
	{
	    UINT8      PtmID;
	    RegInfo    ri;

		if(pParams->ParamCount < 4)
	    {
		    uart_printf("Need more than 4 parameters for config \r\n");
	    }

		PtmID = StrToNum(pParams->ParamList[1]);
		ri.ucRegType = StrToNum(pParams->ParamList[2]);
		ri.ucOffset  = StrToNum(pParams->ParamList[3]);
		trace_debug(DEBUG_FUNC_REGRD, PtmID, &ri);
	}
	else if(strcmp(cmd, "wr") == 0)
	{
	    UINT8      PtmID;
	    RegInfo    ri;

		if(pParams->ParamCount < 5)
	    {
		    uart_printf("Need more than 5 parameters for config \r\n");
	    }

		PtmID = StrToNum(pParams->ParamList[1]);
		ri.ucRegType = StrToNum(pParams->ParamList[2]);
		ri.ucOffset  = StrToNum(pParams->ParamList[3]);
		ri.ulValue   = StrToNum(pParams->ParamList[4]);
		trace_debug(DEBUG_FUNC_REGWR, PtmID, &ri);
	}
    else if(strcmp(cmd, "free") == 0)
    {
        if(DumpBuf)
        {
            MxHeapFree(DDR1NcnbHeap, DumpBuf);
			DumpBuf = NULL;
        }
    }



    return 0;
}
#endif


UINT32 CLineRexTest(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    MXRC rc = 0;

    UINT32 nMode = CMgrGetParam(pParams, 0, 0);

    switch (nMode)
    {
        case 0:
            uart_printf ("REX: TCB priorities check\r\n");
            rc = RexDiagCheckPriors();
            break;

        case 1:
            uart_printf ("REX: 60 CEVA TCBs\r\n");
            rc = RexDiagCheckSimpleList();
            break;

        case 2:
            uart_printf ("REX: SuperTCB with 60 CEVA TCBs\r\n");
            rc = RexDiagCheckSuperTCB();
            break;

        default:
            uart_printf ("Unknown mode, mode=%d\r\n", nMode);
            rc = 1;
            break;
    }

    if (FAILED(rc))
        uart_printf ("FAILED, rc = 0x%08x\r\n", rc);

    return 0;
}

UINT32 SchedTotalTime = 0;
UINT32 SchedMaxTime = 0;
UINT32 SchedMinTime = 0;
UINT32 SchedTotalNum = 0;
UINT32 SchedProcNum = 0;
HANDLE SchedThread;
VUINT32 RSchedToWait=0;

void AddSchedTime (UINT32 nTime)
{
    SchedTotalNum ++;
    SchedTotalTime += nTime;

    SchedMaxTime = MAX(SchedMaxTime, nTime);

    if (SchedMinTime != 0)
        SchedMinTime = MIN(SchedMinTime, nTime);
    else
        SchedMinTime = nTime;
}

UINT32 LocalThread (void * p)
{
    UINT32 t;
    MXRC rc;
    IRQTYPE irq;

    if (SchedProcNum == 0)
    {
        uart_printf ("Sched for %d is [avg:%d, max:%d, min:%d]\r\n",

                SchedTotalNum,
                SchedTotalTime / SchedTotalNum,
                SchedMaxTime,
                SchedMinTime);

        return 0;
    }

    SchedProcNum--;

    irq = ARM_INT_disable();

    t = MxGetTicks();
    rc = MxScheduleThread(SchedThread);
    t = MxGetTicks() - t;

    ARM_INT_restore(irq);

    AddSchedTime (t);

    rc = rc;

    return 0;
}


/**
    @brief This function is designed to measure time needed to schedule the local thread
*/

UINT32 CLineSchedMeasurement(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    MXRC rc;
    UINT32 t;
    IRQTYPE irq;

    SchedProcNum    = 100;
    SchedTotalTime  = 0;
    SchedMaxTime    = 0;
    SchedMinTime    = 0;
    SchedTotalNum   = 0;

    if (HANDLE_NULL(SchedThread))
    {
        rc = MxCreateThread (LocalThread, NULL, 0, 0, &SchedThread);

        if (FAILED(rc))
        {
            uart_printf ("Create thread error\r\n");
            return 0;
        }
    }

    irq = ARM_INT_disable();

    t = MxGetTicks();
    rc = MxScheduleThread(SchedThread);
    t = MxGetTicks() - t;

    ARM_INT_restore(irq);

    AddSchedTime (t);

    return 0;
}

HANDLE RhSchedThread;

UINT32 RSchedThread (void * p)
{
    RSchedToWait = 1;

    return 0;
}

UINT32 CLineRSchedMeasurement(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    MXRC rc;
    UINT32 t;
    IRQTYPE irq;

    SchedProcNum    = 100;
    SchedTotalTime  = 0;
    SchedMaxTime    = 0;
    SchedMinTime    = 0;
    SchedTotalNum   = 0;

    if (HANDLE_NULL(RhSchedThread))
    {
        rc = MxCreateThreadEx (1, RSchedThread, NULL, 0, 0, &RhSchedThread);

        if (FAILED(rc))
        {
            uart_printf ("Create r-thread error\r\n");
            return 0;
        }
    }

    irq = ARM_INT_disable();

    RSchedToWait = 0;

    while (SchedProcNum --)
    {
        t = MxGetTicks();
        rc = MxScheduleThread(RhSchedThread);
        t = MxGetTicks() - t;

        AddSchedTime (t);

        while (RSchedToWait == 0) _NOP();

        RSchedToWait = 0;
    }

    ARM_INT_restore(irq);

    uart_printf ("R-Sched for %d is [avg:%d, max:%d, min:%d]\r\n",

                SchedTotalNum,
                SchedTotalTime / SchedTotalNum,
                SchedMaxTime,
                SchedMinTime);

    rc = rc;

    return 0;
}

HANDLE SwThrA, SwThrB;
UINT32 SwTimeBegin;
IRQTYPE SwIrqCtrl;

UINT32 SwThreadA (void * p)
{
    MxScheduleThread (SwThrB);
    SwTimeBegin = MxGetTicks();
    return 0;
}

UINT32 SwThreadB (void * p)
{
    AddSchedTime (MxGetTicks() - SwTimeBegin);

    if (SchedProcNum == 0)
    {
        uart_printf ("Sw-time for %d is [avg:%d, max:%d, min:%d]\r\n",

                SchedTotalNum,
                SchedTotalTime / SchedTotalNum,
                SchedMaxTime,
                SchedMinTime);

        ARM_INT_restore (SwIrqCtrl);
        return 0;
    }

    SchedProcNum --;

    MxScheduleThread (SwThrA);
    return 0;
}

UINT32 CLineThrSwMeasurement(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    MXRC rc;

    SchedProcNum    = 100;
    SchedTotalTime  = 0;
    SchedMaxTime    = 0;
    SchedMinTime    = 0;
    SchedTotalNum   = 0;

    if (HANDLE_NULL(SwThrA))
    {
        rc = MxCreateThread (SwThreadA, NULL, 0, 0, &SwThrA);

        if (FAILED(rc))
        {
            uart_printf ("Create a-thread error\r\n");
            return 0;
        }
    }

    if (HANDLE_NULL(SwThrB))
    {
        rc = MxCreateThread (SwThreadB, NULL, 0, 0, &SwThrB);

        if (FAILED(rc))
        {
            uart_printf ("Create b-thread error\r\n");
            return 0;
        }
    }

    SwIrqCtrl = ARM_INT_disable ();

    rc = MxScheduleThread(SwThrA);

    return 0;
}

void CheckPart (char * pPartName, HANDLE heap, UINT32 nNum, UINT32 nSize, UINT32 nSync)
{
    UINT32 t = 0, i = 0;
    UINT32 nTotal = 0, Max = 0, Min = 0;
    HANDLE hPart;
    void * pStor, *pBlock;
    MXRC rc;
    IRQTYPE irq;

    pStor = MxHeapAlloc (heap, nNum * nSize);

    if (pStor == NULL)
    {
        uart_printf ("Heap allocation errorr\n");
        return;
    }

    rc = MxCreatePart(pStor, nNum, nSize, NULL, HNULL, HNULL, &hPart);

    if (FAILED(rc))
    {
        MxHeapFree (heap, pStor);
        uart_printf ("Partition create errorr\n");
        return;
    }

    if (nSync)
        MxShareObject (hPart, OBJ_SHARE_ON);

    irq = ARM_INT_disable ();

    while (i < nNum)
    {
        t = MxGetTicks();
        pBlock = MxAllocBlock(hPart);
        t = MxGetTicks() - t;

        if (pBlock == NULL)
            uart_printf ("Part alloc error!!!r\n");

        nTotal += t;

        Max = MAX(t, Max);

        if (Min == 0)
            Min = t;
        else
            Min = MIN(t, Min);

        i++;
    }

    ARM_INT_restore (irq);

    uart_printf ("Part (share:%s) (heap: %s )(num=%d, size=%d): avg:%d max:%d min:%d\r\n", nSync ? "ON" : "OFF", pPartName, nNum, nSize, nTotal / nNum, Max, Min);

    if (nSync)
        MxShareObject (hPart, OBJ_SHARE_OFF);

    MxRemovePart (hPart);
    MxHeapFree (heap, pStor);

}

void CheckQ (char * pHeapName, HANDLE heap, UINT32 nNum, UINT32 nSync)
{
    UINT32 t = 0, i = 0;
    UINT32 nTotal = 0, Max = 0, Min = 0;
    HANDLE hQ;
    void * pStor;
    MXRC rc = 0;
    IRQTYPE irq;

    pStor = MxHeapAlloc (heap, (nNum + 1) * sizeof (UINT32));

    if (pStor == NULL)
    {
        uart_printf ("Heap allocation errorr\n");
        return;
    }

    rc = MxCreateQueue(pStor, nNum + 1, sizeof (UINT32), NULL, HNULL, &hQ);

    if (FAILED(rc))
    {
        MxHeapFree (heap, pStor);
        uart_printf ("Queue create error\n");
        return;
    }

    if (nSync)
        MxShareObject (hQ, OBJ_SHARE_ON);

    irq = ARM_INT_disable ();

    while (i < nNum)
    {
        t = MxGetTicks();
        MxGetQueueData(hQ, (LPVOID)&rc);
        t = MxGetTicks() - t;

        nTotal += t;

        Max = MAX(t, Max);

        if (Min == 0)
            Min = t;
        else
            Min = MIN(t, Min);

        i++;
    }

    uart_printf ("PUT-Queue (share:%s) (heap: %s )(num=%d): avg:%d max:%d min:%d\r\n", nSync ? "ON" : "OFF", pHeapName, nNum, nTotal / nNum, Max, Min);

    t = 0;
    i = 0;
    nTotal = 0;
    Max = 0;
    Min = 0;

    while (i < nNum)
    {
        t = MxGetTicks();
        MxGetQueueData(hQ, (LPVOID)t);
        t = MxGetTicks() - t;

        nTotal += t;

        Max = MAX(t, Max);

        if (Min == 0)
            Min = t;
        else
            Min = MIN(t, Min);

        i++;
    }

    uart_printf ("GET-Queue (share:%s) (heap: %s )(num=%d): avg:%d max:%d min:%d\r\n", nSync ? "ON" : "OFF", pHeapName, nNum, nTotal / nNum, Max, Min);


    ARM_INT_restore (irq);
    if (nSync)
        MxShareObject (hQ, OBJ_SHARE_OFF);

    MxRemoveQueue(hQ);
    MxHeapFree (heap, pStor);

}

void CheckInvalidate (UINT32 nNum, UINT32 nSize)
{
    UINT32 t = 0, i = 0;
    UINT32 nTotal = 0, Max = 0, Min = 0;
    IRQTYPE irq;
    void * pStor;

    pStor = MxHeapAlloc (AppHeap, nSize);

    if (pStor == NULL)
    {
        uart_printf ("Memory allocation error\r\n");
        return;
    }

    irq = ARM_INT_disable();

    while (i < nNum)
    {
        t = MxGetTicks();
        MxCacheInvalidate (pStor, nSize);
        t = MxGetTicks() - t;

        nTotal += t;

        Max = MAX(t, Max);

        if (Min == 0)
            Min = t;
        else
            Min = MIN(t, Min);

        i++;
    }

    ARM_INT_restore(irq);

    uart_printf ("Invalidate (size=%d, num=%d): avg:%d max:%d min:%d\r\n", nSize, nNum, nTotal / nNum, Max, Min);
}

void CheckClean (UINT32 nNum, UINT32 nSize)
{
    UINT32 t = 0, i = 0;
    UINT32 nTotal = 0, Max = 0, Min = 0;
    IRQTYPE irq;
    void * pStor;

    pStor = MxHeapAlloc (AppHeap, nSize);

    if (pStor == NULL)
    {
        uart_printf ("Memory allocation error\r\n");
        return;
    }

    irq = ARM_INT_disable();

    while (i < nNum)
    {
        memset (pStor, i, nSize);

        t = MxGetTicks();
        MxCacheClean (pStor, nSize);
        t = MxGetTicks() - t;

        nTotal += t;

        Max = MAX(t, Max);

        if (Min == 0)
            Min = t;
        else
            Min = MIN(t, Min);

        i++;
    }

    ARM_INT_restore(irq);

    uart_printf ("Clean (size=%d, num=%d): avg:%d max:%d min:%d\r\n", nSize, nNum, nTotal / nNum, Max, Min);
}


UINT32 CLineQPMeasurement(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    CheckPart ("AppHeap", AppHeap, 128, 128, 0);
    CheckPart ("AppHeap", AppHeap, 128, 128, 1);

    CheckPart ("NcnbHeap", NcnbHeap, 128, 128, 0);
    CheckPart ("NcnbHeap", NcnbHeap, 128, 128, 1);

    CheckPart ("DspHeap", DspHeap, 128, 128, 0);
    CheckPart ("DspHeap", DspHeap, 128, 128, 1);

    uart_printf ("\r\n\r\n\r\n");

    CheckQ ("AppHeap", AppHeap, 128, 0);
    CheckQ ("AppHeap", AppHeap, 128, 1);

    CheckQ ("NcnbHeap", NcnbHeap, 128, 0);
    CheckQ ("NcnbHeap", NcnbHeap, 128, 1);

    CheckQ ("DspHeap", DspHeap, 128, 0);
    CheckQ ("DspHeap", DspHeap, 128, 1);

    uart_printf ("\r\n\r\n\r\n");

    CheckInvalidate (128, 256);
    CheckInvalidate (128, 1024);
    CheckInvalidate (128, 2048);
    CheckInvalidate (128, 4096);
    CheckInvalidate (128, 16*1024);
    CheckInvalidate (128, 1024*1024);

    uart_printf ("\r\n");

    CheckClean(128, 256);
    CheckClean(128, 1024);
    CheckClean(128, 2048);
    CheckClean(128, 4096);
    CheckClean(128, 16*1024);
    CheckClean(128, 1024*1024);

    return 0;
}

#ifdef PM_ENABLED

#define __SIZE_CACHABLE_ARRAY       5000


volatile unsigned char *varCacheable;
VUINT32 run = 0;

UINT32 PMTestThread (void * pThreadParam)
{
    UINT32 cpu = MxGetCpuID();
    VUINT32 k,read;

    run = 1;
    while(1)
        if (run == 2)
        {
            for (k = 0; k < __SIZE_CACHABLE_ARRAY; ++k)
                read = varCacheable[k];

            run = 0;
            break;
        }

    return 0;
}

UINT32 CLinePMCacheTest(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{

    UINT32 i;
    IRQTYPE irq;
    HANDLE h;
    VUINT32 k,read;
    VUINT32 pm_t, pm_end_t;

    uart_printf("Performance Monitors: testing... \r\n");

    varCacheable = (volatile unsigned char *) MxHeapAlloc(AppHeap, __SIZE_CACHABLE_ARRAY);

    irq = ARM_INT_disable();

    for (i = 0; i < CPU_NUM; ++i)
    {
        uart_printf("\r\n CPU-%d: \r\n", i);


        uart_printf("After operation cache invalidate\r\n", i);
        MxCacheInvalidate ((LPVOID)varCacheable, __SIZE_CACHABLE_ARRAY);

        if(i)
        {
            MxCreateThreadEx(i, PMTestThread, NULL, NULL, (TPRIORITY)0, &h);
            MxScheduleThread(h);
            while(!run);
        }

        PMStartCalcCycleEx(i);
        PMStartCalcDataCacheMissEx(i);
        PMStartCalcDataCacheHitEx(i);
        pm_t = MxGetTicks();

        if(i)
        {
            run = 2;
            while(run);
        }
        else
        {
            for (k = 0; k < __SIZE_CACHABLE_ARRAY; ++k)
                read = varCacheable[k];
        }
        pm_end_t = MxGetTicks();

        PMStopCalcDataCacheMissEx(i);
        PMStopCalcDataCacheHitEx(i);
        PMStopCalcCycleEx(i);

        uart_printf("missed = 0x%x\r\n", PMGetDataCacheMissNumberEx(i));
        uart_printf("hit = 0x%x\r\n", PMGetDataCacheHitNumberEx(i));
        uart_printf("clk = 0x0%x\r\n", PMGetCycleNumberEx(i));
        uart_printf("ticks = %d\r\n", pm_end_t - pm_t);

        MxCacheInvalidate((LPVOID)varCacheable, 32*2);

        uart_printf("After operation cache invalidate 2 lines:\r\n");

        if (i)
        {
            MxScheduleThread(h);
            while(!run);
        }

        PMStartCalcCycleEx(i);
        PMStartCalcDataCacheMissEx(i);
        PMStartCalcDataCacheHitEx(i);
        pm_t = MxGetTicks();

        if(i)
        {
            run = 2;
            while(run);
        }
        else
        {
            for (k = 0; k < __SIZE_CACHABLE_ARRAY; ++k)
                read = varCacheable[k];
        }
        pm_end_t = MxGetTicks();

        PMStopCalcDataCacheMissEx(i);
        PMStopCalcDataCacheHitEx(i);
        PMStopCalcCycleEx(i);

        uart_printf("missed = 0x%x\r\n", PMGetDataCacheMissNumberEx(i));
        uart_printf("hit = 0x%x\r\n", PMGetDataCacheHitNumberEx(i));
        uart_printf("clk = 0x0%x\r\n", PMGetCycleNumberEx(i));
        uart_printf("ticks = %d\r\n", pm_end_t - pm_t);

        uart_printf("The data in the cache:\r\n");

        if (i)
        {
            MxScheduleThread(h);
            while(!run);
        }

        PMStartCalcCycleEx(i);
        PMStartCalcDataCacheMissEx(i);
        PMStartCalcDataCacheHitEx(i);
        pm_t = MxGetTicks();

        if(i)
        {
            run = 2;
            while(run);
        }
        else
        {
            for (k = 0; k < __SIZE_CACHABLE_ARRAY; ++k)
                read = varCacheable[k];
        }
        pm_end_t = MxGetTicks();

        PMStopCalcDataCacheMissEx(i);
        PMStopCalcDataCacheHitEx(i);
        PMStopCalcCycleEx(i);

        uart_printf("missed = 0x%x\r\n", PMGetDataCacheMissNumberEx(i));
        uart_printf("hit = 0x%x\r\n", PMGetDataCacheHitNumberEx(i));
        uart_printf("clk = 0x0%x\r\n", PMGetCycleNumberEx(i));
        uart_printf("ticks = %d\r\n", pm_end_t - pm_t);
#if 0
        uart_printf("The data was thrashed out from cache:\r\n");
        #define BUF_SIZE_ALLOC 2*1024*1024
{
        VUINT32 *pSrc = (UINT32 *)MxHeapAlloc(AppHeap, BUF_SIZE_ALLOC);
        VUINT32 *pDst = (UINT32 *)MxHeapAlloc(AppHeap, BUF_SIZE_ALLOC);

        if (pSrc == NULL || pDst == NULL)
        {
            uart_printf("Allocation error\r\n");
            return 0;
        }

        for (k = 0; k < BUF_SIZE_ALLOC; ++k)
            read = pDst[k/sizeof(UINT32)];

        for (k = 0; k < BUF_SIZE_ALLOC; ++k)
            read = pSrc[k/sizeof(UINT32)];

        for (k = 0; k < BUF_SIZE_ALLOC; ++k)
             pDst[k/sizeof(UINT32)] = 0x11;

        for (k = 0; k < BUF_SIZE_ALLOC; ++k)
             pSrc[k/sizeof(UINT32)] = 0x11;

        MxHeapFree(AppHeap, pSrc);
        MxHeapFree(AppHeap, pDst);
}


        PMStartCalcCycleEx(i);
        PMStartCalcDataCacheMissEx(i);
        PMStartCalcDataCacheHitEx(i);
        pm_t = MxGetTicks();

        if(i)
        {
            run = 2;
            while(run);
        }
        else
        {
            for (k = 0; k < __SIZE_CACHABLE_ARRAY; ++k)
                read = varCacheable[k];
        }

        pm_end_t = MxGetTicks();

        PMStopCalcDataCacheMissEx(i);
        PMStopCalcDataCacheHitEx(i);
        PMStopCalcCycleEx(i);

        uart_printf("missed = 0x%x\r\n", PMGetDataCacheMissNumberEx(i));
        uart_printf("hit = 0x%x\r\n", PMGetDataCacheHitNumberEx(i));
        uart_printf("clk = 0x0%x\r\n", PMGetCycleNumberEx(i));
        uart_printf("ticks = %d\r\n", pm_end_t - pm_t);
#endif

        if(i)
        {
            MxRemoveThread(h);
        }
    }

    MxHeapFree(AppHeap, (LPVOID)varCacheable);

    ARM_INT_restore(irq);

    return 0;
}

#endif // #ifdef PM_ENABLED

VUINT32 SigNumProc;

MXRC SigTesterCpu1 (LPVOID pClientCtx, LPVOID pMsg)
{
    //uart_printf ("ARM-mode:%x, cpu-%d: ctx=%x, msg=%x\r\n", (UINT32)MxGetCpuMode(), MxGetCpuID(), pClientCtx, pMsg);

    SigNumProc ++;

    return 0;
}

UINT32 CLineSigTests(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    MXRC rc;
    UINT32 t, total = 0, max = 0, min = 0xFFFFFFFF, num = 100, i;
    LPVOID p;

    rc = SvsrRegMsgHandlerEx(1, 25, SigTesterCpu1, (LPVOID)0x12345678);

    if (FAILED(rc))
    {
        uart_printf ("Registration is failed\r\n");
        return 0;
    }

    i = 0;
    SigNumProc = 0;

    while (i < num)
    {
        p = (LPVOID)SvsrAllocMsg();

        _ASSERT_PTR(p);

        t = MxGetTicks();
        SvsrSendSignal (1, 25, p, SVSR_MSG_OPT_DEALLOC);
        t = MxGetTicks() - t;

        total += t;

        max = MAX(t, max);
        min = MIN(t, min);

        i++;
    }

    uart_printf ("To wait the completion ... ");
    while (SigNumProc != num);
    uart_printf ("Done\r\n\r\n");

    uart_printf ("SIGNALLING AVG:%d, MAX:%d, MIN:%d\r\n", total / num, max, min);

    total = 0;
    max = 0;
    min = 0xFFFFFFFF;

    i = 0;
    SigNumProc = 0;

    while (i < num)
    {
        p = (LPVOID)SvsrAllocMsg();

        _ASSERT_PTR(p);

        t = MxGetTicks();
        SvsrSendMsg (1, 25, p, SVSR_MSG_OPT_DEALLOC);
        t = MxGetTicks() - t;

        total += t;

        max = MAX(t, max);
        min = MIN(t, min);

        i++;
    }

    uart_printf ("To wait the completion ... ");
    while (SigNumProc != num);
    uart_printf ("Done\r\n\r\n");

    uart_printf ("MESSAGING AVG:%d, MAX:%d, MIN:%d\r\n", total / num, max, min);

    return 0;
}


UINT32 CLineICpuChkTests(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    ApiHeader * pApi;

    // RX and TX messages
    SvsrSetDumpMask (0x3);

    if (CMgrGetParam(pParams, 0, 0) != 0)
    {
        SvsrSelectICpuMaster (CMgrGetParam(pParams, 1, 0));
        uart_printf ("Select cpu-%d for I-CPU\r\n", CMgrGetParam(pParams, 1, 0));
    }

    pApi = SvsrAllocICApiEx(SVSR_I_CPU_RX);
    _ASSERT_PTR(pApi);

    if (pApi == NULL)
    {
        uart_printf("SendToPhyICpu:Can't allocate the memory for API command\r\n");
        return 0;
    }

    uart_printf ("API ptr is %x\r\n", pApi);

    pApi = ApiCreate(pApi, API_DEF_FLAGS, 1, 0x8001, API_TYPE_CFG_PARAM, 0);

    SvsrPutICpuApiToRx(pApi);

    MxDelayTicks (150000);

    return 0;
}

typedef struct _PM_INFO_
{
    UINT32 idle;
    UINT32 svsr;
    UINT32 irq;
    UINT32 irq_num;

}PM_INFO;

UINT32  PMStarted;
UINT32  PMEnd;
VUINT32 PMTick;
PM_INFO PMArray[CPU_NUM] [5];
HANDLE  hPMThr;
HANDLE  hPMThrCpu2, hPMThrCpu3;

UINT32 PMTimerThr2 (LPVOID p)
{
    MxDelayTicks (150*1200);

    return 0;
}

UINT32 PMTimerThr3 (LPVOID p)
{
    MxDelayTicks (150*20);

    return 0;
}

UINT32 PMTimerThr (LPVOID p)
{
#ifdef PM_ENABLED
    UINT32 i, cpu;
    for (cpu = 0; cpu < CPU_NUM; cpu++)
    {
        PMArray[cpu][PMTick].idle    = PMGetCyclesForArmModeEx (cpu, PM_ARM_MODE_IDLE);
        PMArray[cpu][PMTick].irq     = PMGetCyclesForArmModeEx  (cpu, PM_ARM_MODE_IRQ);
        PMArray[cpu][PMTick].svsr    = PMGetCyclesForArmModeEx (cpu, PM_ARM_MODE_SVSR);
        PMArray[cpu][PMTick].irq_num = PMGetARMProcIrqNumEx (cpu);
    }

    PMTick ++;

    if (PMTick >= sizeof (PMArray[0]) / sizeof (PMArray[0][0]))
    {
        PMEnableProf (0);
        TimerDrvSetProc(0, 0, NULL, NULL);

        for (i = 0; i < PMTick; i++)
        {
            for (cpu = 0; cpu < CPU_NUM; cpu++)
            {
                uart_printf ("%CPU:%d (%02d): svsr[%d] irq[%d] [num:%d] idle[%d] total[%d]\r\n",

                    cpu,
                    i,

                    PMArray[cpu][i].svsr,
                    PMArray[cpu][i].irq,
                    PMArray[cpu][i].irq_num,
                    PMArray[cpu][i].idle,

                    PMArray[cpu][i].svsr + PMArray[cpu][i].irq + PMArray[cpu][i].idle
                );

                MxDelayTicks(150*200);
            }

            uart_printf ("\r\n");
        }

        return 0;
    }

    MxScheduleThread (hPMThrCpu2);
    MxScheduleThread (hPMThrCpu3);
#endif
    return 0;
}

void PMTimerCB (LPVOID p)
{
#ifdef PM_ENABLED
    if (PMTick < sizeof (PMArray[0]) / sizeof (PMArray[0][0]))
    {
        PMGenSyncSignal(0);

        MxScheduleThread (hPMThr);
    }
    else
    {
        TimerDrvSetProc(0, 0, NULL, NULL);
    }
#endif
}


UINT32 CLineCheckPM(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
#ifdef PM_ENABLED

    MXRC rc;

    uart_printf ("This test is designed to run performance monitor for 10 ms and show results\r\n");

    PMStarted = 0;
    PMTick    = 0;

    if (HANDLE_NULL(hPMThr))
    {
        rc = MxCreateThread(PMTimerThr, NULL, 0, 0, &hPMThr);

        if (FAILED(rc))
        {
            uart_printf ("PM create thread error!\r\n");
            return 0;
        }
    }

    if (HANDLE_NULL(hPMThrCpu2))
    {
        rc = MxCreateThreadEx(2, PMTimerThr2, NULL, 0, 0, &hPMThrCpu2);

        if (FAILED(rc))
        {
            uart_printf ("PM create thread cpu-2 error!\r\n");
            return 0;
        }
    }

    if (HANDLE_NULL(hPMThrCpu3))
    {
        rc = MxCreateThreadEx(3, PMTimerThr3, NULL, 0, 0, &hPMThrCpu3);

        if (FAILED(rc))
        {
            uart_printf ("PM create thread cpu-3 error!\r\n");
            return 0;
        }
    }

    PMEnableProf(0xf);
    TimerDrvSetProc(10, 1000, PMTimerCB, NULL);
#endif
    return 0;
}

UINT32 CLineCmd(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
     ApiHeader *pApi = NULL;
     UINT32 nCtrl = 0;
     UINT16 type = 0;
     UINT16 srcInst = 0;
     UINT16 dstInst = 0;
     UINT16 ID = 0;
     UINT16 nParamID = 0;
     UINT16 nDataSize = 0;
     UINT16 i,j;
     UINT32 pParamData[128];

    if(pParams->ParamCount < 5)
    {
        uart_printf("\r\nNot enough parameters (%d < 5)\n", pParams->ParamCount );
        return 0;

    }

    nCtrl = CMgrGetParam (pParams, 0, 0);
    type = CMgrGetParam (pParams, 1, 0);
    srcInst = CMgrGetParam (pParams, 2, 0);
    dstInst = CMgrGetParam (pParams, 3, 0);
    ID = CMgrGetParam (pParams, 4, 0);

    if((nCtrl == 0) ||(type == 0) || (srcInst == 0) || (dstInst == 0) || (ID == 0))
    {
        uart_printf("\r\nincorrect API header: the field '%s' can't be zero\r\n",
            (nCtrl == 0)? "<control>" : ((type == 0)? "<type>" : ((srcInst == 0)? "<srcInstID>" : ((dstInst == 0)? "<dstInstID>" : ((ID == 0)? "<msgID>" : "")))));
        return 0;
    }

    if(dstInst & 0x8000)
        pApi = SvsrAllocICApiEx(SVSR_I_CPU_RX);
    else
        pApi = SvsrAllocICApi();

    ApiCreate( pApi,  nCtrl,  srcInst,  dstInst,  type,  ID);

    i = 5;
    while(i < pParams->ParamCount )
    {
        if((i+2) >= pParams->ParamCount)
        {
                uart_printf("Illegal command or some parameters are cut by console\n");
                SvsrFreeApi(pApi);
                return 0;
        }

        nParamID = CMgrGetParam (pParams, i, 0);
        nDataSize = CMgrGetParam (pParams, i+1, 0);

        for(j = 0; (j*4) < nDataSize;  j++)
        {
             if( (i+2+j) >= pParams->ParamCount)
             {
                uart_printf("Illegal command or some parameters are cut by console\n");
                SvsrFreeApi(pApi);
                return 0;
             }
            pParamData[j] = CMgrGetParam (pParams, i+2+j, 0);
        }

        i = i+2+j;

        ApiAddParam(pApi, NULL,  nParamID, pParamData, nDataSize);

    }

    if(dstInst & 0x8000)
        SvsrPutICpuApiToRx(pApi);
    else
        SvsrSendICApi(pApi);

    return 0;

}

#ifdef  UNIDIAG_ENABLED

#define INVALID_HEX 0xFF

UINT8 _char2hex(char ch)
{

    UINT8 nValue;

    if((ch>='0') && (ch<='9'))
    {
        nValue = ch-'0';
    }
    else if((ch>='A') && (ch<='F'))
    {
        nValue = ch-'A'+10;
    }
    else if((ch>='a') && (ch<='f'))
    {
        nValue = ch-'a'+10;
    }
    else
    {
        return INVALID_HEX;
    }

    return nValue;
}

BOOL _str2mac(char *ch, UINT8 *mac)
{
    int i = 0;
    UINT8 res1,res2;

    while(i < 12)
    {
        res1 = _char2hex(ch[i]);
        res2 = _char2hex(ch[i+1]) ;
        if((res1 == INVALID_HEX) || (res2 == INVALID_HEX))
        {
            return FALSE;
        }
        else
        {
            mac[i>>1] = (res1<<4)|res2;
        }

        i+=2;
    }

    if(ch[i])
        return FALSE;

    return TRUE;
}

extern BOOL Stream_LoadIPv4 (const char * pStrIP, UINT8 * pOutBuf);


UINT32 CLineUDRedirectUDP(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT8 *pMAC = NULL;
    UINT8 mac[6];
    UINT8 ip[4];
    UINT32 port;


    if( (pParams->ParamCount !=2) && (pParams->ParamCount !=3) )
    {
        uart_printf("Invalid parameter number\n");
        return 0;

    }

    if(Stream_LoadIPv4(pParams->ParamList[0], ip) == FALSE)
    {
        uart_printf("Invalid IP parameter\n");
        return 0;
    }

    port = CMgrGetParam(pParams, 1, 0 );
    if( (port == 0) || (port > 0xFFFF) )
    {
        uart_printf("Invalid port parameter\n");
        return 0;
    }

    if(pParams->ParamCount ==3)
    {
        if(!_str2mac(pParams->ParamList[2],mac))
        {
            uart_printf("Invalid mac parameter\n");
            return 0;
        }

        pMAC = &mac[0];
    }


    if(MX_OK != UniDiagSetStreamOverUDP( *(UINT32*)ip, port, pMAC))
    {
        uart_printf("network init error\n");
        return 0;
    }

    return 0;
}

UINT32 CLineUDEnable(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{

    if( (pParams->ParamCount !=1) )
    {
        uart_printf("Invalid parameter number\n");
        return 0;

    }

    UniDiagSetStreamEnableMask(CMgrGetParam(pParams, 0, 0 ));
    return 0;
}

UINT32 CLineUDAPIDiagConfig(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{

    if( (pParams->ParamCount !=1) )
    {
        uart_printf("Invalid parameter number\n");
        return 0;

    }

    UniDiagSetAPIMessageOption(CMgrGetParam(pParams, 0, 0 ));

    return 0;
}


#endif

UINT32 CLineCpuInfo(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    PUINT32 p;
    UINT32 idx = CMgrGetParam (pParams, 0, 0);
    UINT32 i;
    ARMCPUCONTEXT ctx;
    MXRC rc;
    char * pName [] = {" SP", " LR", " PC", "CPSR", "SPSR"};

    rc = MxLoadCpuContext (idx, &ctx);

    if (FAILED(rc))
    {
        printf ("Error, rc=%x\r\n", rc);
        return 0;
    }

    printf ("\r\n========= Cpu-info, core=%d==========\r\n", idx);

    p = (PUINT32)&ctx;

    for (i = 0; i < 18; i++)
    {
        if (i < 13)
        {
            printf ("[r%02d]: 0x%08x ", i, p[i]);
        }
        else
        {
            printf ("[%s]: 0x%08x ", pName[i - 13], p[i]);
        }

        //MxDelayTicks(15000);

        if (i&& ((i+1) % 4)==0)
            printf ("\r\n");
    }

    printf ("\r\n========= Cpu-stack==========\r\n");

    p = (PUINT32)ctx.r[13];

    for (i = 0; i < 32; i++)
    {
        if ((i % 4) == 0)
            printf ("0x%08x:  ", p + i);

        printf ("%08x ", p[i]);

        //MxDelayTicks(15000);

        if (i&& ((i+1) % 4)==0)
            printf ("\r\n");
    }

    return 0;
}

UINT32 CLineGenApiTest(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 tti, num, lst, type;

    if (pParams->ParamCount == 0)
    {
        printf ("\r\nParameters error\r\n");
        return RC_CM_PARAM_ERROR;
    }

    // to turn off the mechanism
    SvsrEnableApiDiag (0, 0, 0, 0, 0);

    if (_strcmp (pParams->ParamList[0], "off") == 0)
    {
        printf ("The test is stopped\r\n");
        return MX_OK;
    }

    tti = CMgrGetParam(pParams, 0, 1);
    num = CMgrGetParam(pParams, 1,~0);
    lst = CMgrGetParam(pParams, 2, 1);
    type = CMgrGetParam(pParams, 3, 0);

    printf ("Start test for TTI-%d(ms), Num-%d, ListSize-%d\r\n",
            tti, num, lst);

    // to turn off the mechanism
    SvsrEnableApiDiag (tti, num, lst, 3, type);

    return 0;
}

UINT32 CLineIntInfo(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 en = 0, i = 0;
    ICDRV_INT_INFO info;
    char * pProcName;
    PRootInfo pInfo;

    uart_printf ("\r\n [INT] [PRI] ( NUMBER ) [    TYPE   ] [  HANDLER / PARAMETER ]   [CPUx] [  PROC  ]\r\n");
    uart_printf ("--------------------------------------------------------------------------------------\r\n");

    while (SUCCESSFUL(ICDrvEnumInt (&en, &info)))
    {
        uart_printf (" [%3d] % 3d   (%9d) [%s / %s] [0x%08x{0x%08x}] ->",
                    info.nIntID & 0xFFFF,
                    info.nIntPr,
                    info.Statistics,
                    info.IntType == INT_TYPE_IRQ ? "IRQ" : "FIQ",
                    info.IntMode == INT_TYPE_EDGE? "EDGE ":"LEVEL",
                    info.Handler,
                    info.HandlerPtr);

        i = 0;
        while (i < 4)
        {
            if (info.CpuMask & (1 << i))
                uart_printf ("[CPU%d]", i);

            i++;
        }

        pInfo = DbgGetObjInfo(info.Handler);

        if (pInfo != NULL)
        {
            pProcName = pInfo->ObjName;
        }
        else
        {
            pProcName = NULL;
        }

        uart_printf ("   %s\r\n", pProcName);

        MxDelayTicks (3*150*1000);
    }

    uart_printf ("--------------------------------------------------------------------------------------\r\n");

    return 0;
}

UINT32 CLineCmdThreads(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    PMXCONTEXT pMxCtx;
    UINT32 cpu_id = CMgrGetParam(b, 0, 0);
    UINT32 num, i;
    PRootInfo pInfo;
    PTHREAD pThrInfo;
    PMTHREAD pMThrInfo;

    if (cpu_id >= CPU_NUM)
    {
        uart_printf ("An error in cpu index, it should be [%d..%d]\r\n", 0, CPU_NUM-1);
        return 0;
    }

    pMxCtx = MxInqRemContext(cpu_id);

    if (pMxCtx == NULL)
        return 0;

    uart_printf ("\r\n [ARM ADDR] [PRIOR] [ TYPE ] [STATUS] [ PARAMETER ] [  PROC NAME   ]\r\n");
    uart_printf ("---------------------------------------------------------------------\r\n");

    num = MxGetObjCountEx(cpu_id, OBJ_TYPE_THREAD);
    i = 0;

    while (i < num)
    {
        pThrInfo = &pMxCtx->threads[i++];

        if (!IS_THR_ALLOC(pThrInfo))
            continue;

        pInfo = DbgGetObjInfo((UINT32)pThrInfo->Proc);

        uart_printf (" 0x%08x  % 5d    S-THR   %s    0x%08x    %s\r\n",
            pThrInfo->Proc,
            pThrInfo->Priority,
            IS_SCHEDULED(pThrInfo) ? "SCHED" : "-----",
            pThrInfo->ProcParam,
            (pInfo!=NULL) ? pInfo->ObjName : ""
            );
    }

    num = MxGetObjCountEx(cpu_id, OBJ_TYPE_MTHREAD);

    if (num && i)
        uart_printf ("\r\n");

    i = 0;

    while (i < num)
    {
        pMThrInfo = &pMxCtx->mthreads[i++];

        if (!IS_MTHR_ALLOC(pMThrInfo))
            continue;

        pInfo = DbgGetObjInfo((UINT32)pMThrInfo->Proc);

        uart_printf (" 0x%08x  % 5d    M-THR   %s    0x%08x    %s\r\n",
            pMThrInfo->Proc,
            pMThrInfo->ThreadPrior,
            IS_M_SCHEDULED(pMThrInfo) ? "SCHED" : "-----",
            pMThrInfo->ProcPtr,
            (pInfo!=NULL) ? pInfo->ObjName : ""
            );
    }

    uart_printf ("---------------------------------------------------------------------\r\n");

    return 0;
}

UINT32 CLineCmdTimers(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *b)
{
    PMXCONTEXT pMxCtx;
    UINT32 cpu_id = CMgrGetParam(b, 0, 0);
    UINT32 num, i;
    PRootInfo pInfo;
    PTIMER pTmrInfo;
    PEVENT pEvtInfo;
    HANDLE h, hEvt;

    if (cpu_id >= CPU_NUM)
    {
        uart_printf ("An error in cpu index, it should be [%d..%d]\r\n", 0, CPU_NUM-1);
        return 0;
    }

    pMxCtx = MxInqRemContext(cpu_id);

    if (pMxCtx == NULL)
        return 0;

    uart_printf ("\r\n [ TIMER HANDLE ] [ PRIOR(us) ] [   STATUS  ] [  EVENT  ] [  PROC NAME   ]\r\n");
    uart_printf ("-------------------------------------------------------------------------\r\n");

    num = MxGetObjCountEx(cpu_id, OBJ_TYPE_TIMER);
    i = 0;

    while (i < num)
    {
        pTmrInfo = &pMxCtx->timers[i];
        h = MxGetObjByIndexEx(cpu_id, OBJ_TYPE_TIMER, i);

        i++;

        if (!IS_TMR_ALLOC(pTmrInfo))
            continue;

        if (IS_FRAC_TMR(pTmrInfo) || HANDLE_NULL(pTmrInfo->hEvent))
        {
            pInfo = DbgGetObjInfo((UINT32)pTmrInfo->Proc);

            uart_printf ("    0x%08x      %5d(us)    %s/%s             %s\r\n",
                HANDLE_TOINT(h),
                (1000/MX_TIMER_FRACTION) * pTmrInfo->nPeriod,
                IS_TMR_ENABLED(pTmrInfo) ? " ON" : "OFF",
                IS_TMR_PERIODIC(pTmrInfo) ? "PERIOD  " : "ONE-SHOT",
                pInfo != NULL ? pInfo->ObjName : ""

                );
        }
        else
        {
            PMXCONTEXT pMxEvtCtx;
            PMXCONTEXT pMxThrCtx;
            PTHREAD pThrInfo;

            hEvt = pTmrInfo->hEvent;

            pMxEvtCtx = MxInqRemContext(HANDLE_GET_CPU(hEvt));
            _ASSERT_PTR(pMxEvtCtx);

            pEvtInfo = &pMxEvtCtx->events[HANDLE_GET_INDEX(hEvt)];
            _ASSERT_PTR(pEvtInfo);

            pMxThrCtx = MxInqRemContext(HANDLE_GET_CPU(pEvtInfo->thread));
            _ASSERT_PTR(pMxThrCtx);

            pThrInfo = &pMxThrCtx->threads[HANDLE_GET_INDEX(pEvtInfo->thread)];
            _ASSERT_PTR(pThrInfo);

            pInfo = DbgGetObjInfo((UINT32)pThrInfo->Proc);

            uart_printf ("    0x%08x      %5d(us)    %s/%s  0x%x   %s\r\n",
                HANDLE_TOINT(h),
                (1000) * pTmrInfo->nPeriod,
                IS_TMR_ENABLED(pTmrInfo) ? " ON" : "OFF",
                IS_TMR_PERIODIC(pTmrInfo) ? "PERIOD  " : "ONE-SHOT",
                HANDLE_TOINT(hEvt),
                pInfo != NULL ? pInfo->ObjName : ""
                );
        }
    }

    uart_printf ("-------------------------------------------------------------------------\r\n");

    return 0;
}

UINT32 CLineDILog(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 i;
    UINT32 Enabled = 0, nVal = 0;

    if (pParams->ParamCount == 0)
    {
        if (DiGetIsForcedLogMask ())
            uart_printf ("\r\nDI forced log mask is: 0x%x\r\n", DiGetForcedLogMask ());
        else
            uart_printf ("\r\nDI forced log mask is not enabled\r\n");
        return 0;
    }

    for (i = 0; i < pParams->ParamCount; i++)
    {
        if (_strcmp (pParams->ParamList[i], "rx") == 0)
        {
            Enabled = 1;
            nVal |= DI_LOG_RX0_LOG_ENABLED | DI_LOG_RX1_LOG_ENABLED;
        }
        else if (_strcmp (pParams->ParamList[i], "tx") == 0)
        {
            Enabled = 1;
            nVal |= DI_LOG_TX0_LOG_ENABLED | DI_LOG_TX1_LOG_ENABLED;
        }
        else if (_strcmp (pParams->ParamList[i], "rx0") == 0)
        {
            Enabled = 1;
            nVal |= DI_LOG_RX0_LOG_ENABLED;
        }
        else if (_strcmp (pParams->ParamList[i], "rx1") == 0)
        {
            Enabled = 1;
            nVal |= DI_LOG_RX1_LOG_ENABLED;
        }
        else if (_strcmp (pParams->ParamList[i], "tx0") == 0)
        {
            Enabled = 1;
            nVal |= DI_LOG_TX0_LOG_ENABLED;
        }
        else if (_strcmp (pParams->ParamList[i], "tx1") == 0)
        {
            Enabled = 1;
            nVal |= DI_LOG_TX1_LOG_ENABLED;
        }
        else if (_strcmp (pParams->ParamList[i], "txrr") == 0)
        {
            Enabled = 1;
            nVal |= DI_LOG_TX_LOG_RR;
        }
        else if (_strcmp (pParams->ParamList[i], "rxrr") == 0)
        {
            Enabled = 1;
            nVal |= DI_LOG_RX_LOG_RR;
        }
        else if (_strcmp (pParams->ParamList[i], "dis") == 0)
        {
            Enabled = 1;
            nVal = 0;
        }
        else if (_strcmp (pParams->ParamList[i], "off") == 0)
        {
            Enabled = 0;
            nVal = 0;
        }
        else if (_strcmp (pParams->ParamList[i], "rr") == 0)
        {
            nVal |= DI_LOG_TX_LOG_RR | DI_LOG_RX_LOG_RR;
        }
        else if (_strcmp (pParams->ParamList[i], "all") == 0)
        {
            Enabled = 1;
            nVal = DI_LOG_TX0_LOG_ENABLED |
                   DI_LOG_TX1_LOG_ENABLED |
                   DI_LOG_RX0_LOG_ENABLED |
                   DI_LOG_RX1_LOG_ENABLED |
                   DI_LOG_TX_LOG_RR       |
                   DI_LOG_RX_LOG_RR;
        }
    }

    uart_printf ("\r\nenabled=%d, mask=0x%x\r\n", Enabled, nVal);

    DiSetForcedLogMask(Enabled, nVal);
    return 0;
}

#ifdef SYS_TRACER_ENABLED

UINT32 CLineSvsrTraceEnable(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 nID = CMgrGetParam(pParams, 0, TID_I_CPU_TX);
    LPVOID p;
    HANDLE h;

    p = SvsrEnableTrace(nID, &h);
    uart_printf ("\r\nID = %d, ptr = %x, h = %x", nID, p, HANDLE_TOINT(h));

    return 0;
}

UINT32 CLineTrace(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 nID = CMgrGetParam(pParams, 0, 1);
    HANDLE h;
    UINT32 nPos = ~0;
    MX_TRACE_PART_INFO* pElm;

    h = SysGetTraceByID(nID);

    if (HANDLE_NULL(h))
    {
        uart_printf ("\r\nCan't find the trace with ID=%d\r\n", nID);
        return 0;
    }

    uart_printf ("\r\n----------- TRACE [%d] ----------------\r\n", nID);

    while((pElm=(MX_TRACE_PART_INFO*)SysGetNextTraceElm(h, &nPos)) != NULL)
    {
        uart_printf ("[%04d] [%s] ", pElm->Index, pElm->OperationID == MX_TRACE_OP_ID_PART_ALLOC ? "ALLOC":" FREE");

        #ifdef MX_TRACE_PART_PTR
        uart_printf ("PTR=0x%08x ", pElm->Ptr);
        #endif

        #ifdef MX_TRACE_PART_CPU_ID
        uart_printf ("CPU=%d ", pElm->CpuID);
        #endif

        #ifdef MX_TRACE_PART_LR
        uart_printf ("LR=%x ", pElm->LR);
        #endif

        #ifdef MX_TRACE_PART_TIME
        uart_printf ("TIME=%x ", pElm->Time);
        #endif

        #ifdef MX_TRACE_PART_FREE_BLOCKS
        uart_printf ("FREE-NUM=%d ", pElm->FreeNum);
        #endif

        uart_printf ("\r\n");
        MxDelayTicks (1500000);
    }

    return 0;
}
#endif

UINT32 CLineCmdSWDiagStart(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
	MXRC rc = MX_OK;
	ApiHeader* ApiStart = NULL;
	ApiParam* pPlace;
	UINT8 buf[128];
	UINT32 enable_flag = 1 ,modId = 1, fillSize = 100, i = 0, bufsize = 128;

	ApiStart = SvsrAllocICApiEx(SVSR_I_CPU_RX);
	ApiStart = ApiCreate (ApiStart, API_DEF_FLAGS, 1, IID_SWDIAG, 1, DIAG_API_TEST_INIT);
        pPlace = ApiAddParam (ApiStart, 0, 2, &modId, sizeof(int));
	pPlace = ApiAddParam (ApiStart, pPlace, 3, &bufsize, sizeof(int));
	pPlace = ApiAddParam (ApiStart, pPlace, 4, &enable_flag, sizeof(int));

        while (i < fillSize)
        {
             buf[i] = i;

             i++;
        }

	pPlace = ApiAddParam (ApiStart, pPlace, 5, buf, fillSize);

	rc = SvsrPutICpuApiToRx(ApiStart); // sends a message to L-ARM
	if (rc != MX_OK)
	{
              uart_printf ("DWDiag test: cannot send the message\n");
	}
	uart_printf ("\nAPI command sent\n");

	return 0;
}

UINT32 CLineCmdSWDiagPrint(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
	MXRC rc = MX_OK;
	ApiHeader* ApiPrint = NULL;
    UINT32 modId = 1;


	ApiPrint = SvsrAllocICApiEx(SVSR_I_CPU_RX);
	ApiPrint = ApiCreate (ApiPrint, API_DEF_FLAGS, 1, IID_SWDIAG, 1, DIAG_IND);
    ApiAddParam (ApiPrint, 0, 2, &modId, sizeof(int));
	rc = SvsrPutICpuApiToRx(ApiPrint); // sends a message to L-ARM
	if (rc != MX_OK)
	{
		uart_printf ("SWDiag test: cannot send the message\n");
	}
	uart_printf ("\nAPI command sent\n");

	return 0;
}

UINT32 CLineCmdSWDiagClose(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
	MXRC rc = MX_OK;
	ApiHeader* ApiClose = NULL;
    UINT32 modId = 1;

	ApiClose = SvsrAllocICApiEx(SVSR_I_CPU_RX);
	ApiClose = ApiCreate (ApiClose, API_DEF_FLAGS, 1, IID_SWDIAG, 1, DIAG_API_TEST_CLOSE);
	ApiAddParam (ApiClose, 0, 2, &modId, sizeof(int));

	rc = SvsrPutICpuApiToRx(ApiClose); // sends a message to L-ARM
	if (rc != MX_OK)
	{
		uart_printf ("SWDiag test: cannot send the message\n");
	}
	uart_printf ("\nCLOSE API command sent\n");

	return 0;
}

UINT32 CLineCmdPtb(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    SvsrLoggerPrintTraceBuf();
    return 0;
}

UINT32 NCNBVAR ceva_in_buf[128];
UINT32 NCNBVAR ceva_out_buf[128];

void CevaMdmaCbDone (LPVOID ctx)
{
    uart_printf ("CEVA cbDone (ptr=0x%x)\r\n", ctx);
}

UINT32 CevaMdmaTestThread(LPVOID p)
{
    UINT32 nMdmaID = (UINT32)p;
    UINT32 rc;
    PMDMA_FDESC pFDesc;

    uart_printf ("Openning MDMA drv - ");
    rc = MdmaDrvOpen(nMdmaID);
    uart_printf ("rc=%d\r\n", rc);

    if (rc != 0)
    {
        uart_printf ("exit\r\n");
        MxRemoveThread (MxGetCurrentMThread());
        return 0;
    }

    pFDesc = MdmaDrvAllocDesc(nMdmaID, 1);

    if (pFDesc == NULL)
    {
        uart_printf ("Error to alloc FDesc\r\n");
        uart_printf ("exit\r\n");
        MxRemoveThread (MxGetCurrentMThread());
        return 0;
    }

    uart_printf ("FDESC=0x%x\r\n", pFDesc);

    uart_printf ("Set copy operation\r\n");
    MdmaDrvSetCopy(pFDesc);
    uart_printf ("Set data 0x%x %d 0x%x %d\r\n", ceva_in_buf, sizeof (ceva_in_buf), ceva_out_buf, sizeof (ceva_out_buf));
    MdmaDrvSetData(pFDesc, ceva_in_buf, sizeof (ceva_in_buf), ceva_out_buf, sizeof (ceva_out_buf));

    memset (ceva_in_buf, 0x11, sizeof (ceva_in_buf));
    memset (ceva_out_buf,0x00, sizeof (ceva_out_buf));

    uart_printf ("Set last fdesc\r\n");
    MdmaDrvSetLastDesc(pFDesc, CevaMdmaCbDone, (void*)0x123);

    uart_printf ("Running - ");
    rc = MdmaDrvRun(nMdmaID, pFDesc);
    uart_printf ("%d\r\n", rc);

    uart_printf ("Waiting for done - ");
    rc = MdmaDrvWaitCompletion(nMdmaID, pFDesc);
    uart_printf ("OK, rc=%d\r\n", rc);

    rc = memcmp(ceva_in_buf, ceva_out_buf, MIN(sizeof (ceva_in_buf), sizeof (ceva_out_buf)));
    uart_printf ("COMPARE RES:%d (%s)\r\n", rc, rc == 0 ? "OK" : "ERROR");

    uart_printf ("Closing MDMA drv - ");
    MdmaDrvClose(nMdmaID);
    uart_printf ("rc=%d\r\n", rc);

    MxRemoveThread (MxGetCurrentMThread());

    return 0;
}

UINT32 CLineCmdTestCevaMDMA(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    HANDLE hThr;
    MXRC rc;
    UINT32 nDstCpu = 1;

    if (CMgrGetParam(b, 0, 1) == 1)
    {
        uart_printf ("MDMA on CEVA tests\r\n");

        memset (ceva_in_buf, 0x11, sizeof (ceva_in_buf));
        memset (ceva_out_buf,0x00, sizeof (ceva_out_buf));

        rc = RexServDiagCmd(REX_DIAG_CMD_CHECK_SYS_MDMA,
                        ceva_in_buf,
                        sizeof (ceva_in_buf),
                        ceva_out_buf,
                        sizeof (ceva_out_buf));

        if (rc != 0)
        {
            uart_printf ("Error code RC=0x%x\r\n", rc);
            return 0;
        }

        rc = memcmp(ceva_in_buf, ceva_out_buf, MIN(sizeof (ceva_in_buf), sizeof (ceva_out_buf)));
        uart_printf ("COMPARE RES:%d (%s)\r\n", rc, rc == 0 ? "OK" : "ERROR");

        return 0;
    }

    rc = MxCreateThreadEx (nDstCpu, CevaMdmaTestThread, NULL, 0, 0, &hThr);

    if (FAILED(rc))
    {
        uart_printf ("Error creation of CEVA MDMA thread on cpu-%d, rc=0x%x\r\n", nDstCpu, rc);
        return 0;
    }

    rc = MxScheduleThread(hThr);

    if (FAILED(rc))
    {
        uart_printf ("Schedule test thread error, rc=0x%x\r\n", rc);
        return 0;
    }

    return 0;
}


UINT32 CLineCmdStackParce(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
	UINT32 z = 0;
	int *bbb = NULL;
	int test = CMgrGetParam(b, 0, 0);

	switch(test)
	{
		case 0:
			_ASSERT(0);
			break;
		case 1:
			*bbb = z;
			break;
		case 2:
			stop_printf("test\r\n");
			break;

	}

	return 0;
}

char * GetAPName(UINT32 nAP)
{
    switch (nAP)
    {
        case 0:
            return "NO ACCESS ";
        case 1:
            return "READ-WRITE";
        case 5:
            return "READ-ONLY ";
        default:
            return "----------";
    }
}

char * GetTexName(UINT32 tex)
{
    switch (tex)
    {
        case 0:
            return "S-ORD";
        case 1:
            return "S-DEV";
        case 2:
            return "W-THR";
        case 3:
            return "WBACK";
        case 4:
            return "NONE ";
        case 5:
            return "RSRV ";
        case 6:
            return "IMPL ";
        case 7:
            return "WB/WA";
        default:
            return "-----";
    }
}

UINT32 CLineCmdMmuInfo(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
    MX_MMU_MEM_SCAN_HANDLE h;
    MX_MMU_MEM_INFO info;
    MASTERTABLE hMt;

    UINT32 i = 0, cpu;

    memset (&h, 0, sizeof (h));

    cpu = CMgrGetParam(b, 0, 0);

    if (cpu >= CPU_NUM)
    {
        uart_printf ("Incorrect CPU id\r\n");
        return 0;
    }

    hMt = (MASTERTABLE)mmutable[cpu];

    uart_printf ("\r\n\r\n[MMU table for CPU-%d]\r\n", cpu);

    uart_printf ("------------------------------------------------------------------------------------\r\n");
    uart_printf (" [       ADDRESS        ]  [  ACCESS  ]  [  CACHE  ]  [  SHARED ]  [ DOM ]  [ TYPE ]\r\n");
    uart_printf ("------------------------------------------------------------------------------------\r\n");

    while (1)
    {
        info = MxMmuScanMem(hMt, &h);

        if (info.Size == 0)
            break;

        if (info.AP == AP_NO_ACCESS)
            continue;

        uart_printf (" |0x%08x..0x%08x|   % 8s   % 8s      % 8s      %03d      %s\r\n",
            info.Addr, info.Addr+info.Size-1,
            GetAPName (info.AP),
            GetTexName(info.Tex),
            info.Shared ? "   YES  " : "   NO   ",
            info.Domain,
            info.XN ? "DATA" : "CODE"
        );

        //uart_printf ("BASE:[%08x .. %08x] Domain:[%03x] AP:[%03x] Shared:[%d] TEX:[%03x]\r\n", info.Addr, info.Addr+info.Size-1, info.Domain, info.AP, info.Shared, info.Shared, info.Tex);

        i++;
    }

    uart_printf ("------------------------------------------------------------------------------------\r\n");

    return 0;
}


UINT32 icpuhand(CharICPUCtx * ctx)
{
	UINT8 ch;
	MXRC rc = MX_OK;

	uart_printf("icpuhand : ");


	while(rc == MX_OK)
	{
		rc = ConICPUGetCh(&ch);	
		uart_printf("%c", ch);
	}
	uart_printf("\r\n");
	
	return 0;
}


UINT32 CLineCmdCICPUTest(CMgrCmdCtx * a, CMgrCmdParamCtx * b)
{
	char *cmd;

	if(b->ParamCount == 0)
	{
		uart_printf("Need set command \r\n");
	}
	
	cmd = b->ParamList[0];

	if(strcmp(cmd, "on") == 0)
	{
		ConICPUInit(icpuhand);
	}
	else if(strcmp(cmd, "on") == 0)
	{
		ConICPUExit();
	}
	else if(strcmp(cmd, "send") == 0)
	{
		cicpu_printf(b->ParamList[1]);
	}
	else
	{
		uart_printf("unknown command\r\n");
	}

	return 0;
}


