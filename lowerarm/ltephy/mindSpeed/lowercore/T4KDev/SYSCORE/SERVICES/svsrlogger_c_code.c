//-------------------------------------------------------------------------------------------
/** @file svsrlogger.c
 *
 * @brief The definition of supervisor logger API
 * @author Mindspeed Technologies
 * @version $Revision: 1.6 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifdef _WIN32

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "svsrlogger_c_code.h"

//Global variable needed for SVSR logger for C-code
//to simulate pPhyDiCtx->PhyStartCurrCount in Real-Time Project
extern UINT32 PhyStartCurrCount;

APILOGGERCTX  logger_ctx;

APILOGGERCTX * SvsrLoggerGetCtx (void)
{
    return &logger_ctx;
}

MXRC SvsrLoggerInit (UINT32 nMask)
{
    APILOGGERCTX * pCtx = SvsrLoggerGetCtx ();

    _SVSR_ASSERT_PTR(pCtx);

    memset (pCtx, 0, sizeof (*pCtx));

    pCtx->EnabledMask = nMask;
    pCtx->EnabledMaskNmm = nMask;

    pCtx->StorageInfo.Version     = LOGGER_VERSION;

    pCtx->pFile = fopen("svsrapilog.bin", "wb");

    // This is dummy currently. All 0's. While closing, we will re-write this piece
    fwrite(&pCtx->StorageInfo, 1, sizeof(STORAGE_HEADER), pCtx->pFile);
    
    fclose(pCtx->pFile);

    svsrPrintf("SVSRLOGGER: [LoggerInit] Initialized\n");

    return MX_OK;
}

MXRC SvsrLoggerResetStorage(void)
{
    APILOGGERCTX * pCtx = SvsrLoggerGetCtx ();

    _SVSR_ASSERT_PTR (pCtx);

    if (pCtx->pFile != NULL)
        fclose(pCtx->pFile);

    pCtx->pFile = fopen("svsrapilog.bin", "wb");

    svsrPrintf("SVSRLOGGER: [ResetStorage] Reset\n");

    return MX_OK;
}

MXRC SvsrLoggerCloseStorage(void)
{
    APILOGGERCTX * pCtx = SvsrLoggerGetCtx ();

    _SVSR_ASSERT_PTR (pCtx);

    pCtx->StorageInfo.LogStorSize = pCtx->StorageInfo.PayloadSize;

    pCtx->pFile = fopen("svsrapilog.bin", "r+b");
    if (pCtx->pFile == NULL)
    {
        printf("SVSRLOGGER: [CloseStorage] Already Closed!!!\n");
        return MX_OK;
    }

    // Write the correct header
    fseek(pCtx->pFile, 0, SEEK_SET);
    fwrite(&pCtx->StorageInfo, 1, sizeof(STORAGE_HEADER), pCtx->pFile);

    // Close file
    fclose(pCtx->pFile);

    svsrPrintf("SVSRLOGGER: [CloseStorage] Closed\n");
    svsrPrintf("            Number of Messages Logged: %d\n", pCtx->StorageInfo.MsgNum);
    svsrPrintf("            Log Size: %d\n", pCtx->StorageInfo.LogStorSize);

    return MX_OK;
}


void LoggerStorageBuildHeader (UINT32 nMsgID, UINT32 nMacPhySrcDstID, UINT32 nLen, API_LOG_INFO * pHeader)
{
    _SVSR_ASSERT_PTR (pHeader);

    pHeader->PayloadLen = nLen;
    pHeader->Message    = nMsgID;
    pHeader->Source     = LOGGER_GET_SRC_ID(nMacPhySrcDstID);
    pHeader->Destination= LOGGER_GET_DST_ID(nMacPhySrcDstID);
    pHeader->PHY_ID     = LOGGER_GET_PHY_ID(nMacPhySrcDstID);
    pHeader->MAC_ID     = LOGGER_GET_MAC_ID(nMacPhySrcDstID);
    pHeader->Timestamp  = 0;
    pHeader->CurrentTTI = PhyStartCurrCount;
}

MXRC LoggerStorageAddMsg(UINT32 nMsgID, UINT32 nMacPhySrcDstID, LPVOID pMsg, UINT32 nMsgSize)
{
    APILOGGERCTX * pCtx = SvsrLoggerGetCtx ();
    API_LOG_INFO apiInfo;
    UINT32 msg_all_size;
    UINT8 padding[4] = {0,0,0,0};
    PUINT8 Payload;
    int i;

    _SVSR_ASSERT_PTR (pCtx);

    memset(&apiInfo, 0, sizeof(API_LOG_INFO));
    pCtx->pFile = fopen("svsrapilog.bin", "r+b");    
    if (pCtx->pFile == NULL)
    {
        printf("SVSRLOGGER: [AddMessage] ERROR: File Not opened!!!\n");
        return MX_OK;
    }
    fseek(pCtx->pFile,0,SEEK_END);

    msg_all_size = sizeof (API_LOG_INFO) + ROUND(nMsgSize,4);

    if(nMsgID == PHY_INIT_REQ)
    {
        pCtx->StorageInfo.StartOffset = pCtx->StorageInfo.StartPhySize = msg_all_size;
    }

    if(nMsgID == PHY_START_REQ || nMsgID == PHY_INIT_IND)
    {       
        pCtx->StorageInfo.StartOffset += msg_all_size;
        pCtx->StorageInfo.StartPhySize += msg_all_size;
    }

    LoggerStorageBuildHeader(nMsgID, nMacPhySrcDstID, nMsgSize, &apiInfo);

    Payload = (PUINT8)&apiInfo;
    printf("SVSRLOGGER: APILog: ");
    for (i = 0; i < sizeof(API_LOG_INFO); i++)
        printf("%02x ", Payload[i]);
    printf("\n");

    Payload = (PUINT8)pMsg;
    printf("SVSRLOGGER: Frst 8: %02x %02x %02x %02x %02x %02x %02x %02x\n", Payload[0], Payload[1], Payload[2], Payload[3]
                                                                          , Payload[4], Payload[5], Payload[6], Payload[7]);
    printf("SVSRLOGGER: Last 8: %02x %02x %02x %02x %02x %02x %02x %02x\n", Payload[nMsgSize-8], Payload[nMsgSize-7], Payload[nMsgSize-6], Payload[nMsgSize-5]
                                                                          , Payload[nMsgSize-4], Payload[nMsgSize-3], Payload[nMsgSize-2], Payload[nMsgSize-1]);


    fwrite(&apiInfo, 1, sizeof(API_LOG_INFO), pCtx->pFile);
    fwrite(pMsg, nMsgSize, sizeof(UINT8), pCtx->pFile);
    if ((ROUND(nMsgSize,4) - nMsgSize) > 0)
        fwrite(&padding, (ROUND(nMsgSize,4) - nMsgSize), sizeof(UINT8), pCtx->pFile);

    pCtx->StorageInfo.PayloadSize += msg_all_size;
    pCtx->StorageInfo.MsgNum ++;

    // update storage header
    pCtx->StorageInfo.LogStorSize = pCtx->StorageInfo.PayloadSize;
    fseek(pCtx->pFile, 0, SEEK_SET);
    fwrite(&pCtx->StorageInfo, 1, sizeof(STORAGE_HEADER), pCtx->pFile);
    
    fclose(pCtx->pFile);
    svsrPrintf("SVSRLOGGER: [AddMsg] Added %d, %d. PayloadSize: %d\r\n", nMsgID, nMsgSize, pCtx->StorageInfo.PayloadSize);
    if ((ROUND(nMsgSize,4) - nMsgSize) > 0)
        svsrPrintf("            Added Padding Len: %d\n", (ROUND(nMsgSize,4) - nMsgSize));

    return RC_STORAGE_OK;
}


MXRC SvsrLoggerAddInit (UINT32 nMsgID, UINT32 nMacPhySrcDstID, LPVOID pMsg, UINT32 nMsgSize)
{
    return SvsrLoggerAddMsg(nMsgID, nMacPhySrcDstID, pMsg, nMsgSize);
}

MXRC SvsrLoggerAddMsgForce (UINT32 nMsgID, UINT32 nMacPhySrcDstID, LPVOID pMsg, UINT32 nMsgSize)
{
    APILOGGERCTX * pCtx = SvsrLoggerGetCtx();

    _SVSR_ASSERT_PTR (pCtx);

    LoggerStorageAddMsg(nMsgID, nMacPhySrcDstID, pMsg, nMsgSize);

    return MX_OK;
}

MXRC SvsrLoggerAddMsg (UINT32 nMsgID, UINT32 nMacPhySrcDstID, LPVOID pMsg, UINT32 nMsgSize)
{
    APILOGGERCTX * pCtx = SvsrLoggerGetCtx();

    _SVSR_ASSERT_PTR (pCtx);

    // If flag not set, return
    if(nMsgID >= 50)
    {
            if ((pCtx->EnabledMaskNmm & (1 << (nMsgID-50))) == 0)
            return MX_OK;
    }
    else
    {
        if ((pCtx->EnabledMask & (1 << nMsgID)) == 0)
            return MX_OK;
    }

    return SvsrLoggerAddMsgForce (nMsgID, nMacPhySrcDstID, pMsg, nMsgSize);
}

#endif
