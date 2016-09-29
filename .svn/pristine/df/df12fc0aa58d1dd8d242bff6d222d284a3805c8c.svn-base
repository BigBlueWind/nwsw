/** @file svsr.h
 *
 * @brief Supervisor message handling
 * @author Mindspeed Technologies
 * @version $Revision: 1.13 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef SVSR_H_
#define SVSR_H_

#include "resultcodes.h"

//
// Limits
//
#define SVSR_MAXMACINST             6
#define SVSR_MAXPHYINST             6

//
// All command handlers must follow this definition
//
typedef RESULTCODE (*CmdHandler)(PTR);

//
// API handler mapping
//
typedef struct CmdMap
{
    U16 msgID;
    CmdHandler cmdHandler;
} CMD_MAP;

//
// Dynamic Instance IDs
//
typedef struct InstMap
{
    U16 instanceID;
    U16 instanceType;
} INST_MAP;

typedef struct _CevaDump_
{
    U32  CevaID;         // It's used in the response 
    U8*  pDumpBuf;       // The pointer to the dump buffer 
    U32  DumpBufSize;    // The original buffer size 
    U32  DumpSize;       // The size of data placed by CEVA
}CevaDumpCtx;

RESULTCODE SvsrInit(void);
RESULTCODE SvsrDone(void);
EXPORT RESULTCODE SvsrDispatch(U32 size, PTR msg);

// Send indication to HOST
RESULTCODE SvsrReady(void);

//
// Command handlers
//

RESULTCODE SvsrCommand(U16 srcID, U16 msgID, U32 paramCount, ...);

// SYS range
RESULTCODE SvsrLoadLarm(PTR msg);
RESULTCODE SvsrLoadDsp(PTR msg);
RESULTCODE SvsrLoadUarm(PTR msg);
RESULTCODE SvsrReset(PTR msg);
RESULTCODE SvsrUnload(PTR msg);

//RRC range
RESULTCODE SvsrCreateRrc(PTR msg);
RESULTCODE SvsrDestroyRrc(PTR msg);

RESULTCODE SvsrLarmHDiagCevaDump(PTR msg);

#endif /* SVSR_H_ */
