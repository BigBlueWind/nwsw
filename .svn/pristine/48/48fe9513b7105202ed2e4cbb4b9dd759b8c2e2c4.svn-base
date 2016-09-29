/** @file svsr.c
 *
 * @brief Supervisor message handling
 * @author Mindspeed Technologies
 * @version $Revision: 1.75 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#include "osal.h"
#include "svsr.h"
#include "apidefs.h"
#include "msgroute.h"
#include "diags.h"
#include "maciface.h"
#include "packetif.h"
#include "phyiface.h"
#include "icpu.h"
#if defined (ENODEB) || defined (UE) || defined (EHDMAC)
#include "rrciface.h"
#endif /* ENODEB || UE || EHDMAC */
#ifdef UE
#include "rlciface.h"
#endif
#include "settings.h"
#ifdef USE_LINUX
#include "icpu.h"
#endif

//
// Supervisor API maps
//

SVSR_APIMSG_STATS SvsrApiMsgStats = {0};

//
// SYS messages
//
CMD_MAP SvsrSysHandler[] =
{
    // Range 0x0001 - 0x0010
    {SVSR_LOADLARM,         SvsrLoadLarm},
    {SVSR_LOADDSP,          SvsrLoadDsp},
    {SVSR_LOADUARM,         SvsrLoadUarm},
    {SVSR_RESET,            SvsrReset},
    {SVSR_IOPARAMS,         NULL},
    {SVSR_UNLOAD,           SvsrUnload},
    {SVSR_APIMSGSTATS,           SvsrApiMsgStatsHandler},
    {0,                     NULL}           // Always last entry
};

//
// MAC messages
//
CMD_MAP SvsrRrcHandler[] =
{
    // Range 0x7000 - 0x7FFF
#if defined (ENODEB) || defined (UE) || defined (EHDMAC)
    {SVSR_CREATERRC,        SvsrCreateRrc},
    {SVSR_DESTROYRRC,       SvsrDestroyRrc},
#endif /* ENODEB || UE || EHDMAC */
    {0,                     NULL}           // Always last entry
};

//
// MAC messages
//
CMD_MAP SvsrMacHandler[] =
{
    // Range 0x4000 - 0x4FFF
    {SVSR_CREATEMAC,        SvsrCreateMac},
    {SVSR_DESTROYMAC,       SvsrDestroyMac},
    {SVSR_CREATEPHY,        SvsrCreatePhy},
    {SVSR_DESTROYPHY,       SvsrDestroyPhy},
    {SVSR_PHYTICK,          SvsrPhyTick},
    {0,                     NULL}           // Always last entry
};

//
// PHY messages
//
CMD_MAP SvsrPhyHandler[] =
{
    // Range 0x5000 - 0x5FFF
    {0,                     NULL}           // Always last entry
};

//
// VOIP messages
//
CMD_MAP SvsrVoipHandler[] =
{
    // Range 0x6000 - 0x6FFF
    {0,                     NULL}           // Always last entry
};

//
// Debug messages
//
CMD_MAP SvsrDebugHandler[] =
{
    // Range 0xD000 - 0xDFFF
    {DIAG_CREATETASK,       DiagCreateTask},
    {DIAG_DESTROYTASK,      DiagDestroyTask},
    {DIAG_CREATESEMA,       DiagCreateSema},
    {DIAG_DESTROYSEMA,      DiagDestroySema},
    {DIAG_CREATEMUTEX,      DiagCreateMutex},
    {DIAG_DESTROYMUTEX,     DiagDestroyMutex},
    {0xD006,                NULL},
    {0xD007,                NULL},
    {0xD008,                NULL},
    {0xD009,                NULL},
    {0xD00A,                NULL},
    {0xD00B,                NULL},
    {0xD00C,                NULL},
    {0xD00D,                NULL},
    {DIAG_ECHO,             DiagEcho},
    {0,                     NULL}           // Always last entry
};

