
// -------------------------------------------------------------------------------------------
/** @file lte_bs_phy_nmm_fdd.c
 *
 * @brief implements LTE NMM control and some signal processing functions
 * @author Mindspeed Technologies
 *
 * COPYRIGHT&copy; 2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
// -------------------------------------------------------------------------------------------

/*======================== includes ==========================*/	
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#include "typedef.h"
#include "ltephydef.h"
#include "prototypes.h"
#include "ext_phy.h"
#include "LtePhyL2Api.h"
#include "lte_bs_phy_nmm_fdd.h"
#include "lte_bs_phy_nmm_tables.h"
#include "lte_bs_phy_nmm_thread.h"

#include "tcb.h"

#ifdef __CC_ARM
#include "svsrlogger.h"
#endif

#ifdef _WIN32
#include "Appids.h"
#include "svsrlogger_c_code.h"
#endif

#ifdef CEVA_INTRINSICS
#include <basic_op.h>
#else
#include "basic_op.h"
#endif

/*======================== macros ==========================*/	
#define     DUMP_IQ_DATA_MODE 0 //0: no dump; 1: dump for cell search; 2: dump for BCCH

#ifdef MX_HEAP_LEAKAGE_CONTROL
#define MX_HEAP_ALLOC(heap, size)   MxHeapAllocEx((heap), (size), _LR)
#else
#define MX_HEAP_ALLOC(heap, size)   MxHeapAlloc((heap), (size))
#endif

#ifndef _WIN32
#define NMM_MALLOC_TRACE 0
#endif

#if NMM_MALLOC_TRACE
#define MemAlloc nmm_memalloc_trace
#define MemFree nmm_memfree_trace
#endif

#define NMM_RSSI_USETCB 1

#define MAX_NUM_CAND_CH_EST 10
	
/*======================== declarations ==========================*/	
// Listening Structure
U32 listeningActiveFlag;
PLISTENINGDESC pGlobalListening = NULL;
#if defined _WIN32
    U8              *pGlobalNMMOutApiMsg = NULL;
    U32             *globalNMMoutApiMsgLen;
#endif


#if NMM_CEVA_DBG
extern char    *arm_dbg;
#endif

#if NMM_MALLOC_TRACE
void           *nmm_memalloc_trace(U32 size, U32 memId, U32 InitFlag);
void           *nmm_memfree_trace(void *ptr, U32 memId);
#endif

#ifdef _WIN32
FILE           *fIQ = NULL;
#endif

extern S16     *PSync_f_arr[];
extern S16     *PSyncExp_arr[];
extern S32     *Psync_foe_arr[];
extern S16     *SSync0_f_arr[];
extern S16     *SSync0Exp_arr[];
extern S16     *SSync1_f_arr[];
extern S16     *SSync1Exp_arr[];
extern S16     *SSync2_f_arr[];
extern S16     *SSync2Exp_arr[];
extern S16     *SSync0Est_arr[];
extern S16     *SSync1Est_arr[];
extern S16     *SSync2Est_arr[];
extern S16     *pci0_arr[];
extern S16     *pci1_arr[];
extern S16     *pci2_arr[];
extern S16     *subframe0_arr[];
extern S16     *subframe1_arr[];
extern S16     *subframe2_arr[];
extern S16     *data_SSS_PSS_arr[];
extern S16      sync_cand;

PTASKLIST               nmm_task_list[NMM_NUM_LIST] = {NULL};
PTCB                    nmm_tcb_arr[NMM_NUM_LIST][MAX_NMM_NUM_TCB_PER_LIST] = { 0 };
#ifdef RT_API_PROC
NCNBVAR volatile U32    nmm_task_list_done[NMM_NUM_LIST];
NCNBVAR volatile U32    nmm_task_list_time[NMM_NUM_LIST];
NCNBVAR volatile U32    nmm_task_list_count[NMM_NUM_LIST];
#else
volatile U32    nmm_task_list_done[NMM_NUM_LIST];
volatile U32    nmm_task_list_count[NMM_NUM_LIST];
#endif
U16             N_PBCH_TCB;
U16             N_PDSCH_tcb;


S16            *nmm_iq_storage = NULL;
U32             nmm_iq_storage_curr_idx = 0;
U32             nmm_iq_storage_subfr_writer = 0;
U32             nmm_iq_storage_subfr_reader = 0;
U32             nmm_subframe_count = 0;
U32             nmm_proc_subframe;

#if DBG
static U32      cnt = 2;
#endif

S16            *pbch_sym_arr[MAX_CELL_FOR_PBCH_EXTRACT * REF_SIGNAL_PER_SLOT * 2 * MAX_ANTENNA_FOR_NMM];
S16            *pbch_exp_arr[MAX_CELL_FOR_PBCH_EXTRACT * REF_SIGNAL_PER_SLOT * 2 * MAX_ANTENNA_FOR_NMM];
S16            *pbch_len_arr[MAX_CELL_FOR_PBCH_EXTRACT * REF_SIGNAL_PER_SLOT * 2 * MAX_ANTENNA_FOR_NMM];

volatile U32    nmm_iq_subframe_copy_done;
extern U8      *current_decoded_sib_packed;     /* FIXME (OS) - tmp */
/*======================== function definitions ==========================*/	



U32 LteBsIsNmmListeningActive(void)
{
    return listeningActiveFlag;
}

PLISTENINGDESC LteBsGetNmmCtx(void)
{
    if (LteBsIsNmmListeningActive())
        return pGlobalListening;
    else
        return NULL;
}

/*
 * the state machine for Network Monitoring Mode
 */

#ifdef NMM_DBG_PRINT
char           *NMMStateDbg[] = {
    "NMM_ABORT",
    "NMM_IDLE",
    "NMM_RSSI_MEASURING",
    "NMM_RSSI_MEASURING_DONE",
    "NMM_SYNC_SEARCHING",
    "NMM_SYNC_SEARCHING_DONE",
    "NMM_PBCH_SEARCHING",
    "NMM_PBCH_SEARCHING_DONE",
    "NMM_PBCH_SYNC_SEARCHING",
    "NMM_SIB1_SEARCHING",
    "NMM_SIB1_SEARCHING_DONE",
    "NMM_FULL_BCCH_SEARCHING",
    "NMM_FULL_BCCH_SEARCHING_DONE",
	"NMM_TRACKING",
};
#endif


//--------------------------------------------------------------------------------------------------------------------------
/** @brief convert linear power to dB 
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param in       input linear power
 *  @return         dB power 
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |in             |               |input        |1              |Real          |1:15:0    |[-2^15,2^15-1] |         |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------
 *  |Block Name          |dbmconv      |
 *  ---------------------------------------
 *  |Input Stream(s)     |in |
 *  ---------------------------------------
 *  |Output Stream(s)    |None|
 *  ---------------------------------------
 *  |Config Parameter(s) |None|
 *  ---------------------------------------
 *  |Status Parameter(s) |dB_power |
 *  ---------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * convert linear power to dB 
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * 
 * 
 *
 * \ingroup group_lte_nmm_rssi
 *
 *
**/
//--------------------------------------------------------------------------------------------------------------------------

/*
 * 20log10(in)
 */
static          S16
dbmconv(S16 in)
{
    S16             expo,
                    temp1,
                    temp2;
    S32             L_temp1;

    // Convert to dB
    expo = norm_l(in);
    L_temp1 = L_shl(in, expo);
    temp1 = log_fx(L_temp1, expo + 15); // since in is in Q15
    temp2 = shr(temp1, 8);      // temp1 is in Q9, 1/2 dB step
    return temp2;
}

static int
is_freqband_supported_by_hw(LISTENINGDESC * pListening, U8 band)
{
    int             i;

    for (i = 0; i < pListening->nb_supported_radio_band; i++)
        if (band == pListening->supported_radio_band_list[i])
            return TRUE;
    return FALSE;

}

static int
is_bw_ok(U8 bw)
{
    int             i;
    U8              NumberOfResourceBlocks[6] = { RB_SLOT_6, RB_SLOT_15, RB_SLOT_25, RB_SLOT_50, RB_SLOT_75, RB_SLOT_100 };
    for (i = 0; i < 6; i++)
        if (bw == NumberOfResourceBlocks[i])
            return TRUE;
    return FALSE;
};

static          S16
get_bw_inv(U8 bw)
{
    int             i;
    U8              NumberOfResourceBlocks[6] = { RB_SLOT_6, RB_SLOT_15, RB_SLOT_25, RB_SLOT_50, RB_SLOT_75, RB_SLOT_100 };
    S16             NumberOfResourceBlocks_inv[6] = { 5461, 2184, 1310, 655, 437, 327 };
    for (i = 0; i < 6; i++)
        if (bw == NumberOfResourceBlocks[i])
            return NumberOfResourceBlocks_inv[i];
    return 0;
};

static int
is_si_periodicity_ok(U16 period)
{
    int             i;
    U16             possible_periodicity[7] = { 80, 160, 320, 640, 1280, 2560, 5120 };
    for (i = 0; i < 7; i++)
        if (period == possible_periodicity[i])
            return TRUE;
    return FALSE;
};

static int
is_si_win_ok(U8 win)
{
    int             i;
    U8              possible_win[7] = { 1, 2, 5, 10, 15, 20, 40 };
    for (i = 0; i < 7; i++)
        if (win == possible_win[i])
            return TRUE;
    return FALSE;
};

static int
is_pci_in_nmm_list(S16 pci, S16 * list, S16 size)
{
    int             i;

    for (i = 0; i < size; i++)
        if (pci == list[i])
            return TRUE;
    return FALSE;
};

static int
is_pci_already_found(S16 pci, CELL_PARAMS * cell, S16 size)
{
    int             i;

    for (i = 0; i < size; i++) {
        if (pci == cell->cell_pci)
		{
			cell->cell_detect_num++;
            return TRUE;
		}
        cell++;
    }
    return FALSE;
};

#ifdef _WIN32
U32
lte_bs_phy_nmm_parse_msg(U32 phyInstance, U32 * inMsg, U32 inMsgSize, U32 * outMsg, U32 * outMsgSize)
{
    U16             len = 0;
    U32            *ptr;
    PGENMSGDESC     pMsg = (PGENMSGDESC) inMsg;
    U32             status = 0;
    volatile U32    t = 0;      // Define time variable used for profiling


    *outMsgSize = 0;
    if(pGlobalNMMOutApiMsg == NULL)
    {
        pGlobalNMMOutApiMsg = (U8*)outMsg;
        globalNMMoutApiMsgLen = outMsgSize;
    }


    len = (U16) pMsg->msgSpecific;
    ptr = ++inMsg;
    SvsrLoggerAddMsgForce(pMsg->msgType, LOGGER_MSG_INFO(0, 0, IID_LTE_MAC, IID_LTE), pMsg, len + sizeof(GENMSGDESC));
    nmm_dbg_printf(("%s (%d)\r\n", __FUNCTION__, pMsg->msgType));

    switch (pMsg->msgType) {
    case NMM_START:
        status = lte_bs_phy_nmm_start(phyInstance, (void *) ptr, len);

		gMode = NMM_MODE;
        break;

    case NMM_STOP:
        status = lte_bs_phy_nmm_stop(phyInstance, (void *) ptr, len);
        break;

    case NMM_RSSI_MEAS_REQ:
        status = lte_bs_phy_nmm_rssi_meas_req(phyInstance, (void *) ptr, len);
        break;

    case NMM_STOP_RSSI_MEAS_REQ:
        status = lte_bs_phy_nmm_stop_rssi_meas_req(phyInstance, (void *) ptr, len);
        break;

    case NMM_CELL_SEARCH_REQ:
        status = lte_bs_phy_nmm_cell_search_req(phyInstance, (void *) ptr, len);
        break;

    case NMM_STOP_CELL_SEARCH_REQ:
        status = lte_bs_phy_nmm_stop_cell_search_req(phyInstance, (void *) ptr, len);
        break;

    case NMM_PBCH_CONFIG_REQ:
        status = lte_bs_phy_nmm_pbch_config_req(phyInstance, (void *) ptr, len);
        break;

    case NMM_STOP_PBCH_REQ:
        status = lte_bs_phy_nmm_stop_pbch_req(phyInstance, (void *) ptr, len);
        break;

    case NMM_SIB1_CONFIG_REQ:
        status = lte_bs_phy_nmm_sib1_config_req(phyInstance, (void *) ptr, len);
        break;

    case NMM_BCCH_CONFIG_REQ:
        status = lte_bs_phy_nmm_bcch_config_req(phyInstance, (void *) ptr, len);
        break;

    case NMM_STOP_BCCH_REQ:
        status = lte_bs_phy_nmm_stop_bcch_req(phyInstance, (void *) ptr, len);
        break;

    default:
        // Not a valid command return failure
        status = FAILURE;
        break;
    }

    return (status);
}

#endif

//--------------------------------------------------------------------------------------------------------------------------
/** @brief schedule the task list to run 
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param listNum      task list number
 *  @return None
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |listNum        |               |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------
 *  |Block Name          |nmm_runTaskList      |
 *  ---------------------------------------
 *  |Input Stream(s)     |None |
 *  ---------------------------------------
 *  |Output Stream(s)    |None|
 *  ---------------------------------------
 *  |Config Parameter(s) |listNum |
 *  ---------------------------------------
 *  |Status Parameter(s) |None |
 *  ---------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * schedule the task list to run;
 * wait until the task list is finished execution.
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * 
 * 
 *
 * \ingroup group_lte_nmm_phy
 *
 *
**/
//--------------------------------------------------------------------------------------------------------------------------

#ifdef SCHEDULER
void nmm_runTaskList(U32 listNum)
{
    U32 taskListRet;
    PTASKLIST pTaskList = NULL;

    if (listNum > NMM_NUM_LIST)
    {
        nmm_dbg_printf(("nmm_runTaskList: Invalid listNum: %d\r\n", listNum));
        return;
    }

    pTaskList = nmm_task_list[listNum];

    nmm_task_list_done[listNum] = 0;
#ifdef RT_API_PROC
    nmm_task_list_time[listNum] = GET_TICKS;
#endif
    nmm_task_list_count[listNum]++;

    taskListRet = ExeTSchedList(pTaskList);
    if(taskListRet != MX_OK)
    {
        nmm_dbg_printf(("ERROR: Task list %d returned %d\r\n", listNum, taskListRet));
        return;
    }
    while(nmm_task_list_done[listNum] == 0);
    ReinitTaskList (pTaskList, NULL);
}
//--------------------------------------------------------------------------------------------------------------------------
/** @brief function called by scheduler when the task list is done 
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param thisList  pointer to task list
 *  @param param     parameter
 *  @return None
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |thisList       |               |input        |1              |pStructure    |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |param          |               |             |               |pReal         |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------
 *  |Block Name          |nmm_task_list_doneCB      |
 *  ---------------------------------------
 *  |Input Stream(s)     |None |
 *  ---------------------------------------
 *  |Output Stream(s)    |None|
 *  ---------------------------------------
 *  |Config Parameter(s) |thisList,param |
 *  ---------------------------------------
 *  |Status Parameter(s) |None |
 *  ---------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * function called by scheduler when the task list is done 
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * 
 * 
 *
 * \ingroup group_lte_nmm_phy
 *
 *
**/
//--------------------------------------------------------------------------------------------------------------------------

void nmm_task_list_doneCB(TASKLIST* thisList, void* param)
{


    _ASSERT_PTR(thisList);

    *((U32*)param) = 1;
#ifdef RT_API_PROC
	{
		U32 listNum;
		listNum = RexGetListID(thisList) - NMM_TASKLIST0_IDX;
		MLogTask(PID_TASKLIST_EXEC_NMM_0+listNum, RESOURCE_LARM, nmm_task_list_time[listNum], GET_TICKS);
	}
#endif
}

#else
void nmm_runTaskList(U32 listNum)
{
    return;
}
#endif

//--------------------------------------------------------------------------------------------------------------------------
/** @brief bypass the TCBs not run in a task list 
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param ppTCB        pointer to the pointer of TCB
 *  @param nTcbNum      number of TCBs to bypass
 *  @param nCtrl        TCB property
 *  @return None
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |ppTCB          |               |input        |variable       |pStructure    |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nTcbNum        |               |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nCtrl          |               |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              |    
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_byPassMultTcb      |
 *  ---------------------------------------
 *  |Input Stream(s)     |ppTCB |
 *  ---------------------------------------
 *  |Output Stream(s)    |ppTCB|
 *  ---------------------------------------
 *  |Config Parameter(s) |nTcbNum,nCtrl |
 *  ---------------------------------------
 *  |Status Parameter(s) |None |
 *  ---------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * bypass the TCBs not run in a task list 
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * 
 * 
 *
 * \ingroup group_lte_nmm_phy
 *
 *
**/
//--------------------------------------------------------------------------------------------------------------------------
#ifdef SCHEDULER
void lte_bs_phy_nmm_byPassMultTcb (TCB ** ppTCB, UINT32 nTcbNum, UINT32 nCtrl)
{
    U16 i;
    for(i=0;i<nTcbNum;i++)
    {
        MarkTCBControl(ppTCB[i], nCtrl,  NULL);
    }
}
#else
void lte_bs_phy_nmm_byPassMultTcb (TCB ** ppTCB, UINT32 nTcbNum, UINT32 nCtrl)
{
    return;
}
#endif

//#endif

//--------------------------------------------------------------------------------------------------------------------------
/** @brief generate API response message to higher layer 
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param phyInstance      PHY instance ID
 *  @param inMsg            input message pointer
 *  @param inMsgSize        length of input message
 *  @param outMsg           output message pointer
 *  @param msgType          message type
 *  @param status           the result of the API message processing
 *  @return None
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |phyInstance    |               |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |inMsg          |               |input        |variable       |pReal         |0:32:0    |Full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |inMsgSize      |               |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |outMsg         |               |output       |variable       |pReal         |0:32:0    |Full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------     
 *  |msgType        |               |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              |
 *  |statys         |               |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_generate_api_msg      |
 *  ---------------------------------------
 *  |Input Stream(s)     |inMsg |
 *  ---------------------------------------
 *  |Output Stream(s)    |outMsg|
 *  ---------------------------------------
 *  |Config Parameter(s) |phyInstance,inMsgSize,msgType,status |
 *  ---------------------------------------
 *  |Status Parameter(s) |None |
 *  ---------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * add message header;
 * call driver function to send API response message to higher layer
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * 
 * 
 *
 * \ingroup group_lte_nmm_api
 *
 *
**/
//--------------------------------------------------------------------------------------------------------------------------
void
lte_bs_phy_nmm_generate_api_msg(U32 phyInstance, U8 * inMsg, U32 inMsgSize, U8 * outMsg, U32 msgType, U32 status)
{

    PLTEPHYDESC     pPhy = getPhyCtxApiProc(phyInstance);
    PINITIND        ptr;
    // PBS_RX pBsRx;
    PMSGIND         pMsg;

    PLISTENINGDESC  pListening = LteBsGetNmmCtx();
    int             outMsgSize = sizeof(MSGIND) + inMsgSize;
    U8             *p8;
    U32            *tmp32;

    U32 *pOutMsg;
#ifdef _WIN32
    pOutMsg = (U32 *)outMsg;
#else
    pOutMsg = (U32 *)PhyNMM2MacIfApiBuffer(inMsgSize + sizeof(MSGIND) + 32);
#endif

#ifndef WIN32
    if(pOutMsg == NULL)
    {
        uart_printf("lte_bs_phy_nmm_generate_api_msg: can't allocate ICPU block (UNMM=%d)\n", GetUNmmInstanceId());
        return;
    }
#endif

    ptr = (PINITIND) pOutMsg;
    ptr->phyEntityId = 0;
    ptr->msgType = (U8) msgType;
    ptr->status = (U8) status;
    pMsg = (PMSGIND) pOutMsg;
    p8 = (U8*)pOutMsg + sizeof(MSGIND);

    /* to be changed */
    pMsg->subFrameNum = pListening->sniffing_time % 10;
    pMsg->frameNumber = (pListening->sniffing_time / 10) & 1023;	//0~1023
	pMsg->reserved = outMsgSize;	//reserved U16 used to indicate msgLength

    if (inMsgSize)
        memcpy(p8, inMsg, inMsgSize);

    switch (msgType) {
    case NMM_RSSI_MEAS_IND:
        tmp32 = (U32 *) p8;
        nmm_dbg_printf(("\t== [%d] NMM_RSSI_MEAS_IND==\r\n", pListening->sniffing_time));
        nmm_dbg_printf(("rssi(dBFS) %d earcn %d\r\n", (S8) (((*tmp32) & 0xff) - 128), extract_l(*tmp32 >> 8)));
        break;

    case NMM_RSSI_MEAS_CNF:
        nmm_dbg_printf(("\t== [%d] NMM_RSSI_MEAS_CNF (done)==\r\n", pListening->sniffing_time));
        break;

    case NMM_CELL_SEARCH_IND:
        nmm_dbg_printf(("\t== [%d] NMM_CELL_SEARCH_IND (done)==\r\n", pListening->sniffing_time));
        nmm_dbg_printf(("cells %d pci %d rsrp %d foe %d timingoffset %d \r\n", pListening->dlrs_foe_cellIdx, *((S16 *) (&p8[2])), p8[1],
			pListening->pcell_search->cell_param[pListening->dlrs_foe_cellIdx].cell_freqency_offset >> 13,
			pListening->pcell_search->cell_param[pListening->dlrs_foe_cellIdx].cell_time_offset >> 4));
        break;

    case NMM_CELL_SEARCH_CNF:
        nmm_dbg_printf(("== NMM_CELL_SEARCH_CNF done ==\r\n"));
        break;

    case NMM_PBCH_DATA_IND:
        nmm_dbg_printf(("\t== [%d] NMM_PBCH_DATA_IND (done)==\r\n", pListening->sniffing_time));
        nmm_dbg_printf(("cells pci %d status %d antenna %d sfn %d phich(%d,%d) bw %d foe %d\r\n", *((S16 *) (&p8[0])), p8[2], p8[3], *((S16 *) (&p8[4])), p8[6], p8[7], p8[8],
            pListening->pcell_search->cell_param[pListening->pbch_search_cell_index].cell_freqency_offset >> 13));
        break;

    case NMM_BCCH_DATA_IND:
        //nmm_dbg_printf(("\t== [%d] NMM_BCCH_DATA_IND (done) status: %d ==\r\n", pListening->sniffing_time,status));
        break;

    case NMM_SIB1_CONFIG_CNF:
        nmm_dbg_printf(("\t== [%d] NMM_SIB1_CONFIG_CNF (done) status: %d ==\r\n", pListening->sniffing_time,status));

    default:
        // Return Failure status
        // ptr->status = (U8) lstatus;
        break;
    }
    // Now message is ready to be send
    DUMP_ARR_ID_UART(pOutMsg, outMsgSize, "x", 0, 1);

    if (outMsgSize)
    {
        SvsrLoggerAddMsgForce(msgType, LOGGER_MSG_INFO(0, 0, IID_LTE_NMM, IID_LTE_MAC),
                        pOutMsg, (U32)outMsgSize);
#ifdef _WIN32
        memcpy(pGlobalNMMOutApiMsg + (*globalNMMoutApiMsgLen), pOutMsg, outMsgSize);
        *globalNMMoutApiMsgLen += (outMsgSize+3) & (~3);
#else
        LteSendMsgToUpperNmm((U32 *) pOutMsg, (U32) outMsgSize);
#endif
    }

#if defined(_LTEAPIDUMP_) && !defined(_WIN32)
    if ((SvsrDumpMask & TXEN) && (outMsgSize != 0))
        LteApiDumpOut(pOutMsg, outMsgSize);
#endif
}



