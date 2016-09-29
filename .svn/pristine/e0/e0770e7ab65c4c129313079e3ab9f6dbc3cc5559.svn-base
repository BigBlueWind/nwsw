/** @file LteMac2PhyInterface.c
 *
 * @brief Message-based MAC-PHY interface
 * @author Mindspeed Technologies
 * @version $Revision: 1.213.8.2 $
 *
 * COPYRIGHT(c) 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "4gmx_arm.h"
#include "heaps.h"
#include "4gmx_heap.h"
#include "apidefs.h"
#include "lte_entry.h"

#include "enbMacEntry.h"
#include "enbMac2PhyIface.h"
#include "LTE_test_config.h"
#include "LteMacTestCfgMgr.h"
#include "LteBsMacDlScheduler.h"
#include "LteBsMacUlScheduler.h"
#include "supervisor.h"
#include "appprintf.h"
#include "bs_static_config.h"
#include "mlog.h"
#include "dataint.h"

#include "lte_mac_state.h"
#include "lte_pusch.h"
#include "lte_phich.h"
#include "lte_if.h"

#include "lte_nmm_state.h"

#include "e_tm.h"
#include "lte_etm.h"

/*UMTSNMM*/
#include "UMTSNwl/UMTSNwlMsgPro.h"

/****************************************************************************
 Private Definitions
 ****************************************************************************/

#define DLTXSDU_QUEUE_SIZE      40
#define PhySduQ_SIZE            40
// Follow 3GPP TS 36.213 Table 7.1.7.2.1-1
// maximum for Itbs=26 Nprb=100 (20MHz QAM64) is 75376 bits = 9422 bytes
// plus overhead for RxSdu headers
#define RXSDUSZ                 10000

/****************************************************************************
 Private Types
 ****************************************************************************/

// Used to fill HI DCI channel
typedef U32 (*tAddToTail)(U8 MessageType, U32 MessageLen, U8* tempo);
typedef struct stMac2PhyQueue
{
    PMAC2PHY_QUEUE_EL StartQueueCurr;
    PMAC2PHY_QUEUE_EL TailQueueCurr;
    tAddToTail pAddToTail;
} MAC2PHY_QUEUE, *PMAC2PHY_QUEUE;


typedef struct
{
    U32 start;
    U32 diff;
    U32 physaddr;
    U32 frame;
    U32 type;
} SvsrApiMsgStats_t;

#define SIZEOF_SVSRAPIMSGSTAT                                   1024
SvsrApiMsgStats_t SvsrApiMsgStats[SIZEOF_SVSRAPIMSGSTAT];
U32 numSvsrApiMsgStats=0;
V32 IcpuFlagErr = 0;

#define NMM_CMD_NUM 11
U8 nmm_message_types[NMM_CMD_NUM] = {NMM_START, NMM_STOP, NMM_RSSI_MEAS_REQ, NMM_STOP_RSSI_MEAS_REQ, NMM_CELL_SEARCH_REQ, NMM_STOP_CELL_SEARCH_REQ,
    NMM_PBCH_CONFIG_REQ, NMM_STOP_PBCH_REQ, NMM_SIB1_CONFIG_REQ, NMM_BCCH_CONFIG_REQ, NMM_STOP_BCCH_REQ};

U32 SvsrLoggerMaskStore = 0;


/*UMTSNMM*/
#define UMTSNMM_CMD_NUM  6
U8 umtsnmm_message_types[UMTSNMM_CMD_NUM] = {NWL_INIT_3G_NWL,NWL_STOP_3G_NWL,
    NWL_CELL_SEARCH_REQ,NWL_STOP_CELL_SEARCH_REQ,NWL_BCH_CONFIG_REQ,NWL_STOP_BCH_REQ};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

//static U8* GetFromBuf(void);
static void AddToBuf(U8* Buf, U32 Size);
static void InitRxSduBuf(void* param);
static U32 ApiListAddToHead(U8 MessageType , U32 MessageLen , U8* MessagePtr);
static U32 ApiListAddToHead2(U8 MessageType , U32 MessageLen , U8* MessagePtr);
static U32 ApiListAddToTail(U8 MessageType , U32 MessageLen , U8* tempo);
static U32 ApiListFree(void);

static MXRC MacSendToPhy(U8 MessageType, LPVOID pData, UINT32 nMsgSize);
static void SendBufToHost(void);
static MXRC SendToPhyICpu(UINT32 nPhyInstID, PMAC2PHY_QUEUE_EL pHead);
static RESULTCODE MacProcRxSdu( PRXSDUIND pRxSduInd);
static void MacFreeRxSduQueue(void);
static void MacProcRxSduQueue(PDLSUBFRDESC pDlSubFrame, PULSUBFRDESC pUlSubFrame);
static RESULTCODE LteMacIsRxNotExpected(void);
static MXRC MacSendToPhyNMM(U8 *MessageTypes, LPVOID *pDatas, UINT32 *nMsgSizes, UINT32 nMessages);
static MXRC MacSendToPhyUMTSNMM(U8 *MessageTypes, LPVOID *pDatas, UINT32 *nMsgSizes, UINT32 nMessages);

/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/

// # SDU to send to host
static U32 RxSduCntr = 0;

// # RxDrain that has been sent to host
static U32 DrainRxCount = 0;

/****************************************************************************
 * Global Variables
 ****************************************************************************/

HANDLE dlControlTxSduQueue = { 0 };
HANDLE dlControlTxSduQueue2 = { 0 };
U8* RxSduBuf = NULL;
U32 RxSduSet = 0;
U32 RxSduGet = 0;
U32 RxBufSize = RXSDUSZ;

pLtePhyHiDciAPISet pHiDciApiSet = NULL;

static U32 prachErrors = 0;
static U32 prachSdus = 0;

U32 TxVectorCntrSent = 0;
U32 RxVectorCntrSent = 0;

UINT32 gMac2PhyDirectFIFOUsed = 0;

MAC2PHY_QUEUE Mac2PhyQueue;
UINT32 Mac2PhyICPUEmulation = 0;

UINT32 doubleSDU = 0;

LPVOID PhySduQ[PhySduQ_SIZE];
VUINT32 PhySduQPut = 0;
VUINT32 PhySduQGet = 0;

PMAC2PHY_QUEUE_EL RxSduHead = NULL;

U32 IsSendToPhyMsgMask = 0;

RESULTCODE (*DciDLPrepareFunctionPtr[MAX_DCI_FORMAT])(U8 *, DCI_DATA_INFO *, U32 *) =
{
    NULL,
    DciPrepareFormat0,                  /* For DCI Foramt 0 */
    DciPrepareFormat1,                  /* For DCI Foramt 1 */
    DciPrepareFormat1a,                 /* For DCI Foramt 1A */
    NULL,                               /* For DCI Foramt 1B */
    NULL,                               /* For DCI Foramt 1C */
    NULL,                               /* For DCI Foramt 1D */
    DciPrepareFormat2,                  /* For DCI Foramt 2 */
    NULL,                               /* For DCI Foramt 2A */
    NULL,                               /* For DCI Fromat 3 */
    NULL                                /* For DCI Format 3A */
};

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

extern UINT32 IMsgDump;
extern UINT32 global_chanIdx;

/*
 * Local Functions
 */

U8* put_bit(U8 *where, U8 *start_end_bit, U8 bit_value)
{
    *where &= ~(1 << *start_end_bit);
    if ( bit_value )
        *where |= (1 << *start_end_bit);
    if ( 0 == *start_end_bit )
    {
        *start_end_bit = 7;
        ++where;
    }
    else
    {
        --*start_end_bit;
    }
    return where;
}

/* Bit field "value" given size "num_bits" (max 32) to be written to memory starting byte "where" and bit offset "start_end_bit".
* If bitfield don't fit 1st byte then writing will continue over next bytes.
 * Return byte ptr where one can continue writing bits. "start_end_bit" updated with bit offset where one can continue to write bits.
 *
 * @param where Pointer to the 1st byte of bitfield destination.
 * @param start_end_bit Pointer to bit offset value (0-7). Value needs to be initialized to offset of the very 1st significant bit from which to start to write bitfield.
 * @param value Bitfiled value to be written to memory (only "num_bits" will be written, up to 32).
 * @param num_bits Number of bits of bitfield to write.
 * @return Pointer to byte where one can continue to write bits. Value under "start_end_bit" updated to bit offset where one can continue to write bits.
 */

U8* put_bit_field(U8 *where, U8 *start_end_bit, U32 value, U8 num_bits)
{
    U32 mask = 1 << (num_bits-1);
    U8 bit_pos = *start_end_bit;
    U8 *byte_pos = where;

    while ( mask )
    {
        // clear bit first
        *byte_pos &= ~(1 << bit_pos);
        // set bit if needed
        if ( value & mask )
            *byte_pos |= 1 << bit_pos;
        if ( 0 == bit_pos )
        {
            bit_pos = 7;
            ++byte_pos;
        }
        else
        {
            --bit_pos;
        }
        mask >>= 1;
    }
    *start_end_bit = bit_pos;
    return byte_pos;
}

U8 MaxRBs2RBAFieldSize(U32 max_rbs)
{
    // WARNING: Format1A only known to be correct for sure
    // Resource block assignment [log2(N_DL_RB * (N_DL_RB + 1)/2)] bits
    // as defined in section 7.1.6.3 of 36.213
    U32 max_value = max_rbs * (max_rbs + 1) / 2;
    U8 num_bits = 1;
    while ( (1 << num_bits) < max_value ) ++num_bits;

    return num_bits;
}

U8* FillRBALocalized(U8 *where, U8 *start_end_bit, DCI_DATA_INFO *pDciInfo)
{
    // function return pointer to byte where to continue to put data
    // value under start_bit ptr updated with the bit where to continue to put info

    U8 rba_field_bit_size = MaxRBs2RBAFieldSize(pDciInfo->MaxRb);
    U32 riv;

    if ((pDciInfo->Lcrb - 1) <= (pDciInfo->MaxRb >> 1))
        riv = pDciInfo->MaxRb * (pDciInfo->Lcrb - 1) + pDciInfo->startRBIndex;
    else
        riv = pDciInfo->MaxRb * (pDciInfo->MaxRb - pDciInfo->Lcrb + 1) + (pDciInfo->MaxRb - 1 - pDciInfo->startRBIndex);

    return put_bit_field(where, start_end_bit, riv, rba_field_bit_size);
}

/** Fill SDU with DCI Format 0 payload
 *
 * @param pSdu Pointer to the target SDU payload block
 * @param pDciInfo TODO:
 * @param index TODO:
 * @return Standard result code
 */
RESULTCODE DciPrepareFormat0(U8 *pSdu, DCI_DATA_INFO *pDciInfo, U32 *index)
{
    U32 i, bit_idx = 0;
    U8  bit_sdu[100] = { 0 };
    U32 riv;
    U32 of;
    U32 ch;
    U8  bitsAlloc;
    U32 CheckVal;

    bit_sdu[bit_idx++] = 0;
    bit_sdu[bit_idx++] = pDciInfo->hopping;

    if ((pDciInfo->Lcrb - 1) <= (pDciInfo->MaxRb >> 1))
        riv = pDciInfo->MaxRb * (pDciInfo->Lcrb - 1) + pDciInfo->startRBIndex;
    else
        riv = pDciInfo->MaxRb * (pDciInfo->MaxRb - pDciInfo->Lcrb + 1) + (pDciInfo->MaxRb - 1 - pDciInfo->startRBIndex);

    bitsAlloc = 0;
    CheckVal = (pDciInfo->MaxRb * (pDciInfo->MaxRb+1)) / 2;
    while( (1 << bitsAlloc) < CheckVal)
        ++bitsAlloc;

    for (i = 0; i < bitsAlloc; i++)
    {
//        of = pDciInfo->numOfRBs - i - 1;
        of = bitsAlloc - i - 1;

        ch = riv & (1 << of);
        bit_sdu[bit_idx++] = (ch >> of)&1;
    }

    /* and coding scheme and redundancy version - 5 bits  */
    for (i = 0; i < 5; i++)
    {
        of = 4 - i;
        ch = pDciInfo->tbInfo[0].mcs & (1 << of);
        bit_sdu[bit_idx++] = (ch >> of)&1;
    }
    /*- New data indicator - 1 bit*/
    bit_sdu[bit_idx++] = pDciInfo->tbInfo[0].txIndicator&1;

    /*TPC command for scheduled PUSCH - 2 bits as defined in section 5.1.1.1 of [3]*/
    bit_sdu[bit_idx++] = 0;
    bit_sdu[bit_idx++] = 0;

    /*Cyclic shift for DM RS - 3 bits as defined in section 5.5.2.1.1 of [2]*/
    bit_sdu[bit_idx++] = 0;
    bit_sdu[bit_idx++] = 0;
    bit_sdu[bit_idx++] = 0;
    /*3bits - FDD*/
    bit_sdu[bit_idx++] = 0;
    bit_sdu[bit_idx++] = 0;
    bit_sdu[bit_idx++] = 0;

    /* CQI request - 1 bit as defined in section 7.2.1 of [3] */

    bit_sdu[bit_idx] = 0;

    memset(pSdu, 0, (bit_idx + 7) >> 3);

    for (i = 0; i < bit_idx; i++)
        pSdu[i >> 3] |= bit_sdu[i] << (7 - (i & 7));

    *index = bit_idx;

    return SUCCESS;
}

/** Sends SDU to PHY
 *  This function uses new API interface (as opposite to PhySendMsg) and enqueues SDU messages
 *  into system queue (non-blocking call)
 *  Should replace all PhySendMsg usage
 *
 * @param ApiId PHY API message ID as defined in LtePhyL2Api.h
 * @param Data Pointer to the message block
 * @param Len Size of the message block
 * @return Standard result code
 */
RESULTCODE SduSend(U8 ApiId, U8 *Data, U32 Len)
{
// TODDO: Replace LteMac2PhyAddToTail by ApiListAddToTail
#ifdef MX_ARM_ENABLED
    if (Mac2PhyQueue.pAddToTail)
    {
        Mac2PhyQueue.pAddToTail(ApiId, Len, Data);
        return SUCCESS;
    }
    MxPutQueueData(dlControlTxSduQueue, Data);
#else
    LteMac2PhyAddToTail(ApiId, Len, Data);
#endif
    return SUCCESS;
}

/** Fill SDU with DCI format 1A payload (variable RBA filed size support)
*
*  _autoRBA here means size of RBA field evaluated based on max number RBs available,
*  unlike DciPrepareFormat1a where it's hardcoded to 10Mhz (11 bits)
*  Once this func is 100% complete it should replace DciPrepareFormat1a and temporary "_autoRBA" postfix removed.
*  Return size of SDU in bits
*
 */
RESULTCODE DciPrepareFormat1a_autoRBA(U8 *pSdu, DCI_DATA_INFO* pDciInfo, U32 *index)
{
    U8 bit_offset = 5; // RBA starts with bit 5
    U8 *pSdu_start = pSdu;
    U32 bit_size;

    pSdu += *index;
    memset(pSdu, 0, DCI_MAX_SIZE);

    /* The fields defined in the DCI formats below are mapped to the information
     * bits a0 to aA-1 as follows. Each field is mapped in the order in which it
     * appears in the description, including the zero-padding bit(s), if any,
     * with the first field mapped to the lowest order information bit a0 and each
     * SUCCESSive field mapped to higher order information bits. The most
     * significant bit of each field is mapped to the lowest order information bit
     * for that field, e.g. the most significant bit of the first field
     * is mapped to a0.
     *
     * DCI format 1A is used for the compact scheduling of one PDSCH codeword
     * and random access procedure initiated by a PDCCH order.
     *
     * - Flag for format0/format1A differentiation - 1 bit, where value 0
     * indicates format 0 and value 1 indicates format 1A
     */

    *pSdu |= 0x1 << 7;              // Format 1A

    /* - Localized/Distributed VRB assignment flag - 1 bit as defined in 7.1.6.3 of 36.213
     * Resource block assignment [log2(N_DL_RB * (N_DL_RB + 1)/2)]
     * as defined in section 7.1.6.3 of 36.213:
     */

    if (pDciInfo->virtulResrcBlkType == 0)
    {
        *pSdu |= 0x0 << 6; /* 0 - indicates Localized */
        pSdu = FillRBALocalized(pSdu, &bit_offset, pDciInfo);
    }
    else
    {
        /*  - For distributed VRB:
         * - If N_DL_RB < 50 or if the format 1A CRC is scrambled by
         * RA-RNTI, P-RNTI, or SI-RNTI
         * [log2(N_DL_RB * (N_DL_RB + 1)/2)] bits provide the
         * resource allocation
         *
         * - else if N_DL_RB >= 50
         * - 1 bit, the MSB indicates the gap value, where value 0
         * indicates Ngap = Ngap_1 and value 1 indicates
         * Ngap = Ngap_2 - ([log2(N_DL_RB * (N_DL_RB + 1)/2)] - 1)
         * bits provide the resource allocation
         */
    }

    /* - Modulation and coding scheme - 5 bits as defined in section 7.1.7 of 36.213 */
    pSdu = put_bit_field(pSdu, &bit_offset, pDciInfo->tbInfo[0].modulationType, 5);

    /* - HARQ process number - 3 bits (FDD), 4 bits (TDD) */
    pSdu = put_bit_field(pSdu, &bit_offset, pDciInfo->harqProcessId & 0x7, 3);

    /* - New data indicator - 1 bit */

    /* - If the format 1A CRC is scrambled by RA-RNTI, P-RNTI, or SI-RNTI:
     * - If N_DL_RB >= 50 then the new data indicator bit indicates the
     * gap value, where value 0 indicates Ngap = Ngap_1 and value 1
     * indicates Ngap = Ngap_2.
     * - Else the new data indicator bit is reserved.
     * - Else
     * - The new data indicator bit indicates new data
     */
    pSdu = put_bit(pSdu, &bit_offset, 1);

    /* - Redundancy version - 2 bits */
    pSdu = put_bit_field(pSdu, &bit_offset, (pDciInfo->tbInfo[0].redundancyVer & 0x3), 2);


    /* - TPC command for PUCCH - 2 bits as defined in section 5.1.2.1 of 36.213 */

    /* - If the format 1A CRC is scrambled by RA-RNTI, P-RNTI, or SI-RNTI:
     * - The most significant bit of the TPC command is reserved.
     * - The least significant bit of the TPC command indicates
     * column N_1A_PRB of the TBS table defined in 36.213.
     * - If least significant bit is 0 then N_1A_PRB = 2 else
     * N_1A_PRB = 3.
     * - Else
     * - The two bits including the most significant bit indicates
     * the TPC command
     */
    pSdu = put_bit_field(pSdu, &bit_offset, (pDciInfo->tpc & 0x3), 2);

    /* - Downlink Assignment Index (this field just applies to TDD operation
     * and is not present in FDD) - 2 bits
     */

    /* If the number of information bits in format 1A is less than that of
     * format 0, zeros shall be appended to format 1A until the payload size
     * equals that of format 0.
     */

    /* If the number of information bits in format 1A belongs to one of the
     * sizes in Table 5.3.3.1.2-1, one zero bit shall be appended to format 1A.
     */

    /* When the format 1A CRC is scrambled with a RA-RNTI, P-RNTI, or SI-RNTI
     * then the following fields among the fields above are reserved:
     * - HARQ process number
     * - Downlink Assignment Index
     * (used for TDD only and is not present in FDD)
     */

    bit_size = ((pSdu - pSdu_start) << 3) + (7 - bit_offset);

    if ( bit_size == 12 || bit_size == 14 || bit_size == 16 ||
         bit_size == 20 || bit_size == 24 || bit_size == 26 ||
         bit_size == 32 || bit_size == 40 || bit_size == 44 || bit_size == 56 )
    {
        bit_size++;
    }

    *index = bit_size;
    return SUCCESS;
}

/** Fill SDU with DCI format 2 payload
 *
 * @param pSdu Pointer to the target SDU payload block
 * @param pDciInfo TODO:
 * @param index TODO:
 * @return Standard result code
 */
