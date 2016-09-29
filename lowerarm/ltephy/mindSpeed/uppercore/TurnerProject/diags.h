/** @file diags.h
 *
 * @brief Diagnostic message handling
 * @author Mindspeed Technologies
 * @version $Revision: 1.8 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef DIAGS_H_
#define DIAGS_H_

#include "basetypes.h"
#include "resultcodes.h"

//
// DiagPrintApi flags
//
#define DIAG_PAYLOAD                    0x00000001
#define DIAG_FULLPAYLOAD                0x00000002
#define DIAG_DECODETLV                  0x00000004
#define DIAG_DECODESAP                  0x00000008

//
// DiagDump flags
//
#define DIAG_DUMPFILE                   0x00000001
#define DIAG_DUMPHEX                    0x00000002

//
// Diagnostics prints bitmask
//
#define DIAG_LEVEL0                     0x00000001      // Init errors
#define DIAG_LEVEL1                     0x00000002      // Init warning
#define DIAG_LEVEL2                     0x00000004      // Init information
#define DIAG_LEVEL3                     0x00000008      // Fatal errors
#define DIAG_LEVEL4                     0x00000010      // Task errors (task exit, recoverable)
#define DIAG_LEVEL5                     0x00000020      // Memory errors (may be recoverable)
#define DIAG_LEVEL6                     0x00000040      // Important information
#define DIAG_LEVEL28                    0x10000000      // Debug prints: status
#define DIAG_LEVEL29                    0x20000000      // Debug prints: PHY API
#define DIAG_LEVEL30                    0x40000000      // Debug prints: message trace
#define DIAG_LEVELALL                   0xFFFFFFFF

//
// Format specifiers for DiagDumpIqSamples
//
#define IQ_FMT_WJ                       0x00000000      // Fixed point, WaveJudge
#define IQ_FMT_VSA                      0x00000001      // Floating point, VSA
#define IQ_FMT_BIN                      0x00000002      // Raw binary buffer
#define IQ_FMT_APPEND                   0x00000004      // Use append mode for storing I/Q samples

RESULTCODE DiagInit(void);
RESULTCODE DiagDone(void);

RESULTCODE DiagCreateTask(PTR msg);
RESULTCODE DiagDestroyTask(PTR msg);
RESULTCODE DiagCreateSema(PTR msg);
RESULTCODE DiagDestroySema(PTR msg);
RESULTCODE DiagCreateMutex(PTR msg);
RESULTCODE DiagDestroyMutex(PTR msg);
RESULTCODE DiagEcho(PTR msg);
RESULTCODE DiagLoopback(U16 destID, int count, int size);

RESULTCODE DiagDump(S32 level, U32 flags, U32 size, PTR data);
RESULTCODE DiagDumpPhyApi(S32 level, PTR vector);
RESULTCODE DiagDumpIqSamples(U32 sf, U32 sfn, U32 format, U32 size, PTR data, ...);

RESULTCODE DiagSetDebugLevel(S32 level);

// TODO: Define NDEBUG for the "release" version"
#ifdef NDEBUG
#define DiagPrint(level,key,fmt, ...)
#define DiagPrintApi(level,key,flags,msg)
#else
int DiagPrint(S32 level, const char *key, const char *fmt, ...);
RESULTCODE DiagPrintApi(S32 level, const char *key, int flags, PTR msg);
#endif

#endif /* DIAGS_H_ */
