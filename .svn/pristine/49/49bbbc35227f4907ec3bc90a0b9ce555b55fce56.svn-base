
#include "osal.h"
#include "t4kcom.h"

long        AssociationID = 0;
HANDLE      LinkUpEvent = NULL;
UCHAR       RxBuf[C4K_API_MAX_COMMAND_SIZE] = {0};
THANDLE     hDevice = NULL;

//////////////////////////////////////////////////////////////////////////
void AcceptCB(UINT aid)
{
    if (AssociationID) {
        _tprintf(_T("T4KCom: Only one connection supported\n"));
        return;
    }

    AssociationID = aid;
    SetEvent(LinkUpEvent);
}
#define DEV_COUNT 3
//////////////////////////////////////////////////////////////////////////
RESULTCODE T4KComOpen(UINT devtype)
{
    RESULTCODE rc = FAILURE;
    int i;

    switch (devtype)
    {
    case RTSM:
    case WDM_DRIVER:
    {
        DWORD err;
        TurnerDevEnumEntry devs[DEV_COUNT];
        const char *devname = NULL;
        int devnum = EnumerateTurnerDevices(devs, DEV_COUNT);
        if (!devnum) {
            _tprintf(_T("T4KCom: No T4K devices found\n"));
            break;
        }
        for ( i= 0; i < devnum; ++i) {
            if (devs[i].DevType == devtype) {
                devname = devs[i].DevName;
                break; // get first available device
            }
        }
        
        if ((err = TOpen(&hDevice, (TURNER_DEVICE_TYPE)devtype, devname, NULL, 0)) != 0) {
            _tprintf(_T("T4KCom: Failed to open T4K device %hs (error %u)\n"), devname, err);
        } else {
            rc = SUCCESS;
        }
        break;
    }
    case SCTP_DRIVER:
        rc = FAILURE;
        break;
    }

    return rc;
}

//////////////////////////////////////////////////////////////////////////
RESULTCODE T4KComClose()
{
    if (hDevice)
    {
        TClose(hDevice);
        hDevice = NULL;
    }
    return SUCCESS;
}

#if 0
//////////////////////////////////////////////////////////////////////////
UINT GetAckErrorCode(PC4kApiHeader pAckMsg)
{
    C4kApiParam * pParam;

    if (pAckMsg == NULL) {
        return 0xFFFF0001;
    }

    if (pAckMsg->Length < 8) {
        return 0xFFFF0002;
    }

    pParam = (C4kApiParam *)((UCHAR*)pAckMsg + sizeof(C4kApiHeader));

    if (pParam->ParamID != 1) {
        return 0xFFFF0003;
    }

    return pParam->ParamData[0];
}

//////////////////////////////////////////////////////////////////////////
void *
T4KSendCommand(
    void * pMsg,
    UINT Length
    )
{
    UINT Res = T4KSendApiCmd(AssociationID, (PC4kApiHeader)pMsg);

    ((PC4kApiHeader)pMsg)->Length = C4K_API_MAX_COMMAND_SIZE;
    if (Res == SUCCESS) {
        Res = T4KRecvApiCmd(AssociationID, (PC4kApiHeader)pMsg, 0);
    }

    return pMsg;
}

