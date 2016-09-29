// -------------------------------------------------------------------------------------------
/** @file lte_bs_phy_nmm_tcb.c
 *
 * @brief TCB and task definitions for LTE NMM
 * @author Mindspeed Technologies
 * @version $Revision: 1.13 $
 *
 * COPYRIGHT&copy; 2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
// -------------------------------------------------------------------------------------------


#ifdef CEVA_INTRINSICS
#include "global.h"
#include <basic_op.h>
#include <asm-insn.h>
#endif

#include <stdio.h>
#include "typedef.h"
#include "ext_phy.h"
#include "lte_bs_phy_nmm_fdd.h"
#include "prototypes.h"


#ifndef _WIN32
#define USE_TCBQ 1
#endif

#if NMM_CEVA_DBG
char           *ceva_dbg;
#endif

extern S16             EXP_HSPHS[2 * FFT_SIZE_096MHZ];
extern S16             ZC25_FD[2 * FFT_SIZE_096MHZ];
extern S16             ZC29_FD[2 * FFT_SIZE_096MHZ];
extern S16             ZC34_FD[2 * FFT_SIZE_096MHZ];
extern S16             ZC25_TD[2 * FFT_SIZE_096MHZ];
extern S16             ZC29_TD[2 * FFT_SIZE_096MHZ];
extern S16             ZC34_TD[2 * FFT_SIZE_096MHZ];

extern S16             S0_REF[62];

#ifdef CEVA_INTRINSICS

extern U8      *DataBuff_Bank1,
               *DataBuff_Bank2;

#define EXTRACT_IO_BUF(tcb, ionum) EXTRACTTCBIOBUF((tcb), (ionum))

/*
 * Main TCB Parser
 */
void
lte_bs_phy_nmm_tcb_parser(U32 task_id, PTCB tcb)
{
    switch (task_id) {
    case TASKID_NMM_DOWNSAMPLING:
        lte_bs_phy_nmm_downsampling_task(tcb);
        break;

    case TASKID_NMM_AGC:
        lte_bs_phy_nmm_agc_task(tcb);
        break;

    case TASKID_NMM_XCORR_LOOP:
        lte_bs_phy_nmm_xcorr_loop_task(tcb);
        break;

    case TASKID_NMM_FIND_PSS_CAND:
        lte_bs_phy_nmm_pss_candidate_selection_loop_task(tcb);
        break;

    case TASKID_NMM_FFT64:
        lte_bs_phy_nmm_fft64_task(tcb);
        break;

    case TASKID_NMM_SSYNC_CHANEST:
        lte_bs_phy_nmm_chanest_task(tcb);
        break;

    case TASKID_NMM_DECODE_SSS:
        lte_bs_phy_nmm_decode_sss_task(tcb);
        break;

    case TASKID_NMM_GET_RSRP:
        lte_bs_phy_nmm_get_rsrp_task(tcb);
        break;

    case TASKID_NMM_PSEUDO_RANDOM_SEQ:
        lte_bs_phy_nmm_pseudo_random_seq_gen_task(tcb);
        break;

    case TASKID_NMM_EXTRACT_PBCH:
        lte_bs_phy_nmm_extract_pbch_task(tcb);
        break;

    case TASKID_NMM_DEMOD_PBCH:
        lte_bs_phy_nmm_demod_pbch_task(tcb);
        break;

    case TASKID_NMM_EXTRACT_PCFI:
        lte_bs_phy_nmm_extract_pcfi_task(tcb);
        break;

    case TASKID_NMM_EXTRACT_PDCCH:
        lte_bs_phy_nmm_extract_pdcch_task(tcb);
        break;

    case TASKID_NMM_EXTRACT_PDSCH:
        lte_bs_phy_nmm_extract_pdsch_task(tcb);
        break;

    case TASKID_NMM_PDCCH_DEMOD:
        lte_bs_phy_nmm_pdcch_demod_task(tcb);
        break;

    case TASKID_NMM_CHANNEL_EST:
        lte_bs_phy_nmm_channel_estimate_task(tcb);
        break;

    case TASKID_NMM_PSS_FOE:
        lte_bs_phy_nmm_pss_foe_task(tcb);
        break;

    case TASKID_NMM_FOE_COMP:
        lte_bs_phy_nmm_freqOffset_comp_task(tcb);
        break;

    default:
        break;
    }
}

#else

#if NMM_CEVA_DBG
extern char    *arm_dbg;
#endif

#define EXTRACT_IO_BUF(tcb, ionum) extractTCBIOBuf((tcb), (ionum))

/*
 * TCB Wrappers
 */
// //////////////////////////////////////////////////////////////////////////////////////
/** @brief lte_bs_phy_nmm_downsampling_tcb Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  ------------------------------------------------------
 *  |Task ID                          |Scheduled on      |
 *  |                                 |Hardware Resource |
 *  ------------------------------------------------------
 *  |TASKID_NMM_DOWNSAMPLING          |RESOURCE_MDMA     |
 *  ------------------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the NMM downsample process.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: lte_bs_phy_nmm_downsampling() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_nmm_cell_search 
 *
 **/
// -------------------------------------------------------------------------------------------
PTCB
lte_bs_phy_nmm_downsampling_tcb(PTASKLIST pTskLst, S16 * IQin, S32 nIQin, S16 * IQout, S32 factor,
                                S16 * hbfir_real_buf_stage0, S16 * hbfir_imag_buf_stage0,
                                S16 * hbfir_real_buf_stage1, S16 * hbfir_imag_buf_stage1,
                                S16 * hbfir_real_buf_stage2, S16 * hbfir_imag_buf_stage2,
                                S16 * hbfir_real_buf_stage3, S16 * hbfir_imag_buf_stage3, S16 * hbfir_real_buf_stage4, S16 * hbfir_imag_buf_stage4, U16 RefillFlag, PTCB refill_tcb)
{
    PTCB            tcb = NULL;
#ifdef SCHEDULER
    U32            *context = NULL;

    nmm_dbg_printf_off(("> %s... ", __FUNCTION__));

    if (RefillFlag == TCBIOBUFFILL)
#if !NMM_USE_TCB_LIST
        return tcb;
    if (1)
#endif
    {
#ifdef USECEVA
#ifdef USE_TCBQ
        tcb = RexCreateTCBQ(TASKID_NMM_DOWNSAMPLING, RESOURCE_CEVA, NULL, 2 * sizeof(U32), NULL, NULL,TIME_NMM_DOWNSAMPLING);
#else
        tcb = CreateTCB(TASKID_NMM_DOWNSAMPLING, RESOURCE_CEVA, NULL, 2 * sizeof(U32), NULL, NULL);
#endif

#else
        tcb = CreateTCB(TASKID_NMM_DOWNSAMPLING, RESOURCE_LARM, NULL, 2 * sizeof(U32), NULL, NULL);
        tcb->ResourceIndex = 0;
        RegARMTask(TASKID_NMM_DOWNSAMPLING, &lte_bs_phy_nmm_downsampling_task);
#endif

        context = (U32 *) tcb->ContextPtr;
        context[0] = nIQin;
        context[1] = factor;

        CreateTCBIOBuf(tcb, 12);
        FillTCBIOBuf(tcb, (U32 *) IQin, 2 * SUBFR_SIZE_096MHZ * factor * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) IQout, 2 * SUBFR_SIZE_096MHZ * sizeof(S16), OUT_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) hbfir_real_buf_stage0, HBFIR_DECIMATION_LEN * sizeof(S16), INOUT_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) hbfir_imag_buf_stage0, HBFIR_DECIMATION_LEN * sizeof(S16), INOUT_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) hbfir_real_buf_stage1, HBFIR_DECIMATION_LEN * sizeof(S16), INOUT_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) hbfir_imag_buf_stage1, HBFIR_DECIMATION_LEN * sizeof(S16), INOUT_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) hbfir_real_buf_stage2, HBFIR_DECIMATION_LEN * sizeof(S16), INOUT_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) hbfir_imag_buf_stage2, HBFIR_DECIMATION_LEN * sizeof(S16), INOUT_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) hbfir_real_buf_stage3, HBFIR_DECIMATION_LEN * sizeof(S16), INOUT_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) hbfir_imag_buf_stage3, HBFIR_DECIMATION_LEN * sizeof(S16), INOUT_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) hbfir_real_buf_stage4, HBFIR_DECIMATION_LEN * sizeof(S16), INOUT_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) hbfir_imag_buf_stage4, HBFIR_DECIMATION_LEN * sizeof(S16), INOUT_BUF, NULL);

#if NMM_USE_TCB_LIST
        AddTCBToList(tcb, pTskLst, NULL);
#else
        ExeTSchedSingle(tcb);
        tcb = NULL;
#endif
    }
    else {
        tcb = refill_tcb;
        context = (U32 *) tcb->ContextPtr;
        context[0] = nIQin;
        context[1] = factor;
        ReFillTCBIOBuf(tcb, (U32 *) IQin, 2 * SUBFR_SIZE_096MHZ * factor * sizeof(S16), IN_BUF, 0);
        ReFillTCBIOBuf(tcb, (U32 *) IQout, 2 * SUBFR_SIZE_096MHZ * sizeof(S16), OUT_BUF, 1);
        ReFillTCBIOBuf(tcb, (U32 *) hbfir_real_buf_stage0, HBFIR_DECIMATION_LEN * sizeof(S16), INOUT_BUF, 2);
        ReFillTCBIOBuf(tcb, (U32 *) hbfir_imag_buf_stage0, HBFIR_DECIMATION_LEN * sizeof(S16), INOUT_BUF, 3);
        ReFillTCBIOBuf(tcb, (U32 *) hbfir_real_buf_stage1, HBFIR_DECIMATION_LEN * sizeof(S16), INOUT_BUF, 4);
        ReFillTCBIOBuf(tcb, (U32 *) hbfir_imag_buf_stage1, HBFIR_DECIMATION_LEN * sizeof(S16), INOUT_BUF, 5);
        ReFillTCBIOBuf(tcb, (U32 *) hbfir_real_buf_stage2, HBFIR_DECIMATION_LEN * sizeof(S16), INOUT_BUF, 6);
        ReFillTCBIOBuf(tcb, (U32 *) hbfir_imag_buf_stage2, HBFIR_DECIMATION_LEN * sizeof(S16), INOUT_BUF, 7);
        ReFillTCBIOBuf(tcb, (U32 *) hbfir_real_buf_stage3, HBFIR_DECIMATION_LEN * sizeof(S16), INOUT_BUF, 8);
        ReFillTCBIOBuf(tcb, (U32 *) hbfir_imag_buf_stage3, HBFIR_DECIMATION_LEN * sizeof(S16), INOUT_BUF, 9);
        ReFillTCBIOBuf(tcb, (U32 *) hbfir_real_buf_stage4, HBFIR_DECIMATION_LEN * sizeof(S16), INOUT_BUF, 10);
        ReFillTCBIOBuf(tcb, (U32 *) hbfir_imag_buf_stage4, HBFIR_DECIMATION_LEN * sizeof(S16), INOUT_BUF, 11);
    }

#else
    nmm_dbg_printf_off(("> %s... ", __FUNCTION__));
    lte_bs_phy_nmm_downsampling(IQin, nIQin, IQout, factor, hbfir_real_buf_stage0, hbfir_imag_buf_stage0, hbfir_real_buf_stage1,
                                hbfir_imag_buf_stage1, hbfir_real_buf_stage2, hbfir_imag_buf_stage2, hbfir_real_buf_stage3, hbfir_imag_buf_stage3,
                                hbfir_real_buf_stage4, hbfir_imag_buf_stage4);
#endif

    nmm_dbg_printf_off(("done.\r\n"));

    return tcb;
}


// //////////////////////////////////////////////////////////////////////////////////////
/** @brief lte_bs_phy_nmm_agc_tcb Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  ------------------------------------------------------
 *  |Task ID                          |Scheduled on      |
 *  |                                 |Hardware Resource |
 *  ------------------------------------------------------
 *  |TASKID_NMM_AGC                   |RESOURCE_MDMA     |
 *  ------------------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the NMM AGC process to downsampling receive data.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: lte_bs_phy_nmm_ssync_agc() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_nmm_cell_search 
 *
 **/
// -------------------------------------------------------------------------------------------
PTCB
lte_bs_phy_nmm_agc_tcb(PTASKLIST pTskLst, S32 * agc_gain, S32 * agc_avg_nrg, S16 * dataPtr, S16 len, PTCB * dep_tcb, U16 RefillFlag, PTCB refill_tcb)
{
    PTCB            tcb = NULL;
#ifdef SCHEDULER
    U32            *context = NULL;

    nmm_dbg_printf_off(("> %s... ", __FUNCTION__));

    if (RefillFlag == TCBIOBUFFILL)
#if !NMM_USE_TCB_LIST
        return tcb;
    if (1)
#endif
    {
#ifdef USECEVA
#ifdef USE_TCBQ
        tcb = RexCreateTCBQ(TASKID_NMM_AGC, RESOURCE_CEVA, NULL, sizeof(U32), NULL, NULL,TIME_NMM_AGC);
#else
        tcb = CreateTCB(TASKID_NMM_AGC, RESOURCE_CEVA, NULL, sizeof(U32), NULL, NULL);
#endif

#else
        tcb = CreateTCB(TASKID_NMM_AGC, RESOURCE_LARM, NULL, sizeof(U32), NULL, NULL);
        tcb->ResourceIndex = 0;
        RegARMTask(TASKID_NMM_AGC, &lte_bs_phy_nmm_agc_task);
#endif

        context = (U32 *) tcb->ContextPtr;
        context[0] = len;

        CreateTCBIOBuf(tcb, 3);
        FillTCBIOBuf(tcb, (U32 *) agc_gain, 1 * sizeof(S32), INOUT_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) agc_avg_nrg, 1 * sizeof(S32), INOUT_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) dataPtr, 2 * len * sizeof(S16), INOUT_BUF, NULL);     //  2 * len

#if NMM_USE_TCB_LIST
        AddTCBToList(tcb, pTskLst, NULL);
        AddDependency(tcb, 1, dep_tcb[NMM_DOWNSAMPLING_TCB]);
#else
        ExeTSchedSingle(tcb);
        tcb = NULL;
#endif
    }
    else {
        tcb = refill_tcb;
        context = (U32 *) tcb->ContextPtr;
        context[0] = len;
        ReFillTCBIOBuf(tcb, (U32 *) agc_gain, 1 * sizeof(S32), INOUT_BUF, 0);
        ReFillTCBIOBuf(tcb, (U32 *) agc_avg_nrg, 1 * sizeof(S32), INOUT_BUF, 1);
        ReFillTCBIOBuf(tcb, (U32 *) dataPtr, 2 * len * sizeof(S16), INOUT_BUF, 2);      // 2 * len
    }