#ifdef UE
CMD_MAP SvsrUeMmiHandler[] =
{
    {MMI_UT_SYSTEM,	SvsrUeMmi}
};
#endif
//
// Debug messages
//
CMD_MAP SvsrDiagHandler[] =
{
    // Range 0xF000 - 0xFFFF
    {0,                     NULL}           // Always last entry
};
CMD_MAP SvsrLarmHDiagHandler[] =
{
    // Range 0xF900 - 0xF9FF
    {0xF900,            NULL},
    {0xF901,            NULL},
    {0xF902,            NULL},
    {0xF903,            NULL},
    {0xF904,            NULL},
    {0xF905,            NULL},
    {0xF906,            NULL},
    {0xF907,            NULL},
    {0xF908,            SvsrLarmHDiagCevaDump},
    {0xF909,            NULL},
    {0,                     NULL}           // Always last entry
};

//
// MAC Instances
//
U16 MacInstances[SVSR_MAXMACINST];

//
// PHY Instances
//
U16 PhyInstances[SVSR_MAXPHYINST];

#if defined (USE_LINUX) && defined (C4KEVM)
char PathPrefix[PATH_MAX] = "/tftpboot";
#endif

#ifdef C4KEVM
CevaDumpCtx dumpinfo[10];
#endif

/** Initialize Supervisor
 *
 * @return Standard result code
 */
RESULTCODE SvsrInit(void)
{
    int i;

    for (i = 0; i < SVSR_MAXMACINST; i++)
    {
        MacInstances[i] = 0;
    }

    for (i = 0; i < SVSR_MAXPHYINST; i++)
    {
        PhyInstances[i] = 0;
    }

    memset(&SvsrApiMsgStats, 0, sizeof(SvsrApiMsgStats));

    return SUCCESS;
}

/** Cleanup Supervisor structures
 *
 * @return Standard result code
 */
RESULTCODE SvsrDone(void)
{
    return SUCCESS;
}

/** Dispatches Supervisor messages
 *
 * @param size
 * @param msg
 * @return Standard result code
 */
RESULTCODE SvsrDispatch(U32 size, PTR msg)
{
    PMSGHEADER pHdr = (PMSGHEADER) msg;
    CMD_MAP *mapPtr = NULL;
    U16 newID = pHdr->msgID;
    RESULTCODE ret = SUCCESS;

    //
    // Check API range
    //

    if (pHdr->msgID >= SYS_FIRST && pHdr->msgID <= SYS_LAST)
    {
        // SYS range
        newID = pHdr->msgID - SYS_FIRST;
        mapPtr = SvsrSysHandler;
    }
    else if (pHdr->msgID >= MAC_FIRST && pHdr->msgID <= MAC_LAST)
    {
        // MAC range
        newID = pHdr->msgID - MAC_FIRST;
        mapPtr = SvsrMacHandler;
    }
    else if (pHdr->msgID >= PHY_FIRST && pHdr->msgID <= PHY_LAST)
    {
        // PHY range
        newID = pHdr->msgID - PHY_FIRST;
        mapPtr = SvsrPhyHandler;
    }
#if defined (ENODEB) || defined (UE) || defined (EHDMAC)
    else if (pHdr->msgID >= RRC_FIRST && pHdr->msgID <= RRC_LAST)
    {
        // RRC range
        newID = pHdr->msgID - RRC_FIRST;
        mapPtr = SvsrRrcHandler;
    }
#endif /* ENODEB || UE || EHDMAC */
    else if (pHdr->msgID >= VOIP_FIRST && pHdr->msgID <= VOIP_LAST)
    {
        // VOIP range
        newID = pHdr->msgID - VOIP_FIRST;
        mapPtr = SvsrVoipHandler;
    }
    else if (pHdr->msgID >= DBG_FIRST && pHdr->msgID <= DBG_LAST)
    {
        // DBG range
        newID = pHdr->msgID - DBG_FIRST;
        mapPtr = SvsrDebugHandler;
    }
    else if (pHdr->msgID >= DIAG_FIRST && pHdr->msgID <= DIAG_LAST)
    {
        // DIAG range
        newID = pHdr->msgID - DBG_FIRST;
        mapPtr = SvsrDiagHandler;
    }
    else if (pHdr->msgID >= LARM_HDIAG_FIRST && pHdr->msgID <= LARM_HDIAG_LAST)
    {
        // LARM-HDIAG range
        newID = pHdr->msgID - LARM_HDIAG_FIRST;
        mapPtr = SvsrLarmHDiagHandler;
    }
#ifdef UE
	else if (pHdr->msgID >= MMI_FIRST && pHdr->msgID <= MMI_LAST)
	{
		newID = 0;
		mapPtr = SvsrUeMmiHandler;
	}
#endif
    //
    // Check if handler exist
    //
    if (mapPtr == NULL || mapPtr[newID].cmdHandler == NULL)
    {
        ret = FAILURE;
    }
    else
    {
        ret = mapPtr[newID].cmdHandler(msg);
    }

    MsgFreeBuffer(msg);

    return ret;
}

