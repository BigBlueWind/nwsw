/** @file console.c
 *
 * @brief Console command interpreter
 * @author Mindspeed Technologies
 * @version $Revision: 1.61 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#include "osal.h"
#include "console.h"
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include "maciface.h"
#include "phyiface.h"
#include "apidefs.h"
#include "msgroute.h"
#include "svsr.h"
#include "settings.h"
#include "transport.h"
#include "icpu.h"
#include "hostiface.h"
#include "auxiface.h"
#include "timers.h"
//#include "larmmsgid.h"
//#include "dspdiag.h"
#if defined (EHDMAC)
#include "rrciface.h"
#endif
#include "diags.h"
#if defined (ENODEB)
#include "LtePhyL2Api.h"
//#include "LteMac2PhyInterface.h"
#endif

#ifdef USE_WINDOWS
#define strcasecmp                  _stricmp
#define strtoull                    _strtoi64
#endif /* USE_WINDOWS */

#ifdef __CC_ARM
// TODO: Check how RVDS' strtol works
#define EINVAL                      22  /* Invalid argument */
#endif /* __CC_ARM */

//
// External dependencies
// TODO: Move to headers
//
extern RESULTCODE MacInit(PhyTxCb txCb, PhyRxCb rxCb, HANDLE toPhy, HANDLE fromPhy);

//
// Limits
//
#define CON_BUFSIZE                 256

//
// Internal types
//
typedef RESULTCODE (*CMDPARSER)(char*);

typedef struct CommandList
{
    char *cmdName;
    CMDPARSER cmdParser;
    char *cmdParam;
    char *cmdHelp;
} CMDLIST;

typedef struct VariableList
{
    char *varName/*[16]*/;
    VARTYPE varType;
    PTR varPtr;
    OSAL_SIZE bufSize;
} VARLIST;

//
// Forward declarations
//
RESULTCODE CmdQuit(char *buffer);
RESULTCODE CmdHelp(char *buffer);
RESULTCODE CmdSet(char *buffer);
RESULTCODE CmdGet(char *buffer);

RESULTCODE CmdStart(char *buffer);
RESULTCODE CmdStop(char *buffer);
RESULTCODE CmdRestart(char *buffer);

RESULTCODE CmdLoadDsp(char *buffer);
RESULTCODE CmdLoadLarm(char *buffer);

RESULTCODE CmdCreateMac(char *buffer);
RESULTCODE CmdCreatePhy(char *buffer);
RESULTCODE CmdDestroyMac(char *buffer);
RESULTCODE CmdDestroyPhy(char *buffer);
#if defined (EHDMAC)
RESULTCODE CmdCreateRrc(char *buffer);
RESULTCODE CmdDestroyRrc(char *buffer);
RESULTCODE CmdRrc(char *buffer);
#endif

RESULTCODE CmdPhyTick(char *buffer);
RESULTCODE CmdPhyStart(char *buffer);
RESULTCODE CmdPhyStop(char *buffer);
RESULTCODE CmdLteDlStart(char *buffer);
RESULTCODE CmdLtePhyStop(char *buffer);
#ifdef C4KEVM
RESULTCODE CmdPhyCevaDump(char *buffer);
RESULTCODE CmdPhyQCevaDump(char *buffer);
#endif
RESULTCODE CmdPhyGetSample(char *buffer);

RESULTCODE CmdCommand(char *buffer);
RESULTCODE CmdIndication(char *buffer);
RESULTCODE CmdReset(char *buffer);
RESULTCODE CmdDiag(char *buffer);
#if defined (C4KEVM)
RESULTCODE CmdRdMem(char *buffer);
RESULTCODE CmdWaveJudgeDl(char *buffer);
RESULTCODE CmdWaveJudgeUl(char *buffer);
#endif
RESULTCODE ConMsgDispatch(U32 size, PTR msg);

//
// Module globals
//
static char ConBuffer[CON_BUFSIZE];
U16 ConSrcID = 0;

//
// Command list
//
CMDLIST CmdList[] =
{
    {"cmd", CmdCommand, "<dst> <msg> [<par_id> <data> ...]", "Dispatch API command"},
    {"createmac", CmdCreateMac, "<id> <phy_id>", "Create MAC instance"},
    {"createphy", CmdCreatePhy, "<id>", "Create PHY instance"},
//  {"destroymac", CmdDestroyMac, "<id>", "Destroy MAC instance"},
//  {"destroyphy", CmdDestroyPhy, "<id>", "Destroy PHY instance"},
    {"diag", CmdDiag, "<module> <target> [<par1>... <parN>]", "Run diagnostics"},
//  {"dsp", CmdLoadDsp, "<path>", "Load DSP firmware"},
    {"exit", CmdQuit, "", "Shut down framework"},
//  {"get", CmdGet, "<var>", "Get value of variable"},
    {"help", CmdHelp, "", "Print this help"},
    {"ind", CmdIndication, "ready|<src> <msg> [<par_id> <data> ...]", "Send indication"},
    {"larm", CmdLoadLarm, "<path>", "Load L-ARM firmware"},
#ifdef ENODEB
    {"ltephystop", CmdLtePhyStop, "", "Stop DL thread"},
#endif
#ifdef C4KEVM
    {"phycevadump", CmdPhyCevaDump, "[<CEVA mask> <bufsize>]", "Do CEVA Dump"},
    {"phyqcevadump", CmdPhyQCevaDump, "[<CEVA mask>]", "Query CEVA Dump"},
#endif
    {"phygetiq", CmdPhyGetSample, "", "Get IQ samples"},
    {"phystart", CmdPhyStart, "[mode, period, count]",
     "Start PHY operation\n\t\tmode is 0=CPRI, 1=Timer, 2=Event, 3=Completion"},
    {"phystop", CmdPhyStop, "", "Stop PHY operation"},
    {"phytick", CmdPhyTick, "", "Trigger PHY TTI"},
    {"quit", CmdQuit, "", "Shut down framework"},
#if defined (C4KEVM)
    {"rdmem", CmdRdMem, "<start_addr> <size> <filename>",
     "Read memory contents from physical address, store to a file"},
#endif
//  {"reset", CmdReset, "", "Reset the device"},
    {"restart", CmdRestart, "<device>", "Restart device [icpu|host|proto|aux|...]"},
    {"start", CmdStart, "<device>", "Start device [icpu|host|proto|aux|...]"},
    {"stop", CmdStop, "<device>", "Stop device [icpu|host|proto|aux|...]"},
//  {"set", CmdSet, "<var>=<val>", "Set value of variable"},
    {"syn", CmdPhyTick, "", "Shorter alias for the 'phytick' command"},
#if defined (C4KEVM)
	// FIXME: This is temporary command
    {"wjdl", CmdWaveJudgeDl, "[index]", "Save DL I/Q samples in WaveJudge format\n"
     "\tStart address and size can be adjusted by -iqdladdr and -iqdlsize settings\n"
     "\tOptional 'index' specifies file name"},
    {"wjul", CmdWaveJudgeUl, "[index]", "Save UL I/Q samples in WaveJudge format\n"
     "\tStart address and size can be adjusted by -iquladdr and -iqulsize settings\n"
     "\tOptional 'index' specifies file name"},
#endif
#if defined (EHDMAC)
    {"createrrc", CmdCreateRrc, "<id>", "Create RRC instance"},
    {"destroyrrc", CmdDestroyRrc, "<id>", "Destroy RRC instance"},
    {"rrc", CmdRrc, "bcch", "Send BCCH request"},
#endif
    {NULL, NULL, NULL, NULL}                // Indicate end of list
};