#else
    nmm_dbg_printf_off(("> %s... ", __FUNCTION__));
    lte_bs_phy_nmm_ssync_agc(agc_gain, agc_avg_nrg, dataPtr,  len);
#endif

    nmm_dbg_printf_off(("done.\r\n"));

    return tcb;
}


// //////////////////////////////////////////////////////////////////////////////////////
/** @brief lte_bs_phy_nmm_xcorr_loop_tcb Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  ------------------------------------------------------
 *  |Task ID                          |Scheduled on      |
 *  |                                 |Hardware Resource |
 *  ------------------------------------------------------
 *  |TASKID_NMM_XCORR_LOOP            |RESOURCE_MDMA     |
 *  ------------------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the PSS correlation.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: lte_bs_phy_nmm_xcorr_loop() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_nmm_cell_search 
 *
 **/
// -------------------------------------------------------------------------------------------
PTCB
lte_bs_phy_nmm_xcorr_loop_tcb(PTASKLIST pTskLst, LISTENING_CELL_SEARCH_DESC * pcell_search, S16 * IQdecimated, S16 * zc_cand, S16 * xcorr_arr, PTCB * dep_tcb, U16 RefillFlag,
                              PTCB refill_tcb)
{
    PTCB            tcb = NULL;
#ifdef SCHEDULER

    nmm_dbg_printf_off(("> %s... ", __FUNCTION__));

    if (RefillFlag == TCBIOBUFFILL)
#if !NMM_USE_TCB_LIST
        return tcb;
    if (1)
#endif
    {
#ifdef USECEVA
#ifdef USE_TCBQ
        tcb = RexCreateTCBQ(TASKID_NMM_XCORR_LOOP, RESOURCE_CEVA, NULL, 0, NULL, NULL,TIME_NMM_XCORR_LOOP);
#else
        tcb = CreateTCB(TASKID_NMM_XCORR_LOOP, RESOURCE_CEVA, NULL, 0, NULL, NULL);
#endif

#else
        tcb = CreateTCB(TASKID_NMM_XCORR_LOOP, RESOURCE_LARM, NULL, 0, NULL, NULL);
        tcb->ResourceIndex = 0;
        RegARMTask(TASKID_NMM_XCORR_LOOP, &lte_bs_phy_nmm_xcorr_loop_task);
#endif

        CreateTCBIOBuf(tcb, 5);
        if(NULL==pcell_search)
        {
            FillTCBIOBuf(tcb, (U32 *) NULL, 2 * (2 * FFT_SIZE_096MHZ + SSS_SEARCH_RANGE) * sizeof(S16), INOUT_BUF, NULL);
        }
        else
        {
            FillTCBIOBuf(tcb, (U32 *) pcell_search->cell_search_xcorr_buf, 2 * (2 * FFT_SIZE_096MHZ + SSS_SEARCH_RANGE) * sizeof(S16), INOUT_BUF, NULL);
        }
        FillTCBIOBuf(tcb, (U32 *) IQdecimated, (2 * SUBFR_SIZE_096MHZ) * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) zc_cand, (2 * FFT_SIZE_096MHZ) * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) xcorr_arr, SUBFR_SIZE_096MHZ * sizeof(S16), INOUT_BUF, NULL);
        if(NULL==pcell_search)
        {
			FillTCBIOBuf(tcb, (U32 *) NULL, 2 * sizeof(S32), IN_BUF, NULL);
        }
        else
        {
			FillTCBIOBuf(tcb, (U32 *) pcell_search->agc_gain, 2 * sizeof(S32), IN_BUF, NULL);
        }

#if NMM_USE_TCB_LIST
        AddTCBToList(tcb, pTskLst, NULL);
        AddDependency(tcb, 1, dep_tcb[NMM_AGC_TCB]);
#else
        ExeTSchedSingle(tcb);
        tcb = NULL;
#endif
    }
    else {
        tcb = refill_tcb;
        ReFillTCBIOBuf(tcb, (U32 *) pcell_search->cell_search_xcorr_buf, 2 * (2 * FFT_SIZE_096MHZ + SSS_SEARCH_RANGE) * sizeof(S16), INOUT_BUF, 0);
        ReFillTCBIOBuf(tcb, (U32 *) IQdecimated, (2 * SUBFR_SIZE_096MHZ) * sizeof(S16), IN_BUF, 1);
        ReFillTCBIOBuf(tcb, (U32 *) zc_cand, (2 * FFT_SIZE_096MHZ) * sizeof(S16), IN_BUF, 2);
        ReFillTCBIOBuf(tcb, (U32 *) xcorr_arr, SUBFR_SIZE_096MHZ * sizeof(S16), INOUT_BUF, 3);
        ReFillTCBIOBuf(tcb, (U32 *) pcell_search->agc_gain, 2 * sizeof(S32), IN_BUF, 4);
    }

#else
    nmm_dbg_printf_off(("> %s... ", __FUNCTION__));
    lte_bs_phy_nmm_xcorr_loop(pcell_search->cell_search_xcorr_buf, IQdecimated, zc_cand, xcorr_arr, pcell_search->agc_gain);
#endif

    nmm_dbg_printf_off(("done.\r\n"));

    return tcb;
}


// //////////////////////////////////////////////////////////////////////////////////////
/** @brief lte_bs_phy_nmm_pss_candidate_selection_loop_tcb Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  ------------------------------------------------------
 *  |Task ID                          |Scheduled on      |
 *  |                                 |Hardware Resource |
 *  ------------------------------------------------------
 *  |TASKID_NMM_FIND_PSS_CAND         |RESOURCE_MDMA     |
 *  ------------------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the PSS detection according to PSS correlation results.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: lte_bs_phy_nmm_pss_candidate_selection_loop() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_nmm_cell_search
 *
 **/
// -------------------------------------------------------------------------------------------
PTCB
lte_bs_phy_nmm_pss_candidate_selection_loop_tcb(PTASKLIST pTskLst, S16 * xcorr_arr, S16 * psync_found_arr, PTCB * dep_tcb, U16 RefillFlag,
                                                PTCB refill_tcb)
{
    PTCB            tcb = NULL;
#ifdef SCHEDULER

    nmm_dbg_printf_off(("> %s... ", __FUNCTION__));

    if (RefillFlag == TCBIOBUFFILL)
#if !NMM_USE_TCB_LIST
        return tcb;
    if (1)
#endif
    {
#ifdef USECEVA
#ifdef USE_TCBQ
        tcb = RexCreateTCBQ(TASKID_NMM_FIND_PSS_CAND, RESOURCE_CEVA, NULL, 0, NULL, NULL,TIME_NMM_FIND_PSS_CAND);
#else
        tcb = CreateTCB(TASKID_NMM_FIND_PSS_CAND, RESOURCE_CEVA, NULL, 0, NULL, NULL);
#endif

#else
        tcb = CreateTCB(TASKID_NMM_FIND_PSS_CAND, RESOURCE_LARM, NULL, 0, NULL, NULL);
        tcb->ResourceIndex = 0;
        RegARMTask(TASKID_NMM_FIND_PSS_CAND, &lte_bs_phy_nmm_pss_candidate_selection_loop_task);
#endif

        CreateTCBIOBuf(tcb, 2);
        FillTCBIOBuf(tcb, (U32 *) xcorr_arr, 3 * SUBFR_SIZE_096MHZ * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) psync_found_arr, 4 * MAX_CELL_FOUND_LIST * sizeof(S16), OUT_BUF, NULL);

#if NMM_USE_TCB_LIST
        AddTCBToList(tcb, pTskLst, NULL);
        AddDependency(tcb, 1, dep_tcb[NMM_PSS_XCORR0_TCB]);
        AddDependency(tcb, 1, dep_tcb[NMM_PSS_XCORR1_TCB]);
        AddDependency(tcb, 1, dep_tcb[NMM_PSS_XCORR2_TCB]);
#else
        ExeTSchedSingle(tcb);
        tcb = NULL;
#endif
    }
    else {
        tcb = refill_tcb;
        ReFillTCBIOBuf(tcb, (U32 *) xcorr_arr, 3 * SUBFR_SIZE_096MHZ * sizeof(S16), IN_BUF, 0);
        ReFillTCBIOBuf(tcb, (U32 *) psync_found_arr, 4 * MAX_CELL_FOUND_LIST * sizeof(S16), OUT_BUF, 1);
    }

#else
    nmm_dbg_printf_off(("> %s... ", __FUNCTION__));
    lte_bs_phy_nmm_pss_candidate_selection_loop(xcorr_arr, psync_found_arr);
#endif

    nmm_dbg_printf_off(("done.\r\n"));

    return tcb;
}


// //////////////////////////////////////////////////////////////////////////////////////
/** @brief lte_bs_phy_nmm_pss_foe_tcb Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  ------------------------------------------------------
 *  |Task ID                          |Scheduled on      |
 *  |                                 |Hardware Resource |
 *  ------------------------------------------------------
 *  |TASKID_NMM_PSS_FOE               |RESOURCE_MDMA     |
 *  ------------------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the frequency offset estimation based on PSS data.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: lte_bs_phy_nmm_pss_foe() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_nmm_cell_search 
 *
 **/
// -------------------------------------------------------------------------------------------
PTCB
lte_bs_phy_nmm_pss_foe_tcb(PTASKLIST pTskLst, S16 *pin, S16 PSS_root, S32 *PSync_foe, PTCB refill_tcb)
{
    PTCB            tcb = NULL;
    S16            *pZC;

    pZC = ZC25_TD;
    if (PSS_root == 1) {
        pZC = ZC29_TD;
    }
    if (PSS_root == 2) {
        pZC = ZC34_TD;
    }

#ifdef SCHEDULER

    nmm_dbg_printf_off(("> %s... ", __FUNCTION__));

    if(refill_tcb == NULL) // fill TCB
    {
#ifdef USECEVA
#ifdef USE_TCBQ
		tcb = RexCreateTCBQ(TASKID_NMM_PSS_FOE, RESOURCE_CEVA, NULL, 0, NULL, NULL,TIME_NMM_PSS_FOE);
#else
		tcb = CreateTCB(TASKID_NMM_PSS_FOE, RESOURCE_CEVA, NULL, 0, NULL, NULL);
#endif

#else
		tcb = CreateTCB(TASKID_NMM_PSS_FOE, RESOURCE_LARM, NULL, 0, NULL, NULL);
		tcb->ResourceIndex = 0;
		RegARMTask(TASKID_NMM_PSS_FOE, &lte_bs_phy_nmm_pss_foe_task);
#endif

		CreateTCBIOBuf(tcb, 3);
		FillTCBIOBuf(tcb, (U32 *) pin, 2 * (FFT_SIZE_096MHZ * 2 + 16) * sizeof(S16), INOUT_BUF, NULL);
		FillTCBIOBuf(tcb, (U32 *) pZC, 2 * FFT_SIZE_096MHZ * sizeof(S16), IN_BUF, NULL);
		FillTCBIOBuf(tcb, (U32 *) PSync_foe, sizeof(S32), OUT_BUF, NULL);
#if NMM_CEVA_DBG
		FillTCBIOBuf(tcb, (U32 *) arm_dbg, 256 * sizeof(char), OUT_BUF, NULL);
#endif

#if NMM_USE_TCB_LIST
    AddTCBToList(tcb, pTskLst, NULL);
#else
    ExeTSchedSingle(tcb);
    tcb = NULL;
#endif
	}
    else //refill
    {
        tcb = refill_tcb;
        ReFillTCBIOBuf(tcb, (U32 *) pin, 2 * (FFT_SIZE_096MHZ * 2 + 16) * sizeof(S16), INOUT_BUF, 0);
        ReFillTCBIOBuf(tcb, (U32 *) pZC, 2 * FFT_SIZE_096MHZ * sizeof(S16), IN_BUF, 1);
        ReFillTCBIOBuf(tcb, (U32 *) PSync_foe, sizeof(S32), OUT_BUF, 2);
    }


#else
    lte_bs_phy_nmm_pss_foe(pin, pZC, PSync_foe);
#endif

    nmm_dbg_printf_off(("done.\r\n"));

    return tcb;
}


// //////////////////////////////////////////////////////////////////////////////////////
/** @brief lte_bs_phy_nmm_fft64_tcb Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  ------------------------------------------------------
 *  |Task ID                          |Scheduled on      |
 *  |                                 |Hardware Resource |
 *  ------------------------------------------------------
 *  |TASKID_NMM_FFT64                 |RESOURCE_MDMA     |
 *  ------------------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the NMM 64-point FFT for PSS and SSS symbol.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: cfft_16t32d_case2() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_nmm_cell_search 
 *
 **/
// -------------------------------------------------------------------------------------------
PTCB
lte_bs_phy_nmm_fft64_tcb(PTASKLIST pTskLst, S16 * pin, S16 * freq, S16 * exp, PTCB refill_tcb)
{
    PTCB            tcb = NULL;
#ifdef SCHEDULER

    nmm_dbg_printf_off(("> %s... ", __FUNCTION__));

    if(refill_tcb == NULL) // fill TCB
    {
#ifdef USECEVA
#ifdef USE_TCBQ
        tcb = RexCreateTCBQ(TASKID_NMM_FFT64, RESOURCE_CEVA, NULL, 0, NULL, NULL,TIME_NMM_FFT64);
#else
        tcb = CreateTCB(TASKID_NMM_FFT64, RESOURCE_CEVA, NULL, 0, NULL, NULL);
#endif

#else
        tcb = CreateTCB(TASKID_NMM_FFT64, RESOURCE_LARM, NULL, 0, NULL, NULL);
        tcb->ResourceIndex = 0;
        RegARMTask(TASKID_NMM_FFT64, &lte_bs_phy_nmm_fft64_task);
#endif

        CreateTCBIOBuf(tcb, 3);
        FillTCBIOBuf(tcb, (U32 *) pin, (2 * FFT_SIZE_096MHZ) * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) freq, (2 * FFT_SIZE_096MHZ) * sizeof(S16), OUT_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) exp, 1 * sizeof(S16), OUT_BUF, NULL);

#if NMM_USE_TCB_LIST
        AddTCBToList(tcb, pTskLst, NULL);
#else
        ExeTSchedSingle(tcb);
        tcb = NULL;