/** Handle CreateMac message
 *
 * @param msg
 * @return Standard result code
 */
RESULTCODE SvsrCreateMac(PTR msg)
{
    PMSGHEADER pHdr = (PMSGHEADER) msg;
    PMSGHEADER pAns;
    RESULTCODE ret = SUCCESS;
    U16 instID = 0;

#if defined(ENODEB) || defined(UE)
    PhyInit();
#endif

    ret |= MacSetInitConfig(msg);
    ret |= MacInit(PhySendMsg, PhyGetMsg, pIMSG_TO_PHY, pIMSG_FROM_PHY);
#ifdef MSMAC
    instID = MSGS_MSMAC;
    // FIXME: remove when debugging ends
    AppInit(&OsalGlobalHeap);
#else
    instID = MSGS_MAC;
#endif
    if (ret == SUCCESS)
    {
        pAns = MsgInitReply(pHdr);
        if (pAns)
        {
            MsgResult(pAns, SUCCESS);
            MsgAppendU32(pAns, PAR_INSTID, instID);
            MsgDispatch(MSGS_SVSR, sizeof(MSGHEADER) + pAns->length, pAns);
        }
        else
        {
            ret = FAILURE;
        }
    }
    else
    {
        MsgSimpleReply(pHdr, ret);
    }
    return ret;
}

#if defined(ENODEB) || defined(UE) || defined(EHDMAC)
/** Handle CreateRrc message
 *
 * @param msg
 * @return Standard result code
 */
RESULTCODE SvsrCreateRrc(PTR msg)
{
    PMSGHEADER pHdr = (PMSGHEADER) msg;
    PMSGHEADER pAns;
    RESULTCODE ret;

    if ((ret = RrcInit(NULL, NULL)) == SUCCESS)
    {
        pAns = MsgInitReply(pHdr);
        if (pAns)
        {
            MsgResult(pAns, SUCCESS);
            // TODO: Use instance ID
            MsgAppendU32(pAns, PAR_INSTID, MSGS_RRC);
            ret = MsgDispatch(MSGS_SVSR, sizeof(MSGHEADER) + pAns->length, pAns);
        }
        else
        {
            ret = FAILURE;
        }
    }
    else
    {
        MsgSimpleReply(pHdr, ret);
    }
    return ret;
}

/** Handle DestroyRrc message
 *
 * @param msg
 * @return Standard result code
 */
RESULTCODE SvsrDestroyRrc(PTR msg)
{
    RESULTCODE ret = SUCCESS;

#if defined (ENODEB) || defined (UE)
    MS_Close();
#endif

    return ret;
}
#endif /* ENODEB || UE */

/** Handle CreatePhy message
 *
 * @param msg
 * @return Standard result code
 */
