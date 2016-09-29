/** @file msgdisp.c
 *
 * @brief MsgDispatch and needed for it functions from ctrlmsg.c
 * @author Mindspeed Technologies
 * @version $Revision: 1.0 $
 *
 * COPYRIGHT(c) 2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#include "osal.h"
#include "ctrlmsg.h"
#include "msgroute.h"
#include "phyapi.h"
#include "icpu.h"
#include "icpu2_lnx.h"
#include "svsr.h"
#include "diags.h"

#include "apidefs.h"

#include <stdarg.h>
#include <stdio.h>

extern int32_t LteMacMsgHandler(void* pClientCtx, void* pMsg);

typedef char __int8;

// Max number of messages in partition
#define MSG_MAXMSG                  1280

#define MSG_SIZE            pHdr->length + sizeof(MSGHEADER)

extern OSAL_HEAP OsalGlobalHeap;
OSAL_MEMARRAY MsgArray;
OSAL_PADDRESS MsgArrayMem;
extern int IcpuMode;

OSAL_MEMARRAY MsgArrayPack;
OSAL_PADDRESS MsgArrayMemPack;
OSAL_TASK PackToHost;
OSAL_SEMAPHORE PackToHostSema;
OSAL_QUEUE PackQueue;

MsgFreeBufferCb     MsgFreeBuffer  = NULL;
MsgAllocBufferCb    MsgAllocBuffer = NULL;

#if defined (C4KEVM)
TMsgICpuStat MsgICpuStat;
#endif

//
// Dynamically registered dispatchers
//

MSGDISPATCHER MsgDispatchers[MSGS_DYNAMIC];

/** Initialize message dispatcher
 *
 * @return Standard result code
 */
EXPORT RESULTCODE MsgDispatcherInit(void)
{
    int i;
    OSAL_PADDRESS pMem = NULL;
    U32 num = MSG_MAXMSG;

    for (i = 0; i < MSGS_DYNAMIC; i++)
    {
        MsgDispatchers[i] = NULL;
    }

    if (OsalAllocHeap(&OsalGlobalHeap, &MsgArrayMem, MSG_MAXSIZE * MSG_MAXMSG) != SUCCESS)
    {
        DiagPrint(DIAG_LEVEL0, "MSG", "No memory [%s,%i]\n", __FILE__, __LINE__);
        return FAILURE;
    }
    OsalCreateMemArray(&MsgArray, MsgArrayMem, MSG_MAXSIZE * MSG_MAXMSG, MSG_MAXSIZE);

	
    if (OsalAllocHeap(&OsalGlobalHeap, &MsgArrayMemPack, MSG_MAXSIZE * num) != SUCCESS)
    {
        DiagPrint(DIAG_LEVEL0, "MSG", "No memory [%s,%i]\n", __FILE__, __LINE__);
        return FAILURE;
    }
	

    if (OsalAllocHeap(&OsalGlobalHeap,
                           &pMem,
                           num * sizeof(OSAL_PADDRESS)) != SUCCESS)
    {
        printf("Error in funtion %s (%s:%d)\n",
               __FUNCTION__, __FILE__, __LINE__);
        return FAILURE;
    }
    if (OsalCreateQueue(&PackQueue,pMem,num * sizeof(OSAL_PADDRESS)) != SUCCESS)
    {
        printf("EnodeB error: PackQueue\n");
        return FAILURE;
    }

    if (OsalCreateSema(&PackToHostSema) != SUCCESS)
    {
        printf("EnodeB error: PackToHostSema\n");
        return FAILURE;
    }

    OsalActivateTask(&PackToHost);
    return SUCCESS;
}

void MsgDispatcherDone(void)
{
    if (MsgArrayMem != NULL)
    {
        OsalDestroyMemArray(&MsgArray);
        OsalFreeHeap(&OsalGlobalHeap, MsgArrayMem);
        MsgArrayMem = NULL;
    }
    if(MsgArrayMemPack != NULL)
    {
        OsalDestroyMemArray(&MsgArrayPack);
        OsalFreeHeap(&OsalGlobalHeap, MsgArrayMemPack);
    }
    OsalDestroySema(&PackToHostSema);
    OsalDeactivateTask(&PackToHost);
}