RESULTCODE DciPrepareFormat2(U8 *pSdu, DCI_DATA_INFO* pDciInfo, U32 *index)
{
    U32 tmp = 0;
    U32 i, bit_idx = 0;
    U8 bit_sdu[100] = { 0 };

    /* The fields defined in the DCI formats below are mapped to the information
     * bits a0 to aA-1 as follows. Each field is mapped in the order in which it
     * appears in the description, including the zero-padding bit(s), if any,
     * with the first field mapped to the lowest order information bit a0 and each
     * successive field mapped to higher order information bits. The most
     * significant bit of each field is mapped to the lowest order information bit
     * for that field, e.g. the most significant bit of the first field
     * is mapped to a0.
     */

    /* - Resource allocation header (resource allocation type 0 / type 1)
     * - 1 bit as defined in section 7.1.6 of 36.213
     * If downlink bandwidth is less than or equal to 10 PRBs, there is no
     * resource allocation header and resource allocation type 0 is assumed.
     */
    bit_sdu[bit_idx++] = pDciInfo->resrcAllocType;

    /* Resource block assignment: */
    if (pDciInfo->resrcAllocType == 0)
    {
        tmp = pDciInfo->numOfRBs / pDciInfo->RGBSize;
        if (pDciInfo->numOfRBs % pDciInfo->RGBSize)
            tmp++;
        for (i = 0; i < tmp; i++)
            bit_sdu[bit_idx++] = 1;
    }
    else
    {
        /* - For resource allocation type 1 as defined in section 7.1.6.2 of 36.213:
         * -  Log2(P) bits of this field are used as a header specific to this
         * resource allocation type to indicate the selected resource blocks subset
         *
         * - 1 bit indicates a shift of the resource allocation span
         * - (N_DL_RB/P - Log2(P) - 1) bits provide the resource allocation
         * where the value of P depends on the number of DL resource blocks as
         * indicated in section 7.1.6 of 36.213
         */
    }

    /* TPC command for PUCCH - 2 bits as defined in section 5.1.2.1 of [3] */
    bit_sdu[bit_idx++] = 0;
    bit_sdu[bit_idx++] = 0;

    /* HARQ process number - 3 bits (FDD), 4 bits (TDD) */
    bit_sdu[bit_idx++] = 0;
    bit_sdu[bit_idx++] = 0;
    bit_sdu[bit_idx++] = 1;

    /* Transport block to codeword swap flag - 1 bit */

    bit_sdu[bit_idx++] = pDciInfo->tbSwapFlag & 0x1;
    memset(pSdu, 0, bit_idx / 8 + 1);
    for (i = 0; i < bit_idx; i++)
        pSdu[i / 8] |= bit_sdu[i] << (8 - (i % 8) - 1);
    /* Transport block 1:
     * - Modulation and coding scheme - 5 bits as defined in section 7.1.7 of [3]
     * - New data indicator - 1 bit
     * - Redundancy version - 2 bits
     * Transport block 2:
     * - Modulation and coding scheme - 5 bits as defined in section 7.1.7 of [3]
     * - New data indicator - 1 bit
     * - Redundancy version - 2 bits
     */
    for (i = 0; i < pDciInfo->numTB; i++)
    {
        U8 tb = pDciInfo->tbInfo[i].mcs << 3;
        tb |= ((0x1 & pDciInfo->tbInfo[i].txIndicator) << 2);
        tb |= 0x3 & pDciInfo->tbInfo[i].redundancyVer;

        pSdu[bit_idx / 8] = tb;
        bit_idx += 8;
    }
    if (pDciInfo->numTB == 1)
    {
        U8 tb = 0x1;
        pSdu[bit_idx / 8] = tb;
        bit_idx += 8;
    }
    if (pDciInfo->numAntennaPort == 2)
    {
        if (pDciInfo->transmissionMode == TXDIVERSITY)
            pSdu[bit_idx / 8] = 0;
        else
            pSdu[bit_idx / 8] = 0x1 << 6;   // Spatial multiplexing
        bit_idx += 3;
    }
    else /* numAntennaPort = 4*/
    {
        uart_printf("numAntennaPort %d\n", pDciInfo->numAntennaPort);
    }
    *index = bit_idx;

    return SUCCESS;
}

/** Fill SDU with DCI format 2 payload.
* Unlike DciPrepareFormat2 it uses Lcrb and startRBIndex to set correct resource allocation.
 *
 * @param pSdu Pointer to the target SDU payload block
 * @param pDciInfo TODO:
 * @param index TODO:
 * @return Standard result code
 */
RESULTCODE DciPrepareFormat2_autoRes(U8 *pSdu, DCI_DATA_INFO* pDciInfo, U32 *index)
{
    U32 tmp = 0;
    U32 i, bit_idx = 0;
    U8 bit_sdu[100] = { 0 };

    /* The fields defined in the DCI formats below are mapped to the information
     * bits a0 to aA-1 as follows. Each field is mapped in the order in which it
     * appears in the description, including the zero-padding bit(s), if any,
     * with the first field mapped to the lowest order information bit a0 and each
     * successive field mapped to higher order information bits. The most
     * significant bit of each field is mapped to the lowest order information bit
     * for that field, e.g. the most significant bit of the first field
     * is mapped to a0.
     */

    /* - Resource allocation header (resource allocation type 0 / type 1)
     * - 1 bit as defined in section 7.1.6 of 36.213
     * If downlink bandwidth is less than or equal to 10 PRBs, there is no
     * resource allocation header and resource allocation type 0 is assumed.
     */
    bit_sdu[bit_idx++] = pDciInfo->resrcAllocType;

    /* Resource block assignment: */
    if (pDciInfo->resrcAllocType == 0)
    {
        U32 rb_idx = pDciInfo->startRBIndex;
        U32 Lcrb = pDciInfo->Lcrb;
        U32 num_rgbs, start_rgb, ri;
        tmp = (pDciInfo->numOfRBs+pDciInfo->RGBSize-1) / pDciInfo->RGBSize;

//        if (pDciInfo->numOfRBs % pDciInfo->RGBSize)
//            tmp++;

        num_rgbs = (Lcrb + pDciInfo->RGBSize - 1) / pDciInfo->RGBSize;

        start_rgb = rb_idx / pDciInfo->RGBSize;

        for ( ri = start_rgb; ri != (start_rgb + num_rgbs); ++ri )
            bit_sdu[bit_idx + ri] = 1;
        bit_idx += tmp;

    }

    else
    {
        /* - For resource allocation type 1 as defined in section 7.1.6.2 of 36.213:
         * -  Log2(P) bits of this field are used as a header specific to this
         * resource allocation type to indicate the selected resource blocks subset
         *
         * - 1 bit indicates a shift of the resource allocation span
         * - (N_DL_RB/P - Log2(P) - 1) bits provide the resource allocation
         * where the value of P depends on the number of DL resource blocks as
         * indicated in section 7.1.6 of 36.213
         */
    }

    /* TPC command for PUCCH - 2 bits as defined in section 5.1.2.1 of [3] */
    bit_sdu[bit_idx++] = 0;
    bit_sdu[bit_idx++] = 0;

    /* HARQ process number - 3 bits (FDD), 4 bits (TDD) */
    bit_sdu[bit_idx++] = 0;
    bit_sdu[bit_idx++] = 0;
    bit_sdu[bit_idx++] = 1;

    /* Transport block to codeword swap flag - 1 bit */

    bit_sdu[bit_idx++] = pDciInfo->tbSwapFlag & 0x1;
    memset(pSdu, 0, bit_idx / 8 + 1);
    for (i = 0; i < bit_idx; i++)
        pSdu[i / 8] |= bit_sdu[i] << (8 - (i % 8) - 1);
    /* Transport block 1:
     * - Modulation and coding scheme - 5 bits as defined in section 7.1.7 of [3]
     * - New data indicator - 1 bit
     * - Redundancy version - 2 bits
     * Transport block 2:
     * - Modulation and coding scheme - 5 bits as defined in section 7.1.7 of [3]
     * - New data indicator - 1 bit
     * - Redundancy version - 2 bits
     */
    for (i = 0; i < pDciInfo->numTB; i++)
    {
        U8 tb = pDciInfo->tbInfo[i].mcs << 3;
        tb |= ((0x1 & pDciInfo->tbInfo[i].txIndicator) << 2);
        tb |= 0x3 & pDciInfo->tbInfo[i].redundancyVer;

        pSdu[bit_idx / 8] = tb;
        bit_idx += 8;
    }
    if (pDciInfo->numTB == 1)
    {
        U8 tb = 0x1;
        pSdu[bit_idx / 8] = tb;
        bit_idx += 8;
    }
    if (pDciInfo->numAntennaPort == 2)
    {
        if (pDciInfo->transmissionMode == TXDIVERSITY)
            pSdu[bit_idx / 8] = 0;
        else
            pSdu[bit_idx / 8] = 0x1 << 6;   // Spatial multiplexing
        bit_idx += 3;
    }
    else /* numAntennaPort = 4*/
    {
        uart_printf("numAntennaPort %d\n", pDciInfo->numAntennaPort);
    }
    *index = bit_idx;

    return SUCCESS;
}

RESULTCODE DciPrepareFormat1C(U8 *pSdu, DCI_DATA_INFO* pDciInfo, U32 *index)
{
    U8 bit_offset = 7;
    U8 *pSdu_start = pSdu;
    U32 bit_size;

    U8 gap_1 = get_gap_1(pDciInfo->MaxRb);
    U8 n_step_rb = get_n_step_rb(pDciInfo->MaxRb);
    U8 rba_field_bit_size = MaxRBs2RBAFieldSize(gap_1/n_step_rb);
    U16 riv, L_crbs, RB_start, N_dl_vrb;

    pSdu += *index;
    memset(pSdu, 0, DCI_MAX_SIZE);

    if (pDciInfo->MaxRb >= 50)
    {
        // if Ndlrbs >= 50, there is bit for gap indication. So far using hardcoded Ngap = Ngap1
        // value 0 indicates Ngap = Ngap1
        pSdu = put_bit(pSdu, &bit_offset, 0);
    }

    // RBA with virtual assignment
    // convert physical RBs to virtual
    N_dl_vrb = pDciInfo->MaxRb / n_step_rb;
    L_crbs   = pDciInfo->Lcrb  / n_step_rb;
    RB_start = pDciInfo->startRBIndex / n_step_rb;

    if ((L_crbs - 1) <= (N_dl_vrb >> 1))
        riv = N_dl_vrb * (L_crbs - 1) + RB_start;
    else
        riv = N_dl_vrb * (N_dl_vrb - L_crbs + 1) + (N_dl_vrb - 1 - RB_start);

    pSdu = put_bit_field(pSdu, &bit_offset, riv, rba_field_bit_size);

    // Modulation and coding scheme - 5 bits as defined in section 7.1.7 of 36.213
    pSdu = put_bit_field(pSdu, &bit_offset, pDciInfo->tbInfo[0].modulationType, 5);

    bit_size = ((pSdu - pSdu_start) << 3) + (7 - bit_offset);

    *index = bit_size;
    return SUCCESS;
}


/** Returns pointer to the next pending RX SDU from the ring buffer
 *
 * @return Pointer to the RX SDU
 */
/*
static U8* GetFromBuf(void)
{
    U8 *pSdu = NULL;

    pSdu = RxSduBuf;
    RxSduGet = (RxSduGet + RXSDUSZ);
    if (RxSduGet >= RxBufSize - 1)
        RxSduGet = 0;

    return pSdu;
}
*/

/** Fill SDU with DCI format 1 payload
 *
 * @param pSdu Pointer to the target SDU payload block
 * @param pDciInfo TODO:
 * @param index TODO:
 * @return Standard result code
 */
RESULTCODE DciPrepareFormat1(U8 *pSdu, DCI_DATA_INFO* pDciInfo, U32 *index)
{
    U32 idx = *index;

    /* The fields defined in the DCI formats below are mapped to the information
     * bits a0 to aA-1 as follows. Each field is mapped in the order in which it
     * appears in the description, including the zero-padding bit(s), if any,
     * with the first field mapped to the lowest order information bit a0 and each
     * successive field mapped to higher order information bits. The most
     * significant bit of each field is mapped to the lowest order information bit
     * for that field, e.g. the most significant bit of the first field
     * is mapped to a0.
     *
     * - Resource allocation header (resource allocation type 0 / type 1)
     * - 1 bit as defined in section 7.1.6 of 36.213
     * If downlink bandwidth is less than or equal to 10 PRBs, there is no
     * resource allocation header and resource allocation type 0 is assumed.
     */

    pSdu[idx] = (pDciInfo->resrcAllocType & 0x1) << 7;

    /* Resource block assignment: */
    if (pDciInfo->resrcAllocType == 0)
    {
        /* - For resource allocation type 0 as defined in section 7.1.6.1 of 36.213:
         * - N_DL_RB/P bits provide the resource allocation
         * !!!! support only type 1 allocation
         */
        printf("Error in function %s (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
        return FAILURE;
    }
    else
    {
        /* - For resource allocation type 1 as defined in section 7.1.6.2 of 36.213:
         * -  Log2(P) bits of this field are used as a header specific to this
         * resource allocation type to indicate the selected resource blocks
         * subset
         * - 1 bit indicates a shift of the resource allocation span
         * - (N_DL_RB/P - Log2(P) - 1) bits provide the resource allocation
         * where the value of P depends on the number of DL resource blocks as
         * indicated in section 7.1.6 of 36.213
         */
    }

    /* - Modulation and coding scheme - 5 bits as defined in section 7.1.7 of 36.213 */

    /* - HARQ process number - 3 bits (FDD), 4 bits (TDD) */

    /* - New data indicator - 1 bit */

    /* - Redundancy version - 2 bits */

    /* - Downlink Assignment Index - 2 bits
     * (this field just applies to TDD operation and is not present in FDD)
     */

    /* If the number of information bits in format 1 is equal to that for format
     * 0/1A, one bit of value zero shall be appended to format 1.
     */

    return SUCCESS;
}

/** Fill SDU with DCI format 1A payload
 *
 * @param pSdu Pointer to the target SDU payload block
 * @param pDciInfo TODO:
 * @param index TODO:
 * @return Standard result code
 */
RESULTCODE DciPrepareFormat1a(U8 *pSdu, DCI_DATA_INFO* pDciInfo, U32 *index)
{
    U32 idx = *index;
    memset(&pSdu[idx], 0, DCI_MAX_SIZE);

    /* The fields defined in the DCI formats below are mapped to the information
     * bits a0 to aA-1 as follows. Each field is mapped in the order in which it
     * appears in the description, including the zero-padding bit(s), if any,
     * with the first field mapped to the lowest order information bit a0 and each
     * SUCCESSive field mapped to higher order information bits. The most
     * significant bit of each field is mapped to the lowest order information bit
     * for that field, e.g. the most significant bit of the first field
     * is mapped to a0.
     *
     * DCI format 1A is used for the compact scheduling of one PDSCH codeword
     * and random access procedure initiated by a PDCCH order.
     *
     * - Flag for format0/format1A differentiation - 1 bit, where value 0
     * indicates format 0 and value 1 indicates format 1A
     */

    pSdu[idx] |= 0x1 << 7;              // Format 1A

    /* - Localized/Distributed VRB assignment flag - 1 bit as defined in 7.1.6.3 of 36.213
     * Resource block assignment [log2(N_DL_RB * (N_DL_RB + 1)/2)] ~= 10.3163= 11  bits
     * as defined in section 7.1.6.3 of 36.213:
     */

    if (pDciInfo->virtulResrcBlkType == 0)
    {
        pSdu[idx] |= 0x0 << 6; /* 0 - indicates Localized */
        /*  - For localized VRB:
         * [log2(N_DL_RB * (N_DL_RB + 1)/2)] bits provide the resource allocation
         */
        pSdu[idx] |= (pDciInfo->resrcAssgmntBitMap & 0x7FF) >> 5;     // 6 bits
        pSdu[++idx] |= (pDciInfo->resrcAssgmntBitMap & 0x001F) << 3;  // 5 bits
    }
    else
    {
        /*  - For distributed VRB:
         * - If N_DL_RB < 50 or if the format 1A CRC is scrambled by
         * RA-RNTI, P-RNTI, or SI-RNTI
         * [log2(N_DL_RB * (N_DL_RB + 1)/2)] bits provide the
         * resource allocation
         *
         * - else if N_DL_RB >= 50
         * - 1 bit, the MSB indicates the gap value, where value 0
         * indicates Ngap = Ngap_1 and value 1 indicates
         * Ngap = Ngap_2 - ([log2(N_DL_RB * (N_DL_RB + 1)/2)] - 1)
         * bits provide the resource allocation
         */
    }
    /* - Modulation and coding scheme - 5 bits as defined in section 7.1.7 of 36.213 */
    pSdu[idx] |= ((pDciInfo->tbInfo[0].modulationType & 0x1F) >> 2);
    pSdu[++idx] |= ((pDciInfo->tbInfo[0].modulationType & 0x3) << 6);

    /* - HARQ process number - 3 bits (FDD), 4 bits (TDD) */
    pSdu[idx] |= ((pDciInfo->harqProcessId & 0x7) << 4);
    /* - New data indicator - 1 bit */

    /* - If the format 1A CRC is scrambled by RA-RNTI, P-RNTI, or SI-RNTI:
     * - If N_DL_RB >= 50 then the new data indicator bit indicates the
     * gap value, where value 0 indicates Ngap = Ngap_1 and value 1
     * indicates Ngap = Ngap_2.
     * - Else the new data indicator bit is reserved.
     * - Else
     * - The new data indicator bit indicates new data
     */
    pSdu[idx] |= 0x1;
    /* - Redundancy version - 2 bits */
    pSdu[++idx] |= ((pDciInfo->tbInfo[0].redundancyVer & 0x3) << 6);

    /* - TPC command for PUCCH - 2 bits as defined in section 5.1.2.1 of 36.213 */

    /* - If the format 1A CRC is scrambled by RA-RNTI, P-RNTI, or SI-RNTI:
     * - The most significant bit of the TPC command is reserved.
     * - The least significant bit of the TPC command indicates
     * column N_1A_PRB of the TBS table defined in 36.213.
     * - If least significant bit is 0 then N_1A_PRB = 2 else
     * N_1A_PRB = 3.
     * - Else
     * - The two bits including the most significant bit indicates
     * the TPC command
     */
    pSdu[idx] |= ((pDciInfo->tpc & 0x3) << 4);

    /* - Downlink Assignment Index (this field just applies to TDD operation
     * and is not present in FDD) - 2 bits
     */

    /* If the number of information bits in format 1A is less than that of
     * format 0, zeros shall be appended to format 1A until the payload size
     * equals that of format 0.
     */

    /* If the number of information bits in format 1A belongs to one of the
     * sizes in Table 5.3.3.1.2-1, one zero bit shall be appended to format 1A.
     */

    /* When the format 1A CRC is scrambled with a RA-RNTI, P-RNTI, or SI-RNTI
     * then the following fields among the fields above are reserved:
     * - HARQ process number
     * - Downlink Assignment Index
     * (used for TDD only and is not present in FDD)
     */
    *index = idx;

    return SUCCESS;
}

/** Add API message to the head of a MAC-PHY message queue
 *
 * @param MessageType PHY API message ID (LtePhyL2Api.h)
 * @param MessageLen Message size
 * @param MessagePtr Pointer to the message buffer
 * @return Standard result code
 */
static RESULTCODE ApiListAddToHead(U8 MessageType, U32 MessageLen, U8* MessagePtr)
{
    PMAC2PHY_QUEUE_EL tempo = (PMAC2PHY_QUEUE_EL) Mac2PhyIfAllocBufferEx(sizeof(MAC2PHY_QUEUE_EL));

    if (tempo == NULL)
    {
        uart_printf("TestMAC, cpu=%d, cannot allocate I-CORE memory (ApiListAddToHead function)\r\n",
                    MxGetCpuID());
        return FAILURE;
    }

    tempo->frameNumber = getFrameNum();
    tempo->subframeNumber = getSubFrameNum();
    tempo->MessageType = MessageType;
    tempo->MessageLen = MessageLen;
    tempo->MessagePtr = MessagePtr;
    tempo->Next = NULL;
    if (Mac2PhyQueue.StartQueueCurr == NULL)
    {
        Mac2PhyQueue.StartQueueCurr = tempo;
        Mac2PhyQueue.TailQueueCurr = tempo;
    }
    else
    {
        tempo->Next = Mac2PhyQueue.StartQueueCurr;
        Mac2PhyQueue.StartQueueCurr = tempo;
    }
    return SUCCESS;
}

/** Add API message to the head of a MAC-PHY message queue
 *  assuming RX vector is right after TX vector in a linked list
 *
 * @param MessageType PHY API message ID (LtePhyL2Api.h)
 * @param MessageLen Message size
 * @param MessagePtr Pointer to the message buffer
 * @return Standard result code
 */
static U32 ApiListAddToHead2(U8 MessageType, U32 MessageLen, U8* MessagePtr)
{
    PMAC2PHY_QUEUE_EL tempo = (PMAC2PHY_QUEUE_EL) Mac2PhyIfAllocBufferEx(sizeof(MAC2PHY_QUEUE_EL));

    if (tempo == NULL)
    {
        uart_printf("TestMAC, cpu=%d, cannot allocate I-CORE memory (ApiListAddToHead2 function)\r\n",
                    MxGetCpuID());
        return FAILURE;
    }

    tempo->frameNumber = getFrameNum();
    tempo->subframeNumber = getSubFrameNum();
    tempo->MessageType = MessageType;
    tempo->MessageLen = MessageLen;
    tempo->MessagePtr = MessagePtr;
    tempo->Next = NULL;
    if (Mac2PhyQueue.StartQueueCurr == NULL)
    {
        Mac2PhyQueue.StartQueueCurr = tempo;
        Mac2PhyQueue.TailQueueCurr = tempo;
    }
    else
    {
        PMAC2PHY_QUEUE_EL tempo2 = Mac2PhyQueue.StartQueueCurr->Next;
        if (tempo2)
        {
            // Insert right after head
            Mac2PhyQueue.StartQueueCurr->Next = tempo;
            tempo->Next = tempo2;
        }
        else
        {
            // Add to tail
            Mac2PhyQueue.TailQueueCurr->Next = tempo;
            Mac2PhyQueue.TailQueueCurr = tempo;
        }
    }
    return SUCCESS;
}

/** Add API message to the tail of a MAC-PHY message queue
 *
 * @param MessageType PHY API message ID (LtePhyL2Api.h)
 * @param MessageLen Message size
 * @param MessagePtr Pointer to the message buffer
 * @return Standard result code
 */
static RESULTCODE ApiListAddToTail(U8 MessageType, U32 MessageLen, U8* MessagePtr)
{
    PMAC2PHY_QUEUE_EL tempo = (PMAC2PHY_QUEUE_EL) Mac2PhyIfAllocBufferEx(sizeof(MAC2PHY_QUEUE_EL));

    if (tempo == NULL)
    {
        uart_printf("TestMAC, cpu=%d, cannot allocate I-CORE memory (ApiListAddToTail function)\r\n",
                    MxGetCpuID());
        return FAILURE;
    }

    tempo->frameNumber = getFrameNum();
    tempo->subframeNumber = getSubFrameNum();
    tempo->MessageType = MessageType;
    tempo->MessageLen = MessageLen;
    tempo->MessagePtr = MessagePtr;
    tempo->Next = NULL;
    if (Mac2PhyQueue.StartQueueCurr == NULL)
    {
        Mac2PhyQueue.StartQueueCurr = tempo;
        Mac2PhyQueue.TailQueueCurr = tempo;
    }
    else
    {
        Mac2PhyQueue.TailQueueCurr->Next = tempo;
        Mac2PhyQueue.TailQueueCurr = tempo;
    }

    return SUCCESS;
}

/** Free API list of a MAC-PHY message queue
 *
 * @return Standard result code
 */
static RESULTCODE ApiListFree(void)
{
    PMAC2PHY_QUEUE_EL Curr, Prev;

    Curr = Mac2PhyQueue.StartQueueCurr;

    while(Curr)
    {
        if(Curr->MessagePtr)
        {
            Mac2PhyIfFreeBuffer((PGENMSGDESC)Curr->MessagePtr);
            Curr->MessagePtr = NULL;
        }
        Prev = Curr;
        Curr = Curr->Next;
        Prev->Next = NULL;
        Mac2PhyIfFreeBuffer((PGENMSGDESC)Prev);
    }

    Mac2PhyQueue.StartQueueCurr = NULL;
    Mac2PhyQueue.TailQueueCurr = NULL;

    return SUCCESS;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/



/** Enables or Disables I-CPU interface emulation in MAC-PHY communication
 *
 * @param nFlag 1 = Enable Emulation
 *              0 = Disable Emulation
 */
void Mac2PhyEnableICPUEmul(UINT32 nFlag)
{
    Mac2PhyICPUEmulation = nFlag;
}

/** Checks if MAC-PHY interface runs in I-CPU emulated mode
 *
 * @return 0 - I-Core mode
 *         1 - Emulated I-CPU mode
 */
UINT32 IsICPUEmulated(void)
{
    return Mac2PhyICPUEmulation;
}

/** Print LTE PHY API message to console output
 *  FIXME: Move to diagnostics module
 *
 * @param vector Pointer to the PHY API message (84xxx-SWG-009-A)
 * @return Standard result code
 */
RESULTCODE DiagDumpPhyApi(PTR vector)
{
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
                    printf("      pSubCh->blockCodeConcatenation=%i\n",
                           pSubCh->blockCodeConcatenation);
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
                           pMapping->reselmInfo[k].startRes, pMapping->reselmInfo[k].numRes);
                }

                pDlCh++;
            }

            pDciCh = (DCICHANNELDESC *) ((U32) pDlSf + (U32) pDlSf->offsetDCIChannels);
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
                pPwrCtl = (DLCMNTXPWRCTL*) ((U32) pDlSf + (U32) pDlSf->offsetPowerCtrl);
                printf("  pPwrCtl->pilotPower=%i\n", pPwrCtl->pilotPower);
                printf("  pPwrCtl->psyncPower=%i\n", pPwrCtl->psyncPower);
                printf("  pPwrCtl->ssyncPower=%i\n", pPwrCtl->ssyncPower);
                printf("  pPwrCtl->ciPower=%i\n", pPwrCtl->ciPower);