/*
 *-------------------------------
 * U16 Supported_RAT Bitfield
 *-------------------------------
 * Bit 0: LTE
 * Bit 1: WCDMA
 * Bit 2: 2G
 * Bit 3 : CDMA (??)
 * Bit 4-15 : Reserved
 *-------------------------------
 * U16 Duplex 0-1
 * 0 : FDD
 * 1 : TDD
 *-------------------------------
 */

//--------------------------------------------------------------------------------------------------------------------------
/** @brief process the NMM start API message 
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param phyInstance  PHY instance ID
 *  @param ptr          pointer to API message 
 *  @param len          length of API message
 *  @return status(result) of API message processing 
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |phyInstance    |               |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |ptr            |               |input        |variable       |pReal Array   |0:8:0     |[0,2^8-1]      |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |len           |                |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              | 
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_start      |
 *  ---------------------------------------
 *  |Input Stream(s)     |None |
 *  ---------------------------------------
 *  |Output Stream(s)    |None|
 *  ---------------------------------------
 *  |Config Parameter(s) |phyInstance,ptr,len |
 *  ---------------------------------------
 *  |Status Parameter(s) |None |
 *  ---------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * process the NMM start API message 
 * allocate memories for NMM;
 * initialize global variables;
 * create TCB lists;
 * generate response message;
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * 
 * 
 *
 * \ingroup group_lte_nmm_api
 *
 *
**/
//--------------------------------------------------------------------------------------------------------------------------
U32
lte_bs_phy_nmm_start(U32 phyInstance, void *ptr, U32 len)
{
    U32             status = 0;
    PLISTENINGDESC  pListening;
    U16            *p16 = (U16 *) ptr;
    PLTEPHYDESC     pPhy = getPhyCtxApiProc(phyInstance);
#ifdef SCHEDULER
    U16 n_cand,i,n_tcb;
#endif

    nmm_dbg_printf(("[%s]\tARM %d\tp16 %x\ttime %lu\r\n", __FUNCTION__, MxGetCpuID(), p16[0], (unsigned long) GET_TICKS));

    /* only 1 global instance possible */
    if (listeningActiveFlag)
    {
        uart_printf("Listening Mode already Started\r\n");
        return (NMM_LTE_IN_USE);
    }

    listeningActiveFlag = 1;

    pGlobalListening = MemAlloc(sizeof(LISTENINGDESC), DDR_MEM, 1);
    pListening = pGlobalListening;
    nmm_iq_storage = MemAlloc(NMM_IQ_STORAGE_SIZE * sizeof(S16), DDR1_NCNB_MEM, 1);
    nmm_dbg_printf(("\n============== nmm_iq_storage addr: %08x  ==============\r\n",(U32)nmm_iq_storage));

    // phy_printf("@ %nmm_iq_storage %x\r\n", nmm_iq_storage);
    _ASSERT_PTR(nmm_iq_storage);
    nmm_iq_storage_curr_idx = 0;
    nmm_iq_storage_subfr_writer = 0;
    nmm_iq_storage_subfr_reader = 0;

    pListening->pbch_earfcn = 0xffff;   /* init */

    pListening->listening_active = p16[0] & NMM_LTE_ENABLE;
    pListening->duplex = p16[1] & 0x0001;
    if (pListening->duplex != 0) {
        status = NMM_LTE_TDD_NOT_SUPPORTED;
        goto nmm_start_end;
    }

    pListening->report_individual_cell_params = TRUE;
    pListening->state = NMM_IDLE;
    pListening->target_state_mask = 0;
    pListening->received_nmm_stop_flag = 0;
	pListening->search_timing_for_all_cells = 1;

    /* query Radio capabilties */
    lte_bs_phy_nmm_radio_register_radio();
    lte_bs_phy_nmm_radio_itf_query_available_band(pListening->supported_radio_band_list, &pListening->nb_supported_radio_band);

    /* switch off 1 antenna */

    /* reconfigure PHY into 2xSISO */

    /*init subframe count*/
    nmm_subframe_count = 0;
    pListening->last_buf_subframe_count = 0;

#ifdef SCHEDULER
    if (nmm_task_list[0] == NULL) {
        nmm_task_list[0] = CreateTaskList(NMM_TASKLIST0_IDX, NULL);
        _ASSERT_PTR(nmm_task_list[0]);
        RegListDoneCB(nmm_task_list[0], nmm_task_list_doneCB, (void*)(nmm_task_list_done+0));
    }

    /*TCBs for list 0*/
    nmm_tcb_arr[0][NMM_DOWNSAMPLING_TCB] = lte_bs_phy_nmm_downsampling_tcb(nmm_task_list[0], NULL, 0, NULL, 0,
                                                                         NULL,NULL,NULL,NULL,NULL,
                                                                         NULL,NULL,NULL,NULL,NULL,
                                                                         TCBIOBUFFILL, NULL);

    nmm_tcb_arr[0][NMM_AGC_TCB] =
        lte_bs_phy_nmm_agc_tcb(nmm_task_list[0], NULL, NULL, NULL, 0, nmm_tcb_arr[0], TCBIOBUFFILL, NULL);

    nmm_tcb_arr[0][NMM_PSS_XCORR0_TCB] = lte_bs_phy_nmm_xcorr_loop_tcb(nmm_task_list[0], NULL, NULL, NULL, NULL, nmm_tcb_arr[0], TCBIOBUFFILL, NULL);
    nmm_tcb_arr[0][NMM_PSS_XCORR1_TCB] = lte_bs_phy_nmm_xcorr_loop_tcb(nmm_task_list[0], NULL, NULL, NULL, NULL, nmm_tcb_arr[0], TCBIOBUFFILL, NULL);
    nmm_tcb_arr[0][NMM_PSS_XCORR2_TCB] = lte_bs_phy_nmm_xcorr_loop_tcb(nmm_task_list[0], NULL, NULL, NULL, NULL, nmm_tcb_arr[0], TCBIOBUFFILL, NULL);

    nmm_dbg_printf_off(("@ %s\tARM %d\tnmm_tcb_arr %x %x\ttime %lu\r\n", __FUNCTION__, MxGetCpuID(), nmm_tcb_arr[0][0], nmm_tcb_arr[0][1], (unsigned long) GET_TICKS));

    nmm_tcb_arr[0][NMM_FIND_CAND_TCB] = lte_bs_phy_nmm_pss_candidate_selection_loop_tcb(nmm_task_list[0], NULL, NULL, nmm_tcb_arr[0], TCBIOBUFFILL, NULL);


    if (nmm_task_list[1] == NULL) {
        nmm_task_list[1] = CreateTaskList(NMM_TASKLIST1_IDX, NULL);
        _ASSERT_PTR(nmm_task_list[1]);
        RegListDoneCB(nmm_task_list[1], nmm_task_list_doneCB, (void*)(nmm_task_list_done+1));
    }

    /*TCBs for list 1*/
    n_tcb = 0;
	for(i=0;i<4;i++)
	{
		nmm_tcb_arr[1][n_tcb++] = lte_bs_phy_nmm_fft64_tcb(nmm_task_list[1], NULL, NULL, NULL,NULL);
	}
	for(i=0;i<3;i++)
	{
		nmm_tcb_arr[1][n_tcb++] = lte_bs_phy_nmm_chanest_tcb(nmm_task_list[1], NULL, NULL, NULL, 0, SHORT_CP, nmm_tcb_arr[1], n_tcb-i-1,NULL);
	}
	for(i=0;i<3;i++)
	{
		nmm_tcb_arr[1][n_tcb++] = lte_bs_phy_nmm_sync_decode_sss_tcb(nmm_task_list[1], NULL, 0, NULL, NULL, nmm_tcb_arr[1], n_tcb-i-1,NULL);
	}


    if (nmm_task_list[2] == NULL) {
        nmm_task_list[2] = CreateTaskList(NMM_TASKLIST2_IDX, NULL);
        _ASSERT_PTR(nmm_task_list[2]);
        RegListDoneCB(nmm_task_list[2], nmm_task_list_doneCB, (void*)(nmm_task_list_done+2));
    }

    /*TCBs for list 2*/
    n_tcb = 0;
    for(n_cand = 0; n_cand < (MAX_NUM_CELLRS_SUBFRM); n_cand++) //!!!
    {
        nmm_tcb_arr[2][n_tcb++] = lte_bs_phy_nmm_get_rsrp_tcb(nmm_task_list[2], 0, NULL, 0, 0,
                                                        0, NULL, 0, 0, 0,NULL);
    }

    if (nmm_task_list[3] == NULL) {
        nmm_task_list[3] = CreateTaskList(NMM_TASKLIST3_IDX, NULL);
        _ASSERT_PTR(nmm_task_list[3]);
        RegListDoneCB(nmm_task_list[3], nmm_task_list_doneCB, (void*)(nmm_task_list_done+3));
    }

    /*TCBs for list 3*/
    n_tcb = 0;

    nmm_tcb_arr[3][n_tcb++] = lte_bs_phy_nmm_channel_estimate_tcb(nmm_task_list[3], NULL, 0,
        0, 0, 0, 0, NULL, 0,NULL,NULL,NULL);

    for(n_cand = 0; n_cand < NUM_SYMB_PBCH; n_cand++)
    {
        nmm_tcb_arr[3][n_tcb++] = lte_bs_phy_nmm_extract_pbch_tcb(nmm_task_list[3],
            0, NULL, 0, 0, NULL, NULL, NULL,0,0,NULL);
    }

    nmm_tcb_arr[3][n_tcb++] = lte_bs_phy_nmm_demod_pbch_tcb(nmm_task_list[3],NULL,0,NULL,NULL,NULL);

    nmm_tcb_arr[3][n_tcb++] = lte_bs_phy_nmm_pseudo_random_seq_gen_tcb(nmm_task_list[3],0,0,NULL,NULL);

	nmm_tcb_arr[3][n_tcb++] = lte_bs_phy_nmm_pss_foe_tcb(nmm_task_list[3], NULL, 0, NULL, NULL);

    for(n_cand = 0; n_cand < 14; n_cand++)
    {
        nmm_tcb_arr[3][n_tcb++] = lte_bs_phy_nmm_freqOffset_comp_tcb(nmm_task_list[3],
            NULL, 0, 0, 0, NULL);
    }

    N_PBCH_TCB = n_tcb;

    if (nmm_task_list[4] == NULL) {
        nmm_task_list[4] = CreateTaskList(NMM_TASKLIST4_IDX, NULL);
        _ASSERT_PTR(nmm_task_list[4]);
        RegListDoneCB(nmm_task_list[4], nmm_task_list_doneCB, (void*)(nmm_task_list_done+4));
    }

    /*TCBs for list 4*/
    n_tcb = 0;
    nmm_tcb_arr[4][NMM_EXTRACT_PCFI_TCB] = lte_bs_phy_nmm_extract_pcfi_tcb(nmm_task_list[4], 0, 0, NULL, NULL, NULL, NULL, NULL,
                                0,0,NULL);
    n_tcb ++;

    nmm_tcb_arr[4][NMM_EXTRACT_PDCCH_TCB] = lte_bs_phy_nmm_extract_pdcch_tcb(nmm_task_list[4], 0, 0, NULL, NULL, 0, NULL, NULL, NULL,
                                 0,0,NULL);
    n_tcb ++;

    nmm_tcb_arr[4][NMM_PDCCH_DEMOD_TCB] = lte_bs_phy_nmm_pdcch_demod_tcb(nmm_task_list[4], NULL, 0, 0, NULL, 0, 0, NULL,
                               NULL);
    n_tcb ++;

    for(n_cand = 0; n_cand < MAX_NUM_PDSCH_SYMB; n_cand++)
    {
        nmm_tcb_arr[4][NMM_EXTRACT_PDSCH_TCB+n_cand] = lte_bs_phy_nmm_extract_pdsch_tcb(nmm_task_list[4],
            0, 0, NULL, 0, NULL, NULL, NULL,
            0,NULL,0,0,
            0,NULL,0,NULL,0);
        n_tcb++;
    }

    N_PDSCH_tcb = n_tcb;

#endif

    pListening->dump_iq_data_mode = DUMP_IQ_DATA_MODE; //0: no dump; 1: dump for cell search; 2: dump for BCCH
    nmm_proc_subframe = 0;
    pListening->N_buf_subframe = 80;
    pListening->N_dump_subframe = 400;

    if(pListening->dump_iq_data_mode > 0)
    {
        pListening->dump_state = 0;
    }
  nmm_start_end:
#ifndef RT_API_PROC
      lte_bs_phy_nmm_generate_api_msg(phyInstance, NULL, 0, (U8 *) pListening->api_output_msg,
                NMM_START_RESP, status);
#endif
    if (status)
        memset(pListening, 0, sizeof(LISTENINGDESC));

    return status;
}


//--------------------------------------------------------------------------------------------------------------------------
/** @brief process the NMM STOP API message  
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param phyInstance  PHY instance ID
 *  @param ptr          pointer to API message 
 *  @param len          length of API message
 *  @return status(result) of API message processing 
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |phyInstance    |               |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |ptr            |               |input        |variable       |pReal Array   |0:8:0     |[0,2^8-1]      |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |len           |                |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              | 
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_stop      |
 *  ---------------------------------------
 *  |Input Stream(s)     |None |
 *  ---------------------------------------
 *  |Output Stream(s)    |None|
 *  ---------------------------------------
 *  |Config Parameter(s) |phyInstance,ptr,len |
 *  ---------------------------------------
 *  |Status Parameter(s) |None |
 *  ---------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * process the NMM STOP API message  
 * free memories for NMM;
 * reinitialize global variables;
 * destroy TCB lists;
 * generate response message;
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * 
 * 
 *
 * \ingroup group_lte_nmm_api
 *
 *
**/
//--------------------------------------------------------------------------------------------------------------------------
U32
lte_bs_phy_nmm_stop(U32 phyInstance, void *ptr, U32 len)
{
    U32             status = 0;
    PLTEPHYDESC     pPhy = getPhyCtxApiProc(phyInstance);
    PLISTENINGDESC  pListening = LteBsGetNmmCtx();


    if (!pListening)
        return 0;

#ifdef RT_API_PROC
    LtePhyApiThreadStopTimer();
    //lte_bs_phy_nmm_thread_kill();
#endif

#if defined(EVM) || defined(RT_API_PROC)
/* do pause fist to ignore irqs before stop  */
    if (PhyDiQueryMode() == AUTO_TIMER)
    {
        status = DiPause(NULL);
    }
    else if (PhyDiQueryMode() == AUTO_PCIE)
    {
        status = PhyDiSetPauseResume(1, getPhyCtxListIndx(0));
    }
    else
    {
        uart_printf("PHYDI: This mode doesn't support PAUSE/RESUME\r\n");
        status = NMM_LTE_DI_ERROR;
    }
    if(status)
    {
        status = NMM_LTE_DI_ERROR;


    }
	else
		status = LtePhyStop(NULL, NULL);
#endif

    uart_printf("%s: NMM_STOP\r\n", __FUNCTION__);

    lte_bs_phy_nmm_generate_api_msg(phyInstance, NULL, 0, (U8 *) pListening->api_output_msg,
        NMM_STOP_RESP, status);

    // clean up for next start
    lte_bs_phy_nmm_destroy_buffers();

    MLogEnableRC (0);

    return status;
}
//--------------------------------------------------------------------------------------------------------------------------
/** @brief destroy the memory allocated by NMM 
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param  None
 *  @return status(result) of the processing
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_destroy_buffers      |
 *  ---------------------------------------
 *  |Input Stream(s)     |None |
 *  ---------------------------------------
 *  |Output Stream(s)    |None|
 *  ---------------------------------------
 *  |Config Parameter(s) |None |
 *  ---------------------------------------
 *  |Status Parameter(s) |None |
 *  ---------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * destroy the memory allocated by NMM 
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * 
 * 
 *
 * \ingroup group_lte_nmm_api
 *
 *
**/
//--------------------------------------------------------------------------------------------------------------------------

U32
lte_bs_phy_nmm_destroy_buffers(void)
{
#ifdef SCHEDULER
    UINT32 i;

    for (i = 0; i < NMM_NUM_LIST; i++)
    {
        if (nmm_task_list[i])
        {
            DestroyTaskList(nmm_task_list[i]);
            nmm_task_list[i] = NULL;
        }
    }
#endif

    if (nmm_iq_storage)
    {
        MemFree(nmm_iq_storage, DDR1_NCNB_MEM);
        nmm_iq_storage= NULL;
    }

    if (pGlobalListening)
    {
        lte_bs_phy_nmm_cell_search_clear();
        MemFree(pGlobalListening, DDR_MEM);
        pGlobalListening = NULL;
    }

    listeningActiveFlag = 0;

    return 0;
}


//--------------------------------------------------------------------------------------------------------------------------
/** @brief process the RSSI MEAS REQ API message 
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param phyInstance  PHY instance ID
 *  @param ptr          pointer to API message 
 *  @param len          length of API message
 *  @return status(result) of API message processing 
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |phyInstance    |               |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |ptr            |               |input        |variable       |pReal Array   |0:8:0     |[0,2^8-1]      |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |len           |                |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              | 
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_rssi_meas_req      |
 *  ---------------------------------------
 *  |Input Stream(s)     |None |
 *  ---------------------------------------
 *  |Output Stream(s)    |None|
 *  ---------------------------------------
 *  |Config Parameter(s) |phyInstance,ptr,len |
 *  ---------------------------------------
 *  |Status Parameter(s) |None |
 *  ---------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * process the RSSI MEAS REQ API message 
 * initialize RSSI measurement state
 * call driver function to switch radio frequency;
 * start RSSI measurement
 * generate response message;
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * 
 * 
 *
 * \ingroup group_lte_nmm_api
 *
 *
**/
//--------------------------------------------------------------------------------------------------------------------------
U32
lte_bs_phy_nmm_rssi_meas_req(U32 phyInstance, void *ptr, U32 len)
{
    U32             status = 0;
    PLTEPHYDESC     pPhy = getPhyCtxApiProc(phyInstance);
    PLISTENINGDESC  pListening = LteBsGetNmmCtx();
    U8             *p8 = (U8 *) ptr;
    U8              freq_band_indicator;
    U16            *p16;
    int             i,
                    nr_earfcnList;
    EARFNC_item    *pEARFNC_item;

    nmm_dbg_printf_off(("@@@ [%s]\r\n", __FUNCTION__));

    if (!pListening && !pListening->listening_active) {
        status = NMM_NOT_ENABLED;
        goto nmm_rssi_meas_req_end;
    }
    freq_band_indicator = p8[0];

    if (!IS_FREQ_BAND_AVAILABLE(freq_band_indicator)) {
        status = NMM_INVALID_FREQ_BAND;
        goto nmm_rssi_meas_req_end;
    }

    if (!is_freqband_supported_by_hw(pListening, freq_band_indicator)) {
        status = NMM_FREQ_BAND_NOT_SUPPORTED_BY_HW;
        goto nmm_rssi_meas_req_end;
    }
    pListening->freq_band_indicator = freq_band_indicator;

    /* measure in 10ms frames */
    pListening->rssi_measurement_period_ms = p8[1] * 10;

    nr_earfcnList = p8[2];
    if (nr_earfcnList > NMM_LTE_MAX_CARRIER_LIST) {
        status = NMM_INVALID_NR_CARRIER_LIST;
        goto nmm_rssi_meas_req_end;
    }
    pListening->nr_earfcnList = nr_earfcnList;

    /* specific carrier search */
    if (nr_earfcnList) {
        memcpy(pListening->rssi_earfcn_list, &p8[3], nr_earfcnList * sizeof(U16));

        i = nr_earfcnList;
        pEARFNC_item = &EARFNC_TABLE[freq_band_indicator];
        p16 = pListening->rssi_earfcn_list;
        do {
            U16             earfcn = *p16++;
            nmm_dbg_printf_off(("@@@ earfcn %d [%d %d] \n", earfcn, pEARFNC_item->range_min, pEARFNC_item->range_max));
            if ((earfcn < pEARFNC_item->range_min) || (earfcn > pEARFNC_item->range_max)) {
                status = NMM_INVALID_CARRIER_LIST;
                goto nmm_rssi_meas_req_end;
            }
        } while (--i);
    }
    nmm_dbg_printf_off(("@@@ freq_band_indicator %d  rssi_measurement_period_ms %d nr_earfcnList %d\n", pListening->freq_band_indicator, pListening->rssi_measurement_period_ms,
                        pListening->nr_earfcnList));

    pListening->cur_earfcn = 0;
    pListening->cur_rssi_measurement_time = 0;
    pListening->averaged_rssi = 0;
    pListening->rssi_meas_avg_factor = divide_s(1, (S16) pListening->rssi_measurement_period_ms);
    nmm_dbg_printf_off(("rssi_meas_avg_factor %d\n", pListening->rssi_meas_avg_factor));

    /* at this point all API params have been validated */
	if ((pListening->state != NMM_IDLE) && (pListening->state != NMM_TRACKING))
	{
        status = NMM_UNEXPECTED_CMD;
        goto nmm_rssi_meas_req_end;
	}

	lte_bs_phy_nmm_cell_search_clear();	/* need to clear mallocated memory*/
	pListening->target_state_mask = 0;
    pListening->state = NMM_RSSI_MEASURING;

    if (nr_earfcnList) {
        SWITCH_RFIC_DL_FREQ(BAND_EARFCN_TO_KHZ(freq_band_indicator, pListening->rssi_earfcn_list[0]));
    }
    else {
        SWITCH_RFIC_DL_FREQ(EARFCNIDX_TO_KHZ(freq_band_indicator, 0));
    }

  nmm_rssi_meas_req_end:
    if (status) {
        /* wrong API, discard all RSSI related parameters */
        pListening->freq_band_indicator = 0;
        pListening->rssi_measurement_period_ms = 0;
        pListening->nr_earfcnList = 0;
        memset(pListening->rssi_earfcn_list, 0, NMM_LTE_MAX_CARRIER_LIST * sizeof(U16));
    }

    lte_bs_phy_nmm_generate_api_msg(phyInstance, NULL, 0, (U8 *) pListening->api_output_msg,
        NMM_RSSI_MEAS_REQ_RESP, status);
    return status;
}