U32 ProcCRC(U32 * pRemainder, U32 polynomial, U32 width, U8 const message[], U32 nBytes)
{
	U32 mask = 0, byte, bit;
	U32 topbit = 1 << (width - 1);

	switch (width)
	{
		case 8: 
			mask = 0xFF;
			break;

		case 12: 
			mask = 0xFFF;
			break;

		case 16: 
			mask = 0xFFFF;
			break;

		case 24: 
			mask = 0xFFFFFF;
			break;

		case 32: 
			mask = 0xFFFFFFFF;
			break;

		default:
			return 0;
	}

    /*
     * Perform modulo-2 division, a byte at a time.
     */
    for (byte = 0; byte < nBytes; ++byte)
    {
        /*
         * Bring the next byte into the remainder.
         */
		*pRemainder ^= (message[byte] << (width - 8));

		// mycode
		*pRemainder &= mask;

        /*
         * Perform modulo-2 division, a bit at a time.
         */
        for (bit = 8; bit > 0; --bit)
        {
            /*
             * Try to divide the current data bit.
             */
			if (*pRemainder & topbit)
            {
				*pRemainder = (((*pRemainder & mask)<< 1) ^ polynomial) & mask;
            }
            else
            {
				*pRemainder = (((*pRemainder & mask) << 1)) & mask;
            }
        }
    }

    /*
     * The final remainder is the CRC result.
     */
    return (*pRemainder) & mask;

}


void CheckDataCRC(PTR msg)
{
    PMSGHEADER pHdr = (PMSGHEADER) msg;
    U32 RefCRC = 0;
    U32 RealCRC = 0;
    U8  DataParIndex;
    U8  CrcParIndex;

    // Define parameters index
    if (pHdr->msgID == SVSR_WRITE_MEMORY)       // command
    {
        DataParIndex = 3;
        CrcParIndex  = 4;
    }
    else if (pHdr->msgID == SVSR_READ_MEMORY)   // query response
    {
        DataParIndex = 2;
        CrcParIndex  = 3;
    }
    else return;

    // Corrupting Payload (for feature testing)
/*    {
        PPARAMHEADER pPayload = MsgGetParam(pHdr, DataParIndex);
        pPayload->data[0] = 0;
        printf("Corrupting!\n");
    }*/

    // Check Reference & Real CRCs
    if (MsgGetU32(pHdr, CrcParIndex, &RefCRC) == SUCCESS)
    {
        PPARAMHEADER pPayload;

        // Calculate Real CRC
        pPayload = MsgGetParam(pHdr, DataParIndex);
        RealCRC = ProcCRC(&RealCRC, 0x864CFB, 32, (U8 *)(&pPayload->data[0]), pPayload->length);

        // Add error code for wrong CRC
        if (RealCRC != RefCRC)
        {
            if (pHdr->msgID == SVSR_WRITE_MEMORY)
            {
                MsgAppendU32(pHdr, 1, SCTP_WRITE_ERROR);
                printf("CRC mismatch for SVSR_WRITE_MEMORY\n");
            }
            else if (pHdr->msgID == SVSR_READ_MEMORY)
            {
                MsgAppendU32(pHdr, 1, ICPU_READ_ERROR);
                printf("CRC mismatch for SVSR_READ_MEMORY\n");
            }

            // Print CRC info
            printf("Real      CRC: 0x%08x\n", RealCRC);               
            printf("Reference CRC: 0x%08x\n", RefCRC);
        }
    }

/*    // Corrupting Payload (for feature testing)
    {
        PPARAMHEADER pPayload = MsgGetParam(pHdr, DataParIndex);
        pPayload->data[0] = 0;
        printf("Corrupting!\n");
    }*/
}