#endif
    }
    else //refill
    {
        tcb = refill_tcb;
        ReFillTCBIOBuf(tcb, (U32 *) pin, (2 * FFT_SIZE_096MHZ) * sizeof(S16), IN_BUF, 0);
        ReFillTCBIOBuf(tcb, (U32 *) freq, (2 * FFT_SIZE_096MHZ) * sizeof(S16), OUT_BUF, 1);
        ReFillTCBIOBuf(tcb, (U32 *) exp, 1 * sizeof(S16), OUT_BUF, 2);

    }

#else
    nmm_dbg_printf_off(("> %s... ", __FUNCTION__));
    cfft_16t32d_case2(pin, 6, freq, exp);
#endif

    nmm_dbg_printf_off(("done.\r\n"));

    return tcb;
}


// //////////////////////////////////////////////////////////////////////////////////////
/** @brief lte_bs_phy_nmm_chanest_tcb Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  ------------------------------------------------------
 *  |Task ID                          |Scheduled on      |
 *  |                                 |Hardware Resource |
 *  ------------------------------------------------------
 *  |TASKID_NMM_SSYNC_CHANEST         |RESOURCE_MDMA     |
 *  ------------------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the SSS data demodulation.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: lte_bs_phy_nmm_ssync_est_channel_estimate() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_nmm_cell_search 
 *
 **/
// -------------------------------------------------------------------------------------------
PTCB
lte_bs_phy_nmm_chanest_tcb(PTASKLIST pTskLst, S16 * SSync_f, S16 * PSync_f, S16 * SSyncEst, S16 PSS_root, S16 lp_cp_flag, PTCB * dep_tcb, S16 last_tcb_idx,
                           PTCB refill_tcb)
{
    PTCB            tcb = NULL;
#ifdef SCHEDULER
    U32            *context = NULL;
    S16             i;
    S16            *pZC;

    pZC = ZC25_FD;
    if (PSS_root == 1) {
        pZC = ZC29_FD;
    }
    if (PSS_root == 2) {
        pZC = ZC34_FD;
    }

    nmm_dbg_printf_off(("> %s... ", __FUNCTION__));

    if(refill_tcb == NULL)
    {
#ifdef USECEVA
#ifdef USE_TCBQ
        tcb = RexCreateTCBQ(TASKID_NMM_SSYNC_CHANEST, RESOURCE_CEVA, NULL, 2 * sizeof(U32), NULL, NULL,TIME_NMM_SSYNC_CHANEST);
#else
        tcb = CreateTCB(TASKID_NMM_SSYNC_CHANEST, RESOURCE_CEVA, NULL, 2 * sizeof(U32), NULL, NULL);
#endif

#else
        tcb = CreateTCB(TASKID_NMM_SSYNC_CHANEST, RESOURCE_LARM, NULL, 2 * sizeof(U32), NULL, NULL);
        tcb->ResourceIndex = 0;
        RegARMTask(TASKID_NMM_SSYNC_CHANEST, &lte_bs_phy_nmm_chanest_task);
#endif

        context = (U32 *) tcb->ContextPtr;
        context[0] = PSS_root;
        context[1] = lp_cp_flag;

        CreateTCBIOBuf(tcb, 5);
        FillTCBIOBuf(tcb, (U32 *) SSync_f, (2 * FFT_SIZE_096MHZ) * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) PSync_f, (2 * FFT_SIZE_096MHZ) * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) EXP_HSPHS, (2 * FFT_SIZE_096MHZ) * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) pZC, (2 * FFT_SIZE_096MHZ) * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) SSyncEst, (2 * FFT_SIZE_096MHZ) * sizeof(S16), INOUT_BUF, NULL);


    #if NMM_USE_TCB_LIST
        AddTCBToList(tcb, pTskLst, NULL);
        for (i = 0; i < 4; i++)
            AddDependency(tcb, 1, dep_tcb[last_tcb_idx - i]);
    #else
        ExeTSchedSingle(tcb);
        tcb = NULL;
    #endif
    }
    else
    {
        tcb = refill_tcb;
        context = (U32 *) tcb->ContextPtr;
        context[0] = PSS_root;
        context[1] = lp_cp_flag;
        ReFillTCBIOBuf(tcb, (U32 *) SSync_f, (2 * FFT_SIZE_096MHZ) * sizeof(S16), IN_BUF, 0);
        ReFillTCBIOBuf(tcb, (U32 *) PSync_f, (2 * FFT_SIZE_096MHZ) * sizeof(S16), IN_BUF, 1);
        ReFillTCBIOBuf(tcb, (U32 *) EXP_HSPHS, (2 * FFT_SIZE_096MHZ) * sizeof(S16), IN_BUF, 2);
        ReFillTCBIOBuf(tcb, (U32 *) pZC, (2 * FFT_SIZE_096MHZ) * sizeof(S16), IN_BUF, 3);
        ReFillTCBIOBuf(tcb, (U32 *) SSyncEst, (2 * FFT_SIZE_096MHZ) * sizeof(S16), INOUT_BUF, 4);
    }

#else
    nmm_dbg_printf_off(("> %s... ", __FUNCTION__));
    lte_bs_phy_nmm_ssync_est_channel_estimate(SSync_f, PSync_f, EXP_HSPHS, SSyncEst, PSS_root, lp_cp_flag);
#endif

    nmm_dbg_printf_off(("done.\r\n"));

    return tcb;
}


// //////////////////////////////////////////////////////////////////////////////////////
/** @brief lte_bs_phy_nmm_channel_estimate_tcb Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  ------------------------------------------------------
 *  |Task ID                          |Scheduled on      |
 *  |                                 |Hardware Resource |
 *  ------------------------------------------------------
 *  |TASKID_NMM_CHANNEL_EST           |RESOURCE_MDMA     |
 *  ------------------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the subframe channel estimation based on DL RS.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: lte_bs_phy_nmm_channel_estimate() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_nmm_phy 
 *
 **/
// -------------------------------------------------------------------------------------------
PTCB
lte_bs_phy_nmm_channel_estimate_tcb(PTASKLIST pTskLst, S16 *Rx_RS_Symbol, S16 ts_num,
									S16 cell_pci, S16 cell_cp, S16 Ndl_rb, S16 expoNfft,
									S16 *chEst_cram, U16 Ntx_ante,U32*inv_noise_var,
									S32 *foeEst, PTCB refill_tcb)
{
    PTCB            tcb = NULL;
#ifdef SCHEDULER
    U32            *context = NULL;
    U16             N_RS_symb_time;

    nmm_dbg_printf_off(("> %s... ", __FUNCTION__));

    if(refill_tcb == NULL)
    {
#ifdef USECEVA
#ifdef USE_TCBQ
        tcb = RexCreateTCBQ(TASKID_NMM_CHANNEL_EST, RESOURCE_CEVA, NULL, 6 * sizeof(U32), NULL, NULL,TIME_NMM_CHANNEL_EST);
#else
        tcb = CreateTCB(TASKID_NMM_CHANNEL_EST, RESOURCE_CEVA, NULL, 6 * sizeof(U32), NULL, NULL);
#endif

#else
        tcb = CreateTCB(TASKID_NMM_CHANNEL_EST, RESOURCE_LARM, NULL, 6 * sizeof(U32), NULL, NULL);
        tcb->ResourceIndex = 0;
        RegARMTask(TASKID_NMM_CHANNEL_EST, &lte_bs_phy_nmm_channel_estimate_task);
#endif

        context = (U32 *) tcb->ContextPtr;
        context[0] = ts_num;
        context[1] = cell_pci;
        context[2] = cell_cp;
        context[3] = Ndl_rb;
        context[4] = expoNfft;
		context[5] = Ntx_ante;

		if(Ntx_ante<=2)
        	N_RS_symb_time = 4;
    	else
        	N_RS_symb_time = 6;

    CreateTCBIOBuf(tcb, 4);
    FillTCBIOBuf(tcb, (U32 *) Rx_RS_Symbol, (2 * (1 << expoNfft) * N_RS_symb_time) * sizeof(S16), IN_BUF, NULL);
    FillTCBIOBuf(tcb, (U32 *) chEst_cram, (2 * Ndl_rb * PRB_12) *Ntx_ante* sizeof(S16), OUT_BUF, NULL);
    FillTCBIOBuf(tcb, (U32 *) inv_noise_var, sizeof(U32), OUT_BUF, NULL);
    FillTCBIOBuf(tcb, (U32 *) foeEst, 2 * sizeof(S32), OUT_BUF, NULL);

#if NMM_USE_TCB_LIST
    AddTCBToList(tcb, pTskLst, NULL);
#else
    ExeTSchedSingle(tcb);
    tcb = NULL;
#endif
    }
    else
    {
        tcb = refill_tcb;
        context = (U32 *) tcb->ContextPtr;
        context[0] = ts_num;
        context[1] = cell_pci;
        context[2] = cell_cp;
        context[3] = Ndl_rb;
        context[4] = expoNfft;
		context[5] = Ntx_ante;
        if(Ntx_ante<=2)
        	N_RS_symb_time = 4;
    	else
        	N_RS_symb_time = 6;

        ReFillTCBIOBuf(tcb, (U32 *) Rx_RS_Symbol, (2 * (1 << expoNfft) * N_RS_symb_time) * sizeof(S16), IN_BUF, 0);
	    ReFillTCBIOBuf(tcb, (U32 *) chEst_cram, (2 * Ndl_rb * PRB_12) *Ntx_ante* sizeof(S16), OUT_BUF, 1);
	    ReFillTCBIOBuf(tcb, (U32 *) inv_noise_var, sizeof(U32), OUT_BUF, 2);
	    ReFillTCBIOBuf(tcb, (U32 *) foeEst, 2 * sizeof(S32), OUT_BUF, 3);
    }

#else
    nmm_dbg_printf_off(("> %s... ", __FUNCTION__));
    lte_bs_phy_nmm_channel_estimate(Rx_RS_Symbol, ts_num, cell_pci, cell_cp, Ndl_rb,
									expoNfft, chEst_cram,Ntx_ante,inv_noise_var, foeEst);

#endif

    nmm_dbg_printf_off(("done.\n"));

    return tcb;
}


// //////////////////////////////////////////////////////////////////////////////////////
/** @brief lte_bs_phy_nmm_freqOffset_comp_tcb Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  ------------------------------------------------------
 *  |Task ID                          |Scheduled on      |
 *  |                                 |Hardware Resource |
 *  ------------------------------------------------------
 *  |TASKID_NMM_FOE_COMP              |RESOURCE_MDMA     |
 *  ------------------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the frequency offset compensation.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: lte_bs_phy_nmm_freqOffset_comp() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_nmm_phy
 *
 **/
// -------------------------------------------------------------------------------------------
PTCB
lte_bs_phy_nmm_freqOffset_comp_tcb(PTASKLIST pTskLst, S16 *pin, S32 foeEst, S16 symbolPos, S16 length, PTCB refill_tcb)
{
    PTCB            tcb = NULL;
#ifdef SCHEDULER
    U32            *context = NULL;

    nmm_dbg_printf_off(("> %s... ", __FUNCTION__));

    if(refill_tcb == NULL)
    {
#ifdef USECEVA
#ifdef USE_TCBQ
    tcb = RexCreateTCBQ(TASKID_NMM_FOE_COMP, RESOURCE_CEVA, NULL, 3 * sizeof(U32), NULL, NULL,TIME_NMM_FOE_COMP);
#else
    tcb = CreateTCB(TASKID_NMM_FOE_COMP, RESOURCE_CEVA, NULL, 3 * sizeof(U32), NULL, NULL);
#endif
#else
    tcb = CreateTCB(TASKID_NMM_FOE_COMP, RESOURCE_LARM, NULL, 3 * sizeof(U32), NULL, NULL);
    tcb->ResourceIndex = 0;
    RegARMTask(TASKID_NMM_FOE_COMP, &lte_bs_phy_nmm_freqOffset_comp_task);
#endif

    context = (U32 *) tcb->ContextPtr;
    context[0] = foeEst;
    context[1] = symbolPos;
    context[2] = length;

    CreateTCBIOBuf(tcb, 1);
    FillTCBIOBuf(tcb, (U32 *) pin, (2 * length) * sizeof(S16), INOUT_BUF, NULL);

#if NMM_USE_TCB_LIST
        AddTCBToList(tcb, pTskLst, NULL);
#else
        ExeTSchedSingle(tcb);
        tcb = NULL;
#endif
    }
    else
    {
        tcb = refill_tcb;
        context = (U32 *) tcb->ContextPtr;
        context[0] = foeEst;
        context[1] = symbolPos;
        context[2] = length;

        ReFillTCBIOBuf(tcb, (U32 *) pin, (2 * length) * sizeof(S16), INOUT_BUF, 0);
    }

#else
    nmm_dbg_printf_off(("> %s... ", __FUNCTION__));
    lte_bs_phy_nmm_freqOffset_comp(pin, foeEst >> 13, symbolPos, length);

#endif

    nmm_dbg_printf_off(("done.\r\n"));

    return tcb;
}


// //////////////////////////////////////////////////////////////////////////////////////
/** @brief lte_bs_phy_nmm_sync_decode_sss_tcb Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  ------------------------------------------------------
 *  |Task ID                          |Scheduled on      |
 *  |                                 |Hardware Resource |
 *  ------------------------------------------------------
 *  |TASKID_NMM_DECODE_SSS            |RESOURCE_MDMA     |
 *  ------------------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the SSS detection.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: lte_bs_phy_nmm_ssync_decode_sss() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_nmm_cell_search
 *
 **/