//--------------------------------------------------------------------------------------------------------------------------
/** @brief process the STOP RSSI MEAS REQ API message  
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param phyInstance  PHY instance ID
 *  @param ptr          pointer to API message 
 *  @param len          length of API message
 *  @return status(result) of API message processing 
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |phyInstance    |               |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |ptr            |               |input        |variable       |pReal Array   |0:8:0     |[0,2^8-1]      |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |len           |                |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              | 
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_stop_rssi_meas_req      |
 *  ---------------------------------------
 *  |Input Stream(s)     |None |
 *  ---------------------------------------
 *  |Output Stream(s)    |None|
 *  ---------------------------------------
 *  |Config Parameter(s) |phyInstance,ptr,len |
 *  ---------------------------------------
 *  |Status Parameter(s) |None |
 *  ---------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * process the STOP RSSI MEAS REQ API message  
 * stop RSSI measurement state
 * generate response message;
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * 
 * 
 *
 * \ingroup group_lte_nmm_api
 *
 *
**/
//--------------------------------------------------------------------------------------------------------------------------
U32
lte_bs_phy_nmm_stop_rssi_meas_req(U32 phyInstance, void *ptr, U32 len)
{
    U32             status = 0;
    PLTEPHYDESC     pPhy = getPhyCtxApiProc(phyInstance);
    PLISTENINGDESC  pListening = LteBsGetNmmCtx();

    nmm_dbg_printf_off(("@@@ [%s]\n", __FUNCTION__));

    if (!pListening && !pListening->listening_active)
    {
        status = NMM_NOT_ENABLED;
        goto nmmStopRssiMeasResp;
    }

    if (pListening->state != NMM_RSSI_MEASURING)
    {
        status = NMM_UNEXPECTED_CMD;
        goto nmmStopRssiMeasResp;
    }

    pListening->state = NMM_IDLE;
    pListening->freq_band_indicator = 0;
    pListening->rssi_measurement_period_ms = 0;
    pListening->nr_earfcnList = 0;
    memset(pListening->rssi_earfcn_list, 0, NMM_LTE_MAX_CARRIER_LIST * sizeof(U16));

nmmStopRssiMeasResp:
    lte_bs_phy_nmm_generate_api_msg(phyInstance, NULL, 0, (U8 *) pListening->api_output_msg,
        NMM_STOP_RSSI_MEAS_REQ_RESP, status);

    return status;
}


//--------------------------------------------------------------------------------------------------------------------------
/** @brief process the CELL SEARCH REQ API message  
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param phyInstance  PHY instance ID
 *  @param ptr          pointer to API message 
 *  @param len          length of API message
 *  @return status(result) of API message processing 
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |phyInstance    |               |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |ptr            |               |input        |variable       |pReal Array   |0:8:0     |[0,2^8-1]      |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |len           |                |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              | 
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_cell_search_req      |
 *  ---------------------------------------
 *  |Input Stream(s)     |None |
 *  ---------------------------------------
 *  |Output Stream(s)    |None|
 *  ---------------------------------------
 *  |Config Parameter(s) |phyInstance,ptr,len |
 *  ---------------------------------------
 *  |Status Parameter(s) |None |
 *  ---------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * process the CELL SEARCH REQ API message  
 * initialize cell search state
 * call driver function to switch radio frequency;
 * start cell search
 * generate response message;
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * 
 * 
 *
 * \ingroup group_lte_nmm_api
 *
 *
**/
//--------------------------------------------------------------------------------------------------------------------------
U32
lte_bs_phy_nmm_cell_search_req(U32 phyInstance, void *ptr, U32 len)
{
    U32             status = 0;
    PLTEPHYDESC     pPhy = getPhyCtxApiProc(phyInstance);
    PLISTENINGDESC  pListening = LteBsGetNmmCtx();
    U32             req_freq;
    U16             earfcn;
    U8              req_band,
                    cell_search_nr_pci_list;
    int             i,
                    j;
    U8             *p8 = (U8 *) ptr;
    U16            *p16,
                   *q16;
    U8              cell_search_meas_bandwidth;

    nmm_dbg_printf(("[%s]\n", __FUNCTION__));

    if (!pListening && !pListening->listening_active) {
        status = NMM_NOT_ENABLED;
        goto PhyCellSearchReqEnd;
    }
    earfcn = *((U16 *) p8);

    earfcn_to_khz(earfcn, &req_freq, &req_band);
    if (req_band > MAX_EUTRA_BAND) {
        status = NMM_INVALID_EARFCN;
        goto PhyCellSearchReqEnd;
    }
    if (!is_freqband_supported_by_hw(pListening, req_band)) {
        status = NMM_FREQ_BAND_NOT_SUPPORTED_BY_HW;
        goto PhyCellSearchReqEnd;
    }
    pListening->cell_search_earfcn = earfcn;
    pListening->cell_search_exhaustive_search = p8[2];

    cell_search_nr_pci_list = p8[3];
    if (cell_search_nr_pci_list > MAX_PCI_LIST) {
        status = NMM_UNEXPECTED_CMD;
        goto PhyCellSearchReqEnd;
    }

    p16 = (U16 *) & p8[4];
    pListening->cell_search_nr_pci_list = cell_search_nr_pci_list;
    if (cell_search_nr_pci_list > 0) {
        /*
         * specific PCI search
         */
        q16 = pListening->cell_search_pci_list;
        for (i = 0; i < cell_search_nr_pci_list; i++) {
            U16             cur_pci = *p16++;
            if (cur_pci > MAX_LTE_CELL_PCI) {
                status = NMM_UNEXPECTED_CMD;
                goto PhyCellSearchReqEnd;
            }
            *q16++ = cur_pci;
        }
    }
    else {
        /*
         * exhaustive PCI search
         */
        if (pListening->cell_search_exhaustive_search == FALSE) {
            status = NMM_UNEXPECTED_CMD;
            goto PhyCellSearchReqEnd;
        }
    }

    p8 = (U8 *) p16;

    cell_search_meas_bandwidth = *p8++;
    if (!is_bw_ok(cell_search_meas_bandwidth)) {
        status = NMM_UNEXPECTED_CMD;
        goto PhyCellSearchReqEnd;
    }
    pListening->cell_search_meas_bandwidth = cell_search_meas_bandwidth;
    pListening->cell_search_meas_bandwidth_inv = get_bw_inv(cell_search_meas_bandwidth);

    pListening->cell_search_rssi_measurement_period = *p8++;
    if (pListening->cell_search_rssi_measurement_period == 0) {
        status = NMM_UNEXPECTED_CMD;
        goto PhyCellSearchReqEnd;
    }

    pListening->rsrp_scale = divide_s(1, (S16) (pListening->cell_search_rssi_measurement_period * PILOTS_SYM_PER_FRAME));

    /*
     * at this point all API params have been validated
     */
	switch(pListening->state)
	{
		case NMM_TRACKING:
			lte_bs_phy_nmm_cell_search_clear();	/* need to clear mallocated memory*/
			pListening->target_state_mask = 0;
			pListening->state = NMM_SYNC_SEARCHING;
			break;

		case NMM_IDLE:
			pListening->target_state_mask = 0;
			pListening->state = NMM_SYNC_SEARCHING;
			break;

		case NMM_PBCH_SEARCHING:
			pListening->target_state_mask |= (1 << pListening->state);  /* we may come from higher state request, e.g. NMM_PBCH_SEARCHING */
			pListening->state = NMM_SYNC_SEARCHING;
			break;

		default:
			status = NMM_UNEXPECTED_CMD;
			goto PhyCellSearchReqEnd;
    }

    SWITCH_RFIC_DL_FREQ(req_freq);

    pListening->pcell_search = (LISTENING_CELL_SEARCH_DESC *) MemAlloc(sizeof(LISTENING_CELL_SEARCH_DESC), CRAM_MEM, 1);
    if (!pListening->pcell_search) {
        status = NMM_LTE_OUT_OF_MEMORY;
        goto PhyCellSearchReqEnd;
    }

	pListening->pcell_search->nb_detected_cell = 0;
    pListening->pcell_search->nb_detected_requested_cell = 0;

    /*
     * xcorr alloc
     */
    pListening->pcell_search->xc25 = MemAlloc(sizeof(S16), CRAM_MEM, 1);
    pListening->pcell_search->xc29 = MemAlloc(sizeof(S16), CRAM_MEM, 1);
    pListening->pcell_search->xc34 = MemAlloc(sizeof(S16), CRAM_MEM, 1);
    pListening->pcell_search->cell_search_xcorr_buf = MemAlloc(2 * (2 * FFT_SIZE_096MHZ + SSS_SEARCH_RANGE) * sizeof(S16), CRAM_MEM, 1);

    for (j = 0; j < MAX_N_SIB_TYPES; j++)
        pListening->pcell_search->cell_param[0].si_periodicity[j] = si_unkown;

    /*
     * Decimator init
     */
    lte_bs_phy_nmm_downsampling_init((void *) pListening->pcell_search);


    {
        // int i;
        // S16 *pin;
        LISTENING_CELL_SEARCH_DESC *pcell_search = pListening->pcell_search;
        // S16 *pci,
        // *subframe;
        // S16 nmm_cur_tcb_cnt = 0;
        S16            *xcorr_arr;
        S16            *psync_found_arr;


        xcorr_arr = (S16 *) MemAlloc(3 * SUBFR_SIZE_096MHZ * 5 * sizeof(S16), CRAM_MEM, 1);		// each arr have half frame length
        psync_found_arr = (S16 *) MemAlloc(SUBFR_SIZE_096MHZ * sizeof(S16), CRAM_MEM, 1);

        pcell_search->xcorr_arr = xcorr_arr;
        pcell_search->psync_found_arr = psync_found_arr;
        pcell_search->xcorr_idx = 0;
    //uart_printf("[%s] free dspheap space: %d\n", __FUNCTION__, MxHeapGetFreeSpace(DspHeap));


        // First task is to decimate. IQDecimated contains the decimated samples
        // IQdecimated = (S16 *) MemAlloc(2 * SUBFR_SIZE_096MHZ * sizeof(S16), CRAM_MEM, 1);

        /*
         * Sync AGC init
         */
		pListening->pcell_search->agc_gain = MemAlloc(sizeof(S32) * 2, CRAM_MEM, 1);
		pListening->pcell_search->agc_gain[1] = 255; //used for pss correlation accumulator
		pListening->pcell_search->agc_avg_nrg = MemAlloc(sizeof(S32), CRAM_MEM, 1);

        pListening->pcell_search->earfcn = earfcn;
        pListening->cell_search_time = NMM_CELL_SEARCH_MAX_TIME_MS;


        // pG = (S16 *) MemAlloc(SUBFR_SIZE_096MHZ * sizeof(S16), CRAM_MEM, 1);
        // pAGC_Eav = (S16 *) MemAlloc(SUBFR_SIZE_096MHZ * sizeof(S16), CRAM_MEM, 1);

        /*
         * at each step the AGC is synchronized
         */
    }

  PhyCellSearchReqEnd:

    nmm_dbg_printf(("@@@ [%s] earfcn %d exhaustive_search %d nr_pci_list %d meas_bandwidth %d rssi_measurement_period %d status%d\n", __FUNCTION__,
                        pListening->cell_search_earfcn, pListening->cell_search_exhaustive_search, pListening->cell_search_nr_pci_list, pListening->cell_search_meas_bandwidth,
                        pListening->cell_search_rssi_measurement_period, status));
    //uart_printf("[%s] free dspheap space: %d\n", __FUNCTION__, MxHeapGetFreeSpace(DspHeap));

    if(pListening->target_state_mask == 0)
    {
        lte_bs_phy_nmm_generate_api_msg(phyInstance, NULL, 0, (U8 *) pListening->api_output_msg,
            NMM_CELL_SEARCH_REQ_RESP, status);
    }

    return status;

}

//--------------------------------------------------------------------------------------------------------------------------
/** @brief free memory allocated for cell search 
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param  None
 *  @return None
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
  *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_cell_search_clear      |
 *  ---------------------------------------
 *  |Input Stream(s)     |None |
 *  ---------------------------------------
 *  |Output Stream(s)    |None|
 *  ---------------------------------------
 *  |Config Parameter(s) |None |
 *  ---------------------------------------
 *  |Status Parameter(s) |None |
 *  ---------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * free memory allocated for cell search
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * 
 * 
 *
 * \ingroup group_lte_nmm_api
 *
 *
**/
//--------------------------------------------------------------------------------------------------------------------------

void
lte_bs_phy_nmm_cell_search_clear(void)
{
    PLISTENINGDESC pListening = LteBsGetNmmCtx();

    if (pListening->pcell_search) {
        int             i;

        for (i = 0; i < pListening->pcell_search->nb_detected_cell; i++) {
			nmm_dbg_printf_off(("[%s] %d\n", __FUNCTION__, i));

			if (pListening->pcell_search->cell_param[i].pbch_scrambling_seq != NULL)
			{
				MemFree(pListening->pcell_search->cell_param[i].pbch_scrambling_seq, CRAM_MEM);
				MemFree(pListening->pcell_search->cell_param[i].pbch_buf, CRAM_MEM);
				MemFree(pListening->pcell_search->cell_param[i].pbch_scale, CRAM_MEM);
				MemFree(pListening->pcell_search->cell_param[i].pdcch_scrambling_seq, CRAM_MEM);
			}
        }
        MemFree(pListening->pcell_search->agc_gain, CRAM_MEM);
        MemFree(pListening->pcell_search->agc_avg_nrg, CRAM_MEM);
        MemFree(pListening->pcell_search->xc25, CRAM_MEM);
        MemFree(pListening->pcell_search->xc29, CRAM_MEM);
        MemFree(pListening->pcell_search->xc34, CRAM_MEM);
        MemFree(pListening->pcell_search->cell_search_xcorr_buf, CRAM_MEM);
        MemFree(pListening->pcell_search->hbfir_state_buf, CRAM_MEM);
        MemFree(pListening->pcell_search->psync_found_arr,CRAM_MEM);
        MemFree(pListening->pcell_search->xcorr_arr,CRAM_MEM);
        MemFree(pListening->pcell_search, CRAM_MEM);

		pListening->pcell_search = NULL;
    }

}

//--------------------------------------------------------------------------------------------------------------------------
/** @brief process the STOP CELL SEARCH API message 
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param phyInstance  PHY instance ID
 *  @param ptr          pointer to API message 
 *  @param len          length of API message
 *  @return status(result) of API message processing 
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |phyInstance    |               |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |ptr            |               |input        |variable       |pReal Array   |0:8:0     |[0,2^8-1]      |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |len           |                |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              | 
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_stop_cell_search_req      |
 *  ---------------------------------------
 *  |Input Stream(s)     |None |
 *  ---------------------------------------
 *  |Output Stream(s)    |None|
 *  ---------------------------------------
 *  |Config Parameter(s) |phyInstance,ptr,len |
 *  ---------------------------------------
 *  |Status Parameter(s) |None |
 *  ---------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * process the STOP CELL SEARCH API message 
 * stop cell search state
 * generate response message;
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * 
 * 
 *
 * \ingroup group_lte_nmm_api
 *
 *
**/
//--------------------------------------------------------------------------------------------------------------------------
U32
lte_bs_phy_nmm_stop_cell_search_req(U32 phyInstance, void *ptr, U32 len)
{
    U32             status = 0;
    PLTEPHYDESC     pPhy = getPhyCtxApiProc(phyInstance);
    PLISTENINGDESC pListening = LteBsGetNmmCtx();

    if (!pListening && !pListening->listening_active)
	{
        status= NMM_NOT_ENABLED;
        goto stop_cell_search_reqEnd;
	}

    if (pListening->state != NMM_SYNC_SEARCHING)
	{
        status = NMM_UNEXPECTED_CMD;
        goto stop_cell_search_reqEnd;
	}

	lte_bs_phy_nmm_cell_search_clear();

    pListening->cell_search_earfcn = 0;
    pListening->cell_search_exhaustive_search = 0;
    pListening->cell_search_nr_pci_list = 0;
    pListening->cell_search_meas_bandwidth = 0;
    pListening->cell_search_rssi_measurement_period = 0;
    pListening->state = NMM_IDLE;

    stop_cell_search_reqEnd:
        lte_bs_phy_nmm_generate_api_msg(phyInstance, NULL, 0, (U8 *) pListening->api_output_msg,
                                                    NMM_STOP_CELL_SEARCH_REQ_RESP, 0);

    return status;
}


//--------------------------------------------------------------------------------------------------------------------------
/** @brief process the PBCH CONFIG REQ API message 
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param phyInstance  PHY instance ID
 *  @param ptr          pointer to API message 
 *  @param len          length of API message
 *  @return status(result) of API message processing 
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |phyInstance    |               |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |ptr            |               |input        |variable       |pReal Array   |0:8:0     |[0,2^8-1]      |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |len           |                |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              | 
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_pbch_config_req      |
 *  ---------------------------------------
 *  |Input Stream(s)     |None |
 *  ---------------------------------------
 *  |Output Stream(s)    |None|
 *  ---------------------------------------
 *  |Config Parameter(s) |phyInstance,ptr,len |
 *  ---------------------------------------
 *  |Status Parameter(s) |None |
 *  ---------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * process the PBCH CONFIG REQ API message
 * initialize PBCH searching state
 * start cell search for PBCH searching 
 * generate response message
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * 
 * 
 *
 * \ingroup group_lte_nmm_api
 *
 *
**/
//--------------------------------------------------------------------------------------------------------------------------
U32
lte_bs_phy_nmm_pbch_config_req(U32 phyInstance, void *ptr, U32 len)
{
    U32             status = 0;
    PLTEPHYDESC     pPhy = getPhyCtxApiProc(phyInstance);
    PLISTENINGDESC  pListening = LteBsGetNmmCtx();
    U32             req_freq;
    U16             earfcn,
                    pci;
    U8              req_band;
    U16            *p16 = (U16 *) ptr;
	U8				APImsg[32];

    nmm_dbg_printf(("[%s]\n", __FUNCTION__));

    if (!pListening && !pListening->listening_active) {
        status = NMM_NOT_ENABLED;
        goto pbch_config_req_end;
    }


    pci = *p16++;
    if (pci > MAX_LTE_CELL_PCI) {
        status = NMM_UNEXPECTED_CMD;
        goto pbch_config_req_end;
    }

    pListening->pbch_pci = pci;

    earfcn = *p16++;
    earfcn_to_khz(earfcn, &req_freq, &req_band);
    if (req_band > MAX_EUTRA_BAND) {
        status = NMM_INVALID_EARFCN;
        goto pbch_config_req_end;
    }


    if (!is_freqband_supported_by_hw(pListening, req_band)) {
        status = NMM_FREQ_BAND_NOT_SUPPORTED_BY_HW;
        goto pbch_config_req_end;
    }

    /* at this point all API params have been validated */
    if (pListening->state == NMM_IDLE)
        pListening->state = NMM_PBCH_SEARCHING;
	else if (pListening->state == NMM_TRACKING)
	{
		lte_bs_phy_nmm_cell_search_clear();	/* need to clear mallocated memory*/
		pListening->target_state_mask = 0;
        pListening->state = NMM_PBCH_SEARCHING;
	}
    else
	{
		status = NMM_UNEXPECTED_CMD;
        goto pbch_config_req_end;
	}

    pListening->pbch_earfcn = earfcn;

    /*
     * at this time we are expecting to use same carrier that has been detected during sync process and not touch radio
     * (if not we need to start sync process !!
     */
    SWITCH_RFIC_DL_FREQ(req_freq);

    if (!pListening->pcell_search || (pListening->pcell_search && (pListening->state == NMM_IDLE) && pListening->pcell_search->nb_detected_cell == 0)) {
        U16             nmm_cell_search_vector[20];
        U16            *p16;
        U8             *p8;
        p16 = (U16 *) nmm_cell_search_vector;
        *p16++ = earfcn;        /* earfcn */
        p8 = (U8 *) p16;
        *p8++ = FALSE;          /* exhaus */
        *p8++ = 1;              /* 1 cell */
        p16 = (U16 *) p8;
        *p16++ = pci;
        p8 = (U8 *) p16;
        *p8++ = RB_SLOT_6;      /* bandwidth - RSRP only, will not be used */
        *p8++ = 1;              /* measperiod - RSRP only, will not be used */
        status = lte_bs_phy_nmm_cell_search_req(phyInstance, (void *) nmm_cell_search_vector, 0);       /* len not used */
    }

  pbch_config_req_end:
	APImsg[0] = 0;	// nr_vendorSpecificList
    lte_bs_phy_nmm_generate_api_msg(phyInstance, APImsg, 1, (U8 *) pListening->api_output_msg,
        NMM_PBCH_CONFIG_CNF, status);

    return status;
}


//--------------------------------------------------------------------------------------------------------------------------
/** @brief process the STOP PBCH CONFIG REQ API message 
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param phyInstance  PHY instance ID
 *  @param ptr          pointer to API message 
 *  @param len          length of API message
 *  @return status(result) of API message processing 
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |phyInstance    |               |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |ptr            |               |input        |variable       |pReal Array   |0:8:0     |[0,2^8-1]      |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |len           |                |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              | 
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_stop_pbch_req      |
 *  ---------------------------------------
 *  |Input Stream(s)     |None |
 *  ---------------------------------------
 *  |Output Stream(s)    |None|
 *  ---------------------------------------
 *  |Config Parameter(s) |phyInstance,ptr,len |
 *  ---------------------------------------
 *  |Status Parameter(s) |None |
 *  ---------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * process the STOP PBCH CONFIG REQ API message
 * update NMM state
 * generate response message
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * 
 * 
 *
 * \ingroup group_lte_nmm_api
 *
 *
**/
//--------------------------------------------------------------------------------------------------------------------------
U32
lte_bs_phy_nmm_stop_pbch_req(U32 phyInstance, void *ptr, U32 len)
{
    U32             status = 0;
    PLTEPHYDESC     pPhy = getPhyCtxApiProc(phyInstance);
    PLISTENINGDESC  pListening = LteBsGetNmmCtx();

	if (pListening->state == NMM_PBCH_SEARCHING_DONE)
		pListening->state = NMM_TRACKING;
	else
	{
		lte_bs_phy_nmm_cell_search_clear();
		pListening->target_state_mask = 0;
        pListening->state = NMM_IDLE;
	}

    lte_bs_phy_nmm_generate_api_msg(phyInstance, NULL, 0, (U8 *) pListening->api_output_msg,
        NMM_STOP_PBCH_REQ_RESP, status);

    return status;
}