//                printf("  pPwrCtl->paprControl=%i\n", pPwrCtl->paprControl);
//                printf("  pPwrCtl->paprThreshold=%i\n", pPwrCtl->paprThreshold);
            }
            break;

        case PHY_TXSDU_REQ:
            pTxSduReq = (PTXSDUREQ) pGen;
            printf("  pTxSdu->nackAck=%i\n", pTxSduReq->nackAck);
            printf("  pTxSdu->uciFormat=%i\n", pTxSduReq->uciFormat);
            printf("  pTxSdu->channelType=%i\n", pTxSduReq->channelType);
            printf("  pTxSdu->phichSeqIndex =%i\n", pTxSduReq->phichSeqIndex);
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

                // FIXME: This is debug check and needs to be removed
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
            printf("  pUlSf->numberOfCtrlChannelDescriptors=%i\n",
                   pUlSf->numberOfCtrlChannelDescriptors);
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
            printf("    pUlCmnCtrl->ackNackSRSSimultaneousTransmission=%i\n",
                   pUlCmnCtrl->ackNackSRSSimultaneousTransmission);
            printf("    pUlCmnCtrl->nSB=%i\n", pUlCmnCtrl->nSB);
            printf("    pUlCmnCtrl->hoppingMode=%i\n", pUlCmnCtrl->hoppingMode);
            printf("    pUlCmnCtrl->puschhoppingOffset=%i\n", pUlCmnCtrl->puschhoppingOffset);
            printf("    pUlCmnCtrl->enable64QAM=%i\n", pUlCmnCtrl->enable64QAM);
            printf("    pUlCmnCtrl->groupHoppingEnabled=%i\n", pUlCmnCtrl->groupHoppingEnabled);
            printf("    pUlCmnCtrl->groupAssignmentPUSCH=%i\n", pUlCmnCtrl->groupAssignmentPUSCH);
            printf("    pUlCmnCtrl->sequenceHoppingEnabled=%i\n",
                   pUlCmnCtrl->sequenceHoppingEnabled);
            printf("    pUlCmnCtrl->cyclicShift=%i\n", pUlCmnCtrl->cyclicShift);

            pUlCh = &pUlSf->ulCh[0];
            for (i = 0; (int) i < (pUlSf->numberofChannelDescriptors
                - pUlSf->numberOfCtrlChannelDescriptors); i++)
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
                printf("    pUlSubCh->mcinfo.codingDescriptor=%i\n",
                       pUlSubCh->mcinfo.codingDescriptor);
                printf("    pUlSubCh->mcinfo.blockCodeConcatenation=%i\n",
                       pUlSubCh->mcinfo.blockCodeConcatenation);
                printf("    pUlSubCh->mcinfo.modulationType=%i\n", pUlSubCh->mcinfo.modulationType);
                printf("    pUlSubCh->mcinfo.mcsType=%i\n", pUlSubCh->mcinfo.mcsType);
                printf("    pUlSubCh->harqInfo.nDi=%i\n", pUlSubCh->harqInfo.nDi);
                printf("    pUlSubCh->harqInfo.rV=%i\n", pUlSubCh->harqInfo.rV);
                printf("    pUlSubCh->harqInfo.flushReq=%i\n", pUlSubCh->harqInfo.flushReq);
                printf("    pUlSubCh->crcInfo.crcLength=%i\n", pUlSubCh->crcInfo.crcLength);
                printf("    pUlSubCh->crcInfo.crcScrambling=%i\n", pUlSubCh->crcInfo.crcScrambling);
                printf("    pUlSubCh->scrInfo.scramblerType=%i\n", pUlSubCh->scrInfo.scramblerType);
                printf("    pUlSubCh->scrInfo.scrinitValueinput=%i\n",
                       pUlSubCh->scrInfo.scrinitValueinput);
                printf("    pUlSubCh->puschDed.betaOffsetACKIndex=%i\n", pUlSubCh->puschDed.betaOffsetACKIndex);
                printf("    pUlSubCh->puschDed.betaOffsetRIIndex=%i\n", pUlSubCh->puschDed.betaOffsetRIIndex);
                printf("    pUlSubCh->puschDed.betaOffsetCQIIndex=%i\n", pUlSubCh->puschDed.betaOffsetCQIIndex);
                printf("    pUlSubCh->puschDed.nDMRS2=%i\n", pUlSubCh->puschDed.nDMRS2);
                printf("    pUlSubCh->puschDed.nsymi=%i\n", pUlSubCh->puschDed.nsymi);
                printf("    pUlSubCh->puschDed.nACK=%i\n", pUlSubCh->puschDed.nACK);
                printf("    pUlSubCh->puschDed.nrbi=%i\n", pUlSubCh->puschDed.nrbi);
                printf("    pUlSubCh->puschDed.nRI=%i\n", pUlSubCh->puschDed.nRI);
                printf("    pUlSubCh->puschDed.nr1CQI=%i\n", pUlSubCh->puschDed.nr1CQI);
                printf("    pUlSubCh->puschDed.nrg1CQI=%i\n", pUlSubCh->puschDed.nrg1CQI);

                printf("    pUlSubCh->cqiPmiRiRpt.cqiReportModeAperiodic=%i\n",
                       pUlSubCh->cqiPmiRiRpt.cqiReportModeAperiodic);
                printf("    pUlSubCh->cqiPmiRiRpt.nomPDSCHRSEPREOffset=%i\n",
                       pUlSubCh->cqiPmiRiRpt.nomPDSCHRSEPREOffset);
                printf("    pUlSubCh->cqiPmiRiRpt.cqiReportPeriodicEnable=%i\n",
                       pUlSubCh->cqiPmiRiRpt.cqiReportPeriodicEnable);
                printf("    pUlSubCh->cqiPmiRiRpt.cqiPUCCHResourceIndex=%i\n",
                       pUlSubCh->cqiPmiRiRpt.cqiPUCCHResourceIndex);
                printf("    pUlSubCh->cqiPmiRiRpt.cqipmiConfigIndex=%i\n",
                       pUlSubCh->cqiPmiRiRpt.cqipmiConfigIndex);
                printf("    pUlSubCh->cqiPmiRiRpt.cqiFormatIndicatorPeriodic=%i\n",
                       pUlSubCh->cqiPmiRiRpt.cqiFormatIndicatorPeriodic);
                printf("    pUlSubCh->cqiPmiRiRpt.K=%i\n", pUlSubCh->cqiPmiRiRpt.K);
                printf("    pUlSubCh->cqiPmiRiRpt.riConfigIndex=%i\n",
                       pUlSubCh->cqiPmiRiRpt.riConfigIndex);
                printf("    pUlSubCh->cqiPmiRiRpt.simultaneousAckNackAndCQI=%i\n",
                       pUlSubCh->cqiPmiRiRpt.simultaneousAckNackAndCQI);

                printf("    pUlSubCh->mapInfo.numberofEntries=%i\n",
                       pUlSubCh->mapInfo.numberofEntries);
                for (j = 0; j < pUlSubCh->mapInfo.numberofEntries; j++)
                {
                    printf("      pUlSubCh->mapInfo.reselmInfo[%i]=(%i,%i)\n", j,
                           pUlSubCh->mapInfo.reselmInfo[j].startRes,
                           pUlSubCh->mapInfo.reselmInfo[j].numRes);
                }

                printf("    pUlCh->timAdvErrInfo.mErrAvgCh[0]=%i\n", pUlCh->timAdvErrInfo.mErrAvgCh[0]);
                printf("    pUlCh->timAdvErrInfo.mErrAvgCh[1]=%i\n", pUlCh->timAdvErrInfo.mErrAvgCh[1]);
                printf("    pUlCh->timAdvErrInfo.mErrExpo=%i\n", pUlCh->timAdvErrInfo.mErrExpo);
                printf("    pUlCh->timAdvErrInfo.mSetDefaults=%i\n",  pUlCh->timAdvErrInfo.mSetDefaults);

                pUlCh++;
            }

            pUlCtrlCh = (ULCTRLCHDESC *) (((U32) pUlSf) + pUlSf->offsetULCtrlChannels);

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
                printf("    pUlCtrlCh->blockCodeConcatenation=%i\n",
                       pUlCtrlCh->blockCodeConcatenation);
                printf("    pUlCtrlCh->modulationType=%i\n", pUlCtrlCh->modulationType);
                printf("    pUlCtrlCh->mcsType=%i\n", pUlCtrlCh->mcsType);

                printf("    pUlCtrlCh->pucchDedCtl.formatType=%i\n",
                       pUlCtrlCh->pucchDedCtl.formatType);
                printf("    pUlCtrlCh->pucchDedCtl.ackNackRepetition=%i\n",
                       pUlCtrlCh->pucchDedCtl.ackNackRepetition);
                printf("    pUlCtrlCh->pucchDedCtl.repetitionFactor=%i\n",
                       pUlCtrlCh->pucchDedCtl.repetitionFactor);
                printf("    pUlCtrlCh->pucchDedCtl.n1PucchANRep=%i\n",
                       pUlCtrlCh->pucchDedCtl.n1PucchANRep);
                printf("    pUlCtrlCh->pucchDedCtl.cqiPUCCHResourceIndex=%i\n",
                       pUlCtrlCh->pucchDedCtl.cqiPUCCHResourceIndex);
                printf("    pUlCtrlCh->pucchDedCtl.srPUCCHResourceIndex=%i\n",
                       pUlCtrlCh->pucchDedCtl.srPUCCHResourceIndex);
                printf("    pUlCtrlCh->pucchDedCtl.dlCqiPmiSizeBits=%i\n",
                    pUlCtrlCh->pucchDedCtl.dlCqiPmiSizeBits);
                printf("    pUlCtrlCh->pucchDedCtl.harqPucchIndex=%i\n",
                    pUlCtrlCh->pucchDedCtl.harqPucchIndex);
                printf("    pUlCtrlCh->pucchDedCtl.harqSizebits=%i\n",
                    pUlCtrlCh->pucchDedCtl.harqSizebits);
                printf("    pUlCtrlCh->pucchDedCtl.n1PUCCHResourceIndex=%i\n",
                    pUlCtrlCh->pucchDedCtl.n1PUCCHResourceIndex);

                printf("    pUlCtrlCh->cqiPmiRiRpt.cqiReportModeAperiodic=%i\n",
                       pUlCtrlCh->cqiPmiRiRpt.cqiReportModeAperiodic);
                printf("    pUlCtrlCh->cqiPmiRiRpt.nomPDSCHRSEPREOffset=%i\n",
                       pUlCtrlCh->cqiPmiRiRpt.nomPDSCHRSEPREOffset);
                printf("    pUlCtrlCh->cqiPmiRiRpt.cqiReportPeriodicEnable=%i\n",
                       pUlCtrlCh->cqiPmiRiRpt.cqiReportPeriodicEnable);
                printf("    pUlCtrlCh->cqiPmiRiRpt.cqiPUCCHResourceIndex=%i\n",
                       pUlCtrlCh->cqiPmiRiRpt.cqiPUCCHResourceIndex);
                printf("    pUlCtrlCh->cqiPmiRiRpt.cqipmiConfigIndex=%i\n",
                       pUlCtrlCh->cqiPmiRiRpt.cqipmiConfigIndex);
                printf("    pUlCtrlCh->cqiPmiRiRpt.cqiFormatIndicatorPeriodic=%i\n",
                       pUlCtrlCh->cqiPmiRiRpt.cqiFormatIndicatorPeriodic);
                printf("    pUlCtrlCh->cqiPmiRiRpt.K=%i\n", pUlCtrlCh->cqiPmiRiRpt.K);
                printf("    pUlCtrlCh->cqiPmiRiRpt.riConfigIndex=%i\n",
                       pUlCtrlCh->cqiPmiRiRpt.riConfigIndex);
                printf("    pUlCtrlCh->cqiPmiRiRpt.simultaneousAckNackAndCQI=%i\n",
                       pUlCtrlCh->cqiPmiRiRpt.simultaneousAckNackAndCQI);

                printf("    pUlCtrlCh->numberofEntries=%i\n", pUlCtrlCh->numberofEntries);
                printf("    pUlCtrlCh->startRes=%i\n", pUlCtrlCh->startRes);
                printf("    pUlCtrlCh->numRes=%i\n", pUlCtrlCh->numRes);

                printf("    pUlCtrlCh->timAdvErrInfo.mSetDefaults=%i\n", pUlCtrlCh->timAdvErrInfo.mSetDefaults);
                printf("    pUlCtrlCh->timAdvErrInfo.mErrAvgCh[0]=%i\n", pUlCtrlCh->timAdvErrInfo.mErrAvgCh[0]);
                printf("    pUlCtrlCh->timAdvErrInfo.mErrAvgCh[1]=%i\n", pUlCtrlCh->timAdvErrInfo.mErrAvgCh[1]);
                printf("    pUlCtrlCh->timAdvErrInfo.mErrExpo=%i\n", pUlCtrlCh->timAdvErrInfo.mErrExpo);

                pUlCtrlCh++;
            }

            pSrs = (SRSDED *) (((U32) pUlSf) + pUlSf->offsetsrsInfo);
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

            pRachCtrl = (RACHCTRL *) (((U32) pUlSf) + pUlSf->offsetRachCtrlStruct);
            printf("    pRachCtrl->prachEnable=%i\n", pRachCtrl->prachEnable);
            printf("    pRachCtrl->rootSequenceIndex=%i\n", pRachCtrl->rootSequenceIndex);
            printf("    pRachCtrl->prachConfigIndex=%i\n", pRachCtrl->prachConfigIndex);
            printf("    pRachCtrl->highSpeedFlag=%i\n", pRachCtrl->highSpeedFlag);
            printf("    pRachCtrl->zeroCorrelationZoneConfig=%i\n",
                   pRachCtrl->zeroCorrelationZoneConfig);
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
            printf("  pRxSdu->merrAvgCh[0] =%i\n", pRxSduInd->mErrAvgCh[0]);
            printf("  pRxSdu->merrAvgCh[1] =%i\n", pRxSduInd->mErrAvgCh[1]);
            printf("  pRxsdu->mErrExpo =%i\n", pRxSduInd->mErrExpo);
            printf("  pRxSdu->timingAdv=%i\n", (void*) pRxSduInd->timingAdv);
            printf("  pRxSdu->ul_CQI =%i\n", pRxSduInd->ul_CQI);
            printf("  pRxSdu->pRxSdu=%p\n", (void*) pRxSduInd->pRxSdu);

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

                // FIXME: This is debug check and needs to be removed
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
            printf("  pInit->rxSduLineMonitorInfoenable=%i\n", pInit->rxSduLineMonitorInfoenable);
            printf("  pInit->txStartConfDisable=%i\n", pInit->txStartConfDisable);
            printf("  pInit->rxStartConfDisable=%i\n", pInit->rxStartConfDisable);
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
    return SUCCESS;
}

/** Allocates memory block for PHY API message
 *
 * @return Pointer to a new message (84xxx-SWG-009-A) or NULL on failure
 */
PGENMSGDESC Mac2PhyIfAllocBuffer(void)
{
    UINT32 lr = __return_address();
    PGENMSGDESC pMsg = (PGENMSGDESC) SvsrAllocMsg();

    if (pMsg == NULL)
    {
        LtePhyShutdown(NULL, NULL);
        uart_printf("Mac2PhyIfAllocBuffer - failed to allocate memory, cpu-%d, lr=%x (SFN=%d SF=%d)\r\n",
                    MxGetCpuID(), lr, getFrameNum(), getSubFrameNum());
    }
    return (PGENMSGDESC) pMsg;
}

/** Allocates memory block for PHY API message
 *
 * @return Pointer to a new message (84xxx-SWG-009-A) or NULL on failure
 */
PGENMSGDESC Mac2PhyIfAllocBufferEx(UINT32 nSize)
{
    UINT32 lr = __return_address();
    //PGENMSGDESC pMsg = (PGENMSGDESC) SvsrAllocMsgEx(nSize);
    PGENMSGDESC pMsg = (PGENMSGDESC) SvsrAllocMsg();

    if (pMsg == NULL)
    {
        uart_printf("Mac2PhyIfAllocBufferEx - failed to allocate memory, cpu-%d, lr=%x\r\n",
                    MxGetCpuID(), lr);
    }
    return (PGENMSGDESC) pMsg;
}