//
// External variables
//
extern OSAL_HEAP OsalGlobalHeap;

//
// Get/Set list
//
VARLIST VarList[CON_MAXVAR] =
{
    {"", VAR_UNKNOWN, NULL, 0}              // Indicate end of list
};

/** Initialize console module
 *
 * @return Standard result code
 */
RESULTCODE ConInit(void)
{
    int i;

    for (i = 0; i < CON_MAXVAR; i++)
    {
        VarList[i].varName = NULL;
        VarList[i].varType = VAR_UNKNOWN;
        VarList[i].varPtr = NULL;
        VarList[i].bufSize = 0;
    }

    if ((ConSrcID = MsgGetAvailableInstance()) != 0)
    {
        MsgRegDispatcher(ConSrcID, ConMsgDispatch);
    }
    return SUCCESS;
}

/** Register variable for GET/SET operation
 *
 * @param name
 * @param type
 * @param varptr
 * @return
 */
RESULTCODE ConRegisterVar(char *name, VARTYPE type, PTR varptr, OSAL_SIZE bufsize)
{
    int i;

    for (i = 0; i < CON_MAXVAR; i++)
    {
        if (VarList[i].varName == NULL)
        {
            VarList[i].varName = name;
            VarList[i].varType = type;
            VarList[i].varPtr = varptr;
            VarList[i].bufSize = bufsize;
            return SUCCESS;
        }
    }
    return FAILURE;
}

/** Dispatch console messages
 *  This implementation only prints message to console if corresponding diag level enabled
 *
 * @param size Size of the message
 * @param msg Pointer to message's header
 * @return Standard result code
 */
RESULTCODE ConMsgDispatch(U32 size, PTR msg)
{
    DiagPrintApi(DIAG_LEVEL30, "CONRX", DIAG_DECODETLV, msg);
    MsgFreeBuffer(msg);

    return SUCCESS;
}

/** Handle keypresses
 *
 * @param ch
 * @return Standard result code
 */
RESULTCODE ConDispatch(char ch)
{
    static int writeIdx = 0;
    int readIdx;
    char *cmd;
    RESULTCODE ret = SUCCESS;

    switch (ch)
    {
        case '\t':
            ConDispatch(' ');
            break;
        case '\r':
            break;
        case '\n':
            ConBuffer[writeIdx] = '\0';
            cmd = strtok(ConBuffer, " ");
            readIdx = 0;
            if (cmd)
            {
                while (CmdList[readIdx].cmdParser != NULL)
                {
                    if (strcasecmp(cmd, CmdList[readIdx].cmdName) == 0)
                    {
                        ret = CmdList[readIdx].cmdParser(ConBuffer);
                        if (ret != ERR_EXIT)
                        {
                            // TODO: Use real CPU ID
                            ConPrompt(UARM);
                        }
                        break;
                    }
                    readIdx++;
                }
            }
            writeIdx = 0;
            break;
        default:
            if (writeIdx < CON_BUFSIZE-1)
            {
                ConBuffer[writeIdx++] = ch;
            }
            break;
    }

    if (ret != ERR_EXIT)
    {
        // To avoid shutdown
        ret = SUCCESS;
    }
    return ret;
}

/**
 *
 * @param buffer
 * @return
 */
RESULTCODE CmdSet(char *buffer)
{
    char *varName, *varValue;
    int readIdx = 0;
    U32 intVal;
#if _MSC_VER > 1200
    U64 lintVal;
#endif

    // Continue token search
    varName = strtok(NULL, " ,=");
    if (varName == NULL)
    {
        printf("Invalid format of SET command\n\n");
        return FAILURE;
    }

    // Continue token search
    varValue = strtok(NULL, "\n");
    if (varValue == NULL)
    {
        printf("Invalid format of SET command\n\n");
        return FAILURE;
    }

    while (VarList[readIdx].varPtr != NULL)
    {
        if (strcasecmp(varName, VarList[readIdx].varName) == 0)
        {
            switch (VarList[readIdx].varType)
            {
                case VAR_U8:
                case VAR_U16:
                case VAR_U32:
                    intVal = strtoul(varValue, NULL, 10);
                    if ((intVal == 0 && errno == EINVAL) || (intVal == ULONG_MAX && errno == ERANGE))
                    {
                        printf("Invalid argument of SET command, no update performed\n");
                    }
                    else
                    {
                        if (VarList[readIdx].varType == VAR_U8)
                        {
                            *((U8*)VarList[readIdx].varPtr) = (U8) intVal;
                        }
                        else if (VarList[readIdx].varType == VAR_U16)
                        {
                            *((U16*)VarList[readIdx].varPtr) = (U16) intVal;
                        }
                        else
                        {
                            *((U32*)VarList[readIdx].varPtr) = (U32) intVal;
                        }
                    }
                    break;
#if _MSC_VER > 1200
                case VAR_U64:
                    lintVal = strtoull(varValue, NULL, 10);
                    break;
#endif
                case VAR_STR:
                    if (strlen(varValue) < VarList[readIdx].bufSize)
                    {
                        strcpy(VarList[readIdx].varPtr, varValue);
                    }
                    break;
                case VAR_PTR:
                    break;
                default:
                    break;
            }
            break;
        }
        readIdx++;
    }

    return SUCCESS;
}

