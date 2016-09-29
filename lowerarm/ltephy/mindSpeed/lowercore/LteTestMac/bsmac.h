/** @file BSMAC.h
 *
 * @brief Check API processing logic
 * @author Mindspeed Technologies
 * @version $Revision: 1.26 $
 *
 * COPYRIGHT(c) 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef _BSMAC_H
#define _BSMAC_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "ltemaccore.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/****************************************************************************
 * Exported Types
 ****************************************************************************/

typedef enum eSchedulingSchema
{
// TODO: Find "magic number" and change it to FDX_SCHEMA
#ifndef MX_ARM_ENABLED
    FDX_SCHEMA,
#endif
    MAIN_SCHEMA,
    RCT_SCHEMA
} SCHEDULING_SCHEMA;

typedef enum eHarqSchema
{
    HARQ_OFF,
    HARQ_ON
} HARQ_SCHEMA;

typedef struct sRxSubFrmList
{
	struct sRxSubFrmList *next;
	struct sRxSubFrmList *prev;
	PULSUBFRDESC pUlSubFr;
} RxSubFrmList, *PRxSubFrmList;
typedef struct ePhyTest
{
    U32    TrDelay;
    U32    CorruptionApi;
} PHY_TEST;


typedef struct sBsContext
{
    //COMMON_DESC MacCommon;
    INITPARM initParm;
    MacPhyCommState TxState;
    MacPhyCommState RxState;
    Mac2PhyInterface Phy;
    SCHEDULING_SCHEMA schedulingSchema;
    // UE related
    U16 NUe;
    UE_DESC* pUe[MAX_UE_COUNT];
    PHY_AUTORUN_CTX PhyAutoCtx;
    U8 SpTdd;
    U8 isMIBconfigured;
    PRxSubFrmList UlHead;
    PRxSubFrmList UlTail;
    U32 RxSubFrmCount;
    HARQ_SCHEMA HarqEna;
    //Exported from COMMON_DESC
    U32 frameNumber;
    U32 subFrameNumber;
    U32 globalTtiCount;
    U32 sduCount;
    U32 errorCount;
    U32 ChanBW;
    U32 CpType;
    U32 InstanceID;
    U32 num_IQSamps;
    U32 RxNumBytes[2];
    U8 DirMode;
    // debug
    TSTPDESC tstpbsDl;
    TSTPDESC tstpbsUl;
    U32 predefConfig;
    PHY_TEST PhyTest; 
} BS_CONTEXT;

typedef enum ePhyAutoSwitch
{
    PhyAutoStart = 0,
    PhyAutoStop = 1,
    PhyAutoShutdown = 2
} PhyAutoSwitch;

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

extern BS_CONTEXT *pBsMacDesc;
extern DLSUBFRDESC DlSubFrame;
extern ULSUBFRDESC UlSubFrame;
extern V32 globalTTITickCount;
#ifndef MX_ARM_ENABLED
extern MAC_PDU_DESC MacPduDesc[];
#endif

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

#ifdef MX_ARM_ENABLED
RESULTCODE eNbMacAutoRun(PhyAutoSwitch AutoSw, U32 PhyInstanceId);
RESULTCODE eNbMacInit(U16 instanceID, MAC_INIT_CONFIG *pMacInitConfig, PhyTxCb txCb, PhyRxCb rxCb,
                      PTR toPhy, PTR fromPhy);
RESULTCODE eNbMacDestroy(void);
#else
RESULTCODE LteMacAutoRun(PhyAutoSwitch AutoSw, U32 PhyInstanceId);
RESULTCODE LteMacAutoRunNmm(PhyAutoSwitch AutoSw, U32* Params);
RESULTCODE LteMacNmmCmd(U32 cmdId);

UE_DESC* LteMacNewUeConnected(MacRlcIoFunc LteMacGetDlDataBlock,
                              MacRlcIoFunc  LteMacPutUlDataBlock);
UE_DESC* GetUeContext(U32 UeId);
#endif

#endif /* _BSMAC_H */