/** Frees memory block previously allocated by Mac2PhyIfAllocBuffer
 *
 * @param pMsg Pointer to the message block to deallocate
 */
void Mac2PhyIfFreeBuffer(PGENMSGDESC pMsg)
{
    SvsrFreeMsg(pMsg);
}

/** Reroutes received PHY_TXSDU.req message (84xxx-SWG-009-A)
 *  to the associated PHY instance
 *
 * @param pDlSubFrame Pointer to current Downlink subframe descriptor
 * @return Standard result code
 */
RESULTCODE LteMacPhyProcessTxSduReq(PDLSUBFRDESC pDlSubFrame)
{
    Mac2PhyInterface *pPhy = &pBsMacDesc->Phy;
    PTXSDUREQ pSduReq;

    while (MxGetQueueData(dlControlTxSduQueue2, &pSduReq) == SUCCESS)
    {
        pPhy->txCb(pPhy->toPhy, pSduReq, pSduReq->msgLen + sizeof(TXSDUREQ));
    }
    while (MxGetQueueData(dlControlTxSduQueue, &pSduReq) == SUCCESS)
    {
        pPhy->txCb(pPhy->toPhy, pSduReq, pSduReq->msgLen + sizeof(TXSDUREQ));
    }

    return SUCCESS;
}

/** Initialization of RX SDU ring buffer
 *
 * @param Pointer to STARTREQ
 */
static void InitRxSduBuf(void* param)
{
    if (pBsMacDesc->PhyAutoCtx.RxSduEna)
    {
        PSTARTREQ pStartReq = (PSTARTREQ) param;

        if (RxSduBuf)
        {
            MxHeapFree(AppHeap, RxSduBuf);
            RxSduBuf = NULL;
        }

        if (pStartReq->count == 0 || pStartReq->count > 120)
            RxBufSize = RXSDUSZ * 120;
        else
            RxBufSize = RXSDUSZ * pStartReq->count;

        RxSduBuf = MxHeapAlloc(AppHeap, RxBufSize);
        if (RxSduBuf == NULL)
        {
            uart_printf("RxSduBuf alloc error\n");
            pBsMacDesc->PhyAutoCtx.RxSduEna = FALSE;
        }
        uart_printf("RxSdu Indication log: 0x%x Size: %d\n", RxSduBuf, RxBufSize);
        RxSduSet = 0;
        RxSduGet = 0;
        RxSduCntr = 0;
    }
}

/** Copies new RX SDU to the ring buffer
 *
 * @param Buf Pointer to the TX SDU
 * @param Size Size of this SDU
 */
static void AddToBuf(U8 *Buf, U32 Size)
{
    if (((RxSduSet + Size) <= RxBufSize) && (RxSduBuf != NULL))
    {
        _memcpy(RxSduBuf + RxSduSet, Buf, Size);
        RxSduSet += ROUND(Size,sizeof(U32));
        RxSduCntr++;
    }
}

/** @breif Forwards contents of the internal ring buffer of received SDUs to Host
 *         like a single I-CPU message
 *
 *  @return Number of RX SDUs send per single I-CPU message
 */
UINT32 DrainRxSduBufToHost(void)
{
    ApiHeader* pInd = NULL;

    // Alloc I-CPU message
    pInd = SvsrAllocICApi();
    if (pInd == NULL)
    {
        uart_printf("No memory in I-CPU block\r\n");
        return 0;
    }

    // Create and send I-CPU message
    pInd = ApiCreate(pInd, API_DEF_FLAGS, IID_LTE_MAC, UARM_INSTID_HOST,
                     API_TYPE_IND, MAC_RXBUFDRAIN_IND);

    ApiAddParam(pInd, NULL, PAR_BIN_DATA, RxSduBuf, RxSduSet);

    SvsrSendICApi(pInd);

    // Calculate DrainRx messages
    DrainRxCount++;

    return RxSduCntr;
}

static U32 SendSduToHostRepeatable(U32 rep, ApiHeader *pInd, PRXSDUIND pRxSduInd, U32 type)
{
    U32     idx;
    U32     startTime, endTime;
    MXRC    rc = RC_SVSR_ICPU_W_ERR;
    U32     pTmp;

    for(idx = 0; idx < rep; idx ++ )
    {
        startTime = MxGetTicks();
        pTmp = (U32)pInd;
        rc = (U32)SvsrSendICApi(pInd);
        endTime = MxGetTicks();
        MxDelayTicks(150000);
        if (numSvsrApiMsgStats<SIZEOF_SVSRAPIMSGSTAT-1)
        {
            SvsrApiMsgStats[numSvsrApiMsgStats].start    = startTime;
            SvsrApiMsgStats[numSvsrApiMsgStats].diff     = (U32)(endTime-startTime);
            SvsrApiMsgStats[numSvsrApiMsgStats].physaddr = (U32)(pTmp);
            SvsrApiMsgStats[numSvsrApiMsgStats].frame    = (U32)(((pRxSduInd->frameNum)<<16) | pRxSduInd->subFrameNum);
            SvsrApiMsgStats[numSvsrApiMsgStats].type     = type;

            numSvsrApiMsgStats++;
        }

        if(rc)
        {
            IcpuFlagErr = 1;
            MxDelayTicks(100000);
        }
        else
            break;
    }
    return rc;
}

typedef struct _LTEMAC_BUFTOHOST_CTX_
{
    HANDLE      hThread;
    HANDLE      hTimer;
    HANDLE      hEvent;

}LTEMAC_BUFTOHOST_CTX, *PLTEMAC_BUFTOHOST_CTX;

LTEMAC_BUFTOHOST_CTX BufToHostDelayCtx;

UINT32 SendBufToHostThread (void * p)
{
    SendBufToHost();
    MxHeapFree(AppHeap, RxSduBuf);
    RxSduBuf = NULL;
    RxSduSet = 0;
    RxSduGet = 0;
    RxSduCntr = 0;

    {
        ApiHeader* pInd = NULL;
        UINT32 LastEna, rc = MX_OK;
        UINT32 idx,rep = 10;

        for(idx = 0; idx < rep; idx ++ )
        {
            pInd = SvsrAllocICApi();
            if(pInd == 0)
            MxDelayTicks(100000);
        else
            break;
        }
        if (pInd == NULL)
        {
            uart_printf("TestMac WARNING: No memory in I-CPU block\r\n");
            return MAC_FAILURE;
        }
        pInd = ApiCreate(pInd, API_DEF_FLAGS, IID_LTE_MAC, UARM_INSTID_HOST,
            API_TYPE_IND, MAC_RXSDU_IND);

        LastEna = 1;
        ApiAddParam(pInd, NULL, PAR_LAST_RXSDU, &LastEna, 4);

        for(idx = 0; idx < rep; idx ++ )
        {
            rc = SvsrSendICApi(pInd);
            if(rc)
                MxDelayTicks(100000);
            else
                break;
        }
        if(rc != MX_OK)
        {
            uart_printf("TestMac WARNING: Last RxSdu skipped\r\n");
            SvsrFreeICApi(pInd);
        }
        else
            uart_printf("TestMac INFO: All SDUs have been sent to u-arm\n");
    } /*end sending PAR_LAST_RXSDU*/

    ApiListFree();

    return 0;
}

#define SEND_BUFTOHOST_DELAY_MS 10

RESULTCODE SendBufToHostPostponed(void)
{
    RESULTCODE rc = SUCCESS;
    PLTEMAC_BUFTOHOST_CTX pCtx = &BufToHostDelayCtx;

    memset(pCtx, 0, sizeof(LTEMAC_BUFTOHOST_CTX));

    if (FAILED(rc = MxCreateEvent (EVENT_MODE_RESET, NULL, &pCtx->hEvent)))
    {
        return rc;
    }

    if (FAILED(rc = MxCreateTimer(TIMER_MODE_ONE_TIME, SEND_BUFTOHOST_DELAY_MS, NULL, pCtx->hEvent, &pCtx->hTimer)))
    {
        MxRemoveEvent (pCtx->hEvent);
        return rc;
    }

    if (FAILED(rc = MxCreateThread(SendBufToHostThread, NULL, NULL, 0, &pCtx->hThread)))
    {
        MxRemoveTimer (pCtx->hTimer);
        MxRemoveEvent (pCtx->hEvent);
        return rc;
    }

    rc = MxScheduleThreadByEvent(pCtx->hThread, pCtx->hEvent, THREAD_EVENT_AUTO_RESET);

    if (FAILED(rc))
    {
        MxRemoveThread(pCtx->hThread);
        MxRemoveTimer (pCtx->hTimer);
        MxRemoveEvent (pCtx->hEvent);

        return rc;
    }

    rc = MxStartTimer (pCtx->hTimer);

    if (FAILED(rc))
    {
        MxRemoveThread(pCtx->hThread);
        MxRemoveTimer (pCtx->hTimer);
        MxRemoveEvent (pCtx->hEvent);

        return rc;
    }

    return rc;
}


/** Forwards contents of the internal ring buffer of received SDUs to Host
 *
 * @return Number of RX SDUs processed
 */
UINT32 SendBufToHostThr(void)
{
    PRXSDUIND pRxSduInd = NULL;
    PRXSTATUSIND pRxStatusInd = NULL;
    U32* pMacPdu;
    ApiHeader* pInd = NULL;
    U32 Len = 0;
    U32 Data, ChType;
    U32 RxSduSentCntr = 0;
    UINT32 nNumLogSdu = RxSduCntr;
    UINT32 nSduOffs = 0;
    UINT32 idx, rep = 10, rc = MX_OK;

    uart_printf("TestMac INFO: Sending RXSDU (num=%d) to the host ...\r\n", nNumLogSdu);

    while ((nNumLogSdu--) && (RxSduBuf != NULL))
    {
    pRxSduInd = (PRXSDUIND) ((U8*) RxSduBuf + nSduOffs);
    switch(pRxSduInd->msgType)
        {
        case PHY_RXSTATUS_IND:

            pRxStatusInd = (PRXSTATUSIND) pRxSduInd;
            if (pRxStatusInd->statusType == PRACH_RESULT)
            {
                ChType = PRACH;
                Len = pRxStatusInd->eventNumber * sizeof(RX_PRACH_STATUS_EVENT);
            }
            else if(pRxStatusInd->statusType == CQIRIHI_RESULT)
            {
                PRX_CQIRIHI_STATUS_EVENT pMsg1= (PRX_CQIRIHI_STATUS_EVENT)&pRxStatusInd->pEventList;
                ChType = CQIRIHI_RESULT;
                Len = pRxStatusInd->eventNumber * sizeof(RX_CQIRIHI_STATUS_EVENT) + pMsg1->status_len;
            }
            else if(pRxStatusInd->statusType == SRS_RESULT)
            {
                ChType = SRS_RESULT;
                Len = pRxStatusInd->eventNumber * (sizeof(RX_SRS_STATUS_EVENT) - sizeof(U8*)) + pRxStatusInd->msgSpecific /*always multiple of 4*/;
            }
            else
            {
                return RxSduSentCntr;
            }
            if (Len >= RXSDUSZ)
            {
                uart_printf("TestMac WARNING: SDU FN#%d SFN#%d size %i skipped\r\n", pRxSduInd->frameNum, pRxSduInd->subFrameNum, Len);
                return RxSduSentCntr;
            }
            pMacPdu = &pRxStatusInd->pEventList;


            for(idx = 0; idx < rep; idx ++ )
            {
                pInd = SvsrAllocICApi();
                if(pInd == NULL)
                    MxDelayTicks(100000);
                else
                    break;
            }
            if (pInd == NULL)
            {
                uart_printf("TestMac WARNING: No memory in I-CPU block\r\n");
                return RxSduSentCntr;
            }

            pInd = ApiCreate(pInd, API_DEF_FLAGS, IID_LTE_MAC, UARM_INSTID_HOST,
                             API_TYPE_IND, MAC_RXSTATUS_IND);

            Data = (U32) pRxStatusInd->frameNum;
            ApiAddParam(pInd, NULL, PAR_PHY_SFN, &Data, 4);

            Data = (U32) pRxStatusInd->subFrameNum;
            ApiAddParam(pInd, NULL, PAR_PHY_SFCOUNT, &Data, 4);

            ApiAddParam(pInd, NULL, PAR_PHY_CH_TYPE, &ChType, 4);

            ApiAddParam(pInd, NULL, PAR_BIN_DATA, pMacPdu, Len);

            rc = SendSduToHostRepeatable(rep, pInd, pRxSduInd, pRxSduInd->msgType);

            if(rc != MX_OK)
            {
                uart_printf("TestMac WARNING: SDU FN#%d SFN#%d skipped\r\n", pRxSduInd->frameNum, pRxSduInd->subFrameNum);
                SvsrFreeICApi(pInd);
            }
            else
                RxSduSentCntr ++;

            nSduOffs += ROUND((Len + sizeof(RXSTATUSIND)) , sizeof(U32));
            break;


        case PHY_RXSDU_IND:

            if(pRxSduInd->chanType == PUCCH)
                Len = ((pRxSduInd->numBitsRx & 0xFFFF) + 7) >> 3;
            else
                Len = (pRxSduInd->numBitsRx + 7) >> 3;

            if (Len >= RXSDUSZ)
            {
                uart_printf("TestMac WARNING: SDU FN#%d SFN#%d size %i skipped\r\n", pRxSduInd->frameNum, pRxSduInd->subFrameNum, Len);
                return RxSduSentCntr;
            }
            pMacPdu = &pRxSduInd->pRxSdu;

            for(idx = 0; idx < rep; idx ++ )
                {
                    pInd = SvsrAllocICApi();
                    if(pInd == NULL)
                        MxDelayTicks(100000);
                    else
                        break;
                }
            if (pInd == NULL)
            {
                uart_printf("TestMac WARNING: No memory in I-CPU block\r\n");
                return RxSduSentCntr;
            }
            pInd = ApiCreate(pInd, API_DEF_FLAGS, IID_LTE_MAC, UARM_INSTID_HOST,
                            API_TYPE_IND, MAC_RXSDU_IND);

            Data = (U32) pRxSduInd->frameNum;
            ApiAddParam(pInd, NULL, PAR_PHY_SFN, &Data, 4);

            Data = (U32) pRxSduInd->subFrameNum;
            ApiAddParam(pInd, NULL, PAR_PHY_SFCOUNT, &Data, 4);

            Data = (U32) pRxSduInd->chanType;
            ApiAddParam(pInd, NULL, PAR_PHY_CH_TYPE, &Data, 4);

            Data = (U32)pRxSduInd->channelId;
            ApiAddParam(pInd, NULL, PAR_PHY_CH_IDX, &Data, 4);

            Data = (U32)pRxSduInd->mErrAvgCh[0];
            ApiAddParam(pInd, NULL, PAR_M_ERR_AVG_CH_0, &Data, 4);

            Data = (U32)pRxSduInd->mErrAvgCh[1];
            ApiAddParam(pInd, NULL, PAR_M_ERR_AVG_CH_1, &Data, 4);

            Data = (U32)pRxSduInd->mErrExpo;
            ApiAddParam(pInd, NULL, PAR_M_ERR_EXPO, &Data, 4);

            Data = (U32)pRxSduInd->timingAdv;
            ApiAddParam(pInd, NULL, PAR_TIMING_ADV, &Data, 4);

            ApiAddParam(pInd, NULL, PAR_BIN_DATA, pMacPdu, Len);


            rc = SendSduToHostRepeatable(rep, pInd, pRxSduInd, pRxSduInd->msgType);

            if(rc != MX_OK)
            {

                uart_printf("TestMac WARNING: SDU FN#%d SFN#%d skipped\r\n", pRxSduInd->frameNum, pRxSduInd->subFrameNum);
                SvsrFreeICApi(pInd);
            }
            else
                RxSduSentCntr ++;

            nSduOffs += ROUND((Len + sizeof(RXSDUIND)), sizeof (U32));
            break;

        default:

            uart_printf("TestMac WARNING: [%x] unknown message type\n", pRxSduInd->msgType);
            uart_printf("TestMac INFO: %d of %d Rx SDUs has been received to HOST\r\n", RxSduSentCntr, RxSduCntr);
            return RxSduSentCntr;

        }
    }

    uart_printf("TestMac INFO: DONE! (%d of %d SDUs)\r\n", RxSduSentCntr, RxSduCntr);
    return RxSduSentCntr;
}

/** Send contents of the RX SDU ring buffer to Host
 *  FIXME: Merge with the SendBufToHostThr or create a thread for latter
 *
 */
static void SendBufToHost(void)
{
    ApiHeader* pInd = NULL;

    // Drain mode processing...
    if (MacDrainRxSduModeOn())
    {
        if (RxSduSet)
        {
            // Drain buffer
            DrainRxSduBufToHost();

            // Calculate DrainRx messages
            DrainRxCount++;
        }

        // Provide diag info and clear counter
        uart_printf("All DrainRxSDU messages has been sent (#=%d)\n", DrainRxCount);
        DrainRxCount = 0;

        return;
    }

    // Normal mode processing...
    if (!RxSduSet)
    {
        pInd = SvsrAllocICApi();
        if (pInd == NULL)
        {
            return;
        }
        pInd = ApiCreate(pInd, API_DEF_FLAGS, IID_LTE_MAC, UARM_INSTID_DEFMAC, API_TYPE_IND,
                         MAC_RXSDU_IND);

        ApiAddErrorCode(pInd, FAILURE);
        SvsrSendICApi(pInd);
        uart_printf("RxSdu buffer is empty\r\n");
        return;
    }
    SendBufToHostThr();
}


static void LteMacPhyCfgProgramInfo(U32 ConsoleVal, U32 Mode)
{
    if (ConsoleVal & PHYINIT_NEW_MAC_TO_PHY_INTERFACE)
    {
        uart_printf("MAC 2 PHY FIFO %s\n", Mode? "at phy enabled": "received from console");
    }
    if (ConsoleVal & PHYINIT_PHY_MAINTAINS_PBCH)
    {
        uart_printf("PBCH generation %s\n", Mode? "at phy enabled": "received from console");
    }
    if (ConsoleVal & PHYINIT_ROBUST_CONTROL)
    {
        uart_printf("robust-control %s\n", Mode? "at phy enabled": "received from console");
    }
    if (ConsoleVal & PHYINIT_USE_TXSDU_POINTER)
    {
        uart_printf("use TxSdu pointer %s\n", Mode? "at phy enabled": "received from console");
    }
    if (ConsoleVal & PHYINIT_TWO_MUXCONTROL_LISTS)
    {
        uart_printf("two mux control lists %s\n", Mode? "at phy enabled": "received from console");
    }
    if (ConsoleVal & PHYINIT_DONT_FREE_NEXT_MISSED_VECTOR)
    {
        uart_printf("don't free next missed vector %s\n", Mode? "at phy enabled": "received from console");
    }
    if (ConsoleVal & PHYINIT_PHY_SEND_SECOND_TTI_IND)
    {
        uart_printf("phy send second tti ind %s\n", Mode? "at phy enabled": "received from console");
    }
    if (ConsoleVal & PHYINIT_MUTE_RXSDU_FOR_SR_RACH)
    {
        uart_printf("mute rxsdu for sr rach %s\n", Mode? "at phy enabled": "received from console");
    }
    if (ConsoleVal & PHYINIT_LOW_LATENCY_PATH)
    {
        uart_printf("low latency path %s\n", Mode? "at phy enabled": "received from console");
    }
}


/** Send API message (84xxx-SWG-009-A) to the PHY using legacy interface
 *
 * @param MessageType PHY API message ID (LtePhyL2Api.h)
 * @param param Message-dependent parameter
 * @return Standard result code
 */