//--------------------------------------------------------------------------------------------------------------------------
/** @brief process the SIB1 CONFIG REQ API message 
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param phyInstance  PHY instance ID
 *  @param ptr          pointer to API message 
 *  @param len          length of API message
 *  @return status(result) of API message processing 
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |phyInstance    |               |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |ptr            |               |input        |variable       |pReal Array   |0:8:0     |[0,2^8-1]      |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |len           |                |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              | 
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_sib1_config_req      |
 *  ---------------------------------------
 *  |Input Stream(s)     |None |
 *  ---------------------------------------
 *  |Output Stream(s)    |None|
 *  ---------------------------------------
 *  |Config Parameter(s) |phyInstance,ptr,len |
 *  ---------------------------------------
 *  |Status Parameter(s) |None |
 *  ---------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * process the SIB1 CONFIG REQ API message
 * initialize SIB1 searching state
 * start SIB1 searching processing
 * generate response message
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * 
 * 
 *
 * \ingroup group_lte_nmm_api
 *
 *
**/
//--------------------------------------------------------------------------------------------------------------------------
U32
lte_bs_phy_nmm_sib1_config_req(U32 phyInstance, void *ptr, U32 len)
{
    U32             status = 0;
    PLTEPHYDESC     pPhy = getPhyCtxApiProc(phyInstance);
    PLISTENINGDESC  pListening = LteBsGetNmmCtx();
	U8				APImsg[32];

    U8             *p8 = (U8 *) ptr;
    U8              sib1_retry_count = *p8;

    nmm_dbg_printf(("[%s]\n", __FUNCTION__));

    if (!pListening && !pListening->listening_active) {
        status = NMM_NOT_ENABLED;
        goto sib1_config_req_end;
    }

    if (sib1_retry_count > MAX_SI_DECODE_RETRY_COUNT || sib1_retry_count < MIN_SI_DECODE_RETRY_COUNT) {
        status = NMM_UNEXPECTED_CMD;
        goto sib1_config_req_end;
    }

    if (pListening->pbch_earfcn == 0xffff) {
        /* no NMM_PBCH_CONFIG_REQ API command has been received yet => exit */
        status = NMM_UNEXPECTED_CMD;
        goto sib1_config_req_end;
    }

    if (pListening->state == NMM_TRACKING)
    {
        pListening->state = NMM_SIB1_SEARCHING;
        pListening->pcell_search->cell_param[pListening->pbch_search_cell_index].BCCH_type = 0;
    }
    else{
        status = NMM_UNEXPECTED_CMD;
        goto sib1_config_req_end;
    }

    pListening->sib1_retry_count = sib1_retry_count;    /* OS: not used? */
	pListening->sib_report_cnt = 0;
    pListening->sib_report_flag = 1;
	pListening->sib_end_time[0] = sib1_retry_count * 4; /* sib1 may occur 4 times per 80ms */
	pListening->nr_si_periodicity_list = 1; //used for sib period decision loop
	
  sib1_config_req_end:
	APImsg[0] = 0;	// nr_vendorSpecificList

	lte_bs_phy_nmm_generate_api_msg(phyInstance, APImsg, 1, (U8 *) pListening->api_output_msg, NMM_SIB1_CONFIG_CNF, status);

    return status;
}


//--------------------------------------------------------------------------------------------------------------------------
/** @brief process the BCCH CONFIG REQ API message 
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param phyInstance  PHY instance ID
 *  @param ptr          pointer to API message 
 *  @param len          length of API message
 *  @return status(result) of API message processing 
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |phyInstance    |               |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |ptr            |               |input        |variable       |pReal Array   |0:8:0     |[0,2^8-1]      |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |len           |                |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              | 
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_bcch_config_req      |
 *  ---------------------------------------
 *  |Input Stream(s)     |None |
 *  ---------------------------------------
 *  |Output Stream(s)    |None|
 *  ---------------------------------------
 *  |Config Parameter(s) |phyInstance,ptr,len |
 *  ---------------------------------------
 *  |Status Parameter(s) |None |
 *  ---------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * process the BCCH CONFIG REQ API message
 * initialize BCCH searching state
 * start BCCH searching processing
 * generate response message
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * 
 * 
 *
 * \ingroup group_lte_nmm_api
 *
 *
**/
//--------------------------------------------------------------------------------------------------------------------------
U32
lte_bs_phy_nmm_bcch_config_req(U32 phyInstance, void *ptr, U32 len)
{
    U32             status = 0;
    PLTEPHYDESC     pPhy = getPhyCtxApiProc(phyInstance);
    PLISTENINGDESC  pListening = LteBsGetNmmCtx();
    U8             *p8 = (U8 *) ptr;
    U8              nr_si_periodicity_list = *p8;
    U8              si_window,
                    si_retry_count;
    U16             siPeriodicityList[MAX_SI_LIST];
    U16            *p16 = siPeriodicityList;
    U16            *q16 = pListening->si_periodicity_list;
    int             i;
	U8				APImsg[32];
	U16				maxsiPeriodicity;

    nmm_dbg_printf_off(("@@@ [%s]\n", __FUNCTION__));

    if (!pListening && !pListening->listening_active) {
        status = NMM_NOT_ENABLED;
        goto bcch_config_req_end;
    }

    if (nr_si_periodicity_list > MAX_SI_LIST) {
        status = NMM_UNEXPECTED_CMD;
        goto bcch_config_req_end;
    }

    if ((pListening->state == NMM_TRACKING) || (pListening->state == NMM_SIB1_SEARCHING))
        pListening->state = NMM_FULL_BCCH_SEARCHING;
    else{
        status = NMM_UNEXPECTED_CMD;
        goto bcch_config_req_end;
    }

	maxsiPeriodicity = 0;
    pListening->nr_si_periodicity_list = nr_si_periodicity_list;
    if (nr_si_periodicity_list > 0) {
        /* femtoapi unfriendly alignment */
        memcpy(siPeriodicityList, &p8[1], nr_si_periodicity_list * sizeof(U16));
        for (i = 0; i < nr_si_periodicity_list; i++) {
            U16             period = *p16++;
            if (!is_si_periodicity_ok(period)) {
                status = NMM_UNEXPECTED_CMD;
                goto bcch_config_req_end;
            }
            *q16++ = period;
            //DCI_1C_DEBUG
            pListening->si_index_list[i] = i;

			if (period > maxsiPeriodicity)
				maxsiPeriodicity = period;

        }
    }

    /* skip si periodicty + list array */
    p8 = p8 + 1 + (nr_si_periodicity_list * sizeof(U16));
    si_window = *p8++;
    if (!is_si_win_ok(si_window)) {
        status = NMM_UNEXPECTED_CMD;
        goto bcch_config_req_end;
    }
    pListening->si_window = si_window;

    si_retry_count = *p8++;
    if (si_retry_count > MAX_SI_DECODE_RETRY_COUNT || si_retry_count < MIN_SI_DECODE_RETRY_COUNT) {
        status = NMM_UNEXPECTED_CMD;
        goto bcch_config_req_end;
    }
    pListening->si_retry_count = si_retry_count;

    pListening->sib_report_flag = (1 << nr_si_periodicity_list) - 1; // bit0 indicate SIB2, bit1 SIB3, and so on
	for (i = 0; i < nr_si_periodicity_list; i++) {
		pListening->sib_end_time[i] = si_retry_count * si_window;
	}
	pListening->sib_report_cnt = 0;

    pListening->pcell_search->cell_param[pListening->pbch_search_cell_index].BCCH_type = 1;
    pListening->state = NMM_FULL_BCCH_SEARCHING;

  bcch_config_req_end:
	APImsg[0] = 0;	// nr_vendorSpecificList

    lte_bs_phy_nmm_generate_api_msg(phyInstance, APImsg, 1, (U8 *) pListening->api_output_msg, NMM_BCCH_CONFIG_CNF, status);

    return status;
}


//--------------------------------------------------------------------------------------------------------------------------
/** @brief process the STOP BCCH CONFIG REQ API message 
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param phyInstance  PHY instance ID
 *  @param ptr          pointer to API message 
 *  @param len          length of API message
 *  @return status(result) of API message processing 
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |phyInstance    |               |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |ptr            |               |input        |variable       |pReal Array   |0:8:0     |[0,2^8-1]      |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |len           |                |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              | 
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_stop_bcch_req      |
 *  ---------------------------------------
 *  |Input Stream(s)     |None |
 *  ---------------------------------------
 *  |Output Stream(s)    |None|
 *  ---------------------------------------
 *  |Config Parameter(s) |phyInstance,ptr,len |
 *  ---------------------------------------
 *  |Status Parameter(s) |None |
 *  ---------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * process the STOP BCCH CONFIG REQ API message
 * stop SIB1/BCCH searching processing
 * generate response message
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * 
 * 
 *
 * \ingroup group_lte_nmm_api
 *
 *
**/
//--------------------------------------------------------------------------------------------------------------------------
U32
lte_bs_phy_nmm_stop_bcch_req(U32 phyInstance, void *ptr, U32 len)
{
    U32             status = 0;
    PLTEPHYDESC     pPhy = getPhyCtxApiProc(phyInstance);
    PLISTENINGDESC  pListening = LteBsGetNmmCtx();

    if ((pListening->state == NMM_TRACKING) || (pListening->state == NMM_SIB1_SEARCHING) || (pListening->state == NMM_FULL_BCCH_SEARCHING))
	{
		lte_bs_phy_nmm_cell_search_clear();
		pListening->target_state_mask = 0;
        pListening->state = NMM_IDLE;
	}
    else{
        status = NMM_UNEXPECTED_CMD;
    }

    lte_bs_phy_nmm_generate_api_msg(phyInstance, NULL, 0, (U8 *) pListening->api_output_msg,
        NMM_STOP_BCCH_REQ_RESP, status);

    return status;
}

//--------------------------------------------------------------------------------------------------------------------------
/** @brief main function of cell search processing 
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param phyInstance      PHY instance ID
 *  @param sigIn            input IQ data
 *  @param outMsg           pointer to output message
 *  @param outMsgSize       pointer to length of output message
 *  @return None
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |phyInstance    |               |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |sigIn          |               |input        |variable       |pComplex Array|1:0:15    |Full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |outMsg        |                |output       |variable       |pReal         |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |outMsgSize    |                |output       |1              |pReal         |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_cell_search      |
 *  ---------------------------------------
 *  |Input Stream(s)     |sigIn |
 *  ---------------------------------------
 *  |Output Stream(s)    |outMsg|
 *  ---------------------------------------
 *  |Config Parameter(s) |phyInstance|
 *  ---------------------------------------
 *  |Status Parameter(s) |outMsgSize |
 *  ---------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * main function of cell search processing
 * call the cell search TCB to process the IQ data of one subframe;
 * call the cell search measurement function if desired cells are found.
 * generate CELL SEARCH CONFIRM response message when enough number of cell searching is 
 *  done in case of exhaustive searching
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * 
 * 
 *
 * \ingroup group_lte_nmm_cell_search
 *
 *
**/
//--------------------------------------------------------------------------------------------------------------------------

static void
lte_bs_phy_nmm_cell_search(U32 phyInstance, S16 * sig_in, U8* outMsg, U32* outMsgSize)
{
    PLTEPHYDESC     pPhy = getPhyCtxApiProc(phyInstance);
    PLISTENINGDESC  pListening = LteBsGetNmmCtx();
    int             subframe_size = 1 * 15 * pPhy->pBs->Nfft;
    int             decimation_factor = pPhy->pBs->Nfft >> 6;
    S16            *IQdecimated;
    int             i,
                    k;
    S16            *pin;
    LISTENING_CELL_SEARCH_DESC *pcell_search = pListening->pcell_search;
    volatile U32    t = GET_TICKS;
    U8              APImsg[2 + (MAX_CELL_FOUND_LIST * 3)];
    S16            *psync_found_arr = pcell_search->psync_found_arr;

#ifdef _WIN32
    if (!fIQ)
        fIQ = fopen("NMMIQ.dat", "w");
    if (!fIQ)
        exit(1);
#endif

    decimation_factor = pPhy->pBs->Nfft >> 6;

    IQdecimated = (S16 *) MemAlloc(2 * SUBFR_SIZE_096MHZ * sizeof(S16), CRAM_MEM, 1);

    nmm_dbg_printf_off(("@@@ [%s] fft %d subframe_size %d IQdecimated %x decimation_factor %d sub %d\n", __FUNCTION__, pPhy->pBs->Nfft, subframe_size, IQdecimated,
                        decimation_factor, pPhy->pBsRx->subframeNumber));

    /* Downsampling */
    lte_bs_phy_nmm_downsampling_tcb(nmm_task_list[0], sig_in, subframe_size, IQdecimated, decimation_factor,
                                    &pListening->pcell_search->hbfir_state_buf[0 * HBFIR_DECIMATION_LEN], &pListening->pcell_search->hbfir_state_buf[1 * HBFIR_DECIMATION_LEN],
                                    &pListening->pcell_search->hbfir_state_buf[2 * HBFIR_DECIMATION_LEN], &pListening->pcell_search->hbfir_state_buf[3 * HBFIR_DECIMATION_LEN],
                                    &pListening->pcell_search->hbfir_state_buf[4 * HBFIR_DECIMATION_LEN], &pListening->pcell_search->hbfir_state_buf[5 * HBFIR_DECIMATION_LEN],
                                    &pListening->pcell_search->hbfir_state_buf[6 * HBFIR_DECIMATION_LEN], &pListening->pcell_search->hbfir_state_buf[7 * HBFIR_DECIMATION_LEN],
                                    &pListening->pcell_search->hbfir_state_buf[8 * HBFIR_DECIMATION_LEN], &pListening->pcell_search->hbfir_state_buf[9 * HBFIR_DECIMATION_LEN],
                                    TCBIOBUFREFILL, nmm_tcb_arr[0][NMM_DOWNSAMPLING_TCB]);

    // NMM_BET_CHECK_DOWNSAMPLING((sigIn, subframe_size, decimation_factor, IQdecimated));

    /* AGC */
    lte_bs_phy_nmm_agc_tcb(nmm_task_list[0], pcell_search->agc_gain, pcell_search->agc_avg_nrg, IQdecimated, SUBFR_SIZE_096MHZ, nmm_tcb_arr[0],
                           TCBIOBUFREFILL, nmm_tcb_arr[0][NMM_AGC_TCB]);

#if DBG
    for (i = 0; i < (SUBFR_SIZE_096MHZ * 2); i += 2) {
        IQdecimated[i] = cnt;
        IQdecimated[i + 1] = cnt;
        cnt++;
    }
#endif

    /* Cross-corr */
    lte_bs_phy_nmm_xcorr_loop_tcb(nmm_task_list[0], pcell_search, IQdecimated, ZC25_TD, pcell_search->xcorr_arr + (pcell_search->xcorr_idx % 5) * 3 * SUBFR_SIZE_096MHZ, nmm_tcb_arr[0], TCBIOBUFREFILL, nmm_tcb_arr[0][NMM_PSS_XCORR0_TCB]);
    lte_bs_phy_nmm_xcorr_loop_tcb(nmm_task_list[0], pcell_search, IQdecimated, ZC29_TD, pcell_search->xcorr_arr + (pcell_search->xcorr_idx % 5) * 3 * SUBFR_SIZE_096MHZ + SUBFR_SIZE_096MHZ, nmm_tcb_arr[0], TCBIOBUFREFILL, nmm_tcb_arr[0][NMM_PSS_XCORR1_TCB]);
    lte_bs_phy_nmm_xcorr_loop_tcb(nmm_task_list[0], pcell_search, IQdecimated, ZC34_TD, pcell_search->xcorr_arr + (pcell_search->xcorr_idx % 5) * 3 * SUBFR_SIZE_096MHZ + 2 * SUBFR_SIZE_096MHZ, nmm_tcb_arr[0], TCBIOBUFREFILL, nmm_tcb_arr[0][NMM_PSS_XCORR2_TCB]);

	/*bypass the following TCBs*/
	lte_bs_phy_nmm_byPassMultTcb(nmm_tcb_arr[0] + NMM_FIND_CAND_TCB, 1, BYPASS_TCB);

#ifdef SCHEDULER
    /*
     * Run first task list: downsampling, agc, cross-corr, candidate selection.
     */
    nmm_dbg_printf_off(("@ %s\tARM %d\tBefore exec\ttime %lu\n", __FUNCTION__, MxGetCpuID(), (unsigned long) GET_TICKS));

    /* wait for current iq subframe copy to be finished */
    while (nmm_iq_subframe_copy_done == 0);


    nmm_runTaskList(0);

    nmm_dbg_printf_off(("@ %s\tARM %d\tAfter exec\ttime %lu\n", __FUNCTION__, MxGetCpuID(), (unsigned long) GET_TICKS));
#endif

	if ((pcell_search->agc_gain[1] == 255) || (pcell_search->agc_gain[0] < pcell_search->agc_gain[1]))
		pcell_search->agc_gain[1] = pcell_search->agc_gain[0];

	pcell_search->xcorr_idx++;
	if (pcell_search->xcorr_idx > 35)	// 4 radio frame
	{
		/* Candidate Selection */
		lte_bs_phy_nmm_pss_candidate_selection_loop_tcb(nmm_task_list[0], pcell_search->xcorr_arr + ((pcell_search->xcorr_idx-1) % 5) * 3 * SUBFR_SIZE_096MHZ,
			pcell_search->psync_found_arr, nmm_tcb_arr[0], TCBIOBUFREFILL, nmm_tcb_arr[0][NMM_FIND_CAND_TCB]);

		/*bypass the following TCBs*/
		lte_bs_phy_nmm_byPassMultTcb(nmm_tcb_arr[0], NMM_FIND_CAND_TCB, BYPASS_TCB);
		nmm_runTaskList(0);

		for (i = 0; i < MAX_CELL_FOUND_LIST; i++)
			if (pcell_search->psync_found_arr[i*2] == 3)
				break;
			else
				nmm_dbg_printf_off(("index %d NID_2 %d peakIdx %d peakValue %d Pnr %d\n", i, pcell_search->psync_found_arr[i*2], pcell_search->psync_found_arr[i*2+1],
					pcell_search->psync_found_arr[i*2 + MAX_CELL_FOUND_LIST*2], pcell_search->psync_found_arr[i*2+1 + MAX_CELL_FOUND_LIST*2]));

		if (pcell_search->xcorr_idx == 40)
		{
			pcell_search->xcorr_idx = 0;
			pcell_search->agc_gain[1] = 255;
			memset(pcell_search->xcorr_arr, 0, 3 * SUBFR_SIZE_096MHZ * 5 * sizeof(S16));
		}

		DUMP_ARR_ID_UART(IQdecimated, 2 * SUBFR_SIZE_096MHZ, "d", 0, 0);
		DUMP_ARR_ID_UART(pListening->pcell_search->hbfir_state_buf, HBFIR_DECIMATION_LEN * MAX_HBFIR_DECIMATION_STAGE * 2, "d", 0, 0);
		DUMP_ARR_ID_UART(pcell_search->psync_found_arr, SUBFR_SIZE_096MHZ, "d", 0, 0);
		DUMP_ARR_ID_UART(pcell_search->xcorr_arr, 3 * SUBFR_SIZE_096MHZ, "d", 0, 1);

#ifdef _WIN32
		DUMP_ARR_ID_F(IQdecimated, (SUBFR_SIZE_096MHZ * 2), "d", 1, 1, fIQ);
#endif

		DUMP_ARR_ID_UART(sig_in, (30 * 2), "d", 0, 1);
		DUMP_ARR_ID_UART(IQdecimated, (30 * 2), "d", 0, 1);
		DUMP_ARR_ID_UART(pcell_search->psync_found_arr, SUBFR_SIZE_096MHZ, "d", 0, 0);

		/* parse psync_found array and prepare decoding list */
		lte_bs_phy_nmm_ssync_loop(pcell_search, IQdecimated, pcell_search->psync_found_arr);

		for (i = 0; i < sync_cand; i++) {
			S16             pss_cp;
			CELL_PARAMS    *new_cell;
			S16             idx_start;
			U16				pciIdx, cellValidNum = 0;
			S16				cur_pci[3], cur_subframe[3], cell_cp[3];

			if(pcell_search->nb_detected_cell == MAX_CELL_PER_CARRIER)
				break;

			idx_start = psync_found_arr[2*i+1];
			nmm_dbg_printf_off(("<=== PCI  %d %d %d index %d ===>\n", *pci0_arr[i], *pci1_arr[i], *pci2_arr[i], idx_start));

			if (*pci0_arr[i] >= 0) {
				cur_pci[cellValidNum] = *pci0_arr[i];
				cur_subframe[cellValidNum] = *subframe0_arr[i];
				cell_cp[cellValidNum] = SHORT_CP;
				cellValidNum++;
			}
			
			/* if nothing found, then try with short CP, case shifted by 4 */
			if (*pci1_arr[i] >= 0) {
				cur_pci[cellValidNum] = *pci1_arr[i];
				cur_subframe[cellValidNum] = *subframe1_arr[i];
				cell_cp[cellValidNum] = SHORT_CP;
				cellValidNum++;
			}

			/* if no short CP was found, then try with long CP */
			if (*pci2_arr[i] >= 0) {
				cur_pci[cellValidNum] = *pci2_arr[i];
				cur_subframe[cellValidNum] = *subframe2_arr[i];
				cell_cp[cellValidNum] = LONG_CP;
				cellValidNum++;
			}
			
			for (pciIdx = 0; pciIdx < cellValidNum; pciIdx++)
			{
				if (is_pci_already_found(cur_pci[pciIdx], pcell_search->cell_param, pcell_search->nb_detected_cell)) {
					nmm_dbg_printf_off(("@@@ pci %d already detected @ %d\n", cur_pci[pciIdx], idx_start));
					continue;
				}

				if (pListening->state == NMM_PBCH_SYNC_SEARCHING && cur_pci[pciIdx] != pListening->pbch_pci) {
					nmm_dbg_printf_off(("@@@ unwanted PCI detected (%d) during NMM_PBCH_SYNC_SEARCHING state\n", cur_pci[pciIdx]));
					// the one which was found is not in the list of cells to be found
					continue;
				}

				/* check if PCI is in PCI list when not doing exhaustive search */
				if (pListening->cell_search_exhaustive_search == FALSE && !is_pci_in_nmm_list(cur_pci[pciIdx], (S16 *) pListening->cell_search_pci_list, pListening->cell_search_nr_pci_list)) {
					nmm_dbg_printf_off(("@@@ unwanted detected cell %d, %d, %d\n", idx_start, cur_pci[pciIdx], *subframe));
					continue;
				}

				/* at this time Cell presence is confirmed and its PCI is known */
				new_cell = &pcell_search->cell_param[pcell_search->nb_detected_cell];
				new_cell->cell_pci = cur_pci[pciIdx];
				new_cell->cell_cp = cell_cp[pciIdx];
				new_cell->cell_subframe = cur_subframe[pciIdx];
				new_cell->cell_freqency_offset = *Psync_foe_arr[i];
				pcell_search->nb_detected_cell++;
				new_cell->cell_antennas = 1; // for later use in FOE
				if(new_cell->cell_cp == SHORT_CP)
				{
					new_cell->pbch_re_per_frame = 240;
					pss_cp = (PSS_SHORT_CP_096MHZ * decimation_factor) >> 1;
				}
				else
				{
					new_cell->pbch_re_per_frame = 216;
					pss_cp = (PSS_LONG_CP_096MHZ * decimation_factor) >> 1;
				}

				pcell_search->nb_detected_requested_cell ++;

				new_cell->cell_detect_num = 1;
				/*
				 * symbol timing search in undecimated buffer location
				 * idx_start=(i-1 - 4.5)*decimation_fac;
				 */
				idx_start = (idx_start + 1) * decimation_factor - pss_cp;	// add 1 to make consistent with previous
				idx_start = idx_start - DECIM_16_GRP_DELAY - pPhy->pBs->Nfft;

				/* set slot 0 index */
				idx_start = idx_start + pPhy->pBs->Nfft + pss_cp - (15 * pPhy->pBs->Nfft >> 1);

				if (idx_start < -(pPhy->pBs->Nfft + pPhy->pBs->Nfft/8))
				{
					idx_start += 15 * pPhy->pBs->Nfft;
					new_cell->cell_subframe = (new_cell->cell_subframe + 1) % 10;
				}
				new_cell->cell_current_sym_cnt = SYMBOL(0);
        		new_cell->cell_time_offset = (S32) (idx_start - 8) << 4;	// advance OFDM symbol window by 8 sample
				new_cell->cell_current_sym_idx = 0;

				nmm_dbg_printf_off(("@@@ Adjusted index 1 %d / %d rem %d pss_cp %d\n", idx_start, subframe_size, subframe_size - idx_start, pss_cp));

				/*
				* precompute PBCH scrambling sequence
				*/
				if (pListening->target_state_mask & (1 << NMM_PBCH_SEARCHING)) {
					if (new_cell->pbch_scrambling_seq == NULL) {
						new_cell->pbch_scrambling_seq = MemAlloc(PBCH_SIZE_IN_BITS * sizeof(U8), CRAM_MEM, 1);
						new_cell->pbch_buf = MemAlloc(2 * PBCH_SIZE * sizeof(S16), CRAM_MEM, 1);
						new_cell->pbch_scale = MemAlloc(PBCH_SYM_PER_FRAME * PBCH_NB_FRAME * sizeof(S16), CRAM_MEM, 1);
						new_cell->pdcch_scrambling_seq = MemAlloc(PDCCH_MAX_SIZE_IN_BITS * sizeof(U8), CRAM_MEM, 1);

						for (k = 0; k < MAX_N_SIB_TYPES; k++)
							new_cell->si_periodicity[k] = si_unkown;
					}

					lte_bs_phy_nmm_pseudo_random_seq_gen_tcb(NULL, PBCH_SIZE_IN_BITS, new_cell->cell_pci, new_cell->pbch_scrambling_seq,
						nmm_tcb_arr[3][2+NUM_SYMB_PBCH]);
					lte_bs_phy_nmm_byPassMultTcb(nmm_tcb_arr[3], (2+NUM_SYMB_PBCH), BYPASS_TCB);
					lte_bs_phy_nmm_byPassMultTcb(nmm_tcb_arr[3]+3+NUM_SYMB_PBCH, (N_PBCH_TCB-(3+NUM_SYMB_PBCH)), BYPASS_TCB);
					nmm_runTaskList(3);
				}

					/*
				 * counter for RSRQ measuring
				 */
				new_cell->rsrp_measure_time = (pListening->cell_search_rssi_measurement_period * PILOTS_SYM_PER_FRAME);
				new_cell->avg_rsrp = 0;

				if ((pListening->cell_search_exhaustive_search == FALSE) && (pcell_search->nb_detected_requested_cell == pListening->cell_search_nr_pci_list))
				{
					pListening->state = NMM_SYNC_SEARCHING_DONE;
            		pListening->dlrs_foe_cellIdx = 0;
        		}
			}

		}

		for (i = 0; i < sync_cand; i++) {

			MemFree(PSync_f_arr[i], CRAM_MEM);
			MemFree(PSyncExp_arr[i], CRAM_MEM);
			MemFree(Psync_foe_arr[i], CRAM_MEM);

			MemFree(SSync0_f_arr[i], CRAM_MEM);
			MemFree(SSync0Exp_arr[i], CRAM_MEM);

			MemFree(SSync1_f_arr[i], CRAM_MEM);
			MemFree(SSync1Exp_arr[i], CRAM_MEM);

			MemFree(SSync2_f_arr[i], CRAM_MEM);
			MemFree(SSync2Exp_arr[i], CRAM_MEM);

			MemFree(SSync0Est_arr[i], CRAM_MEM);
			MemFree(SSync1Est_arr[i], CRAM_MEM);
			MemFree(SSync2Est_arr[i], CRAM_MEM);

			nmm_dbg_printf_off(("<=== PCI  %d %d %d ===>\n", *pci0_arr[i], *pci1_arr[i], *pci2_arr[i]));

			MemFree(pci0_arr[i], CRAM_MEM);
			MemFree(pci1_arr[i], CRAM_MEM);
			MemFree(pci2_arr[i], CRAM_MEM);

			MemFree(subframe0_arr[i], CRAM_MEM);
			MemFree(subframe1_arr[i], CRAM_MEM);
			MemFree(subframe2_arr[i], CRAM_MEM);

			MemFree(data_SSS_PSS_arr[i], CRAM_MEM);

		}
	}

    /* store last decimated points for the next subfr frame xcor */
    pin = &IQdecimated[(2 * (SUBFR_SIZE_096MHZ - (FFT_SIZE_096MHZ - 1 + SSS_SEARCH_RANGE)))];
    memcpy(pcell_search->cell_search_xcorr_buf, pin, (2 * (FFT_SIZE_096MHZ - 1 + SSS_SEARCH_RANGE)) * sizeof(S16));

    /* store last un- decimated points for the next subfr symbol timing search */
    pin = &sig_in[(2 * (subframe_size - (MAX_CP_SIZE + MAX_DECIM_GRP_DELAY + LEN_EXTRA_CP)))];
    memcpy(pcell_search->cell_search_sym_bound_buf, pin, (2 * (MAX_CP_SIZE + MAX_DECIM_GRP_DELAY + LEN_EXTRA_CP)) * sizeof(S16));

    MemFree(IQdecimated, CRAM_MEM);

    pListening->cell_search_time--;

    /* current carrier cell searching timeout */
    if (pListening->cell_search_time == 0) {
		pListening->dlrs_foe_cellIdx = 0;
		pListening->dlrs_foe_cnt = 1;

		if (pListening->cell_search_exhaustive_search == TRUE) {
			for (i = 0; i < pcell_search->nb_detected_cell; i++)
				if (pListening->pcell_search->cell_param[i].cell_detect_num > 3)
				{
					pListening->dlrs_foe_cellIdx = i;
					pListening->dlrs_foe_cnt = 0;
					break;
				}
		}

		if (pListening->dlrs_foe_cnt == 1)
			pcell_search->nb_detected_cell = 0;

        if (pcell_search->nb_detected_cell > 0) {
			pListening->state = NMM_SYNC_SEARCHING_DONE;
            pListening->dlrs_foe_cellIdx = 0;
        }
		else
		{
			pListening->state = NMM_IDLE;
			/*nr_vendorSpecificList*/
			APImsg[0] = 0;

            if(pListening->target_state_mask == 0)
            {
			    lte_bs_phy_nmm_generate_api_msg(phyInstance, APImsg, 1, (U8 *) pListening->api_output_msg,
				    NMM_CELL_SEARCH_CNF, 0);
            }
            else
            {
                /*send PBCH data indication (fail)*/
                if((pListening->target_state_mask & (1<<NMM_PBCH_SEARCHING)) != 0)
                {
                    U8              APImsg[NMM_PBCH_DATA_IND_API_SIZE+1];
                    U16            *p16;

                    p16 = (U16 *) APImsg;
                    *p16 = pListening->pbch_pci;
                    APImsg[2] = MIB_FAILED;
                    APImsg[3] = 0;
                    p16 = (U16 *) & APImsg[4];
                    *p16 = 0;
                    APImsg[6] = 0;
                    APImsg[7] = 0;
                    APImsg[8] = 0;
				    APImsg[9] = 0;	// nr_vendorSpecificList

                    lte_bs_phy_nmm_generate_api_msg(phyInstance, APImsg, NMM_PBCH_DATA_IND_API_SIZE + 1, (U8 *) pListening->api_output_msg, NMM_PBCH_DATA_IND, 0);

                }
            }

			lte_bs_phy_nmm_cell_search_clear();
		}
    }
}
//--------------------------------------------------------------------------------------------------------------------------
/** @brief  RSRP and frequency offset estimation as part of the cell search function 
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param phyInstance      PHY instance ID
 *  @param sigIn            input IQ data
 *  @param outMsg           pointer to output message
 *  @param outMsgSize       pointer to length of output message
 *  @return None
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |phyInstance    |               |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |sigIn          |               |input        |variable       |pComplex Array|1:0:15    |Full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |outMsg        |                |output       |variable       |pReal         |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |outMsgSize    |                |output       |1              |pReal         |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_cell_search_measure_loop      |
 *  ---------------------------------------
 *  |Input Stream(s)     |sigIn |
 *  ---------------------------------------
 *  |Output Stream(s)    |outMsg|
 *  ---------------------------------------
 *  |Config Parameter(s) |phyInstance|
 *  ---------------------------------------
 *  |Status Parameter(s) |outMsgSize |
 *  ---------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * RSRP and frequency offset estimation as part of the cell search function
 * this is done after a cell is found
 * when enough times of measurement is done 
 *    if the cell search is requested by higher layer generate CELL SEARCH INDICATION message to higher layer;
 *    if the cell search is part of PBCH searching turn to PBCH searching state
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * 
 * 
 *
 * \ingroup group_lte_nmm_cell_search
 *
 *
**/
//--------------------------------------------------------------------------------------------------------------------------