/**
 *
 * @param buffer
 * @return
 */
RESULTCODE CmdGet(char *buffer)
{
    char *varName;
    int readIdx = 0;

    // Continue token search
    varName = strtok(NULL, "\n");
    if (varName == NULL)
    {
        printf("Invalid format of GET command\n\n");
        return FAILURE;
    }

    while (VarList[readIdx].varPtr != NULL)
    {
        if (strcasecmp(varName, VarList[readIdx].varName) == 0)
        {
            printf("\n%s = ", VarList[readIdx].varName);
            switch (VarList[readIdx].varType)
            {
                case VAR_U8:
                    printf("%hhu\n", *((U8*)(VarList[readIdx].varPtr)));
                    break;
                case VAR_U16:
                    printf("%hu\n", *((U16*)(VarList[readIdx].varPtr)));
                    break;
                case VAR_U32:
                    printf("%u\n", *((U32*)(VarList[readIdx].varPtr)));
                    break;
                case VAR_U64:
                    printf("%llu\n", *((U64*)(VarList[readIdx].varPtr)));
                    break;
                case VAR_STR:
                    printf("'%s'\n", *((char**)(VarList[readIdx].varPtr)));
                    break;
                case VAR_PTR:
                    printf("%p\n", *((PTR*)(VarList[readIdx].varPtr)));
                    break;
                default:
                    break;
            }
            break;
        }
        readIdx++;
    }
    return SUCCESS;
}

/**
 *
 * @param buffer
 * @return
 */
RESULTCODE CmdHelp(char *buffer)
{
    int i = 0;

    printf("\nHelp for T4KFW console commands\n\n");
    while (CmdList[i].cmdParser != NULL)
    {
        printf("%s %s\n    %s\n", CmdList[i].cmdName, CmdList[i].cmdParam, CmdList[i].cmdHelp);
        i++;
    }

    return SUCCESS;
}

/**
 *
 * @param buffer
 * @return
 */
RESULTCODE CmdQuit(char *buffer)
{
    return ERR_EXIT;
}

/** Display console prompt
 *
 * @param cpuID Where we are running
 * @return Standard result code
 */
RESULTCODE ConPrompt(U32 cpuID)
{
    if (cpuID == UARM)
    {
        printf("\nU-ARM>");
    }
    else
    {
        printf("\nL-ARM>");
    }
    return SUCCESS;
}

/** Execute 'Load DSP' command from console
 *
 * @param buffer
 * @return
 */
RESULTCODE CmdLoadDsp(char *buffer)
{
    char *path;

    // Continue token search
    path = strtok(NULL, "\n");
    if (path == NULL)
    {
        printf(C4KFW "No firmware image specified\n\n");
        return FAILURE;
    }

    return SvsrCommand(ConSrcID, SVSR_LOADDSP, 1, PAR_PATH, TYPE_STRING, path);
}

/** Execute 'Load L-ARM' command from console
 *
 * @param buffer
 * @return
 */
RESULTCODE CmdLoadLarm(char *buffer)
{
    char *path;

    // Continue token search
    path = strtok(NULL, "\n");
    if (path == NULL)
    {
        printf(C4KFW "No firmware image specified\n\n");
        return FAILURE;
    }

    return SvsrCommand(ConSrcID, SVSR_LOADLARM, 1, PAR_PATH, TYPE_STRING, path);
}

/** Start device operation
 *
 * @param buffer
 * @return
 */
RESULTCODE CmdStart(char *buffer)
{
    RESULTCODE ret = FAILURE;
    char *device;

    // Continue token search
    device = strtok(NULL, " \n");
    if (device == NULL)
    {
        printf(C4KFW "No device specified\n\n");
        return ret;
    }

    if (strcasecmp(device, "icpu") == 0)
    {
        ret = IcpuInit(UARM);
    }
    else if (strcasecmp(device, "host") == 0)
    {
        ret = HostInit();
    }
    else if (strcasecmp(device, "proto") == 0)
    {
        int proto = PROTO_SCTP;
        device = strtok(NULL, " \n");
        if (device)
        {
            if (strcasecmp(device, "sctp") == 0)
            {
                proto = PROTO_SCTP;
            }
            else if (strcasecmp(device, "tcp") == 0)
            {
                proto = PROTO_TCP;
            }
            else if (strcasecmp(device, "udp") == 0)
            {
                proto = PROTO_UDP;
            }
            else
            {
                printf("WARNING: Invalid protocol specified, default to SCTP\n");
            }
        }
        ret = ProtoInit(proto);
    }
    else if (strcasecmp(device, "aux") == 0)
    {
        ret = AuxInit();
    }

    return ret;
}

/** Stop device operation
 *
 * @param buffer
 * @return
 */
RESULTCODE CmdStop(char *buffer)
{
    RESULTCODE ret = FAILURE;
    char *device;

    // Continue token search
    device = strtok(NULL, " \n");
    if (device == NULL)
    {
        printf(C4KFW "No device specified\n\n");
        return ret;
    }

    if (strcasecmp(device, "icpu") == 0)
    {
        ret = IcpuDone();
    }
    else if (strcasecmp(device, "host") == 0)
    {
        ret = HostDone();
    }
    else if (strcasecmp(device, "proto") == 0)
    {
        ret = ProtoDone();
    }
    else if (strcasecmp(device, "aux") == 0)
    {
        ret = AuxDone();
    }

    return ret;
}