/*! \brief This function routes all control messages
 *
 *  \param srcID ID of message source
 *  \param size Size of message in bytes (including all headers)
 *  \param msg Pointer to next control message available
 *  \return Standard result code
*/
EXPORT RESULTCODE MsgDispatch(U32 srcID, U32 size, PTR msg)
{
    PMSGHEADER pHdr = (PMSGHEADER) msg;
    RESULTCODE ret = FAILURE;
    U16 route = pHdr->dstID & MSGS_ROUTE;

//PTR tmp_buf = NULL;
//    printf("API: Src=%4.4X(%4.4X) Dst=%4.4X Type=%d MsgID=%4.4X Ctrl=%X Len=%d W0=%X W1=%X\n", pHdr->srcID, srcID, pHdr->dstID, pHdr->type, pHdr->msgID, 
//		pHdr->control, pHdr->length, *(U32*)((U32)pHdr+16), *(U32*)((U32)pHdr+ 20));

#if defined (C4KEVM)
    if ((pHdr->srcID & MSGS_ROUTE) == MSGS_LARM)
    {
        MsgICpuStat.RxMsgNum ++;
        MsgICpuStat.RxApi = *pHdr;
    }
#endif

    if (route == MSGS_UARM)
    {
        // Check CRC
        if (pHdr->msgID == SVSR_READ_MEMORY)
        {
            CheckDataCRC(msg);
            size = MSG_SIZE;
        }

        //
        // Default route
        //
        switch (pHdr->dstID)
        {
#ifdef USE_RPC
            case MSGS_RPC_REQ:
                ret = LteMacMsgHandler(NULL, msg);                    //TODO Helal, check it with Mark
                break;
            case MSGS_RPC_REP:
                ret = LteMacMsgHandler(NULL, msg);                    //TODO Helal, check it with Mark
                break;
            case 51: //TODO helal put the correct IID_LTE_RPC
            	ret = LteMacMsgHandler(NULL, msg);                    //TODO Helal, check it with Mark
            	break;
#endif /* USE_RPC */
			default:
                // Look for dynamically registered message
                if ((pHdr->dstID > MSGS_LAST) && (pHdr->dstID <= MSGS_LAST + MSGS_DYNAMIC))
                {
                    if (MsgDispatchers[pHdr->dstID-MSGS_LAST-1] != NULL)
                        ret = MsgDispatchers[pHdr->dstID-MSGS_LAST-1](size, msg);
                    else
                        MsgFreeBuffer(msg);
                }
                else
                    MsgFreeBuffer(msg);
                break;
        }
    }
    else if (route == MSGS_LARM)
    {
        // Check CRC
        if (pHdr->msgID == SVSR_WRITE_MEMORY)
        {
            CheckDataCRC(msg);
            size = MSG_SIZE;
        }
        
#if defined (C4KEVM)
        MsgICpuStat.TxMsgNum ++;
        MsgICpuStat.TxApi = *pHdr;
#endif
#ifndef ARM_OPTIMIZE
        fprintf(stderr, "SendToLARM: %d %X\n", srcID, (U32)IcpuSendMsg);
#endif /* ARM_OPTIMIZE */
    
        switch (srcID)
        {
        case MSGS_UARM:

                MsgFreeBuffer(msg);
            break;
        case MSGS_LARM:
#ifndef ARM_OPTIMIZE
			fprintf(stderr, "MsgDispatch() IcpuSendMsg \n"); //make it nomlog after debugging
#endif /* ARM_OPTIMIZE */
        	if (!IcpuSendMsg(size, msg)) {
#ifndef ARM_OPTIMIZE
        		fprintf(stderr, "MsgDispatch() IcpuSendMsg() returned false \n");
#endif /* ARM_OPTIMIZE */
        	}
            //MsgFreeBuffer(msg);
            break;
        default:
#ifndef ARM_OPTIMIZE
			  fprintf(stdout, "msgdisp.c: default to LARM=%u\n", srcID);
#endif /* ARM_OPTIMIZE */
            if (IcpuSendMsg)
                ret = IcpuSendMsg(size, msg);
            else
                MsgFreeBuffer(msg);
            break;
        }
    }
    else if (route == MSGS_LOCAL)
    {
        MsgFreeBuffer(msg);
    }

    return ret;
}


/*! \brief Append parameter to the message
 *
 *  \param msg Pointer to allocated mesage
 *  \param paramID ID of parameter
 *  \param paramLen Length of parameter (exact size in bytes)
 *  \param paramData Pointer to actual values
 *  \return Standard result code
*/
RESULTCODE MsgAppendParam(PMSGHEADER msg, U16 paramID, U16 paramLen, PTR paramData)
{
    PPARAMHEADER ptr = (PPARAMHEADER) &msg->param[RoundUp32(msg->length) / sizeof(U32)];

    if (paramLen > MSG_MAXSIZE - sizeof(MSGHEADER) - sizeof(PARAMHEADER) - msg->length - sizeof(ptr->data[0]))
    {
        return FAILURE;
    }
    ptr->paramID = paramID;
    ptr->length = paramLen;

    memcpy(ptr->data, paramData, paramLen);

    msg->length = RoundUp32(msg->length) + sizeof(PARAMHEADER) + ptr->length - sizeof(ptr->data[0]);

    return SUCCESS;
}

/*! \brief Append U8 parameter to the message
 *
 *  \param msg Pointer to allocated mesage
 *  \param paramID ID of parameter
 *  \param paramData Unsigned char to append
 *  \return Standard result code
*/
RESULTCODE MsgAppendU8(PMSGHEADER msg, U16 paramID, U8 paramData)
{
    PPARAMHEADER ptr = (PPARAMHEADER) &msg->param[RoundUp32(msg->length) / sizeof(U32)];

    ptr->paramID = paramID;
    ptr->length = sizeof(U8);       // Use real length
    ptr->data[0] = (U32)paramData;

    msg->length = RoundUp32(msg->length) + sizeof(PARAMHEADER) - sizeof(ptr->data[0]) + ptr->length;

    return SUCCESS;
}