RESULTCODE SvsrCreatePhy(PTR msg)
{
    PMSGHEADER pHdr = (PMSGHEADER) msg;
    PMSGHEADER pRsp;
    PPARAMHEADER pParam = MsgGetNextParam(pHdr, NULL);
    RESULTCODE ret = SUCCESS;
    U16 instID = 0;

    while (pParam)
    {
        switch (pParam->paramID)
        {
            case 0:                 // End of list
                pParam = NULL;      // Stop
                continue;
            case PAR_DOMAIN:
                break;
            case PAR_TYPE:
                break;
            default:                // Unknown parameter received
                pParam = NULL;      // Stop
                continue;
        }
        pParam = MsgGetNextParam(pHdr, pParam);
    }

    // TODO: Search for free slot

    if (PhyEnable(&instID) != SUCCESS)
    {
        ret = FAILURE;
        MsgSimpleReply(msg, ret);
    }
    else
    {
        // Create response
        if ((pRsp = MsgInitReply(msg)) != NULL)
        {
            MsgResult(pRsp, ret);
            MsgAppendU32(pRsp, PAR_INSTID, MSGS_LARM | instID);
            ret = MsgDispatch(MSGS_SVSR, pRsp->length + sizeof(MSGHEADER), pRsp);
        }
        else
        {
            ret = FAILURE;
        }
    }

    return ret;
}

#if defined(UE)
/**
 *
 * @param msg
 * @return
 */
RESULTCODE SvsrUeMmi(PTR msg)
{
    PMSGHEADER pHdr = (PMSGHEADER) msg;
    PMSGHEADER pAns;
	PPARAMHEADER param = (PPARAMHEADER)pHdr->param;
	PPARAMHEADER pAnsPar = NULL;
	RESULTCODE ret = SUCCESS;

	switch(param->paramID)
	{
	case MMI_SWITCH_ON:
		RrcConnectionRequest();
		break;
	case MMI_SWITCH_OFF:
		break;
	case MMI_POWER_ON:
#if defined(_USRDLL)
	    ret = RrcInit(NULL,NULL);
	    ret |= MacInit(PhySendMsg, PhyGetMsg, pIMSG_TO_PHY, pIMSG_FROM_PHY) << 4;
        ret |= RlcInit();
        ret |= PhyInit() << 8;
        ret |= PhyEnable(0) <<12;
#endif
		break;
	case MMI_POWER_OFF:
		break;
	default:
		printf("MMI ERROR: unsupported message %x\n", param->paramID);
		ret = FAILURE;
		break;
	}
    if ((pAns = MsgInitReply(pHdr)) != NULL)
    {
        pAnsPar = (PPARAMHEADER)pAns->param;
        pAnsPar->paramID = param->paramID;
        pAnsPar->length = TYPE_U32;
        pAnsPar->data[0] = ret;
        pAns->length += sizeof(PARAMHEADER) + pAnsPar->length;
        MsgDispatch(MSGS_SVSR, sizeof(MSGHEADER) + pAns->length, pAns);
    }
    else
    {
        ret = FAILURE;
    }
	return ret;
}
#endif /*UE*/

/** Handle DestroyMac message
 *
 * @param msg
 * @return Standard result code
 */
RESULTCODE SvsrDestroyMac(PTR msg)
{
    RESULTCODE ret;

    // Destroy MAC instance
    ret = MacDone();
    MsgSimpleReply(msg, ret);

    return ret;
}

/** Handle CreateMac message
 *
 * @param msg
 * @return Standard result code
 */
RESULTCODE SvsrDestroyPhy(PTR msg)
{
    RESULTCODE ret;

    // What should we do here?
    ret = PhyDone();
    MsgSimpleReply(msg, ret);
    return SUCCESS;
}

/** Send SVSR_READY message
 *
 * @return Standard result code
 */
RESULTCODE SvsrReady(void)
{
    extern const int Domain4G;
    extern const int Mac4G;

    return MsgIndication(MSGS_SVSR, MSGS_HOST, SVSR_LOADUARM, 2,
                         PAR_DOMAIN, TYPE_U32, Domain4G,
                         PAR_TYPE, TYPE_U32, Mac4G);
}

/** Handle LoadLarm message
 *  This is for demonstration purposes
 *
 * @param msg
 * @return Standard result code
 */
