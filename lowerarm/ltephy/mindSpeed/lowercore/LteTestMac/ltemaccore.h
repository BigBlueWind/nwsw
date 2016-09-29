/** @file ltemaccore.h
 *
 * @brief This file defines the structures, constants and variables
 *        used in the implementation of an LTE MAC
 * @author Mindspeed Technologies
 * @version $Revision: 1.13 $
 *
 * COPYRIGHT(c) 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef LTEMACCORE_H
#define LTEMACCORE_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "resultcodes.h"

#ifdef MX_ARM_ENABLED
#include "4gmx_compiler.h"
#include "enbMacIface.h"
#else
#include "osal.h"
#include "simu_util.h"
#include "maciface.h"
#endif

#ifndef AC_MSPD_TDD
#include "LtePhyL2Api.h"
#else
#include "LtePhyL2Api_TDD.h"
#endif

#ifdef MX_ARM_ENABLED
#include "appprintf.h"
#endif

#include "LTE_test_config.h"
#include "dci.h"

#ifndef MAX
#define MAX(x, y) ((x) >= (y) ? (x) : (y))
#endif

#ifndef MIN
#define MIN(x, y) ((x) <= (y) ? (x) : (y))
#endif

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/* LCID for both DL-SCH and UL-SCH */
#define MAC_CCCH_LCID               0x00        // CCCH
#define MAC_DCCH_LCID               0x01        // CCCH

#define LCID_MIN_LCHAN              0x02        //DTCH
#define LCID_MAX_LCHAN              0x0A
#define LCID_PADDING                0x1F        // Padding

/* UL-SCH only */
#define LCID_POWER                  0x1A        // Power headroom report
#define LCID_C_RNTI                 0x1B        // C-RNTI
#define LCID_TBSR                   0x1C        // Truncated BSR
#define LCID_SBSR                   0x1D        // Short BSR
#define LCID_LBSR                   0x1E        // Long BSR

/* DL-SCH only */
#define LCID_UECONT                 0x1C        // UE contention resolution
#define LCID_ADVANCE                0x1D        // Timing advance
#define LCID_DRX                    0x1E        // DRX command

#define MAX_CONNECTION              600
#define MAX_MAC_PDU_SIZE            9421
#define MAX_MAC_SDU_SIZE            9421
#define MAC_PDU_NUMBER              (1024)

#define MAX_SDU_BLOCKS              50

#define CP_NORMAL                   0
#define CP_EXTENDED                 1


// Array for IQ samples for a subFrame calculated as (assuming 20 MHz BW)
// (2048 + 160)*2 + (2048 + 144)*2 = 30720 Samples
// The In/Out samples are stored as pairs of fixed point 16 bits, two's
// complement notation, so we need 4*30720 bytes or 122880 bytes for
// each case
#define LTEIQMAX 122880
#define MAX_LTEIQ  2*31720*4  // 4 antennas, 31720 samples, 4 bytes per sample
#define PHY_IQ_CHUNK                (12*1024) 	// = 122880

#ifdef WIN32
/* Under Win32 such name already exist */
#define PCCH PCCH_
#endif

#define DEFAULT_REF_SIG_BOOST       0
#define DEFAULT_NUM_CTL_SYMBOLS     3
#define DEFAULT_ANT_PORT_COUNT      4
#define ONE_ANT_PORT_COUNT          1
#define DEFAULT_PHICH_RESOURCE      2
#define DEFAULT_PHICH_DURATION      1

#define PERSIST_PERIOD_PERMANENT    0xFF
#define PERSIST_PERIOD_CURRENT      0

#define MAX_PHY_CHAN                16

/* Actual header length is 2 bytes */
#define MAC_SDU_HDR_SIZE_SHORT      2

/* MAC header for MAC SDU with 15-bit length (DL-SCH, UL-SCH) */
/* Actual header length is 3 bytes */
#define MAC_SDU_HDR_SIZE_LONG       3

// Actual header length is 1 byte
#define MAC_CTRL_HDR_SIZE           1

#define MAX_UE_COUNT                1

#define MAX_MCS_INDEX               29
#define MAX_IBS_INDEX               27
#define MAX_RESOURCE_BLOCK          110

#define PBCH_SIZE_ONE_SUBFRAME_BITS (24)
//#define PBCH_SIZE_ONE_SUBFRAME_BITS_DBG (480)

#define CCCH_MCS_INDEX              0