// -------------------------------------------------------------------------------------------
PTCB
lte_bs_phy_nmm_sync_decode_sss_tcb(PTASKLIST pTskLst, S16 * SSyncEst, S16 NID_2, S16 * NID_cell, S16 * Subframe, PTCB * dep_tcb, S16 last_tcb_idx,
	PTCB refill_tcb)
{
    PTCB            tcb = NULL;
#ifdef SCHEDULER
    U32            *context = NULL;
    S16             i;

    nmm_dbg_printf_off(("> %s... ", __FUNCTION__));
    if(refill_tcb == NULL)
    {
#ifdef USECEVA
#ifdef USE_TCBQ
        tcb = RexCreateTCBQ(TASKID_NMM_DECODE_SSS, RESOURCE_CEVA, NULL, 1 * sizeof(U32), NULL, NULL,TIME_NMM_DECODE_SSS);
#else
        tcb = CreateTCB(TASKID_NMM_DECODE_SSS, RESOURCE_CEVA, NULL, 1 * sizeof(U32), NULL, NULL);
#endif

#else
        tcb = CreateTCB(TASKID_NMM_DECODE_SSS, RESOURCE_LARM, NULL, 1 * sizeof(U32), NULL, NULL);
        tcb->ResourceIndex = 0;
        RegARMTask(TASKID_NMM_DECODE_SSS, &lte_bs_phy_nmm_decode_sss_task);
#endif

        context = (U32 *) tcb->ContextPtr;
        context[0] = NID_2;

        CreateTCBIOBuf(tcb, 4);
        FillTCBIOBuf(tcb, (U32 *) SSyncEst, (2 * FFT_SIZE_096MHZ) * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) S0_REF, 62 * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) NID_cell, sizeof(S16), OUT_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) Subframe, sizeof(S16), OUT_BUF, NULL);

    #if NMM_USE_TCB_LIST
        AddTCBToList(tcb, pTskLst, NULL);
        for (i = 0; i < 3; i++)
            AddDependency(tcb, 1, dep_tcb[last_tcb_idx - i]);
    #else
        ExeTSchedSingle(tcb);
        tcb = NULL;
    #endif
    }
    else
    {
        tcb = refill_tcb;
        context = (U32 *) tcb->ContextPtr;
        context[0] = NID_2;
        ReFillTCBIOBuf(tcb, (U32 *) SSyncEst, (2 * FFT_SIZE_096MHZ) * sizeof(S16), IN_BUF, 0);
        ReFillTCBIOBuf(tcb, (U32 *) S0_REF, 62 * sizeof(S16), IN_BUF, 1);
        ReFillTCBIOBuf(tcb, (U32 *) NID_cell, sizeof(S16), OUT_BUF, 2);
        ReFillTCBIOBuf(tcb, (U32 *) Subframe, sizeof(S16), OUT_BUF, 3);
    }

#else
    nmm_dbg_printf_off(("> %s... ", __FUNCTION__));
    lte_bs_phy_nmm_ssync_decode_sss(SSyncEst, S0_REF, NID_2, NID_cell, Subframe);
#endif

    nmm_dbg_printf_off(("done.\r\n"));

    return tcb;
}


// //////////////////////////////////////////////////////////////////////////////////////
/** @brief lte_bs_phy_nmm_get_rsrp_tcb Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  ------------------------------------------------------
 *  |Task ID                          |Scheduled on      |
 *  |                                 |Hardware Resource |
 *  ------------------------------------------------------
 *  |TASKID_NMM_GET_RSRP              |RESOURCE_MDMA     |
 *  ------------------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the NMM RSRP measurement in cell search.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: lte_bs_phy_nmm_get_rsrp() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_nmm_cell_search 
 *
 **/
// -------------------------------------------------------------------------------------------
PTCB
lte_bs_phy_nmm_get_rsrp_tcb(PTASKLIST pTskLst, S16 expoNfft, SYMBOL_INFO * psymb_info, S16 cell_pci, S16 cell_cp, S16 cell_search_meas_bandwidth, 
                            S32 * rsrp, S16 rsrp_scale, S16 num_pilots, S16 size_inverse,PTCB refill_tcb)
{
    PTCB            tcb = NULL;

#ifdef SCHEDULER
    U32            *context = NULL;
    S16            *pin=NULL;
    U16             symb_index = 0;

    nmm_dbg_printf_off(("> %s... ", __FUNCTION__));
    if(NULL != psymb_info)
    {
        pin= psymb_info->start + psymb_info->cp_size * 2;
        symb_index = psymb_info->symb_index;
    }

    if(refill_tcb == NULL)
    {

#ifdef USECEVA
#ifdef USE_TCBQ
        tcb = RexCreateTCBQ(TASKID_NMM_GET_RSRP, RESOURCE_CEVA, NULL, 8 * sizeof(U32), NULL, NULL,TIME_NMM_GET_RSRP);
#else
        tcb = CreateTCB(TASKID_NMM_GET_RSRP, RESOURCE_CEVA, NULL, 8 * sizeof(U32), NULL, NULL);
#endif

#else
        tcb = CreateTCB(TASKID_NMM_GET_RSRP, RESOURCE_LARM, NULL, 8 * sizeof(U32), NULL, NULL);
        tcb->ResourceIndex = 0;
        RegARMTask(TASKID_NMM_GET_RSRP, &lte_bs_phy_nmm_get_rsrp_task);
#endif

        context = (U32 *) tcb->ContextPtr;
        *context++ = (U32) expoNfft;
        *context++ = (U32) cell_pci;
        *context++ = (U32) cell_cp;
        *context++ = (U32) cell_search_meas_bandwidth;
        *context++ = (U32) num_pilots;
        *context++ = (U32) size_inverse;
        *context++ = (U32) symb_index;
        *context++ = (U32) rsrp_scale;

        CreateTCBIOBuf(tcb, 2);
        FillTCBIOBuf(tcb, (U32 *) pin, (2 * (1 << expoNfft)) * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) rsrp, sizeof(S32), OUT_BUF, NULL);
#if NMM_CEVA_DBG
        FillTCBIOBuf(tcb, (U32 *) arm_dbg, 256 * sizeof(char), OUT_BUF, NULL);
#endif

#if NMM_USE_TCB_LIST
        AddTCBToList(tcb, pTskLst, NULL);
#else
        ExeTSchedSingle(tcb);
        tcb = NULL;
#endif
    }
    else
    {
        tcb = refill_tcb;

        context = (U32 *) tcb->ContextPtr;
        *context++ = (U32) expoNfft;
        *context++ = (U32) cell_pci;
        *context++ = (U32) cell_cp;
        *context++ = (U32) cell_search_meas_bandwidth;
        *context++ = (U32) num_pilots;
        *context++ = (U32) size_inverse;
        *context++ = (U32) symb_index;
        *context++ = (U32) rsrp_scale;

        ReFillTCBIOBuf(tcb, (U32 *) pin, (2 * (1 << expoNfft)) * sizeof(S16), IN_BUF, 0);
        ReFillTCBIOBuf(tcb, (U32 *) rsrp, sizeof(S32), OUT_BUF, 1);
#if NMM_CEVA_DBG
        ReFillTCBIOBuf(tcb, (U32 *) arm_dbg, 256 * sizeof(char), OUT_BUF, 2);
#endif

    }

#else
    nmm_dbg_printf_off(("> %s... ", __FUNCTION__));
    lte_bs_phy_nmm_get_rsrp(expoNfft, psymb_info->start + psymb_info->cp_size * 2, psymb_info->symb_index, cell_pci, cell_cp, cell_search_meas_bandwidth,
                            rsrp, rsrp_scale, num_pilots, size_inverse);
#endif

    nmm_dbg_printf_off(("done.\r\n"));

    return tcb;
}


// //////////////////////////////////////////////////////////////////////////////////////
/** @brief lte_bs_phy_nmm_pseudo_random_seq_gen_tcb Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  ------------------------------------------------------
 *  |Task ID                          |Scheduled on      |
 *  |                                 |Hardware Resource |
 *  ------------------------------------------------------
 *  |TASKID_NMM_PSEUDO_RANDOM_SEQ     |RESOURCE_MDMA     |
 *  ------------------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the pseudorandom sequence generation.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: PseudoRandomSeqGen() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_nmm_phy 
 *
 **/
// -------------------------------------------------------------------------------------------
PTCB
lte_bs_phy_nmm_pseudo_random_seq_gen_tcb(PTASKLIST pTskLst, S32 N, U32 cinit, U8 * c,PTCB refill_tcb)
{
    PTCB            tcb = NULL;
#ifdef SCHEDULER
    U32            *context = NULL;

    nmm_dbg_printf_off(("> %s... ", __FUNCTION__));

    if(refill_tcb == NULL)
    {
#ifdef USECEVA
#ifdef USE_TCBQ
        tcb = RexCreateTCBQ(TASKID_NMM_PSEUDO_RANDOM_SEQ, RESOURCE_CEVA, NULL, 2 * sizeof(U32), NULL, NULL,TIME_NMM_PSEUDO_RANDOM_SEQ);
#else
        tcb = CreateTCB(TASKID_NMM_PSEUDO_RANDOM_SEQ, RESOURCE_CEVA, NULL, 2 * sizeof(U32), NULL, NULL);
#endif

#else
        tcb = CreateTCB(TASKID_NMM_PSEUDO_RANDOM_SEQ, RESOURCE_LARM, NULL, 2 * sizeof(U32), NULL, NULL);
        tcb->ResourceIndex = 0;
        RegARMTask(TASKID_NMM_PSEUDO_RANDOM_SEQ, &lte_bs_phy_nmm_pseudo_random_seq_gen_task);
#endif

        context = (U32 *) tcb->ContextPtr;
        context[0] = N;
        context[1] = cinit;

        CreateTCBIOBuf(tcb, 1);
        FillTCBIOBuf(tcb, (U32 *) c, N * sizeof(U8), OUT_BUF, NULL);
#if NMM_USE_TCB_LIST
        AddTCBToList(tcb, pTskLst, NULL);
#else
        ExeTSchedSingle(tcb);
        tcb = NULL;
#endif
    }
    else
    {
        tcb = refill_tcb;
        context = (U32 *) tcb->ContextPtr;
        context[0] = N;
        context[1] = cinit;

        ReFillTCBIOBuf(tcb, (U32 *) c, N * sizeof(U8), OUT_BUF, 0);
    }

#else
    nmm_dbg_printf_off(("> %s... ", __FUNCTION__));
    PseudoRandomSeqGen(N, cinit, c);
#endif

    nmm_dbg_printf_off(("done.\r\n"));

    return tcb;
}


// //////////////////////////////////////////////////////////////////////////////////////
/** @brief lte_bs_phy_nmm_extract_pbch_tcb Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  ------------------------------------------------------
 *  |Task ID                          |Scheduled on      |
 *  |                                 |Hardware Resource |
 *  ------------------------------------------------------
 *  |TASKID_NMM_EXTRACT_PBCH          |RESOURCE_MDMA     |
 *  ------------------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the pbch data extraction.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: lte_bs_phy_nmm_extract_pbch() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_nmm_MIB 
 *
 **/
// -------------------------------------------------------------------------------------------
PTCB
lte_bs_phy_nmm_extract_pbch_tcb(PTASKLIST ptask_list, S16 expo_nfft, SYMBOL_INFO * psymb_info,
								S16 cell_pci, S16 cell_cp, S16 * pbch, S16 * channelEst_buf,
								S16 * expo_fac,U16 Ntx_ante,U32 inv_noise_var,
                                PTCB refill_tcb)
{
    PTCB            tcb = NULL;

#ifdef SCHEDULER
    U32            *context = NULL;
    S16            *pin = NULL;
    U16             symb_index = 0;
    if(NULL != psymb_info)
    {
       pin = psymb_info->start + psymb_info->cp_size * 2;
       symb_index = psymb_info->symb_index;
    }
    nmm_dbg_printf_off(("> %s... ", __FUNCTION__));
    if(refill_tcb == NULL)
    {
#ifdef USECEVA
#ifdef USE_TCBQ
        tcb = RexCreateTCBQ(TASKID_NMM_EXTRACT_PBCH, RESOURCE_CEVA, NULL, 5 * sizeof(U32), NULL, NULL,TIME_NMM_EXTRACT_PBCH);
#else
        tcb = CreateTCB(TASKID_NMM_EXTRACT_PBCH, RESOURCE_CEVA, NULL, 5 * sizeof(U32), NULL, NULL);
#endif

#else
        tcb = CreateTCB(TASKID_NMM_EXTRACT_PBCH, RESOURCE_LARM, NULL, 5 * sizeof(U32), NULL, NULL);
        tcb->ResourceIndex = 0;
        RegARMTask(TASKID_NMM_EXTRACT_PBCH, &lte_bs_phy_nmm_extract_pbch_task);
#endif

        context = (U32 *) tcb->ContextPtr;
        *context++ = (U32) expo_nfft;
        *context++ = (U32) cell_pci;
        *context++ = (U32) cell_cp;
        *context++ = (U32) symb_index;
		*context++ = (U32) Ntx_ante;

        CreateTCBIOBuf(tcb, 4);
        FillTCBIOBuf(tcb, (U32 *) pin, (2 * (1 << expo_nfft)) * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) channelEst_buf, (2 * PBCH_NB_RB * PRB_12)*Ntx_ante * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) pbch, (2 * PBCH_NB_RB * PRB_12) * sizeof(S16), OUT_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) expo_fac, sizeof(S16), OUT_BUF, NULL);
#if NMM_CEVA_DBG
        FillTCBIOBuf(tcb, (U32 *) arm_dbg, 256 * sizeof(char), OUT_BUF, NULL);
#endif

#if NMM_USE_TCB_LIST
        AddTCBToList(tcb, ptask_list, NULL);
#else
        ExeTSchedSingle(tcb);
        tcb = NULL;
#endif
    }
    else
    {
        tcb = refill_tcb;
        context = (U32 *) tcb->ContextPtr;
        *context++ = (U32) expo_nfft;
        *context++ = (U32) cell_pci;
        *context++ = (U32) cell_cp;
        *context++ = (U32) symb_index;
		*context++ = (U32) Ntx_ante;

        ReFillTCBIOBuf(tcb, (U32 *) pin, (2 * (1 << expo_nfft)) * sizeof(S16), IN_BUF, 0);
        ReFillTCBIOBuf(tcb, (U32 *) channelEst_buf, (2 * PBCH_NB_RB * PRB_12) *Ntx_ante * sizeof(S16), IN_BUF, 1);
        ReFillTCBIOBuf(tcb, (U32 *) pbch, (2 * PBCH_NB_RB * PRB_12) * sizeof(S16), OUT_BUF, 2);
        ReFillTCBIOBuf(tcb, (U32 *) expo_fac, sizeof(S16), OUT_BUF, 3);
#if NMM_CEVA_DBG
        ReFillTCBIOBuf(tcb, (U32 *) arm_dbg, 256 * sizeof(char), OUT_BUF, 4);
#endif


    }

#else
    nmm_dbg_printf_off(("> %s... ", __FUNCTION__));
    lte_bs_phy_nmm_extract_pbch(expo_nfft, psymb_info->start + psymb_info->cp_size * 2,
								psymb_info->symb_index, cell_pci, cell_cp, channelEst_buf, pbch,
								expo_fac,Ntx_ante,inv_noise_var);
#endif

    nmm_dbg_printf_off(("done.\r\n"));

    return tcb;
}


