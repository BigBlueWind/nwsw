//-----------------------------------------------------------------------------
/** @file lte_bs_phy_nmm_thread.c
 * 
 * @brief NMM related thread processing 
 * @author Mindspeed Technologies
 *
 * COPYRIGHT&copy; 2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-----------------------------------------------------------------------------

/*======================== includes ==========================*/	
#include "typedef.h"
#include "ltephydef.h"
#include "prototypes.h"
#include "ext_phy.h"
#include "LtePhyL2Api.h"
#include "lte_bs_phy_nmm_fdd.h"
#include "lte_bs_phy_nmm_thread.h"
#include "globalstoragearrays.h"

/*======================== macros ==========================*/	
#define NMM_DISPATCH_THREAD_ARM ((MxGetARMID () == 0) ? 2 : 0)
// #define NMM_NUM_OF_SUBFR_TO_START_SYNC 10
#define NMM_NUM_OF_SUBFR_TO_START_SYNC      0

/*======================== variable declarations ==========================*/	
extern S16     *nmm_iq_storage;
extern U32      nmm_iq_storage_curr_idx;
extern U32      nmm_iq_storage_subfr_writer;
extern U32      nmm_iq_storage_subfr_reader;
extern U32      nmm_subframe_count;

int             nmm_force_immediate_process = 0;
U32             nmm_finished = 0;

NMM2PHY_QUEUE                 nmm_to_phy_queue;

volatile NCNBVAR U16 nmm_iq_subframe_store_done;

#ifdef RT_API_PROC

typedef struct _tagLTE_PHY_NMM_THREAD_CTX {
    HANDLE          hLtePhyThread;
    PLTEPHYDESC     pPhy;

} LTE_PHY_NMM_THREAD_CTX, *PLTE_PHY_NMM_THREAD_CTX;

static LTE_PHY_API_THREAD_CTX lte_bs_phy_nmm_api_ctx;
static LTE_PHY_NMM_THREAD_CTX LtePhyNmmCtx;

/*======================== function declarations ==========================*/
extern void lte_bs_phy_nmm_update_subframe_count(U32 N_buf_subframe);
extern void lte_bs_phy_nmm_check_proc_flag(U16 *proc_flag);

extern void PhyDiCopyIqNMM(void);

/*======================== function definitions ==========================*/	

LTE_PHY_NMM_THREAD_CTX *
lte_bs_phy_nmm_get_ctx(void)
{
    return &LtePhyNmmCtx;
}

LTE_PHY_API_THREAD_CTX *
lte_bs_phy_nmm_api_get_ctx(void)
{
     return &lte_bs_phy_nmm_api_ctx;
}




void
lte_bs_phy_nmm_reinit_hbfir(void)
{
    PLISTENINGDESC pListening = LteBsGetNmmCtx();
    S16            *hbfir_state_buf = pListening->pcell_search->hbfir_state_buf;

    memset(hbfir_state_buf, 0, (HBFIR_DECIMATION_LEN * MAX_HBFIR_DECIMATION_STAGE * 2) * sizeof(S16));
}