#define PRINT_FAILURE_POINT printf("Error in funtion %s (%s:%d)% \n", \
                                   __FUNCTION__, __FILE__, __LINE__)

/* Max number of CCE per Subframe */
#define MAX_CCE                     10

#define CCE_ALLOCATED               1

#define DCI_MAX_SIZE                128
#define BCH_MAX_SIZE                6

#ifdef FUNCTION_CALLS_TRACE
#define PRINT_FUNCTION_NAME(s)      printf("%s\n", s)
#else
#define PRINT_FUNCTION_NAME(s)
#endif

/****************************************************************************
 * Exported Types
 ****************************************************************************/
typedef enum MacMode
{
    MACMODE_NRT = 0,
    MACMODE_RT, 
    MACMODE_RT2
} MAC_MODE;

typedef enum ListType
{
    LISTTYPEDIS = 0,
    LISTTYPEENA 
} LIST_TYPE;

/* Transport channels used by the physical layer*/
typedef enum TransportChannelT
{
    DLSCH_TRAN_CH = 0,
    BCH_TRAN_CH,
    PCH_TRAN_CH,
    ULSCH_TRAN_CH
} TRANSPORT_CHANNEL_TYPE;

typedef enum LogicChanType
{
    CCCH,
    DCCH,
    DTCH,
    BCCH,
    PCCH,
    MCCH,
    MTCH
} LOGIC_CHAN_TYPE;

typedef enum TransmissonModeT
{
    TRANS_MODE_1 = 1, /* Transmission from a single eNodeB antenna port; */
    TRANS_MODE_2, /* Transmit diversity */
    TRANS_MODE_3, /* Open-loop spatial multiplexing */
    TRANS_MODE_4, /* Closed-loop spatial multiplexing */
    TRANS_MODE_5, /* Multi-user Multiple-Input Multiple-Output (MIMO) */
    TRANS_MODE_6, /* Closed-loop rank-1 precoding */
    TRANS_MODE_7  /* Transmission using UE-specific reference signals */
}TRANSMISSON_MODE;

// 36.211 - Table 6.9.3-1
enum PHICH_DURATION
{
    PHICHDUR_NORMAL = 1,
    PHICHDUR_EXTEND2,
    PHICHDUR_EXTEND3,
};

enum PHICH_MIB_DURATION
{
    PHICHDUR_MIB_NORMAL = 0,
    PHICHDUR_MIB_EXTEND,
};

enum PHICH_RESOURCE
{
    PHICHRES_ONE_SIXTH = 0,
    PHICHRES_HALF,
    PHICHRES_ONE,
    PHICHRES_TWO,
};

typedef struct MibInfoT
{
    unsigned int systemFrameNumberHi:2;
    unsigned int phich_resource:2;  // enum {oneSixth, half, one, two}
    unsigned int phich_duration:1;  // enum {normal, extended}
    unsigned int dl_Bandwidth:3;    // enum {n6, n15, n25, n50, n75, n100} - firstly transmitted byte

    unsigned int spareHi:2;
    unsigned int systemFrameNumberLo:6; // 8 bits

    unsigned int spareLo:8; // 10 bits
}MIB_INFO;

typedef struct Dci2CceT
{
    DCI_DATA_INFO dciCCEInfo;
    U8 cceIndex;
    U8 isCCEFree;
}DCI_2_CCE;

typedef struct DciCCEContainerT
{
    DCI_2_CCE dciCceInfo[MAX_CCE];
    /* DCICCEInfoCmnChannel  dciCCEInfoCmnChannel[MAX_DL_COMMON_CHANNEL_MSG];*/
    U8 arrayCCEAllocated[ MAX_CCE];
    U8 countOfCceAllocated;
}DCI_CCE_CONTAINER;

#ifdef MX_ARM_ENABLED
typedef int (*MacRlcIoFunc)(U16 *logicChanID, PTR pBuf, U32 blockSize);
#else
typedef int (*MacRlcIoFunc)(U16 *logicChanID, PTR pBuf, OSAL_SIZE blockSize);
#endif