RESULTCODE SvsrLoadLarm(PTR msg)
{
    PPARAMHEADER param;
    RESULTCODE ret = SUCCESS;
    char *path;

    param = MsgGetParam(msg, PAR_PATH);
    if (param)
    {
        path = (char *) &param->data[0];

#if defined (USE_LINUX) && defined (C4KEVM)
        {
            int fileId = rand();
            char buffer[PATH_MAX];
            char *template;
            int status = 0;

            // Stop previous I-CPU activity
            IcpuDone();

            // Ensure we are ROOT
            status = setuid(0);

            // Copy new image to the predefined location
            if (status == 0)
            {
                char *template = "cp -f %s/%s /lib/firmware/lowarm%i.elf";

                if (strlen(path) + strlen(PathPrefix) + 1 < PATH_MAX - strlen(template))
                {
                    // Remove armldr module from kernel
                    status = system("rmmod armldr");     // Don't care about status
                    DiagPrint(DIAG_LEVEL6, NULL, "'rmmod armldr' done, status is [%i]\n", status);

                    sprintf(buffer, template, PathPrefix, path, fileId);
                    status = system(buffer);
                    DiagPrint(DIAG_LEVEL6, NULL, "'%s' done, status is [%i]\n", buffer, status);
                }
            }
            else
            {
                DiagPrint(DIAG_LEVEL2, "WARNING", "Not a root user [%i]\n", status);
            }

            // Reload I-CPU module
            if (status == 0)
            {
                // TODO: Change coreid for C4KEVM
                template = "modprobe armldr coreid=4 noipi=0 fileid=%i diag=1";
                sprintf(buffer, template, fileId);
                status = system(buffer);
                DiagPrint(DIAG_LEVEL6, NULL, "'%s' done, status is [%i]\n", buffer, status);
            }

            // Re-create I-CPU device node
            if (status == 0)
            {
                template = "rm -f /dev/lowarm";
                // Remove old device node
                status = system(template);
                DiagPrint(DIAG_LEVEL6, NULL, "'%s' done, status is [%i]\n", template, status);
                // Create new device node
                template = "mknod /dev/lowarm c 243 0";
                status = system(template);
                DiagPrint(DIAG_LEVEL6, NULL, "'%s' done, status is [%i]\n", template, status);
            }
            if (status != 0)
            {
                ret = errno;
            }

            // Remove temporary file
            sprintf(buffer, "rm -f /lib/firmware/lowarm%i.elf", fileId);
            status = system(buffer);
            DiagPrint(DIAG_LEVEL6, NULL, "'%s' done, status is [%i]\n", buffer, status);
            // Restart I-CPU driver
            IcpuInit(UARM);
        }

#endif /* USE_LINUX && C4KFPGA || C4KEVM */

#if defined (USE_WINDOWS) || defined (SIMUPC)
        // This is sending of SVSR_READY on behalf of L-ARM
        // This is debug code
        // May be needed in simulation environments
        MsgIndication(MSGS_LARM | MSGS_SVSR,
                      MSGS_HOST,
                      LARM_SVSR_READY,
                      1,
                      PAR_RESULT, TYPE_U32, ret);
#endif /* USE_WINDOWS || SIMUPC */

#ifdef C4K_EVM
        memset(&dumpinfo, 0, sizeof(dumpinfo));
#endif

    }
    MsgSimpleReply(msg, ret);
    return ret;
}

/** Handle LoadDsp message
 *  This is for demonstration purposes
 *
 * @param msg
 * @return Standard result code
 */