/*! \brief Append U16 parameter to the message
 *
 *  \param Msg Pointer to allocated mesage
 *  \param ParamID ID of parameter
 *  \param ParamData Unsigned short to append
 *  \return Standard result code
*/
RESULTCODE MsgAppendU16(PMSGHEADER msg, U16 paramID, U16 paramData)
{
    PPARAMHEADER ptr = (PPARAMHEADER) &msg->param[RoundUp32(msg->length) / sizeof(U32)];

    ptr->paramID = paramID;
    ptr->length = sizeof(U16);      // Use real length
    ptr->data[0] = (U32)paramData;

    msg->length = RoundUp32(msg->length) + sizeof(PARAMHEADER) - sizeof(ptr->data[0]) + ptr->length ;

    return SUCCESS;
}

/*! \brief Append U32 parameter to the message
 *
 *  \param Msg Pointer to allocated mesage
 *  \param ParamID ID of parameter
 *  \param ParamData Unsigned int to append
 *  \return Standard result code
*/
RESULTCODE MsgAppendU32(PMSGHEADER msg, U16 paramID, U32 paramData)
{
    PPARAMHEADER ptr = (PPARAMHEADER) &msg->param[RoundUp32(msg->length) / sizeof(U32)];

    ptr->paramID = paramID;
    ptr->length = sizeof(U32);      // Use real length
    ptr->data[0] = paramData;

    msg->length = RoundUp32(msg->length) + sizeof(PARAMHEADER);

    return SUCCESS;
}

/** Get U8 type parameter
 *  If such parameter not present in the message, return error
 *
 * @param msg Pointer to message
 * @param paramID Parameter ID to search
 * @param val [OUT] Value of the parameter
 * @return Standard result code
 */
RESULTCODE MsgGetU8(PMSGHEADER msg, U16 paramID, U8 *value)
{
    PPARAMHEADER par = MsgGetNextParam(msg, NULL);
    while (par)
    {
        if (par->paramID == paramID)
        {
            *value = (U8) par->data[0];
            return SUCCESS;
        }
        par = MsgGetNextParam(msg, par);
    }
    return FAILURE;
}

/** Get U16 type parameter
 *  If such parameter not present in the message, return error
 *
 * @param msg Pointer to message
 * @param paramID Parameter ID to search
 * @param val [OUT] Value of the parameter
 * @return Standard result code
 */
RESULTCODE MsgGetU16(PMSGHEADER msg, U16 paramID, U16 *value)
{
    PPARAMHEADER par = MsgGetNextParam(msg, NULL);
    while (par)
    {
        if (par->paramID == paramID)
        {
            *value = (U16) par->data[0];
            return SUCCESS;
        }
        par = MsgGetNextParam(msg, par);
    }
    return FAILURE;
}

/** Get U32 type parameter
 *  If such parameter not present in the message, return error
 *
 * @param msg Pointer to message
 * @param paramID Parameter ID to search
 * @param val [OUT] Value of the parameter
 * @return Standard result code
 */
RESULTCODE MsgGetU32(PMSGHEADER msg, U16 paramID, U32 *value)
{
    PPARAMHEADER par = MsgGetNextParam(msg, NULL);
    while (par)
    {
        if (par->paramID == paramID)
        {
            *value = par->data[0];
            return SUCCESS;
        }
        par = MsgGetNextParam(msg, par);
    }
    return FAILURE;
}

/** Get string or binary type parameter
 *  If such parameter not present in the message, return error
 *
 * @param msg Pointer to message
 * @param paramID Parameter ID to search
 * @param value [OUT] Value of the parameter
 * @param len [OUT] Length of the parameter
 * @return Standard result code
 */
RESULTCODE MsgGetParamVal(PMSGHEADER msg, U16 paramID, PTR *value, U32 *len)
{
    PPARAMHEADER par = MsgGetNextParam(msg, NULL);

    while (par)
    {
        if (par->paramID == paramID)
        {
            *value = &par->data[0];
            *len = par->length;
            return SUCCESS;
        }
        par = MsgGetNextParam(msg, par);
    }
    return FAILURE;
}

/** Get U8 type parameter
 *  If such parameter not present in the message, return default value
 *
 * @param msg Pointer to message
 * @param paramID Parameter ID to search
 * @param dflt Default value to use if not present
 * @return Parameter value
 */