RESULTCODE LteMacSendToPhyMessage(U32 MessageType, PTR param, U32 flag)
{
    PGENMSGDESC pToPhyMsg = NULL;
    PULSUBFRDESC pUlSubFrame;
    PDLSUBFRDESC pDlSubFrame;
    U32 msgSize = 0;
    PTR payload = NULL;
    U16 len;
    PGENMSGDESC buffer = NULL;


    /*UMTSNMM*/
    U16 UmtsnmmCmdType = 0xffff;
    

	T_3G_INIT_NWL *pUmtsNmmInitMsg;
	T_3G_Stop_NWL *pStopUmtsNmmMsg;
	T_3G_CELL_SEARCH_REQ *pUmtsNmmCSReqMsg;
	T_3G_STOP_CELL_SEARCH_REQ *pUmtsNmmCSStopMsg;
	
	T_3G_BCH_CONFIG_REQ *pUmtsNmmBchReqMsg;
	T_3G_STOP_PBCH_REQ	*pUmtsNmmBchStopMsg;


    switch (MessageType)
    {
        case PHY_TXSTART_REQ:
        case PHY_RXSTART_REQ:
        case PHY_INIT_REQ:
        case PHY_START_REQ:
        case PHY_STOP_REQ:
        case PHY_SHUTDOWN_REQ:
        case PHY_RECONFIG_REQ:
            buffer = Mac2PhyIfAllocBufferEx(MAX(sizeof (DLSUBFRDESC), sizeof (ULSUBFRDESC)));
            if (buffer == NULL)
            {
                printf("ERROR: No memory [%s,%s:%d]\n", __FUNCTION__, __FILE__, __LINE__);
                return FAILURE;
            }
            pToPhyMsg = (PGENMSGDESC) buffer;
            // This is index in PhyInst[] array
            pToPhyMsg->phyEntityId = 0;
            pToPhyMsg->msgType = MessageType;
            payload = (PTR) (pToPhyMsg + 1);
            break;


            case UMTSNMM_CMD:
                UmtsnmmCmdType = *((U16 * ) param);

                if (UmtsnmmCmdType >= UMTSNMM_CMD_NUM)
                {
                	uart_printf("ERROR: unsupported UMTSNMM command %d [%s,%s:%d]\n", UmtsnmmCmdType, __FUNCTION__, __FILE__, __LINE__);
                    return FAILURE;
                }

                buffer = Mac2PhyIfAllocBufferEx(MAX(sizeof (DLSUBFRDESC), sizeof (ULSUBFRDESC)));
                if (buffer == NULL)
                {
                    printf("ERROR: No memory [%s,%s:%d]\n", __FUNCTION__, __FILE__, __LINE__);
                    return FAILURE;
                }

                pToPhyMsg = (PGENMSGDESC) buffer;
                pToPhyMsg->msgSpecific = sizeof(GENMSGDESC);	//for request without parameters
                pToPhyMsg->phyEntityId = 0;
                pToPhyMsg->msgType = umtsnmm_message_types[UmtsnmmCmdType];
                payload = (PTR) (pToPhyMsg + 1);                
            break;

        default:
            break;
    }

    switch (MessageType)
    {
        case PHY_TXSTART_REQ:
            pDlSubFrame = (PDLSUBFRDESC) param;
            len = lte_if_copy_tx_vector(payload, pDlSubFrame);
            if (pToPhyMsg)
            {
                pToPhyMsg->msgSpecific = len;
            }

            msgSize = len + sizeof(GENMSGDESC);

            TxVectorCntrSent++;

            ApiListAddToHead(PHY_TXSTART_REQ, msgSize, (U8*) buffer);
            break;

        case PHY_TXSDU_REQ:
            pDlSubFrame = (PDLSUBFRDESC) param;
            return LteMacPhyProcessTxSduReq(pDlSubFrame);

        case PHY_RXSTART_REQ:
        {
            pUlSubFrame = (PULSUBFRDESC) param;
            len = lte_if_copy_rx_vector(payload, pUlSubFrame);

            if (pToPhyMsg)
            {
                pToPhyMsg->phyEntityId = 0;
                pToPhyMsg->msgSpecific = len;
            }

            msgSize = len + sizeof(GENMSGDESC);

            RxVectorCntrSent++;

            ApiListAddToHead2(PHY_RXSTART_REQ, msgSize, (U8*) buffer);
            break;
        }

        case PHY_INIT_REQ:
        {
            PINITPARM pParm = (PINITPARM) payload;
            PBS_STATIC_CONFIG pbs_static_config = get_static_config();
            // Copy config to the message

            if (pbs_static_config->use_internal_etm_test)
            {
                uart_printf("WARNING!!!!! hardcoded pParm->phyLayerId = 1 & powers to zerro\r\n");
                pBsMacDesc->initParm.phyLayerId = 1;
                pBsMacDesc->initParm.referenceSignalPower = 0;
                pBsMacDesc->initParm.primarySyncSignalPower = 0;
                pBsMacDesc->initParm.secondarySyncSignalPower = 0;
                pBsMacDesc->initParm.phyCfg = PHYINIT_NEW_MAC_TO_PHY_INTERFACE;// | PHYINIT_PHY_MAINTAINS_PBCH;
                pBsMacDesc->initParm.txStartConfDisable = 0;
                pBsMacDesc->initParm.rxStartConfDisable = 0;
            }
            else
            {
#ifndef AC_MSPD_TDD
                if(param)
                {// console case - highest priority
                    LteMacPhyCfgProgramInfo(*(U32*)param, 0);
                    pBsMacDesc->initParm.phyCfg |= ((*(U32*)param) & 0x3ff);
                }            
#endif
            }

            if (pParm)
            {
                memcpy(pParm, &pBsMacDesc->initParm, sizeof(INITPARM));
            }
            // This is index in PhyInst[] array
            if (pToPhyMsg)
            {
                pToPhyMsg->phyEntityId = 0;
                pToPhyMsg->msgSpecific = sizeof(INITPARM);
            }
            msgSize = sizeof(GENMSGDESC) + sizeof(INITPARM);

            gMac2PhyDirectFIFOUsed = (pParm->phyCfg & PHYINIT_NEW_MAC_TO_PHY_INTERFACE)? 1 : 0;

            LteMacPhyCfgProgramInfo(pParm->phyCfg, 1);

            pHiDciApiSet = lte_phich_dci_fake;

            ApiListFree();

            Mac2PhyQueue.pAddToTail = ApiListAddToTail;
            uart_printf("\r\nLinked List MAC<=>PHY interface is enabled\r\n");

            uart_printf("MAC 2 PHY queue is set\n");

            uart_printf("New HI_DCI0 API supported mode\n");
            pHiDciApiSet = lte_phich_dci_set;
            LteBsMacDlSchedulerReconfigByPhyMode();

            // if we use new MAC<=>PHY interface
            // just send this command to the new instance

            MacSendToPhy(PHY_INIT_REQ, buffer, msgSize);
        }
        break;

        case PHY_RECONFIG_REQ:
            {
                PINITPARM pParm = (PINITPARM) payload;

                if(param)
                {// console case - highest priority
                    LteMacPhyCfgProgramInfo(*(U32*)param, 0);
                    pBsMacDesc->initParm.phyCfg |= ((*(U32*)param) & 0x3ff);
                }  

                if (pParm)
                {
                    memcpy(pParm, &pBsMacDesc->initParm, sizeof(INITPARM));
                }

                if (pToPhyMsg)
                {
                    pToPhyMsg->phyEntityId = 0;
                    pToPhyMsg->msgSpecific = sizeof(INITPARM);
                }
                msgSize = sizeof(GENMSGDESC) + sizeof(INITPARM);

                gMac2PhyDirectFIFOUsed = (pParm->phyCfg & PHYINIT_NEW_MAC_TO_PHY_INTERFACE)? 1 : 0;

                LteMacPhyCfgProgramInfo(pParm->phyCfg, 1);

                pHiDciApiSet = lte_phich_dci_fake;

                ApiListFree();

                Mac2PhyQueue.pAddToTail = ApiListAddToTail;
                uart_printf("\r\nLinked List MAC<=>PHY interface is enabled\r\n");
                pHiDciApiSet = lte_phich_dci_set;
                LteBsMacDlSchedulerReconfigByPhyMode();

                // if we use new MAC<=>PHY interface
                // just send this command to the new instance

                MacSendToPhy(PHY_RECONFIG_REQ, buffer, msgSize);
            }
            break;


        case PHY_START_REQ:
            globalTTITickCount = pBsMacDesc->initParm.radioFrameNumber * 10 + pBsMacDesc->initParm.subframeNumber;
            setFrameSubFrameNum();
            setTxFrameSubFrameNum();

            pDlSubFrame = &DlSubFrame;
            pUlSubFrame = &UlSubFrame;

            ApiListFree();
            MacFreeRxSduQueue();

            // store API logger mask as PHY may reset it
            if(SvsrLoggerGetMask() != 0)
                SvsrLoggerMaskStore = SvsrLoggerGetMask();

            if(gMac2PhyDirectFIFOUsed == 0)
            {// prepare all but RxVector, send with first TxStartInd
                LteBsMacDlScheduler(pBsMacDesc, pDlSubFrame);
                LteBsMacUlScheduler(pBsMacDesc, pUlSubFrame);
                LteMacSendToPhyMessage(PHY_TXSTART_REQ, pDlSubFrame, 1);
            }
#if 0
//#ifdef LOWLATENCY
            else
            {// prepare all, send with PhyStartConf received
                LteBsMacDlScheduler(pBsMacDesc, pDlSubFrame);
                LteBsMacUlScheduler(pBsMacDesc, pUlSubFrame);

                LteMacSendToPhyMessage(PHY_TXSTART_REQ, pDlSubFrame, 1);
                LteMacSendToPhyMessage(PHY_RXSTART_REQ, LteBsMacUlVectorGet(), 1);

                LteMacCheckMsgIntegrity();
            }
//#endif
#endif

            InitRxSduBuf(param);

            if (pToPhyMsg)
            {
                memcpy(pToPhyMsg, param, sizeof(STARTREQ));
            }

            // if we use new MAC<=>PHY interface
            // just send this command to the new instance

            MacSendToPhy(PHY_START_REQ, buffer, sizeof(STARTREQ));
            break;

        case PHY_STOP_REQ:

            if(pBsMacDesc->PhyAutoCtx.RunEna == TRUE)
            {
                IsSendToPhyMsgMask |= (1<<PhyAutoStop);
            }
            else
            {
                MacSendToPhy(PHY_STOP_REQ, buffer, sizeof(GENMSGDESC));
            }
            if (IcpuFlagErr)
            {
                U32 i;
                for (i=0; i<numSvsrApiMsgStats; i++)
                {
                    uart_printf("%d: SvsrApiMsgStats.start = 0x%8.8x, SvsrApiMsgStats.diff = 0x%8.8x, SvsrApiMsgStats.physaddr = 0x%8.8x, SvsrApiMsgStats.frame = 0x%8.8x, SvsrApiMsgStats.type = 0x%8.8x\r\n",
                                i, SvsrApiMsgStats[i].start,          SvsrApiMsgStats[i].diff,        SvsrApiMsgStats[i].physaddr,        SvsrApiMsgStats[i].frame,        SvsrApiMsgStats[i].type);
                }
            }
            break;

        case PHY_SHUTDOWN_REQ:
            if(pBsMacDesc->PhyAutoCtx.RunEna == TRUE)
            {
                IsSendToPhyMsgMask |= (1<<PhyAutoShutdown);
            }
            else
            {
                MacSendToPhy(PHY_SHUTDOWN_REQ, buffer, sizeof(GENMSGDESC));
            }
            break;
        /*UMTSNMM*/
        case UMTSNMM_CMD:
        {

        	UINT32 UmtsNmmmsg_sizes[UMTSNMM_CMD_NUM] = {sizeof(T_3G_INIT_NWL), sizeof(T_3G_Stop_NWL),
        	    sizeof(T_3G_CELL_SEARCH_REQ), sizeof(T_3G_STOP_CELL_SEARCH_REQ), 
        	    sizeof(T_3G_BCH_CONFIG_REQ),sizeof(T_3G_STOP_PBCH_REQ)};
        	U8 *p8;
        	LPVOID pData[5];
            U32 loop;

            UmtsnmmCmdType = *((U16 * ) param);

	    switch (UmtsnmmCmdType)
	    {
	        case 0: // UMTS NMM init message
                pToPhyMsg->msgSpecific = sizeof(T_3G_INIT_NWL) + sizeof(GENMSGDESC);
                pUmtsNmmInitMsg = (T_3G_INIT_NWL *)payload;
                break;

	        case 1: // UMTS NMM stop message
                pToPhyMsg->msgSpecific = sizeof(T_3G_Stop_NWL) + sizeof(GENMSGDESC);
                pStopUmtsNmmMsg = (T_3G_Stop_NWL *)payload;
                break;


	        case 2: // UMTS NMM CELL SEARCH request message
                pToPhyMsg->msgSpecific = sizeof(T_3G_CELL_SEARCH_REQ) + sizeof(GENMSGDESC);
                pUmtsNmmCSReqMsg = (T_3G_CELL_SEARCH_REQ *)payload;
                pUmtsNmmCSReqMsg->exhaustiveSearch =1;
                pUmtsNmmCSReqMsg->measurementPeriod =2;
                pUmtsNmmCSReqMsg->nr_pscList =10;
                for(loop=0;loop<pUmtsNmmCSReqMsg->nr_pscList;loop++)
                {
                    pUmtsNmmCSReqMsg->pciList[loop]=loop;
                }
                break;

                case 3: // UMTS NMM CELL SEARCH stop message
                pToPhyMsg->msgSpecific = sizeof(T_3G_STOP_CELL_SEARCH_REQ) + sizeof(GENMSGDESC);
                pUmtsNmmCSStopMsg = (T_3G_STOP_CELL_SEARCH_REQ *)payload;
                break;

                case 4: // UMTS NMM BCH DECODER request message
                pToPhyMsg->msgSpecific = sizeof(T_3G_BCH_CONFIG_REQ) + sizeof(GENMSGDESC);
                pUmtsNmmBchReqMsg = (T_3G_BCH_CONFIG_REQ *)payload;
                pUmtsNmmBchReqMsg->psc=0;
                break;


                case 5: // UMTS NMM BCH DECODER stop message
                pToPhyMsg->msgSpecific = sizeof(T_3G_STOP_PBCH_REQ) + sizeof(GENMSGDESC);
                pUmtsNmmBchStopMsg = (T_3G_STOP_PBCH_REQ *)payload;
                break;
                
                default:
                {
                    uart_printf("Message type error:\
                                        \t---0: UMTS NMM INIT \r\n  \
                                        \t---1: UMTS NMM STOP \r\n   \
                                        \t---2: UMTS NMM CELL SEARCH START \r\n \
                                        \t---3: UMTS NMM CELL SEARCH START \r\n \
                                        \t---4: UMTS NMM BCH DECODER START \r\n \
                                        \t---5: UMTS NMM BCH DECODER STOP \r\n ");
                    
                    return;
                }
           }


	        pData[0] = buffer;
	        MacSendToPhyUMTSNMM(&umtsnmm_message_types[UmtsnmmCmdType], pData, &UmtsNmmmsg_sizes[UmtsnmmCmdType], 1);

            break;
        }
        default:
            printf("Invalid PHY message [%d]\n", MessageType);
            break;
    }

    return SUCCESS;
}

RESULTCODE LteMacSendToPhyNmmMessage(U32 MessageType, PTR param, U32 flag)
{
    U32 msgSize = 0;
    PGENMSGDESC buffer = NULL;
    PGENMSGDESC pGenHdr = (PGENMSGDESC)param;

    switch (MessageType)
    {
        case NMM_START:
        case NMM_STOP:

        case NMM_RSSI_MEAS_REQ:
        case NMM_STOP_RSSI_MEAS_REQ:

        case NMM_CELL_SEARCH_REQ:
        case NMM_STOP_CELL_SEARCH_REQ:

        case NMM_PBCH_CONFIG_REQ:
        case NMM_STOP_PBCH_REQ:

        case NMM_SIB1_CONFIG_REQ:

        case NMM_BCCH_CONFIG_REQ:
        case NMM_STOP_BCCH_REQ:

            buffer = Mac2PhyIfAllocBufferEx(sizeof(fapi_nmm_rssi_meas_req_desc_t));
            if (buffer == NULL)
            {
                printf("ERROR: No memory [%s,%s:%d]\n", __FUNCTION__, __FILE__, __LINE__);
                return FAILURE;
            }
            break;

        default:
            break;
    }

    switch (MessageType)
    {
        case NMM_START:
            {
                //fapi_nmm_start_desc_t *nmm_start_desc = (fapi_nmm_start_desc_t *)param;
                PLTE_NMM_CTX plte_nmm_ctx = (PLTE_NMM_CTX)get_lte_nmm_ctx();

    // allocate memory for rx_nmm_buf
                if (plte_nmm_ctx->RxNmmBuf)
                {
                    MxHeapFree(AppHeap, plte_nmm_ctx->RxNmmBuf);
                    plte_nmm_ctx->RxNmmBuf = NULL;
                }

                plte_nmm_ctx->RxNmmSize = NMM_IND_SIZE * MAX_NUM_NMM_INTO_BUF;
                uart_printf("warning!!!need to check RxNmmSize\n");

                plte_nmm_ctx->RxNmmBuf = MxHeapAlloc(AppHeap, plte_nmm_ctx->RxNmmSize);
                if (plte_nmm_ctx->RxNmmBuf == NULL)
                {
                    uart_printf("RxNmmBuf alloc error\n");
                }

                plte_nmm_ctx->RxNmmSet = 0;
                plte_nmm_ctx->RxNmmCntr = 0;

            }

            msgSize = pGenHdr->msgSpecific;
            memcpy(buffer, param, msgSize);
            MacSendToPhy(MessageType, buffer, msgSize);
            break;

        case NMM_RSSI_MEAS_REQ:
        case NMM_STOP_RSSI_MEAS_REQ:

        case NMM_CELL_SEARCH_REQ:
        case NMM_STOP_CELL_SEARCH_REQ:

        case NMM_PBCH_CONFIG_REQ:
        case NMM_STOP_PBCH_REQ:

        case NMM_SIB1_CONFIG_REQ:

        case NMM_BCCH_CONFIG_REQ:
        case NMM_STOP_BCCH_REQ:

            msgSize = pGenHdr->msgSpecific;
            memcpy(buffer, param, msgSize);
            MacSendToPhy(MessageType, buffer, msgSize);
            break;

        case NMM_STOP:
            msgSize = pGenHdr->msgSpecific;

            MacSendToPhy(NMM_STOP, buffer, msgSize);
            if (IcpuFlagErr)
            {
                U32 i;
                for (i=0; i<numSvsrApiMsgStats; i++)
                {
                    uart_printf("%d: SvsrApiMsgStats.start = 0x%8.8x, SvsrApiMsgStats.diff = 0x%8.8x, SvsrApiMsgStats.physaddr = 0x%8.8x, SvsrApiMsgStats.frame = 0x%8.8x, SvsrApiMsgStats.type = 0x%8.8x\r\n",
                                i, SvsrApiMsgStats[i].start,          SvsrApiMsgStats[i].diff,        SvsrApiMsgStats[i].physaddr,        SvsrApiMsgStats[i].frame,        SvsrApiMsgStats[i].type);
                }
            }

            break;

        default:
            printf("Invalid NMM PHY message [%d]\n", MessageType);
            return FAILURE;
    }

    return SUCCESS;
}