/** Restart device operation
 *
 * @param buffer
 * @return
 */
RESULTCODE CmdRestart(char *buffer)
{
    RESULTCODE ret = FAILURE;
    char *device;

    // Continue token search
    device = strtok(NULL, " \n");
    if (device == NULL)
    {
        printf(C4KFW "No device specified\n\n");
        return ret;
    }

    if (strcasecmp(device, "icpu") == 0)
    {
        IcpuDone();
        ret = IcpuInit(UARM);
    }
    else if (strcasecmp(device, "host") == 0)
    {
        HostDone();
        ret = HostInit();
    }
    else if (strcasecmp(device, "proto") == 0)
    {
        int proto = PROTO_SCTP;
        device = strtok(device, "\n");
        if (device)
        {
            if (strcasecmp(device, "sctp") == 0)
            {
                proto = PROTO_SCTP;
            }
            else if (strcasecmp(device, "tcp") == 0)
            {
                proto = PROTO_TCP;
            }
            else if (strcasecmp(device, "udp") == 0)
            {
                proto = PROTO_UDP;
            }
            else
            {
                printf("WARNING: Invalid protocol specified, default to SCTP\n");
            }
        }
        ProtoDone();
        ret = ProtoInit(proto);
    }
    else if (strcasecmp(device, "aux") == 0)
    {
        AuxDone();
        ret = AuxInit();
    }

    return ret;
}

/**
 *
 * @param buffer Pointer to the command buffer
 * @return Standard result code
 */
RESULTCODE CmdCreateMac(char *buffer)
{
    RESULTCODE ret = SUCCESS;

    ret |= MacInit(PhySendMsg, PhyGetMsg, pIMSG_TO_PHY, pIMSG_FROM_PHY);
    if (ret == SUCCESS)
    {
        U16 InstId;
#if (defined (MSMAC) || defined (UE))
        InstId = MSGS_MSMAC;

        // FIXME: remove when debugging ends
        AppInit(&OsalGlobalHeap);
#else
        InstId = MSGS_MAC;
#endif
        printf(C4KFW "Created MAC instance [0x%04X]\n", InstId);
    }
    else
    {
        // TODO: Replace errno with error code
        printf(C4KFW "MAC creation failed [%i]\n", errno);
    }

    return ret;
}

/**
 *
 * @param buffer Pointer to the command buffer
 * @return Standard result code
 */
RESULTCODE CmdCreatePhy(char *buffer)
{
    U16 InstId;

    if (PhyEnable(&InstId) == SUCCESS)
    {
        printf(C4KFW "Created PHY instance [0x%X]\n", InstId);
    }
    else
    {
        printf(C4KFW "PHY creation failed\n");
    }
    return SUCCESS;
}

#if defined (EHDMAC)
/** Create RRC instance
 *
 * @param buffer Pointer to the command buffer
 * @return Standard result code
 */
RESULTCODE CmdCreateRrc(char *buffer)
{
    if (RrcInit(NULL, NULL) == SUCCESS)
    {
        printf(C4KFW "Created RRC instance\n");
    }
    else
    {
        printf(C4KFW "RRC creation failed\n");
    }
    return SUCCESS;
}

/** Destroy RRC instance
 *
 * @param buffer Pointer to the command buffer
 * @return Standard result code
 */
RESULTCODE CmdDestroyRrc(char *buffer)
{
    char *varValue;
    int intValue;

    // Continue token search
    varValue = strtok(NULL, "\n");
    if (varValue == NULL)
    {
        printf(C4KFW "Invalid format of DESTROYRRC command\n\n");
        return FAILURE;
    }

    intValue = strtoul(varValue, NULL, 10);
    if ((intValue == 0 && errno == EINVAL) || (intValue == ULONG_MAX && errno == ERANGE))
    {
        printf(C4KFW "Invalid argument of DESTROYRRC command\n");
    }
    else
    {
        // TODO: Destroy here....
        printf(C4KFW "Deleted RRC instance [%i]\n", intValue);
    }

    return SUCCESS;
}

/** Send RRC command
 *
 * @param buffer Pointer to the command buffer
 * @return Standard result code
 */
RESULTCODE CmdRrc(char *buffer)
{
#if !defined (UE) && !defined (ARICENT_MSPD_MWC_2010)

    extern RESULTCODE RrcSendBcchMsg(U16 sfn);
#endif

    char *varValue;
    int value = 0;

    // Continue token search
    varValue = strtok(NULL, " \n");
    if (varValue == NULL)
    {
        printf(C4KFW "Invalid format of RRC command\n\n");
        return FAILURE;
    }

    if (strcasecmp(varValue, "bcch") == 0)
    {
        // Continue token search
        varValue = strtok(NULL, " \n");
        if (varValue != NULL)
        {
            value = strtoul(varValue, NULL, 10);
            if (value == ULONG_MAX)
                value = 0;
        }
#if !defined (UE) && !defined (ARICENT_MSPD_MWC_2010)

        // Send MAC_BCCH_MSG_REQ
        RrcSendBcchMsg(value);
#endif
    }
    return SUCCESS;
}
#endif /* EHDMAC */

/**
 *
 * @param buffer Pointer to the command buffer
 * @return Standard result code
 */
RESULTCODE CmdDestroyMac(char *buffer)
{
    char *varValue;
    int intValue;

    // Continue token search
    varValue = strtok(NULL, "\n");
    if (varValue == NULL)
    {
        printf(C4KFW "Invalid format of DESTROYMAC command\n\n");
        return FAILURE;
    }

    intValue = strtoul(varValue, NULL, 10);
    if ((intValue == 0 && errno == EINVAL) || (intValue == ULONG_MAX && errno == ERANGE))
    {
        printf(C4KFW "Invalid argument of DESTROYMAC command\n");
    }
    else
    {
        if (MacDone() == SUCCESS)
        {
            printf(C4KFW "Deleted MAC instance [%i]\n", intValue);
        }
        else
        {
            // TODO: Replace errno with error code
            printf(C4KFW "MAC deletion failed [%i]\n", errno);
        }
    }

    return SUCCESS;
}

