/** @file nulltask.c
 *
 * @brief Null task for Windows
 * @author Mindspeed Technologies
 * @version $Revision: 1.12 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef USE_WINDOWS

#include "osal.h"
#include "console.h"
#include "settings.h"

OSAL_SEMAPHORE NullTaskSema;

//void CtrlCHandler(int signal)
BOOL WINAPI CtrlCHandler(DWORD dwCtrlType)
{
    OsalRaiseSema(&NullTaskSema);
    return TRUE;                    // All signals handled
}

void NullTask(U32 cpuid)
{
    int ch;
    int console;

    // TODO: Move to better place
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    OsalCreateSema(&NullTaskSema);
//  signal(SIGINT, CtrlCHandler);
    SetConsoleCtrlHandler(CtrlCHandler, TRUE);
    ReadSettingInt("console", &console, 0);
    if (console != 0)
    {
        ConPrompt(cpuid);
        fflush(stdout);
        for (;;)
        {
            ch = getchar();
            if (ch == EOF)
            {
                break;
            }
            if (ConDispatch((char)ch) != SUCCESS)
            {
                break;
            }
        }
    }
    else
    {
        fflush(stdout);
        OsalYield();
        OsalWaitForSema(&NullTaskSema, OSAL_WAIT_INFINITE);
    }
    printf(C4KFW "Quit\n");
}

void NullTaskStop(void)
{
    OsalRaiseSema(&NullTaskSema);
}

#endif /* USE_WINDOWS */
