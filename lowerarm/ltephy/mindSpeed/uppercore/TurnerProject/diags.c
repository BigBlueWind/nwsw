/** @file diags.c
 *
 * @brief Diagnostic message handling
 * @author Mindspeed Technologies
 * @version $Revision: 1.19 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#include "diags.h"
#include "osal.h"
#include "apidefs.h"
#include "msgroute.h"
#if defined (ENODEB)
#include "LtePhyL2Api.h"
#endif
#include "settings.h"
#include <math.h>

//
// Global debug level
//
S32 DiagDebugLevel = DIAG_LEVEL0 | DIAG_LEVEL1 | DIAG_LEVEL2;

#ifndef ROUTER

//
// Loopback test
//
U16 DiagLoopbackDestID = 0;
U16 DiagLoopbackSrcID = 0;
int DiagLoopbackSize = 0;
int DiagLoopbackCount = 0;
PTR DiagLoopbackBuffer = NULL;
OSAL_DELAYM DiagLoopbackStartTime;

//
// Forward declarations
//
void DiagTask(PTR data);

RESULTCODE DiagCreateTask(PTR msg)
{
    PMSGHEADER pHdr = (PMSGHEADER) msg;
    PPARAMHEADER pParam = MsgGetNextParam(pHdr, NULL);
    RESULTCODE ret = SUCCESS;

    //
    // Message parameters
    //
    OSAL_TASK task = 0;
    U32 taskParam = 0;
    char *taskName = "DiagTask";
    OSAL_PRIORITY taskPriority = OSAL_PRIORITY_NORMAL;
    OSAL_PADDRESS pStackBase = NULL;
    OSAL_SIZE stackSize = 0;

    while (pParam)
    {
        switch (pParam->paramID)
        {
            case 0:                 // End of list
                pParam = NULL;      // Stop
                continue;
            case PAR_TASKPARAM:
                taskParam = pParam->data[0];
                break;
            case PAR_TASKNAME:
                // Stored in message
                taskName = (char *) &pParam->data[0];
                break;
            case PAR_TASKPRIORITY:
                taskPriority = pParam->data[0];
                break;
            case PAR_STACKBASE:
                // TODO: Check case when sizeof(void*) != sizeof(int)
                pStackBase = (OSAL_PADDRESS) pParam->data[0];
                break;
            case PAR_STACKSIZE:
                stackSize = (OSAL_SIZE) pParam->data[0];
                break;
            default:                // Unknown parameter received
                pParam = NULL;      // Stop
                continue;
        }
        pParam = MsgGetNextParam(pHdr, pParam);
    }

    if (ret == SUCCESS)
    {
        if (OsalCreateTask(&task, DiagTask, &taskParam, taskName, taskPriority,
                       pStackBase, stackSize) == SUCCESS)
        {
            // TODO: return task id
        }
        else
        {
            // TODO: return failure
        }
    }
    return ret;
}

RESULTCODE DiagDestroyTask(PTR msg)
{
    return SUCCESS;
}

RESULTCODE DiagCreateSema(PTR msg)
{
    return SUCCESS;
}

RESULTCODE DiagDestroySema(PTR msg)
{
    return SUCCESS;
}

RESULTCODE DiagCreateMutex(PTR msg)
{
    return SUCCESS;
}

RESULTCODE DiagDestroyMutex(PTR msg)
{
    return SUCCESS;
}

/** Echoes a message back to sender
 *
 * @param msg Pointer to the inbound message
 * @return Standard result code
 */
RESULTCODE DiagEcho(PTR msg)
{
    PMSGHEADER pAns;
    PMSGHEADER pHdr = (PMSGHEADER) msg;
    PPARAMHEADER pPar = MsgGetNextParam(msg, NULL);

    if (pPar == NULL)
    {
        return FAILURE;
    }
    if (pPar->paramID != PAR_ECHO)
    {
        return FAILURE;
    }

    if ((pAns = MsgInitReply(pHdr)) != NULL)
    {
        MsgResult(pAns, SUCCESS);
        MsgAppendParam(pAns, pPar->paramID, pPar->length, pPar->data);
        MsgDispatch(MSGS_SVSR, pAns->length + sizeof(MSGHEADER), pAns);
    }
    return SUCCESS;
}

/** This task for debugging purposes
 *
 * @param data Passed from Host
 */
void DiagTask(PTR data)
{
}

/** Message sink for loopback tests
 *
 * @param size Size of the inbound loopback message
 * @param msg Pointer to the inbound loopback message
 * @return Standard result code
 */
RESULTCODE DiagLoopbackHandler(U32 size, PTR msg)
{
    OSAL_DELAYM stopTime;

    MsgFreeBuffer(msg);
    if (--DiagLoopbackCount == 0)
    {
        OsalGetTimeMs(&stopTime);
        DiagPrint(DIAG_LEVELALL, NULL, "Done loopback test in %i ms\n",
                  stopTime - DiagLoopbackStartTime);
        MsgDeregDispatcher(DiagLoopbackSrcID);
        MsgFreeBuffer(DiagLoopbackBuffer);
        DiagLoopbackBuffer = NULL;
    }
    else
    {
        MsgIndication(DiagLoopbackSrcID, DiagLoopbackDestID, DIAG_LOOPBACK, 1, 4, TYPE_BINARY,
                      DiagLoopbackSize, DiagLoopbackBuffer);
    }
    return SUCCESS;
}

/** Run loopback diagnostics test against specified module
 *
 * @param destID Destination of loopback packets
 * @param count Number of loops
 * @param size Packet size
 * @return Standard result code
 */