static void
lte_bs_phy_nmm_cell_search_measure_loop(U32 phyInstance, S16 * sigIn, U8* outMsg, U32* outMsgSize)
{
	PLTEPHYDESC     pphy = getPhyCtxApiProc(phyInstance);
    PLISTENINGDESC  pListening = LteBsGetNmmCtx();
	LISTENING_CELL_SEARCH_DESC *pcell_search = pListening->pcell_search;
	CELL_PARAMS    *current_cell = &pcell_search->cell_param[pListening->dlrs_foe_cellIdx];
    SYMBOL_INFO    *psymb_info = current_cell->symb_info;
	S16             channelEsti_buf[2400];
	S16				i;
	U8              APImsg[2 + (MAX_CELL_FOUND_LIST * 3)];

	lte_bs_phy_nmm_channel_estimate_loop(NULL, current_cell, PBCH_NB_RB, pphy->pBs->expoNfft, channelEsti_buf);

	while (psymb_info->start != NULL) {
		U16             symb_index = psymb_info->symb_index;

		if ((current_cell->cell_cp == SHORT_CP && (symb_index == SYMBOL(0) || symb_index == SYMBOL(4) || symb_index == SYMBOL(7) || symb_index == SYMBOL(11))) ||
			(current_cell->cell_cp == LONG_CP && (symb_index == SYMBOL(0) || symb_index == SYMBOL(3) || symb_index == SYMBOL(6) || symb_index == SYMBOL(9))))
		{
			/* do measurment as long as RSRP measure time has not ellapsed */
			if (current_cell->rsrp_measure_time > 0) {
				/* do measurment as long as RSRP measure time has not ellapsed */
				lte_bs_phy_nmm_get_rsrp_tcb(nmm_task_list[2], pphy->pBs->expoNfft, psymb_info, current_cell->cell_pci, current_cell->cell_cp,
					pListening->cell_search_meas_bandwidth, &current_cell->avg_rsrp, pListening->rsrp_scale,
					(pListening->cell_search_meas_bandwidth * REF_SIGNAL_PER_RB), (pListening->cell_search_meas_bandwidth_inv >> 1),
					nmm_tcb_arr[2][0]);

				/*bypass the TCBs not used
				if(rsrp_to_measure_num<(MAX_NUM_CELLRS_SUBFRM)) //!!!
				{
				lte_bs_phy_nmm_byPassMultTcb(nmm_tcb_arr[2]+rsrp_to_measure_num, ((MAX_NUM_CELLRS_SUBFRM)-rsrp_to_measure_num), BYPASS_TCB); //!!!
				}*/
				lte_bs_phy_nmm_byPassMultTcb(nmm_tcb_arr[2]+1, ((MAX_NUM_CELLRS_SUBFRM)-1), BYPASS_TCB);

				nmm_runTaskList(2);

				current_cell->rsrp_measure_time--;

				nmm_dbg_printf_off(("cell %d  current_cell->avg_rsrp  %d \r\n", pListening->dlrs_foe_cellIdx, current_cell->avg_rsrp));

				if (current_cell->rsrp_measure_time == 0) {
					if ((pListening->report_individual_cell_params) && (pListening->target_state_mask == 0))
					{
						S16             tmp;
						S16				shift;

						if (current_cell->avg_rsrp == 0)
							current_cell->avg_rsrp = 1;

						shift = norm_l(current_cell->avg_rsrp) - 1;
						current_cell->avg_rsrp = L_shl(current_cell->avg_rsrp, shift);
						tmp = dbmconv(round_s(current_cell->avg_rsrp));
						tmp -= 3 * shift;

						tmp -= 30; //because the 1024-FFT has 30dB gain against time domain power

						/* FAPI mapping 0..128 */
						tmp = tmp + 128;

						APImsg[0] = 1;  /* single cell */
						APImsg[1] = (U8) tmp;
						*((S16 *) & APImsg[2]) = current_cell->cell_pci;
						APImsg[4] = 0;	// nr_vendorSpecificList

						nmm_dbg_printf_off(("zz %x %x %x %x (%d %d)\n", APImsg[0], APImsg[1], APImsg[2], APImsg[3], round_s(current_cell->avg_rsrp),
							dbmconv((S16) round_s(current_cell->avg_rsrp))));

						lte_bs_phy_nmm_generate_api_msg(phyInstance, APImsg, SINGLE_NMM_CELL_SEARCH_IND_API_SIZE + 1, (U8 *) pListening->api_output_msg, NMM_CELL_SEARCH_IND,0);
					}

					pListening->dlrs_foe_cellIdx++;

					if (pListening->cell_search_exhaustive_search == TRUE) {
						for (i = pListening->dlrs_foe_cellIdx; i < pcell_search->nb_detected_cell; i++)
							if (pListening->pcell_search->cell_param[i].cell_detect_num > 3)
							{
								pListening->dlrs_foe_cellIdx = i;
								pListening->dlrs_foe_cnt = 3;
								break;
							}

							if (pListening->dlrs_foe_cnt != 3)
								pListening->dlrs_foe_cellIdx = pListening->pcell_search->nb_detected_cell; // there is no valid cell, so set to end
							else
								pListening->dlrs_foe_cnt = 0;
					}

					if (pListening->dlrs_foe_cellIdx == pListening->pcell_search->nb_detected_cell)
					{
						pListening->dlrs_foe_cellIdx = 0;
						if ((!pListening->report_individual_cell_params) && (pListening->target_state_mask == 0))
						{
							U8             *p8 = APImsg;

							*p8++ = (U8) pcell_search->nb_detected_cell;
							for (i = 0; i < pcell_search->nb_detected_cell; i++) {
								S16             tmp;
								S16				shift;

								current_cell = &pcell_search->cell_param[i];
								shift = norm_l(current_cell->avg_rsrp) - 1;
								current_cell->avg_rsrp = L_shl(current_cell->avg_rsrp, shift);
								tmp = dbmconv(round_s(current_cell->avg_rsrp));
								tmp -= 3 * shift;

								/* FAPI mapping 0..128 */
								tmp = tmp + 128; 
								*p8++ = (U8) tmp;

								/* no good, alignement issue on ARM */
								*((S16 *) p8) = current_cell->cell_pci;
								p8 += 2;
							}
							p8[0] = 0;	// nr_vendorSpecificList

							lte_bs_phy_nmm_generate_api_msg(phyInstance, APImsg, 2 + (pcell_search->nb_detected_cell * 3), (U8 *) pListening->api_output_msg, NMM_CELL_SEARCH_IND, 0);
						}

						/*nr_vendorSpecificList*/
						APImsg[0] = 0;

						if(pListening->target_state_mask == 0)
						{
							pListening->state = NMM_IDLE;
							lte_bs_phy_nmm_generate_api_msg(phyInstance, APImsg, 1, (U8 *) pListening->api_output_msg,
								NMM_CELL_SEARCH_CNF, 0);
							lte_bs_phy_nmm_cell_search_clear();
						}

						if (pListening->target_state_mask & (1 << NMM_PBCH_SEARCHING)) {
							pListening->target_state_mask &= ~(1 << NMM_PBCH_SEARCHING);
							pListening->state = NMM_PBCH_SEARCHING;
						}
					}

				}
			}
		}

        psymb_info++;
	}
}
//--------------------------------------------------------------------------------------------------------------------------
/** @brief do RSSI measurement  
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param phyInstance      PHY instance ID
 *  @param sigIn            input IQ data
 *  @param outMsg           pointer to output message
 *  @param outMsgSize       pointer to length of output message
 *  @return None
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |phyInstance    |               |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |sigIn          |               |input        |variable       |pComplex Array|1:0:15    |Full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |outMsg        |                |output       |variable       |pReal         |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |outMsgSize    |                |output       |1              |pReal         |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_rssi_measure      |
 *  ---------------------------------------
 *  |Input Stream(s)     |sigIn |
 *  ---------------------------------------
 *  |Output Stream(s)    |outMsg|
 *  ---------------------------------------
 *  |Config Parameter(s) |phyInstance|
 *  ---------------------------------------
 *  |Status Parameter(s) |outMsgSize |
 *  ---------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * do RSSI measurement
 * call RSSI measurement TCB to do the measurement
 * generate RSSI MEASUREMENT INDICATION and RSSI MEASUREMENT CONFIRM message to higher layer
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * 
 * 
 *
 * \ingroup group_lte_nmm_rssi
 *
 *
**/
//--------------------------------------------------------------------------------------------------------------------------

static void
lte_bs_phy_nmm_rssi_measure(U32 phyInstance, S16 * sigIn, U8* outMsg, U32* outMsgSize)
{
    PLTEPHYDESC     pPhy = getPhyCtxApiProc(phyInstance);
    PLISTENINGDESC  pListening = LteBsGetNmmCtx();
    S32             tmp;
    U8              tmp8;
    U32             APImsg;
    U32             L_temp,tmp2;
	S16				i, shift;
	S32				rssiTmp;
	S32			   *agc_gain, *agc_avg_nrg;

    agc_gain = MemAlloc(sizeof(S32), CRAM_MEM, 1);
    agc_avg_nrg = MemAlloc(sizeof(S32), CRAM_MEM, 1);

    /* measurment ongoing for current earfcn */
    if (pListening->cur_rssi_measurement_time++ < pListening->rssi_measurement_period_ms) {
#ifndef NMM_RSSI_USETCB
        tmp = READ_RSSI;
#else
		/* use AGC TCB to get rssi */
		rssiTmp = 0;
		for (i = 0; i < 16; i++)
		{
			lte_bs_phy_nmm_agc_tcb(nmm_task_list[0], agc_gain, agc_avg_nrg, &sigIn[2*SUBFR_SIZE_096MHZ*i], SUBFR_SIZE_096MHZ, nmm_tcb_arr[0],
								   TCBIOBUFREFILL, nmm_tcb_arr[0][NMM_AGC_TCB]);
			/*bypass the following TCBs*/
			lte_bs_phy_nmm_byPassMultTcb(nmm_tcb_arr[0], NMM_AGC_TCB, BYPASS_TCB);
			lte_bs_phy_nmm_byPassMultTcb(nmm_tcb_arr[0] + NMM_AGC_TCB + 1, NMM_FIND_CAND_TCB - NMM_AGC_TCB, BYPASS_TCB);
			nmm_runTaskList(0);

			rssiTmp += L_shr(*agc_avg_nrg, (S16) *agc_gain*2 + 4);
		}

        tmp = rssiTmp&0xffff;
        L_temp = tmp * pListening->rssi_meas_avg_factor;
        tmp2 = (L_temp >> 15);
        tmp = (rssiTmp >> 16)&0xffff;
        L_temp = tmp * pListening->rssi_meas_avg_factor;
        L_temp <<= 1;
        tmp = L_temp + tmp2;

#endif
        pListening->averaged_rssi = L_add(tmp, pListening->averaged_rssi);
    }

    MemFree(agc_gain, CRAM_MEM);
    MemFree(agc_avg_nrg, CRAM_MEM);

    if (pListening->cur_rssi_measurement_time == pListening->rssi_measurement_period_ms) {
        /*
         * measurment done for current earfcn,
         * report avg RSSI in dbm in steps onf 1dB
         */
        nmm_dbg_printf_off(("@@@ [%s] averaged_rssi %d \r\n", __FUNCTION__, pListening->averaged_rssi));

		shift = norm_l(pListening->averaged_rssi) - 1;
		pListening->averaged_rssi = L_shl(pListening->averaged_rssi, shift);
        tmp = dbmconv(round_s(pListening->averaged_rssi));
		tmp -= 3 * shift;

        tmp = tmp + 128 + 3; //full scale is 2^30, so need add 3dB
        tmp8 = (U8) tmp;

        /*
         * NMM_RSSI_MEAS_IND Output format
         * U8 rssi      0-128 (-128..0)       The measured RSSI in dBFS in steps of 1dB.
         * U16 earfcn   0-65535                         The carrier measured.
         * data packed into word because of alignement
         */
		if (pListening->nr_earfcnList)
			APImsg = tmp8 | (pListening->rssi_earfcn_list[pListening->cur_earfcn] << 8);
		else
			APImsg = tmp8 | ((pListening->cur_earfcn + EARFNC_TABLE[pListening->freq_band_indicator].range_min) << 8);
        // printf("*APImsg %08x\n",*APImsg);
        lte_bs_phy_nmm_generate_api_msg(phyInstance, (U8 *) & APImsg, sizeof(U8) + sizeof(U16) + sizeof(U8), (U8 *) pListening->api_output_msg, NMM_RSSI_MEAS_IND, 0);

        pListening->cur_earfcn++;

        if (pListening->nr_earfcnList) {
            if (pListening->cur_earfcn == pListening->nr_earfcnList) {
                pListening->state = NMM_IDLE;
				tmp8 = 0;	// nr_vendorSpecificList
                lte_bs_phy_nmm_generate_api_msg(phyInstance, &tmp8, 1, (U8 *) pListening->api_output_msg, NMM_RSSI_MEAS_CNF, 0);

                return;
            }
            SWITCH_RFIC_DL_FREQ(BAND_EARFCN_TO_KHZ(pListening->freq_band_indicator, pListening->rssi_earfcn_list[pListening->cur_earfcn]));
        }
        else {
            /*
             * full search case
             */
            if (pListening->cur_earfcn == NB_EARFCN_PER_BAND(pListening->freq_band_indicator)) {
                pListening->state = NMM_IDLE;
				tmp8 = 0;	// nr_vendorSpecificList
                lte_bs_phy_nmm_generate_api_msg(phyInstance, &tmp8, 1, (U8 *) pListening->api_output_msg, NMM_RSSI_MEAS_CNF, 0);

                return;
            }
            SWITCH_RFIC_DL_FREQ(EARFCNIDX_TO_KHZ(pListening->freq_band_indicator, pListening->cur_earfcn));
        }

        pListening->cur_rssi_measurement_time = 0;
        pListening->averaged_rssi = 0;
    }
}