U8 MsgGetU8Dflt(PMSGHEADER msg, U16 paramID, U8 dflt)
{
    PPARAMHEADER par = MsgGetNextParam(msg, NULL);

    while (par)
    {
        if (par->paramID == paramID)
        {
            return (U8) par->data[0];
        }
        par = MsgGetNextParam(msg, par);
    }
    return dflt;
}

/** Get U16 type parameter
 *  If such parameter not present in the message, return default value
 *
 * @param msg Pointer to message
 * @param paramID Parameter ID to search
 * @param dflt Default value to use if not present
 * @return Parameter value
 */
U16 MsgGetU16Dflt(PMSGHEADER msg, U16 paramID, U16 dflt)
{
    PPARAMHEADER par = MsgGetNextParam(msg, NULL);

    while (par)
    {
        if (par->paramID == paramID)
        {
            return (U16) par->data[0];
        }
        par = MsgGetNextParam(msg, par);
    }
    return dflt;
}

/** Get U32 type parameter
 *  If such parameter not present in the message, return default value
 *
 * @param msg Pointer to message
 * @param paramID Parameter ID to search
 * @param dflt Default value to use if not present
 * @return Parameter value
 */
U32 MsgGetU32Dflt(PMSGHEADER msg, U16 paramID, U32 dflt)
{
    PPARAMHEADER par = MsgGetNextParam(msg, NULL);

    while (par)
    {
        if (par->paramID == paramID)
        {
            return par->data[0];
        }
        par = MsgGetNextParam(msg, par);
    }
    return dflt;
}

/** Get string or binary type parameter
 *  If such parameter not present in the message, return default value
 *
 * @param msg Pointer to message
 * @param paramID Parameter ID to search
 * @param dflt Default value to use if not present
 * @param len [IN/OUT] Length of the parameter
 * @return Parameter value
 */
PTR MsgGetParamDflt(PMSGHEADER msg, U16 paramID, PTR dflt, U32 *len)
{
    PPARAMHEADER par = MsgGetNextParam(msg, NULL);

    while (par)
    {
        if (par->paramID == paramID)
        {
            *len = par->length;
            return &par->data[0];
        }
        par = MsgGetNextParam(msg, par);
    }
    return dflt;
}

/*! \brief Get next parameter from message
 *
 *  \param msg Pointer to allocated mesage
 *  \param param Pointer to previous parameter (NULL if this is 1st call)
 *  \return Pointer to next parameter or NULL when no more left
*/
PPARAMHEADER MsgGetNextParam(PMSGHEADER msg, PPARAMHEADER param)
{
    if (param == NULL)
    {
        // Called 1st time, return 1st parameter
        if (msg->length)
        {
            return (PPARAMHEADER) &msg->param[0];
        }
    }
    else
    {
        // Get next parameter
        PPARAMHEADER pNext = (PPARAMHEADER) &(param->data[RoundUp32(param->length)/sizeof(U32)]);
        // Check if total length is not exceeded
        // GCC allows pointer arithmetics on void*
        if (((PTR)pNext - (PTR)msg) < msg->length + sizeof(MSGHEADER))
        {
            // Check if parameter valid
            if (pNext->paramID != 0)
            {
                return pNext;
            }
        }
    }
    return NULL;
}

/*! \brief Get parameter by its ID
 *
 *  \param msg Pointer to allocated mesage
 *  \param paramID Parameter ID to seek
 *  \return Pointer to parameter if found or NULL if not
*/
PPARAMHEADER MsgGetParam(PMSGHEADER msg, U16 paramID)
{
    PPARAMHEADER par = MsgGetNextParam(msg, NULL);

    while (par)
    {
        if (par->paramID == paramID)
        {
            return par;
        }
        par = MsgGetNextParam(msg, par);
    }
    return NULL;
}

/*! \brief Initialize newly allocated message
 *
 *  \param Msg Pointer to allocated mesage
 *  \return Standard return code
*/
void MsgInitMessage(PMSGHEADER pHdr)
{
    pHdr->control = CTRL_VERSION;
    // Zero length
    pHdr->length = 0;
    // Invalid type
    pHdr->type = 0;
    // Invalid instance
    pHdr->srcID = 0;
    pHdr->dstID = 0;
    // Invalid message ID
    pHdr->msgID = 0;
}

