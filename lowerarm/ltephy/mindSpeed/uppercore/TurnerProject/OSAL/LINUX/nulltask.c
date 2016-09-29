/** @file nulltask.c
 *
 * @brief Null task (default loop) for Linux
 * @author Mindspeed Technologies
 * @version $Revision: 1.15 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef USE_LINUX

#include "osal.h"
#include "console.h"
#include "settings.h"

OSAL_SEMAPHORE NullTaskSema;

void SigIntHandler(int signum)
{
    OsalRaiseSema(&NullTaskSema);
}

/** Null Task
 *
 * @param cpuid CPU ID
 */
void NullTask(U32 cpuid)
{
    int ch;
    int console;
    struct sigaction ctrlC;

    OsalCreateSema(&NullTaskSema);
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
        fflush(stderr);

        ctrlC.sa_handler = SigIntHandler;
        sigemptyset(&ctrlC.sa_mask);
        ctrlC.sa_flags = 0;
        sigaction(SIGINT, &ctrlC, NULL);
        sigaction(SIGTERM, &ctrlC, NULL);

        OsalYield();
        OsalWaitForSema(&NullTaskSema, OSAL_WAIT_INFINITE);
    }
    OsalDestroySema(&NullTaskSema);
    printf(C4KFW "Exiting...\n");
}

void NullTaskStop(void)
{
    int console;

    ReadSettingInt("console", &console, 0);

    if (console)
    {
        // Which one is preferred?
        raise(SIGQUIT);
//      raise(SIGINT);
    }
    else
    {
        OsalRaiseSema(&NullTaskSema);
    }
}

#endif /* USE_LINUX */
