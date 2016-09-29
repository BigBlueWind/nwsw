/** @file ltemac.c
 *
 * @brief
 * @author Mindspeed Technologies
 * @version $Revision: 1.8 $
 *
 * COPYRIGHT(c) 2008 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#include "osal.h"
#include "ltemaccore.h"
#include "math.h"
#include "macMemUtil.h"
#include "ctrlmsg.h"
#include "fdxmac.h"
#include "rrciface.h"
#include "LteMacTestCfgMgr.h"
#include "bsmac.h"

//
// Mode of operation
//
unsigned int MacRtMode = MACMODE_NRT;
unsigned int PhyOnLinuxRT = 0;

unsigned int MacMaxChanDescs = 39;

extern MAC_PDU_DESC MacPduDesc[];
extern BS_CONTEXT* pBsMacDesc;
extern ULSUBFRDESC UlSubFrame;
extern DLSUBFRDESC DlSubFrame;

// TODO: List of IDs
/** @brief Create a logic channel.
 *
 *  @param LogicChanType The type of the logic channel: DCCH, DTCH, etc.
 *  @param LogicChanID
 *  @return Standard result code
 *
 *  Detailed description starts here.
*/
#ifdef ENODEB
RESULTCODE LteMacCreateLogicChannel(void* handle,
                                    U32 LogicChanType,
                                    U32 LogicChanID)
{
    UE_DESC *pUe = (UE_DESC*) handle;
    PLOGIC_CHAN_DESC    pLogicChan;
    RESULTCODE Status = FAILURE;

    switch(LogicChanType)
    {
    case CCCH:
        LogicChanID = CCCH;
        break;
    case DCCH:
        LogicChanID = DCCH; // DCCH can be 2
        break;
    case DTCH:
        {
            U32 i;
            for(i = LCID_MIN_LCHAN; i< (LCID_MAX_LCHAN - LCID_MIN_LCHAN + 1); i++)
            {
                if(pUe->pLogicChan[i] == NULL)
                    break;
            }
            if(i < LCID_MAX_LCHAN)
                LogicChanID = i;
            break;
        }
    default:
        break;

    }
    if (LogicChanID <= LCID_MAX_LCHAN)
    {
        if (pUe->pLogicChan[LogicChanID] != NULL)
        {
            return Status;
        }
        pLogicChan = malloc(sizeof(LOGIC_CHAN_DESC));
        if (pLogicChan)
        {
            // TODO: Remove memset in the future
            memset(pLogicChan, 0, sizeof(LOGIC_CHAN_DESC));
            pLogicChan->LCID = (U16)LogicChanID;
            pLogicChan->ChanType = (U16)LogicChanType;
            // TODO: MCs depends on test
            pLogicChan->DownLinkMCS = 28;
            pLogicChan->UpLinkMCS = 28;

            if (pUe->pLogicChan[LogicChanID] == NULL)
            {
                pUe->pLogicChan[LogicChanID] = pLogicChan;
                pUe->NLogicChannel++;
            }
            Status = SUCCESS;
        }
        else
        {
            printf("Create Logic Channel Error: fail malloc \n");
        }
    }
    else
    {
        printf("Create Logic Channel Error: invalid logic ID:%d \n",
            LogicChanID);
    }
    return Status;
}
void SetTestLogicalChannelId_Type(U32 UeId,
                                  U32 LogicChanIDOld,
                                  U32 LogicChanIDNew)
{
        UE_DESC *pUe = GetUeContext(UeId);
        if (pUe == NULL)
        {
            printf("MAC ERROR: the UE with %d ID did not find\n", UeId);
            return;
        }
        if(pUe->pLogicChan[LogicChanIDOld] != NULL)
        {
            pUe->pLogicChan[LogicChanIDOld]->LCID = LogicChanIDNew;
            printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! %d-old LCid, %d-new LCid\n",
                LogicChanIDOld,LogicChanIDNew);

        }
}
void LteMacPduDecoder(U8* pMacPdu, U32 Len, PTR hCtx)
{
   // PLOGIC_CHAN_DESC* LChArr, MacRlcIoFunc toUpper)
    UE_DESC* UeCtx = (UE_DESC*)hCtx;
    PLOGIC_CHAN_DESC* LChArr = UeCtx->pLogicChan;
    int i;
    U8 lcid;
    MAC_PDU_DESC pduDesc;
    int blocks = 0;
    U32 SduLen;
    U32 HeadSize;

    for (i=0;;)
    {
        HeadSize = GetMacHeaderSize(pMacPdu,&SduLen);
        if (Len >= HeadSize)
        {
            Len -= HeadSize;
        }
        else
        {
            printf("MAC Decoder Error: wrong MAC header\n");
            return;
        }

        if (SduLen)
        {
            pduDesc.SduBlockLen[i] = SduLen;
            pduDesc.LCID[i] = ((MAC_HEADER*)pMacPdu)->Long.LCID;
            i++;
            if (Len >= SduLen)
            {
                Len -= SduLen;
            }
            else
            {
                printf("MAC Decoder Error: wrong MAC SDU%d length %d\n",
                    i, SduLen);
                break;
            }

        }
        if (((MAC_HEADER*)pMacPdu)->Long.E == 0)
        {
            blocks = i;
            pMacPdu += HeadSize;
            break;
        }
        pMacPdu += HeadSize;
    }
    for (i=0; i< blocks; i++)
    {
       lcid = pduDesc.LCID[i];
       if (lcid == LCID_PADDING)
       {
           return;
       }
       if ((LChArr[lcid] == NULL) || (lcid > LCID_MAX_LCHAN))
        {
            printf("MAC Decoder Error: LCH %x does not open\n", lcid );
            return;
        }

        UeCtx->LteMacPutDataBlock(NULL, pMacPdu, pduDesc.SduBlockLen[i]);// putDataBlock
        pMacPdu += pduDesc.SduBlockLen[i];
    }
}