RESULTCODE SvsrLoadDsp(PTR msg)
{
    RESULTCODE ret = FAILURE;
    PPARAMHEADER param;
    char *path;

    param = MsgGetParam(msg, PAR_PATH);
    if (param)
    {
        path = (char *) &param->data[0];
#if defined (USE_LINUX) && defined (C4KEVM)
        {
            int status;
            char buffer[PATH_MAX];
            char *template = "cp -f %s/%s /lib/firmware/";

            // Ensure we are ROOT
            status = setuid(0);

            // Copy DSP firmware to a predefined location
            if (status == 0)
            {
                if (strlen(path) + strlen(template) + strlen(PathPrefix) + 1 < PATH_MAX)
                {
                    system("rmmod cevaboot");   // Don't care about status

                    sprintf(buffer, template, PathPrefix, path);
                    status = system(buffer);
                    printf(C4KFW "%s done, status is [%i]\n", buffer, status);
                }
                else
                {
                    printf(C4KFW "WARNING: Module path is invalid\n");
                    status = -1;
                }
            }
            else
            {
                printf(C4KFW "ERROR: Not a root user [%i]\n", status);
            }

            // Insert cevaboot module
            if (status == 0)
            {
                // TODO: Check dsp_mask for C4KEVM
                template = "modprobe cevaboot dsp_mask=1";
                status = system(template);
                printf(C4KFW "%s done, status is [%i]\n", template, status);
            }

            // Write image name
            if (status == 0)
            {
                char *name = basename(path);
                FILE *sysfs = fopen("/sys/bus/platform/devices/ceva-dsp/image_name", "w");
                if (sysfs)
                {
                    fwrite(name, strlen(name), 1, sysfs);
                    printf(C4KFW "image_name registered: %s\n", name);
                    fclose(sysfs);
                }
                else
                {
                    printf(C4KFW "Unable to open sysfs for writing [%i]\n", errno);
                }
            }

            // Read execution status
            if (status == 0)
            {
                FILE *sysfs = fopen("/sys/bus/platform/devices/ceva-dsp/boot_status", "r");
                if (sysfs)
                {
                    if ((status = fread(buffer, sizeof(buffer), 1, sysfs)) > 0)
                    {
                        buffer[status] = '\0';
                        printf(C4KFW "boot_status read: %s\n", buffer);
                        ret = atoi(buffer);
                    }
                    fclose(sysfs);
                }
                else
                {
                    printf(C4KFW "Unable to open sysfs for reading [%i]\n", errno);
                }
            }
        }
#else  /* USE_LINUX && C4KEVM */
        ret = SUCCESS;
#endif /* USE_LINUX && C4KEVM */
    }
    else
    {
        // FIXME: Debug
        printf("DSP firmware path is missing\n");
        ret = FAILURE;
    }

    MsgSimpleReply(msg, ret);
    return ret;
}

/** Handle LoadUarm message
 *  This is for demonstration purposes
 *
 * @param msg
 * @return Standard result code
 */
RESULTCODE SvsrLoadUarm(PTR msg)
{
    // TODO: This U-ARM is already loaded.
    // Shall we return ERROR or SUCCESS ?
    PPARAMHEADER param;
    RESULTCODE ret = SUCCESS;
    char *path;

    param = MsgGetParam(msg, PAR_PATH);
    if (param)
    {
        path = (char *) &param->data[0];
        ret = SUCCESS;
    }
    // Acknowledge
    MsgSimpleReply(msg, ret);
    // Indication 
    SvsrReady();

    return ret;
}

/** Handle Reset message
 *
 * @param msg
 * @return Standard result code
 */
RESULTCODE SvsrReset(PTR msg)
{
    // TODO: Shall we reply at all?
    MsgSimpleReply(msg, SUCCESS);
    OsalWaitDelay(1000000);
#ifdef USE_LINUX
    syscall(SYS_reboot, LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2, LINUX_REBOOT_CMD_RESTART, NULL);
//  reboot(RB_AUTOBOOT);
#endif /* USE_LINUX */
#ifdef USE_WINDOWS
#if _MSC_VER > 1200
    ExitWindowsEx(EWX_REBOOT | EWX_FORCEIFHUNG, SHTDN_REASON_MAJOR_OTHER);
#else
    ExitWindowsEx(EWX_REBOOT | EWX_FORCEIFHUNG, 0);
#endif
#endif /* USE_WINDOWS */
    return SUCCESS;
}

/** Start PHY process
 *
 * @param msg
 * @return Standard result code
 */
RESULTCODE SvsrPhyTick(PTR msg)
{
    extern OSAL_SEMAPHORE PhyTickSema;
    RESULTCODE ret;

    ret = OsalRaiseSema(&PhyTickSema);

    // Create response
    return MsgSimpleReply(msg, ret);
}

/** Handle Unload message
 *
 * @param msg
 * @return Standard result code
 */
RESULTCODE SvsrUnload(PTR msg)
{
    extern void NullTaskStop(void);

    PPARAMHEADER param;
    char *keyword;

    param = MsgGetParam(msg, PAR_PATH);
    if (param)
    {
        keyword = (char *) &param->data[0];
        if (strcmp(keyword, "STOP") == 0)
        {
            MsgSimpleReply(msg, SUCCESS);
            NullTaskStop();
            return SUCCESS;
        }
    }

    MsgSimpleReply(msg, FAILURE);
    return FAILURE;
}