RESULTCODE DiagLoopback(U16 destID, int count, int size)
{
    DiagLoopbackBuffer = MsgAllocBuffer(MSG_COMMON);
    if (DiagLoopbackBuffer == NULL)
        return FAILURE;
    // Get dynamic source / destination ID
    DiagLoopbackSrcID = MsgGetAvailableInstance();
    if (DiagLoopbackSrcID == 0)
        return FAILURE;
    DiagLoopbackDestID = destID;
    if (count <= 0)
        return FAILURE;
    DiagLoopbackCount = count;
    if (size >= MSG_MAXSIZE - sizeof(MSGHEADER) - sizeof(PARAMHEADER))
        return FAILURE;
    DiagLoopbackSize = size;
    // Register dynamic message sink
    MsgRegDispatcher(DiagLoopbackSrcID, DiagLoopbackHandler);
    DiagPrint(DIAG_LEVELALL, NULL, "Starting loopback test [SrcID=%04X,DstID=%04X,Size=%i,Count=%i]\n",
           DiagLoopbackSrcID, DiagLoopbackDestID, DiagLoopbackSize, DiagLoopbackCount);
    OsalGetTimeMs(&DiagLoopbackStartTime);
    // Don't care about param ID
    MsgIndication(DiagLoopbackSrcID, DiagLoopbackDestID, DIAG_LOOPBACK, 1, PAR_LPB_DATA, TYPE_BINARY,
                  DiagLoopbackSize, DiagLoopbackBuffer);

    return SUCCESS;
}

#endif /* !ROUTER */

#ifndef NDEBUG
/** Diagnostics printing to console or other output device
 *
 * @param level Bitmask specifying current debug level
 * @param key Message header
 * @param fmt printf-like format specification
 * @return Number of characters printed
 */
int DiagPrint(S32 level, const char *key, const char *fmt, ...)
{
    int ret;
    va_list arg;

    if ((level & DiagDebugLevel) == 0)
    {
        return 0;
    }
    va_start(arg, fmt);
    // TODO: Atomic print
    if (key != NULL && *key != '\0')
        printf("%s:", key);
    ret = vprintf(fmt, arg);
    va_end(arg);
    // Print immediately
    fflush(stdout);
    return ret;
}

/** Decode and print MSPD API message
 *
 * @param level Bitmask specifying current debug level
 * @param key Message header
 * @param flags Controls output format
 *              DIAG_PAYLOAD     print message's payload after header
 *              DIAG_FULLPAYLOAD print all bytes, otherwise first 16
 *              DIAG_DECODETLV   decode parameters
 * @param msg Pointer to the message's header
 * @return Standard result code
 */
RESULTCODE DiagPrintApi(S32 level, const char *key, int flags, PTR msg)
{
    PMSGHEADER pHdr = (PMSGHEADER) msg;
    unsigned char *ptr;
    unsigned int i;

    if ((level & DiagDebugLevel) == 0)
        return FALSE;

    printf("%s:C=%08X,T=%04X,S=%04X,D=%04X,M=%04X,L=%i\n", key,
           pHdr->control, pHdr->type, pHdr->srcID, pHdr->dstID, pHdr->msgID, pHdr->length);
    if (flags & (DIAG_PAYLOAD | DIAG_FULLPAYLOAD | DIAG_DECODETLV))
    {
        if (flags & DIAG_DECODETLV)
        {
            if (pHdr->control & CTRL_BYPASS)
            {
                // ? Call DiagDumpPhyApi or not ?
            }
            else
            {
                PPARAMHEADER pParam = MsgGetNextParam(pHdr, NULL);
                while (pParam)
                {
                    printf("  [ID=%04X,Len=%04X,{", pParam->paramID, pParam->length);
                    if (pParam->length <= sizeof(U32))
                        printf("%08X}]\n", pParam->data[0]);
                    else
                    {
                        ptr = (unsigned char *) &pParam->data[0];
                        // Print row of 16 bytes
                        for (i = 0; i < 16; i++)
                        {
                            if (i >= pParam->length)
                                break;
                            printf("%02X ", *ptr++);
                        }
                        printf("}]\n");
                    }
                    pParam = MsgGetNextParam(pHdr, pParam);
                }
            }
        }
        else
        {
            ptr = (unsigned char *) MsgGetDataOffset(pHdr);
            // Print row of 16 bytes
            for (i = 0; i < 16; i++)
            {
                if ((flags & DIAG_FULLPAYLOAD) == 0)
                {
                    if (i >= pHdr->length)
                        break;
                }
                printf("%02X ", *ptr++);
            }
            printf("\n");
        }
    }
    return SUCCESS;
}
#endif // !NDEBUG

/** Dump binary object to a file on a file system or other device
 *
 * @param level Bitmask specifying current debug level
 * @param flags Controls output format
 *              DIAG_DUMPFILE create file on a file system
 *              DIAG_DUMPHEX print hexidecimal strings
 * @param size Object's size
 * @param data Pointer to the object
 * @return Standard result code
 */
RESULTCODE DiagDump(S32 level, U32 flags, U32 size, PTR data)
{
    if ((level & DiagDebugLevel) == 0)
        return FAILURE;

    if (flags & DIAG_DUMPFILE)
    {
        FILE *file;
        char filename[256];
        static int v = 0;

        sprintf(filename, "c4kdump-%04X.dmp", v++);
        file = fopen(filename, "wb");
        if (file)
        {
            fwrite(data, size, 1, file);
            fclose(file);
            return SUCCESS;
        }
        else
            return FAILURE;
    }
    // TODO: Combine with others
    if (flags & DIAG_DUMPHEX)
    {
        unsigned char *pDat = (unsigned char *) data;
        unsigned int i, j;

        for (i = 0; i < size;)
        {
            for (j = 0; j < 16; j++)
            {
                printf("%02X ", pDat[j]);
                if (i + j + 1 >= size)
                    break;
            }
            for (j = 0; j < 16; j++)
            {
                if (isprint(pDat[j]))
                    printf("%c", pDat[j]);
                else
                    printf(".");
                if (i + j + 1 >= size)
                    break;
            }
            printf("\n");
            i += 16;
            pDat += 16;

            // FIXME: This is debug check and to be removed
            if (i > 128)
                break;
        }
    }
    return FAILURE;
}