#elif UE
RESULTCODE LteUeMacCreateLogicChannel(PTR dlhandle,
                                        PTR ulhandle,
                                        U16 tag,
                                        U16 LogicChanID,
                                        U16 LogicChanType,
                                        U32 RrcMode)
{
    DlUeContext* DlUeCtx = (DlUeContext*)dlhandle;
    UlUeContext* UlUeCtx = (UlUeContext*)ulhandle;
    PLOGIC_CHAN_DESC    pLogicChan = NULL;
    RESULTCODE Status = FAILURE;

    if (tag == PAR_DL_LC_CREATE)
    {
        if(DlUeCtx && DlUeCtx->numLogicalChannels < MAX_NUMBER_OF_LOGICAL_CHANNEL)
        {
            pLogicChan = malloc(sizeof(LOGIC_CHAN_DESC));
            if (pLogicChan)
            {
                memset(pLogicChan, 0, sizeof(LOGIC_CHAN_DESC));
                pLogicChan->LCID = LogicChanID;
                pLogicChan->ChanType = LogicChanType;
                pLogicChan->DownLinkMCS = 28;
                if (DlUeCtx->logicalChannel[LogicChanID] == NULL)
                {
                    DlUeCtx->logicalChannel[LogicChanID] = pLogicChan;
                    DlUeCtx->numLogicalChannels ++;
                }
                else
                {
                    printf("Create Logic Channel Error: fail malloc \n");
                    Status = FAILURE;
                }
                Status = SUCCESS;
            }
        }
       else
       {
              printf("Create Logic Channel Error: invalid logic ID:%d \n",
              LogicChanID);
              Status = FAILURE;
       }

    }
    else 
        if(tag == PAR_UL_LC_CREATE)
        {
            if(UlUeCtx && UlUeCtx->numLogicalChannels < MAX_NUMBER_OF_LOGICAL_CHANNEL)
            {
                pLogicChan = malloc(sizeof(LOGIC_CHAN_DESC));
                if (pLogicChan)
                {
                    memset(pLogicChan, 0, sizeof(LOGIC_CHAN_DESC));
                    pLogicChan->LCID = LogicChanID;
                    pLogicChan->ChanType = LogicChanType;
                    pLogicChan->UpLinkMCS = 28;

                    if (UlUeCtx->logicalChannel[LogicChanID] == NULL)
                    {
                        UlUeCtx->logicalChannel[LogicChanID] = pLogicChan;
                        UlUeCtx->numLogicalChannels ++;
                    }
                    else
                    {
                        printf("Create Logic Channel Error: fail malloc \n");
                        Status = FAILURE;
                    }
                    Status = SUCCESS;
                }
            }
            else
            {
                  printf("Create Logic Channel Error: invalid logic ID:%d \n",
                  LogicChanID);
                  Status = FAILURE;
            }
        }
        else
        {
            printf("LTE MAC ERROR: invalid MAC created tag");
            return FAILURE;
        }
    return Status;
}
void SetTestLogicalChannelId_Type(PTR handle,
                                  U16 tag,
                                  U32 LogicChanIDOld,
                                  U32 LogicChanIDNew)

