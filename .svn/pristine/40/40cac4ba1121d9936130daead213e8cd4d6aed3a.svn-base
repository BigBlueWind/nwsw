/** @file trace.c
 *
 * @brief Trace
 * @author Mindspeed Technologies
 * @version $Revision: 1.5 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#include "osal.h"
#include "c4ktrace.h"
#include <io.h>
#include <fcntl.h>

HANDLE C4KMailSlot = INVALID_HANDLE_VALUE;
HANDLE C4KPipe = INVALID_HANDLE_VALUE;
FILE *PipeStream = NULL;
int PipeDesc = -1;
int C4KTraceAppID = 0;

void C4KTraceInit(int appid, int usepipe)
{
    C4KTraceAppID = appid;
    C4KMailSlot = CreateFile(MAIL_FDX,
        GENERIC_WRITE,
        FILE_SHARE_WRITE | FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (C4KMailSlot == INVALID_HANDLE_VALUE)
    {
        // Called standalone, work usual way
        DWORD err;
        err = GetLastError();
        return;
    }
    if (usepipe && WaitNamedPipe(PIPE_FDX, NMPWAIT_WAIT_FOREVER) != 0)
    {
        C4KPipe = CreateFile(PIPE_FDX,
            GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            0,
            NULL);
        if (C4KPipe == INVALID_HANDLE_VALUE)
        {
            return;
        }

        // stdout already open at this time and we need to replace it
        PipeDesc = _open_osfhandle((intptr_t)C4KPipe, _O_TEXT);
        PipeStream = _fdopen(PipeDesc, (const char*)_T("w"));
        *stdout = *PipeStream;
    }
    else
    {
        PipeDesc = _open_osfhandle((intptr_t)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
        PipeStream = _fdopen(PipeDesc, (const char*)_T("w"));
        *stdout = *PipeStream;
        *stderr = *PipeStream;
    }

    // Disable buffering
//  setvbuf(stdout, NULL, _IONBF, 0);
//  setvbuf(stderr, NULL, _IONBF, 0);
    // Use small buffering
//  setvbuf(stdout, NULL, _IOLBF, 128);
//  setvbuf(stderr, NULL, _IOLBF, 128);
}

void C4KTrace(int stage, int msgid, int progress)
{
    MAIL_MESSAGE Msg;
    DWORD written;

    if (C4KMailSlot != INVALID_HANDLE_VALUE)
    {
        Msg.Process = C4KTraceAppID;
        Msg.Stage = stage;
        Msg.MsgID = msgid;
        Msg.Progress = progress;

        WriteFile(C4KMailSlot, &Msg, sizeof(Msg), &written, NULL);
    }
    fflush(stderr);
    fflush(stdout);
}

void C4KTraceEx(int stage, int msgid, int progress, int data0, int data1, int data2, int data3)
{
    MAIL_MESSAGE Msg;
    DWORD written;

    if (C4KMailSlot != INVALID_HANDLE_VALUE)
    {
        Msg.Process = C4KTraceAppID;
        Msg.Stage = stage;
        Msg.MsgID = msgid;
        Msg.Progress = progress;

        Msg.Data[0] = data0;
        Msg.Data[1] = data1;
        Msg.Data[2] = data2;
        Msg.Data[3] = data3;

        WriteFile(C4KMailSlot, &Msg, sizeof(Msg), &written, NULL);
    }
    fflush(stderr);
    fflush(stdout);
}

void C4KTraceDone(void)
{
    fflush(stderr);
    fflush(stdout);
    if (C4KMailSlot != INVALID_HANDLE_VALUE)
    {
        CloseHandle(C4KMailSlot);
        C4KMailSlot = INVALID_HANDLE_VALUE;
    }
    if (C4KPipe != INVALID_HANDLE_VALUE)
    {
        CloseHandle(C4KPipe);
        C4KPipe = INVALID_HANDLE_VALUE;
    }
}