/** Store I/Q samples to a file using various formats
 *
 * @param sf System Frame Number
 * @param sfn Subframe Number
 * @param format Output format
 *               IQ_FMT_WJ  Text file compatible with WaveJudge
 *               IQ_FMT_VSA Text file compatible with VSA
 *               IQ_FMT_BIN Binary file
 * @param size Data size
 * @param data Pointer to the data
 * @return Standard result code
 */
RESULTCODE DiagDumpIqSamples(U32 sfn, U32 sf, U32 format, U32 size, PTR data, ...)
{
    FILE *file;
    char filename[16];
    char modeFlag[] = {'w', '\0', '\0', '\0'};
    va_list argptr;

    if (format & IQ_FMT_APPEND)
        modeFlag[0] = 'a';

    sprintf(filename, "iq_%04i_%1i.txt", sfn, sf);
    if ((file = fopen(filename, modeFlag)) == NULL)
        return FAILURE;

    va_start(argptr, data);

    switch (format)
    {
    case IQ_FMT_WJ:
        {
            S16 *ptr = (S16 *) data;
            // Size specifies number of I/Q pairs, e.g. 15360 for 10 MHz 1 antenna
            while (size--)
            {
                fprintf(file, "\t%i\t%i\n", *ptr, *(ptr+1));
                ptr += 2;
            }
        }
        break;
    case IQ_FMT_VSA:
        {
            S16 *ptr = (S16 *) data;
            int fftSize = va_arg(argptr, int);
            double arg1 = pow(2.0, 11.0);
            double arg2 = sqrt(fftSize);

            while (size--)
            {
                fprintf(file, "%3.9f %3.9f\n", *(ptr) / arg1 * arg2, *(ptr+1) / arg1 * arg2);
                ptr += 2;
            }
        }
        break;
    case IQ_FMT_BIN:
        fwrite(data, size, 1, file);
        break;
    default:
        return FAILURE;
    }
    va_end(argptr);
    fclose(file);
    return SUCCESS;
}

/** Print LTE PHY API message
 *
 * @param level Bitmask specifying current debug level
 * @param vector Pointer to the PHY API message
 * @return Standard result code
 */