// //////////////////////////////////////////////////////////////////////////////////////
/** @brief lte_bs_phy_nmm_demod_pbch_tcb Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  ------------------------------------------------------
 *  |Task ID                          |Scheduled on      |
 *  |                                 |Hardware Resource |
 *  ------------------------------------------------------
 *  |TASKID_NMM_DEMOD_PBCH            |RESOURCE_MDMA     |
 *  ------------------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the pbch demodulation.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: lte_bs_phy_nmm_demod_pbch() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_nmm_MIB
 *
 **/
// -------------------------------------------------------------------------------------------
PTCB
lte_bs_phy_nmm_demod_pbch_tcb(PTASKLIST ptask_list,S16 * pbch, S16 pbch_re_per_frame, S16 * pbch_scale, S8 * bdet_soft,PTCB refill_tcb)
{
    PTCB            tcb = NULL;
#ifdef SCHEDULER

    U32            *context = NULL;
    nmm_dbg_printf_off(("> %s... ", __FUNCTION__));

    if(refill_tcb == NULL)
    {
#ifdef USECEVA
#ifdef USE_TCBQ
        tcb = RexCreateTCBQ(TASKID_NMM_DEMOD_PBCH, RESOURCE_CEVA, NULL, sizeof(U32), NULL, NULL,TIME_NMM_DEMOD_PBCH);
#else
        tcb = CreateTCB(TASKID_NMM_DEMOD_PBCH, RESOURCE_CEVA, NULL, sizeof(U32), NULL, NULL);
#endif

#else
        tcb = CreateTCB(TASKID_NMM_DEMOD_PBCH, RESOURCE_LARM, NULL, sizeof(U32), NULL, NULL);
        tcb->ResourceIndex = 0;
        RegARMTask(TASKID_NMM_DEMOD_PBCH, &lte_bs_phy_nmm_demod_pbch_task);
#endif

        context = (U32 *) tcb->ContextPtr;
        *context++ = (U32) pbch_re_per_frame;

        CreateTCBIOBuf(tcb, 3);
        FillTCBIOBuf(tcb, (U32 *) pbch, 2 * PBCH_RE_PER_FRAME * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) pbch_scale, PBCH_SYM_PER_FRAME * PBCH_NB_FRAME * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) bdet_soft, 2 * PBCH_RE_PER_FRAME * sizeof(S8), OUT_BUF, NULL);

#if NMM_USE_TCB_LIST
        AddTCBToList(tcb, ptask_list, NULL);
#else
        ExeTSchedSingle(tcb);
        tcb = NULL;
#endif
    }
    else
    {
        tcb = refill_tcb;
        context = (U32 *) tcb->ContextPtr;
        *context++ = (U32) pbch_re_per_frame;

        ReFillTCBIOBuf(tcb, (U32 *) pbch, 2 * PBCH_RE_PER_FRAME * sizeof(S16), IN_BUF, 0);
        ReFillTCBIOBuf(tcb, (U32 *) pbch_scale, PBCH_SYM_PER_FRAME * PBCH_NB_FRAME * sizeof(S16), IN_BUF, 1);
        ReFillTCBIOBuf(tcb, (U32 *) bdet_soft, 2 * PBCH_RE_PER_FRAME * sizeof(S8), OUT_BUF, 2);
    }

#else
    nmm_dbg_printf_off(("> %s... ", __FUNCTION__));
    lte_bs_phy_nmm_demod_pbch(pbch, pbch_re_per_frame, pbch_scale, bdet_soft);
#endif

    nmm_dbg_printf_off(("done.\r\n"));

    return tcb;
}


// //////////////////////////////////////////////////////////////////////////////////////
/** @brief lte_bs_phy_nmm_extract_pcfi_tcb Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  ------------------------------------------------------
 *  |Task ID                          |Scheduled on      |
 *  |                                 |Hardware Resource |
 *  ------------------------------------------------------
 *  |TASKID_NMM_EXTRACT_PCFI          |RESOURCE_MDMA     |
 *  ------------------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the PCFI extraction.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: lte_bs_phy_nmm_extract_pcfi() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_nmm_SIB
 *
 **/
// -------------------------------------------------------------------------------------------
PTCB
lte_bs_phy_nmm_extract_pcfi_tcb(PTASKLIST ptask_list, S16 expo_nfft, S16 bw,
								SYMBOL_INFO * psymb_info, S16 * pcfi_re_idx,
								S16 * channelEst_buf, S16 * pcfi, S16 * expo_fac,
                                U16 Ntx_ante,U32 inv_noise_var,PTCB refill_tcb)
{
    PTCB            tcb = NULL;

#ifdef SCHEDULER
    U32            *context = NULL;
    S16            *pin = NULL;

    if(NULL != psymb_info)
    {
       pin = psymb_info->start + psymb_info->cp_size * 2;
    }

    nmm_dbg_printf_off(("> %s... ", __FUNCTION__));

    if(refill_tcb==NULL)
    {
#ifdef USECEVA
#ifdef USE_TCBQ
        tcb = RexCreateTCBQ(TASKID_NMM_EXTRACT_PCFI, RESOURCE_CEVA, NULL, 3 * sizeof(U32), NULL, NULL,TIME_NMM_EXTRACT_PCFI);
#else
        tcb = CreateTCB(TASKID_NMM_EXTRACT_PCFI, RESOURCE_CEVA, NULL, 3 * sizeof(U32), NULL, NULL);
#endif

#else
        tcb = CreateTCB(TASKID_NMM_EXTRACT_PCFI, RESOURCE_LARM, NULL, 3 * sizeof(U32), NULL, NULL);
        tcb->ResourceIndex = 0;
        RegARMTask(TASKID_NMM_EXTRACT_PCFI, &lte_bs_phy_nmm_extract_pcfi_task);
#endif

        context = (U32 *) tcb->ContextPtr;
        *context++ = (U32) expo_nfft;
        *context++ = (U32) bw;
		*context++ = (U32) Ntx_ante;

        CreateTCBIOBuf(tcb, 5);
        FillTCBIOBuf(tcb, (U32 *) pin, (2 * (1 << expo_nfft)) * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) pcfi_re_idx, PCFICH_NUM_RE * sizeof(S16), IN_BUF, NULL);
    	FillTCBIOBuf(tcb, (U32 *) channelEst_buf, (2 * (1 << expo_nfft)) *Ntx_ante * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) pcfi, (2 * PCFICH_NUM_RE) * sizeof(S16), OUT_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) expo_fac, sizeof(S16), OUT_BUF, NULL);
#if NMM_CEVA_DBG
        FillTCBIOBuf(tcb, (U32 *) arm_dbg, 256 * sizeof(char), OUT_BUF, NULL);
#endif

#if NMM_USE_TCB_LIST
        AddTCBToList(tcb, ptask_list, NULL);
#else
        ExeTSchedSingle(tcb);
        tcb = NULL;
#endif
    }
    else
    {
        tcb = refill_tcb;
        context = (U32 *) tcb->ContextPtr;
        *context++ = (U32) expo_nfft;
        *context++ = (U32) bw;
		*context++ = (U32) Ntx_ante;

        ReFillTCBIOBuf(tcb, (U32 *) pin, (2 * (1 << expo_nfft)) * sizeof(S16), IN_BUF, 0);
        ReFillTCBIOBuf(tcb, (U32 *) pcfi_re_idx, PCFICH_NUM_RE * sizeof(S16), IN_BUF, 1);
        ReFillTCBIOBuf(tcb, (U32 *) channelEst_buf, (2 * (1 << expo_nfft)) *Ntx_ante* sizeof(S16), IN_BUF, 2);
        ReFillTCBIOBuf(tcb, (U32 *) pcfi, (2 * PCFICH_NUM_RE) * sizeof(S16), OUT_BUF, 3);
        ReFillTCBIOBuf(tcb, (U32 *) expo_fac, sizeof(S16), OUT_BUF, 4);
#if NMM_CEVA_DBG
        ReFillTCBIOBuf(tcb, (U32 *) arm_dbg, 256 * sizeof(char), OUT_BUF, 5);
#endif
    }

#else
    nmm_dbg_printf_off(("> %s... ", __FUNCTION__));
    lte_bs_phy_nmm_extract_pcfi(expo_nfft, psymb_info->start + psymb_info->cp_size * 2,
								bw, pcfi_re_idx, channelEst_buf, pcfi, expo_fac,Ntx_ante,inv_noise_var);
#endif

    nmm_dbg_printf_off(("done.\r\n"));

    return tcb;
}


// //////////////////////////////////////////////////////////////////////////////////////
/** @brief lte_bs_phy_nmm_extract_pdcch_tcb Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  ------------------------------------------------------
 *  |Task ID                          |Scheduled on      |
 *  |                                 |Hardware Resource |
 *  ------------------------------------------------------
 *  |TASKID_NMM_EXTRACT_PDCCH         |RESOURCE_MDMA     |
 *  ------------------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the PDCCH extraction.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: lte_bs_phy_nmm_extract_pdcch() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_nmm_sib
 *
 **/
// -------------------------------------------------------------------------------------------
PTCB
lte_bs_phy_nmm_extract_pdcch_tcb(PTASKLIST ptask_list, S16 expo_nfft, S16 bw,
								SYMBOL_INFO * psymb_info, S16 * pdcch_idx,
								S16 pdcch_re, S16 * channelEst_buf, S16 * pdcch_sym,
								S16* expo_fac,U16 Ntx_ante,U32 inv_noise_var,
                                 PTCB refill_tcb)
{
    PTCB            tcb = NULL;

#ifdef SCHEDULER
    U32            *context = NULL;
    S16            *pin = NULL;

    if(psymb_info != NULL)
    {
        pin = psymb_info->start + psymb_info->cp_size * 2;
    }

    nmm_dbg_printf_off(("> %s... ", __FUNCTION__));

    if(refill_tcb == NULL)
    {
#ifdef USECEVA
#ifdef USE_TCBQ
        tcb = RexCreateTCBQ(TASKID_NMM_EXTRACT_PDCCH, RESOURCE_CEVA, NULL, 4 * sizeof(U32), NULL, NULL,TIME_NMM_EXTRACT_PDCCH);
#else
        tcb = CreateTCB(TASKID_NMM_EXTRACT_PDCCH, RESOURCE_CEVA, NULL, 4 * sizeof(U32), NULL, NULL);
#endif

#else
        tcb = CreateTCB(TASKID_NMM_EXTRACT_PDCCH, RESOURCE_LARM, NULL, 4 * sizeof(U32), NULL, NULL);
        tcb->ResourceIndex = 0;
        RegARMTask(TASKID_NMM_EXTRACT_PDCCH, &lte_bs_phy_nmm_extract_pdcch_task);
#endif

        context = (U32 *) tcb->ContextPtr;
        *context++ = (U32) expo_nfft;
        *context++ = (U32) bw;
        *context++ = (U32) pdcch_re;
    	*context++ = (U32) Ntx_ante;

        CreateTCBIOBuf(tcb, 5);
        FillTCBIOBuf(tcb, (U32 *) pin, (2 * (1 << expo_nfft)) * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) pdcch_idx, pdcch_re * sizeof(S16), IN_BUF, NULL);
    	FillTCBIOBuf(tcb, (U32 *) channelEst_buf, (2 * (1 << expo_nfft)) * Ntx_ante* sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) pdcch_sym, (2 * pdcch_re) * sizeof(S16), OUT_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) expo_fac, sizeof(S16), OUT_BUF, NULL);
#if NMM_CEVA_DBG
        FillTCBIOBuf(tcb, (U32 *) arm_dbg, 256 * sizeof(char), OUT_BUF, NULL);
#endif

#if NMM_USE_TCB_LIST
        AddTCBToList(tcb, ptask_list, NULL);
#else
        ExeTSchedSingle(tcb);
        tcb = NULL;
#endif
    }
    else
    {
        tcb = refill_tcb;
        context = (U32 *) tcb->ContextPtr;
        *context++ = (U32) expo_nfft;
        *context++ = (U32) bw;
        *context++ = (U32) pdcch_re;
		*context++ = (U32) Ntx_ante;

        ReFillTCBIOBuf(tcb, (U32 *) pin, (2 * (1 << expo_nfft)) * sizeof(S16), IN_BUF, 0);
        ReFillTCBIOBuf(tcb, (U32 *) pdcch_idx, pdcch_re * sizeof(S16), IN_BUF, 1);
        ReFillTCBIOBuf(tcb, (U32 *) channelEst_buf, (2 * (1 << expo_nfft)) * Ntx_ante* sizeof(S16), IN_BUF, 2);
        ReFillTCBIOBuf(tcb, (U32 *) pdcch_sym, (2 * pdcch_re) * sizeof(S16), OUT_BUF, 3);
        ReFillTCBIOBuf(tcb, (U32 *) expo_fac, sizeof(S16), OUT_BUF, 4);
#if NMM_CEVA_DBG
        ReFillTCBIOBuf(tcb, (U32 *) arm_dbg, 256 * sizeof(char), OUT_BUF, 5);
#endif
    }

#else
    nmm_dbg_printf_off(("> %s... ", __FUNCTION__));
    lte_bs_phy_nmm_extract_pdcch(expo_nfft, psymb_info->start + psymb_info->cp_size * 2,
								bw, pdcch_idx, pdcch_re, channelEst_buf, pdcch_sym, expo_fac,Ntx_ante,inv_noise_var);
#endif

    nmm_dbg_printf_off(("done.\r\n"));

    return tcb;
}


// //////////////////////////////////////////////////////////////////////////////////////
/** @brief lte_bs_phy_nmm_pdcch_demod_tcb Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  ------------------------------------------------------
 *  |Task ID                          |Scheduled on      |
 *  |                                 |Hardware Resource |
 *  ------------------------------------------------------
 *  |TASKID_NMM_PDCCH_DEMOD           |RESOURCE_MDMA     |
 *  ------------------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the PDCCH demodulation.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: lte_bs_phy_nmm_pdcch_demod() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_nmm_rssi 
 *
 **/
