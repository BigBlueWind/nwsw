#ifdef ENODEB

#include "RrcMacParse.h"
#include "rrciface.h"
#include "rrc_types.h"
#include "msgroute.h"

U8* SetRadioBearer(U8* InBuf, U32* InLen, U32* OutBuf, U32* OutLen);
RESULTCODE MacUeCreateParse(U32 UeId, U32* buf, U8 dir)
{
    ueContextDlConf* DlCtx = NULL;
    ueContextDlConf* UlCtx = NULL;
  	RESULTCODE ret = SUCCESS;

    if (dir == DOWNLINK)
    {
        DlCtx = malloc(sizeof(ueContextDlConf));
        DlCtx->CodRate = buf[0];
        DlCtx->MaxRb = buf[1];
        DlCtx->Mod = buf[2];
        DlCtx->UePriority = buf[3];
    }
    if (dir == UPLINK)
    {
        UlCtx = malloc(sizeof(ueContextUlConf));
        UlCtx->CodRate = buf[0];
        UlCtx->MaxRb = buf[1];
        UlCtx->Mod = buf[2];
        UlCtx->UePriority = buf[3];
    }
  //  ret = createMacUeEntity(DlCtx, UlCtx);
    if(DlCtx)
        free(DlCtx);
    if(UlCtx)
        free(UlCtx);
    return ret;
}


RESULTCODE MacUeConfigParse(U32 UeId, U32* buf, U8 dir)
{
    ueContextDlConf* DlCtx = NULL;
    ueContextDlConf* UlCtx = NULL;
  	RESULTCODE ret = SUCCESS;

    if (dir == DOWNLINK)
    {
        DlCtx = malloc(sizeof(ueContextDlConf));
        DlCtx->CodRate = buf[0];
        DlCtx->MaxRb = buf[1];
        DlCtx->Mod = buf[2];
        DlCtx->UePriority = buf[3];
    }
    if (dir == UPLINK)
    {
        UlCtx = malloc(sizeof(ueContextUlConf));
        UlCtx->CodRate = buf[0];
        UlCtx->MaxRb = buf[1];
        UlCtx->Mod = buf[2];
        UlCtx->UePriority = buf[3];
    }
 //   ret = reconfigureMacUeEntity(DlCtx, UlCtx);
    if(DlCtx)
        free(DlCtx);
    if(UlCtx)
        free(UlCtx);
    return ret;
}
U8* GetParamHeader(PPARAMHEADER Param, U8* InBuf, U32* Len)
{
    if(*Len >= sizeof(U32))
    {
        *Len -=sizeof(U32);
        Param->paramID = *(U16*)InBuf;
        InBuf+=2;
        Param->length = *(U16*)InBuf;
        InBuf+=2;
        return InBuf;
    }
    else
    {
        Param->paramID = 0;
        Param->length = 0;
        return NULL;
    }
}
U8* GetDataU32(U32* Data, U8* InBuf, U32* Len )
{
    *Data = *(U32*)InBuf;
    if(*Len >= sizeof(U32))
        *Len -=sizeof(U32);
    else
        return NULL;
    return InBuf + sizeof(U32);

}
   
RESULTCODE SystemCtrlParse(U8* InBuf, U32 InLen)
{
    U8 TxBuffer[200] = {0};
    U8 ParamTempo[200] = {0};
    PMSGHEADER pHdr = (PMSGHEADER) TxBuffer;
    U32* Param = &pHdr->param[0];
    PPARAMHEADER ParHeader;
    U32* bParOut= (U32*)ParamTempo;
    U16* swap_buf;
    U32 ret = SUCCESS;
    U32 idx = 0;
   

    pHdr->control = CTRL_VERSION;
    pHdr->length = sizeof(MSGHEADER);
    pHdr->srcID = MSGS_RRC;
    pHdr->dstID = MSGS_MAC;
    pHdr->type = MSGT_CONFIG;
    pHdr->msgID = 0;

    swap_buf = (U16*)InBuf;
    printf("!!! Len %d\n", InLen);
   /* 
    swap_buf = (U16*)InBuf;
    for(idx = 0; idx < InLen; idx++)
    {
        swap_buf[idx] = swap16(swap_buf[idx]);
    }*/
    ParHeader = (PPARAMHEADER)InBuf;
    InBuf += sizeof(U32);
  
    if(ParHeader->paramID != SYSTEM_CTRL_REQ)
    {
        printf("!!!!!!!!!!!! ERROR %x unknown message\n",ParHeader->paramID );
        return -1;
    }
    InLen = ParHeader->length - sizeof(U32) - sizeof(U32);
    ParHeader = (PPARAMHEADER)InBuf;
    InBuf += sizeof(U32);
    if(ParHeader->paramID == SYSTEM_CTRL_REQ_COMMON)
    {
        U32 CellId_Type;
        U32 RoutingInfoType, TimingInfoType;
        InBuf = GetParamHeader(ParHeader, InBuf,&InLen);//CellId_Type 0xf00a
        InBuf = GetDataU32(&CellId_Type, InBuf, &InLen);
        InBuf = GetParamHeader(ParHeader, InBuf, &InLen); //0x4002
        RoutingInfoType = *InBuf;
        InLen--;
        InBuf++;
        InBuf = GetParamHeader(ParHeader, InBuf, &InLen); //0x4002
        TimingInfoType = *InBuf;
        InLen--;
        InBuf++;

        InBuf = GetParamHeader(ParHeader, InBuf, &InLen); //0x4002
        InBuf+= sizeof(U16);
        InLen -= sizeof(U16);
    }
    InBuf = GetParamHeader(ParHeader, InBuf, &InLen);
    if (ParHeader->paramID == SYSTEM_CTRL_REQ_REQUEST)
    {
        pHdr->msgID = MAC_RECONFIGURE_UE_ENTITY;
        pHdr->length += sizeof(U32);
        *Param = 0x77;
        Param ++;
        while((InBuf = GetParamHeader(ParHeader, InBuf, &InLen)) != NULL)
        {
            switch(ParHeader->paramID)
            {
                case CELL_CONF_REQ:
                      break;
                case RADIOBEARER_LIST:
                    {
                        U32 i = 0;
                        U32 Len = 0;
                        U32 ArSize;
                        InBuf = GetDataU32(&ArSize, InBuf, &InLen);
                        for(i = 0; i < ArSize; i++)
                        {
                            InBuf = GetParamHeader(ParHeader, InBuf, &InLen);
                            if(ParHeader->paramID == RADIOBEARER_TYPE)
                            {
                                InBuf = SetRadioBearer(InBuf, &InLen, Param, &Len);
                                Param += Len;
                                pHdr->length += Len;
                            }

                        }

                      break;
                    }
                default:
                     break;
            }
        }
    }
    MsgDispatch(MSGS_RRC, pHdr->length , pHdr); //goto upper layer
        
    return 0;
}