/**
 *
 * @param buffer Pointer to the command buffer
 * @return Standard result code
 */
RESULTCODE CmdDestroyPhy(char *buffer)
{
    char *varValue;
    int intValue;

    // Continue token search
    varValue = strtok(NULL, "\n");
    if (varValue == NULL)
    {
        printf(C4KFW "Invalid format of DESTROYPHY command\n\n");
        return FAILURE;
    }

    intValue = strtoul(varValue, NULL, 10);
    if ((intValue == 0 && errno == EINVAL) || (intValue == ULONG_MAX && errno == ERANGE))
    {
        printf(C4KFW "Invalid argument of DESTROYPHY command\n");
    }
    else
    {
        PhyDone();
        printf(C4KFW "Deleted PHY instance [%i]\n", intValue);
    }

    return SUCCESS;
}

/** Dispatch manually entered command
 *
 * @param buffer
 * @return
 */
RESULTCODE CmdCommand(char *buffer)
{
    int paramID = 0;
    int radix = 10;
    int intVal = 0;
    PMSGHEADER pHdr;
    char *nextTok = strtok(NULL, " ,\n");

    enum
    {
        STT_DSTID, STT_MSGID, STT_PARID, STT_DATA
    } cmdState = STT_DSTID;

    pHdr = MsgAllocBuffer(MSG_COMMON);
    if (pHdr == NULL)
    {
        return FAILURE;
    }
    MsgInitMessage(pHdr);
    pHdr->type = MSGT_CONFIG;
    pHdr->srcID = ConSrcID;

    while (nextTok)
    {
        switch (*nextTok)
        {
            case '0':
                if (*(nextTok+1) == 'x' || *(nextTok+1) == 'X')
                {
                    radix = 16;
                }
                else
                {
                    radix = 10;
                }
                intVal = strtoul(nextTok, NULL, radix);
                break;
            case '\"':
                intVal = 1;
                radix = 0;      // Indicate this is string parameter
                break;
            default:
                radix = 10;
                intVal = strtoul(nextTok, NULL, radix);
                break;
        }
        if ((intVal == 0 && errno == EINVAL) || (intVal == ULONG_MAX && errno == ERANGE))
        {
            printf("Invalid argument of CMD command\n");
            MsgFreeBuffer(pHdr);
            return FAILURE;
        }
        switch (cmdState)
        {
            case STT_DSTID:
                pHdr->dstID = intVal;
                cmdState = STT_MSGID;
                break;
            case STT_MSGID:
                pHdr->msgID = intVal;
                cmdState = STT_PARID;
                break;
            case STT_PARID:
                paramID = intVal;
                cmdState = STT_DATA;
                break;
            case STT_DATA:
                if (radix == 0)
                {
                    // This is string parameter
                    nextTok = strtok(nextTok, "\"");
                    if (nextTok == NULL)
                    {
                        // Incomplete string
                        printf("Malformed parameter\n");
                        return FAILURE;
                    }
                    else
                    {
                        // Add terminating zero byte
                        MsgAppendParam(pHdr, paramID, strlen(nextTok)+1, nextTok);
                    }
                }
                else
                {
                    MsgAppendU32(pHdr, paramID, intVal);
                }
                cmdState = STT_PARID;
                break;
        }
        nextTok = strtok(NULL, " ,\n");
    }
    return MsgDispatch(MSGS_HOST, pHdr->length + sizeof(MSGHEADER), pHdr);
}

/** Send indication to Host
 *
 * @param buffer
 * @return
 */
RESULTCODE CmdIndication(char *buffer)
{
    int paramID = 0;
    int radix = 10;
    int intVal = 0;
    PMSGHEADER pHdr;
    char *nextTok = strtok(NULL, " ,\n");

    enum
    {
        STT_SRCID, STT_MSGID, STT_PARID, STT_DATA
    } cmdState = STT_SRCID;

    pHdr = MsgAllocBuffer(MSG_COMMON);
    if (pHdr == NULL)
    {
        return FAILURE;
    }
    MsgInitMessage(pHdr);
    pHdr->type = MSGT_INDICATION;
    pHdr->dstID = MSGS_HOST;

    while (nextTok)
    {
        switch (*nextTok)
        {
            case '0':
                if (*(nextTok+1) == 'x' || *(nextTok+1) == 'X')
                {
                    radix = 16;
                }
                else
                {
                    radix = 10;
                }
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                intVal = strtoul(nextTok, NULL, radix);
                break;
            case '\"':
                intVal = 1;
                radix = 0;      // Indicate this is string parameter
                break;
            default:
                if (strcasecmp(nextTok, "ready") == 0)
                {
                    MsgFreeBuffer(pHdr);
                    return SvsrReady();
                }
                else
                {
                    printf("Unknown indication\n");
                    MsgFreeBuffer(pHdr);
                    return FAILURE;
                }
                break;
        }
        if ((intVal == 0 && errno == EINVAL) || (intVal == ULONG_MAX && errno == ERANGE))
        {
            printf("Invalid argument of IND command\n");
            MsgFreeBuffer(pHdr);
            return FAILURE;
        }
        switch (cmdState)
        {
            case STT_SRCID:
                pHdr->dstID = intVal;
                cmdState = STT_MSGID;
                break;
            case STT_MSGID:
                pHdr->msgID = intVal;
                cmdState = STT_PARID;
                break;
            case STT_PARID:
                paramID = intVal;
                cmdState = STT_DATA;
                break;
            case STT_DATA:
                if (radix == 0)
                {
                    // This is string parameter
                    nextTok = strtok(nextTok, "\"");
                    if (nextTok == NULL)
                    {
                        // Incomplete string
                        printf("Malformed parameter\n");
                        return FAILURE;
                    }
                    else
                    {
                        // Add terminating zero byte
                        MsgAppendParam(pHdr, paramID, strlen(nextTok)+1, nextTok);
                    }
                }
                else
                {
                    MsgAppendU32(pHdr, paramID, intVal);
                }
                cmdState = STT_PARID;
                break;
        }
        nextTok = strtok(NULL, " ,\n");
    }
    return MsgDispatch(MSGS_HOST, pHdr->length + sizeof(MSGHEADER), pHdr);
}