/*! \brief Append/update result of operation to the message
 *
 *  \param Msg Pointer to allocated mesage
 *  \param Result Result code
 *  \return Standard return code
*/
RESULTCODE MsgResult(PMSGHEADER msg, U32 result)
{
    PPARAMHEADER pParam = MsgGetNextParam(msg, NULL);

    // This block will be skipped if no parameters exist yet
    while (pParam)
    {
        if (pParam->paramID == PAR_RESULT)
        {
            // Result already exist, update with new one
            pParam->data[0] = result;
            return SUCCESS;
        }
        pParam = MsgGetNextParam(msg, pParam);
    }

    // Fall here if not found, append to the end
    return MsgAppendU32(msg, PAR_RESULT, result);
}

/*! \brief Initialize newly allocated message with fields from source message
 *
 *  \param Msg Pointer to allocated mesage
 *  \return Standard return code
*/
PMSGHEADER MsgInitReply(PMSGHEADER pSrc)
{
    PMSGHEADER pAns;

    if (OsalAllocMemArray(&MsgArray, (OSAL_PADDRESS *) &pAns) == SUCCESS)
    {
        pAns->control = CTRL_VERSION;
        // Zero length
        pAns->length = 0;
        // Type depends on source type
        if (pSrc->type == MSGT_CONFIG)
        {
            pAns->type = MSGT_ANSWER;
        }
        else if (pSrc->type == MSGT_QUERY)
        {
            pAns->type = MSGT_RESPONSE;
        }
        else
        {
            pAns->type = MSGT_INDICATION;
        }
        pAns->srcID = pSrc->dstID;
        pAns->dstID = pSrc->srcID;
        pAns->msgID = pSrc->msgID;
    }
    else
    {
        pAns = NULL;
    }
    return pAns;
}

/** Get header size
 *
 * @return Header size
 */
U32 MsgGetHdrSize(void)
{
    return sizeof(MSGHEADER);
}

/** Get data offset from the header
 *
 *  Useful for protocol encapsulation / wrapping
 *
 * @param msg Pointer to the beginning of the message
 * @return Pointer to the data part
 */
PTR MsgGetDataOffset(PTR msg)
{
    PMSGHEADER pHdr = (PMSGHEADER) msg;
    return &pHdr->param[0];
}

PTR MsgGetDataOffsetList(PTR msg)
{
    return (PTR)msg;
}


/** Register dynamic message dispatcher
 *
 * @param msgDest In range [MSGS_LAST+1 .. MSGS_LAST+MSGS_DYNAMIC]
 * @param msgCB Pointer to the callback function
 * @return Standard result code
 */
RESULTCODE MsgRegDispatcher(U16 msgDest, MSGDISPATCHER msgCB)
{
    if (msgDest <= MSGS_LAST || msgDest > MSGS_LAST + MSGS_DYNAMIC)
    {
        // Out of dynamic range
        return FAILURE;
    }
    if (MsgDispatchers[msgDest-MSGS_LAST-1] != NULL)
    {
        // Already registered
        return FAILURE;
    }

    MsgDispatchers[msgDest-MSGS_LAST-1] = msgCB;

    return SUCCESS;
}

/** Deregister dynamic message dispatcher
 *
 * @param msgDest In range [MSGS_LAST+1 .. MSGS_LAST+MSGS_DYNAMIC]
 * @return Standard result code
 */
RESULTCODE MsgDeregDispatcher(U16 msgDest)
{
    if (msgDest <= MSGS_LAST || msgDest > MSGS_LAST + MSGS_DYNAMIC)
    {
        // Out of dynamic range
        return FAILURE;
    }
    if (MsgDispatchers[msgDest-MSGS_LAST-1] == NULL)
    {
        // Already deregistered
        return FAILURE;
    }

    MsgDispatchers[msgDest-MSGS_LAST-1] = NULL;

    return SUCCESS;
}

/** Return next available instance ID
 *
 * @return Non-zero on success, zero on failure
 */
U16 MsgGetAvailableInstance(void)
{
    int i;

    for (i = 0; i < MSGS_DYNAMIC; i++)
    {
        if (MsgDispatchers[i] == NULL)
        {
            return i + MSGS_LAST + 1;
        }
    }
    return 0;
}

/** Simple result-like reply to the incoming message
 *
 * @param srcMsg
 * @param resultCode
 * @return
 */
RESULTCODE MsgSimpleReply(PMSGHEADER srcMsg, U32 resultCode)
{
    PMSGHEADER pHdr;

    if ((pHdr = MsgInitReply(srcMsg)) != NULL)
    {
        MsgResult(pHdr, resultCode);
        return MsgDispatch(pHdr->srcID, pHdr->length + sizeof(MSGHEADER), pHdr);
    }
    return FAILURE;
}