//--------------------------------------------------------------------------------------------------------------------------
/** @brief thread for NMM signal processing 
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param pParam       
 *  @return processing result status
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |pParam         |               |input        |variable       |pReal         |0:8:0     |Full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_thread      |
 *  ---------------------------------------
 *  |Input Stream(s)     |None |
 *  ---------------------------------------
 *  |Output Stream(s)    |None|
 *  ---------------------------------------
 *  |Config Parameter(s) |pParam |
 *  ---------------------------------------
 *  |Status Parameter(s) |None |
 *  ---------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * it's called every subframe by the IQ data interrupt
 * it bufferes the IQ data for a desired number of subframes
 * when enough IQ data is buffered it calls the NMM main function to process the buffered data
 * it also checks the missed(not processed) number of subframes during the processing
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

MXRC
lte_bs_phy_nmm_thread(LPVOID pParam)
{
    MXRC            rc = MX_OK;
    PLTEPHYDESC     pPhy;
    int             subframe_size;
    S16            *sigIn;
    U32             cur_idx = 0;
    int             i;
    U16             cellIdx,cellFound;
    U16             proc_flag;
    PUINT8 p_iq;
    PLISTENINGDESC pListening = LteBsGetNmmCtx();
    U32             N_buf_subframe = pListening->N_buf_subframe;

    pPhy = getPhyCtxListProc(0);
    p_iq = (PUINT8)pPhy->pBsRx->adcbuf[0]; /* FIXME (OS) - tmp, 1 antenna only */

    PhyDiCopyIqNMM();
    lte_bs_phy_nmm_fill_iq_buf(0, (S16 *) p_iq);


    pPhy = getPhyCtxListProc(0);
    _ASSERT_PTR(pPhy);
    subframe_size = 1 * 15 * pPhy->pBs->Nfft;

     if (pListening->state >= NMM_RSSI_MEASURING) {

        if (nmm_iq_storage_subfr_writer > NMM_NUM_OF_SUBFR_TO_START_SYNC || nmm_force_immediate_process) {
            int             item_to_process;

            if (nmm_iq_storage_subfr_reader > NMM_IQ_STORAGE_SIZE / (2 * subframe_size))
                nmm_iq_storage_subfr_reader = 0;

            item_to_process = (nmm_iq_storage_subfr_writer - nmm_iq_storage_subfr_reader);

            if (item_to_process == N_buf_subframe) 
            {
                /*update subframe number*/
                lte_bs_phy_nmm_update_subframe_count(N_buf_subframe);
                
                for (i = 1; i < (item_to_process-1); i++) 
                {
                    lte_bs_phy_nmm_check_proc_flag(&proc_flag);
                    if(proc_flag == 1)
                    {
                        cur_idx = nmm_iq_storage_subfr_reader * (2 * subframe_size);

                        sigIn = &nmm_iq_storage[cur_idx];
                        if (pListening->state > NMM_SYNC_SEARCHING)
                        {
                            if (pListening->state == NMM_SYNC_SEARCHING_DONE)
                            {
                                sigIn += (pListening->pcell_search->cell_param[pListening->dlrs_foe_cellIdx].cell_time_offset >> 4)*2;
                            }
                            else
                            {
                                //find the cell with the pci
                                cellFound=0;
                                for(cellIdx=0;cellIdx<pListening->pcell_search->nb_detected_cell;cellIdx++)
                                {
                                    if(pListening->pcell_search->cell_param[cellIdx].cell_pci==pListening->pbch_pci)
                                    {
                                        cellFound=1;
                                        break;
                                    }
                                }
                                if(cellFound==1)
                                {
                                   sigIn += (pListening->pcell_search->cell_param[cellIdx].cell_time_offset >> 4)*2;
                                }
                            }
                        }

                        lte_bs_phy_nmm_main(0, sigIn, NULL, NULL);
                    }
                    nmm_iq_storage_subfr_reader++;
                }// i
            
                //reset to the beginning
                nmm_iq_storage_curr_idx = 0;
                nmm_iq_storage_subfr_writer = 1;
                nmm_iq_storage_subfr_reader = 1;
                            
            }// buffer full            
        }
    }

    return rc;
}