/** Reset device
 *
 * @param buffer Not used
 * @return Standard result code
 */
RESULTCODE CmdReset(char *buffer)
{
    return SvsrCommand(ConSrcID, SVSR_RESET, 0);
}

/** PHY TTI
 *
 * @param buffer Not used
 * @return Standard result code
 */
RESULTCODE CmdPhyTick(char *buffer)
{
    return MsgCommand(MSGS_SVSR,
                      MSGS_LARM | MSGS_LTEPHY,
                      PHY_LTE_PHYTICK_MSG,
                      0);
}

#ifdef C4KEVM
extern CevaDumpCtx dumpinfo[];
/** Dump Ceva configuration
 *
 * @param buffer Not used
 * @return Standard result code
 */
RESULTCODE CmdPhyCevaDump(char *buffer)
{
    char *maskStr, *bufsizeStr;
    PMSGHEADER pHdr = MsgAllocBuffer(MSG_COMMON);
    U32 phyMask = 0x3FF; // all 10 CEVAs by default
    U32 phyBufsize = (64 * 1024); // max buffer by default for each CEVA dumping

    if (pHdr == NULL)
    {
        DiagPrint(DIAG_LEVEL5, "ERROR", "No memory [%s,%i]\n", __FUNCTION__, __LINE__);
        return FAILURE;
    }

    // Continue token search
    maskStr = strtok(NULL, " \n");
    if (maskStr)
    {
        phyMask = strtoul(maskStr, NULL, 0);
        bufsizeStr = strtok(NULL, " \n");
        if (bufsizeStr)
            phyBufsize = strtoul(bufsizeStr, NULL, 0);
    }

    pHdr->control = CTRL_VERSION; // Encapsulated protocol;
    pHdr->length = 0;
    pHdr->type = MSGT_CONFIG;
    pHdr->srcID = MSGS_SVSR;
    pHdr->dstID = MSGS_LARM | 2; // DspDiag of Ceva0 is used
    pHdr->msgID = LARM_HDIAG_CEVADUMP /*DIAG_CEVA_DUMP*/;

    MsgAppendU32(pHdr, 2 /*DSPDIAG_PARAM_MASK*/, phyMask);
    MsgAppendU32(pHdr, 3 /*DSPDIAG_PARAM_BUFINFO*/, phyBufsize);

    return MsgDispatch(MSGS_MAC, pHdr->length + sizeof(MSGHEADER), pHdr);
}

/** Store CEVA dump to a file
 *
 * @param CevaID
 * @param size Data size
 * @param data Pointer to the data
 * @return Standard result code
 */
static RESULTCODE CevaDumpSaveData(U32 CevaID, PTR data, U32 size)
{
    FILE *file;
    char filename[16];
    char modeFlag[] = {'w', '\0', '\0', '\0'};

    sprintf(filename, "DataCeva%i.bin", CevaID);
    if ((file = fopen(filename, modeFlag)) == NULL)
        return FAILURE;

    fwrite(data, size, 1, file);

    fclose(file);
    return SUCCESS;
}

/** Dump Ceva query and store data to file
 *
 * @param buffer Not used
 * @return Standard result code
 */
RESULTCODE CmdPhyQCevaDump(char *buffer)
{
    extern PTR IcpuMapIo(PTR addr, U32 len);
    extern void IcpuUnmapIo(PTR addr);

    char *maskStr;
    U32 phyMask = 0x3FF; // all 10 CEVAs by default
    U32 i;

    // Continue token search
    maskStr = strtok(NULL, " \n");
    if (maskStr)
    {
        phyMask = strtoul(maskStr, NULL, 0);
    }

    i = 0;
    while(phyMask)
    {
        if(phyMask & 1)
        {
            U8* start;
            U32 count;
            start = dumpinfo[i].pDumpBuf;
            count = dumpinfo[i].DumpSize;
            if(start && count)
            {
                U32* virtAddr;

                virtAddr = IcpuMapIo((PTR) start, count);
                if (virtAddr == NULL)
                {
                    DiagPrint(DIAG_LEVELALL, "ERROR", "Unable to map address [0x%X:%u]\n", start, count);
                    return FAILURE;
                }
                DiagPrint(DIAG_LEVELALL, NULL, "Saving CEVA%d dump [Addr=0x%X, VA=0x%X, Size=%u]... ",
                      dumpinfo[i].CevaID, start, virtAddr, count);
                CevaDumpSaveData(dumpinfo[i].CevaID, virtAddr, count);
                IcpuUnmapIo((PTR) start);
                DiagPrint(DIAG_LEVELALL, NULL, "Done\n");
            }            
        }
        phyMask >>= 1;
        ++i;
    }
    
    return SUCCESS;
}
#endif

RESULTCODE CmdPhyGetSample(char *buffer)
{
#if defined (ENODEB)
    PMSGHEADER pHdr = MsgAllocBuffer(MSG_COMMON);
    if (pHdr == NULL)
    {
        return FAILURE;
    }

    pHdr->control = CTRL_VERSION; // Encapsulated protocol;
    pHdr->length = 0;
    pHdr->type = MSGT_QUERY;
    pHdr->srcID = MSGS_MAC;
    pHdr->dstID = MSGS_LARM | 0x19;
    pHdr->msgID = 0x5119;
    MsgAppendU32(pHdr, 6, 1);
    MsgAppendU32(pHdr, 3, 1500);
    MsgAppendU32(pHdr, 4, 0);

    return MsgDispatch(MSGS_MAC, pHdr->length + sizeof(MSGHEADER), pHdr);

#endif

    return SUCCESS;
}

/** Start PHY
 *
 * @param buffer Not used
 * @return Standard result code
 */
