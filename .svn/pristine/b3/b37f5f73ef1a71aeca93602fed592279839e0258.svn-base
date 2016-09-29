/** @file main.c
 *
 * @brief Application startup
 * @author Mindspeed Technologies
 * @version $Revision: 1.124 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#include "osal.h"
#include "bsp.h"
#include "hostiface.h"
#include "spusched.h"
#include "msgroute.h"
#include "settings.h"
#include "maciface.h"
#include "phyiface.h"
#include "packetif.h"
#include "transport.h"
#include "svsr.h"
#include "auxiface.h"
#include "console.h"
#include "c4ktrace.h"
#include "rrciface.h"
#ifdef UE
#include "rlciface.h"
#endif
#include "version.h"
#include "diags.h"

//
// Framework ID
//
const int VerMajor = U_VERSION_MAJOR;
const int VerMinor = U_VERSION_MINOR;
const int VerPatch = U_VERSION_BUILD;

#ifdef M823XX
char EvmBoard[] = "M823xx";
#elif defined C4KFPGA
char EvmBoard[] = "C4K-FPGA";
#elif defined C4KEVM
char EvmBoard[] = "M84xxx";
#else
char EvmBoard[] = "Sim-PC";
#endif /* M823XX */

extern void NullTask(U32 cpuid);

// TODO: __ARM_argv_veneer crashes when using (int argc, char *argv[])
/** Upper ARM startup routine
 *
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    //
    // Low-level initialization
    //
    if (BspInit(UARM) != SUCCESS)
    {
        // Fatal: dead here
        return ERR_EXIT;
    }

    //
    // Start up Operating System services on U-ARM
    // Must be called before any other tasks
    //
    if (OsalInit(UARM) != SUCCESS)
    {
        // Fatal: dead here
        return ERR_EXIT;
    }
    // Inform tracer we started loading
    C4KTRACE(STG_LOADAPP, MSG_CURRENT, 0);
    // Read configuration right after loading
    // TODO: __ARM_argv_veneer
    InitSettings(argc, argv);
    DiagInit();

    // Device driver initialization
    if (TimerInit(UARM) != SUCCESS)
    {
        // printf may not be accessible until UART init'd
        return ERR_EXIT;
    }

#if defined (USE_BARE) || defined (USE_RTXC)
    //
    // This block is for bare metal access only
    //
    UartInit(UARM);
    PhiInit(UARM);
    GemacInit(UARM);
#endif /* USE_BARE || USE_RTXC */

    SvsrInit();
    MsgDispatcherInit();

    //
    // Top-level tasks
    //

    // AUX Interface is the highest priority (various debug modes)
    AuxInit();
    // ICPU interface is the next priority (both MQ and DEV)
    IcpuInit(UARM);
    // HOST interface after that
    HostInit();
    // Direct connection at last
    ProtoInit(PROTO_SCTP);
    // Console at any time
    ConInit();

    DiagPrint(DIAG_LEVEL2, "T4KFW", "Framework is running on %s, version %i.%i.%i\n",
              EvmBoard, VerMajor, VerMinor, VerPatch);
#ifdef M823XX
    // This is debugging code
    //
    // (SupervisorReady)
    // Len,Idx,Typ,Cls,Func,ID,Payload[6]
    PhiWrite(14, "\x0E\x00\xD4\x03\x00\x00\xFF\xFF\xFF\xFF\x20\x00\x00\x00");
    // HelloWorld3
    // EthDst = FF:FF:FF:FF:FF:FF (broadcast)
    // EthSrc = 00:11:22:33:44:55 (default assignment)
    // Type = 0806 (ARP)
    // HwType = 0001 (Eth)
    // Proto = 0800 (IP)
    // HwSz = 6, ProtoSz = 4, OpCode = 0001 (req)
    // SenderMAC = 00:11:22:33:44:55
    // SenderIP = 192.168.74.2 (C0.A8.4A.02)
    // TargetMAC = 00:00:00:00:00:00
    // TargetIP = 192.168.74.1 (C0.A8.4A.01)
    // Trailer 18 bytes
    GemacWrite(60, "\xFF\xFF\xFF\xFF\xFF\xFF\x00\x11\x22\x33\x44\x55\x08\x06\x00\x01\x08\x00\x06\x04\x00\x01\x00\x11\x22\x33\x44\x55\xC0\xA8\x4A\x02\x00\x00\x00\x00\x00\x00\xC0\xA8\x4A\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00");
#endif /* M82sXX */

    // Inform tracer we have passed init stage
    C4KTRACE(STG_LOADAPP, MSG_PASSED, 100);

    // Send SVSR_READY message
    // for PC Simulation we will get SvsrReady from BOOTUARM with EnodeB/BSMAC
#if defined(_USRDLL) || defined(C4KFPGA) || defined (C4KEVM)
    SvsrReady();
#elif defined (UE) && !defined (_USRDLL)
    // TODO: Move to the appropriate handlers
    /* for MAC-2-MAC mode */
    // TODO: Enable this when script is ready
//  if (IcpuMode != IMODE_HBI)
    {
        PhyInit();
        RrcInit(NULL, NULL);
        MacInit(PhySendMsg, PhyGetMsg, pIMSG_TO_PHY, pIMSG_FROM_PHY);
        RlcInit();
        PhyEnable(NULL);
    }
    SvsrReady();
#endif

    //
    // Default loop, wait here for an exit signal
    //
    NullTask(UARM);

    //
    // Default loop exited, stop all modules
    //
#if defined(PHY_EMU)
    if (MacDone() != SUCCESS)
    {
        // TODO: Error code
        return 1;
    }

    if (PhyDone() != SUCCESS)
    {
        // TODO: Error code
        return 2;
    }
#endif

    ProtoDone();
    HostDone();
    IcpuDone();
    AuxDone();
    MsgDispatcherDone();
    SvsrDone();
    DiagDone();
    TimerDone(UARM);

    printf(C4KFW "Framework stopped\n");

    return SUCCESS;
}

/** Lower ARM startup routine
 *  Not used in default configuration
 */
int mainl(void)
{
    if (BspInit(LARM) != SUCCESS)
    {
        return EXIT_FAILURE;
    }

    // Start up Operating System services on L-ARM
    if (OsalInit(LARM) != SUCCESS)
    {
        return EXIT_FAILURE;
    }

    // Device driver initialization
    if (IcpuInit(LARM) != SUCCESS)
    {
        return EXIT_FAILURE;
    }

    //
    // Default loop
    //
    NullTask(LARM);

    return SUCCESS;
}

#ifdef _USRDLL
/** DLL entry point
 *
 */
BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_DETACH:
        break;
    case DLL_PROCESS_ATTACH:
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}
#endif /* _USRDLL */