//--------------------------------------------------------------------------------------------------------------------------
/** @brief calculate the starting address of each OFDM symbol in the IQ data 
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param phyInstance          PHY instance ID
 *  @param current_cell         pointer to current cell info structure
 *  @param sigIn                pointer to input IQ data
 *  @param symb_info            pointer to symbol info structure
 *  @return None
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |phyInstance    |               |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |current_cell   |               |input        |1              |pStructure    |         |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |sigIn          |               |input        |1              |pComplex array|1:0:15    |Full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |symb_info      |               |input        |1              |pStructure    |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_get_symbol_start      |
 *  ---------------------------------------
 *  |Input Stream(s)     |sigIn |
 *  ---------------------------------------
 *  |Output Stream(s)    |symb_info|
 *  ---------------------------------------
 *  |Config Parameter(s) |phyInstance,current_cell |
 *  ---------------------------------------
 *  |Status Parameter(s) |None |
 *  ---------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * calculate the starting address of each OFDM symbol in the IQ data
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * 
 * 
 *
 * \ingroup group_lte_nmm_phy
 *
 *
**/
//--------------------------------------------------------------------------------------------------------------------------
static void
lte_bs_phy_nmm_get_symbol_start(U32 phyInstance, CELL_PARAMS * current_cell, S16 * sigIn, SYMBOL_INFO * symb_info)
{
    PLTEPHYDESC     pPhy = getPhyCtxApiProc(phyInstance);
    PLISTENINGDESC  pListening = LteBsGetNmmCtx();
    S16             subframe_size = 1 * 15 * pPhy->pBs->Nfft;
    U16             decimation_factor = pPhy->pBs->Nfft >> 6;
    S16            *pin;
    // LISTENING_CELL_SEARCH_DESC *pcell_search = pListening->pcell_search;
    // CELL_PARAMS *current_cell = pcell_search->cell_param;
    S16             current_sym_length;
    S16             symb_per_subfr = SYM_PER_SUBFR_SHORT_CP;
    S16             cp_size;

#if DBG
    U32             i;

    for (i = 0; i < (subframe_size * 2); i += 2) {
        sigIn[i] = (S16) (cnt & 0x3fff);
        sigIn[i + 1] = (S16) (cnt & 0x3fff);
        cnt++;
    }
#endif

    /* get next symbol length */
    if (current_cell->cell_cp == LONG_CP) {
        symb_per_subfr = SYM_PER_SUBFR_LONG_CP;
        cp_size = (LONG_CP_BASE_SIZE * decimation_factor >> 1);
        current_sym_length = pPhy->pBs->Nfft + cp_size;
    }
    else if (current_cell->cell_current_sym_cnt == SYMBOL(0) || current_cell->cell_current_sym_cnt == SYMBOL(7)) {
        cp_size = (SHORT_CP0_BASE_SIZE * decimation_factor >> 1);
        current_sym_length = pPhy->pBs->Nfft + cp_size;
    }
    else {
        cp_size = (SHORT_CP_BASE_SIZE * decimation_factor >> 1);        /* 4.5 */
        current_sym_length = pPhy->pBs->Nfft + cp_size;
    }


	while (current_cell->cell_current_sym_idx + current_sym_length <= subframe_size) {
		pin = &sigIn[2 * current_cell->cell_current_sym_idx];

        nmm_dbg_printf_off(("pin @%x (sym idx %d)\n", pin, current_cell->cell_current_sym_idx));
        symb_info->start = pin;
        symb_info->len = current_sym_length;
        symb_info->cp_size = cp_size;
        symb_info->symb_index = current_cell->cell_current_sym_cnt;
        symb_info->fft_done = FALSE;
        symb_info++;

        current_cell->cell_current_sym_idx += current_sym_length;
        nmm_dbg_printf_off(("@@@ subfr %d sym %d cell_current_sym_cnt %d len %d\n", current_cell->cell_subframe, current_cell->cell_current_sym_idx,
                            current_cell->cell_current_sym_cnt, current_sym_length));

        current_cell->cell_current_sym_cnt++;
        if (current_cell->cell_current_sym_cnt == symb_per_subfr) {
            current_cell->cell_current_sym_cnt = 0;
            current_cell->cell_subframe++;
            if (current_cell->cell_subframe == SUBFRAME_PER_RADIO)
			{
                current_cell->cell_subframe = 0;
                current_cell->sfn++;
				current_cell->sfn &= 1023; //0~1023
			}
        }

        /* get next symbol length */
        if (current_cell->cell_cp == LONG_CP) {
            symb_per_subfr = SYM_PER_SUBFR_LONG_CP;
            cp_size = (LONG_CP_BASE_SIZE * decimation_factor >> 1);
            current_sym_length = pPhy->pBs->Nfft + cp_size;
        }
        else if (current_cell->cell_current_sym_cnt == SYMBOL(0) || current_cell->cell_current_sym_cnt == SYMBOL(7)) {
            cp_size = (SHORT_CP0_BASE_SIZE * decimation_factor >> 1);
            current_sym_length = pPhy->pBs->Nfft + cp_size;
        }
        else {
            cp_size = (SHORT_CP_BASE_SIZE * decimation_factor >> 1);    /* 4.5 */
            current_sym_length = pPhy->pBs->Nfft + cp_size;
        }
    }

    /* mark last symbol */
    symb_info->start = NULL;

    nmm_dbg_printf_off(("@@@ RSRP measure remaining %d\n", current_cell->cell_current_sym_idx - subframe_size));
    current_cell->cell_current_sym_idx = (current_cell->cell_current_sym_idx - subframe_size);
    nmm_dbg_printf_off(("@@@ RSRP measure end cell_current_sym_idx %d\n", current_cell->cell_current_sym_idx));
}


//--------------------------------------------------------------------------------------------------------------------------
/** @brief calculate the starting address of each OFDM symbol in the IQ data for all cells 
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param phyInstance  PHY instance ID
 *  @param sigIn        pointer to input IQ data
 *  @return None
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |phyInstance    |               |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |sigIn          |               |input        |1              |pComplex array|1:0:15    |Full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_get_cell_symbol_info      |
 *  ---------------------------------------
 *  |Input Stream(s)     |sigIn |
 *  ---------------------------------------
 *  |Output Stream(s)    |None|
 *  ---------------------------------------
 *  |Config Parameter(s) |phyInstance |
 *  ---------------------------------------
 *  |Status Parameter(s) |None |
 *  ---------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * calculate the starting address of each OFDM symbol in the IQ data for all cells
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * 
 * 
 *
 * \ingroup group_lte_nmm_phy
 *
 *
**/
//--------------------------------------------------------------------------------------------------------------------------
static void
lte_bs_phy_nmm_get_cell_symbol_info(U32 phyInstance, S16 * sigIn)
{
    PLTEPHYDESC     pPhy = getPhyCtxApiProc(phyInstance);
    PLISTENINGDESC  pListening = LteBsGetNmmCtx();
    U32             i;
    LISTENING_CELL_SEARCH_DESC *pcell_search = pListening->pcell_search;
    CELL_PARAMS    *current_cell;

    if (!pcell_search)
        return;
    current_cell = pcell_search->cell_param;

    /* wait for current iq subframe copy to be finished */
    while (nmm_iq_subframe_copy_done == 0);

    if (pcell_search->nb_detected_cell) {
        if (pListening->search_timing_for_all_cells)
            /* loop at all detected cells (precise timing on all detected cells) */
            i = pcell_search->nb_detected_cell;
        else
            /* strongest one is the reference */
            i = 1;

        do {
            SYMBOL_INFO    *psymb_info = current_cell->symb_info;
            lte_bs_phy_nmm_get_symbol_start(phyInstance, current_cell, sigIn, psymb_info);
            current_cell++;
        } while (--i);
    }
}

static void
lte_bs_phy_nmm_check_cell_sync_during_pch(U32 phyInstance)
{
    PLTEPHYDESC     pPhy = getPhyCtxApiProc(phyInstance);
    PLISTENINGDESC  pListening = LteBsGetNmmCtx();
    LISTENING_CELL_SEARCH_DESC *pcell_search = pListening->pcell_search;

    if (!pcell_search)
        return;

    /* goes back to PBCH tracking */
    if (pcell_search->nb_detected_cell)
        pListening->state = NMM_PBCH_SEARCHING;
}


//-------------------------------------------------------------------------------------------
/** @brief Channel Estimation Loop
 *
 *  <1> Hardware Resource:
 *  ARM
 *  @param pTskLst            pointer to task list
 *  @param current_cell		  Point to cell parameter structure
 *  @param Ndl_rb   		  Number of downlink RB
 *  @param expo_nfft		  Number of FFT exponent
 *  @param chEst_cram		  Pointer to channel estimation memory
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length   |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |         |               |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |pTskLst      |         |pInput			|1        |pStructure	  |			  |			   |      | 
 *  ---------------------------------------------------------------------------------------------------
 *  |current_cell |         |pInput			|1        |pStructure	  |			  |			   |      | 
 *  ---------------------------------------------------------------------------------------------------
 *  |Ndl_rb  	  |         |Input   		|		  |Real			  |(1:15:0)   |Full Range  |	  |
 *  ---------------------------------------------------------------------------------------------------
 *  |expo_nfft    |         |Input   		|		  |Real			  |(1:15:0)   |Full Range  |	  |
 *  ---------------------------------------------------------------------------------------------------
 *  |chEst_cram   |         |pOutput	    |         |pComplex Array |(1:0:15)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_channel_estimate_loop  |
 *  --------------------------------------------------------
 *  |Input Streams       |None   		           |
 *  --------------------------------------------------------
 *  |Output Streams      |chEst_cram					   |
 *  --------------------------------------------------------
 *  |Config Parameters   |current_cell,Ndl_rb,expo_nfft |
 *  --------------------------------------------------------
 *  |Status Parameters   |none		                       |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to do frequency offset compensation, and channel estimation function
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 * <6> References:
 * 3GPP 36.211
 *
 *  \ingroup group_lte_nmm_cell_search	
 *
 **/
void
lte_bs_phy_nmm_channel_estimate_loop(PTASKLIST pTskLst, CELL_PARAMS *current_cell, S16 Ndl_rb, S16 expo_nfft, S16 *chEst_cram)
{
    PLISTENINGDESC  pListening = LteBsGetNmmCtx();
    S16         Ndl_symb;
    S16         *Rx_RS_symbol;
    U16         Ntx_ante = current_cell->cell_antennas;
    U16         N_RS_symb_time = 4;
    U16         N_fft = 1<<expo_nfft;
	S16			i;
	S16			symbolPos;
	S16			fft_size = 1 << expo_nfft;
	S16			CP_scale = 1 << (expo_nfft-7);
	S32			*foeCur;

    // Init variables
    Ndl_symb = (current_cell->cell_cp == SHORT_CP) ? PBCH_SYM_START_SHORT_CP : PBCH_SYM_START_LONG_CP;

	/* compensate frequency offset for all OFDM symbols in time domain */
	for (i = 0; i < Ndl_symb * 2; i++)
	{
		if (current_cell->cell_cp == SHORT_CP)
		{
			if (i < Ndl_symb)
				symbolPos = i * fft_size + (i * SHORT_CP_BASE_SIZE + SHORT_CP0_BASE_SIZE) * CP_scale;
			else
				symbolPos = i * fft_size + ((i - 1) * SHORT_CP_BASE_SIZE + 2 * SHORT_CP0_BASE_SIZE) * CP_scale;
		}
		else
			symbolPos = i * fft_size + (i + 1) * LONG_CP_BASE_SIZE * CP_scale;

		lte_bs_phy_nmm_freqOffset_comp_tcb(pTskLst, current_cell->symb_info[i].start + current_cell->symb_info[i].cp_size * 2,
										   current_cell->cell_freqency_offset, symbolPos, (1 << expo_nfft), nmm_tcb_arr[3][i+4+NUM_SYMB_PBCH]);
	}


    /*bypass the following TCBs*/
    lte_bs_phy_nmm_byPassMultTcb(nmm_tcb_arr[3], 4+NUM_SYMB_PBCH, BYPASS_TCB);
	if ((Ndl_symb * 2 + 4 + NUM_SYMB_PBCH) < N_PBCH_TCB)
		lte_bs_phy_nmm_byPassMultTcb(nmm_tcb_arr[3] + Ndl_symb * 2 + 4+NUM_SYMB_PBCH, N_PBCH_TCB - (Ndl_symb * 2 + 4+NUM_SYMB_PBCH), BYPASS_TCB);
    nmm_runTaskList(3);

    switch(Ntx_ante){
        case 1:
        case 2:
            N_RS_symb_time = 4;
            break;
        case 4:
            N_RS_symb_time = 6;
            break;
    };

    Rx_RS_symbol = MemAlloc(sizeof(S16) * 2 * N_fft * N_RS_symb_time, CRAM_MEM, 1);

    memcpy(Rx_RS_symbol, current_cell->symb_info[0].start + current_cell->symb_info[0].cp_size * 2, sizeof(S16) * 2 * N_fft);
    memcpy(Rx_RS_symbol + 2 * N_fft, current_cell->symb_info[Ndl_symb-3].start + current_cell->symb_info[Ndl_symb-3].cp_size * 2, sizeof(S16) * 2 * N_fft);
    memcpy(Rx_RS_symbol + 2 * N_fft * 2, current_cell->symb_info[Ndl_symb].start + current_cell->symb_info[Ndl_symb].cp_size * 2, sizeof(S16) * 2 * N_fft);
    memcpy(Rx_RS_symbol + 2 * N_fft * 3, current_cell->symb_info[Ndl_symb*2-3].start + current_cell->symb_info[Ndl_symb*2-3].cp_size * 2, sizeof(S16) * 2 * N_fft);

    if(Ntx_ante==4)
    {
        memcpy(Rx_RS_symbol + 2 * N_fft * 4, current_cell->symb_info[1].start + current_cell->symb_info[1].cp_size * 2, sizeof(S16) * 2 * N_fft);
        memcpy(Rx_RS_symbol + 2 * N_fft * 5, current_cell->symb_info[Ndl_symb+1].start + current_cell->symb_info[Ndl_symb+1].cp_size * 2, sizeof(S16) * 2 * N_fft);
    }

    foeCur = MemAlloc(sizeof(S32) * 2, CRAM_MEM, 1);

    lte_bs_phy_nmm_channel_estimate_tcb(pTskLst, Rx_RS_symbol, current_cell->cell_subframe << 1,
		current_cell->cell_pci, current_cell->cell_cp, Ndl_rb, expo_nfft,
		&chEst_cram[0], Ntx_ante,&current_cell->inv_noise_var, foeCur,
        nmm_tcb_arr[3][0]);
    //!!! for now just use the list of PBCH
    /*bypass the following TCBs*/
    lte_bs_phy_nmm_byPassMultTcb(nmm_tcb_arr[3]+1, (N_PBCH_TCB-1), BYPASS_TCB);
    nmm_runTaskList(3);

	if(pListening->state == NMM_SYNC_SEARCHING_DONE)
	{
		if (*foeCur == 0)
			current_cell->cell_detect_num -= 2;

		current_cell->cell_freqency_offset += *foeCur >> 1;
	}

	if ((current_cell->sfn % 20) == 0)
		nmm_dbg_printf_off(("state %s, timingOffset pre %d, current %d \r\n", NMMStateDbg[pListening->state], current_cell->cell_time_offset >> 4, *(foeCur+1)));
	if (*(foeCur+1) > 16)
		*(foeCur+1) = 16;
	else if (*(foeCur+1) < -16)
		*(foeCur+1) = -16;

	if ((pListening->state == NMM_SYNC_SEARCHING_DONE) || (pListening->state == NMM_PBCH_SEARCHING) || (pListening->state == NMM_PBCH_SEARCHING_DONE))
		current_cell->cell_time_offset -= (*(foeCur+1) + 8);	// advance OFDM symbol window by 8 sample%

    MemFree(Rx_RS_symbol, CRAM_MEM);
    MemFree(foeCur, CRAM_MEM);
    Rx_RS_symbol = NULL;
	foeCur = NULL;
}
//-------------------------------------------------------------------------------------------
/** @brief implement PBCH extraction
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param phy_instance    PHY instance ID
 *  @param channelEsti_buf Pointer to channel estimation buffer
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length      |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |            |               |           |            |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |phy_instance |         |Input          |1           |Real           |(0:32:0)   |Full range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |channelEsti_buf|       |Input          |variable    |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_extract_pbch      |
 *  --------------------------------------------------------
 *  |Input Streams       |channelEst_buf		       |
 *  --------------------------------------------------------
 *  |Output Streams      |None				   |
 *  --------------------------------------------------------
 *  |Config Parameters   |phy_instance   |
 *  --------------------------------------------------------
 *  |Status Parameters   |none		                       |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to extract pbch from time-domain OFDM symbol
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 * <6> References:
 * 3GPP 36.211
 *
 *  \ingroup group_lte_nmm_MIB 
 *
 **/
void
lte_bs_phy_nmm_extract_pbch_loop(U32 phy_instance,S16 *channelEsti_buf)
{
    PLTEPHYDESC     pphy = getPhyCtxApiProc(phy_instance);
    PLISTENINGDESC  pListening = LteBsGetNmmCtx();
    LISTENING_CELL_SEARCH_DESC *pcell_search = pListening->pcell_search;
    CELL_PARAMS    *current_cell = pcell_search->cell_param;
    SYMBOL_INFO    *psymb_info = current_cell->symb_info;
    U32             i;
    U32             npbch_to_extract = 0;
    volatile U32    nmm_ticks;

    nmm_ticks = GET_TICKS;

#if NMM_CEVA_DBG
    arm_dbg = MemAlloc(sizeof(S8) * 256, CRAM_MEM, 1);
#endif

    if (!pcell_search->nb_detected_cell)
        return;


    /* get cell matching wanted PCI */
    i = pcell_search->nb_detected_cell;
    if (pListening->search_timing_for_all_cells)
        psymb_info = current_cell->symb_info;
    else
        psymb_info = pcell_search->cell_param[0].symb_info;

    do {

        if (current_cell->cell_pci == pListening->pbch_pci) {

            if (current_cell->cell_subframe == PBCH_SUBFRAME)
			{
				/* implement derotation estimation*/

				while (psymb_info->start != NULL) {
					U16             symb_index = psymb_info->symb_index;

					nmm_dbg_printf_off(("symb_start_array %x current_sym_length %d [%d/%d]\n", psymb_info->start, psymb_info->len, current_cell->cell_subframe, psymb_info->symb_index));

					/* tmp work-around - should never happen */
					if (npbch_to_extract == NUM_SYMB_PBCH)
						break;

					if (current_cell->cell_subframe == PBCH_SUBFRAME) {
						U16             pbch_start_sym = (current_cell->cell_cp == SHORT_CP) ? SYMBOL(PBCH_SYM_START_SHORT_CP) : SYMBOL(PBCH_SYM_START_LONG_CP);
						U16             is_pbch_sym_0_1 = (symb_index == pbch_start_sym) || (symb_index == pbch_start_sym + 1) ||
							((symb_index == pbch_start_sym + 3)&&(current_cell->cell_cp == LONG_CP));
						U16             is_pbch_sym_2_3 = (symb_index == pbch_start_sym + 2) || ((symb_index == pbch_start_sym + 3)&&(current_cell->cell_cp == SHORT_CP));
						S16            *pbch_sym = &current_cell->pbch_buf[2 * current_cell->pbch_buf_idx];
						S16            *expo_fac = &current_cell->pbch_scale[current_cell->pbch_sym_idx];

						if (is_pbch_sym_0_1) {
							nmm_dbg_printf_off(("PBCH is here %x current_sym_length %d [%d/%d] expo @%x (%x, %d)\n", psymb_info->start, psymb_info->len, current_cell->cell_subframe,
												psymb_info->symb_index, expo_fac, current_cell->pbch_scale, current_cell->pbch_sym_idx));
							lte_bs_phy_nmm_extract_pbch_tcb(nmm_task_list[3],
								pphy->pBs->expoNfft, psymb_info, current_cell->cell_pci,
								current_cell->cell_cp, pbch_sym, channelEsti_buf, expo_fac,
								current_cell->cell_antennas,current_cell->inv_noise_var,
								nmm_tcb_arr[3][1+npbch_to_extract++]);
							current_cell->pbch_buf_idx += PBCH_NB_RB * (PRB_12 - 4);
							current_cell->pbch_sym_idx++;
						}
						else if (is_pbch_sym_2_3) {
							nmm_dbg_printf_off(("PBCH is here %x current_sym_length %d [%d/%d] expo @%x (%x, %d)\n", psymb_info->start, psymb_info->len, current_cell->cell_subframe,
												psymb_info->symb_index, expo_fac, current_cell->pbch_scale, current_cell->pbch_sym_idx));
							lte_bs_phy_nmm_extract_pbch_tcb(nmm_task_list[3],
								pphy->pBs->expoNfft, psymb_info, current_cell->cell_pci,
								current_cell->cell_cp, pbch_sym, channelEsti_buf, expo_fac,
								current_cell->cell_antennas,current_cell->inv_noise_var,
								nmm_tcb_arr[3][1+npbch_to_extract++]);
							current_cell->pbch_buf_idx += PBCH_NB_RB * PRB_12;
							current_cell->pbch_sym_idx++;
						}

					}

					psymb_info++;
				}//psymb info loop
			}
        }

        current_cell++;
    } while (--i);

    /*bypass the unused TCBs*/
    lte_bs_phy_nmm_byPassMultTcb(nmm_tcb_arr[3], 1, BYPASS_TCB);
    lte_bs_phy_nmm_byPassMultTcb(nmm_tcb_arr[3]+1+npbch_to_extract, (N_PBCH_TCB-1-npbch_to_extract), BYPASS_TCB);
    nmm_runTaskList(3);

#if NMM_CEVA_DBG
    nmm_dbg_printf_off(("%s\n", arm_dbg));
    MemFree(arm_dbg, CRAM_MEM);
#endif


    MLogTask(PID_NMM_MIB_PBCH_EXTRACT, RESOURCE_LARM, nmm_ticks, GET_TICKS);
}

//--------------------------------------------------------------------------------------------------------------------------
/** @brief main function of PBCH searching processing 
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param phyInstance      PHY instance ID
 *  @param sigIn            input IQ data
 *  @param outMsg           pointer to output message
 *  @param outMsgSize       pointer to length of output message
 *  @return None
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |phyInstance    |               |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |sigIn          |               |input        |variable       |pComplex Array|1:0:15    |Full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |outMsg        |                |output       |variable       |pReal         |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |outMsgSize    |                |output       |1              |pReal         |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_pbch_searching      |
 *  ---------------------------------------
 *  |Input Stream(s)     |sigIn |
 *  ---------------------------------------
 *  |Output Stream(s)    |outMsg|
 *  ---------------------------------------
 *  |Config Parameter(s) |phyInstance|
 *  ---------------------------------------
 *  |Status Parameter(s) |outMsgSize |
 *  ---------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * main function of PBCH searching processing
 * call PBCH searching TCB to do the PBCH searching
 * generate PBCH DATA INDICATION message to higher layer
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * 
 * 
 *
 * \ingroup group_lte_nmm_MIB
 *
 *
**/
//--------------------------------------------------------------------------------------------------------------------------