RESULTCODE LteMacSendToPhyNmmMessageTest(U32 MessageType, PTR param, U32 flag)
{
    PGENMSGDESC pToPhyMsg = NULL;
    U32 msgSize = 0;
    PTR payload = NULL;
    PGENMSGDESC buffer = NULL;
    U16 nmmCmdType;
    PGENMSGDESC pToPhyMsg_rssi_req = NULL;
    PGENMSGDESC pToPhyMsg2 = NULL;
    PGENMSGDESC pToPhyMsg3 = NULL;
    PGENMSGDESC pToPhyMsg4 = NULL;
    PTR payload_rssi_req = NULL;
    PTR payload2 = NULL;
    PTR payload3 = NULL;
    PTR payload4 = NULL;
    PGENMSGDESC buffer_rssi_req = NULL;
    PGENMSGDESC buffer2 = NULL;
    PGENMSGDESC buffer3 = NULL;
    PGENMSGDESC buffer4 = NULL;
    U16 rssi_earfcn_list[NMM_LTE_MAX_CARRIER_LIST]={3100,3101};
    U16 siPeriodicity_list[MAX_SI_LIST] = {160,320,640,1280};
    U16 tmp_i;
    U32 param32 = 0;

    nmmCmdType = 0;    // default is nmm start
    if (param)
    {
        nmmCmdType = *((U16 * ) param);

        param32 = *((U32 *) param);
    }

    switch (MessageType)
    {
        case NMM_STOP:
            buffer = Mac2PhyIfAllocBufferEx(sizeof(fapi_nmm_start_desc_t));
            if (buffer == NULL)
            {
                printf("ERROR: No memory [%s,%s:%d]\n", __FUNCTION__, __FILE__, __LINE__);
                return FAILURE;
            }
            pToPhyMsg = (PGENMSGDESC) buffer;
            // This is index in PhyInst[] array
            pToPhyMsg->phyEntityId = 0;
            pToPhyMsg->msgType = MessageType;
            payload = (PTR) (pToPhyMsg + 1);
            break;

        /* FIXME (OS) - tmp - hard-coded 4 messages for nmm test: start, cell search, pbch and sib1 */
        case NMM_START:
            buffer = Mac2PhyIfAllocBufferEx(sizeof(fapi_nmm_start_desc_t));
            if (buffer == NULL) {
                uart_printf("ERROR: No memory [%s,%s:%d]\n", __FUNCTION__, __FILE__, __LINE__);
                return FAILURE;
            }

            buffer_rssi_req = Mac2PhyIfAllocBufferEx(sizeof(fapi_nmm_rssi_meas_req_desc_t));
            if (buffer_rssi_req == NULL) {
                uart_printf("ERROR: No memory [%s,%s:%d]\n", __FUNCTION__, __FILE__, __LINE__);
                return FAILURE;
            }

            buffer2 = Mac2PhyIfAllocBufferEx(sizeof(fapi_nmm_cell_search_req_desc_t));
            if (buffer2 == NULL) {
                uart_printf("ERROR: No memory [%s,%s:%d]\n", __FUNCTION__, __FILE__, __LINE__);
                return FAILURE;
            }

            buffer3 = Mac2PhyIfAllocBufferEx(sizeof(fapi_nmm_pbch_config_req_desc_t));
            if (buffer3 == NULL) {
                uart_printf("ERROR: No memory [%s,%s:%d]\n", __FUNCTION__, __FILE__, __LINE__);
                return FAILURE;
            }

            buffer4 = Mac2PhyIfAllocBufferEx(sizeof(fapi_nmm_sib1_config_req_desc_t));
            if (buffer4 == NULL) {
                uart_printf("ERROR: No memory [%s,%s:%d]\n", __FUNCTION__, __FILE__, __LINE__);
                return FAILURE;
            }

            pToPhyMsg = (PGENMSGDESC) buffer;
            pToPhyMsg->phyEntityId = 0;
            pToPhyMsg->msgSpecific = sizeof(fapi_nmm_start_desc_t) + sizeof(GENMSGDESC);
            pToPhyMsg->msgType = MessageType;
            payload = (PTR) (pToPhyMsg + 1);

            pToPhyMsg_rssi_req = (PGENMSGDESC) buffer_rssi_req;
            pToPhyMsg_rssi_req->phyEntityId = 0;
            pToPhyMsg_rssi_req->msgSpecific = sizeof(fapi_nmm_rssi_meas_req_desc_t) + sizeof(GENMSGDESC);
            pToPhyMsg_rssi_req->msgType = NMM_RSSI_MEAS_REQ;
            payload_rssi_req = (PTR) (pToPhyMsg_rssi_req + 1);

            pToPhyMsg2 = (PGENMSGDESC) buffer2;
            pToPhyMsg2->phyEntityId = 0;
            pToPhyMsg2->msgSpecific = sizeof(fapi_nmm_cell_search_req_desc_t) + sizeof(GENMSGDESC);
            pToPhyMsg2->msgType = NMM_CELL_SEARCH_REQ;
            payload2 = (PTR) (pToPhyMsg2 + 1);

            pToPhyMsg3 = (PGENMSGDESC) buffer3;
            pToPhyMsg3->phyEntityId = 0;
            pToPhyMsg3->msgSpecific = sizeof(fapi_nmm_pbch_config_req_desc_t) + sizeof(GENMSGDESC);
            pToPhyMsg3->msgType = NMM_PBCH_CONFIG_REQ;
            payload3 = (PTR) (pToPhyMsg3 + 1);

            pToPhyMsg4 = (PGENMSGDESC) buffer4;
            pToPhyMsg4->phyEntityId = 0;
            pToPhyMsg4->msgSpecific = sizeof(fapi_nmm_sib1_config_req_desc_t) + sizeof(GENMSGDESC);
            pToPhyMsg4->msgType = NMM_SIB1_CONFIG_REQ;
            payload4 = (PTR) (pToPhyMsg4 + 1);

            break;

    case NMM_CMD:
        if (nmmCmdType >= NMM_CMD_NUM)
        {
            uart_printf("ERROR: unsupported NMM command %d [%s,%s:%d]\n", nmmCmdType, __FUNCTION__, __FILE__, __LINE__);
            return FAILURE;
        }

        buffer = Mac2PhyIfAllocBufferEx(MAX(sizeof (DLSUBFRDESC), sizeof (ULSUBFRDESC)));
        if (buffer == NULL)
        {
            printf("ERROR: No memory [%s,%s:%d]\n", __FUNCTION__, __FILE__, __LINE__);
            return FAILURE;
        }

            pToPhyMsg = (PGENMSGDESC) buffer;
            pToPhyMsg->msgSpecific = sizeof(GENMSGDESC);    //for request without parameters
            pToPhyMsg->phyEntityId = 0;
            pToPhyMsg->msgType = nmm_message_types[nmmCmdType];
            payload = (PTR) (pToPhyMsg + 1);

            break;

        default:
            break;
    }

    switch (MessageType)
    {
        case NMM_START:
        {
            #define NMM_START_MSG_NUM 5
            U8 message_types[NMM_START_MSG_NUM] = {NMM_START, NMM_RSSI_MEAS_REQ, NMM_CELL_SEARCH_REQ, NMM_PBCH_CONFIG_REQ, NMM_SIB1_CONFIG_REQ};
            LPVOID datas[NMM_START_MSG_NUM];
            UINT32 message_sizes[NMM_START_MSG_NUM] = {sizeof(fapi_nmm_start_desc_t),
                sizeof(fapi_nmm_rssi_meas_req_desc_t),sizeof(fapi_nmm_cell_search_req_desc_t), sizeof(fapi_nmm_pbch_config_req_desc_t), sizeof(fapi_nmm_sib1_config_req_desc_t)};
            U8 *p8;

            fapi_nmm_start_desc_t *nmm_start_desc = (fapi_nmm_start_desc_t *)payload;
            STARTREQ start_params;
            fapi_nmm_rssi_meas_req_desc_t *rssi_req_desc = (fapi_nmm_rssi_meas_req_desc_t *)payload_rssi_req;
            fapi_nmm_cell_search_req_desc_t *cell_search_desc = (fapi_nmm_cell_search_req_desc_t *)payload2;
            fapi_nmm_pbch_config_req_desc_t *pbch_config_desc = (fapi_nmm_pbch_config_req_desc_t *)payload3;
            fapi_nmm_sib1_config_req_desc_t *sib1_config_desc = (fapi_nmm_sib1_config_req_desc_t *)payload4;

            PLTE_NMM_CTX plte_nmm_ctx = (PLTE_NMM_CTX)get_lte_nmm_ctx();

            uart_printf("%s: NMM_START\n", __FUNCTION__);


// allocate memory for rx_nmm_buf
            if (plte_nmm_ctx->RxNmmBuf)
            {
                MxHeapFree(AppHeap, plte_nmm_ctx->RxNmmBuf);
                plte_nmm_ctx->RxNmmBuf = NULL;
            }

            plte_nmm_ctx->RxNmmSize = NMM_IND_SIZE * MAX_NUM_NMM_INTO_BUF;
            uart_printf("warning!!!need to check RxNmmSize\n");

            plte_nmm_ctx->RxNmmBuf = MxHeapAlloc(AppHeap, plte_nmm_ctx->RxNmmSize);
            if (plte_nmm_ctx->RxNmmBuf == NULL)
            {
                uart_printf("RxNmmBuf alloc error\n");
            }
            uart_printf("RxNmm Indication log: 0x%x Size: %d\n", plte_nmm_ctx->RxNmmBuf, plte_nmm_ctx->RxNmmSize);
            plte_nmm_ctx->RxNmmSet = 0;
            plte_nmm_ctx->RxNmmCntr = 0;

            plte_nmm_ctx->cnt_nmm_rssi_meas_ind = 100;
            plte_nmm_ctx->cnt_local_nmm_rssi_meas_ind = 0;

            nmm_start_desc->supported_rat = 1; /* lte */
            nmm_start_desc->duplex = 0; /* fdd */

#define NMM_TIMER

#ifdef NMM_TIMER
            start_params.mode = 1; /* 0:cpri;1:cpri_simulator */
            start_params.msgType = NMM_START;
            start_params.phyEntityId = 0;
            start_params.count = 0;//50;
            start_params.period = 5;//4:for 20MHz 1AC; 5;
#else
            start_params.mode = 4; /* 0:cpri;1:cpri_simulator */
            start_params.msgType = NMM_START;
            start_params.phyEntityId = 0;
            start_params.count = 0;//50;
            start_params.period = 0;//4:for 20MHz 1AC; 5;
#endif
            memcpy(nmm_start_desc->vendor_specific_list, &start_params, sizeof(STARTREQ));

            /*RSSI meas req*/
            rssi_req_desc->freqBandIndicator = 7;
            rssi_req_desc->measurementPeriod = 10;
            rssi_req_desc->Nr_earfcnList = 2;

            p8 = (U8*)(&(rssi_req_desc->Nr_earfcnList))+1;
            memcpy(p8, rssi_earfcn_list,2*sizeof(U16));
            p8 = (U8*)(&(rssi_req_desc->Nr_earfcnList))+ 2*sizeof(U16) + 1;
            p8[0] = 0; /*nr_vendorSpecificList*/
            message_sizes[1] = p8 + 1 - (U8 *)rssi_req_desc;
            pToPhyMsg_rssi_req->msgSpecific = message_sizes[1] + sizeof(GENMSGDESC);

            cell_search_desc->earfcn = 1;
            cell_search_desc->exhaustive_search = FALSE;
            cell_search_desc->nr_pci_list = 1;
            cell_search_desc->pci_list[0] = 1;
            p8 = (U8 *)&cell_search_desc->pci_list[1];
            *p8++ = RB_SLOT_50;
            *p8++ = 1;
            p8[0] = 0; /*nr_vendorSpecificList*/
            message_sizes[2] = p8 + 1 - (U8 *)cell_search_desc;
            pToPhyMsg2->msgSpecific = message_sizes[2] + sizeof(GENMSGDESC);

            pbch_config_desc->pci = 1;
            pbch_config_desc->earfcn = 1;
            pbch_config_desc->nr_vendor_specific_list = 0;
            message_sizes[3] = 5;
            pToPhyMsg3->msgSpecific = message_sizes[3] + sizeof(GENMSGDESC);

            sib1_config_desc->retry_count = 1;
            sib1_config_desc->nr_vendor_specific_list = 0;
            message_sizes[4] = 2;
            pToPhyMsg4->msgSpecific = message_sizes[4] + sizeof(GENMSGDESC);

            datas[0] = buffer;
            datas[1] = buffer_rssi_req;
            datas[2] = buffer2;
            datas[3] = buffer3;
            datas[4] = buffer4;

            //MacSendToPhyNMM(message_types, datas, message_sizes, NMM_START_MSG_NUM);
            if(0) //for test RSSI meas
            {
                MacSendToPhyNMM(message_types, datas, message_sizes, 2);
            }
            else //for test cell search etc.
            {
                for(tmp_i=1;tmp_i<=3;tmp_i++)
                {
                    message_types[tmp_i] = message_types[tmp_i+1];
                    datas[tmp_i]=datas[tmp_i+1];
                    message_sizes[tmp_i]=message_sizes[tmp_i+1];
                }
                MacSendToPhyNMM(message_types, datas, message_sizes, 1);
            }
            return MAC_SUCCESS;
        }

        case NMM_STOP:
            pToPhyMsg->msgSpecific = sizeof(GENMSGDESC);
            msgSize = sizeof(GENMSGDESC);

            MacSendToPhy(NMM_STOP, buffer, msgSize);
            if (IcpuFlagErr)
            {
                U32 i;
                for (i=0; i<numSvsrApiMsgStats; i++)
                {
                    uart_printf("%d: SvsrApiMsgStats.start = 0x%8.8x, SvsrApiMsgStats.diff = 0x%8.8x, SvsrApiMsgStats.physaddr = 0x%8.8x, SvsrApiMsgStats.frame = 0x%8.8x, SvsrApiMsgStats.type = 0x%8.8x\r\n",
                                i, SvsrApiMsgStats[i].start,          SvsrApiMsgStats[i].diff,        SvsrApiMsgStats[i].physaddr,        SvsrApiMsgStats[i].frame,        SvsrApiMsgStats[i].type);
                }
            }

            break;

        case NMM_CMD:
        {
            U8 *p8;

            STARTREQ start_params;
            fapi_nmm_start_desc_t *nmm_start_desc;
            fapi_nmm_rssi_meas_req_desc_t *rssi_req_desc;
            fapi_nmm_cell_search_req_desc_t *cell_search_desc;
            fapi_nmm_pbch_config_req_desc_t *pbch_config_desc;
            fapi_nmm_sib1_config_req_desc_t *sib1_config_desc;
            fapi_nmm_bcch_config_req_desc_t *bcch_config_desc;

            switch (nmmCmdType)
            {
                case 0: // nmm start
                    nmm_start_desc = (fapi_nmm_start_desc_t *)payload;
                    nmm_start_desc->supported_rat = 1; /* lte */
                    nmm_start_desc->duplex = 0; /* fdd */

                    start_params.mode = 4; /* 0:cpri;1:cpri_simulator */
                    start_params.msgType = NMM_START;
                    start_params.phyEntityId = 0;
                    start_params.count = 0;//50;
                    start_params.period = 0;//4:for 20MHz 1AC; 5;
                    memcpy(nmm_start_desc->vendor_specific_list, &start_params, sizeof(STARTREQ));

                    pToPhyMsg->msgSpecific = sizeof(fapi_nmm_start_desc_t) + sizeof(GENMSGDESC);

                    break;

                case 1: // nmm stop
                    pToPhyMsg->msgSpecific = sizeof(GENMSGDESC);

                    break;

                case 2: // rssi meas req
                    rssi_req_desc = (fapi_nmm_rssi_meas_req_desc_t *)payload;

                    rssi_req_desc->freqBandIndicator = 7;
                    rssi_req_desc->measurementPeriod = 10;
                    rssi_req_desc->Nr_earfcnList = 2;

                    p8 = (U8*)(&(rssi_req_desc->Nr_earfcnList))+1;
                    memcpy(p8,rssi_earfcn_list,2*sizeof(U16));
                    //p8 = (U8*)(&(rssi_req_desc->Nr_earfcnList))+ 2*sizeof(U16);
                    p8 += 2*sizeof(U16);
                    *p8++ = 0; /*nr_vendorSpecificList*/
                    pToPhyMsg->msgSpecific = (p8 - (U8 *)rssi_req_desc) + sizeof(GENMSGDESC);

                    break;

                case 4: // cell search req
                    cell_search_desc = (fapi_nmm_cell_search_req_desc_t *)payload;
                    cell_search_desc->earfcn = 1;
                    cell_search_desc->exhaustive_search = FALSE;
                    cell_search_desc->nr_pci_list = 1;
        			cell_search_desc->pci_list[0] = param32 >> 16;
                    p8 = (U8 *)&cell_search_desc->pci_list[1];
                    *p8++ = RB_SLOT_50;
                    *p8++ = 1;
                    *p8++ = 0; /*nr_vendorSpecificList*/
                    pToPhyMsg->msgSpecific = (p8 - (U8 *)cell_search_desc) + sizeof(GENMSGDESC);

                    break;

                case 6: // pbch config req
                    pbch_config_desc = (fapi_nmm_pbch_config_req_desc_t *)payload;
                	pbch_config_desc->pci = param32 >> 16;
                    pbch_config_desc->earfcn = 1;
                    pbch_config_desc->nr_vendor_specific_list = 0;
                    pToPhyMsg->msgSpecific = 5 + sizeof(GENMSGDESC);

                    break;

                case 8: // sib1 config req
                    sib1_config_desc = (fapi_nmm_sib1_config_req_desc_t *)payload;
                    sib1_config_desc->retry_count = 4;
                    sib1_config_desc->nr_vendor_specific_list = 0;
                    pToPhyMsg->msgSpecific = 2 + sizeof(GENMSGDESC);

                    break;

                case 9: // bcch config req
                    bcch_config_desc = (fapi_nmm_bcch_config_req_desc_t *)payload;
                    bcch_config_desc->nr_siPeriodicityList = 4;
        			p8 = (U8 *)(&bcch_config_desc->nr_siPeriodicityList) + 1;
        			memcpy(p8,siPeriodicity_list,4*sizeof(U16));  //siPeriodicityList
        			p8 += 4*sizeof(U16);
                    *p8++ = 20;   //siWindow
                    *p8++ = 7;   //retry_count
                    *p8++ = 0;   //nr_vendor_specific_list
                    pToPhyMsg->msgSpecific = (p8 - (U8 *)bcch_config_desc) + sizeof(GENMSGDESC);

                    break;

                case 3: // stop rssi meas req
                case 5: // stop cell search req
                case 7: // stop pbch req
                case 10: // stop bcch req
                    p8 = (U8 *)payload;
                    *p8 = 0;    // nr_vendor_specific_list
                    pToPhyMsg->msgSpecific = sizeof(GENMSGDESC) + 1;

                    break;

            }

            MacSendToPhy(nmm_message_types[nmmCmdType], buffer, pToPhyMsg->msgSpecific);

        }
        break;

        default:
            printf("Invalid NMM PHY message [%d]\n", MessageType);
            break;
    }

    return SUCCESS;
}


/** Send API message (84xxx-SWG-009-A) to the PHY using extended interface
 *
 * @param MessageType PHY API message ID (LtePhyL2Api.h)
 * @param pData Pointer to the message buffer
 * @param nMsgSize Message size
 * @return 4GMX return code
 */
static MXRC MacSendToPhy(U8 MessageType, LPVOID pData, UINT32 nMsgSize)
{
    PMAC2PHY_QUEUE_EL pCtrlBlock;
    MXRC rc;

    _ASSERT_PTR (pData);

    //pCtrlBlock = (PMAC2PHY_QUEUE_EL) SvsrAllocMsgEx(sizeof(MAC2PHY_QUEUE_EL));
    pCtrlBlock = (PMAC2PHY_QUEUE_EL) SvsrAllocMsg();

    if (pCtrlBlock == NULL)
    {
        uart_printf("No memory to send I-CORE data, pData=%x, Size=%d\r\n", pData, nMsgSize);
        _ASSERT_PTR(pCtrlBlock);
        return 1;
    }

    memset(pCtrlBlock, 0, sizeof(*pCtrlBlock));

    pCtrlBlock->MessagePtr = (U8*) pData;
    pCtrlBlock->MessageLen = nMsgSize;
    pCtrlBlock->MessageType = MessageType;
    pCtrlBlock->Next = NULL;

    if (MessageType >=  NMM_START && MessageType < NMM_final_to_be_updated)
        rc = SvsrSendMsg(0, IID_LTE_NMM, pCtrlBlock, SVSR_MSG_OPT_DEFAULT);
    else
        rc = SvsrSendMsg(0, IID_LTE_EX, pCtrlBlock, SVSR_MSG_OPT_DEFAULT);
    _ASSERT_RC(rc);
    if (FAILED(rc))
    {
        uart_printf("MacSendToPhy::SvsrSendMsg has failed, rc=%x\r\n", rc);
        return rc;
    }
    return MX_OK;
}

static MXRC MacSendToPhyNMM(U8 *MessageTypes, LPVOID *pDatas, UINT32 *nMsgSizes, UINT32 nMessages)
{
    PMAC2PHY_QUEUE_EL pCtrlBlock;
    PMAC2PHY_QUEUE_EL pCtrlPreviousBlock = NULL;
    PMAC2PHY_QUEUE_EL pHead = NULL;
    U8 MessageType;
    LPVOID pData;
    UINT32 nMsgSize;
    MXRC rc;
    U32 i;

    for (i = 0; i < nMessages; i++) {
        MessageType = MessageTypes[i];
        pData = pDatas[i];
        nMsgSize = nMsgSizes[i];

        _ASSERT_PTR (pData);
        pCtrlBlock = (PMAC2PHY_QUEUE_EL) SvsrAllocMsgEx(sizeof(MAC2PHY_QUEUE_EL));
        if (pCtrlBlock == NULL) {
            uart_printf("No memory to send I-CORE data, pData=%x, Size=%d\r\n", pData, nMsgSize);
            _ASSERT_PTR(pCtrlBlock);

            return 1;
        }

        if (i == 0)
            pHead = pCtrlBlock;

        memset(pCtrlBlock, 0, sizeof(*pCtrlBlock));
        pCtrlBlock->MessagePtr = (U8*) pData;
        pCtrlBlock->MessageLen = nMsgSize;
        pCtrlBlock->MessageType = MessageType;
        pCtrlBlock->Next = NULL;
        if (pCtrlPreviousBlock)
            pCtrlPreviousBlock->Next = pCtrlBlock;
        pCtrlPreviousBlock = pCtrlBlock;
    }

    rc = SvsrSendMsg(0, IID_LTE_NMM, pHead, SVSR_MSG_OPT_DEFAULT);

    _ASSERT_RC(rc);
    if (FAILED(rc))
    {
        uart_printf("MacSendToPhy::SvsrSendMsg has failed, rc=%x\r\n", rc);
        return rc;
    }
    return MX_OK;
}

/** Process API message received from PHY
 *
 * @param pMsgHdr Pointer to the message buffer
 * @param pDlSubFrame Pointer to the current Downlink vector
 * @param pUlSubFrame Pointer to the current Uplink vector
 * @param ptrCtx Not used
 * @return Standard result code
 */
extern U32 rssi_meas_nr_earfcnList;

