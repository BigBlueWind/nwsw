//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL version.c
 *
 * @brief It defines version of SYSCORE/PHY libraries
 * @author Mindspeed Technologies
 * @version $Revision: 1.8 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "version.h"
#include "appprintf.h"
#include <string.h>
#include "devinfo.h"

const char sysversion [] = MAKE_VERS(VERSION);

const char * SysGetVersion (void)
{
    return sysversion;
}

void PrintSysVersion (void)
{
    UINT32 nLen = strlen (sysversion);
    UINT32 i;
    UINT8 mxhVer, mxlVer;

    uart_printf ("\r\n");

    for (i = 0; i < nLen+9; i++)
    uart_printf ("=");

    if (DevInfoIsT4Kx6())
    {
        uart_printf ("\r\n%s (T4K x6)\r\n", sysversion);
    }
    else if (DevInfoIsT4Kx7())
    {
        uart_printf ("\r\n%s (T4K x7)\r\n", sysversion);
    }
    else
    {
        uart_printf ("\r\n%s (T4K)\r\n", sysversion);
    }

    MxGetVersion(&mxhVer, &mxlVer);

    uart_printf("$Version: CORE(%d.%02d)\r\n$Version: 4GMX(%d.%02d)\r\n",
             APP_H_VER, APP_L_VER,
             mxhVer, mxlVer);

    uart_printf ("IMG-date: %s\r\n", __DATE__);
    uart_printf ("IMG-time: %s\r\n", __TIME__);

    for (i = 0; i < nLen+9; i++)
    uart_printf ("=");

    uart_printf ("\r\n");
}