//--------------------------------------------------------------------------------------------------------------------------
/** @brief creates the NMM signal processing thread 
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param None
 *  @return processing result status
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
 *  |Block Name          |BLOCK_NAME      |
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
 * creates the NMM signal processing thread
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

MXRC
lte_bs_phy_nmm_thread_init()
{
    MXRC            rc = MX_OK;
    PLTE_PHY_NMM_THREAD_CTX pThCtx = lte_bs_phy_nmm_get_ctx();

    _ASSERT_PTR(pThCtx);

    memset(pThCtx, 0, sizeof(LTE_PHY_NMM_THREAD_CTX));
    nmm_dbg_printf(("[%s]\tARM %d\ttime %lu\r\n", __FUNCTION__, MxGetCpuID(), (unsigned long) GET_TICKS));

    rc |= MxCreateThreadEx(0, lte_bs_phy_nmm_thread, NULL, pThCtx, 0, &pThCtx->hLtePhyThread);

    if (FAILED(rc)) {
        MxRemoveThread(pThCtx->hLtePhyThread);
        pThCtx->hLtePhyThread = HNULL;
    }

    return rc;
}

//--------------------------------------------------------------------------------------------------------------------------
/** @brief remove the NMM signal processing thread 
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param None
 *  @return processing result status
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
 *  |Block Name          |BLOCK_NAME      |
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
 * remove the NMM signal processing thread
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

MXRC
lte_bs_phy_nmm_thread_kill()
{
    MXRC            rc = MX_OK;
    PLTE_PHY_NMM_THREAD_CTX pThCtx = lte_bs_phy_nmm_get_ctx();

    if (HANDLE_NOT_NULL(pThCtx->hLtePhyThread))
    {
        MxRemoveThread(pThCtx->hLtePhyThread);
        pThCtx->hLtePhyThread = HNULL;
    }

    return rc;
}


void
lte_bs_phy_nmm_schedule_thread(void)
{
    PLTE_PHY_NMM_THREAD_CTX pThCtx = lte_bs_phy_nmm_get_ctx();
    PLTEPHYDESC     pPhy = getPhyCtxApiProc(0);

    _ASSERT((pThCtx != NULL));

    pThCtx->pPhy = pPhy;

    MxScheduleThreadEx(pThCtx->hLtePhyThread, 0, 0);
    return;
}


void
lte_bs_phy_nmm_force_end(void)
{
#if NMM_NUM_OF_SUBFR_TO_START_SYNC
    if (nmm_iq_storage_subfr_writer < NMM_NUM_OF_SUBFR_TO_START_SYNC)
#endif
        nmm_force_immediate_process = 1;
}

void
lte_bs_phy_nmm_wait4end(void)
{
    // while(!nmm_finished);
    phy_printf("lte_bs_phy_nmm_wait4end\r\n");
}

extern char           *NMMStateDbg[];


void
lte_bs_phy_nmm_iq_subframe_store_done(void)
{
    nmm_iq_subframe_store_done = 1;
    return;
}

U8 check_buffer_flag(PLISTENINGDESC pListening,U32 subframe_size)
{
    U8 start_dump = 0;
    U8 continue_buf_flag = 0;
    U32 N_dump_subframe = pListening->N_dump_subframe;

    if(pListening->dump_iq_data_mode == 0)
    {
        if(pListening->state >= NMM_RSSI_MEASURING)
            continue_buf_flag = 1;
    }
    else {
        if (pListening->dump_state == 0) {
            if(pListening->dump_iq_data_mode == 1) {
                start_dump = 1;
            } 
            else if (pListening->dump_iq_data_mode == 2) {
			    continue_buf_flag = 1;
                if(pListening->state == NMM_PBCH_SEARCHING_DONE) {
                    
                    CELL_PARAMS *current_cell = pListening->pcell_search->cell_param 
                        + pListening->pbch_search_cell_index;                

                    lte_bs_phy_nmm_update_subframe_count(0);                    

                    // not run the main program again                    
                    nmm_iq_storage_subfr_writer = 1;
                    nmm_iq_storage_subfr_reader = 0;
                                        
                    if(((current_cell->sfn == 1023)||(current_cell->sfn == 511)) && (current_cell->cell_subframe==0))
                        start_dump = 1;

                    pListening->N_buf_subframe = pListening->N_dump_subframe + 1;
                }
            }
            if (start_dump == 1) {
                nmm_dbg_printf(("\r\n ==== starting buffering IQ data ====== \r\n"));
                nmm_iq_storage_subfr_writer = 0;
                pListening->dump_state = 1;                  
                continue_buf_flag = 1;
            }
        }
        else if (pListening->dump_state == 1) {
            if(nmm_iq_storage_subfr_writer < (N_dump_subframe)) {
                continue_buf_flag = 1;
            }
            else
            {                                     
                nmm_dbg_printf(("\r\n ==== finished buffering IQ data ====== \r\n"));
                nmm_dbg_printf(("\n=== command for dump: rdmem r 0x%08x %d -o nmmIq.bin ======\r\n",(U32)nmm_iq_storage,N_dump_subframe*subframe_size*4));
               
                pListening->dump_state = 2;                
            }
        }        
    }

    return continue_buf_flag;

}
//--------------------------------------------------------------------------------------------------------------------------
/** @brief calls driver function to buffer the IQ data 
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param phyInstance          PHY instance ID
 *  @param sigIn                pointer to input signal
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
 *  |phyInstance    |               |input        |1              |Real          |0:32:0    |Full range     |              |
 *  |sigIn          |               |input        |variable       |pComplex array|1:0:15    |Full range     |              |    
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------
 *  |Block Name          |BLOCK_NAME      |
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
 * calls driver function to buffer the IQ data
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

void
lte_bs_phy_nmm_fill_iq_buf(U32 phyInstance, S16 * sigIn)
{
    PLTEPHYDESC     pPhy = getPhyCtxListProc(phyInstance);

    U32 tTotal = 0;
    U32             subframe_size = 1 * 15 * pPhy->pBs->Nfft;
    
    

    PMDMA_FDESC     pMdma;
    PLISTENINGDESC pListening = LteBsGetNmmCtx();

    nmm_dbg_printf_off(("@@@ [%d] idx %d lte_bs_phy_nmm_fill_iq_buf state [%s], phyctx %x, sigin %x, nmm_iq_storage %x\r\n", nmm_iq_storage_subfr_writer, nmm_iq_storage_curr_idx, NMMStateDbg[pListening->state], pPhy, sigIn, nmm_iq_storage));

    if (!sigIn)
        return;

    /* expect to be called every ms */
    if (check_buffer_flag(pListening,subframe_size) == 1) {
        

        DUMP_ARR_ID_UART(sigIn, 10, "d", 0, 1);

        if (nmm_iq_storage_curr_idx > NMM_IQ_STORAGE_SIZE) {
            nmm_iq_storage_curr_idx = 0;
            nmm_iq_storage_subfr_writer = 0;
        }


        tTotal = GET_TICKS;


        pMdma = MdmaGetSingleDesc(MDMA_HW_SYS);
        _ASSERT_PTR(pMdma);
        MdmaSetCopy(pMdma);
        MdmaSetData(pMdma, sigIn, 2 * subframe_size * sizeof(S16), &nmm_iq_storage[nmm_iq_storage_curr_idx], 2 * subframe_size * sizeof(S16));
        nmm_iq_subframe_store_done = 0;
        MdmaRunEx(MDMA_HW_SYS, pMdma, (void (*)(LPVOID)) lte_bs_phy_nmm_iq_subframe_store_done, NULL);
        while(nmm_iq_subframe_store_done==0);

        MLogTask(2017, RESOURCE_LARM, tTotal, GET_TICKS);

        nmm_iq_storage_curr_idx += 2 * subframe_size;
        nmm_iq_storage_subfr_writer++;
        nmm_dbg_printf_off(("@@@nmm_iq_storage_curr_idx = %d subfr %d subframe_size %d\r\n", nmm_iq_storage_curr_idx, nmm_iq_storage_subfr_writer, subframe_size));
    }
}