//////////////////////////////////////////////////////////////////////////
BOOLEAN
T4KSendInputData(
    void * pDataBuf,
    UINT DataBufSize,
    USHORT DestInstID
    )
{
    INT Res = SUCCESS;
    UINT BuffSize, i = 0;
    PUCHAR CmdMsgBuf;

    if (!DataBufSize)
        return FALSE;

    BuffSize = max(DataBufSize + C4K_API_EXT_HDR_SIZE, C4K_API_MAX_COMMAND_SIZE);
    CmdMsgBuf = (PUCHAR)malloc(BuffSize);

    ((PC4kApiHeader)CmdMsgBuf)->SrcInstID = C4K_HOST_ID;
    ((PC4kApiHeader)CmdMsgBuf)->DstInstID = DestInstID;
    ((PC4kApiHeader)CmdMsgBuf)->MsgType = C4K_API_TYPE_SET;
    ((PC4kApiHeader)CmdMsgBuf)->MsgID = C4K_API_DIAG_INPUT;

    if (hDevice && BuffSize > C4K_API_COMMAND_SINGULARITY/*C4K_API_MAX_COMMAND_SIZE*/) {
        UINT CmdLen = DataBufSize;
        while (CmdLen) {
            ((PC4kApiHeader)CmdMsgBuf)->Control = C4K_API_FLAGS_VERSION | C4K_API_FLAGS_EXT_HDR | (!i++ ? C4K_API_FLAGS_FIRST_BUF : 0);
            ((PC4kApiHeader)CmdMsgBuf)->Length = 
                (CmdLen + C4K_API_EXT_HDR_SIZE > C4K_API_COMMAND_SINGULARITY)
                ? C4K_API_COMMAND_SINGULARITY - C4K_API_HDR_SIZE
                : CmdLen + C4K_API_EXT_HDR_SIZE - C4K_API_HDR_SIZE;
            ((PC4kExtApiHeader)CmdMsgBuf)->TotalSize = DataBufSize;

            PUCHAR ToSendBuf = (PUCHAR)pDataBuf + ((PC4kExtApiHeader)CmdMsgBuf)->TotalSize - CmdLen;
            UINT16 ToSendLen = ((PC4kApiHeader)CmdMsgBuf)->Length - C4K_API_EXT_HDR_SIZE + C4K_API_HDR_SIZE;

            memcpy((PUCHAR)CmdMsgBuf + C4K_API_EXT_HDR_SIZE, ToSendBuf, ToSendLen);

            if (Res = T4KSendApiCmd(AssociationID, CmdMsgBuf))
                break;

            ((PC4kApiHeader)CmdMsgBuf)->Length = C4K_API_MAX_COMMAND_SIZE;
            if ((Res = T4KRecvApiCmd(AssociationID, (PC4kApiHeader)CmdMsgBuf)) != 0)
                break;

            CmdLen -= ToSendLen;
            if (!CmdLen)
                break;

            ((PC4kApiHeader)CmdMsgBuf)->SrcInstID = C4K_HOST_ID;
            ((PC4kApiHeader)CmdMsgBuf)->DstInstID = DestInstID;
            ((PC4kApiHeader)CmdMsgBuf)->MsgType = C4K_API_TYPE_SET;
            ((PC4kApiHeader)CmdMsgBuf)->MsgID = C4K_API_DIAG_INPUT;
        }
    } else {
        ((PC4kApiHeader)CmdMsgBuf)->Control = C4K_API_FLAGS_VERSION | (hDevice ? 0 : C4K_API_FLAGS_EXT_HDR);
        ((PC4kApiHeader)CmdMsgBuf)->Length = DataBufSize;

        memcpy((PUCHAR)CmdMsgBuf + C4K_API_HDR_SIZE, pDataBuf, DataBufSize);

        Res = T4KSendApiCmd(AssociationID, (PC4kApiHeader)CmdMsgBuf);

        ((PC4kApiHeader)CmdMsgBuf)->Length = BuffSize;
        if (Res == SUCCESS) {
            Res = T4KRecvApiCmd(AssociationID, (PC4kApiHeader)CmdMsgBuf, TRUE);
        }
    }

    free(CmdMsgBuf);

    return (Res == SUCCESS) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////
BOOLEAN
T4KGetOutputData(
    void * pDataBuf,
    UINT DataBufSize,
    USHORT DestInstID
    )
{
    PC4kApiParam param;
    C4kApiParam param_size, param_offset;
    INT Res = SUCCESS;
    UCHAR CmdMsg[C4K_API_MAX_COMMAND_SIZE] = {0};
    UINT32 offset = 0, size;

    if (!DataBufSize)
        return FALSE;

    param_size.ParamLength = sizeof(size);
    param_size.ParamID = C4KAPI_PID_SIZEID;
    param_size.ParamData[0] = (UINT32)&size;

    param_offset.ParamLength =  sizeof(offset);
    param_offset.ParamID = C4KAPI_PID_OFFSID;
    param_offset.ParamData[0] = (UINT32)&offset;

    do {
        ((PC4kApiHeader)CmdMsg)->Control = C4K_API_FLAGS_VERSION;
        ((PC4kApiHeader)CmdMsg)->SrcInstID = C4K_HOST_ID;
        ((PC4kApiHeader)CmdMsg)->DstInstID = DestInstID;
        ((PC4kApiHeader)CmdMsg)->MsgType = C4K_API_TYPE_QUERY;
        ((PC4kApiHeader)CmdMsg)->MsgID = C4K_API_DIAG_OUTPUT;
        ((PC4kApiHeader)CmdMsg)->Length = 0;

        size = min(C4K_API_COMMAND_SINGULARITY - C4K_API_HDR_SIZE - C4K_PARAM_HDR_SIZE, DataBufSize - offset);

        SCTPSetParamApiCmd((PC4kApiHeader)CmdMsg, &param_size);
        SCTPSetParamApiCmd((PC4kApiHeader)CmdMsg, &param_offset);

        if ((Res = T4KSendApiCmd(AssociationID, (PC4kApiHeader)CmdMsg)) != SUCCESS)
            break;

        ((PC4kApiHeader)CmdMsg)->Length = C4K_API_MAX_COMMAND_SIZE;
        if ((Res = T4KRecvApiCmd(AssociationID, (PC4kApiHeader)CmdMsg)) != SUCCESS)
            break;

        param = SCTPGetParamApiCmd((PC4kApiHeader)CmdMsg, C4KAPI_PID_OUTPUT);

        memcpy((UCHAR*)pDataBuf + offset, (PUCHAR)param->ParamData, param->ParamLength);

        offset += param->ParamLength;
    } while (offset < DataBufSize);

    return (Res == SUCCESS) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////
RESULTCODE
T4KSetContextBuffer(
    void * pCtxBuf,
    unsigned int CtxBufSize,
    USHORT DestInstID
    )
{
    INT Res = SUCCESS;
    UCHAR CmdMsg[C4K_API_MAX_COMMAND_SIZE] = {0};
    C4kApiParam param;

    if (!CtxBufSize || (CtxBufSize > C4K_API_COMMAND_SINGULARITY - C4K_API_HDR_SIZE - C4K_PARAM_HDR_SIZE))
        return FAILURE;

    param.ParamLength = CtxBufSize;
    param.ParamID = C4KAPI_PID_CONTEXT;
    param.ParamData[0] = (UINT32)pCtxBuf;

    ((PC4kApiHeader)CmdMsg)->Control = C4K_API_FLAGS_VERSION;
    ((PC4kApiHeader)CmdMsg)->SrcInstID = C4K_HOST_ID;
    ((PC4kApiHeader)CmdMsg)->DstInstID = DestInstID;
    ((PC4kApiHeader)CmdMsg)->MsgType = C4K_API_TYPE_SET;
    ((PC4kApiHeader)CmdMsg)->MsgID = C4K_API_DIAG_CONTEXT;
    ((PC4kApiHeader)CmdMsg)->Length = 0;

    SCTPSetParamApiCmd((PC4kApiHeader)CmdMsg, &param);

    Res = T4KSendApiCmd(AssociationID, (PC4kApiHeader)CmdMsg);

    ((PC4kApiHeader)CmdMsg)->Length = C4K_API_MAX_COMMAND_SIZE;
    if (Res == SUCCESS) {
        Res = T4KRecvApiCmd(AssociationID, (PC4kApiHeader)CmdMsg);
    }

    return Res;
}

//////////////////////////////////////////////////////////////////////////
RESULTCODE
T4KGetContextBuffer(
    void * pCtxBuf,
    unsigned int * pCtxBufSize,
    USHORT DestInstID
    )
{
    C4kApiParam param;
    PC4kApiParam p_param;
    UINT32 zero=0;
    INT Res = SUCCESS;
    UCHAR CmdMsg[C4K_API_MAX_COMMAND_SIZE] = {0};

    if (!*pCtxBufSize || (*pCtxBufSize > C4K_API_COMMAND_SINGULARITY - C4K_API_HDR_SIZE - C4K_PARAM_HDR_SIZE))
        return FAILURE;

    ((PC4kApiHeader)CmdMsg)->Control = C4K_API_FLAGS_VERSION;
    ((PC4kApiHeader)CmdMsg)->SrcInstID = C4K_HOST_ID;
    ((PC4kApiHeader)CmdMsg)->DstInstID = DestInstID;
    ((PC4kApiHeader)CmdMsg)->MsgType = C4K_API_TYPE_QUERY;
    ((PC4kApiHeader)CmdMsg)->MsgID = C4K_API_DIAG_CONTEXT;
    ((PC4kApiHeader)CmdMsg)->Length = 0;

    param.ParamLength = 4;
    param.ParamID = C4KAPI_PID_CONTEXT;
    param.ParamData[0] = (UINT32)&zero;

    SCTPSetParamApiCmd((PC4kApiHeader)CmdMsg, &param);

    Res = T4KSendApiCmd(AssociationID, (PC4kApiHeader)CmdMsg);

    ((PC4kApiHeader)CmdMsg)->Length = C4K_API_MAX_COMMAND_SIZE;
    if (Res == SUCCESS ) {
        Res = T4KRecvApiCmd(AssociationID, (PC4kApiHeader)CmdMsg);
    }

    if (Res != SUCCESS)
        return FAILURE;

    p_param = SCTPGetParamApiCmd((PC4kApiHeader)CmdMsg, C4KAPI_PID_CONTEXT);

    assert(p_param);

    assert(*pCtxBufSize >= p_param->ParamLength);

    memcpy((unsigned char *)pCtxBuf, p_param->ParamData, p_param->ParamLength);

    *pCtxBufSize = p_param->ParamLength;

    return SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
RESULTCODE
T4KGetControlBuffer(
    void * pCtlBuf,
    unsigned int * pCtlBufSize,
    USHORT DestInstID
    )
{
    PC4kApiParam param;
    INT Res = SUCCESS;
    UCHAR CmdMsg[C4K_API_MAX_COMMAND_SIZE] = {0};

    if (!*pCtlBufSize || (*pCtlBufSize > C4K_API_COMMAND_SINGULARITY - C4K_API_HDR_SIZE - C4K_PARAM_HDR_SIZE))
        return FAILURE;

    ((PC4kApiHeader)CmdMsg)->Control = C4K_API_FLAGS_VERSION;
    ((PC4kApiHeader)CmdMsg)->SrcInstID = C4K_HOST_ID;
    ((PC4kApiHeader)CmdMsg)->DstInstID = DestInstID;
    ((PC4kApiHeader)CmdMsg)->MsgType = C4K_API_TYPE_QUERY;
    ((PC4kApiHeader)CmdMsg)->MsgID = C4K_API_DIAG_CONTROL;
    ((PC4kApiHeader)CmdMsg)->Length = 0;

    Res = T4KSendApiCmd(AssociationID, (PC4kApiHeader)CmdMsg);

    ((PC4kApiHeader)CmdMsg)->Length = C4K_API_MAX_COMMAND_SIZE;
    if (Res == SUCCESS) {
        Res = T4KRecvApiCmd(AssociationID, (PC4kApiHeader)CmdMsg);
    }

    if (Res != SUCCESS)
        return FAILURE;

    param = SCTPGetParamApiCmd((PC4kApiHeader)CmdMsg, C4KAPI_PID_CONTROL);

    assert(*pCtlBufSize >= param->ParamLength);

    memcpy((unsigned char *)pCtlBuf, param->ParamData, param->ParamLength);

    *pCtlBufSize = param->ParamLength;

    return SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
RESULTCODE
T4KSetDebugBuffer(
    void * pDbgBuf,
    unsigned int pDbgBufSize,
    USHORT DestInstID,
    USHORT ParamID
    )
{
    INT Res = SUCCESS;
    UCHAR CmdMsg[C4K_API_MAX_COMMAND_SIZE] = {0};
    C4kApiParam param;

    if (!pDbgBufSize || (pDbgBufSize > C4K_API_COMMAND_SINGULARITY - C4K_API_HDR_SIZE - C4K_PARAM_HDR_SIZE))
        return FAILURE;

    param.ParamLength = pDbgBufSize;
    param.ParamID = ParamID;
    param.ParamData[0] = (UINT32)pDbgBuf;

    ((PC4kApiHeader)CmdMsg)->Control = C4K_API_FLAGS_VERSION;
    ((PC4kApiHeader)CmdMsg)->SrcInstID = C4K_HOST_ID;
    ((PC4kApiHeader)CmdMsg)->DstInstID = DestInstID;
    ((PC4kApiHeader)CmdMsg)->MsgType = C4K_API_TYPE_SET;
    ((PC4kApiHeader)CmdMsg)->MsgID = 0xF906;
    ((PC4kApiHeader)CmdMsg)->Length = 0;

    SCTPSetParamApiCmd((PC4kApiHeader)CmdMsg, &param);

    Res = T4KSendApiCmd(AssociationID, (PC4kApiHeader)CmdMsg);

    ((PC4kApiHeader)CmdMsg)->Length = C4K_API_MAX_COMMAND_SIZE;
    if (Res == SUCCESS) {
        Res = T4KRecvApiCmd(AssociationID, (PC4kApiHeader)CmdMsg);
    }

    return Res;
}

//////////////////////////////////////////////////////////////////////////
RESULTCODE
T4KGetDebugBuffer(
    void * pDbgBuf,
    unsigned int * pDbgBufSize,
    USHORT DestInstID,
    UINT32 BlockSelect
    )
{
    PC4kApiParam p_out_param;
    C4kApiParam param;
    UINT32 blk_sel = BlockSelect;
    INT Res = SUCCESS;
    UCHAR CmdMsg[C4K_API_MAX_COMMAND_SIZE] = {0};

    if (!*pDbgBufSize || (*pDbgBufSize > C4K_API_COMMAND_SINGULARITY - C4K_API_HDR_SIZE - C4K_PARAM_HDR_SIZE))
        return FAILURE;

    param.ParamLength = 4;
    param.ParamID = DIAG_CONTEXT_PARAM_ID;
    param.ParamData[0] = (UINT32)&blk_sel;

    ((PC4kApiHeader)CmdMsg)->Control = C4K_API_FLAGS_VERSION;
    ((PC4kApiHeader)CmdMsg)->SrcInstID = C4K_HOST_ID;
    ((PC4kApiHeader)CmdMsg)->DstInstID = DestInstID;
    ((PC4kApiHeader)CmdMsg)->MsgType = C4K_API_TYPE_QUERY;
    ((PC4kApiHeader)CmdMsg)->MsgID = 0xF906;
    ((PC4kApiHeader)CmdMsg)->Length = 0;

    SCTPSetParamApiCmd((PC4kApiHeader)CmdMsg, &param);

    Res = T4KSendApiCmd(AssociationID, (PC4kApiHeader)CmdMsg);

    ((PC4kApiHeader)CmdMsg)->Length = C4K_API_MAX_COMMAND_SIZE;
    if (Res == SUCCESS ) {
        Res = T4KRecvApiCmd(AssociationID, (PC4kApiHeader)CmdMsg);
    }

    if (Res != SUCCESS)
        return FAILURE;

    p_out_param = SCTPGetParamApiCmd((PC4kApiHeader)CmdMsg, C4KAPI_PID_CONTEXT);

    assert(*pDbgBufSize >= p_out_param->ParamLength);

    memcpy((unsigned char *)pDbgBuf, p_out_param->ParamData, p_out_param->ParamLength);

    *pDbgBufSize = p_out_param->ParamLength;

    return SUCCESS;
}
#endif 

//////////////////////////////////////////////////////////////////////////
RESULTCODE T4KLoadImage(PCHAR pImageName, USHORT MsgId)
{
    //assert(pImageName);

    _tprintf(
        _T("T4KCom: Loading %s image %hs\n"),
        (MsgId == C4K_API_LOAD_L_ARM) ? _T("LowARM") : _T("DSP"),
        pImageName
        );

    if (hDevice) {
        DWORD err, brmerr;
        if (err = TReset(hDevice)) {
            _tprintf(_T("T4KCom: Failed to reset T4K device (error %u)\n"), err);
            return FAILURE;
        }
        if (err = TBoot(hDevice, pImageName, 0, &brmerr)) {
            _tprintf(_T("T4KCom: Failed to boot T4K device (error %u, BRM error 0x%.8X)\n"), err, brmerr);
            return FAILURE;
        }

        UCHAR SvsrReadyBuf[C4K_API_MAX_COMMAND_SIZE];
        C4kApiHeader* pInd = (C4kApiHeader*)SvsrReadyBuf;

        err = TRecvInd(hDevice, pInd, sizeof(SvsrReadyBuf), INFINITE);

        // Check if we get Supervisor Ready Message
        if (err == 0 && pInd->MsgID == C4K_API_SUPERV_READY)
        {
            _tprintf(_T("T4KCom: Received SUPERVISOR_READY\n"));
            return SUCCESS;
        }
        return FAILURE;
    }
    else 
        _tprintf(_T("T4KCom: There is no open device\n"));

    return SUCCESS;
}


//////////////////////////////////////////////////////////////////////////
RESULTCODE T4KLoadDspImage(PCHAR pImageName)
{
    return T4KLoadImage(pImageName, C4K_API_LOAD_DSP);
}

//////////////////////////////////////////////////////////////////////////
RESULTCODE T4KLoadLowArmImage(PCHAR pImageName)
{
    return T4KLoadImage(pImageName, C4K_API_LOAD_L_ARM);
}

//////////////////////////////////////////////////////////////////////////
RESULTCODE T4KSendApiCmd(UINT assocID, PVOID cmd)
{
    if (hDevice) {
        return TSendCmd(hDevice, (C4kApiHeader*)cmd, /*5000*/INFINITE);
    }
    return FAILURE;
}

RESULTCODE T4KRecvIndCmd(UINT assocID, PVOID p_cmd_buf, BOOLEAN skip_superv_ready)
{
    RESULTCODE rc = FAILURE;
    C4kApiHeader *p_cmd = (C4kApiHeader *)p_cmd_buf;

    do {
        if (hDevice) {
            // Returns 0 on success
            rc = TRecvInd(hDevice, (C4kApiHeader*)p_cmd_buf, C4K_API_MAX_COMMAND_SIZE, /*5000*/INFINITE);
        }
    } while (skip_superv_ready && p_cmd->MsgType == 0x0005 && p_cmd->MsgID == 0xf800);// skip supervisor ready message

    return (RESULTCODE)rc;
}



//////////////////////////////////////////////////////////////////////////
RESULTCODE T4KRecvApiCmd(UINT assocID, PVOID p_cmd_buf, BOOLEAN skip_superv_ready)
{
    RESULTCODE rc = FAILURE;
    C4kApiHeader *p_cmd = (C4kApiHeader *)p_cmd_buf;

    do {
        if (hDevice) {
            // Returns 0 on success
            rc = TRecvAck(hDevice, (C4kApiHeader*)p_cmd_buf, C4K_API_MAX_COMMAND_SIZE, /*5000*/INFINITE);
        }
    } while (skip_superv_ready && p_cmd->MsgType == 0x0005 && p_cmd->MsgID == 0xf800);// skip supervisor ready message

    return (RESULTCODE)rc;
}

//////////////////////////////////////////////////////////////////////////
RESULTCODE T4KTransact(PVOID out_msg, PVOID in_msg)
{
    RESULTCODE res = FAILURE;
    C4kApiHeader *p_out = (C4kApiHeader *)out_msg;
    C4kApiHeader *p_in = (C4kApiHeader *)in_msg;

    if (hDevice) 
    {
        if ((res = TSendCmd(hDevice, p_out, 5000)) == 0)
        {
            res = TRecvAck(hDevice, p_in, C4K_API_MAX_COMMAND_SIZE, /*10000*/INFINITE);
        }
    }
    return res;
}