void
lte_bs_phy_nmm_pbch_searching(U32 phyInstance, S16 * sigIn, U8* outMsg, U32* outMsgSize)
{
    PLTEPHYDESC     pPhy = getPhyCtxApiProc(phyInstance);
    PLISTENINGDESC  pListening = LteBsGetNmmCtx();
    U32             i;
    LISTENING_CELL_SEARCH_DESC *pcell_search = pListening->pcell_search;
    CELL_PARAMS    *current_cell;
    U16            N_tx_ante_index;
    U16             pbch_dec_success = 0;
    S16             channelEsti_buf[2400*MAX_N_TX_ANTE];

    if (!pcell_search || (pcell_search && pcell_search->nb_detected_cell == 0))
        return;


    /* get cell matching wanted PCI */
    current_cell = pcell_search->cell_param;
    i = 0;

    while (i++ < pcell_search->nb_detected_cell) {
        if ((current_cell->cell_pci == pListening->pbch_pci) && (current_cell->cell_subframe == PBCH_SUBFRAME)) {
            S16            *pbch = NULL;

            if(pListening->state == NMM_PBCH_SEARCHING)
            {
                current_cell->cell_antennas = MAX_N_TX_ANTE;
            }
            lte_bs_phy_nmm_channel_estimate_loop(NULL, current_cell, PBCH_NB_RB, pPhy->pBs->expoNfft, channelEsti_buf);
			if (pListening->state != NMM_PBCH_SEARCHING)
			{
				return;
			}

            pbch_dec_success = 0;

			for(N_tx_ante_index=0;N_tx_ante_index<=2;N_tx_ante_index++)
			{
				current_cell->cell_antennas = 1<<N_tx_ante_index;

	            lte_bs_phy_nmm_extract_pbch_loop(phyInstance,channelEsti_buf);

	            /* check if we have enough data for doing decoding */
                pbch = &current_cell->pbch_buf[2*(current_cell->pbch_buf_idx-current_cell->pbch_re_per_frame)];

                if (current_cell->pbch_buf_idx >= current_cell->pbch_re_per_frame*PBCH_NB_FRAME)
                    current_cell->pbch_buf_idx = 0;
                /* handle wrap */
                current_cell->pbch_sym_idx &= (PBCH_SYM_PER_FRAME * PBCH_NB_FRAME - 1);

	            DUMP_ARR_ID_UART(pbch, 2 * PBCH_RE_PER_FRAME, "d", 0, 1);

                if (pbch)
                {
                    if(lte_bs_phy_nmm_decode_pbch(current_cell, pbch))
                    {
                        pbch_dec_success = 1;
                        break;
                    }
                }
            }

            if(pbch_dec_success==0)
                current_cell->mib_decoded_failed_cnt++;


            /* we've successfully decoded enough MIB, send indication */
            if (current_cell->mib_decoded_success_cnt == MAX_MIB_SUCCESS_DECODED) {
                U8              APImsg[NMM_PBCH_DATA_IND_API_SIZE+1];
                U16            *p16;

                p16 = (U16 *) APImsg;
                *p16 = current_cell->cell_pci;
                APImsg[2] = MIB_OK;
                APImsg[3] = current_cell->cell_antennas;
                p16 = (U16 *) & APImsg[4];
                *p16 = current_cell->sfn;
                APImsg[6] = current_cell->phich_duration;
                APImsg[7] = current_cell->phich_resource;
                APImsg[8] = current_cell->dl_tx_bw;
				APImsg[9] = 0;	// nr_vendorSpecificList

                pListening->pbch_search_cell_index = (U16)i-1;
                lte_bs_phy_nmm_generate_api_msg(phyInstance, APImsg, NMM_PBCH_DATA_IND_API_SIZE + 1, (U8 *) pListening->api_output_msg, NMM_PBCH_DATA_IND, 0);


#define MTX_DEBUG 0
#if MTX_DEBUG
				pListening->state = NMM_SIB1_SEARCHING;
                pListening->sib1_retry_count = 1;
				pListening->sib_report_flag = 1;
				pListening->sib_end_time[0] = 80 * 4;
#else
                pListening->state = NMM_PBCH_SEARCHING_DONE;
#endif
                current_cell->mib_decoded_failed_cnt = 0;

                return;
            }

            /* unsuccessful in MIB decoding for 1 sec (MAX_MIB_FAILED_DECODED) */
            if (current_cell->mib_decoded_failed_cnt == MAX_MIB_FAILED_DECODED) {
                U8              APImsg[NMM_PBCH_DATA_IND_API_SIZE+1];
                U16            *p16;

                p16 = (U16 *) APImsg;
                *p16 = current_cell->cell_pci;
                APImsg[2] = MIB_FAILED;
                APImsg[3] = 0;
                p16 = (U16 *) & APImsg[4];
                *p16 = 0;
                APImsg[6] = 0;
                APImsg[7] = 0;
                APImsg[8] = 0;
				APImsg[9] = 0;	// nr_vendorSpecificList

                lte_bs_phy_nmm_generate_api_msg(phyInstance, APImsg, NMM_PBCH_DATA_IND_API_SIZE + 1, (U8 *) pListening->api_output_msg, NMM_PBCH_DATA_IND, 0);

                lte_bs_phy_nmm_cell_search_clear();
				pListening->state = NMM_IDLE;
                return;
            }

            return;
        }

        current_cell++;
    };
}




//--------------------------------------------------------------------------------------------------------------------------
/** @brief main function of SIB searching processing 
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param phyInstance      PHY instance ID
 *  @param sigIn            input IQ data
 *  @param outMsg           pointer to output message
 *  @param outMsgSize       pointer to length of output message
 *  @return None
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |phyInstance    |               |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |sigIn          |               |input        |variable       |pComplex Array|1:0:15    |Full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |outMsg        |                |output       |variable       |pReal         |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |outMsgSize    |                |output       |1              |pReal         |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_sib1_searching      |
 *  ---------------------------------------
 *  |Input Stream(s)     |sigIn |
 *  ---------------------------------------
 *  |Output Stream(s)    |outMsg|
 *  ---------------------------------------
 *  |Config Parameter(s) |phyInstance|
 *  ---------------------------------------
 *  |Status Parameter(s) |outMsgSize |
 *  ---------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * main function of SIB searching processing
 * call SIB decoding TCB to the SIB decoding
 * generate BCCH DATA INDICATION message to higher layer
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * 
 * 
 *
 * \ingroup group_lte_nmm_SIB
 *
 *
**/
//--------------------------------------------------------------------------------------------------------------------------

void
lte_bs_phy_nmm_sib1_searching(U32 phyInstance, S16 * sigIn, U8* outMsg, U32* outMsgSize)
{
#define MAX_CFI_VALUE   3
    PLTEPHYDESC     pPhy = getPhyCtxApiProc(phyInstance);
    PLISTENINGDESC  pListening = LteBsGetNmmCtx();
    U32             i;
    LISTENING_CELL_SEARCH_DESC *pcell_search = pListening->pcell_search;
    CELL_PARAMS    *current_cell;
    SYMBOL_INFO    *psymb_info;
    U8             *sib;
    S16            *pdsch;
    S16             slot_number;
    U16             symb_index;
    S16             channelEsti_buf[2400*MAX_N_TX_ANTE];
    volatile U32    nmm_ticks;
    U8              *current_decoded_sib_packed=NULL;
    U16             SIB1_subframe;
    U16             sfn,subsfn,total_subsfn,mod_subsfn;
    U16             sib_x,n_sib = 0,read_sib2x;
    U16             cfi_tries[MAX_CFI_VALUE+1]={0,1,2,3},n_cfi;
	U16				trackingFlag = 0;

    if (!pcell_search || (pcell_search && pcell_search->nb_detected_cell == 0)) {
        return;
    }

    current_cell = pcell_search->cell_param + pListening->pbch_search_cell_index;
    sfn = current_cell->sfn;
    subsfn = current_cell->cell_subframe;

	if (!pListening->sib_report_flag)
	{
        U8              api_msg[NMM_BCCH_DATA_IND_API_SIZE];

		if (pListening->sib_report_cnt != pListening->nr_si_periodicity_list)
		{
			api_msg[0] = 0;     /* 0 SIB1 message sent */
			api_msg[1] = 0;		/* nr_vendorSpecificList */

			lte_bs_phy_nmm_generate_api_msg(phyInstance, api_msg, 2, (U8 *) pListening->api_output_msg,
											NMM_BCCH_DATA_IND, NMM_SIB_INCOMPLETE);

			nmm_dbg_printf(("send BCCH ind with NMM_SIB_INCOMPLETE ..."));
		}

		if(current_cell->BCCH_type==0)
			nmm_dbg_printf(("SIB1 decode finished\r\n"));
		else
			nmm_dbg_printf(("BCCH decode finished\r\n"));

		current_cell->sib_decoded_success_cnt = 0;
		current_cell->sib_decoded_failed_cnt = 0;
		pListening->state = NMM_TRACKING;
		return;
	}

    SIB1_subframe = (((sfn % 2) == 0) && (subsfn==5));
    if(current_cell->BCCH_type==0)
    {
        if(SIB1_subframe == 0)
        {
            trackingFlag = 1;
        }
		n_sib = 0;
    }
    else if(current_cell->BCCH_type==1)
    {
        read_sib2x = 0;

        if(SIB1_subframe == 0)
        {
            total_subsfn = sfn*10+subsfn;
            for(n_sib=0;n_sib<pListening->nr_si_periodicity_list;n_sib++)
            {
				if (pListening->sib_report_flag & (1 << n_sib))
				{
					sib_x = pListening->si_index_list[n_sib] * pListening->si_window;
					mod_subsfn = total_subsfn % pListening->si_periodicity_list[n_sib];
					if((mod_subsfn >= sib_x) && (mod_subsfn < (sib_x +pListening->si_window)))
					{
						current_cell->SI_subfrm_counter = mod_subsfn - sib_x;
						read_sib2x = 1;
						break;
					}
                }

            }
        }

        if(read_sib2x == 0)
            trackingFlag = 1;

    }
    if (pListening->search_timing_for_all_cells)
        psymb_info = current_cell->symb_info;
    else
        psymb_info = pcell_search->cell_param[0].symb_info;

        nmm_dbg_printf_off(("[%s] sfn %d\n", __FUNCTION__, current_cell->sfn));


    /* implement derotation estimation*/
	if (trackingFlag == 1)
	{
		if (current_cell->cell_subframe == 0)
		{
			lte_bs_phy_nmm_channel_estimate_loop(NULL, current_cell, PBCH_NB_RB, pPhy->pBs->expoNfft, channelEsti_buf);
		}

		return;
	}

			lte_bs_phy_nmm_channel_estimate_loop(NULL, current_cell, current_cell->dl_tx_bw, pPhy->pBs->expoNfft, channelEsti_buf);

        /*
         * CFI @symbol0
         */

        /*
         * read & decode PCFICH if not done
         * Need to have at least one MIB decoded to have cell bw to compute PCFICH assignement
         * Notes : Probably need to remove this as CFI can vary in time !!!
         */
        nmm_ticks = GET_TICKS;
        if (!current_cell->cfi_decoded) {
            S16            *expo_fac;
            S16            *pcfi_sym;

            nmm_dbg_printf_off(("PCFICH_SYM is here %x current_sym_length %d [%d/%d] mib %d bw %d\n", psymb_info->start, psymb_info->len, current_cell->cell_subframe,
                                psymb_info->symb_index, current_cell->mib_decoded_success_cnt, current_cell->dl_tx_bw));

            expo_fac = MemAlloc(sizeof(S16), CRAM_MEM, 1);
            pcfi_sym = MemAlloc(2 * PCFICH_NUM_RE * sizeof(S16), CRAM_MEM, 1);

            /* get PCFICH RE index */
            lte_bs_phy_nmm_get_pcfi_idx(current_cell);
            lte_bs_phy_nmm_extract_pcfi_tcb(NULL, pPhy->pBs->expoNfft, current_cell->dl_tx_bw,
				psymb_info, current_cell->pcfi_re_idx, channelEsti_buf, pcfi_sym,
				expo_fac,current_cell->cell_antennas,current_cell->inv_noise_var,
				nmm_tcb_arr[4][NMM_EXTRACT_PCFI_TCB]);

            lte_bs_phy_nmm_byPassMultTcb(nmm_tcb_arr[4]+NMM_EXTRACT_PCFI_TCB+1, N_PDSCH_tcb-(NMM_EXTRACT_PCFI_TCB+1), BYPASS_TCB);
            nmm_runTaskList(4);

            /* expect basic channel compensation to go here */
            DUMP_ARR_ID_UART(pcfi_sym, (2 * PCFICH_NUM_RE), "d", 0, 1);

            lte_bs_phy_nmm_decode_pcfi(current_cell, pcfi_sym, current_cell->cell_subframe * 2,
                *expo_fac);

            MemFree(expo_fac, CRAM_MEM);
            MemFree(pcfi_sym, CRAM_MEM);
        }
        MLogTask(PID_NMM_SIB1_PCFI, RESOURCE_LARM, nmm_ticks, GET_TICKS);

        if (current_cell->cfi_decoded) {

            cfi_tries[0] = current_cell->cfi_val;
            cfi_tries[current_cell->cfi_val]=0;


            for (n_cfi = 0; n_cfi < MAX_CFI_VALUE + 1; n_cfi ++)
            {

                /*
                 * once PCFI is decoded, get PHICH location prior to PDCCH decoding
                 * no need to decode HI
                 */
                S16             len;
                S16            *k_l_row0 = current_cell->phich_re_idx;
                S16            *k_l_row1 = current_cell->phich_sym_idx;
                nmm_ticks = GET_TICKS;
                nmm_dbg_printf_off(("PHICH_SYM is here %x current_sym_length %d [%d/%d] mib %d bw %d\n", psymb_info->start, psymb_info->len, current_cell->cell_subframe,
                                    psymb_info->symb_index, current_cell->mib_decoded_success_cnt, current_cell->dl_tx_bw));

                psymb_info = current_cell->symb_info;
                symb_index = psymb_info->symb_index;

                current_cell->cfi_val = (U8)cfi_tries[n_cfi];
                if((current_cell->pdcch_decoded) || (current_cell->sib_decoded_success_cnt>0))
                    break;

                /* get PHICH RE index */
                len = lte_bs_phy_nmm_get_phich_idx(current_cell);
                DUMP_ARR_ID_UART(k_l_row0, len, "d", 0, 1);
                DUMP_ARR_ID_UART(k_l_row1, len, "d", 0, 1);
                MLogTask(PID_NMM_SIB1_PHICH_IDX, RESOURCE_LARM, nmm_ticks, GET_TICKS);

                /*
                 * PDCCH starts @symbol0
                 */
                while (symb_index <= current_cell->cfi_val) {
                    S16            *pdcch_idx;      // [MAXSUBCARRIERS]; /* 2 dimensions because can be spread on 1,2 or 3 symbols */
                    S16             pdcch_re;
                    S16            *pdcch_sym;
                    S16            *expo_fac;
                    nmm_ticks = GET_TICKS;
                    nmm_dbg_printf_off(("PDCCH is here %x current_sym_length %d [%d/%d] cfi %d\n", psymb_info->start, psymb_info->len, current_cell->cell_subframe, psymb_info->symb_index,
                                        current_cell->cfi_val));

                    /* get PDCCH RE index (need only to be done when CFI changes) */
                    pdcch_idx = (S16 *) MemAlloc(MAXSUBCARRIERS * sizeof(S16), CRAM_MEM, 1);
                    pdcch_re = lte_bs_phy_nmm_get_pdcch_idx(current_cell, symb_index, pdcch_idx);
                    DUMP_ARR_ID_UART(pdcch_idx, pdcch_re, "d", 0, 1);
                    MLogTask(PID_NMM_SIB1_PDCCH_IDX, RESOURCE_LARM, nmm_ticks, GET_TICKS);

                    nmm_ticks = GET_TICKS;
                    current_cell->pdcch_re_num[SYMBOL(symb_index)] = pdcch_re;
                    expo_fac = MemAlloc(sizeof(S16), CRAM_MEM, 1);
                    pdcch_sym = (S16 *) MemAlloc(2 * pdcch_re * sizeof(S16), CRAM_MEM, 1);

                    /* Then PDCCH data of this symbol is extracted on the pdcch_idx indices */
                    lte_bs_phy_nmm_extract_pdcch_tcb(NULL, pPhy->pBs->expoNfft,
					    current_cell->dl_tx_bw, psymb_info, pdcch_idx, pdcch_re,
					    channelEsti_buf, pdcch_sym, expo_fac,current_cell->cell_antennas,
                        current_cell->inv_noise_var,
					    nmm_tcb_arr[4][NMM_EXTRACT_PDCCH_TCB]);

                    lte_bs_phy_nmm_byPassMultTcb(nmm_tcb_arr[4], NMM_EXTRACT_PDCCH_TCB, BYPASS_TCB);
                    lte_bs_phy_nmm_byPassMultTcb(nmm_tcb_arr[4]+NMM_EXTRACT_PDCCH_TCB+1, (N_PDSCH_tcb-NMM_EXTRACT_PDCCH_TCB-1), BYPASS_TCB);
                    nmm_runTaskList(4);

                    current_cell->pdcch_buf[SYMBOL(symb_index)] = pdcch_sym;
                    current_cell->pdcch_scale[SYMBOL(symb_index)] = expo_fac;
                    current_cell->pdcch_index[SYMBOL(symb_index)] = (U16*)pdcch_idx;
                    current_cell->pdcch_extracted_cnt++;

                    /* expect PDCCH channel compensation to go here */
                    MLogTask(PID_NMM_SIB1_PDCCH_EXTRACT, RESOURCE_LARM, nmm_ticks, GET_TICKS);

                    /* continue next symbol processing */
                    psymb_info++;
                    symb_index = psymb_info->symb_index;
                }


                /*
                 * Here we decode the PDCCH (blind search because many possible locations),
                 * which gives us a dci_message (type 1A or 1C) containing the location of PDSCH
                 */
                lte_bs_phy_nmm_decode_pdcch(current_cell, current_cell->cell_subframe * 2);

                for (i = 0; i <= current_cell->cfi_val; i++) {
                    MemFree(current_cell->pdcch_buf[SYMBOL(i)], CRAM_MEM);
                    MemFree(current_cell->pdcch_scale[SYMBOL(i)], CRAM_MEM);
                    MemFree(current_cell->pdcch_index[SYMBOL(i)], CRAM_MEM);
                }

                current_cell->cfi_decoded = 0;
                /*
                 * PDSCH follows CFI
                 */
                if (current_cell->pdcch_decoded) {
                    U16             num_pdsch_symb = 0;
                    U16             num_resource_blocks;
                    S16            *expo_fac;
                    S16            *curr_expo_fac;
                    S16            *curr_pdsch;
                    U16            max_N_pdsch_re;
                    U16             subsfn;

                    lte_bs_phy_nmm_decode_dci(current_cell);
                    num_resource_blocks = current_cell->num_resource_blocs;
                    max_N_pdsch_re = num_resource_blocks * PRB_12;

                    nmm_ticks = GET_TICKS;

                    /* get number of pdsch symbols (using current cfi interpretation, i.e. val = 0..3) */
                    num_pdsch_symb = (current_cell->cell_cp == SHORT_CP) ? SYM_PER_SUBFR_SHORT_CP - (current_cell->cfi_val + 1) : SYM_PER_SUBFR_LONG_CP - (current_cell->cfi_val + 1);
                    pdsch = MemAlloc(num_pdsch_symb * 2 * num_resource_blocks * PRB_12 * sizeof(S16), CRAM_MEM, 1);
                    expo_fac = MemAlloc(num_pdsch_symb * sizeof(S16), CRAM_MEM, 1);
                    current_decoded_sib_packed = (U8 *) MemAlloc(2048, DDR_MEM, 1);

                    curr_pdsch = pdsch;
                    curr_expo_fac = expo_fac;
                    current_cell->pdsch_buf = pdsch;
                    subsfn = current_cell->cell_subframe;

                    for (i = 0; i < num_pdsch_symb; i++) {

                        lte_bs_phy_nmm_extract_pdsch_tcb(nmm_task_list[4],
						    pPhy->pBs->expoNfft, current_cell->dl_tx_bw, psymb_info,
						    num_resource_blocks, channelEsti_buf, curr_pdsch, curr_expo_fac,
                            current_cell->cell_antennas,current_cell->PDSCH_PRB_set,
						    current_cell->cell_cp,symb_index,current_cell->cell_pci,
						    &(current_cell->N_pdsch_re_symb[symb_index - 1]),current_cell->inv_noise_var,
						    nmm_tcb_arr[4][NMM_EXTRACT_PDSCH_TCB+i],subsfn);

					    DUMP_ARR_ID_UART(curr_pdsch, 48, "d", 0, 1);

                        curr_pdsch += max_N_pdsch_re*2;


                        /* continue next symbol processing */
                        psymb_info++;
                        symb_index = psymb_info->symb_index;
                    }

                    MLogTask(PID_NMM_SIB1_PDSCH_EXTRACT, RESOURCE_LARM, nmm_ticks, GET_TICKS);

                    /*bypass the TCBs*/
                    lte_bs_phy_nmm_byPassMultTcb(nmm_tcb_arr[4], NMM_EXTRACT_PDSCH_TCB, BYPASS_TCB);
                    if(num_pdsch_symb<MAX_NUM_PDSCH_SYMB)
                    {
                        lte_bs_phy_nmm_byPassMultTcb(nmm_tcb_arr[4]+NMM_EXTRACT_PDSCH_TCB+num_pdsch_symb, (MAX_NUM_PDSCH_SYMB-num_pdsch_symb), BYPASS_TCB);
                    }

                    nmm_runTaskList(4);

                    /* get last symbol info */
                    nmm_ticks = GET_TICKS;
                    psymb_info--;
                    symb_index = psymb_info->symb_index;
                    sib = (U8 *) MemAlloc((current_cell->transport_block_size + CRC24A_NBITS) * sizeof(U8), CRAM_MEM, 1);
                    slot_number = current_cell->cell_subframe * 2;

                    lte_bs_phy_nmm_decode_pdsch(current_cell, pPhy->pBs->expoNfft, symb_index,
                        current_cell->dl_tx_bw, psymb_info, slot_number, sib,current_decoded_sib_packed);

                    MemFree(pdsch, CRAM_MEM);
                    MemFree(expo_fac, CRAM_MEM);
                    MemFree(sib, CRAM_MEM);
                    MLogTask(PID_NMM_SIB1_PDSCH_DECODE, RESOURCE_LARM, nmm_ticks, GET_TICKS);

                    /* we've successfully decoded enough SIB, send indication */
                    if (current_cell->sib_decoded_success_cnt == MAX_SIB_SUCCESS_DECODED) {
                        U8              api_msg[NMM_BCCH_DATA_IND_API_SIZE];
                        U16            *p16;
                        U8             *p8;

                        nmm_dbg_printf(("\r\n====  SIB index %d decoded ====\r\n", n_sib));

                        api_msg[0] = 1;     /* 1 SIB1 message sent */
                        p16 = (U16 *) & api_msg[1];
                        *p16++ = current_cell->transport_block_size >> 3; /* BCCH payload length in byte*/
                        *p16 = current_cell->sfn;   /* radio frame # */
                        api_msg[5] = (U8)current_cell->cell_subframe;       /* subframe */
                        p8 = &api_msg[6];
                        memcpy(p8, current_decoded_sib_packed, current_cell->transport_block_size >> 3);
					    p8 += current_cell->transport_block_size >> 3;
					    *p8 = 0;	 /* nr_vendorSpecificList */

                        lte_bs_phy_nmm_generate_api_msg(phyInstance, api_msg, 7 + (current_cell->transport_block_size >> 3), (U8 *) pListening->api_output_msg,
                                                        NMM_BCCH_DATA_IND, 0);

                        DUMP_ARR_ID_UART(api_msg, 7 + (current_cell->transport_block_size >> 3), "x", 0, 0);

                        current_cell->sib_decoded_success_cnt = 0;
						pListening->sib_report_flag &= ~(1 << n_sib);
						pListening->sib_report_cnt++;
                    }
                    MemFree(current_decoded_sib_packed,DDR_MEM);
                } // PDCCH decoded
                else
                {
                    current_cell->sib_decoded_failed_cnt ++;
                }
            }
        }// CFI decoded
        else
        {
            current_cell->sib_decoded_failed_cnt ++;
        }

	pListening->sib_end_time[n_sib]--;
	if (pListening->sib_end_time[n_sib] <= 0)
	{
		pListening->sib_report_flag &= ~(1 << n_sib);
		pListening->sib_end_time[n_sib] = 0;
	}

#if 0
    if(current_cell->sib_decoded_failed_cnt == MAX_SIB_FAILED_DECODED) {
        U8              api_msg[NMM_BCCH_DATA_IND_API_SIZE];

        api_msg[0] = 0;     /* 0 SIB1 message sent */
		api_msg[1] = 0;		/* nr_vendorSpecificList */

        lte_bs_phy_nmm_generate_api_msg(phyInstance, api_msg, 2, (U8 *) pListening->api_output_msg,
                                        NMM_BCCH_DATA_IND, 0);

        current_cell->sib_decoded_success_cnt = 0;
        current_cell->sib_decoded_failed_cnt = 0;
        lte_bs_phy_nmm_cell_search_clear();
        pListening->state = NMM_IDLE;
    }
#endif
}