typedef U32 (*BufferLengthFunc)(PTR buf);
typedef U32 (*BufferlcIDFunc)(PTR buf);
typedef struct LogicChanDesc
{

    U32 lastScheduledTick;    /* contain the information regarding when this logical channel
    was scheduled */
    U32 queueLoad;            /* indicates the data available for this logical channel */
    U32 maximumBitRate;       /* indicates the maximum bit rate of the UE */
    U32 guaranteedBitRate;    /* indicates that this UE should atleast get this bit rate */
    TRANSPORT_CHANNEL_TYPE transportChannel;
    S8 logicalChannelId;      /* indicates the id of this logical channel*/
    U8 logicalChannelPriority; /* indicates the priority of this logical channel */
    U16 LCID;
    LOGIC_CHAN_TYPE ChanType;
    U16 DownlinkNumRB;
    U16 UplinkNumRB;
    U16 DownLinkMCS;
    U16 UpLinkMCS;
    MacRlcIoFunc LteMacGetDataBlock;
    BufferLengthFunc BufferLength;
    BufferlcIDFunc BufferlcID;
#ifndef MX_ARM_ENABLED
    OSAL_QUEUE* LCQueue;
    OSAL_SEMAPHORE queueLoadSem; /* semaphore, which should be taken before updating */
                                 /* the queue load ofthis Logical Channel */
#endif
} LOGIC_CHAN_DESC, *PLOGIC_CHAN_DESC;

/* MAC header for MAC SDU with 7-bit length (DL-SCH, UL-SCH) */
typedef struct MacHeaderShort
{
    unsigned short LCID:5;
    unsigned short E:1;
    unsigned short R:2;
    unsigned short L:7;
    unsigned short F:1;
} MAC_HEADER_SHORT;

typedef struct MacHeaderLong
{
    unsigned short LCID:5;
    unsigned short E:1;
    unsigned short R:2;
    unsigned short L1:7;
    unsigned short F:1;
    unsigned char  L2:8;
} MAC_HEADER_LONG;

/* MAC header for fixed size MAC control elements */
typedef struct MacHeaderCtrl
{
    unsigned char LCID:5;
    unsigned char E:1;
    unsigned char R:2;
} MAC_HEADER_CTRL;

typedef union MacHeader
{
    MAC_HEADER_LONG Long;
    MAC_HEADER_SHORT Short;
    MAC_HEADER_CTRL Ctrl;
} MAC_HEADER;

typedef struct _sPhyAutoRunCtx_
{
     U32 AutoMode;
     U32 TimerResolution;
     U32 SFCount;
     U32 SFStartCount;
     U32 Direction;
     U32 RunEna;
     U32 RxSduEna;
} PHY_AUTORUN_CTX;

typedef struct UeDesc
{
    // UE specific
    U32 DlImcs;
    U32 UlImcs;
    U32 DlItbs;
    U32 UlItbs;

    // Callbacks provided by upper layer
   // MacRlcIoFunc LteMacGetDataBlock;
    MacRlcIoFunc LteMacPutDataBlock;

    // number of logic channel per UE
    U32 NLogicChannel;
    PLOGIC_CHAN_DESC pLogicChan[LCID_MAX_LCHAN - LCID_MIN_LCHAN + 1];

} UE_DESC, *PUE_DESC;

typedef struct tagMacPhyCommState
{
    U32 NPhyChanLeft;
    U32 NextChanID;
} MacPhyCommState, *PMacPhyCommState;

typedef struct Mac2PhyInterfaceT
{
    PhyTxCb  txCb;
    PhyRxCb  rxCb;
    PTR toPhy;
    PTR fromPhy;
    U32 pMem[2];
} Mac2PhyInterface;

/*
typedef struct CommonDesc
{
    unsigned int frameNumber;
    unsigned int subFrameNumber;
    unsigned int globalTtiCount;
    unsigned int sduCount;
    unsigned int errorCount;

    U32 ChanBW;
    U32 CpType;
    U32 InstanceID;
    U32 num_IQSamps;
    U32 RxNumBytes[2];
    U8 DirMode;

    U8 isMIBconfigured;

    INITPARM initParm;
    MacPhyCommState TxState;
    MacPhyCommState RxState;

    TSTPDESC tstpbsDl;
    TSTPDESC tstpbsUl;
    U32 predefConfig;

    Mac2PhyInterface Phy;
} COMMON_DESC, *PCOMMON_DESC;
*/

typedef RESULTCODE (*MSGPARSER) (U32 size, PTR msg);

typedef struct CrcDesc
{
    //  0, 8, 16 bits (Bch) 24 bits (Dlsch)
    U8 Crclength :6;
    //  0 Disabled 1 enabled
    U8 CrcScrambling :2;
} CRC_DESC, *PCRC_DESC;