U8* SetRadioBearer(U8* InBuf, U32* InLen, U32* OutBuf, U32* OutLen)
{
    PPARAMHEADER ParHeader =  malloc(sizeof(PARAMHEADER));
    PPARAMHEADER MacParHeader = (PPARAMHEADER)OutBuf;
    U32 SrbId;
    U32 SrbIdNew;

    InBuf = GetParamHeader(ParHeader, InBuf, InLen);
    switch(ParHeader->paramID)
    {
    case RADIOBEARER_ID:
        InBuf = GetDataU32(&SrbId, InBuf, InLen);
        MacParHeader->paramID = PAR_RECONFIGURE_LC;
        *MacParHeader->data = SrbId;
        *OutLen = sizeof(U32)+ sizeof(U32);
        OutBuf = (U32*)(&MacParHeader->data[1]);
        InBuf = SetRadioBearer(InBuf, InLen, OutBuf, OutLen);
        MacParHeader->length = *OutLen;
        break;
    case RADIOBEARER_CONFIG:
        InBuf = SetRadioBearer(InBuf, InLen, OutBuf, OutLen);
        break;
    case RADIOBEARER_CONFIG_INFO:
        InBuf = SetRadioBearer(InBuf, InLen, OutBuf, OutLen);
        break;
    case PDCP_CONFIG_TYPE:
        InBuf++;
        (*InLen)--;
        InBuf = SetRadioBearer(InBuf, InLen, OutBuf, OutLen);
        break;
    case RLC_CONFIG_TYPE:
        InBuf+=2;
        (*InLen)-=2;
        InBuf = SetRadioBearer(InBuf, InLen, OutBuf, OutLen);
        break;
    case LOGICAL_CHANNEL_ID:
        {
        U32 temp;
        InBuf = GetDataU32(&temp, InBuf, InLen);
        InBuf = SetRadioBearer(InBuf, InLen, OutBuf, OutLen);
        break;
        }
    case MAC_CONFIG:
        InBuf = SetRadioBearer(InBuf, InLen, OutBuf, OutLen);
        break;
    case MAC_LOGICAL_CHANNEL_CONFIG:
        InBuf = SetRadioBearer(InBuf, InLen, OutBuf, OutLen);
        break;
    case MAC_TESTMODE_CONFIG:
        InBuf = SetRadioBearer(InBuf, InLen, OutBuf, OutLen);
        break;
    case MAC_TESTMODE_INFO:
        InBuf = SetRadioBearer(InBuf, InLen, OutBuf, OutLen);
        break;
    case MAC_TEST_DLLOG_CH_ID:
        InBuf = GetDataU32(&SrbIdNew, InBuf, InLen);
        InBuf = GetParamHeader(ParHeader, InBuf, InLen);
        MacParHeader = (PPARAMHEADER)OutBuf;
        MacParHeader->paramID = PAR_DL_LC_RECONFIG_TEST;
        MacParHeader->length = sizeof(U32)+ sizeof(U32);
        MacParHeader->data[0] = SrbIdNew;
        if(ParHeader->paramID == MAC_TEST_SCH_NoHEADER_MANIP_TYPE)
        {
            InBuf = GetDataU32(&MacParHeader->data[1], InBuf, InLen);
            MacParHeader->length += sizeof(U32);
        }
        *OutLen += MacParHeader->length;
        break;
    case MAC_TESTMODE_CONFIG_NONE:
        MacParHeader = (PPARAMHEADER)OutBuf;
        MacParHeader->paramID = PAR_DL_LC_RECONFIG_TEST;
        MacParHeader->length = sizeof(U32)+ sizeof(U32);
        MacParHeader->data[0] = 1;// *InBuf;
        InBuf++;
        *OutLen += MacParHeader->length;
        break;
    default:
        printf("!!!!!!!!!!!!!!TTCN request parse error: %x unknown message\n", ParHeader->paramID);
        return InBuf;
    }

    return InBuf;
}
#endif /* ENODEB */