void
lte_bs_phy_nmm_iq_subframe_copy_done(void)
{
    nmm_iq_subframe_copy_done = 1;
    return;
}
//--------------------------------------------------------------------------------------------------------------------------
/** @brief update the subframe number and frame number  
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param N_delta_subframe  number of missed subframes
 *  @return None
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |N_delta_subframe|              |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_update_subframe_number      |
 *  ---------------------------------------
 *  |Input Stream(s)     |None |
 *  ---------------------------------------
 *  |Output Stream(s)    |None|
 *  ---------------------------------------
 *  |Config Parameter(s) |N_delta_subframe |
 *  ---------------------------------------
 *  |Status Parameter(s) |None |
 *  ---------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * update the subframe number and frame number  based on the number of ommitted subframes
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * 
 * 
 *
 * \ingroup group_lte_nmm_thread
 *
 *
**/
//--------------------------------------------------------------------------------------------------------------------------

void lte_bs_phy_nmm_update_subframe_number(U32 N_delta_subframe)
{
    PLISTENINGDESC pListening = LteBsGetNmmCtx();
    U32 n_cell;
    U32 N_total_subframe;

    if(pListening->pcell_search != NULL)
    {
        for(n_cell=0;n_cell<pListening->pcell_search->nb_detected_cell;n_cell++)
        {
            N_total_subframe = pListening->pcell_search->cell_param[n_cell].cell_subframe + N_delta_subframe;

            pListening->pcell_search->cell_param[n_cell].cell_subframe = N_total_subframe%10;

            if(pListening->pcell_search->cell_param[n_cell].mib_decoded_success_cnt == MAX_MIB_SUCCESS_DECODED)
            {
                pListening->pcell_search->cell_param[n_cell].sfn = pListening->pcell_search->cell_param[n_cell].sfn +
                    ((U16)(N_total_subframe/10));

                if(pListening->pcell_search->cell_param[n_cell].sfn >= 1024)
                {
                    pListening->pcell_search->cell_param[n_cell].sfn -= 1024;
                }
            }

        }
    }
}
//--------------------------------------------------------------------------------------------------------------------------
/** @brief calculate the missed number of subframes and update the frame number and sub frame number 
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param N_buf_subframe  number of buffered subframes of IQ data
 *  @return None
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |N_buf_subframe |               |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_update_subframe_count      |
 *  ---------------------------------------
 *  |Input Stream(s)     |None |
 *  ---------------------------------------
 *  |Output Stream(s)    |None|
 *  ---------------------------------------
 *  |Config Parameter(s) |N_buf_subframe |
 *  ---------------------------------------
 *  |Status Parameter(s) |None |
 *  ---------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * calculate the missed number of subframes and update the frame number and sub frame number 
 * this is needed because the NMM is working in a buffering and processing mode
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * 
 * 
 *
 * \ingroup group_lte_nmm_thread
 *
 *
**/
//--------------------------------------------------------------------------------------------------------------------------

void lte_bs_phy_nmm_update_subframe_count(U32 N_buf_subframe)
{
    /*one of the buffered subframes is not processed*/
    PLISTENINGDESC pListening = LteBsGetNmmCtx();
    S32 N_delta_subframe = nmm_subframe_count - (S32)(N_buf_subframe - 1) -
        pListening->last_buf_subframe_count - 1;

    if(N_delta_subframe < 0)
    {
        N_delta_subframe += MAX_NMM_SUBFRAME_COUNT;
    }

	N_delta_subframe += 2;
    lte_bs_phy_nmm_update_subframe_number(N_delta_subframe);
    pListening->sniffing_time += N_delta_subframe;
	pListening->sniffing_time &= 0x7FFF;

    pListening->last_buf_subframe_count = nmm_subframe_count;

	if(pListening->state == NMM_SYNC_SEARCHING)
	{
		memset(pListening->pcell_search->xcorr_arr, 0, 3 * SUBFR_SIZE_096MHZ * 5 * sizeof(S16));
		pListening->pcell_search->xcorr_idx = 0;
	}
}
//--------------------------------------------------------------------------------------------------------------------------
/** @brief check whether BCCH is present in the current subframe 
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param proc_flag    pointer to processing flag
 *  @return None
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |proc_flag      |               |output       |1              |Real          |0:16:0    |[0,2^16-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_check_proc_flag      |
 *  ---------------------------------------
 *  |Input Stream(s)     |None |
 *  ---------------------------------------
 *  |Output Stream(s)    |None|
 *  ---------------------------------------
 *  |Config Parameter(s) |phyInstance,ptr,len |
 *  ---------------------------------------
 *  |Status Parameter(s) |None |
 *  ---------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * check whether BCCH is present in the current subframe
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * 
 * 
 *
 * \ingroup group_lte_nmm_thread
 *
 *
**/
//--------------------------------------------------------------------------------------------------------------------------

void lte_bs_phy_nmm_check_proc_flag(U16 *proc_flag)
{
    PLISTENINGDESC pListening = LteBsGetNmmCtx();
    CELL_PARAMS    *current_cell;
    U16             SIB1_subframe;
    U16             sfn,subsfn,total_subsfn,mod_subsfn;
    U16             sib_x,n_sib,read_sib2x;


    *proc_flag = 1;
    if(pListening->state == NMM_FULL_BCCH_SEARCHING)
    {
        current_cell = pListening->pcell_search->cell_param + pListening->pbch_search_cell_index;
        sfn = current_cell->sfn;
        subsfn = (current_cell->cell_subframe+1);

        if(subsfn == 10)
        {
            sfn ++;
            subsfn = 0;
        }


        SIB1_subframe = (((sfn % 2) == 0) && (subsfn==5));

        read_sib2x = 0;

        if(SIB1_subframe == 0)
        {
            total_subsfn = sfn*10+subsfn;
            for(n_sib=0;n_sib<pListening->nr_si_periodicity_list;n_sib++)
            {
                sib_x = pListening->si_index_list[n_sib] * pListening->si_window;
                mod_subsfn = total_subsfn % pListening->si_periodicity_list[n_sib];
                if((mod_subsfn >= sib_x) && (mod_subsfn < (sib_x +pListening->si_window)))
                {
                    read_sib2x = 1;
                    break;
                }

            }
        }

        if(read_sib2x == 0)
            *proc_flag = 0;
    }

    if(*proc_flag == 0)
        lte_bs_phy_nmm_update_subframe_number(1);

	pListening->sniffing_time++;
	pListening->sniffing_time &= 0x7FFF;
}

//--------------------------------------------------------------------------------------------------------------------------
/** @brief main function of NMM 
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param phyInstance      PHY instance ID
 *  @param sigIn            input IQ data
 *  @param outMsg           pointer to output message
 *  @param outMsgSize       pointer to length of output message
 *  @return None
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |phyInstance    |               |input        |1              |Real          |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |sigIn          |               |input        |variable       |pComplex Array|1:0:15    |Full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |outMsg        |                |output       |variable       |pReal         |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------ 
 *  |outMsgSize    |                |output       |1              |pReal         |0:32:0    |[0,2^32-1]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_main      |
 *  ---------------------------------------
 *  |Input Stream(s)     |sigIn |
 *  ---------------------------------------
 *  |Output Stream(s)    |outMsg|
 *  ---------------------------------------
 *  |Config Parameter(s) |phyInstance|
 *  ---------------------------------------
 *  |Status Parameter(s) |outMsgSize |
 *  ---------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * main function of NMM
 * it's called every subframe in the interrupt of the IQ data;
 * based on the internal state of NMM(RSSI measurement, cell search, PBCH searching,
 * SIB decoding etc.) it calls corresponding processing function
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * 
 * 
 *
 * \ingroup group_lte_nmm_thread
 *
 *
**/
//--------------------------------------------------------------------------------------------------------------------------
void
lte_bs_phy_nmm_main(U32 phyInstance, S16 * sigIn, U32* outMsg, U32* outMsgSize)
{
    PLTEPHYDESC     pPhy;
    PLISTENINGDESC  pListening = LteBsGetNmmCtx();
    U32             subframe_size;
    S16            *sig_in;
    int             decimation_factor;
    volatile U32    t_global;
    volatile U32    nmm_ticks;
#if !defined WIN32 && !defined CEVA_INTRINSICS
    PMDMA_FDESC     pMdma;
#endif

    nmm_dbg_printf_off(("@ %s\tARM %d\tsniffing_time %d [%s]\ttime %lu sigin %x\n", __FUNCTION__, MxGetCpuID(), pListening->sniffing_time, NMMStateDbg[pListening->state],
                        (unsigned long) GET_TICKS, sigIn));

#if defined RT_API_PROC
    pPhy = getPhyCtxListProc(phyInstance);
#else
    pPhy = getPhyCtxApiProc(phyInstance);
#endif


    _ASSERT_PTR(pPhy);
    subframe_size = 1 * 15 * pPhy->pBs->Nfft;
    decimation_factor = pPhy->pBs->Nfft >> 6;

    t_global = GET_TICKS;
    nmm_ticks = GET_TICKS;

#if NMM_HEAP_LEAKAGE_CONTROL
    UINT32          ddr_num = MxHeapGetAllocNum(AppHeap),
        ddr_num_nmm_diff;
    UINT32          ddr1_num = MxHeapGetAllocNum(DDR1Heap),
        ddr1_num_nmm_diff;
    UINT32          ddr1_ncnb_num = MxHeapGetAllocNum(DDR1NcnbHeap),
        ddr1_ncnb_num_nmm_diff;
    UINT32          cram_num = MxHeapGetAllocNum(DspHeap),
        cram_num_nmm_diff;
    UINT32          ncnb_num = MxHeapGetAllocNum(NcnbHeap),
        ncnb_num_nmm_diff;
#endif
    if (!sigIn)
        return;

    sig_in = (S16 *) MemAlloc(2 * SUBFR_SIZE_096MHZ * decimation_factor * sizeof(S16), CRAM_MEM, 0);
    DUMP_ARR_ID_UART(sigIn, 10, "d", 0, 0);

#if defined WIN32 || defined CEVA_INTRINSICS
    nmm_iq_subframe_copy_done = 1;
    memcpy(sig_in, sigIn, 2 * SUBFR_SIZE_096MHZ * decimation_factor * sizeof(S16));
#else
    pMdma = MdmaGetSingleDesc(MDMA_HW_SYS);
    _ASSERT_PTR(pMdma);
    MdmaSetCopy(pMdma);
    MdmaSetData(pMdma, sigIn, 2 * SUBFR_SIZE_096MHZ * decimation_factor * sizeof(S16), sig_in, 2 * SUBFR_SIZE_096MHZ * decimation_factor * sizeof(S16));
    nmm_iq_subframe_copy_done = 0;
    MdmaRunEx(MDMA_HW_SYS, pMdma, (void (*)(LPVOID)) lte_bs_phy_nmm_iq_subframe_copy_done, NULL);
#endif
    MLogTask(PID_NMM_MAIN_PRE_TASKS, RESOURCE_LARM, nmm_ticks, GET_TICKS);

	/* expect to be called every ms */
	switch (pListening->state) {
		case NMM_RSSI_MEASURING:
			lte_bs_phy_nmm_rssi_measure(phyInstance, sig_in, (U8*)outMsg, outMsgSize);
			break;

		case NMM_RSSI_MEASURING_DONE:
			// lte_bs_phy_nmm_generate_api_msg(phyInstance, pListening->api_output_msg, sizeof(U8) + sizeof(U16), NMM_RSSI_MEAS_IND, 0);
			pListening->state = NMM_IDLE;
			break;

		case NMM_PBCH_SYNC_SEARCHING:
			/* PSS/SSS tracking */
			lte_bs_phy_nmm_cell_search(phyInstance, sig_in, (U8*)outMsg, outMsgSize);

			/* symbol tracking */
			lte_bs_phy_nmm_get_cell_symbol_info(phyInstance, sig_in);
			lte_bs_phy_nmm_check_cell_sync_during_pch(phyInstance);
			break;

		case NMM_SYNC_SEARCHING:
			nmm_ticks = GET_TICKS;

			/* PSS/SSS tracking */
			lte_bs_phy_nmm_cell_search(phyInstance, sig_in, (U8*)outMsg, outMsgSize);

			MLogTask(PID_NMM_SYNC_SEARCH_GLOBAL, RESOURCE_LARM, nmm_ticks, GET_TICKS);
			break;

		case NMM_SYNC_SEARCHING_DONE:
			lte_bs_phy_nmm_get_cell_symbol_info(phyInstance, sig_in);
			lte_bs_phy_nmm_cell_search_measure_loop(phyInstance, sig_in, (U8*)outMsg, outMsgSize);

			break;

		case NMM_IDLE:
			lte_bs_phy_nmm_get_cell_symbol_info(phyInstance, sig_in);
			break;

		case NMM_TRACKING:
		case NMM_PBCH_SEARCHING_DONE:
		case NMM_PBCH_SEARCHING:
			nmm_ticks = GET_TICKS;
			lte_bs_phy_nmm_get_cell_symbol_info(phyInstance, sig_in);

			/* PBCH tracking */
			lte_bs_phy_nmm_pbch_searching(phyInstance, sig_in, (U8*)outMsg, outMsgSize);
			MLogTask(PID_NMM_MIB_GLOBAL, RESOURCE_LARM, nmm_ticks, GET_TICKS);

			break;

		case NMM_SIB1_SEARCHING:
		case NMM_FULL_BCCH_SEARCHING:
			nmm_ticks = GET_TICKS;
			lte_bs_phy_nmm_get_cell_symbol_info(phyInstance, sig_in);

			/* SIB tracking */
			lte_bs_phy_nmm_sib1_searching(phyInstance, sig_in, (U8*)outMsg, outMsgSize);
			MLogTask(PID_NMM_SIB1_GLOBAL, RESOURCE_LARM, nmm_ticks, GET_TICKS);
			break;

		default:
			break;
	}

    nmm_ticks = GET_TICKS;
    /* keep last symbol + cp size */
#if defined WIN32 || defined CEVA_INTRINSICS
    memcpy(pListening->last_IQ_sym, &sig_in[2 * (subframe_size - MAX_SYMB_AND_CP_SIZE)], (2 * MAX_SYMB_AND_CP_SIZE) * sizeof(S16));
#else
    pMdma = MdmaGetSingleDesc(MDMA_HW_SYS);
    _ASSERT_PTR(pMdma);
    MdmaSetCopy(pMdma);
    MdmaSetData(pMdma, &sig_in[2 * (subframe_size - MAX_SYMB_AND_CP_SIZE)], (2 * MAX_SYMB_AND_CP_SIZE) * sizeof(S16), pListening->last_IQ_sym,
                (2 * MAX_SYMB_AND_CP_SIZE) * sizeof(S16));
    MdmaRunEx(MDMA_HW_SYS, pMdma, NULL, NULL);
#endif

    MemFree(sig_in, CRAM_MEM);


#if NMM_HEAP_LEAKAGE_CONTROL
    ddr_num_nmm_diff = MxHeapGetAllocNum(AppHeap) - ddr_num;
    ddr1_num_nmm_diff = MxHeapGetAllocNum(DDR1Heap) - ddr1_num;
    ddr1_ncnb_num_nmm_diff = MxHeapGetAllocNum(DDR1NcnbHeap) - ddr1_ncnb_num;
    cram_num_nmm_diff = MxHeapGetAllocNum(DspHeap) - cram_num;
    ncnb_num_nmm_diff = MxHeapGetAllocNum(NcnbHeap) - ncnb_num;

    phy_printf("// nmm heap leakage control: %d%d%d%d%d\n", ddr_num_nmm_diff, ddr1_num_nmm_diff, ddr1_ncnb_num_nmm_diff, cram_num_nmm_diff, ncnb_num_nmm_diff);
#endif

    MLogTask(PID_NMM_MAIN_POST_TASKS, RESOURCE_LARM, nmm_ticks, GET_TICKS);
    MLogTask(PID_NMM_MAIN_GLOBAL, RESOURCE_LARM, t_global, GET_TICKS);

}

#if NMM_MALLOC_TRACE
void           *
nmm_memalloc_trace(U32 size, U32 memId, U32 InitFlag)
{
    void           *ptr = 0;
    HANDLE          HeapHandle = DspHeap;

#if WIN_BLD || CEVA_BUILD

    ptr = malloc(size);         // Should be memory type dependent

#else
    int             addr = __return_address() - 4;

#ifdef MX_HEAP_LEAKAGE_CONTROL
    UINT32          _LR = MX_GET_RADDR;
#endif

    switch (memId) {
    case COMMON_MEM:
        ptr = malloc(size);
        break;

    case DDR_MEM:
        ptr = MX_HEAP_ALLOC(AppHeap, size);
        break;

    case DDR1_MEM:
        ptr = MX_HEAP_ALLOC(DDR1Heap, size);
        break;

    case DDR1_NCNB_MEM:
        ptr = MX_HEAP_ALLOC(DDR1NcnbHeap, size);
        break;

    case CRAM_MEM:
        ptr = MX_HEAP_ALLOC(DspHeap, size);
        break;

    case NCNB_MEM:
        ptr = MX_HEAP_ALLOC(NcnbHeap, size);
        break;

    default:
        {
            printf("Scheduler: unknown Memory type !!!\r");
        }
    }

    if (ptr == NULL) {
        PHEAP           pHeapObj;
        ICHEAP         *pHeap;

        switch (memId) {
        case DDR_MEM:
            HeapHandle = AppHeap;
            break;

        case DDR1_MEM:
            HeapHandle = DDR1Heap;
            break;

        case DDR1_NCNB_MEM:
            HeapHandle = DDR1NcnbHeap;
            break;

        case CRAM_MEM:
            HeapHandle = DspHeap;
            break;

        case NCNB_MEM:
            HeapHandle = NcnbHeap;
            break;
        }

        pHeapObj = MxiGetHeapDescr(MxInqContext(), HeapHandle);
        pHeap = (ICHEAP *) pHeapObj->HeapStorage;

        uart_printf("Can't allocate %d bytes: memId = %d (Free %d) LR=%X\n", size, memId, MxHeapGetFreeSpace(HeapHandle), addr);

        uart_printf("Can only provide %d bytes (%X)\r\n", MxHeapGetMaxFreeBlockSize(HeapHandle), pHeap->FreeBlock);

        if (SvsrLoggerGetMask())
            SvsrLoggerFlush();

        if (MLogGetMask() != 0)
            MLogPrint();

        while (1);
    }
#endif

#ifdef _WIN32
    if (ptr == NULL)
        _CrtDumpMemoryLeaks();
    _ASSERT(ptr != NULL);
#endif

#if 0
    if (ptr == 0) {
#if WIN_BLD || CEVA_BUILD
#ifdef _WIN32
        printf("Can't allocate memory !!!\n");
        // exit(1);
#else
        // exit(1);
#endif

#else
        printf("Can't allocate memory !!!\n");
        i = 1;
        while (i) {
            tmp = tmp;
        }
        return 0;
#endif
    }
#endif

    if (InitFlag == 1) {
        memset(ptr, 0, size);
    }
#if 0
#ifndef _WIN32
    MxiRestInt(irq);
#endif
#endif

#ifdef MX_HEAP_LEAKAGE_CONTROL
    phy_printf("  ->%s (%d, @%x, num %d)\n", __FUNCTION__, memId, ptr, MxHeapGetAllocNum(HeapHandle));
#endif
    return ptr;
}
void           *
nmm_memfree_trace(void *ptr, U32 memId)
{
    HANDLE          h = DspHeap;
#if WIN_BLD || CEVA_BUILD

    free(ptr);

#else
    switch (memId) {
    case COMMON_MEM:
        free(ptr);
        break;

    case DDR_MEM:
        h = AppHeap;
        MxHeapFree(AppHeap, ptr);
        break;

    case DDR1_MEM:
        h = DDR1Heap;
        MxHeapFree(DDR1Heap, ptr);
        break;

    case DDR1_NCNB_MEM:
        h = DDR1NcnbHeap;
        MxHeapFree(DDR1NcnbHeap, ptr);
        break;

    case CRAM_MEM:
        h = DspHeap;
        MxHeapFree(DspHeap, ptr);
        break;

    case NCNB_MEM:
        h = NcnbHeap;
        MxHeapFree(NcnbHeap, ptr);
        break;

    default:
        {
            S32             i = 1;
            while (i) {
                printf("Scheduler: unknown Memory type !!!\r");
            }
        }
    }

#endif
    phy_printf("  <-%s (%d, @%x, num %d)\n", __FUNCTION__, memId, ptr, MxHeapGetAllocNum(h));

    return;
}
#endif