//--------------------------------------------------------------------------------------------------------------------------
/** @brief creates the NMM API thread 
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param None
 *  @return processing result status
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
 *  |Block Name          |BLOCK_NAME      |
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
 * creates the NMM API thread
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

MXRC
lte_bs_phy_nmm_init_api_thread(void)
{
    MXRC rc = MX_OK;
    PLTE_PHY_API_THREAD_CTX thctx = lte_bs_phy_nmm_api_get_ctx();

    _ASSERT_PTR(thctx);

    if (HANDLE_NOT_NULL(thctx->hLtePhyThread))
        return MX_OK;

    memset(thctx, 0, sizeof(LTE_PHY_API_THREAD_CTX));

    rc |= MxCreateThreadEx(API_DISPATCH_THREAD_ARM, lte_bs_phy_nmm_api_thread,
                           NULL,
                           thctx,
                           0,
                           &thctx->hLtePhyThread);

    if (FAILED(rc))
    {
        MxRemoveThread(thctx->hLtePhyThread);
        thctx->hLtePhyThread = HNULL;
    }

    return rc;
}


void
lte_bs_phy_nmm_schedule_api_thread(void)
{
    PLTE_PHY_API_THREAD_CTX thctx = lte_bs_phy_nmm_api_get_ctx();
    PLTEPHYDESC phy = getPhyCtxApiProc(0);

    _ASSERT((thctx != NULL));

    thctx->pPhy = phy;

    MxScheduleThreadEx(thctx->hLtePhyThread, 0, PHY_SMP_THREAD);

    return;
}


MXRC
lte_bs_phy_nmm_msg_handler(LPVOID client_ctx, LPVOID msg)
{// message comes without Transcede API header
    PLTE_PHY_API_THREAD_CTX thctx = lte_bs_phy_nmm_api_get_ctx();

    thctx->pMsg = (PMAC2PHY_QUEUE_EL)msg;

    lte_bs_phy_nmm_schedule_api_thread();

    return SVSR_MSG_OPT_SAVE;
}


MXRC
lte_bs_phy_nmm_api_handler(LPVOID pClientCtx, ApiHeader *pApi, ApiHeader *pResp)
{// message comes with Transcede API header
    PLTE_PHY_API_THREAD_CTX pThCtx = lte_bs_phy_nmm_api_get_ctx();
    ApiParam * pParam;
    PNMM2PHY_QUEUE_EL pElm;

// ------------------------------------------------------------------------------
    _ASSERT_PTR(pApi);
    pParam = ApiGetParamByID(pApi, LTE_NMM_LINKEDLIST_PARAM_ID);
    _ASSERT_PTR(pParam);

    if (pParam != NULL)
    {
        pThCtx->pMsg = (PMAC2PHY_QUEUE_EL)pParam->ParamData[0];
        _ASSERT_PTR(pThCtx->pMsg);

        // we need to invalidate the list of blocks

        pElm = (PNMM2PHY_QUEUE_EL)pThCtx->pMsg;

        while (pElm != NULL)
        {
            // to invalidate the list element
            MxCacheInvalidate(pElm, sizeof (NMM2PHY_QUEUE_EL));

            _ASSERT_PTR(pElm->message_ptr);

            if (pElm->message_ptr != NULL)
            {
                MxCacheInvalidate(pElm->message_ptr, pElm->message_len);
            }

            pElm = pElm->next;
        }

        if (GetUNmmInstanceId() == 0)
            SetUNmmInstanceId(pApi->SrcInstID);

        lte_bs_phy_nmm_schedule_api_thread();

        return APIHEC_DROP_RESP;
    }
    else
    {
        uart_printf("%s: ApiAddErrorCode\r\n", __FUNCTION__);
        ApiAddErrorCode(pApi, RC_LTE_NMM_PARAM_ERROR);
    }

    return APIHEC_OK;
}

extern U32 listeningActiveFlag;

U8 check_responde_API_flag(PLISTENINGDESC pListening)
{
    U8 respond_flag = 1;

    if(listeningActiveFlag == 1)
    {
        if(pListening->dump_state > 0)
        {        
             respond_flag = 0;
        }
        else if(pListening->dump_iq_data_mode == 2)
        {
            if(pListening->state == NMM_PBCH_SEARCHING_DONE)
            {
                respond_flag = 0;
            }
        }
    }

    return respond_flag ;

   
}
//--------------------------------------------------------------------------------------------------------------------------
/** @brief parse and process the API message sent from higher layer 
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param param   *
 *  @return processing result status
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |param          |               |input        |variable       |pReal         |0:8:0     |Full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------
 *  |Block Name          |BLOCK_NAME      |
 *  ---------------------------------------
 *  |Input Stream(s)     |None |
 *  ---------------------------------------
 *  |Output Stream(s)    |None|
 *  ---------------------------------------
 *  |Config Parameter(s) |param |
 *  ---------------------------------------
 *  |Status Parameter(s) |None |
 *  ---------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * it's called when an NMM API message is received
 * parse the API message sent from higher layer;
 * call corresponding API message processing function according to the message type;
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

UINT32
lte_bs_phy_nmm_api_thread(LPVOID param)
{
    PLTE_PHY_API_THREAD_CTX pThCtx = (PLTE_PHY_API_THREAD_CTX)param;
    PLISTENINGDESC pListening = LteBsGetNmmCtx();
    PLTEPHYDESC pPhyList, pPhyApi;
    U32 phyInstance = 0;
    PMAC2PHY_QUEUE_EL pMsg, curr, tmpElm;
    PGENMSGDESC pMsg1;
    U32* inMsg  = NULL;
    U32 status = 0;
    U32 *ptr;
    U16             len = 0;
    U32 tTotal = GET_TICKS;
    S16 nmm_cell_search_vector[20];
    U32 i;
    S16 *p16;
    p16 = (S16 *)nmm_cell_search_vector;
    *p16++ = 0;         /* pci */
    *p16++ = 1;         /* earfcn */

    _ASSERT(pThCtx != NULL);

    _my_sfn++;
    //gFreeQueueElement = 1;

    if(check_responde_API_flag(pListening) == 0)
        return 0;    

    pMsg = pThCtx->pMsg;
    curr = pMsg;

    while (curr)
    {
        // U8 * p8 = (U8 *)curr->MessagePtr;
        // U16 *p16;
        inMsg =  (U32*)curr->MessagePtr;
        pMsg1 = (PGENMSGDESC) inMsg;
        len = (U16) pMsg1->msgSpecific;
        ptr = inMsg + 1;

        SvsrLoggerAddMsgForce(pMsg1->msgType, LOGGER_MSG_INFO(0, 0, IID_LTE_MAC, IID_LTE_NMM), pMsg1, len + sizeof(GENMSGDESC));

 //       uart_printf("%s: pMsg1->msgType = %d\r\n", __FUNCTION__, pMsg1->msgType);

        switch (pMsg1->msgType) {
        case NMM_START: {
            fapi_nmm_start_desc_t *nmm_start_desc = (fapi_nmm_start_desc_t *)ptr;
            // U16 *p16 = (U16 *)nmm_start_desc;


            if (gMode == LTE_MODE)
            {// hope that PHY operation is stopped, reconfigure PHY before NMM start
                U32 OutSize = 0;

                LtePhyApiThreadStopTimer();

                // Clean up for next start
                gStartSubfrTimeMark = 0;
                gFirstMessageArrived = 0;
                LtePhyStartApiFree();

                // Shutdown PHY
                PhyDiShutdown(&OutSize);

                // Re-initialize the Phy
                gTTIPauseResumeCount = 0;

                // Process INIT_REQ
                uart_printf("\r\nRe-Configuring the PHY\r\n");
                uart_printf(    "----------------------\r\n\r\n");

                LteBsPhyInit(0, (PINITPARM)&gStoredInitParm);
            }

            MLogEnableRC (1);
            SvsrLoggerSetResetMarker();
            MLogSetMask(0xFFFFFFFF);

            /* set adcbuf pointers */
            pPhyList = getPhyCtxListProc(0);
            _ASSERT_PTR(pPhyList);
            pPhyApi =  getPhyCtxApiProc(0);
            _ASSERT_PTR(pPhyApi);
            for(i = 0; i < pPhyApi->pBsRx->Nrx_antennas; i++) {
                pPhyList->pBsRx->adcbuf[i] = (S16*)QueryCramIqBuf(UL, 0, i);
                pPhyApi->pBsRx->adcbuf[i] = (S16*)QueryCramIqBuf(UL, 1, i);
            }

            /* FIXME (OS) - tmp: using vendor_specific_list to pass lte start params */
            status = lte_bs_phy_nmm_start(phyInstance, (void *) ptr, len);
            pListening = LteBsGetNmmCtx();
            if (status)
                goto nmmStartResp;


            // p16 = (U16 *)nmm_start_desc->vendor_specific_list;
            memcpy(&pListening->phystart_req, nmm_start_desc->vendor_specific_list, sizeof(STARTREQ)); /* tmp - aligment to check on payload */
            gMode = NMM_MODE;
            pListening->di_mode = pListening->phystart_req.mode;
            status = LteStart(phyInstance, &pListening->phystart_req, 0);

            nmmStartResp:
            /*config response*/
            PhyDiSetPauseResume(0,0);
            lte_bs_phy_nmm_generate_api_msg(phyInstance, NULL, 0, (U8 *) pListening->api_output_msg,
                NMM_START_RESP, status);

//            uart_printf("\r\n%s: NMM_START: status = %d\r\n", __FUNCTION__, status);
            break;
        }

        case NMM_STOP:
            //uart_printf("%s: NMM_STOP\r\n", __FUNCTION__);
            LtePhyApiThreadStopTimer();
            pListening->received_nmm_stop_flag = 1;
            status = 0;
            break;

        case NMM_RSSI_MEAS_REQ:
            {
                status = lte_bs_phy_nmm_rssi_meas_req(phyInstance, (void *) ptr, len);
            }
            break;

        case NMM_STOP_RSSI_MEAS_REQ:
            {
                uart_printf("\r\n%s: NMM_STOP_RSSI_MEAS_REQ\r\n", __FUNCTION__);
                status = lte_bs_phy_nmm_stop_rssi_meas_req(phyInstance, (void *) ptr, len);
            }
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

            uart_printf("unsupported pMsg1->msgType = %d\n", pMsg1->msgType);
            status = FAILURE;
            break;
        }

        tmpElm = curr;
        curr = curr->Next;

        /*free the message and its message content*/
        SvsrFreeIMem(tmpElm->MessagePtr);
        SvsrFreeIMem(tmpElm);

    }

    MLogTask(PID_NMM_API_THREAD, RESOURCE_LARM, tTotal, GET_TICKS);

    return status;
}


#endif