RESULTCODE CmdPhyStart(char *buffer)
{
#if defined (ENODEB)

    char *modeStr, *periodStr, *countStr;
    PMSGHEADER pHdr = MsgAllocBuffer(MSG_COMMON);
    int phyMode = AUTO_TIMER;
    int phyModePeriod = 10;
    int phyModeCount = 0;

    if (pHdr == NULL)
    {
        DiagPrint(DIAG_LEVEL5, "ERROR", "No memory [%s,%i]\n", __FUNCTION__, __LINE__);
        return FAILURE;
    }

    // Continue token search
    modeStr = strtok(NULL, " \n");
    if (modeStr)
    {
        phyMode = strtoul(modeStr, NULL, 0);
        periodStr = strtok(NULL, " \n");
        if (periodStr)
        {
            phyModePeriod = strtoul(periodStr, NULL, 0);
            countStr = strtok(NULL, " \n");
            if (countStr)
                phyModeCount = strtoul(countStr, NULL, 0);
        }
    }
    DiagPrint(DIAG_LEVEL28, NULL, "\nStart PHY with Mode=%i,Period=%i,Count=%i\n",
              phyMode, phyModePeriod, phyModeCount);

    pHdr->control = CTRL_DEFAULT;
    pHdr->length = 0;
    pHdr->type = MSGT_CONFIG;
    pHdr->srcID = ConSrcID;
    pHdr->dstID = MSGS_MAC;
    pHdr->msgID = PHY_LTE_START;
    MsgAppendU32(pHdr, PAR_PHY_INSTANCE_ID, 0);             // Default PHY instance ID
    MsgAppendU32(pHdr, PAR_PHY_SFCOUNT, phyModeCount);
    MsgAppendU32(pHdr, PAR_PHY_TIME_RESOLUTION, phyModePeriod);
    MsgAppendU32(pHdr, PAR_PHY_TICK_MODE, phyMode);

    return MsgDispatch(MSGS_HOST, pHdr->length + sizeof(MSGHEADER), pHdr);

#else // ENODEB
    return SUCCESS;
#endif
}

/** Stop PHY
 *
 * @param buffer Not used
 * @return Standard result code
 */
RESULTCODE CmdPhyStop(char *buffer)
{
#if defined (ENODEB)

#if 1
     PMSGHEADER pHdr = MsgAllocBuffer(MSG_COMMON);
    if (pHdr == NULL)
    {
        return FAILURE;
    }
    pHdr->control = CTRL_VERSION;
    pHdr->length = 0;
    pHdr->type = MSGT_CONFIG;
    pHdr->srcID = MSGS_HOST;
    pHdr->dstID = MSGS_MAC;
    pHdr->msgID = PHY_LTE_STOP;
    return MsgDispatch(MSGS_HOST, pHdr->length + sizeof(MSGHEADER), pHdr);

#else
    return PhySendToPhyMsg(PHY_STOP_REQ, NULL);
#endif

#else // ENODEB
    return SUCCESS;
#endif
}

#ifdef ENODEB
RESULTCODE CmdLtePhyStop(char *buffer)
{
    extern U32 globalTTITickCount;
    PMSGHEADER pHdr = MsgAllocBuffer(MSG_COMMON);

    if (pHdr == NULL)
    {
        return FAILURE;
    }

    DiagPrint(DIAG_LEVELALL, NULL, "PHY stopped [TTI %d]\n", globalTTITickCount);

    pHdr->control = CTRL_VERSION;
    pHdr->length = 4;
    pHdr->type = MSGT_CONFIG;
    pHdr->srcID = MSGS_MAC;
    pHdr->dstID = MSGS_LTEPHY | MSGS_LARM;
    pHdr->msgID = PHY_LTE_TIMER_STOP;

    return MsgDispatch(MSGS_MAC, pHdr->length + sizeof(MSGHEADER), pHdr);
}
#endif // ENODEB

RESULTCODE CmdDiag(char *buffer)
{
    char *module;
    char *dest;
    char *param;
    int destID, count, size;

    // Continue token search
    module = strtok(NULL, " ,;\n");
    if (module == NULL)
    {
        DiagPrint(DIAG_LEVELALL, NULL, "No module specified for DIAG command\n");
        return FAILURE;
    }

    // Continue token search
    dest = strtok(NULL, " ,;\n");
    if (dest == NULL)
    {
        DiagPrint(DIAG_LEVELALL, NULL, "No destination specified for DIAG command\n");
        return FAILURE;
    }

    if (strcasecmp(module, "loopback") == 0)
    {
        if (strcasecmp(dest, "icpu") == 0)
        {
            destID = MSGS_SVSR | MSGS_LARM;
        }
        else if (strcasecmp(dest, "ltephy") == 0)
        {
            destID = MSGS_LTEPHY | MSGS_LARM;
        }
        else if (strcasecmp(dest, "host") == 0)
        {
            destID = MSGS_HOST;
        }
        else
        {
            DiagPrint(DIAG_LEVELALL, NULL, "Destination ID is invalid\n");
            return FAILURE;
        }
        param = strtok(NULL, " ,;\n");
        if (param == NULL)
        {
            DiagPrint(DIAG_LEVELALL, NULL, "No parameter defined for LOOPBACK module\n");
            return FAILURE;
        }
        count = strtoul(param, NULL, 10);
        if ((count == 0 && errno == EINVAL) || (count == ULONG_MAX && errno == ERANGE))
        {
            printf(C4KFW "Invalid count value\n");
            return FAILURE;
        }
        param = strtok(NULL, " ,;\n");
        if (param == NULL)
        {
            DiagPrint(DIAG_LEVELALL, NULL, "No parameter defined for LOOPBACK module\n");
            return FAILURE;
        }
        size = strtoul(param, NULL, 10);
        if ((size == 0 && errno == EINVAL) || (size == ULONG_MAX && errno == ERANGE))
        {
            DiagPrint(DIAG_LEVELALL, NULL, "Invalid size value\n");
            return FAILURE;
        }
        DiagLoopback(destID, count, size);
    }
    if (strcasecmp(module, "print") == 0)
    {
        int level = strtol(dest, NULL, 0);
        if ((level == 0 && errno == EINVAL) || (level == LONG_MAX && errno == ERANGE))
        {
            DiagPrint(DIAG_LEVELALL, NULL, "Invalid argument of DIAG PRINT command\n");
        }
        else
        {
            DiagSetDebugLevel(level);
        }
    }

    return SUCCESS;
}