// -------------------------------------------------------------------------------------------
#if ORIG_CODE
PTCB
lte_bs_phy_nmm_pdcch_demod_tcb(PTASKLIST ptask_list, S32 * pdcch32, S16 NusedE, S8 * bDetSoft, S16 pdcch_re_num, U32 cinit, U8 * pdcch_scrambling_seq)
{
    PTCB            tcb = NULL;
#ifdef SCHEDULER
    U32            *context = NULL;

    nmm_dbg_printf_off(("> %s... ", __FUNCTION__));

#ifdef USECEVA
#ifdef USE_TCBQ
    tcb = RexCreateTCBQ(TASKID_NMM_PDCCH_DEMOD, RESOURCE_CEVA, NULL, 3 * sizeof(U32), NULL, NULL,TIME_NMM_PDCCH_DEMOD);
#else
    tcb = CreateTCB(TASKID_NMM_PDCCH_DEMOD, RESOURCE_CEVA, NULL, 3 * sizeof(U32), NULL, NULL);
#endif

#else
    tcb = CreateTCB(TASKID_NMM_PDCCH_DEMOD, RESOURCE_LARM, NULL, 3 * sizeof(U32), NULL, NULL);
    tcb->ResourceIndex = 0;
    RegARMTask(TASKID_NMM_PDCCH_DEMOD, &lte_bs_phy_nmm_pdcch_demod_task);
#endif

    context = (U32 *) tcb->ContextPtr;
    context[0] = NusedE;
    context[1] = pdcch_re_num;
    context[2] = cinit;

    CreateTCBIOBuf(tcb, 3);
    FillTCBIOBuf(tcb, (U32 *) pdcch32, 2 * NusedE * sizeof(U32), IN_BUF, NULL);
    FillTCBIOBuf(tcb, (U32 *) bDetSoft, 2 * NusedE * sizeof(U32), OUT_BUF, NULL);
    FillTCBIOBuf(tcb, (U32 *) pdcch_scrambling_seq, PDCCH_MAX_SIZE_IN_BITS * sizeof(U8), IN_BUF, NULL);

    ExeTSchedSingle(tcb);
    tcb = NULL;

#else
    nmm_dbg_printf_off(("> %s... ", __FUNCTION__));
    lte_bs_phy_nmm_pdcch_demod(pdcch32, NusedE, bDetSoft, cinit, pdcch_scrambling_seq);
#endif

    nmm_dbg_printf_off(("done.\r\n"));

    return tcb;
}
#else
PTCB
lte_bs_phy_nmm_pdcch_demod_tcb(PTASKLIST ptask_list, S16 * pdcch, S16 NusedE, S16 pci, S8 * bDetSoft, S16 pdcch_re_num, U32 cinit, U8 * pdcch_scrambling_seq,
                               PTCB refill_tcb)
{
    PTCB            tcb = NULL;
#ifdef SCHEDULER
    U32            *context = NULL;

    nmm_dbg_printf_off(("> %s... ", __FUNCTION__));

    if(refill_tcb==NULL)
    {
#ifdef USECEVA
#ifdef USE_TCBQ
        tcb = RexCreateTCBQ(TASKID_NMM_PDCCH_DEMOD, RESOURCE_CEVA, NULL, 4 * sizeof(U32), NULL, NULL,TIME_NMM_PDCCH_DEMOD);
#else
        tcb = CreateTCB(TASKID_NMM_PDCCH_DEMOD, RESOURCE_CEVA, NULL, 4 * sizeof(U32), NULL, NULL);
#endif

#else
        tcb = CreateTCB(TASKID_NMM_PDCCH_DEMOD, RESOURCE_LARM, NULL, 4 * sizeof(U32), NULL, NULL);
        tcb->ResourceIndex = 0;
        RegARMTask(TASKID_NMM_PDCCH_DEMOD, &lte_bs_phy_nmm_pdcch_demod_task);
#endif

        context = (U32 *) tcb->ContextPtr;
        context[0] = NusedE;
        context[1] = pci;
        context[2] = pdcch_re_num;
        context[3] = cinit;

        CreateTCBIOBuf(tcb, 3);
        FillTCBIOBuf(tcb, (U32 *) pdcch, 2 * NusedE * sizeof(S16), IN_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) bDetSoft, 2 * NusedE * sizeof(U32), OUT_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) pdcch_scrambling_seq, PDCCH_MAX_SIZE_IN_BITS * sizeof(U8), IN_BUF, NULL);

#if NMM_USE_TCB_LIST
        AddTCBToList(tcb, ptask_list, NULL);
#else
        ExeTSchedSingle(tcb);
        tcb = NULL;
#endif
    }
    else
    {
        tcb = refill_tcb;
        context = (U32 *) tcb->ContextPtr;
        context[0] = NusedE;
        context[1] = pci;
        context[2] = pdcch_re_num;
        context[3] = cinit;

        ReFillTCBIOBuf(tcb, (U32 *) pdcch, 2 * NusedE * sizeof(S16), IN_BUF, 0);
        ReFillTCBIOBuf(tcb, (U32 *) bDetSoft, 2 * NusedE * sizeof(U32), OUT_BUF, 1);
        ReFillTCBIOBuf(tcb, (U32 *) pdcch_scrambling_seq, PDCCH_MAX_SIZE_IN_BITS * sizeof(U8), IN_BUF, 2);

    }

#else
    nmm_dbg_printf_off(("> %s... ", __FUNCTION__));
    lte_bs_phy_nmm_pdcch_demod(pdcch, NusedE, pci, bDetSoft, cinit,
								pdcch_scrambling_seq);
#endif

    nmm_dbg_printf_off(("done.\r\n"));

    return tcb;
}
#endif


// //////////////////////////////////////////////////////////////////////////////////////
/** @brief lte_bs_phy_nmm_extract_pdsch_tcb Function.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  <2> TCB Descriptors:
 *
 *  TABLE_START
 *  ------------------------------------------------------
 *  |Task ID                          |Scheduled on      |
 *  |                                 |Hardware Resource |
 *  ------------------------------------------------------
 *  |TASKID_NMM_EXTRACT_PDSCH          |RESOURCE_MDMA     |
 *  ------------------------------------------------------
 *  TABLE_END
 *  <BR>
 *
 *
 *  <3> Detailed Description:
 *  This function creates a TCB (Task Control Block) for the NMM PDSCH extraction.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: lte_bs_phy_nmm_extract_pdsch() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_nmm_rssi 
 *
 **/
// -------------------------------------------------------------------------------------------
PTCB
lte_bs_phy_nmm_extract_pdsch_tcb(PTASKLIST ptask_list, S16 expo_nfft, S16 bw,
								SYMBOL_INFO * psymb_info, S16 pdsch_Nrb,
								S16 * channelEst_buf, S16 * pdsch_sym, S16 * expo_fac,
                                 U16 Ntx_ante,U8*pdsch_prb_set,U16 cell_cp,U16 isym,
								 U16 cell_id,U16*p_N_pdsch_re,U32 inv_noise_var,
								 PTCB refill_tcb,U16 subsfn)
{
    PTCB            tcb = NULL;

#ifdef SCHEDULER
    U16            pdsch_re = pdsch_Nrb*PRB_12;
    U32            *context = NULL;
    S16            *pin = NULL;

    if(NULL != psymb_info)
    {
        pin = psymb_info->start + psymb_info->cp_size * 2;
    }

    nmm_dbg_printf_off(("> %s... ", __FUNCTION__));
    if(refill_tcb == NULL)
    {

#ifdef USECEVA
#ifdef USE_TCBQ
        tcb = RexCreateTCBQ(TASKID_NMM_EXTRACT_PDSCH, RESOURCE_CEVA, NULL, 9 * sizeof(U32), NULL, NULL,TIME_NMM_EXTRACT_PDSCH);
#else
        tcb = CreateTCB(TASKID_NMM_EXTRACT_PDSCH, RESOURCE_CEVA, NULL, 9 * sizeof(U32), NULL, NULL);
#endif

#else
        tcb = CreateTCB(TASKID_NMM_EXTRACT_PDSCH, RESOURCE_LARM, NULL, 9 * sizeof(U32), NULL, NULL);
        tcb->ResourceIndex = 0;
        RegARMTask(TASKID_NMM_EXTRACT_PDSCH, &lte_bs_phy_nmm_extract_pdsch_task);
#endif

        context = (U32 *) tcb->ContextPtr;
        *context++ = (U32) expo_nfft;
        *context++ = (U32) bw;
	    *context++ = (U32) pdsch_Nrb;
	    *context++ = (U32) Ntx_ante;
	    *context++ = (U32) cell_cp;
	    *context++ = (U32) isym;
	    *context++ = (U32) cell_id;
	    *context++ = (U32) inv_noise_var;
        *context++ = (U32) subsfn;

        CreateTCBIOBuf(tcb, 6);
        FillTCBIOBuf(tcb, (U32 *) pin, (2 * (1 << expo_nfft)) * sizeof(S16), IN_BUF, NULL);
    	FillTCBIOBuf(tcb, (U32 *) channelEst_buf, (2 * (1 << expo_nfft)) *Ntx_ante * sizeof(S16), IN_BUF, NULL);
    	FillTCBIOBuf(tcb, (U32 *) pdsch_sym, pdsch_re * 2 * sizeof(S16), OUT_BUF, NULL);
        FillTCBIOBuf(tcb, (U32 *) expo_fac, sizeof(S16), OUT_BUF, NULL);
    	FillTCBIOBuf(tcb, (U32 *) pdsch_prb_set, sizeof(U8)*pdsch_Nrb*N_TS_SUBFRM, IN_BUF, NULL);
   	 	FillTCBIOBuf(tcb, (U32 *) p_N_pdsch_re, sizeof(U16), OUT_BUF, NULL);
#if NMM_CEVA_DBG
        FillTCBIOBuf(tcb, (U32 *) arm_dbg, 256 * sizeof(char), OUT_BUF, NULL);
#endif

#if NMM_USE_TCB_LIST
        AddTCBToList(tcb, ptask_list, NULL);
#else
        ExeTSchedSingle(tcb);
        tcb = NULL;
#endif
    }
    else
    {
        tcb = refill_tcb;
        context = (U32 *) tcb->ContextPtr;
        *context++ = (U32) expo_nfft;
        *context++ = (U32) bw;
	    *context++ = (U32) pdsch_Nrb;
	    *context++ = (U32) Ntx_ante;
	    *context++ = (U32) cell_cp;
	    *context++ = (U32) isym;
	    *context++ = (U32) cell_id;
	    *context++ = (U32) inv_noise_var;
        *context++ = (U32) subsfn;

    	ReFillTCBIOBuf(tcb, (U32 *) pin, (2 * (1 << expo_nfft)) * sizeof(S16), IN_BUF, 0);
    	ReFillTCBIOBuf(tcb, (U32 *) channelEst_buf, (2 * (1 << expo_nfft)) *Ntx_ante * sizeof(S16), IN_BUF, 1);
    	ReFillTCBIOBuf(tcb, (U32 *) pdsch_sym, pdsch_re * 2 * sizeof(S16), OUT_BUF, 2);
        ReFillTCBIOBuf(tcb, (U32 *) expo_fac, sizeof(S16), OUT_BUF, 3);
    	ReFillTCBIOBuf(tcb, (U32 *) pdsch_prb_set, sizeof(U8)*pdsch_Nrb*N_TS_SUBFRM, IN_BUF, 4);
   	 	ReFillTCBIOBuf(tcb, (U32 *) p_N_pdsch_re, sizeof(U16), OUT_BUF, 5);
#if NMM_CEVA_DBG
        ReFillTCBIOBuf(tcb, (U32 *) arm_dbg, 256 * sizeof(char), OUT_BUF, 6);
#endif
    }

#else
    nmm_dbg_printf_off(("> %s... ", __FUNCTION__));
    lte_bs_phy_nmm_extract_pdsch(expo_nfft, psymb_info->start + psymb_info->cp_size * 2,
								bw, pdsch_Nrb, channelEst_buf, pdsch_sym, expo_fac,Ntx_ante,pdsch_prb_set,cell_cp,
								isym,cell_id,p_N_pdsch_re,inv_noise_var,subsfn);
#endif

    nmm_dbg_printf_off(("done.\r\n"));

    return tcb;
}

#endif


/*
 * TCB Tasks
 * Should be compiled only on Ceva (CEVA_INTRINSICS) or in case of ARM TCBs (!USECEVA)
 */

#if defined CEVA_INTRINSICS || !defined USECEVA


////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE NMM Cell Search Downsample Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_LARM
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_downsampling_task   |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in lte_bs_phy_nmm_tcb.c for the NMM cell search downsample function.<BR><BR>
 *  Will use this if we want to move ceva or MAP functions to the ARM to test if they are causing problems. <BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: lte_bs_phy_nmm_downsampling() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_nmm_cell_search
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void
lte_bs_phy_nmm_downsampling_task(PTCB tcb)
{
    U32            *context = NULL;
    S16            *IQin;
    S32             nIQin;
    S16            *IQout;
    S32             factor;
    S16            *hbfir_real_buf_stage0;
    S16            *hbfir_imag_buf_stage0;
    S16            *hbfir_real_buf_stage1;
    S16            *hbfir_imag_buf_stage1;
    S16            *hbfir_real_buf_stage2;
    S16            *hbfir_imag_buf_stage2;
    S16            *hbfir_real_buf_stage3;
    S16            *hbfir_imag_buf_stage3;
    S16            *hbfir_real_buf_stage4;
    S16            *hbfir_imag_buf_stage4;
	S16				i;

    /* extract context */
    context = (U32 *) tcb->ContextPtr;
    nIQin = (S32) context[0];
    factor = (S32) context[1];

    /* extract IO buffers */
    IQin = (S16 *) EXTRACT_IO_BUF(tcb, 0);
    IQout = (S16 *) EXTRACT_IO_BUF(tcb, 1);
    hbfir_real_buf_stage0 = (S16 *) EXTRACT_IO_BUF(tcb, 2);
    hbfir_imag_buf_stage0 = (S16 *) EXTRACT_IO_BUF(tcb, 3);
    hbfir_real_buf_stage1 = (S16 *) EXTRACT_IO_BUF(tcb, 4);
    hbfir_imag_buf_stage1 = (S16 *) EXTRACT_IO_BUF(tcb, 5);
    hbfir_real_buf_stage2 = (S16 *) EXTRACT_IO_BUF(tcb, 6);
    hbfir_imag_buf_stage2 = (S16 *) EXTRACT_IO_BUF(tcb, 7);
    hbfir_real_buf_stage3 = (S16 *) EXTRACT_IO_BUF(tcb, 8);
    hbfir_imag_buf_stage3 = (S16 *) EXTRACT_IO_BUF(tcb, 9);
    hbfir_real_buf_stage4 = (S16 *) EXTRACT_IO_BUF(tcb, 10);
    hbfir_imag_buf_stage4 = (S16 *) EXTRACT_IO_BUF(tcb, 11);

	for (i = 0; i < nIQin * 2; i++)
		IQin[i] >>= 4;
    /* run function */
#ifdef CEVA_INTRINSICS
    lte_bs_phy_nmm_downsampling_ceva(IQin, nIQin, IQout,
                                     hbfir_real_buf_stage0,
                                     hbfir_real_buf_stage1,
                                     hbfir_real_buf_stage2,  hbfir_real_buf_stage3, hbfir_real_buf_stage4);
#else
    lte_bs_phy_nmm_downsampling(IQin, nIQin, IQout, factor,
                                hbfir_real_buf_stage0, hbfir_imag_buf_stage0,
                                hbfir_real_buf_stage1, hbfir_imag_buf_stage1,
                                hbfir_real_buf_stage2, hbfir_imag_buf_stage2, hbfir_real_buf_stage3, hbfir_imag_buf_stage3, hbfir_real_buf_stage4, hbfir_imag_buf_stage4);
#endif
}


