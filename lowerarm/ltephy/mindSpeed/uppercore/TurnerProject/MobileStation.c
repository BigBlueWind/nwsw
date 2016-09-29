/** @file MobileStation.c
 *
 * @brief MS startup (DLL calls)
 * @author Mindspeed Technologies
 * @version $Revision: 1.11 $
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
#include "MS_externs.h"

#ifdef MSMAC
void WimaxMobileStationInit()
{
        OsalInit(UARM);
        AuxInit();
        MsgDispatcherInit();
        SvsrInit();
        IcpuInit(UARM);
        HostInit();

        SvsrReady();
  
}
#endif
#ifdef UE
void LteMobileStationInit(int argc, char *argv[])
{
    InitSettings(argc, argv);
    OsalInit(UARM);
    AuxInit();
    MsgDispatcherInit();
    SvsrInit();
    IcpuInit(UARM);
    HostInit();
    SvsrReady();
}
#endif
#ifdef MSMAC
void WimaxMobileStationDone()
{
    PhyDone();
    HostDone();
    IcpuDone();
    AuxDone();
}
#endif
#ifdef UE
void LteMobileStationDone()
{
    PhyDone();
    HostDone();
    IcpuDone();
    AuxDone();
}
#endif