/** Handle API messages statistics query message
 *
 * @param msg
 * @return Standard result code
 */
RESULTCODE SvsrApiMsgStatsHandler(PTR msg)
{
    PMSGHEADER pHdr;
    pHdr = MsgInitReply((PMSGHEADER)msg);

    if(pHdr)
    {
        U32 paramID = PAR_SVSR_APIMSGSTATS_START;
        U32* pData = (U32*)&SvsrApiMsgStats;
        U32 Num = sizeof(SvsrApiMsgStats) / sizeof(U32);

        while(Num--)
        {
            MsgAppendU32(pHdr, paramID, *pData);
            paramID++;
            pData++;
        }
        MsgDispatch(MSGS_SVSR, sizeof(MSGHEADER) + pHdr->length, pHdr);
    }

    return SUCCESS;
}

/** Create configuration command
 *
 * @param srcID ID of command source
 * @param msgID Command ID
 * @param paramCount Number of parameters
 * @return
 */
RESULTCODE SvsrCommand(U16 srcID, U16 msgID, U32 paramCount, ...)
{
    va_list paramList;
    PMSGHEADER pHdr;

    if ((pHdr = MsgAllocBuffer(MSG_COMMON)) == NULL)
    {
        return FAILURE;
    }

    MsgInitMessage(pHdr);
    pHdr->control = CTRL_VERSION;
    pHdr->type = MSGT_CONFIG;
    pHdr->srcID = srcID;
    pHdr->dstID = MSGS_SVSR;
    pHdr->msgID = msgID;

    va_start(paramList, paramCount);
    while (paramCount)
    {
        U32 paramID = va_arg(paramList, U32);
        U32 paramType = va_arg(paramList, U32);
        switch (paramType)
        {
        case TYPE_U8:
            {
                int val = va_arg(paramList, int);
                MsgAppendU8(pHdr, paramID, val);
            }
            break;
        case TYPE_U16:
            {
                int val = va_arg(paramList, int);
                MsgAppendU16(pHdr, paramID, val);
            }
            break;
        case TYPE_U32:
            {
                int val = va_arg(paramList, int);
                MsgAppendU32(pHdr, paramID, val);
            }
            break;
        case TYPE_STRING:
            {
                char *val = va_arg(paramList, char *);
                int len = strlen(val);
                MsgAppendParam(pHdr, paramID, len + 1, val);
            }
            break;
        case TYPE_BINARY:
            {
                int len = va_arg(paramList, int);
                void *val = va_arg(paramList, void *);
                MsgAppendParam(pHdr, paramID, len, val);
            }
            break;
        default:
            return FAILURE;
        }
        paramCount--;
    }
    va_end(paramList);

    return SvsrDispatch(pHdr->length + sizeof(MSGHEADER), pHdr);
}

/** Process LARM answer on LARM_HDIAG_CEVADUMP (console command used)
 *
 * @param msg
 * @return Standard result code
 */
RESULTCODE SvsrLarmHDiagCevaDump(PTR msg)
{
    PPARAMHEADER param = NULL;

#ifdef C4KEVM
    while ((param = MsgGetNextParam(msg, param)) != NULL)
    {
        if(param->paramID == 4)
        {
            CevaDumpCtx* pParamData;
            U32 CevaID;

            pParamData = (CevaDumpCtx*) &param->data[0];
            CevaID = pParamData->CevaID;
            dumpinfo[CevaID].CevaID = CevaID;
            dumpinfo[CevaID].pDumpBuf = pParamData->pDumpBuf;
            dumpinfo[CevaID].DumpSize = pParamData->DumpSize;
            dumpinfo[CevaID].DumpBufSize = pParamData->DumpBufSize;
            printf("Got params for Ceva%d dump [pBuf=0x%X DumpSize=%u]\n",
                CevaID, (U32)pParamData->pDumpBuf, pParamData->DumpSize);
        }
    }
#endif
    
    return SUCCESS;
}