////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE NMM Cell Search AGC Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_LARM
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_agc_task    |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in lte_bs_phy_nmm_tcb.c for the NMM cell search AGC function.<BR><BR>
 *  Will use this if we want to move ceva or MAP functions to the ARM to test if they are causing problems. <BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: lte_bs_phy_nmm_ssync_agc() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_nmm_cell_search
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void
lte_bs_phy_nmm_agc_task(PTCB tcb)
{
    U32            *context = NULL;
    S32            *agc_gain;
    S32            *agc_avg_nrg;
    S16            *dataPtr;
    S16             len;

    /* extract context */
    context = (U32 *) tcb->ContextPtr;
    len = (S16) context[0];

    /* extract IO buffers */
    agc_gain = (S32 *) EXTRACT_IO_BUF(tcb, 0);
    agc_avg_nrg = (S32 *) EXTRACT_IO_BUF(tcb, 1);
    dataPtr = (S16 *) EXTRACT_IO_BUF(tcb, 2);

    /* run function */
#ifdef CEVA_INTRINSICS
    //lte_bs_phy_nmm_ssync_agc_x1641(agc_gain, agc_avg_nrg, in, out, G, pAGC_Eav, len);
    lte_bs_phy_nmm_ssync_agc(agc_gain, agc_avg_nrg, dataPtr,  len);
#else
    lte_bs_phy_nmm_ssync_agc(agc_gain, agc_avg_nrg, dataPtr, len);
#endif
}


////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE NMM Cell Search PSS correlation Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_LARM
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_xcorr_loop_task    |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in lte_bs_phy_nmm_tcb.c for the NMM cell search PSS correlation function.<BR><BR>
 *  Will use this if we want to move ceva or MAP functions to the ARM to test if they are causing problems. <BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: lte_bs_phy_nmm_xcorr_loop() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_nmm_cell_search
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void
lte_bs_phy_nmm_xcorr_loop_task(PTCB tcb)
{
    S16            *xcorr_buf;
    S16            *IQdecimated;
    S16            *zc_cand;
    S16            *xcorr_arr;
	S32			   *agc_gain;
#ifdef CEVA_INTRINSICS
    S16            *IQdecimated_int;
    S16            *zc_cand_int;
    S16            *xcorr_arr_int;
#endif

    /* extract IO buffers */
    xcorr_buf = (S16 *) EXTRACT_IO_BUF(tcb, 0);
    IQdecimated = (S16 *) EXTRACT_IO_BUF(tcb, 1);
    zc_cand = (S16 *) EXTRACT_IO_BUF(tcb, 2);
    xcorr_arr = (S16 *) EXTRACT_IO_BUF(tcb, 3);
    agc_gain = (S32 *) EXTRACT_IO_BUF(tcb, 4);

#ifdef CEVA_INTRINSICS

    IQdecimated_int = (S16 *) DataBuff_Bank1;
    zc_cand_int = IQdecimated_int + (2 * SUBFR_SIZE_096MHZ);
    xcorr_arr_int = zc_cand_int + (2 * FFT_SIZE_096MHZ);

    MemCpyFromExt((void *) IQdecimated_int, (void *) IQdecimated, (2 * SUBFR_SIZE_096MHZ) * sizeof(S16));
    MemCpyFromExt((void *) zc_cand_int, (void *) zc_cand, (2 * FFT_SIZE_096MHZ) * sizeof(S16));
    MemCpyFromExt((void *) xcorr_arr_int, (void *) xcorr_arr, SUBFR_SIZE_096MHZ * sizeof(S16));

    /* run function */
    lte_bs_phy_nmm_xcorr_loop(xcorr_buf, IQdecimated_int, zc_cand_int, xcorr_arr_int, agc_gain);

    MemCpyToExt((void *) xcorr_arr, (void *) xcorr_arr_int, SUBFR_SIZE_096MHZ * sizeof(S16));

#else
    lte_bs_phy_nmm_xcorr_loop(xcorr_buf, IQdecimated, zc_cand, xcorr_arr, agc_gain);
#endif
}


////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE NMM Cell Search PSS detection Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_LARM
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_pss_candidate_selection_loop_task    |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in lte_bs_phy_nmm_tcb.c for the NMM cell search PSS detection function.<BR><BR>
 *  Will use this if we want to move ceva or MAP functions to the ARM to test if they are causing problems. <BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: lte_bs_phy_nmm_pss_candidate_selection_loop() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_nmm_cell_search
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void
lte_bs_phy_nmm_pss_candidate_selection_loop_task(PTCB tcb)
{
    S16            *psync_found_arr;
    S16            *xcorr_arr_ptr;

    /* extract IO buffers */
    xcorr_arr_ptr = (S16 *) EXTRACT_IO_BUF(tcb, 0);
    psync_found_arr = (S16 *) EXTRACT_IO_BUF(tcb, 1);

#ifdef CEVA_INTRINSICS
    /* run function */
    lte_bs_phy_nmm_pss_candidate_selection_loop(xcorr_arr_ptr, psync_found_arr);

#else
    lte_bs_phy_nmm_pss_candidate_selection_loop(xcorr_arr_ptr, psync_found_arr);
#endif
}


////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE NMM PSS frequency offset estimation Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_LARM
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_pss_foe_task    |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in lte_bs_phy_nmm_tcb.c for the PSS frequency offset estimation function.<BR><BR>
 *  Will use this if we want to move ceva or MAP functions to the ARM to test if they are causing problems. <BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: lte_bs_phy_nmm_pss_foe() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_nmm_cell_search
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void
lte_bs_phy_nmm_pss_foe_task(PTCB tcb)
{
    S16            *pin;
    S16            *pZC;
    S32            *PSync_foe;


    /* extract IO buffers */
    pin = (S16 *) EXTRACT_IO_BUF(tcb, 0);
    pZC = (S16 *) EXTRACT_IO_BUF(tcb, 1);
    PSync_foe = (S32 *) EXTRACT_IO_BUF(tcb, 2);

#ifdef CEVA_INTRINSICS

    /* run function */
    lte_bs_phy_nmm_pss_foe(pin, pZC, PSync_foe);

#else
    lte_bs_phy_nmm_pss_foe(pin, pZC, PSync_foe);

#endif
}


////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE NMM frequency offset compensation Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_LARM
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_freqOffset_comp_task    |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in lte_bs_phy_nmm_tcb.c for the NMM frequency offset compensation function.<BR><BR>
 *  Will use this if we want to move ceva or MAP functions to the ARM to test if they are causing problems. <BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: lte_bs_phy_nmm_freqOffset_comp() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_nmm_cell_search
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void
lte_bs_phy_nmm_freqOffset_comp_task(PTCB tcb)
{
    U32            *context = NULL;
    S16            *pin;
	S32			   foeEst;
	S16			   symbolPos;
	S16			   length;

    /* extract context */
    context = (U32 *) tcb->ContextPtr;
    foeEst  = (S32) context[0];
    symbolPos = (S16) context[1];
    length  = (S16) context[2];

    /* extract IO buffers */
    pin = (S16 *) EXTRACT_IO_BUF(tcb, 0);

#ifdef CEVA_INTRINSICS

    /* run function */
    lte_bs_phy_nmm_freqOffset_comp(pin, L_shr(foeEst, 13), symbolPos, length);

#else
    lte_bs_phy_nmm_freqOffset_comp(pin, foeEst >> 13, symbolPos, length);

#endif
}


////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE NMM 64-point FFT Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_LARM
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_fft64_task    |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in lte_bs_phy_nmm_tcb.c for the NMM 64-point FFT on PSS and SSS function.<BR><BR>
 *  Will use this if we want to move ceva or MAP functions to the ARM to test if they are causing problems. <BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: cfft_16t32d_case2() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_nmm_cell_search
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void
lte_bs_phy_nmm_fft64_task(PTCB tcb)
{
    S16            *timedomain;
    S16             m;
    S16            *freqdomain;
    S16            *expo;
#ifdef CEVA_INTRINSICS
    S16            *timedomain_ceva;
    S16            *freqdomain_ceva;
    S16            *expo_ceva;
#endif

    m = 6;

    /* extract IO buffers */
    timedomain = (S16 *) EXTRACT_IO_BUF(tcb, 0);
    freqdomain = (S16 *) EXTRACT_IO_BUF(tcb, 1);
    expo = (S16 *) EXTRACT_IO_BUF(tcb, 2);

#ifdef CEVA_INTRINSICS

    timedomain_ceva = (S16 *) DataBuff_Bank1;
    freqdomain_ceva = timedomain_ceva + (2 * FFT_SIZE_096MHZ);
    expo_ceva = freqdomain_ceva + (2 * FFT_SIZE_096MHZ);

    MemCpyFromExt((void *) timedomain_ceva, (void *) timedomain, EXTRACTTCBIOBUFSZ(tcb, 0));

    /* run function */
    cfft_16t32d(timedomain_ceva, m, freqdomain_ceva, expo_ceva);

    MemCpyToExt((void *) freqdomain, (void *) freqdomain_ceva, EXTRACTTCBIOBUFSZ(tcb, 1));
    MemCpyToExt((void *) expo, (void *) expo_ceva, EXTRACTTCBIOBUFSZ(tcb, 2));

#else
    cfft_16t32d_case2(timedomain, m, freqdomain, expo);
#endif
}


////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE NMM SSS demodulation Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_LARM
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_chanest_task    |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in lte_bs_phy_nmm_tcb.c for the NMM SSS demodulation function.<BR><BR>
 *  Will use this if we want to move ceva or MAP functions to the ARM to test if they are causing problems. <BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: lte_bs_phy_nmm_ssync_est_channel_estimate() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_nmm_cell_search
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void
lte_bs_phy_nmm_chanest_task(PTCB tcb)
{
    U32            *context = NULL;
    S16             lp_cp_flag;
    S16            *SSync_f;
    S16            *PSync_f;
    S16            *exp_hsphs;
    S16            *pZC;
    S16            *SSyncEst;
    S16             PSS_root;

    /* extract context */
    context = (U32 *) tcb->ContextPtr;
    PSS_root = (S16) context[0];
    lp_cp_flag = (S16) context[0];

    /* extract IO buffers */
    SSync_f = (S16 *) EXTRACT_IO_BUF(tcb, 0);
    PSync_f = (S16 *) EXTRACT_IO_BUF(tcb, 1);
    exp_hsphs = (S16 *) EXTRACT_IO_BUF(tcb, 2);
    pZC = (S16 *) EXTRACT_IO_BUF(tcb, 3);
    SSyncEst = (S16 *) EXTRACT_IO_BUF(tcb, 4);

    /* run function */
#ifdef CEVA_INTRINSICS
    lte_bs_phy_nmm_ssync_est_channel_estimate_i1641(SSync_f, PSync_f, exp_hsphs, pZC, SSyncEst, lp_cp_flag);
//    lte_bs_phy_nmm_ssync_est_channel_estimate(SSync_f, PSync_f, exp_hsphs, SSyncEst, PSS_root, lp_cp_flag);
#else
    lte_bs_phy_nmm_ssync_est_channel_estimate(SSync_f, PSync_f, exp_hsphs, SSyncEst, PSS_root, lp_cp_flag);
#endif
}


////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE NMM Channel Estimation Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_LARM
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_channel_estimate_task    |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in lte_bs_phy_nmm_tcb.c for the NMM channel estimation function.<BR><BR>
 *  Will use this if we want to move ceva or MAP functions to the ARM to test if they are causing problems. <BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: lte_bs_phy_nmm_channel_estimate() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_nmm_phy
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void
lte_bs_phy_nmm_channel_estimate_task(PTCB tcb)
{
    U32            *context = NULL;
    S16             ts_num;
    S16             cell_pci;
    S16             cell_cp;
    S16             Ndl_rb;
    S16             expoNfft;
    S16            *pIn;
    S16            *chEst_cram;
    S32            *foeEst;

    U16             Ntx_ante;
    U32             *inv_noise_var = NULL; //!!!
    /* extract context */
    context  = (U32 *) tcb->ContextPtr;
    ts_num   = (S16) context[0];
    cell_pci = (S16) context[1];
    cell_cp  = (S16) context[2];
    Ndl_rb   = (S16) context[3];
    expoNfft= (S16) context[4];
    Ntx_ante = (U16)context[5];

    /* extract IO buffers */
    pIn = (S16 *) EXTRACT_IO_BUF(tcb, 0);
    chEst_cram = (S16 *) EXTRACT_IO_BUF(tcb, 1);
	inv_noise_var = (U32*)EXTRACT_IO_BUF(tcb,2);
    foeEst = (S32 *) EXTRACT_IO_BUF(tcb, 3);

    /* run function */

    lte_bs_phy_nmm_channel_estimate(pIn, ts_num, cell_pci, cell_cp, Ndl_rb, expoNfft,
									chEst_cram, Ntx_ante,inv_noise_var,foeEst);

}