/** Fast way to create multi-parameter reply
 *
 *  All parameters must be passed in triads (except binaries):
 *    - int Parameter ID
 *    - int Parameter Type
 *    - int Length (for binary only, strings will be calculated)
 *    - U32/U16/U8 Parameter Value
 *
 * @param srcMsg Pointer to source message to reply to
 * @param resultCode Result of operation
 * @param paramCount Number of optional parameters
 * @return Standard result code
 */
RESULTCODE MsgReply(PMSGHEADER srcMsg, U32 resultCode, U32 paramCount, ...)
{
    int paramType, val;
    U16 paramID, len;
    char *strVal;
    PTR binVal;
    va_list argptr;
    PMSGHEADER pHdr;

    pHdr = MsgInitReply(srcMsg);
    if (pHdr == NULL)
    {
        return FAILURE;
    }
    va_start(argptr, paramCount);
    while (paramCount--)
    {
        paramID = (U16)va_arg(argptr, int);
        paramType = va_arg(argptr, int);

        switch (paramType)
        {
        case TYPE_U8:
            val = va_arg(argptr, int);
            MsgAppendU8(pHdr, paramID, (U8)val);
            break;
        case TYPE_U16:
            val = va_arg(argptr, int);
            MsgAppendU16(pHdr, paramID, (U16)val);
            break;
        case TYPE_U32:
            val = va_arg(argptr, int);
            MsgAppendU32(pHdr, paramID, (U32)val);
            break;
        case TYPE_STRING:
            strVal = va_arg(argptr, char *);
            len = (U16) (strlen(strVal) + 1);
            MsgAppendParam(pHdr, paramID, len, strVal);
            break;
        case TYPE_BINARY:
            len = (U16) (va_arg(argptr, int) + 1);
            binVal = va_arg(argptr, PTR);
            MsgAppendParam(pHdr, paramID, len, binVal);
            break;
        default:
            MsgFreeBuffer(pHdr);
            return FAILURE;
        }
    }
    va_end(argptr);
    return MsgDispatch(MSGS_SVSR, sizeof(MSGHEADER) + pHdr->length, pHdr);
}

/** Fast way to create multi-parameter indication
 *
 *  All parameters must be passed in triads (except binaries):
 *    - int Parameter ID
 *    - int Parameter Type
 *    - int Length (for binary only, strings will be calculated)
 *    - U32/U16/U8 Parameter Value
 *
 * @param srcID Instance ID of message source
 * @param dstID Instance ID of message destination
 * @param msgID Indication message code
 * @param paramCount Number of optional parameters
 * @return Standard result code
 */
RESULTCODE MsgIndication(U16 srcID, U16 dstID, U16 msgID, U32 paramCount, ...)
{
    int paramType, val;
    U16 paramID, len;
    char *strVal;
    PTR binVal;
    va_list argptr;
    PMSGHEADER pHdr;

    pHdr = MsgAllocBuffer(MSG_COMMON);
    if (pHdr == NULL)
    {
        return FAILURE;
    }
    va_start(argptr, paramCount);

    pHdr->control = CTRL_DEFAULT;
    pHdr->type = MSGT_INDICATION;
    pHdr->length = 0;
    pHdr->srcID = srcID;
    pHdr->dstID = dstID;
    pHdr->msgID = msgID;

    while (paramCount--)
    {
        paramID = (U16)va_arg(argptr, int);
        paramType = va_arg(argptr, int);

        switch (paramType)
        {
        case TYPE_U8:
            val = va_arg(argptr, int);
            MsgAppendU8(pHdr, paramID, (U8)val);
            break;
        case TYPE_U16:
            val = va_arg(argptr, int);
            MsgAppendU16(pHdr, paramID, (U16)val);
            break;
        case TYPE_U32:
            val = va_arg(argptr, int);
            MsgAppendU32(pHdr, paramID, (U32)val);
            break;
        case TYPE_STRING:
            strVal = va_arg(argptr, char *);
            len = (U16) (strlen(strVal) + 1);
            MsgAppendParam(pHdr, paramID, len, strVal);
            break;
        case TYPE_BINARY:
            len = (U16) (va_arg(argptr, int) + 1);
            binVal = va_arg(argptr, PTR);
            MsgAppendParam(pHdr, paramID, len, binVal);
            break;
        default:
            MsgFreeBuffer(pHdr);
            return FAILURE;
        }
    }

    va_end(argptr);

    return MsgDispatch(srcID, sizeof(MSGHEADER) + pHdr->length, pHdr);
}