{
    if(tag == PAR_DL_LC_RECONFIG)
    {
        DlUeContext* DlUeCtx = (DlUeContext*)handle;
        if(DlUeCtx->logicalChannel[LogicChanIDOld] != NULL)
        {
            DlUeCtx->logicalChannel[LogicChanIDOld]->LCID = LogicChanIDNew;
            printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! %d-old LCid, %d-new LCid\n",
                LogicChanIDOld,LogicChanIDNew);
        }

    }
    else
    {
    UlUeContext* UlUeCtx = (UlUeContext*)handle;
    if(UlUeCtx->logicalChannel[LogicChanIDOld] != NULL)
        {
            UlUeCtx->logicalChannel[LogicChanIDOld]->LCID = LogicChanIDNew;
            printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! %d-old LCid, %d-new LCid\n",
                LogicChanIDOld,LogicChanIDNew);
        }

    }

}
#endif /*UE*/


/** @brief Schedule the downlink radio resource to selected channels.
 *
 *  @param Param1 Description of function parameter
 *  @param Param2 Description of another parameter
 *  @return Description of return value
 *
 *  Detailed description starts here.
*/
RESULTCODE LteMacDestroyLogicChannel(void* handle, U32 LogicChanId)
{
    UE_DESC *pUe = (UE_DESC*) handle;

    RESULTCODE ret = FAILURE;

    if (LogicChanId >= LCID_MIN_LCHAN && LogicChanId <= LCID_MAX_LCHAN)
    {
        if (pUe)
        {
            if (pUe->NLogicChannel == 0)
            {
                printf("Destroy Logic Channel Error: should have no active logic channel\n");
            }
            else if (pUe->pLogicChan[LogicChanId] == NULL)
            {
                printf("Destroy Logic Channel Error: invalid logic channel pointer\n");
            }
            else
            {
                MacFree((void**)&pUe->pLogicChan[LogicChanId - LCID_MIN_LCHAN]);
                pUe->NLogicChannel--;
                ret = SUCCESS;
            }
        }
        else
        {
            printf("Destroy Logic Channel Error: invalid UE\n");
        }
    }
    else
    {
        printf("Invalid LCID\n");
    }

    return ret;
}

int GetMacHeaderSize(void* h, U32* SduLen)
{
     MAC_HEADER* Header = (MAC_HEADER*)h;
     unsigned short longlen;

     if (Header->Ctrl.LCID == LCID_PADDING)
     {
         *SduLen = 0;
         return MAC_CTRL_HDR_SIZE;
     }
     else if(Header->Ctrl.LCID == LCID_ADVANCE)
     {
         *SduLen = 1;
         return MAC_CTRL_HDR_SIZE;        
     }
     else if(Header->Ctrl.LCID == LCID_DRX)
     {
         *SduLen = 0;
         return MAC_CTRL_HDR_SIZE;        
     }


     if(Header->Long.F == 1)
     {
         // Byte swap
         longlen = (Header->Long.L1 << 8) | Header->Long.L2;
        *SduLen = longlen;
        return MAC_SDU_HDR_SIZE_LONG;
     }
     if(Header->Long.F == 0)
     {
        *SduLen = Header->Short.L;
        return MAC_SDU_HDR_SIZE_SHORT;
     }
     return 0;
}
void LastSduHeader(void* h)
{
    MAC_HEADER* Header = (MAC_HEADER*)h;
    Header->Ctrl.E = 0;
}

U8 GetLastSduHeaderStatus(void* h)
{
    MAC_HEADER* Header = (MAC_HEADER*)h;
    return Header->Ctrl.E;
}

