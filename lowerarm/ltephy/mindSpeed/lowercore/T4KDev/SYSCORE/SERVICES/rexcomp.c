//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL Rexcomp.c
 *
 * @brief
 * @author Mindspeed Technologies
 * @version $Revision: 1.6 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "rexcomp.h"
#include "DSPdrv.h"

UINT32 InitTSched(SCHEDSTRC * p)
{
    REXCFG cfg;

    memset (&cfg, 0, sizeof (cfg));

    cfg.Res [RESOURCE_CEVA] = DspDrvGetDevMask();   // 10 or 9 CEVAS
    cfg.Res [RESOURCE_MAP]  = 1;                    // 1 MAP

    return RexInit(&cfg);
}


UINT32 ExeTSchedSingle(TCB* thisTCB)
{
    return RexExecTCB (thisTCB);
}

UINT32 ExeTSchedList(TASKLIST* thisList)
{
#if 0
    UINT32 t = MxGetTicks ();
    RexExecList (thisList);

    t = MxGetTicks () - t;
    uart_printf ("New:ExeTSchedList = %d\r\n", t);

    return 0;
#else

    //UINT32 lr = __return_address ();
    //uart_printf (">>> PHY-RexExecList, list:(%x), lr=%x\r\n", thisList, lr);
    return RexExecList (thisList);
#endif
}