/** Fast way to create multi-parameter command
 *
 *  All parameters must be passed in triads (except binaries):
 *    - int Parameter ID
 *    - int Parameter Type
 *    - int Length (for binary only, strings will be calculated)
 *    - U32/U16/U8 Parameter Value
 *
 * @param srcID Instance ID of message source
 * @param dstID Instance ID of message destination
 * @param msgID Indication message code
 * @param paramCount Number of optional parameters
 * @return Standard result code
 */
RESULTCODE MsgCommand(U16 srcID, U16 dstID, U16 msgID, U32 paramCount, ...)
{
    int paramType, val;
    U16 paramID, len;
    char *strVal;
    PTR binVal;
    va_list argptr;
    PMSGHEADER pHdr;

    pHdr = MsgAllocBuffer(MSG_COMMON);
    if (pHdr == NULL)
    {
        return FAILURE;
    }
    va_start(argptr, paramCount);

    pHdr->control = CTRL_DEFAULT;
    pHdr->type = MSGT_CONFIG;
    pHdr->length = 0;
    pHdr->srcID = srcID;
    pHdr->dstID = dstID;
    pHdr->msgID = msgID;

    while (paramCount--)
    {
        paramID = (U16)va_arg(argptr, int);
        paramType = va_arg(argptr, int);

        switch (paramType)
        {
        case TYPE_U8:
            val = va_arg(argptr, int);
            MsgAppendU8(pHdr, paramID, (U8)val);
            break;
        case TYPE_U16:
            val = va_arg(argptr, int);
            MsgAppendU16(pHdr, paramID, (U16)val);
            break;
        case TYPE_U32:
            val = va_arg(argptr, int);
            MsgAppendU32(pHdr, paramID, (U32)val);
            break;
        case TYPE_STRING:
            strVal = va_arg(argptr, char *);
            len = (U16) (strlen(strVal) + 1);
            MsgAppendParam(pHdr, paramID, len, strVal);
            break;
        case TYPE_BINARY:
            len = (U16) (va_arg(argptr, int) + 1);
            binVal = va_arg(argptr, PTR);
            MsgAppendParam(pHdr, paramID, len, binVal);
            break;
        default:
            MsgFreeBuffer(pHdr);
            return FAILURE;
        }
    }

    va_end(argptr);

    return MsgDispatch(srcID, sizeof(MSGHEADER) + pHdr->length, pHdr);
}

/** Allocate memory block for message storing
 *
 * @param flags - doesn't have metter
 * @return Pointer to a newly allocated message
 */
EXPORT PMSGHEADER Msg2AllocBuffer(int flags)
{
    if (flags == MSG_FAST)
    {
        return (PMSGHEADER) Icpu2AllocBlock();
    }
    else
    {
        OSAL_PADDRESS pBlock;
        
        if (OsalAllocMemArray(&MsgArray, &pBlock) != SUCCESS)
        {
            return NULL;
        }
        return (PMSGHEADER) pBlock;
    } 
}

/** Allocate memory block for message storing
 *
 * @param flags MSG_COMMON - allocate in common storage or MSG_FAST - allocate in fast I-CPU storage
 * @return Pointer to a newly allocated message
 */
EXPORT PMSGHEADER Msg1AllocBuffer(int flags)
{
    OSAL_PADDRESS pBlock;


    if ((flags & MSG_FAST) && (IcpuMode == IMODE_DIRECT))
    {
            if ((pBlock = IcpuAllocBlock()) == NULL)
        {
            // Try to allocate common memory
            if (OsalAllocMemArray(&MsgArray, &pBlock) != SUCCESS)
            {
                return NULL;
            }
        }
    }
    else
        if (OsalAllocMemArray(&MsgArray, &pBlock) != SUCCESS)
        {
            return NULL;
        }
    return (PMSGHEADER) pBlock;
}


EXPORT void Msg2FreeBuffer(PMSGHEADER pMsg)
{
    if (Icpu2FreeBlock((void *) pMsg) != SUCCESS)
        OsalFreeMemArray(&MsgArray, (OSAL_PADDRESS)pMsg);
}

/**
 *
 * @param pMsg
 */
EXPORT void Msg1FreeBuffer(PMSGHEADER pMsg)
{
    if (IcpuMode == IMODE_DIRECT)
    {
        if (IcpuFreeBlock(pMsg) != SUCCESS)
        {
            OsalFreeMemArray(&MsgArray, pMsg);
        }
    }
    else
    {
        // NULL check will be performed in OsalFreeMemArray
        OsalFreeMemArray(&MsgArray, (OSAL_PADDRESS) pMsg);
    }
}