U32 AddMacHeader(MAC_HEADER* Header, U32 len, U32 LcId)
{
	if(len == 0)
	{
		Header->Ctrl.R = 0;
		Header->Ctrl.LCID = LCID_PADDING;     // Padding
		Header->Ctrl.E = 1;                   // Last subheader
		return MAC_CTRL_HDR_SIZE;
	}

	if (len < 128)
    {
        Header->Short.F = 0;
        Header->Short.E = 1;
        Header->Short.LCID = LcId;
        Header->Short.L = len;                 // 7 bits
        Header->Short.R = 0;
        return MAC_SDU_HDR_SIZE_SHORT;
    }

    Header->Long.F = 1;
    Header->Long.E = 1;
    Header->Long.LCID = LcId & 0x1F;
    // Swap bytes
    Header->Long.L1 = (len >> 8) & 0x7F;                 // 15 bits
    Header->Long.L2 = len & 0xFF;
    Header->Long.R = 0;
    return MAC_SDU_HDR_SIZE_LONG;

}
static void QueueLock(OSAL_QUEUE* LCQueue)
{
#ifdef USE_LINUX
    pthread_spin_lock(&LCQueue->lock);
#else
    EnterCriticalSection(&LCQueue->lock);
#endif
}
static void QueueUnLock(OSAL_QUEUE* LCQueue)
{
#ifdef USE_LINUX
    pthread_spin_unlock(&LCQueue->lock);
#else
    LeaveCriticalSection(&LCQueue->lock);
#endif
}

U8* AddMacHeader2Buffer(PLOGIC_CHAN_DESC pLogicCh, U8* buf, U32* pblockSize, U8* blockNum)
{
    OSAL_QUEUE* LCQueue = pLogicCh->LCQueue;
    U32 temp_get = 0;
    U32 len ;
    U32 MacHeaderSize = 0;
    PTR ptr = NULL;
    U32 NumPadding = *pblockSize;
    U32 SduChId;

    *blockNum = 0;
    QueueLock(LCQueue);
    if(LCQueue->get == LCQueue->put)
    {
        QueueUnLock(LCQueue);
         return buf;
    }
    temp_get = LCQueue->get;
    while (temp_get != LCQueue->put)
    {
        ptr = LCQueue->storage[temp_get];
        if (ptr == NULL)
        {
            break;
        }
        len = pLogicCh->BufferLength(ptr);
        if(len < (NumPadding - MAC_SDU_HDR_SIZE_LONG))
        {
            SduChId = pLogicCh->BufferlcID(ptr);
            (*blockNum)++;
            MacHeaderSize =AddMacHeader((MAC_HEADER*)buf, len, pLogicCh->BufferlcID(ptr));
            NumPadding -= (len + MacHeaderSize);
            if(NumPadding == 0)
            {
                LastSduHeader(buf);
                buf += MacHeaderSize;
                *pblockSize = NumPadding;
                QueueUnLock(LCQueue);
                return buf;
            }
            buf += MacHeaderSize;
        }
        else
            break;
       if (++temp_get == LCQueue->size)
            temp_get = 0;
    }
    QueueUnLock(LCQueue);
       
    while(NumPadding)
    {
        AddMacHeader((MAC_HEADER*)buf, 0, LCID_PADDING);
        NumPadding--;
        if(NumPadding)
            buf++;
    }
    LastSduHeader(buf);
    buf++;
    NumPadding = 0;
    
    *pblockSize = NumPadding;
    return buf;
}