RESULTCODE DiagDumpPhyApi(S32 level, PTR vector)
{
#if defined (ENODEB)
    PGENMSGDESC pGen = (PGENMSGDESC) vector;
    PDLSUBFRDESC pDlSf;
    PULSUBFRDESC pUlSf;
    PTXSDUREQ pTxSduReq;
    PRXSDUIND pRxSduInd;
    DLCHANDESC *pDlCh;
    ULCHANDESC *pUlCh;
    DLSUBCHINFO *pSubCh;
    ULSUBCHINFO *pUlSubCh;
    CRCINFO *pCrc;
    SCRAMBLERINFO *pScram;
    DLDEDICATEDPRECODERINFO *pPrecoder;
    MAPPINGINFO *pMapping;
    DCICHANNELDESC *pDciCh;
    DLCMNTXPWRCTL *pPwrCtl;
    PULSUBFRCMNCTRL pUlCmnCtrl;
    ULCTRLCHDESC *pUlCtrlCh;
    SRSDED *pSrs;
    RACHCTRL *pRachCtrl;
    PINITPARM pInit;
    PMSGIND pMsgInd;
    unsigned int i, j, k;
    unsigned char *pDat;

    if ((level & DiagDebugLevel) == 0)
        return FAILURE;

    printf("pGen->msgSpecific=%i\n", pGen->msgSpecific);
    printf("pGen->msgType=%i\n", pGen->msgType);
    printf("pGen->phyEntityId=%i\n", pGen->phyEntityId);

    switch (pGen->msgType)
    {
    case PHY_TXSTART_REQ:
        pDlSf = (PDLSUBFRDESC) (pGen + 1);

        printf("  pDlSf->frameNumber=%i\n", pDlSf->frameNumber);
        printf("  pDlSf->subframeNumber=%i\n", pDlSf->subframeNumber);
        printf("  pDlSf->subframeType=%i\n", pDlSf->subframeType);
        printf("  pDlSf->antennaPortcount=%i\n", pDlSf->antennaPortcount);
        printf("  pDlSf->numberofChannelDescriptors=%i\n", pDlSf->numberofChannelDescriptors);
        printf("  pDlSf->offsetPowerCtrl=%i\n", pDlSf->offsetPowerCtrl);
        printf("  pDlSf->offsetDCIChannels=%i\n", pDlSf->offsetDCIChannels);
        printf("  pDlSf->offsetCustomFeatures=%i\n", pDlSf->offsetCustomFeatures);
        printf("  pDlSf->numCtrlSymbols=%i\n", pDlSf->numCtrlSymbols);
        printf("  pDlSf->phichResource=%i\n", pDlSf->phichResource);
        printf("  pDlSf->phichDuration=%i\n", pDlSf->phichDuration);
        printf("  pDlSf->numberOfPhichChannels=%i\n", pDlSf->numberOfPhichChannels);
        printf("  pDlSf->numCtrlChannels=%i\n", pDlSf->numCtrlChannels);

        pDlCh = &pDlSf->dlCh[0];
        for (i = 0; i < pDlSf->numberofChannelDescriptors - pDlSf->numCtrlChannels; i++)
        {
            printf("CHANNEL %i:\n----------\n", i);
            printf("    pDlCh->offsetNextCh=%i\n", pDlCh->offsetNextCh);
            printf("    pDlCh->channelId=%i\n", pDlCh->channelId);
            printf("    pDlCh->txpowerControl=%i\n", pDlCh->txpowerControl);
            printf("    pDlCh->persistEnable=%i\n", pDlCh->persistEnable);
            printf("    pDlCh->repeatCycle=%i\n", pDlCh->repeatCycle);
            printf("    pDlCh->channelType=%i\n", pDlCh->channelType);
            printf("    pDlCh->hCid=%i\n", pDlCh->hCid);
            printf("    pDlCh->numCodeWords=%i\n", pDlCh->numCodeWords);
            printf("    pDlCh->nLayers=%i\n", pDlCh->nLayers);
            printf("    pDlCh->transmissionMode=%i\n", pDlCh->transmissionMode);

            pSubCh = &pDlCh->subChInfo[0];
            for (j = 0; j < pDlCh->numCodeWords; j++)
            {
                printf("SUBCHANNEL %i:\n----------\n", j);
                printf("      pSubCh->codingDescriptor=%i\n", pSubCh->codingDescriptor);
                printf("      pSubCh->blockCodeConcatenation=%i\n", pSubCh->blockCodeConcatenation);
                printf("      pSubCh->modulationType=%i\n", pSubCh->modulationType);
                printf("      pSubCh->mcsType=%i\n", pSubCh->mcsType);
                printf("      pSubCh->nDi=%i\n", pSubCh->nDi);
                printf("      pSubCh->rV=%i\n", pSubCh->rV);
                printf("      pSubCh->flushReq=%i\n", pSubCh->flushReq);

                pSubCh++;
            }

            pCrc = &pDlCh->crcInfo;
            printf("    pCrc->crcLength=%i\n", pCrc->crcLength);
            printf("    pCrc->crcScrambling=%i\n", pCrc->crcScrambling);

            pScram = &pDlCh->scrInfo;
            printf("    pScram->scramblerType=%i\n", pScram->scramblerType);
            printf("    pScram->scrinitValueinput=%i\n", pScram->scrinitValueinput);

            pPrecoder = &pDlCh->dlPrecoderInfo;
            printf("    pPrecoder->cddType=%i\n", pPrecoder->cddType);
            printf("    pPrecoder->codeBookIdx=%i\n", pPrecoder->codeBookIdx);
            printf("    pPrecoder->codebookSubsetRestriction[]=[%i %i %i %i %i %i %i %i]\n",
                   pPrecoder->codebookSubsetRestriction[0],
                   pPrecoder->codebookSubsetRestriction[1],
                   pPrecoder->codebookSubsetRestriction[2],
                   pPrecoder->codebookSubsetRestriction[3],
                   pPrecoder->codebookSubsetRestriction[4],
                   pPrecoder->codebookSubsetRestriction[5],
                   pPrecoder->codebookSubsetRestriction[6],
                   pPrecoder->codebookSubsetRestriction[7]);

            pMapping = &pDlCh->mapInfo;
            printf("    pMapping->numberofEntries=%i\n", pMapping->numberofEntries);
            for (k = 0; k < pMapping->numberofEntries; k++)
            {
                printf("        pMapping->reselmInfo=[%i,%i]\n",
                       pMapping->reselmInfo[k].startRes,
                       pMapping->reselmInfo[k].numRes);
            }

            pDlCh++;
        }

        pDciCh = (DCICHANNELDESC *) ((uintptr_t) pDlSf + (uintptr_t) pDlSf->offsetDCIChannels);
        for (i = 0; i < pDlSf->numCtrlChannels; i++)
        {
            printf("CTRL CHANNEL %i:\n---------------\n", i);
            printf("    pDciCh->offsetNextCh=%i\n", pDciCh->offsetNextCh);
            printf("    pDciCh->channelId=%i\n", pDciCh->channelId);
            printf("    pDciCh->txpowerControl=%i\n", pDciCh->txpowerControl);
            printf("    pDciCh->crcLength=%i\n", pDciCh->crcLength);
            printf("    pDciCh->crcScrambling=%i\n", pDciCh->crcScrambling);
            printf("    pDciCh->channelType=%i\n", pDciCh->channelType);
            printf("    pDciCh->numCodeWords=%i\n", pDciCh->numCodeWords);
            printf("    pDciCh->nLayers=%i\n", pDciCh->nLayers);
            printf("    pDciCh->transmissionMode=%i\n", pDciCh->transmissionMode);
            printf("    pDciCh->scrmblerInitValue=%i\n", pDciCh->scrmblerInitValue);
            printf("    pDciCh->numberofEntries=%i\n", pDciCh->numberofEntries);
            printf("    pDciCh->startRes=%i\n", pDciCh->startRes);
            printf("    pDciCh->numRes=%i\n", pDciCh->numRes);

            pDciCh++;
        }
        if (pDlSf->offsetPowerCtrl != 0)
        {
            pPwrCtl = (DLCMNTXPWRCTL*) ((uintptr_t)pDlSf + (uintptr_t)pDlSf->offsetPowerCtrl);
            printf("  pPwrCtl->pilotPower=%i\n", pPwrCtl->pilotPower);
            printf("  pPwrCtl->psyncPower=%i\n", pPwrCtl->psyncPower);
            printf("  pPwrCtl->ssyncPower=%i\n", pPwrCtl->ssyncPower);
            printf("  pPwrCtl->ciPower=%i\n", pPwrCtl->ciPower);
            printf("  pPwrCtl->paprControl=%i\n", pPwrCtl->paprControl);
            printf("  pPwrCtl->paprThreshold=%i\n", pPwrCtl->paprThreshold);
        }
        break;

    case PHY_TXSDU_REQ:
        pTxSduReq = (PTXSDUREQ) pGen;
        printf("  pTxSdu->nackAck=%i\n", pTxSduReq->nackAck);
        printf("  pTxSdu->uciFormat=%i\n", pTxSduReq->uciFormat);
        printf("  pTxSdu->channelType=%i\n", pTxSduReq->channelType);
        printf("  pTxSdu->lulRbIdx=%i\n", pTxSduReq->lulRbIdx);
        printf("  pTxSdu->cwId=%i\n", pTxSduReq->cwId);
        printf("  pTxSdu->msgLen=%i\n", pTxSduReq->msgLen);
        printf("  pTxSdu->phichGrpNumber=%i\n", pTxSduReq->phichGrpNumber);
        printf("  pTxSdu->maxBitsperCw=%i\n", pTxSduReq->maxBitsperCw);

        pDat = (unsigned char *) (pTxSduReq + 1);
        for (i = 0; i < pTxSduReq->msgLen;)
        {
            for (j = 0; j < 16; j++)
            {
                printf("%02X ", pDat[j]);
                if (i + j + 1 >= pTxSduReq->msgLen)
                    break;
            }
            for (j = 0; j < 16; j++)
            {
                if (isprint(pDat[j]))
                    printf("%c", pDat[j]);
                else
                    printf(".");
                if (i + j + 1 >= pTxSduReq->msgLen)
                    break;
            }
            printf("\n");
            i += 16;
            pDat += 16;
            
            // FIXME: This is debug check and to be removed
            if (i > 64)
                break;
        }
        break;

    case PHY_RXSTART_REQ:
        pUlSf = (PULSUBFRDESC) (pGen + 1);

        printf("  pUlSf->frameNumber=%i\n", pUlSf->frameNumber);
        printf("  pUlSf->subframeNumber=%i\n", pUlSf->subframeNumber);
        printf("  pUlSf->subframeType=%i\n", pUlSf->subframeType);
        printf("  pUlSf->antennaPortcount=%i\n", pUlSf->antennaPortcount);
        printf("  pUlSf->numberofChannelDescriptors=%i\n", pUlSf->numberofChannelDescriptors);
        printf("  pUlSf->numberOfCtrlChannelDescriptors=%i\n", pUlSf->numberOfCtrlChannelDescriptors);
        printf("  pUlSf->numberSrsinSf=%i\n", pUlSf->numberSrsinSf);
        printf("  pUlSf->offsetRachCtrlStruct=%i\n", pUlSf->offsetRachCtrlStruct);
        printf("  pUlSf->offsetULCtrlChannels=%i\n", pUlSf->offsetULCtrlChannels);
        printf("  pUlSf->offsetsrsInfo=%i\n", pUlSf->offsetsrsInfo);
        printf("  pUlSf->offsetCustomFeatures=%i\n", pUlSf->offsetCustomFeatures);

        pUlCmnCtrl = &pUlSf->ulSfrCtrl;
        printf("    pUlCmnCtrl->deltaPUCCHShift=%i\n", pUlCmnCtrl->deltaPUCCHShift);
        printf("    pUlCmnCtrl->nRBCQI=%i\n", pUlCmnCtrl->nRBCQI);
        printf("    pUlCmnCtrl->nCSAn=%i\n", pUlCmnCtrl->nCSAn);
        printf("    pUlCmnCtrl->n1PucchAN=%i\n", pUlCmnCtrl->n1PucchAN);
        printf("    pUlCmnCtrl->srsBandwitdhConfig=%i\n", pUlCmnCtrl->srsBandwitdhConfig);
        printf("    pUlCmnCtrl->srsSubframeConfig=%i\n", pUlCmnCtrl->srsSubframeConfig);
        printf("    pUlCmnCtrl->ackNackSRSSimultaneousTransmission=%i\n", pUlCmnCtrl->ackNackSRSSimultaneousTransmission);
        printf("    pUlCmnCtrl->nSB=%i\n", pUlCmnCtrl->nSB);
        printf("    pUlCmnCtrl->hoppingMode=%i\n", pUlCmnCtrl->hoppingMode);
        printf("    pUlCmnCtrl->puschhoppingOffset=%i\n", pUlCmnCtrl->puschhoppingOffset);
        printf("    pUlCmnCtrl->enable64QAM=%i\n", pUlCmnCtrl->enable64QAM);
        printf("    pUlCmnCtrl->groupHoppingEnabled=%i\n", pUlCmnCtrl->groupHoppingEnabled);
        printf("    pUlCmnCtrl->groupAssignmentPUSCH=%i\n", pUlCmnCtrl->groupAssignmentPUSCH);
        printf("    pUlCmnCtrl->sequenceHoppingEnabled=%i\n", pUlCmnCtrl->sequenceHoppingEnabled);
        printf("    pUlCmnCtrl->cyclicShift=%i\n", pUlCmnCtrl->cyclicShift);

        pUlCh = &pUlSf->ulCh[0];
        for (i = 0; (int)i < (pUlSf->numberofChannelDescriptors - pUlSf->numberOfCtrlChannelDescriptors); i++)
        {
            printf("CHANNEL %i:\n----------\n", i);
            printf("    pUlCh->offsetNextCh=%i\n", pUlCh->offsetNextCh);
            printf("    pUlCh->channelId=%i\n", pUlCh->channelId);
            printf("    pUlCh->txpowerControl=%i\n", pUlCh->txpowerControl);
            printf("    pUlCh->persistEnable=%i\n", pUlCh->persistEnable);
            printf("    pUlCh->repeatCycle=%i\n", pUlCh->repeatCycle);
            printf("    pUlCh->channelType=%i\n", pUlCh->channelType);
            printf("    pUlCh->halfIterations=%i\n", pUlCh->halfIterations);

            pUlSubCh = &pUlCh->ulSubChInfo;
            printf("    pUlSubCh->mcinfo.codingDescriptor=%i\n", pUlSubCh->mcinfo.codingDescriptor);
            printf("    pUlSubCh->mcinfo.blockCodeConcatenation=%i\n", pUlSubCh->mcinfo.blockCodeConcatenation);
            printf("    pUlSubCh->mcinfo.modulationType=%i\n", pUlSubCh->mcinfo.modulationType);
            printf("    pUlSubCh->mcinfo.mcsType=%i\n", pUlSubCh->mcinfo.mcsType);
            printf("    pUlSubCh->harqInfo.nDi=%i\n", pUlSubCh->harqInfo.nDi);
            printf("    pUlSubCh->harqInfo.rV=%i\n", pUlSubCh->harqInfo.rV);
            printf("    pUlSubCh->harqInfo.flushReq=%i\n", pUlSubCh->harqInfo.flushReq);
            printf("    pUlSubCh->crcInfo.crcLength=%i\n", pUlSubCh->crcInfo.crcLength);
            printf("    pUlSubCh->crcInfo.crcScrambling=%i\n", pUlSubCh->crcInfo.crcScrambling);
            printf("    pUlSubCh->scrInfo.scramblerType=%i\n", pUlSubCh->scrInfo.scramblerType);
            printf("    pUlSubCh->scrInfo.scrinitValueinput=%i\n", pUlSubCh->scrInfo.scrinitValueinput);
            printf("    pUlSubCh->puschDed.betaOffsetACKIndex=%i\n", pUlSubCh->puschDed.betaOffsetACKIndex);
            printf("    pUlSubCh->puschDed.betaOffsetRIIndex=%i\n", pUlSubCh->puschDed.betaOffsetRIIndex);
            printf("    pUlSubCh->puschDed.betaOffsetCQIIndex=%i\n", pUlSubCh->puschDed.betaOffsetCQIIndex);
            printf("    pUlSubCh->cqiPmiRiRpt.cqiReportModeAperiodic=%i\n", pUlSubCh->cqiPmiRiRpt.cqiReportModeAperiodic);
            printf("    pUlSubCh->cqiPmiRiRpt.nomPDSCHRSEPREOffset=%i\n", pUlSubCh->cqiPmiRiRpt.nomPDSCHRSEPREOffset);
            printf("    pUlSubCh->cqiPmiRiRpt.cqiReportPeriodicEnable=%i\n", pUlSubCh->cqiPmiRiRpt.cqiReportPeriodicEnable);
            printf("    pUlSubCh->cqiPmiRiRpt.cqiPUCCHResourceIndex=%i\n", pUlSubCh->cqiPmiRiRpt.cqiPUCCHResourceIndex);
            printf("    pUlSubCh->cqiPmiRiRpt.cqipmiConfigIndex=%i\n", pUlSubCh->cqiPmiRiRpt.cqipmiConfigIndex);
            printf("    pUlSubCh->cqiPmiRiRpt.cqiFormatIndicatorPeriodic=%i\n", pUlSubCh->cqiPmiRiRpt.cqiFormatIndicatorPeriodic);
            printf("    pUlSubCh->cqiPmiRiRpt.K=%i\n", pUlSubCh->cqiPmiRiRpt.K);
            printf("    pUlSubCh->cqiPmiRiRpt.riConfigIndex=%i\n", pUlSubCh->cqiPmiRiRpt.riConfigIndex);
            printf("    pUlSubCh->cqiPmiRiRpt.simultaneousAckNackAndCQI=%i\n", pUlSubCh->cqiPmiRiRpt.simultaneousAckNackAndCQI);

            printf("    pUlSubCh->mapInfo.numberofEntries=%i\n", pUlSubCh->mapInfo.numberofEntries);
            for (j = 0; j < pUlSubCh->mapInfo.numberofEntries; j++)
            {
                printf("      pUlSubCh->mapInfo.reselmInfo[%i]=(%i,%i)\n", j,
                       pUlSubCh->mapInfo.reselmInfo[j].startRes,
                       pUlSubCh->mapInfo.reselmInfo[j].numRes);
            }
            pUlCh++;
        }
        pUlCtrlCh = &pUlSf->ulCtlCh[0];
        for (i = 0; i < pUlSf->numberOfCtrlChannelDescriptors; i++)
        {
            printf("CTRL CHANNEL %i:\n----------\n", i);
            printf("    pUlCtrlCh->offsetNextCh=%i\n", pUlCtrlCh->offsetNextCh);
            printf("    pUlCtrlCh->channelId=%i\n", pUlCtrlCh->channelId);
            printf("    pUlCtrlCh->txpowerControl=%i\n", pUlCtrlCh->txpowerControl);
            printf("    pUlCtrlCh->crcLength=%i\n", pUlCtrlCh->crcLength);
            printf("    pUlCtrlCh->channelType=%i\n", pUlCtrlCh->channelType);
            printf("    pUlCtrlCh->scrmblerInitValue=%i\n", pUlCtrlCh->scrmblerInitValue);
            printf("    pUlCtrlCh->codingDescriptor=%i\n", pUlCtrlCh->codingDescriptor);
            printf("    pUlCtrlCh->blockCodeConcatenation=%i\n", pUlCtrlCh->blockCodeConcatenation);
            printf("    pUlCtrlCh->modulationType=%i\n", pUlCtrlCh->modulationType);
            printf("    pUlCtrlCh->mcsType=%i\n", pUlCtrlCh->mcsType);

            printf("    pUlCtrlCh->pucchDedCtl.formatType=%i\n", pUlCtrlCh->pucchDedCtl.formatType);
            printf("    pUlCtrlCh->pucchDedCtl.ackNackRepetition=%i\n", pUlCtrlCh->pucchDedCtl.ackNackRepetition);
            printf("    pUlCtrlCh->pucchDedCtl.repetitionFactor=%i\n", pUlCtrlCh->pucchDedCtl.repetitionFactor);
            printf("    pUlCtrlCh->pucchDedCtl.n1PucchANRep=%i\n", pUlCtrlCh->pucchDedCtl.n1PucchANRep);
            printf("    pUlCtrlCh->pucchDedCtl.cqiPUCCHResourceIndex=%i\n", pUlCtrlCh->pucchDedCtl.cqiPUCCHResourceIndex);
            printf("    pUlCtrlCh->pucchDedCtl.srPUCCHResourceIndex=%i\n", pUlCtrlCh->pucchDedCtl.srPUCCHResourceIndex);

            printf("    pUlCtrlCh->cqiPmiRiRpt.cqiReportModeAperiodic=%i\n", pUlCtrlCh->cqiPmiRiRpt.cqiReportModeAperiodic);
            printf("    pUlCtrlCh->cqiPmiRiRpt.nomPDSCHRSEPREOffset=%i\n", pUlCtrlCh->cqiPmiRiRpt.nomPDSCHRSEPREOffset);
            printf("    pUlCtrlCh->cqiPmiRiRpt.cqiReportPeriodicEnable=%i\n", pUlCtrlCh->cqiPmiRiRpt.cqiReportPeriodicEnable);
            printf("    pUlCtrlCh->cqiPmiRiRpt.cqiPUCCHResourceIndex=%i\n", pUlCtrlCh->cqiPmiRiRpt.cqiPUCCHResourceIndex);
            printf("    pUlCtrlCh->cqiPmiRiRpt.cqipmiConfigIndex=%i\n", pUlCtrlCh->cqiPmiRiRpt.cqipmiConfigIndex);
            printf("    pUlCtrlCh->cqiPmiRiRpt.cqiFormatIndicatorPeriodic=%i\n", pUlCtrlCh->cqiPmiRiRpt.cqiFormatIndicatorPeriodic);
            printf("    pUlCtrlCh->cqiPmiRiRpt.K=%i\n", pUlCtrlCh->cqiPmiRiRpt.K);
            printf("    pUlCtrlCh->cqiPmiRiRpt.riConfigIndex=%i\n", pUlCtrlCh->cqiPmiRiRpt.riConfigIndex);
            printf("    pUlCtrlCh->cqiPmiRiRpt.simultaneousAckNackAndCQI=%i\n", pUlCtrlCh->cqiPmiRiRpt.simultaneousAckNackAndCQI);

            printf("    pUlCtrlCh->numberofEntries=%i\n", pUlCtrlCh->numberofEntries);
            printf("    pUlCtrlCh->startRes=%i\n", pUlCtrlCh->startRes);
            printf("    pUlCtrlCh->numRes=%i\n", pUlCtrlCh->numRes);

            pUlCtrlCh++;
        }

        pSrs = (SRSDED *)(((uintptr_t)pUlSf) + pUlSf->offsetsrsInfo);
        for (i = 0; i < pUlSf->numberSrsinSf; i++)
        {
            printf("SRS %i:\n----------\n", i);
            printf("      pSrs->enableDisable=%i\n", pSrs->enableDisable);
            printf("      pSrs->srsBandwidth=%i\n", pSrs->srsBandwidth);
            printf("      pSrs->srsHoppingBandwidth=%i\n", pSrs->srsHoppingBandwidth);
            printf("      pSrs->transmissionComb=%i\n", pSrs->transmissionComb);
            printf("      pSrs->cyclicShift=%i\n", pSrs->cyclicShift);
            printf("      pSrs->freqDomainPosition=%i\n", pSrs->freqDomainPosition);
            printf("      pSrs->duration=%i\n", pSrs->duration);
            printf("      pSrs->srsConfigIndex=%i\n", pSrs->srsConfigIndex);

            pSrs++;
        }

        pRachCtrl = (RACHCTRL *)(((uintptr_t)pUlSf) + pUlSf->offsetRachCtrlStruct);
        printf("    pRachCtrl->prachEnable=%i\n", pRachCtrl->prachEnable);
        printf("    pRachCtrl->rootSequenceIndex=%i\n", pRachCtrl->rootSequenceIndex);
        printf("    pRachCtrl->prachConfigIndex=%i\n", pRachCtrl->prachConfigIndex);
        printf("    pRachCtrl->highSpeedFlag=%i\n", pRachCtrl->highSpeedFlag);
        printf("    pRachCtrl->zeroCorrelationZoneConfig=%i\n", pRachCtrl->zeroCorrelationZoneConfig);
        printf("    pRachCtrl->prachFreqOffset=%i\n", pRachCtrl->prachFreqOffset);
        break;

    case PHY_RXSTART_IND:
        pMsgInd = (PMSGIND) pGen;
        printf("  pRxStartInd->subFrameNum=%i\n", pMsgInd->subFrameNum);
        break;

    case PHY_RXSDU_IND:
        pRxSduInd = (PRXSDUIND) pGen;
        printf("  pRxSdu->channelId=%i\n", pRxSduInd->channelId);
        printf("  pRxSdu->msgType=%i\n", pRxSduInd->msgType);
        printf("  pRxSdu->phyEntityId=%i\n", pRxSduInd->phyEntityId);
        printf("  pRxSdu->numBitsRx=%i\n", pRxSduInd->numBitsRx);
        printf("  pRxSdu->status=%i\n", pRxSduInd->status);
        printf("  pRxSdu->frameNum=%i\n", pRxSduInd->frameNum);
        printf("  pRxSdu->subFrameNum=%i\n", pRxSduInd->subFrameNum);
        printf("  pRxSdu->chanType=%i\n", pRxSduInd->chanType);
        printf("  pRxSdu->pRxSdu=%p\n", (void*)pRxSduInd->pRxSdu);

        // Currently payload starts at this field
        pDat = (unsigned char *) (&pRxSduInd->pRxSdu);
        for (i = 0; i < pRxSduInd->numBitsRx >> 3;)
        {
            for (j = 0; j < 16; j++)
            {
                printf("%02X ", pDat[j]);
                if (i + j + 1 >= (pRxSduInd->numBitsRx >> 3))
                    break;
            }
            for (j = 0; j < 16; j++)
            {
                if (isprint(pDat[j]))
                    printf("%c", pDat[j]);
                else
                    printf(".");
                if (i + j + 1 >= (pRxSduInd->numBitsRx >> 3))
                    break;
            }
            printf("\n");
            i += 16;
            pDat += 16;

            // FIXME: This is debug check and to be removed
            if (i > 64)
                break;
        }
        break;

    case PHY_RXEND_IND:
        pMsgInd = (PMSGIND) pGen;
        printf("  pRxEndInd->subFrameNum=%i\n", pMsgInd->subFrameNum);
        printf("  pRxEndInd->frameNumber=%i\n", pMsgInd->frameNumber);
        break;

    case PHY_INIT_REQ:
        pInit = (PINITPARM) (pGen + 1);

        printf("  pInit->channelBandwidth=%i\n", pInit->channelBandwidth);
        printf("  pInit->frtypeDuplexmode=%i\n", pInit->frtypeDuplexmode);
        printf("  pInit->radioAccessMode=%i\n", pInit->radioAccessMode);
        printf("  pInit->physicalResourceBandwidth=%i\n", pInit->physicalResourceBandwidth);
        printf("  pInit->numberResourceBlocksperSlot=%i\n", pInit->numberResourceBlocksperSlot);
        printf("  pInit->phyLayerCellIdGroup=%i\n", pInit->phyLayerCellIdGroup);
        printf("  pInit->phyLayerId=%i\n", pInit->phyLayerId);
        printf("  pInit->txAntennaPortCount=%i\n", pInit->txAntennaPortCount);
        printf("  pInit->fastfwdorPreclockingNumber=%i\n", pInit->fastfwdorPreclockingNumber);
        printf("  pInit->fftSize=%i\n", pInit->fftSize);
        printf("  pInit->numberUsedSubcarriers=%i\n", pInit->numberUsedSubcarriers);
        printf("  pInit->nMaxDlRb=%i\n", pInit->nMaxDlRb);
        printf("  pInit->nMaxUlRb=%i\n", pInit->nMaxUlRb);
        printf("  pInit->referenceSignalPower=%i\n", pInit->referenceSignalPower);
        printf("  pInit->primarySyncSignalPower=%i\n", pInit->primarySyncSignalPower);
        printf("  pInit->secondarySyncSignalPower=%i\n", pInit->secondarySyncSignalPower);
        printf("  pInit->numDataRePerPRB=%i\n", pInit->numDataRePerPRB);
        printf("  pInit->cyclicPrefixType=%i\n", pInit->cyclicPrefixType);
        printf("  pInit->rxAntennaPortCount=%i\n", pInit->rxAntennaPortCount);
        printf("  pInit->pb=%i\n", pInit->pb);
        printf("  pInit->customExtensionReportEn=%i\n", pInit->customExtensionReportEn);
        printf("  pInit->rachReportMode=%i\n", pInit->rachReportMode);
        printf("  pInit->txSduConfEnable=%i\n", pInit->txSduConfEnable);
        printf("  pInit->sduConfig=%i\n", pInit->sduConfig);
        printf("  pInit->radioFrameNumber=%i\n", pInit->radioFrameNumber);
        printf("  pInit->subframeNumber=%i\n", pInit->subframeNumber);
        printf("  pInit->slotNumber=%i\n", pInit->slotNumber);
        printf("  pInit->srsBandwidthConfig=%i\n", pInit->srsBandwidthConfig);
        printf("  pInit->srsSubframeConfig=%i\n", pInit->srsSubframeConfig);
        printf("  pInit->srsSimultaneousANandSRS=%i\n", pInit->srsSimultaneousANandSRS);
        printf("  pInit->prachConfigurationIndex=%i\n", pInit->prachConfigurationIndex);
        printf("  pInit->prachFrequencyOffset=%i\n", pInit->prachFrequencyOffset);
        printf("  pInit->prachHighSpeedFlag=%i\n", pInit->prachHighSpeedFlag);
        printf("  pInit->prachCyclicShiftConfig=%i\n", pInit->prachCyclicShiftConfig);
        printf("  pInit->prachRootSequenceIndex=%i\n", pInit->prachRootSequenceIndex);
        break;

    default:
        printf("Unknown command:\n----------------\n");
        // Dump 64 bytes of unknown command
        pDat = (unsigned char *) vector;
        for (i = 0; i < 64; i += 16)
        {
            for (j = 0; j < 16; j++)
            {
                printf("%02X ", pDat[j]);
            }
            for (j = 0; j < 16; j++)
            {
                if (isprint(pDat[j]))
                    printf("%c", pDat[j]);
                else
                    printf(".");
            }
            printf("\n");
            pDat += 16;
        }
        break;
    }
#endif /* ENODEB */
    return SUCCESS;
}

/** Initialize diagnostics module
 *
 * @return Standard result code
 */
RESULTCODE DiagInit(void)
{
    ReadSettingInt("diag", &DiagDebugLevel, DiagDebugLevel);
    return SUCCESS;
}

/** Cleanup diagnostics module
 *
 * @return Standard result code
 */
RESULTCODE DiagDone(void)
{
    return SUCCESS;
}

/** Sets debugging level
 *
 * @param level Debugging level bitmask
 * @return Standard result code
 */
RESULTCODE DiagSetDebugLevel(S32 level)
{
    DiagDebugLevel = level;
    return SUCCESS;
}