#if defined (C4KEVM)
/** Stores memory contents to a file on a disk
 *
 * @param buffer Command parameters (console input)
 * @return Standard result code
 */
RESULTCODE CmdRdMem(char *buffer)
{
    extern PTR IcpuMapIo(PTR addr, U32 len);
    extern void IcpuUnmapIo(PTR addr);

    char *startAddr;
    char *size;
    char *fileName;
    unsigned int start, count;
    U32* virtAddr;
    FILE *file;

    // Continue token search
    startAddr = strtok(NULL, " ,\n");
    if (startAddr == NULL)
    {
        DiagPrint(DIAG_LEVELALL, NULL, "No start address specified\n");
        return FAILURE;
    }

    // Continue token search
    size = strtok(NULL, " ,\n");
    if (size == NULL)
    {
        DiagPrint(DIAG_LEVELALL, NULL, "No size specified\n");
        return FAILURE;
    }

    fileName = strtok(NULL, "\n");
    if (fileName == NULL)
    {
        DiagPrint(DIAG_LEVELALL, NULL, "No file name specified\n");
        return FAILURE;
    }

    start = strtoul(startAddr, NULL, 0);
    if ((start == 0 && errno == EINVAL) || (start == ULONG_MAX && errno == ERANGE))
    {
        DiagPrint(DIAG_LEVELALL, NULL, "Invalid start address\n");
        return FAILURE;
    }

    count = strtoul(size, NULL, 10);
    if ((count == 0 && errno == EINVAL) || (count == ULONG_MAX && errno == ERANGE))
    {
        DiagPrint(DIAG_LEVELALL, NULL, "Invalid size value\n");
        return FAILURE;
    }

    file = fopen(fileName, "wb");
    if (file == NULL)
    {
        DiagPrint(DIAG_LEVELALL, "ERROR", "Unable to create file [%s]\n", fileName);
        return FAILURE;
    }
    virtAddr = IcpuMapIo((PTR) start, count);
    if (virtAddr == NULL)
    {
        DiagPrint(DIAG_LEVELALL, "ERROR", "Unable to map address [%p:%u]\n", (PTR) start, count);
        fclose(file);
        return FAILURE;
    }
    fwrite(virtAddr, count, 1, file);
    fclose(file);
    IcpuUnmapIo((PTR) start);
    DiagPrint(DIAG_LEVELALL, NULL, "Store completed [%p-->%p;%X]\n", (PTR) start, virtAddr, count);

    return SUCCESS;
}

/** Stores DL I/Q samples buffer in a disk file in WaveJudge format
 *  Default start address is 0x0A800080 and size is
 *
 * @param buffer Command parameters (console input)
 * @return Standard result code
 */
RESULTCODE CmdWaveJudgeDl(char *buffer)
{
    extern PTR IcpuMapIo(PTR addr, U32 len);
    extern void IcpuUnmapIo(PTR addr);

    int start, count;
    U32* virtAddr;
    char *indexStr;
    int index;

    ReadSettingInt("iqdladdr", &start, 0x0A800080);
    ReadSettingInt("iqdlsize", &count, 4915200);

    // Continue token search
    indexStr = strtok(NULL, "\n");
    if (indexStr == NULL)
    {
        index = 0;
    }
    else
        index = strtoul(indexStr, NULL, 0);

    virtAddr = IcpuMapIo((PTR) start, count);
    if (virtAddr == NULL)
    {
        DiagPrint(DIAG_LEVELALL, "ERROR", "Unable to map address [%p:%u]\n", (PTR) start, count);
        return FAILURE;
    }
    DiagPrint(DIAG_LEVELALL, NULL, "Saving DL I/Q in WJ format [Addr=%p,VA=%p,Size=%i]... ",
              start, virtAddr, count);
    DiagDumpIqSamples(index, 0, IQ_FMT_WJ, count >> 2, virtAddr);
    IcpuUnmapIo((PTR) start);
    DiagPrint(DIAG_LEVELALL, NULL, "Done\n");

    return SUCCESS;
}

/** Stores UL I/Q samples buffer in a disk file in WaveJudge format
 *  Default start address is 0x0A800080 and size is
 *
 * @param buffer Command parameters (console input)
 * @return Standard result code
 */
RESULTCODE CmdWaveJudgeUl(char *buffer)
{
    extern PTR IcpuMapIo(PTR addr, U32 len);
    extern void IcpuUnmapIo(PTR addr);

    int start, count;
    U32* virtAddr;
    char *indexStr;
    int index;

    ReadSettingInt("iquladdr", &start, 0x0ACB0080);
    ReadSettingInt("iqulsize", &count, 4915200);

    // Continue token search
    indexStr = strtok(NULL, "\n");
    if (indexStr == NULL)
    {
        index = 0;
    }
    else
        index = strtoul(indexStr, NULL, 0);

    virtAddr = IcpuMapIo((PTR) start, count);
    if (virtAddr == NULL)
    {
        DiagPrint(DIAG_LEVELALL, "ERROR", "Unable to map address [%p:%u]\n", (PTR) start, count);
        return FAILURE;
    }
    DiagPrint(DIAG_LEVELALL, NULL, "Saving UL I/Q in WJ format [Addr=%p,VA=%p,Size=%i]... ",
              start, virtAddr, count);
    DiagDumpIqSamples(index, 0, IQ_FMT_WJ, count >> 2, virtAddr);
    IcpuUnmapIo((PTR) start);
    DiagPrint(DIAG_LEVELALL, NULL, "Done\n");

    return SUCCESS;
}
#endif /* C4KEVM */