/** @brief Encode the MAC PDU
 *
 *  @param Param1 Description of function parameter
 *  @param Param2 Description of another parameter
 *  @return Description of return value
 *
 *  Detailed description starts here.
*/
void LteMacPduEncoder(MAC_PDU_DESC *pMacPduDesc, UE_DESC *pUe, U32 SubFrameType)
{
    U32 i,j;
    U32 len;
    U16 SduChId;
    U32 buf_offset = 0;
    U32 rest_len =0;
    U8 BlockNum[32] = {0};
    U8* PduBuf,*buf;
    U32 put;
    U32 k,m;
    PTSTPDESC pTstpbsDl = &pBsMacDesc->tstpbsDl;
    U32 Offset;
    U32 MaxSduSize = MAX_MAC_SDU_SIZE + sizeof(MSGHEADER) + sizeof(TXSDUREQ);

#ifdef ENODEB
    for (k = 0; k < pTstpbsDl->numDataCh; k++)
    {
        Offset= 0;
        for (m = 0; m < DlSubFrame.dlCh[k].numCodeWords; m++)
        {
            if (m == 1)
            {
                Offset = FdxMac.tCh[k].tbBytes[0];
            }
            len = FdxMac.tCh[k].tbBytes[m];
#ifdef PHY_SDU_36696
            len = 36696/8;
#endif
#else /* ENODEB */
            len = TX_FIX_SIZE - 3;
#endif /* ENODEB */

            put = pMacPduDesc->NumPutBlock;
            if (++put == MAX_SDU_BLOCKS)
                put = 0;
            if (put == pMacPduDesc->NumGetBlock)
            {
                return;
            }
            pMacPduDesc->TBSize = len;
            PduBuf = (U8*) MacAllocBlock(MaxSduSize);
            if(PduBuf == NULL)
            {
                printf(C4KFW "ERROR: No memory [%s]\n", __FUNCTION__);
                return;
            }
            buf = (U8*)MsgGetDataOffset(PduBuf) + sizeof(TXSDUREQ);
            rest_len = pMacPduDesc->TBSize;
            for (i = 0, j = 0; i < pUe->NLogicChannel; i++)
            {
                // FIXME: BlockNum[CCCH] was not initialized
                if ((BlockNum[CCCH] != 0) && (i == DCCH))
                    continue;
                buf = AddMacHeader2Buffer(pUe->pLogicChan[i], buf, &rest_len, &BlockNum[i]);
                if (rest_len == 0)
                    break;
            }
            if (rest_len == pMacPduDesc->TBSize)
            {
                MacFree((void**)&PduBuf);
                return;
            }
            for (i = 0, j = 0; i < pUe->NLogicChannel; i++)
            {
                if (pUe->pLogicChan[i]->LteMacGetDataBlock)
                {
                    while (BlockNum[i]--)
                    {
                        // FIXME: BlockNum[CCCH] was not initialized
                        if ((BlockNum[CCCH] != 0) && (i == DCCH))
                            continue;
                        buf_offset = pUe->pLogicChan[i]->LteMacGetDataBlock(&SduChId, buf, len);
                        buf += buf_offset;
                    }
                }
            }
            pMacPduDesc->pSduBlock[pMacPduDesc->NumPutBlock] = PduBuf;
            pMacPduDesc->NumPutBlock = put;
#ifdef ENODEB
        }
    }
#endif
}

void LteMacReleasePdu(MAC_PDU_DESC* pMacPduDesc)
{
    U32 i;

    // release each pdu blocks.
    for (i = 0; i < pMacPduDesc->NumSduBlock; i++)
    {
        MacFreeBlock((void**) &pMacPduDesc->pSduBlock[i]);
        pMacPduDesc->SduBlockLen[i] = 0;
    }
    pMacPduDesc->NumSduBlock = 0;
    pMacPduDesc->pUe = NULL;

//  MacFree((void**)pPMacPduDesc);
}

void LteMacReleaseSubFrame(PDLSUBFRDESC pSubFrame)
{
    unsigned int i;
    MAC_PDU_DESC* pMacPduDesc;

    i = pSubFrame->numberofChannelDescriptors;
    if (i >= MacMaxChanDescs)
    {
        // FIXME: This is debug assert
        printf("Invalid number of descriptors\n");
        i = MacMaxChanDescs - 1;
    }
    while (i)
    {
        pMacPduDesc = &MacPduDesc[i];
        LteMacReleasePdu(pMacPduDesc);
        i--;
    }
    pSubFrame->numberofChannelDescriptors = 0;
}

char LteMacHasSthToSend(PDLSUBFRDESC pSubFrame)
{
    int i;

    if (pSubFrame->numberofChannelDescriptors < MacMaxChanDescs)
    {
        for (i = 0; i < (int) pSubFrame->numberofChannelDescriptors; i++)
        {
            if (MacPduDesc[i].NumSduBlock > 0)
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}

