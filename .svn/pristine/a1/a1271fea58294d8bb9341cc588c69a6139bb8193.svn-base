/** @file t4kcom.h
 *
 * @brief Communication to HBI driver through turnerio.dll
 * @author Mindspeed Technologies
 * @version $Revision: 1.4 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef __T4KCOM_H__
#define __T4KCOM_H__

#include "osal.h"
#include "turnerio.h"

#ifdef __cplusplus
extern "C" {
#endif

void * T4KSendCommand(void * pMsg,UINT Length);

RESULTCODE T4KComOpen(UINT devtype);
RESULTCODE T4KComClose(void);
BOOLEAN T4KSendInputData(void * pDataBuf, UINT DataBufSize, USHORT DestInstID);

RESULTCODE T4KGetOutputData(void * pDataBuf, UINT DataBufSize, USHORT DestInstID);
RESULTCODE T4KSetContextBuffer(void * pCtxBuf,unsigned int CtxBufSize,USHORT DestInstID);
RESULTCODE T4KGetContextBuffer( void * pCtxBuf, unsigned int * pCtxBufSize,USHORT DestInstID);
RESULTCODE T4KSetControlBuffer(void * pCtlBuf,unsigned int CtlBufSize,USHORT DestInstID);
RESULTCODE T4KGetControlBuffer(void * pCtlBuf,unsigned int * pCtlBufSize,USHORT DestInstID);
RESULTCODE T4KSetDebugBuffer(void * pDbgBuf,unsigned int pDbgBufSize,USHORT DestInstID,USHORT ParamID);
RESULTCODE T4KGetDebugBuffer(void * pDbgBuf, unsigned int * pDbgBufSize,USHORT DestInstID,UINT32 BlockSelect);

RESULTCODE T4KLoadDspImage(PCHAR pImageName);
RESULTCODE T4KLoadLowArmImage(PCHAR pImageName);

RESULTCODE T4KSendApiCmd(UINT assocID, PVOID cmd);
RESULTCODE T4KRecvApiCmd(UINT assocID, PVOID cmd, BOOLEAN skip_superv_ready);
RESULTCODE T4KRecvIndCmd(UINT assocID, PVOID cmd, BOOLEAN skip_superv_ready);

UINT GetAckErrorCode(PC4kApiHeader pAckMsg );

RESULTCODE T4KTransact( PVOID out_msg, PVOID in_msg);

#ifdef __cplusplus
}
#endif

#define DIAG_CONTEXT_PARAM_ID       3
#define DIAG_CONTROL_PARAM_ID       4
#define DIAG_LOG_CONTROL            13


#endif /*__T4KCOM_H__ */