RESULTCODE LteMacProcessFromPhyMessage(PGENMSGDESC pMsgHdr,
                                       PDLSUBFRDESC pDlSubFrame,
                                       PULSUBFRDESC pUlSubFrame,
                                       PTR ptrCtx)
{
    U32 nmm_resp_size = 0;

    switch (pMsgHdr->msgType)
    {

        case NMM_RSSI_MEAS_IND:
        case NMM_CELL_SEARCH_IND:
        case NMM_PBCH_DATA_IND:
        case NMM_BCCH_DATA_IND:
            {
                PLTE_NMM_CTX plte_nmm_ctx = (PLTE_NMM_CTX)get_lte_nmm_ctx();
                MSGIND* pnmm_resp_msg = (MSGIND*) pMsgHdr;

                nmm_resp_size = pnmm_resp_msg->reserved; //reserved U16 used to indicate msgLength

                // Add new NMM indication to buffer
#if 0
                add_nmm_ind_to_buf((U8*) pnmm_resp_msg, nmm_resp_size);
#else
                {
                    ApiHeader* buf;
                    buf = (ApiHeader *) SvsrAllocICApi();
                    if (buf == NULL)
                        return RC_SVSR_ALLOC_ERROR;

                    buf = ApiCreate(buf, API_DEF_FLAGS, IID_LTE_MAC, UARM_INSTID_DEFMAC, API_TYPE_CFG_RESP, pMsgHdr->msgType);
                    ApiAddParam(buf, NULL, PAR_BIN_DATA, pMsgHdr, nmm_resp_size);

                    SvsrSendICApi(buf);
                }
#endif

                if (plte_nmm_ctx->RxNmmCntr > 10)
                {
                    drain_nmm_ind_buf();
                    // Clear buffer counters
                    plte_nmm_ctx->RxNmmSet = 0;
                    plte_nmm_ctx->RxNmmCntr = 0;
                }
            }

            break;

        case NMM_CELL_SEARCH_CNF:
        case NMM_START_RESP:
        case NMM_STOP_RESP:

        case NMM_RSSI_MEAS_REQ_RESP:
        case NMM_RSSI_MEAS_CNF:
        case NMM_STOP_RSSI_MEAS_REQ_RESP:

        case NMM_CELL_SEARCH_REQ_RESP:
        case NMM_STOP_CELL_SEARCH_REQ_RESP:

        case NMM_PBCH_CONFIG_CNF:
        case NMM_STOP_PBCH_REQ_RESP:

        case NMM_SIB1_CONFIG_CNF:
        case NMM_BCCH_CONFIG_CNF:
            {
                PLTE_NMM_CTX plte_nmm_ctx = (PLTE_NMM_CTX)get_lte_nmm_ctx();
                MSGIND* pnmm_resp_msg = (MSGIND*) pMsgHdr;
                ApiHeader* buf;

                nmm_resp_size = pnmm_resp_msg->reserved;//reserved U16 used to indicate msgLength

                uart_printf("%s: send RSP/CNF to UARM(Host) %d len=%d\r\n", __FUNCTION__, pMsgHdr->msgType, nmm_resp_size);

                if(pMsgHdr->msgType == NMM_RSSI_MEAS_CNF
                    || pMsgHdr->msgType == NMM_CELL_SEARCH_CNF
                    || pMsgHdr->msgType == NMM_STOP_PBCH_REQ_RESP
                    || pMsgHdr->msgType == NMM_STOP_RSSI_MEAS_REQ_RESP
                    || pMsgHdr->msgType == NMM_STOP_CELL_SEARCH_REQ_RESP
                    || pMsgHdr->msgType == NMM_STOP_RESP)
                {
                    if(plte_nmm_ctx->RxNmmCntr)
                    {
                        drain_nmm_ind_buf();
                        // Clear buffer counters
                        plte_nmm_ctx->RxNmmSet = 0;
                        plte_nmm_ctx->RxNmmCntr = 0;
                    }
                }

                buf = (ApiHeader *) SvsrAllocICApi();
                if (buf == NULL)
                    return RC_SVSR_ALLOC_ERROR;

                buf = ApiCreate(buf, API_DEF_FLAGS, IID_LTE_MAC, UARM_INSTID_DEFMAC, API_TYPE_CFG_RESP, pMsgHdr->msgType);
                ApiAddParam(buf, NULL, PAR_BIN_DATA, pMsgHdr, nmm_resp_size);

                SvsrSendICApi(buf);
            }

            break;

        case PHY_TXSTART_CONF:
            break;

        case PHY_TXSTART_IND:
            {
            // This is from PHY indicating beginning of transmission
            PBS_STATIC_CONFIG pbs_static_config = get_static_config();
            if (pbs_static_config->use_internal_etm_test)
            {
                if ( SUCCESS != etm_get_vectors(&Mac2PhyQueue.StartQueueCurr, getFrameNum(), getSubFrameNum()) )
                    return FAILURE;

                LteMacCheckMsgIntegrity();
                if (IsICPUEmulated())
                {
                    SendToPhyICpu(IID_LTE_EX, Mac2PhyQueue.StartQueueCurr);
                }
                else
                {
                    SvsrSendMsg(0, IID_LTE_EX, Mac2PhyQueue.StartQueueCurr, SVSR_MSG_OPT_DEFAULT);
                }
                
                Mac2PhyQueue.StartQueueCurr = NULL;
                Mac2PhyQueue.TailQueueCurr = NULL;
                globalTTITickCount++;
                setFrameSubFrameNum();
                setTxFrameSubFrameNum();                                
                return SUCCESS;
            }
            if(gMac2PhyDirectFIFOUsed == 0)
            {
                if(IsSendToPhyMsgMask)
                {
                    ApiListFree();
                    if(IsSendToPhyMsgMask & (1<<PhyAutoShutdown))
                    {
                        LteMacSendToPhyMessage(PHY_SHUTDOWN_REQ, NULL, 1);
                    }
                    else if(IsSendToPhyMsgMask & (1<<PhyAutoStop))
                    {
                        LteMacSendToPhyMessage(PHY_STOP_REQ, NULL, 1);
                    }
                    IsSendToPhyMsgMask = 0;
                    break;
                }
                else
                {
                    LteMacSendToPhyMessage(PHY_RXSTART_REQ, LteBsMacUlVectorGet(), 1);

                    LteMacCheckMsgIntegrity();
					
                    if (IsICPUEmulated())
                    {
                        SendToPhyICpu(IID_LTE_EX, Mac2PhyQueue.StartQueueCurr);
                    }
                    else
                    {
                        SvsrSendMsg(0, IID_LTE_EX, Mac2PhyQueue.StartQueueCurr, SVSR_MSG_OPT_DEFAULT);
                    }
					
                     Mac2PhyQueue.StartQueueCurr = NULL;
                     Mac2PhyQueue.TailQueueCurr = NULL;
                }
                globalTTITickCount++;
                setFrameSubFrameNum();
                setTxFrameSubFrameNum();
            }

            //globalTTITickCount++;
            //setFrameSubFrameNum();
            //setTxFrameSubFrameNum();

            LteBsMacDlScheduler(pBsMacDesc, pDlSubFrame);

            global_chanIdx = pDlSubFrame->numberofChannelDescriptors;
            LteBsMacUlScheduler(pBsMacDesc, pUlSubFrame);

            MacProcRxSduQueue(pDlSubFrame, pUlSubFrame);

            LteMacSendToPhyMessage(PHY_TXSTART_REQ, pDlSubFrame, 1);

            if(gMac2PhyDirectFIFOUsed)
            {
                if(IsSendToPhyMsgMask)
                {
                    ApiListFree();
                    if(IsSendToPhyMsgMask & (1<<PhyAutoShutdown))
                    {
                        LteMacSendToPhyMessage(PHY_SHUTDOWN_REQ, NULL, 1);
                    }
                    else if(IsSendToPhyMsgMask & (1<<PhyAutoStop))
                    {
                        LteMacSendToPhyMessage(PHY_STOP_REQ, NULL, 1);
                    }
                    IsSendToPhyMsgMask = 0;
                    break;
                }
                else
                {
                    if(LteMacIsRxNotExpected() == SUCCESS)
                    {
                        LteMacSendToPhyMessage(PHY_RXSTART_REQ, LteBsMacUlVectorGet(), 1);

                        LteMacCheckMsgIntegrity();
						
                        if (IsICPUEmulated())
                        {
                            SendToPhyICpu(IID_LTE_EX, Mac2PhyQueue.StartQueueCurr);
                        }
                        else
                        {
                            SvsrSendMsg(0, IID_LTE_EX, Mac2PhyQueue.StartQueueCurr, SVSR_MSG_OPT_DEFAULT);
                        }

                        Mac2PhyQueue.StartQueueCurr = NULL;
                        Mac2PhyQueue.TailQueueCurr = NULL;
                    }
                }
                 globalTTITickCount++;
                 setFrameSubFrameNum();
                 setTxFrameSubFrameNum();
            }
            }
            break;

        case PHY_TXSDU_CONF:
            // This is from PHY indicating receipt of last SDU
            // Need to send next SDU if there's any left
            // TODO: Update state machine
            break;

        case PHY_TXEND_IND:
            // This is from PHY indicating ending of transmission
            // TODO: Update state machine, nothing to send
            break;

        case PHY_RXSTATUS_IND:
        {
            PRXSTATUSIND pRxStatusInd = (PRXSTATUSIND) pMsgHdr;
            PRX_PRACH_STATUS_EVENT pPrachEvents =
                (PRX_PRACH_STATUS_EVENT) &pRxStatusInd->pEventList;
            if (pRxStatusInd->eventNumber != 1 || pPrachEvents->prachPreambleId != 17
                || pPrachEvents->timingOffset != 0)
            {
                prachErrors++;
            }

            if (pRxStatusInd->eventNumber)
            {
                prachSdus++;
            }

            if (pBsMacDesc->PhyAutoCtx.RxSduEna == TRUE)
            {
                U32 RxSduSize = sizeof(RXSTATUSIND);
                U32 HeadersSize = sizeof (ApiHeader) + 1 * sizeof (ApiParamHeader);

                if(pRxStatusInd->statusType == PRACH_RESULT)
                {
                    RxSduSize += pRxStatusInd->eventNumber * sizeof(RX_PRACH_STATUS_EVENT);
                }
                else if(pRxStatusInd->statusType == CQIRIHI_RESULT)
                {
                    PRX_CQIRIHI_STATUS_EVENT pMsg1= (PRX_CQIRIHI_STATUS_EVENT)&pRxStatusInd->pEventList;

                    RxSduSize += pRxStatusInd->eventNumber * sizeof(RX_CQIRIHI_STATUS_EVENT) + pMsg1->status_len;

                    lte_pusch_update_cqi_pmi_ri_harq(pMsg1, pRxStatusInd);
                }
                else
                {
                    RxSduSize += pRxStatusInd->eventNumber * (sizeof(RX_SRS_STATUS_EVENT) + (pRxStatusInd->msgSpecific - sizeof(U8*)));
                }

                // Check buffer overflow and DrainSdu's feature enabled
                if ((RxSduSize + RxSduSet > SvsrGetCommBlockSize() - HeadersSize)
                     && MacDrainRxSduModeOn())
                {
                    // Drain buffer
                    DrainRxSduBufToHost();

                    // Clear buffer counters
                    RxSduSet = 0;
                    RxSduCntr = 0;
                }

                // Add new SDU to buffer
                AddToBuf((U8*) pRxStatusInd, RxSduSize);
            }

        }
        break;

        case PHY_RXSTART_CONF:
            // This is from PHY indicating receipt of RxVector
            // TODO: Update state machine, nothing to send
            break;

        case PHY_RXSTART_IND:
            //printf("PHY_RXSTART_IND\n");
            // This is from PHY indicating start of reception
            // TODO: Update state machine, nothing to send
            break;

        case PHY_RXSDU_IND:
            return MacProcRxSdu((PRXSDUIND)pMsgHdr);

        case PHY_RXEND_IND:
            // This is from PHY indicating end of reception
            // TODO: Update state machine, nothing to send
            break;

        case PHY_INIT_IND:
            // TODO: Update state machine, we are ready to work
            break;

        case PHY_START_CONF:
            // restore API logger mask as PHY may reset it during last transaction
            if(SvsrLoggerMaskStore != 0)
                SvsrLoggerSetMask(SvsrLoggerMaskStore);
#if 0
//#ifdef LOWLATENCY
            if(gMac2PhyDirectFIFOUsed)
            {// send all list
                if (IsICPUEmulated())
                {
                    SendToPhyICpu(IID_LTE_EX, Mac2PhyQueue.StartQueueCurr);
                }
                else
                {
                    SvsrSendMsg(0, IID_LTE_EX, Mac2PhyQueue.StartQueueCurr, SVSR_MSG_OPT_DEFAULT);
                }

                Mac2PhyQueue.StartQueueCurr = NULL;
                Mac2PhyQueue.TailQueueCurr = NULL;
            }
//#endif
#endif
            break;

        case PHY_STOP_CONF:
            break;

        case PHY_STOP_IND:
            pBsMacDesc->PhyAutoCtx.RunEna = FALSE;
            ApiListFree();
            {
                    ApiHeader* buf;

                    buf = (ApiHeader *) SvsrAllocICApi();
                    if (buf == NULL)
                        return RC_SVSR_ALLOC_ERROR;

                    buf = ApiCreate(buf, API_DEF_FLAGS, IID_LTE_MAC, UARM_INSTID_DEFMAC, API_TYPE_IND,
                                    PHY_LTE_STOP);
                    ApiAddErrorCode(buf, SUCCESS);
                    SvsrSendICApi(buf);
            }

            if (Mac2PhyQueue.StartQueueCurr != NULL)
            {
                PMAC2PHY_QUEUE_EL curr,tmp ;

                curr = Mac2PhyQueue.StartQueueCurr;
                while (curr)
                {
                    tmp = curr;
                    curr = curr->Next;
                    tmp->Next = NULL;
                    SvsrFreeIMem(tmp);
                }
                Mac2PhyQueue.StartQueueCurr = NULL;
                Mac2PhyQueue.TailQueueCurr = NULL;
            }

            if ((pBsMacDesc->PhyAutoCtx.RxSduEna == TRUE) && RxSduBuf != NULL)
            {
                if (((1 << MxGetCpuID ()) & MX_TIMER_CORES) != 0)
                    SendBufToHostPostponed(); // this is WA since some messages now (like PRACH) may come after PhyStopInd
                else
                    SendBufToHostThread(NULL);
            }

            break;

        default:
            break;
    }

    return SUCCESS;
}

/** Send API message (84xxx-SWG-009-A) to the PHY using I-Core interface
 *
 * @param nPhyInstID Instance ID of the associated PHY
 * @param pHead Pointer to the transmit queue head
 * @return 4GMX return code
 */
static MXRC SendToPhyICpu(UINT32 nPhyInstID, PMAC2PHY_QUEUE_EL pHead)
{
    PMAC2PHY_QUEUE_EL pElm, tmp, pPrev = NULL;
    PMAC2PHY_QUEUE_EL pListBlock;
    LPVOID pPhyMsg;
    ApiHeader *pApi;
    PMAC2PHY_QUEUE_EL ICpuHead = NULL;

    _ASSERT_PTR(pHead);

    pElm = pHead;
    while (pElm != NULL)
    {
        // yes <RX> is used
        pListBlock = (PMAC2PHY_QUEUE_EL) SvsrAllocICApiEx(SVSR_I_CPU_RX);

        if (pListBlock == NULL)
        {
            uart_printf("There is no memory in I-CPU partition for list block\r\n");
            _ASSERT_PTR(pListBlock);
            return 1;
        }

        // to save the first list block it will be a head
        if (ICpuHead == NULL)
        {
            ICpuHead = pListBlock;
        }

        // yes <RX> is used
        pPhyMsg = SvsrAllocICApiEx(SVSR_I_CPU_RX);

        if (pPhyMsg == NULL)
        {
            uart_printf("There is no memory in I-CPU partition for PHY message\r\n");
            _ASSERT_PTR(pPhyMsg);
            return 2;
        }

        pListBlock->frameNumber = pElm->frameNumber;
        pListBlock->subframeNumber = pElm->subframeNumber;
        pListBlock->MessageType = pElm->MessageType;
        pListBlock->MessageLen = pElm->MessageLen;
        pListBlock->MessagePtr = pPhyMsg;
        pListBlock->Next = NULL;

        if (pPrev != NULL)
        {
            pPrev->Next = pListBlock;
        }

        pPrev = pListBlock;

        if (pElm->MessagePtr != NULL)
        {
            memcpy(pPhyMsg, pElm->MessagePtr, pElm->MessageLen + 32);
        }
        else
        {
            _ASSERT_PTR(pElm->MessagePtr);
            uart_printf("pList->MessagePtr is NULL\r\n");
        }

        MxCacheClean(pListBlock, sizeof(MAC2PHY_QUEUE_EL));
        // TODO: Use cache line size constant
        MxCacheClean(pPhyMsg, pElm->MessageLen + 32);

        pElm = pElm->Next;
    }

    // Create API message
    pApi = SvsrAllocICApiEx(SVSR_I_CPU_RX);
    _ASSERT_PTR(pApi);

    if (pApi == NULL)
    {
        uart_printf("SendToPhyICpu:Can't allocate the memory for API command\r\n");
        return 3;
    }

    pApi = ApiCreate(pApi, API_DEF_FLAGS, 1, nPhyInstID, API_TYPE_CFG_PARAM, 0);
    ApiAddParam(pApi, NULL, 1, &ICpuHead, sizeof(ICpuHead));
    SvsrPutICpuApiToRx(pApi);

    // Free list of messages

    pElm = pHead;
    while (pElm)
    {
        tmp = pElm;
        pElm = pElm->Next;

        if (tmp->MessagePtr != NULL)
        {
            SvsrFreeIMem(tmp->MessagePtr);
        }
        SvsrFreeIMem(tmp);
    }
    return MX_OK;
}

static RESULTCODE MacApplyTimAdvErr(PULSUBFRDESC pUlSubFrame, PTIMADVERRINF ptimAdvErrInfo, PRXSDUIND pRxSduInd)
{
    U32 NumChans;

    if(pRxSduInd->chanType == PUSCH)
    {
        U32 i = 0;
        NumChans = pUlSubFrame->numberofChannelDescriptors - pUlSubFrame->numberOfCtrlChannelDescriptors;
        while(i < NumChans)
        {
            ULCHANDESC* pChan = &pUlSubFrame->ulCh[i];
            if(pRxSduInd->channelId == pChan->channelId)
            {
                pChan->timAdvErrInfo.mErrAvgCh[0] = ptimAdvErrInfo->mErrAvgCh[0];
                pChan->timAdvErrInfo.mErrAvgCh[1] = ptimAdvErrInfo->mErrAvgCh[1];
                pChan->timAdvErrInfo.mErrExpo = ptimAdvErrInfo->mErrExpo;
                pChan->timAdvErrInfo.mSetDefaults = ptimAdvErrInfo->mSetDefaults;
                return SUCCESS;
            }
            ++i;
        }
    }
    else if(pRxSduInd->chanType == PUCCH)
    {
        U32 i = 0;
        NumChans = pUlSubFrame->numberOfCtrlChannelDescriptors;
        while(i < NumChans)
        {
            ULCTRLCHDESC* pChan = &pUlSubFrame->ulCtlCh[i];
            if(pRxSduInd->channelId == pChan->channelId)
            {
                pChan->timAdvErrInfo.mErrAvgCh[0] = ptimAdvErrInfo->mErrAvgCh[0];
                pChan->timAdvErrInfo.mErrAvgCh[1] = ptimAdvErrInfo->mErrAvgCh[1];
                pChan->timAdvErrInfo.mErrExpo = ptimAdvErrInfo->mErrExpo;
                pChan->timAdvErrInfo.mSetDefaults = ptimAdvErrInfo->mSetDefaults;
                return SUCCESS;
            }
            ++i;
        }
    }
    return FAILURE;
}

RESULTCODE MacProcRxSduStatus(PRXSTATUSIND pRxStatusInd)
{

    U32 RxSduSize = sizeof(RXSTATUSIND);
    U32 HeadersSize = sizeof (ApiHeader) + 1 * sizeof (ApiParamHeader);

    if(pRxStatusInd->statusType == CQIRIHI_RESULT)
    {
        PRX_CQIRIHI_STATUS_EVENT pMsg1= (PRX_CQIRIHI_STATUS_EVENT)&pRxStatusInd->pEventList;
        RxSduSize += pRxStatusInd->eventNumber * sizeof(RX_CQIRIHI_STATUS_EVENT) + pMsg1->status_len;
    }
    else
    {
        uart_printf("TestMac Error: message type [%d] in CQI-RI-HARQ list is wrong\n",pRxStatusInd->statusType);
        return FAILURE;
    }

    // Check buffer overflow and DrainSdu's feature enabled
    if ((RxSduSize + RxSduSet > SvsrGetCommBlockSize() - HeadersSize)
         && MacDrainRxSduModeOn())
    {
        // Drain buffer
        DrainRxSduBufToHost();

        // Clear buffer counters
        RxSduSet = 0;
        RxSduCntr = 0;
    }

    // Add new SDU to buffer
    AddToBuf((U8*) pRxStatusInd, RxSduSize);

    return SUCCESS;
}

