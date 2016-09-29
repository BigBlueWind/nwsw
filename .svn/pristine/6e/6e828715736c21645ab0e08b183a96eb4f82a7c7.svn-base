//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL servmgr.c
 *
 * @brief Services manager, it's designed to easily integrate new services
*       to the system, it's used like an entry point
 * @author Mindspeed Technologies
 * @version $Revision: 1.19 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "servmgr.h"
#include "appprintf.h"
#include "globalstoragearrays.h"
#include "svsrlogger.h"
#include "genlog.h"
#include "iqphone.h"

#ifdef SPACC_DRV_ENABLED
#include "spaccdrv.h"
#endif

#ifdef DSP_PRINTF_ENABLED
#include "dspprintf.h"
#endif // #ifdef DSP_PRINTF_ENABLED

#include "dataint.h"

#ifdef PM_ENABLED
#include "pm.h"
#endif // #ifdef PM_ENABLED

#include "systimers.h"
#include "timerdrv.h"
#include "unified_diag.h"
#include "ptmdiag.h"
#include "supervisor.h"
#include "cline.h"
#include "intercpu.h"
#include "hbidrv.h"
#include "heaps.h"

typedef struct _SERV_ITEM_
{
    UINT32 CpuMask;           // The processor ID, where this service should be initialized
    MXRC (*Entry)(void);    // An entry point of service
    char * Name;
}SEREVITEM;

const SEREVITEM  ServTable [] =
{
        {0x01,              SystemHeapsInit,            "Heaps"},

#if defined(SVSR_ON_ARM_1_ENABLED)
        {0x01,              HBIShareDataObjects ,       "HBI"},
#endif
        {0x01,              ICInit,                     "I-CPU"},

#ifdef CMD_LINE_INIT
        {0x01,              CLInit,                     "Console"},
#endif
        {0x01,              SvsrInit,                   "Supervisor"},
        {MX_TIMER_CORES,    TmrDrvInitLocalTimer,       "Local timer"},
        {0x01,              SysTimerStartWithDiag,      "Global timer"},
        {0x01,              InitializeStorageArrays,    "Global Storage"},

#ifdef SVSR_API_LOGGER_ENABLED
        {0x01,              SvsrLoggerEntry,            "API logger"},
#endif
#ifdef GENLOG_ENABLED
        {0x01,              GenLogEntry,                "General logger"},
#endif
#ifdef SPACC_DRV_ENABLED
        {0x01,              SPAccDrvInit,               "SPAcc"},
#endif
        {0x01,              DiInit,                     "Antenna interface"},
#ifdef PM_ENABLED
        {0x0F,              PMInit,                     "Performance monitor"},
#endif 
#ifdef IQPHONE_ENABLED
        {0x01,              IQPhoneInit,                "IQ player/recorder"},
#endif        
#ifdef UNIDIAG_ENABLED
        {0x01,              UniDiagInit,                "Unidiag"},
#endif
#ifdef PTM_ENABLED
	    {0x01,              PtmDiagInit,                "PTM"},
#endif
        // the table terminator
        {0,     NULL,                           NULL}
};


/** @brief This function initialize list of system services
           specified in the table inside

    @param nCtrl [in] - reserved, it should be zero

    @return [MXRC] an error code */

MXRC ServInit (UINT32 nCtrl)
{
    UINT32  CpuMask = 1 << MxGetCpuID ();
    UINT32 i = 0, nCount = 0, nl, bPrint = 1;
    MXRC rc;

    while (ServTable[i].Entry != NULL)
    {
        if (ServTable[i].CpuMask & CpuMask)
        {
            if (bPrint)
            {
                uart_printf ("------------------------------\r\n");
                uart_printf ("     L-ARM SERVICES         \r\n");
                uart_printf ("------------------------------\r\n");

                bPrint = 0;
            }
        
            uart_printf ("SERVICE [%S]", ServTable[i].Name);
            
            uart_printf_reset_num();
            uart_printf_set_align(2);
            uart_printf_set_header("\r\n------------------------------\r\n");

            rc = ServTable[i].Entry();

            uart_printf_set_align(0);
            uart_printf_set_header(NULL);

            if ( (nl = uart_printf_get_num()) != 0)
            {
                uart_printf ("------------------------------\r\n");
                uart_printf ("[%S] result is ", ServTable[i].Name);
            }

            if (FAILED(rc))
            {
                uart_printf ("{FAILED[rc=0x%08x]}\r\n", (UINT32)rc);
            }
            else
            {
                uart_printf ("{OK}\r\n");
            }

            if (nl)
                uart_printf ("\r\n");

            nCount ++;
        }

        i++;
    }

    return MX_OK;
}

