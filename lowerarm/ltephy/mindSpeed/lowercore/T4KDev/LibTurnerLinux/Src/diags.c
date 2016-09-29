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
#include "settings.h"

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