////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE NMM Cell Search SSS Detection Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_LARM
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_decode_sss_task    |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in lte_bs_phy_nmm_tcb.c for the NMM cell search SSS detection function.<BR><BR>
 *  Will use this if we want to move ceva or MAP functions to the ARM to test if they are causing problems. <BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: lte_bs_phy_nmm_ssync_decode_sss() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_nmm_cell_search
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void
lte_bs_phy_nmm_decode_sss_task(PTCB tcb)
{
    U32            *context = NULL;
    S16            *SSyncEst;
    S16             NID_2;
    S16            *NID_cell;
    S16            *Subframe;
    S16 *s0_ref;

    /* extract context */
    context = (U32 *) tcb->ContextPtr;
    NID_2 = (S16) context[0];

    /* extract IO buffers */
    SSyncEst = (S16 *) EXTRACT_IO_BUF(tcb, 0);
    s0_ref = (S16 *) EXTRACT_IO_BUF(tcb, 1);
    NID_cell = (S16 *) EXTRACT_IO_BUF(tcb, 2);
    Subframe = (S16 *) EXTRACT_IO_BUF(tcb, 3);

    /* run function */
#ifdef CEVA_INTRINSICS
    lte_bs_phy_nmm_ssync_decode_sss(SSyncEst, s0_ref, NID_2, NID_cell, Subframe);
    //lte_bs_phy_nmm_ssync_decode_sss_ceva(SSyncEst, NID_2, NID_cell, Subframe);
#else
    lte_bs_phy_nmm_ssync_decode_sss(SSyncEst, s0_ref, NID_2, NID_cell, Subframe);
#ifdef __CC_ARM
    nmm_dbg_printf_off(("NID_cell %d\r\n", *NID_cell));
#endif
#endif
}


////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE NMM Cell Search RSRP measurement Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_LARM
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_get_rsrp_task    |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in lte_bs_phy_nmm_tcb.c for the NMM cell search RSRP measurement function.<BR><BR>
 *  Will use this if we want to move ceva or MAP functions to the ARM to test if they are causing problems. <BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: lte_bs_phy_nmm_get_rsrp() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_nmm_cell_search
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void
lte_bs_phy_nmm_get_rsrp_task(PTCB tcb)
{
    U32            *context = NULL;
    S16             expoNfft;
    S16            *pin;
    S16             cell_pci,
                    cell_cp,
                    cell_search_meas_bandwidth,
                    num_pilots,
                    size_inverse,
                    symb_index,
					rsrp_scale;
    S32            *rsrp;

    context = (U32 *) tcb->ContextPtr;
    expoNfft = (S16) * context++;
    cell_pci = (S16) * context++;
    cell_cp = (S16) * context++;
    cell_search_meas_bandwidth = (S16) * context++;
    num_pilots = (S16) * context++;
    size_inverse = (S16) * context++;
    symb_index = (S16) * context++;
    rsrp_scale = (S16) * context++;

    pin = (S16 *) EXTRACT_IO_BUF(tcb, 0);
    rsrp = (S32 *) EXTRACT_IO_BUF(tcb, 1);
#if NMM_CEVA_DBG
    ceva_dbg = (char *) EXTRACT_IO_BUF(tcb, 2);
#endif

    lte_bs_phy_nmm_get_rsrp(expoNfft, pin, symb_index, cell_pci, cell_cp, cell_search_meas_bandwidth, rsrp, rsrp_scale, num_pilots, size_inverse);
#ifdef __CC_ARM
    nmm_dbg_printf_off(("%s exp %d pci %d cp %d bw %d num_pilots %d size_inverse %d rsrp %d\r\n", __FUNCTION__, expoNfft, cell_pci, cell_cp, cell_search_meas_bandwidth,
                    num_pilots, size_inverse, *rsrp));
#endif
}


////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE NMM PseudoRandom Sequence Generation Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_LARM
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_pseudo_random_seq_gen_task    |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in lte_bs_phy_nmm_tcb.c for the NMM PseudoRandom sequence generation function.<BR><BR>
 *  Will use this if we want to move ceva or MAP functions to the ARM to test if they are causing problems. <BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: PseudoRandomSeqGen() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_nmm_phy
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void
lte_bs_phy_nmm_pseudo_random_seq_gen_task(PTCB tcb)
{
    U32            *context = NULL;
    S32             N;
    U32             cinit;
    U8             *c;

    /* extract context */
    context = (U32 *) tcb->ContextPtr;
    N = (S32) context[0];
    cinit = (U32) context[1];

    /* extract IO buffers */
    c = (U8 *) EXTRACT_IO_BUF(tcb, 0);

    /* run function */
    PseudoRandomSeqGen(N, cinit, c);
}


////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE NMM PBCH Extraction Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_LARM
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_extract_pbch_task    |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in lte_bs_phy_nmm_tcb.c for the NMM PBCH extraction function.<BR><BR>
 *  Will use this if we want to move ceva or MAP functions to the ARM to test if they are causing problems. <BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: lte_bs_phy_nmm_extract_pbch() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_nmm_MIB
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void
lte_bs_phy_nmm_extract_pbch_task(PTCB tcb)
{
    U32            *context = NULL;
    S16             expo_nfft;
    S16            *pin,
                   *channelEst_buf;
    S16             cell_pci,
                    cell_cp,
                    symb_index;
    S16            *pbch,
                   *expo_fac;

    U16            Ntx_ante;
    context = (U32 *) tcb->ContextPtr;
    expo_nfft = (S16) * context++;
    cell_pci = (S16) * context++;
    cell_cp = (S16) * context++;
    symb_index = (S16) * context++;
    Ntx_ante = (U16) * context++;

    pin = (S16 *) EXTRACT_IO_BUF(tcb, 0);
    channelEst_buf = (S16 *) EXTRACT_IO_BUF(tcb, 1);
    pbch = (S16 *) EXTRACT_IO_BUF(tcb, 2);
    expo_fac = (S16 *) EXTRACT_IO_BUF(tcb, 3);
#if NMM_CEVA_DBG
    ceva_dbg = (char *) EXTRACT_IO_BUF(tcb, 4);
#endif

    lte_bs_phy_nmm_extract_pbch(expo_nfft, pin, symb_index, cell_pci, cell_cp,
    							channelEst_buf, pbch, expo_fac,Ntx_ante,0);

}


////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE NMM PBCH Demodulation Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_LARM
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_demod_pbch_task    |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in lte_bs_phy_nmm_tcb.c for the NMM PBCH demodulation function.<BR><BR>
 *  Will use this if we want to move ceva or MAP functions to the ARM to test if they are causing problems. <BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: lte_bs_phy_nmm_demod_pbch() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_nmm_MIB
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void
lte_bs_phy_nmm_demod_pbch_task(PTCB tcb)
{
    U32            *context = NULL;
    S16            *pbch;
    S16            *pbch_scale;
    S8             *bdet_soft;
	S16				pbch_re_per_frame;

    context = (U32 *) tcb->ContextPtr;
    pbch_re_per_frame = (S16) * context++;

    /* extract IO buffers */
    pbch = (S16 *) EXTRACT_IO_BUF(tcb, 0);
    pbch_scale = (S16 *) EXTRACT_IO_BUF(tcb, 1);
    bdet_soft = (S8 *) EXTRACT_IO_BUF(tcb, 2);

    /* run function */
    lte_bs_phy_nmm_demod_pbch(pbch, pbch_re_per_frame, pbch_scale, bdet_soft);
}


////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE NMM PCFI extraction Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_LARM
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_extract_pcfi_task    |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in lte_bs_phy_nmm_tcb.c for the NMM PCFI extraction function.<BR><BR>
 *  Will use this if we want to move ceva or MAP functions to the ARM to test if they are causing problems. <BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: lte_bs_phy_nmm_extract_pcfi() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_nmm_SIB
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void
lte_bs_phy_nmm_extract_pcfi_task(PTCB tcb)
{
    U32            *context = NULL;
    S16             expo_nfft;
    S16            *pin,
                   *channelEst_buf;
    S16             bw;
    S16            *pcfi_re_idx;
    S16            *pcfi;
    S16            *expo_fac;

    U16             Ntx_ante;
    U32             inv_noise_var = 0;
    context = (U32 *) tcb->ContextPtr;
    expo_nfft = (S16) * context++;
    bw = (S16) * context++;
    Ntx_ante = (U16) * context++;

    /* extract IO buffers */
    pin = (S16 *) EXTRACT_IO_BUF(tcb, 0);
    pcfi_re_idx = (S16 *) EXTRACT_IO_BUF(tcb, 1);
    channelEst_buf = (S16 *) EXTRACT_IO_BUF(tcb, 2);
    pcfi = (S16 *) EXTRACT_IO_BUF(tcb, 3);
    expo_fac = (S16 *) EXTRACT_IO_BUF(tcb, 4);

    /* run function */
   lte_bs_phy_nmm_extract_pcfi(expo_nfft, pin, bw, pcfi_re_idx, channelEst_buf,
        pcfi, expo_fac,Ntx_ante,inv_noise_var);

}


////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE NMM PDCCH Extraction Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_LARM
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_extract_pdcch_task    |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in lte_bs_phy_nmm_tcb.c for the NMM PDCCH extraction function.<BR><BR>
 *  Will use this if we want to move ceva or MAP functions to the ARM to test if they are causing problems. <BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: lte_bs_phy_nmm_extract_pdcch() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_nmm_SIB
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void
lte_bs_phy_nmm_extract_pdcch_task(PTCB tcb)
{
    U32            *context = NULL;
    S16             expo_nfft;
    S16            *pin,
                   *channelEst_buf;
    S16             bw;
    S16             num_pdcch_re;
    S16            *pdcch_re_idx;
    S16            *pdcch;
    S16            *expo_fac;

    U16             Ntx_ante;
    U32 inv_noise_var = 0;
    context = (U32 *) tcb->ContextPtr;
    expo_nfft = (S16) * context++;
    bw = (S16) * context++;
    num_pdcch_re = (S16) * context++;
    Ntx_ante = (U16) * context++;

    /* extract IO buffers */
    pin = (S16 *) EXTRACT_IO_BUF(tcb, 0);
    pdcch_re_idx = (S16 *) EXTRACT_IO_BUF(tcb, 1);
    channelEst_buf = (S16 *) EXTRACT_IO_BUF(tcb, 2);
    pdcch = (S16 *) EXTRACT_IO_BUF(tcb, 3);
    expo_fac = (S16 *) EXTRACT_IO_BUF(tcb, 4);

    /* run function */

    lte_bs_phy_nmm_extract_pdcch(expo_nfft, pin, bw, pdcch_re_idx, num_pdcch_re,
        channelEst_buf, pdcch, expo_fac,Ntx_ante,inv_noise_var);

}


////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE NMM PDSCH Extraction Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_LARM
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_extract_pdsch_task    |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in lte_bs_phy_nmm_tcb.c for the NMM PDSCH extraction function.<BR><BR>
 *  Will use this if we want to move ceva or MAP functions to the ARM to test if they are causing problems. <BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: lte_bs_phy_nmm_extract_pdsch() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_nmm_SIB
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void
lte_bs_phy_nmm_extract_pdsch_task(PTCB tcb)
{
    U32            *context = NULL;
    S16             expo_nfft;
    S16            *pin,
                   *channelEst_buf;
    S16             bw;
    S16             num_pdsch_rb;
    S16            *pdsch;
    S16            *expo_fac;

    U16             Ntx_ante;
    U8              *PDSCH_prb_set =  NULL;
    U16             cell_cp = 0,isym=0,cell_id=0,*p_N_pdsch_re=NULL;
    U32             inv_noise_var;
    U16             subsfn;
    context = (U32 *) tcb->ContextPtr;
    expo_nfft = (S16) * context++;
    bw = (S16) * context++;
    num_pdsch_rb = (S16) * context++;
    Ntx_ante = (U16) *context++ ;
    cell_cp = (U16) *context++ ;
    isym = (U16) *context++ ;
    cell_id = (U16) *context++ ;
    inv_noise_var = (U32) *context++ ;
    subsfn = (U16)*context++ ;

    /* extract IO buffers */
    pin = (S16 *) EXTRACT_IO_BUF(tcb, 0);
    channelEst_buf = (S16 *) EXTRACT_IO_BUF(tcb, 1);
    pdsch = (S16 *) EXTRACT_IO_BUF(tcb, 2);
    expo_fac = (S16 *) EXTRACT_IO_BUF(tcb, 3);
    PDSCH_prb_set = (U8*)EXTRACT_IO_BUF(tcb, 4);
    p_N_pdsch_re = (U16*)EXTRACT_IO_BUF(tcb, 5);
    /* run function */

    lte_bs_phy_nmm_extract_pdsch(expo_nfft, pin, bw, num_pdsch_rb, channelEst_buf,
        pdsch, expo_fac,Ntx_ante,PDSCH_prb_set,cell_cp,isym,cell_id,p_N_pdsch_re,
        inv_noise_var,subsfn);

}

#endif


////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE NMM PDCCH Demodulation Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_LARM
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_pdcch_demod_task    |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in lte_bs_phy_nmm_tcb.c for the NMM PDCCH demodulation function.<BR><BR>
 *  Will use this if we want to move ceva or MAP functions to the ARM to test if they are causing problems. <BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: lte_bs_phy_nmm_pdcch_demod() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_nmm_SIB
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
#if ORIG_CODE
void
lte_bs_phy_nmm_pdcch_demod_task(PTCB tcb)
{
    U32            *context = NULL;
    S32            *pdcch32;
    S16             NusedE;
    S8             *bDetSoft;
    S8             *bDetHard;
    S16             pdcch_re_num;
    U32             cinit;
    U8             *pdcch_scrambling_seq;

    context = (U32 *) tcb->ContextPtr;
    NusedE = (S16) * context++;
    pdcch_re_num = (S16) * context++;
    cinit = (U32) * context++;

    /* extract IO buffers */
    pdcch32 = (S32 *) EXTRACT_IO_BUF(tcb, 0);
    bDetSoft = (S8 *) EXTRACT_IO_BUF(tcb, 1);
    pdcch_scrambling_seq = (U8 *) EXTRACT_IO_BUF(tcb, 2);

    /* run function */
    lte_bs_phy_nmm_pdcch_demod(pdcch32, NusedE, bDetSoft, cinit, pdcch_scrambling_seq);
}
#else
void
lte_bs_phy_nmm_pdcch_demod_task(PTCB tcb)
{
    U32            *context = NULL;
    S16            *pdcch;
    S16             NusedE;
    S16             pci;
    S8             *bDetSoft;
    //S16             pdcch_re_num;
    U32             cinit;
    U8             *pdcch_scrambling_seq;

    context = (U32 *) tcb->ContextPtr;
    NusedE = (S16) * context++;
    pci = (S16) * context++;
    context++; //pdcch_re_num
    cinit = (U32) * context++;

    /* extract IO buffers */
    pdcch = (S16 *) EXTRACT_IO_BUF(tcb, 0);
    bDetSoft = (S8 *) EXTRACT_IO_BUF(tcb, 1);
    pdcch_scrambling_seq = (U8 *) EXTRACT_IO_BUF(tcb, 2);

    /* run function */
    lte_bs_phy_nmm_pdcch_demod(pdcch, NusedE, pci, bDetSoft, cinit, pdcch_scrambling_seq);
}
#endif