/** Process just received single RX SDU
 *
 *   - add it to queue for further additional processing, store it in the buffer
 *     or
 *   - pack many SDU to single I-CPU message and send to Host immediately
 *
 * @param pRxSduInd Pointer to the current RX SDU message
 */
static RESULTCODE MacProcRxSdu( PRXSDUIND pRxSduInd)
{
    U8 NextSF = getSubFrameNum();
    U32 Idx;
    U32 ChanId, Put;
    PTIMADVERRINF_FSF pBuf;

    lte_harq_proc_rxsdu(pRxSduInd);

    if(++NextSF >= 10)
         NextSF = 0;

    // SF counter is incremented right after we sent all the APIs to PHY.
    // RX/TX vectors for getSubFrameNum()-1 are processed right now at PHY part.
    // RX/TX vectors for getSubFrameNum() are ready to be sent to PHY
    // timingAdv is supposed to come exactly into getSubFrameNum()+1

    // Actually here we may patch RxTx vectors wich are ready to be sent to PHY (pDlSubFrame, pUlSubFrame) based on hot RxSdu info

    // TimAdvCmdMap will be used by DL scheduler to create PDSCH payload.
    // We will skip one SF if it will be placed in MacProcRxSduQueue
    testConfConfig.TimAdvCmdMap[NextSF] = (U8)pRxSduInd->timingAdv;
    //uart_printf("Got TimAdv: NextSF=%d val=0x%X\r\n", NextSF, pRxSduInd->timingAdv);

    // LoopBack mErrAvgCh, mErrExpo -> patch Rx Vector which ready for sending
    Idx = pRxSduInd->chanType - PUSCH;
    testConfConfig.timAdvErrInfo[Idx].mErrAvgCh[0] = pRxSduInd->mErrAvgCh[0];
    testConfConfig.timAdvErrInfo[Idx].mErrAvgCh[1] = pRxSduInd->mErrAvgCh[1];
    testConfConfig.timAdvErrInfo[Idx].mErrExpo = pRxSduInd->mErrExpo;
    testConfConfig.timAdvErrInfo[Idx].mSetDefaults = 0;

    {
        PULSUBFRDESC pRxVecCurr = LteBsMacUlVectorGet();
        U32 RxSduFSF = pRxSduInd->frameNum * 10 + pRxSduInd->subFrameNum;
        U32 RxVecFSF = pRxVecCurr->frameNumber * 10 + pRxVecCurr->subframeNumber;
        U32 DeltaSF;

        DeltaSF = (RxVecFSF >= RxSduFSF)? (RxVecFSF - RxSduFSF) : ((U32)((U32)0xffff * 10 + 9) - RxSduFSF + RxVecFSF + 1);

        if(DeltaSF == TIMERRINFO_DELAY_SF)
        {
            MacApplyTimAdvErr(pRxVecCurr, &testConfConfig.timAdvErrInfo[Idx], pRxSduInd);
        }
        else
        {// suppose 2
            ChanId = pRxSduInd->channelId;
            Put = testConfConfig.timAdvErrInfoArr_put[ChanId];
            pBuf = &testConfConfig.timAdvErrInfoArrFSF[Put][ChanId];

            pBuf->mErrAvgCh[0] = pRxSduInd->mErrAvgCh[0];
            pBuf->mErrAvgCh[1] = pRxSduInd->mErrAvgCh[1];
            pBuf->mErrExpo = pRxSduInd->mErrExpo;
            pBuf->Fnumber = pRxSduInd->frameNum;
            pBuf->SFnumber = pRxSduInd->subFrameNum;
            pBuf->mSetDefaults = (DeltaSF < TIMERRINFO_DELAY_SF)? 0 : 1;
            if(++Put >= TIMERRBUFDEPTH)
                Put = 0;
             testConfConfig.timAdvErrInfoArr_put[ChanId] = Put;
        }
    }

    if((pRxSduInd->chanType == PUCCH) && (pRxSduInd->pucchDetected == 0))
    {// it's DEAD_ZONE pucch, trash it for TestMAC
        return SVSR_MSG_OPT_DEFAULT;
    }

    if (pBsMacDesc->PhyAutoCtx.RxSduEna == TRUE)
    {
        U32 RxSduSize = sizeof(RXSDUIND);
        U32 HeadersSize = sizeof (ApiHeader) + 1 * sizeof (ApiParamHeader);

        if(pRxSduInd->chanType == PUCCH)
            RxSduSize += ((pRxSduInd->numBitsRx & 0xFFFF) + 7) >> 3;
        else
            RxSduSize += (pRxSduInd->numBitsRx + 7) >> 3;


        // Check buffer overflow and DrainSdu's feature enabled
        if ((RxSduSize + RxSduSet > SvsrGetCommBlockSize() - HeadersSize)
             && MacDrainRxSduModeOn())
        {
            // Drain buffer
            DrainRxSduBufToHost();

            // Clear SDU buffer counters
            RxSduSet = 0;
            RxSduCntr = 0;
        }

        // Add new SDU to buffer
        AddToBuf((U8*) pRxSduInd, RxSduSize);

    }

    if(!testConfConfig.DontUsePuschRxSduForHiAndDci0)
    {
        if(pRxSduInd->chanType == PUSCH)
        {
            PhySduQ[PhySduQPut] = pRxSduInd;

            if (++PhySduQPut >= PhySduQ_SIZE)
                PhySduQPut = 0;
            return SVSR_MSG_OPT_SAVE;
        }
    }

    return SVSR_MSG_OPT_DEFAULT;
}

/** Drain queue containing RX SDUs
 *
 * @param none
 */
static void MacFreeRxSduQueue(void)
{
    PGENMSGDESC pMsgHdr;
    U32 rc;

    while (PhySduQGet != PhySduQPut)
    {
        pMsgHdr = (PGENMSGDESC) PhySduQ[PhySduQGet];

        if(pMsgHdr)
        {
            if ((rc = SvsrFreeIMem(pMsgHdr)) != MX_OK)
                uart_printf("%s: ERROR - cann't free the RxSdu [%x]\n", __FUNCTION__, rc);
        }

        if (++PhySduQGet >= PhySduQ_SIZE)
            PhySduQGet = 0;
    }
}

/** Process queue containing RX SDUs
 *
 * @param pDlSubFrame Pointer to the current Downlink vector
 * @param pUlSubFrame Pointer to the current Uplink vector
 */
static void MacProcRxSduQueue(PDLSUBFRDESC pDlSubFrame, PULSUBFRDESC pUlSubFrame)
{
    PGENMSGDESC pMsgHdr;
    PRXSDUIND pRxSduInd;
    U32 FrameNum;
    U32 SubFrameNum;
    U32 rc;

    if (PhySduQGet == PhySduQPut)
        return;

    pMsgHdr = (PGENMSGDESC) PhySduQ[PhySduQGet];

    if (++PhySduQGet >= PhySduQ_SIZE)
        PhySduQGet = 0;

    pRxSduInd = (PRXSDUIND) pMsgHdr;

    FrameNum = pRxSduInd->frameNum;
    SubFrameNum = pRxSduInd->subFrameNum;

    if(testConfConfig.ULEnabled)
        pHiDciApiSet(pDlSubFrame, pUlSubFrame, pRxSduInd);

    if ((rc = SvsrFreeIMem(pMsgHdr)) != MX_OK)
        uart_printf("TestMac ERROR: cann't free the RxSdu [%x]\n", rc);


    // TODO: just drain rest of RxSdu-s for current TTI for now
    while (PhySduQGet != PhySduQPut)
    {
        pMsgHdr = (PGENMSGDESC) PhySduQ[PhySduQGet];

        if(pMsgHdr)
        {
            pRxSduInd = (PRXSDUIND) pMsgHdr;

            if((pRxSduInd->frameNum != FrameNum) || (pRxSduInd->subFrameNum != SubFrameNum))
                break;

            if ((rc = SvsrFreeIMem(pMsgHdr)) != MX_OK)
                uart_printf("TestMac ERROR: cann't free the RxSdu [%x]\n", rc);
        }

        if (++PhySduQGet >= PhySduQ_SIZE)
            PhySduQGet = 0;
    }
}

/** Process Rx Sdu lists received from PHY
 *
 * @param pClientCtx Not used
 * @param pMsg Pointer to the message buffer
 * @return Standard result code
 */

MXRC LteMacRxSduHandler(LPVOID pClientCtx, LPVOID pMsg)
{
    PMAC2PHY_QUEUE_EL Curr, Tail;
    PRXSDUIND pRxSduInd;
    MXRC rc = SUCCESS;

    RxSduHead = pMsg;
    Curr = pMsg;
    while(Curr)
    {
        pRxSduInd = (PRXSDUIND)Curr->MessagePtr;
        switch(Curr->MessageType)
        {
            case PHY_RXSDU_IND:
                rc = MacProcRxSdu(pRxSduInd);
            break;

            case PHY_RXSTATUS_IND:
                rc = MacProcRxSduStatus((PRXSTATUSIND)pRxSduInd);
            break;

            case PHY_RXEND_IND:
                //uart_printf("TestMac INFO: PHY_RXEND_IND received\n");
                // we have it when some UL channels exist
                if (gMac2PhyDirectFIFOUsed && Mac2PhyQueue.StartQueueCurr)
                {
                    LteMacSendToPhyMessage(PHY_RXSTART_REQ, LteBsMacUlVectorGet(), 1);

                    LteMacCheckMsgIntegrity();
                    if (IsICPUEmulated())
                    {
                        SendToPhyICpu(IID_LTE_EX, Mac2PhyQueue.StartQueueCurr);
                    }
                    else
                    {
                        SvsrSendMsg(0, IID_LTE_EX, Mac2PhyQueue.StartQueueCurr, SVSR_MSG_OPT_DEFAULT);
                    }

                    Mac2PhyQueue.StartQueueCurr = NULL;
                    Mac2PhyQueue.TailQueueCurr = NULL;
                }
                rc = SVSR_MSG_OPT_DEFAULT;
            break;

            case PHY_ERROR_IND:
                {
                    STARTREQ startReq = {0};
                    pBsMacDesc->PhyAutoCtx.RunEna = FALSE;
                    startReq.mode = 0;
                    startReq.msgType = PHY_SHUTDOWN_REQ;
                    startReq.phyEntityId = IID_LTE;
                    ApiListFree();
                    LteMacSendToPhyMessage(PHY_SHUTDOWN_REQ, &startReq, 1);
                    uart_printf("TestMac: Got PHY_ERROR_IND, send PHY_SHUTDOWN_REQ\r\n");
                    rc = SVSR_MSG_OPT_DEFAULT;
                }
            break;

            default:
                uart_printf("TestMac ERROR: unknown type %d of Rx list [%x]\n", Curr->MessageType, rc);
            break;
        }

        if(rc == SVSR_MSG_OPT_DEFAULT)
        {
            if ((rc = SvsrFreeIMem(pRxSduInd)) != MX_OK)
                uart_printf("TestMac ERROR: cann't free the RxSdu [%x]\n", rc);
        }

        Tail = Curr->Next;
        SvsrFreeIMem(Curr);
        Curr = Tail;
    }

    RxSduHead = NULL;
    return rc;
}

static RESULTCODE LteMacIsRxNotExpected(void)
{// it's just required if we need to patch current RxVector based on RxSdu, RxStatus (like timing error advance info)
#if 1
    // if RxVectors patching are not required, return SUCCESS
    return SUCCESS;
#else
    PULSUBFRDESC pUlSubFrame = NULL;
    PRxSubFrmList pList = pBsMacDesc->UlHead;
    int TxFn = getFrameNum();
    int TxSFn = getSubFrameNum();
    int RxFn;
    int RxSFn;

    RxSFn = TxFn * 10 + TxSFn - 2;
    RxFn = RxSFn / 10;
    RxSFn = RxSFn % 10;

    while(pList)
    {
        pUlSubFrame = pList->pUlSubFr;
        if(pUlSubFrame->frameNumber == RxFn && pUlSubFrame->subframeNumber == RxSFn)
        {// found currently expected channels
            if((pUlSubFrame->numberofChannelDescriptors == 0)
                && (pUlSubFrame->numberSrsinSf == 0))
                return SUCCESS;
        }
        pList = pList->next;
    }
    return FAILURE;
#endif
}

/** Frees internal structures related to the MAC-PHY communication
 *
 * @param pMac2Phy Pointer to the communication context
 * @return Standard result code
 */
RESULTCODE LtePhyIfaceClean(Mac2PhyInterface *pMac2Phy)
{
    pMac2Phy->txCb = NULL;
    pMac2Phy->rxCb = NULL;
    pMac2Phy->toPhy = NULL;
    pMac2Phy->fromPhy = NULL;

    if(HANDLE_NOT_NULL(dlControlTxSduQueue))
    {
        MxShareObject(dlControlTxSduQueue, OBJ_SHARE_OFF);
        MxRemoveQueue(dlControlTxSduQueue);
        dlControlTxSduQueue = HNULL;
    }

    if(HANDLE_NOT_NULL(dlControlTxSduQueue2))
    {
        MxShareObject(dlControlTxSduQueue2, OBJ_SHARE_OFF);
        MxRemoveQueue(dlControlTxSduQueue2);
        dlControlTxSduQueue2 = HNULL;
    }

    if(pMac2Phy->pMem[0])
    {
        MxHeapFree(AppHeap, (U32*) pMac2Phy->pMem[0]);
        pMac2Phy->pMem[0] = NULL;
    }

    if(pMac2Phy->pMem[1])
    {
        MxHeapFree(AppHeap, (U32*) pMac2Phy->pMem[1]);
        pMac2Phy->pMem[1] = NULL;
    }

    return SUCCESS;
}

/** Initializes internal structures related to the MAC-PHY communication
 *
 * @param pMac2Phy Pointer to the communication context
 * @param txCb Callback function used to transmit API messages to PHY
 * @param rxCb Callback function used to receive messages from PHY
 * @param toPhy Application-defined parameter associated with the txCb
 * @param fromPhy Application-defined parameter associated with the rxCb
 * @return Standard result code
 */
RESULTCODE LtePhyIfaceInit(Mac2PhyInterface *pMac2Phy,
                           PhyTxCb txCb,
                           PhyRxCb rxCb,
                           PTR toPhy,
                           PTR fromPhy)
{
    PTR pMem = NULL;
    MXRC rc;

    if (HANDLE_NULL(dlControlTxSduQueue))
    {
        /* Tx Ue Queue  */
        pMem = MxHeapAlloc(AppHeap, DLTXSDU_QUEUE_SIZE * sizeof(U32*));
        if (pMem == NULL)
        {
            printf("ERROR: No memory [%s,%s:%d]\n", __FUNCTION__, __FILE__, __LINE__);
            return FAILURE;
        }

        rc = MxCreateQueue(pMem, DLTXSDU_QUEUE_SIZE, sizeof(U32*), NULL, HNULL,
                           &dlControlTxSduQueue);
        if (FAILED(rc))
        {
            printf("ERROR: No memory [%s,%s:%d]\n", __FUNCTION__, __FILE__, __LINE__);
            return FAILURE;
        }
        rc = MxShareObject(dlControlTxSduQueue, OBJ_SHARE_ON);
        _ASSERT_RC(rc);
        if (FAILED(rc))
            return rc;

        pMac2Phy->pMem[0] = (U32) pMem;
    }

    if (HANDLE_NULL(dlControlTxSduQueue2))
    {
        pMem = MxHeapAlloc(AppHeap, DLTXSDU_QUEUE_SIZE * sizeof(U32*));
        if (pMem == NULL)
        {
            printf("ERROR: No memory [%s,%s:%d]\n", __FUNCTION__, __FILE__, __LINE__);
            return FAILURE;
        }

        rc = MxCreateQueue(pMem, DLTXSDU_QUEUE_SIZE, sizeof(U32*), NULL, HNULL,
                           &dlControlTxSduQueue2);
        if (FAILED(rc))
        {
            printf("ERROR: No memory [%s,%s:%d]\n", __FUNCTION__, __FILE__, __LINE__);
            return FAILURE;
        }

        rc = MxShareObject(dlControlTxSduQueue2, OBJ_SHARE_ON);
        _ASSERT_RC(rc);
        if (FAILED(rc))
            return rc;

        pMac2Phy->pMem[1] = (U32) pMem;
    }

    // Init PHY interface
    pMac2Phy->txCb = txCb;
    pMac2Phy->rxCb = rxCb;
    pMac2Phy->toPhy = toPhy;
    pMac2Phy->fromPhy = fromPhy;

    return SUCCESS;
}

/** Initialize PHY parameters to default values
 *  Will be used in PHY_INIT.req message
 *
 * @param pParm Pointer to preallocated structure holding PHY initialization values
 * @return Standard result code
 */
RESULTCODE LtePhyIfInitDefault(PINITPARM pParm)
{
    pParm->channelBandwidth = BW_10_0_MHZ;
    pParm->frtypeDuplexmode = FDD;
    pParm->radioAccessMode = OFDMA;
    pParm->physicalResourceBandwidth = PRB_12;
    pParm->numberResourceBlocksperSlot = RB_SLOT_50;
    pParm->phyLayerCellIdGroup = 0;
    pParm->phyLayerId = 0;
    pParm->txAntennaPortCount = TXANT_ONE;
    pParm->rxAntennaPortCount = RXANT_ONE;
    pParm->txStartConfDisable = 1;
    pParm->rxStartConfDisable = 1;
    pParm->fastfwdorPreclockingNumber = 1600;
    pParm->fftSize = FFTSZ_1024;
    pParm->numberUsedSubcarriers = NUSC_600;
    pParm->nMaxDlRb = NMAXRB_50;
    pParm->nMaxUlRb = NMAXRB_50;
    pParm->referenceSignalPower = 250;
    pParm->primarySyncSignalPower = 65;
    pParm->secondarySyncSignalPower = 65;
    pParm->numDataRePerPRB = 80;
    pParm->cyclicPrefixType = NORMAL;
    pParm->txSduConfEnable = 0;
    pParm->radioFrameNumber = 0;
    pParm->subframeNumber = 0;
    pParm->slotNumber = 0;
    pParm->srsBandwidthConfig = 0;
    pParm->srsSubframeConfig = 15;      // SRS disabled
    pParm->srsSimultaneousANandSRS = 0;

    pParm->prachConfigurationIndex = 0;
    pParm->prachFrequencyOffset = 0;
    pParm->prachHighSpeedFlag = 0;
    pParm->prachCyclicShiftConfig = 0;
    pParm->prachRootSequenceIndex = 0;

    pParm->rxAntennaPortCount = 1;
    pParm->txAntennaPortCount = 1;

    return SUCCESS;
}



/*UMTSNMM*/
static MXRC MacSendToPhyUMTSNMM(U8 *MessageTypes, LPVOID *pDatas, UINT32 *nMsgSizes, UINT32 nMessages)
{
    PMAC2PHY_QUEUE_EL pCtrlBlock;
    PMAC2PHY_QUEUE_EL pCtrlPreviousBlock = NULL;
    PMAC2PHY_QUEUE_EL pHead = NULL;
    U8 MessageType;
    LPVOID pData;
    UINT32 nMsgSize;
    MXRC rc;
    U32 i;

    for (i = 0; i < nMessages; i++) {
        MessageType = MessageTypes[i];
        pData = pDatas[i];
        nMsgSize = nMsgSizes[i];

        _ASSERT_PTR (pData);
        pCtrlBlock = (PMAC2PHY_QUEUE_EL) SvsrAllocMsgEx(sizeof(MAC2PHY_QUEUE_EL));
        if (pCtrlBlock == NULL) {
            uart_printf("No memory to send I-CORE data, pData=%x, Size=%d\r\n", pData, nMsgSize);
            _ASSERT_PTR(pCtrlBlock);

            return 1;
        }

        if (i == 0)
            pHead = pCtrlBlock;

        memset(pCtrlBlock, 0, sizeof(*pCtrlBlock));
        pCtrlBlock->MessagePtr = (U8*) pData;
        pCtrlBlock->MessageLen = nMsgSize;
        pCtrlBlock->MessageType = MessageType;
        pCtrlBlock->Next = NULL;
        if (pCtrlPreviousBlock)
            pCtrlPreviousBlock->Next = pCtrlBlock;
        pCtrlPreviousBlock = pCtrlBlock;
    }

    rc = SvsrSendMsg(0, IDD_UMTS_NWL, pHead, SVSR_MSG_OPT_DEFAULT);

    _ASSERT_RC(rc);
    if (FAILED(rc))
    {
        uart_printf("MacSendToPhy::SvsrSendMsg has failed, rc=%x\r\n", rc);
        return rc;
    }
    return MX_OK;
}