typedef struct ScrambleDesc
{
    // 0 DL Scrambler   1 UL Scrambler with provisions for ACK/NACK
    // and Rank Indication Handling
    U8              ScramblerType;
    // particular initialization value portion that is required
    // by the PHY to generate the Cinit value
    // for PUSCH/PDSCH/PUCCH, this is n_RNTI (Radio Network Temporary
    // Identifier that is associated with a UE once an RRC connection exists)
    U16             ScrInitValInput;
} SCRAMBLE_DESC, *PSCRAMBLE_DESC;

typedef struct AntDesc
{
    U8 CddType :1;
    U8 CodeBookIdx :4;
    U8 CodeBookSubsetRes[8];
} ANT_DESC, *PANT_DESC;

typedef struct BlockDesc
{
    // Number of resource unit
    U16 NumUsedUnits;
    // Number of resource unit to the beginning of the next map
    U16 GapUnitCount;
} BLOCK_DESC, *PBLOCK_DESC;

typedef struct MacPduDesc
{
    U32 TBSize;
    U32 NumSduBlock;
    U32 SduBlockOffset;
    U32 SduHeaderOffset;
    MAC_HEADER SduHeader[MAX_SDU_BLOCKS];
    U8 *pSduBlock[MAX_SDU_BLOCKS];
    U32 SduBlockLen[MAX_SDU_BLOCKS];
    U32 LCID[MAX_SDU_BLOCKS];
    U32 NumPutBlock;
    U32 NumGetBlock;
    UE_DESC* pUe;
} MAC_PDU_DESC, *PMAC_PDU_DESC;

typedef enum MacConfModes
{
    MACCONF_FDX0 = 0,
    MACCONF_FDX1,
    MACCONF_MAIN,
    MACCONF_RCT
}MACCONFMODE;

typedef struct MacInitConfig_t
{
    /* PHY specific */
    U32 phyInstanceId;
    INITPARM phyInitParam;
    /* MAC specific */
    U32 macConfMode;
    U32 configId;
    U32 DrainRxSduModeOn;
    U32 ConsoleVals[20];
    U32 ConsoleValsNum;
} MAC_INIT_CONFIG;

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
extern U16 Imcs2ItbsTable[29];
extern U32 Itbs2TbsTable[27][110];
//extern COMMON_DESC* pLteMacCommon;
extern unsigned int MacRtMode;
extern unsigned int PhyOnLinuxRT;
extern unsigned int MacMaxChanDescs;
extern S16 CA_srsSubframeConfigTable[];
extern U16 UESpecificSRSPeriodicityTable[9][4];
/****************************************************************************
 * Exported Functions
 ****************************************************************************/
// Called by RLC when a corresponding RLC link is created. Return LCID
RESULTCODE LteMacCreateLogicChannel(void* pUe, U32 LogicChanType, U32 LogicChanID);
void SetTestLogicalChannelId_Type(U32 UeId, U32 LogicChanIDOld, U32 LogicChanIDNew);
int GetMacHeaderSize(void* h, U32* SduLen);
void LteMacPduDecoder(U8* pMacPdu, U32 Len, PTR hCtx);
// Called by RLC when a
// corresponding RLC link is destroyed. Return destroy status
RESULTCODE LteMacDestroyLogicChannel(void* pUe, U32 LogicChanId);

void LteMacPduEncoder(MAC_PDU_DESC *pMacPdu, UE_DESC *pUe, U32 SubFrameType);


void LteMacReleasePdu(MAC_PDU_DESC *pMacPduDesc);

void LteMacReleaseSubFrame(PDLSUBFRDESC pSubFrame);

char LteMacHasSthToSend(PDLSUBFRDESC pSubFrame);
// TODO: Replace with list
void* MacCreateUE(void);
// TODO: Implement
U16 MacGetAvailChanID(void);

RESULTCODE LteMacInit(PTR pHeap,
                U16 instanceID,
                MAC_INIT_CONFIG *pMacInitConfig,
                PhyTxCb txCb,
                PhyRxCb rxCb,
                PTR toPhy,
                PTR fromPhy);
RESULTCODE LteMacDestroy(void);

void LastSduHeader(void* h);
U8 GetLastSduHeaderStatus(void* h);
void MacFreeBlock(void** p);
U32 AddMacHeader(MAC_HEADER* Header, U32 len, U32 LcId);

#ifndef MX_ARM_ENABLED
U32 GetUeId();
#else
INLINE U32 MacGetTick(void)
{
    return *(volatile U32 *)0xFE050004;
}
#endif

#endif /* LTEMACCORE_H */

