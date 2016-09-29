/** @file LteMacTestCfgMgr.c
 *
 * @brief Controler of 36.141 test
 * @author Mindspeed Technologies
 * @version $Revision: 1.231 $
 *
 * COPYRIGHT(c) 2008-2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

/****************************************************************************
* Project Includes
****************************************************************************/

#ifdef MX_ARM_ENABLED
#include <string.h>
#include "4gmx_queue.h"
#include "supervisor.h"
#else
#include "osal.h"
#include "basetypes.h"
#include "resultcodes.h"
#endif

#include "bsmac.h"
#include "LteMacTestCfgMgr.h"

#ifdef MX_ARM_ENABLED
#include "enbMac2PhyIface.h"
#include "enbMacIface.h"
#else
#include "LteMac2PhyInterface.h"
#endif

#include "LteBsMacDlScheduler.h"
#include "LteBsMacUlScheduler.h"
#include "LteTestTables.h"
#include "bs_static_config.h"

#ifdef MX_ARM_ENABLED
#include "dci.h"
#include "MacUtils.h"
#else
#include "timers.h"
#include "msgroute.h"
#include "TableTdd.h"
#endif

#ifndef AC_MSPD_TDD
#include "LtePhyL2Api.h"
#else
#include "LtePhyL2Api_TDD.h"
#endif

#include "lte_mac_typedef.h"

#include "lte_mac_state.h"
#include "lte_pdsch.h"
#include "lte_pusch.h"

#include "lte_pdcch.h"
#include "lte_pucch.h"

#include "lte_phich.h"
#include "lte_pbch.h"
#include "lte_rnti.h"
#include "lte_modulation.h"
#include "lte_etm.h"

#ifndef MX_ARM_ENABLED
#include "udp_stream.h"
#endif

#include "e_tm.h"
#include "lte_etm.h"

E_TM_CONFIG etm_cfg;


/****************************************************************************
Private Definitions
****************************************************************************/

#define MINUS_INF_Q8        (-128 * 256)
#define FLOAT2Q8(x)         (S32)(x * 256)
#define Q82FLOAT(x)         (float)(x / 256)

#define CONSTANT_BYTES_OF_DCIFORMAT_2     23

// TODO: redefined for END
#ifndef ROUND
#define ROUND(x, dx)  (((x) + ((dx) - 1) ) & ~((dx) - 1))
#endif

/****************************************************************************
Private Types
****************************************************************************/

char TEST_MGR_PREDEF_CONFIG_STR[TEST_MGR_CONFIG_MAX_ID][64] =
{
    "TEST_MGR_NO_CONFIG",                          // 0
    "TEST_MGR_10MHZ_1ANT_RT",                      // 1
    "TEST_MGR_10MHZ_1ANT_RT_MAX_THROUGHPUT",       // 2
    "TEST_MGR_10MHZ_1ANT_RT_NO_REPACK",            // 3
    "TEST_MGR_20MHZ_1ANT_RT",                      // 4
    "TEST_MGR_20MHZ_1ANT_RT_MAX_THROUGHPUT",       // 5
    "TEST_MGR_TM500",                              // 6
    "TEST_MGR_1_10MHz_2ANT",                       // 7
    "TEST_MGR_1_20MHz_2ANT",                       // 8
    "TEST_MGR_10MHZ_1ANT_RT_MAX_QAM16",            // 9
    "TEST_MGR_10MHZ_1ANT_RT_QAM16_QAM64_2SDU",     // 10
    "TEST_MGR_10MHZ_1ANT_RT_QAM64_25_50",          // 11
    "TEST_MGR_10MHz_2ANT_1CW",                     // 12
    "TEST_MGR_20MHz_2ANT_BCH",                     // 13
    "TEST_MGR_20MHz_2ANT_BCH_PDCCH",               // 14
    "TEST_MGR_20MHz_2ANT_SPMUX",                   // 15
    "TEST_MGR_20MHz_2ANT_DIVERSITY",               // 16
    "TEST_HARQ",                                   // 17
    "TEST_HARQ_OPTIMIZATION_QAM64",                // 18
    "TEST_HARQ_OPTIMIZATION_QAM16",                // 19
    "TEST_1TXHISDU",                               // 20
    "TEST_1TXDCIULSDU",                            // 21
    "TEST_4HISDU_R1TXDCIULSDU",                    // 22
    "TEST_MGR_20MHz_2ANT_SPMUX_RT39",              // 23
    "TEST_MGR_20MHz_2ANT_SPMUX_RT43",              // 24
    "TEST_10MHz_XDL_XUL_HARQ_RT45",                // 25
    "TEST_6UE_1ANT_1LAYER_10MHz_MAX_THROUGHPUT",   // 26
    "TEST_6UE_1ANT_1LAYER_20MHz_MAX_THROUGHPUT",   // 27
    "TEST_6UE_2ANT_2LAYER_10MHz_MAX_THROUGHPUT",   // 28
    "TEST_6UE_2ANT_2LAYER_20MHz_MAX_THROUGHPUT",   // 29
    "TEST_MGR_TM500_CTRL",                         // 30
    "TEST_MGR_10MHz_2ANT_SPMUX",                   // 31
    "TEST_4UE_10MHz_1ANT_MAX_THROUGHPUT",          // 32

    "TEST_4UE_10Mhz_1ANT_MAX_THROUGHPUT_1",        // 34
    "TEST_4UE_10Mhz_1ANT_MAX_THROUGHPUT_2",        // 35
    "TEST_4UE_10Mhz_1ANT_MAX_THROUGHPUT_3",        // 36
    "TEST_4UE_10Mhz_1ANT_MAX_THROUGHPUT_4",        // 37

    "TEST_3UE_10Mhz_1ANT_MAX_THROUGHPUT_1",        // 38
    "TEST_3UE_10Mhz_1ANT_MAX_THROUGHPUT_2",        // 39
    "TEST_3UE_10Mhz_1ANT_MAX_THROUGHPUT_3",        // 40
    "TEST_3UE_10Mhz_1ANT_MAX_THROUGHPUT_4",        // 41

    "TEST_2UE_10Mhz_1ANT_MAX_THROUGHPUT_1",        // 42
    "TEST_2UE_10Mhz_1ANT_MAX_THROUGHPUT_2",        // 43
    "TEST_2UE_10Mhz_1ANT_MAX_THROUGHPUT_3",        // 44
    "TEST_2UE_10Mhz_1ANT_MAX_THROUGHPUT_4",        // 45

    "TEST_8UE_10Mhz_2ANT_MAX_THROUGHPUT_1",        // 46
    "TEST_5UE_10Mhz_2ANT_MAX_THROUGHPUT_2",        // 47

    "TEST_4UE_10Mhz_2ANT_CQI_HARQ",                // 48
    "TEST_4UE_10Mhz_2ANT_HARQ",                    // 49

    "TEST_1UE_10Mhz_1ANT_UDP_MPEG2",               // 50
    "TEST_1UE_10Mhz_1ANT_UDP_MPEG2_DIRECT",        // 51

    "TEST_xUE_10Mhz_1ANT_VAR_THROUGHPUT",           // 52

    "TEST_2Pdsch2AntSpatialMuxQAM64_2Pusch2AntQAM16_10MHZ", //53
    "TEST_3Pdsch2AntSpatialMuxQAM64_3Pusch2AntQAM16_10MHZ", //54
    "TEST_4Pdsch2AntSpatialMuxQAM64_4Pusch2AntQAM16_10MHZ", //55
    
    "TEST_MGR_1_10MHz_2TXANT_2RXANT",                       //57
    "TEST_MGR_1_10MHz_2ANT_HDX",                            //58
    "TEST_MGR_1_10MHz_2TXANT_2RXANT_HDX",                   //59
    
    "TEST_ETM_LIB"                                          // 60
};

int TEST_MGR_PREDEF_CONFIG_DETAILS[TEST_MGR_CONFIG_MAX_ID][11]=
{
        {1, SINGLEANT, 1, 3, 1, 1, 10, 4, 0, 0, 0},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
};

extern U8 MCSIndex_TableDl[4][2];
extern U8 MCSIndex_TableUl[4];

tGetRvi pGetRvi = NULL;

tTestMgrGetPdschMap pTestMgrGetBoostedPdsch = NULL;
tTestMgrGetPdschMap pTestMgrGetDeBoostedPdsch = NULL;

tTestMgrGetPdcch pTestMgrGetPdcch = NULL;

tTestMgrGetPusch pTestMgrGetPusch = NULL;

tTestMgrGetPucch pTestMgrGetPucch = NULL;

lte_pdsch_set_payload_cb   plte_pdsch_set_payload = lte_pdsch_set_payload;

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

typedef struct
{
    U8                  mcs_index;
    U8                  mod_order;
    U8                  tbs_index;
}PREDEF_MCS_INDEX_TABLE_DL;


typedef struct
{
    U8                  mcs_index;
    U8                  mod_order;
    U8                  tbs_index;
    U8                  red_version;
}PREDEF_MCS_INDEX_TABLE_UL;


PREDEF_MCS_INDEX_TABLE_UL predef_mcs_index_table_ul[32] = {
    {0,  QPSK,  0,  0},
    {1,  QPSK,  1,  0},
    {2,  QPSK,  2,  0},
    {3,  QPSK,  3,  0},
    {4,  QPSK,  4,  0},
    {5,  QPSK,  5,  0},
    {6,  QPSK,  6,  0},
    {7,  QPSK,  7,  0},
    {8,  QPSK,  8,  0},
    {9,  QPSK,  9,  0},
    {10, QPSK,  10, 0},
    {11, QAM16, 10, 0},
    {12, QAM16, 11, 0},
    {13, QAM16, 12, 0},
    {14, QAM16, 13, 0},
    {15, QAM16, 14, 0},
    {16, QAM16, 15, 0},
    {17, QAM16, 16, 0},
    {18, QAM16, 17, 0},
    {19, QAM16, 18, 0},
    {20, QAM16, 19, 0},
    {21, QAM64, 19, 0},
    {22, QAM64, 20, 0},
    {23, QAM64, 21, 0},
    {24, QAM64, 22, 0},
    {25, QAM64, 23, 0},
    {26, QAM64, 24, 0},
    {27, QAM64, 25, 0},
    {28, QAM64, 26, 0},
    {29, QAM64,  0, 1},
    {30, QAM64,  0, 2},
    {31, QAM64,  0, 3}
};

PREDEF_MCS_INDEX_TABLE_UL predef_mcs_index_table_dl[32] = {
    {0,  QPSK,  0},
    {1,  QPSK,  1},
    {2,  QPSK,  2},
    {3,  QPSK,  3},
    {4,  QPSK,  4},
    {5,  QPSK,  5},
    {6,  QPSK,  6},
    {7,  QPSK,  7},
    {8,  QPSK,  8},
    {9,  QPSK,  9},
    {10, QAM16, 9},
    {11, QAM16, 10},
    {12, QAM16, 11},
    {13, QAM16, 12},
    {14, QAM16, 13},
    {15, QAM16, 14},
    {16, QAM16, 15},
    {17, QAM64, 15},
    {18, QAM64, 16},
    {19, QAM64, 17},
    {20, QAM64, 18},
    {21, QAM64, 19},
    {22, QAM64, 20},
    {23, QAM64, 21},
    {24, QAM64, 22},
    {25, QAM64, 23},
    {26, QAM64, 24},
    {27, QAM64, 25},
    {28, QAM64, 26},
    {29, QPSK,  255},
    {30, QAM16, 255},
    {31, QAM64, 255}
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/


/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/

TEST_MGR_CTX                testMgrCtx;
BS_CONFOR_TEST_CONFIG       testConfConfig;

BS_STATIC_CONFIG            StaticConfig;
DCI_INFO                    dci_info[MAX_PDSCH_CHANNELS];

PBS_STATIC_CONFIG clr_static_config(void)
{
    memset((char *)&StaticConfig, 0x00, sizeof(BS_STATIC_CONFIG));
    return &StaticConfig;
}

BS_CONFOR_TEST_CONFIG* get_bs_conf_test_config(void)
{
    return (&testConfConfig);
}

PBS_STATIC_CONFIG get_static_config(void)
{
    return &StaticConfig;
}

DCI_INFO* get_dci_info(U32 ch_idx)
{
    return &dci_info[ch_idx];
}


U32 rbm_find_all_gaps(RBMap *rbm, U32 vrb, U32 *gap_offsets, U32 *gap_sizes)
{
    // if vrb = 1, then caller must care about prb_num being compliant with virtual
    U32 i, gap_offset, gap_size, gap_idx, n_step_rb;

    gap_idx = 0;
    gap_size = 0;

    n_step_rb = get_n_step_rb(rbm->max_rbs);
    gap_offset = 0;
    // find all gaps
    for ( i = 0; i != rbm->max_rbs; ++i )
    {
        if ( rbm->ids[i] == RB_UNASSIGNED )
        {
            if ( !gap_size )
            {
                // skip this offset if not compliant to virtual
                if ( vrb && (i % n_step_rb) )
                    continue;
                gap_offset = i; // remember gap start
            }
            gap_size++;
        }
        else if ( gap_size )
        {
            gap_sizes[gap_idx] = gap_size;
            gap_offsets[gap_idx] = gap_offset;
            ++gap_idx;
            gap_size = 0;
        }
    }

    if ( gap_size )
    {
        gap_sizes[gap_idx] = gap_size;
        gap_offsets[gap_idx] = gap_offset;
        ++gap_idx;
    }
    return gap_idx;
}

U32 rbm_find_gap(RBMap *rbm, U32 prb_num, U8 vrb)
{
    // walk through entire map and find the gap
    // which is most suitable for required size

    // if vrb = 1, then caller must care about prb_num being compliant with virtual
    U32 i, gap_offset, gap_size, gaps_num;

    U32 gap_offsets[110];
    U32 gap_sizes[110];

    // find all gaps
    gaps_num = rbm_find_all_gaps(rbm, vrb, &gap_offsets[0], &gap_sizes[0]);

    gap_offset = 0;
    gap_size = RB_UNASSIGNED;
    // find the best gap
    for ( i = 0; i != gaps_num; ++i )
    {
        if ( gap_sizes[i] < prb_num ) // gap is smaller than needed
            continue;
        if ( gap_sizes[i] == prb_num ) // gap is exactly of required size
            return gap_offsets[i];

        // looking for smallest most suitable gap
        if ( gap_sizes[i] < gap_size )
        {
            gap_offset = gap_offsets[i];
            gap_size = gap_sizes[i];
        }
    }
    if ( gap_size == RB_UNASSIGNED )
        return RB_UNASSIGNED;  // gap not found

    return gap_offset; // return offset of the best gap
}

// rbm_get_prb() - get actual physical RBoffset and RBsize for given id
// rbm_get_prb() must be invoked after rbm_done()
U32 rbm_get_prb(RBMap *rbm, U32 id, S32 *rb_offset, U32 *rb_size)
{
    int i, k;
    for ( i = 0; i != rbm->max_rbs; ++i )
    {
        if ( rbm->ids[i] != id )
            continue;
        for ( k = i; k != rbm->max_rbs; ++k )
            if ( rbm->ids[k] != id )
                break;
        *rb_offset = i;
        *rb_size = k - i;
        return 1;
    }
    return 0; // id not found
}


// rbm stands for "Resource Blocks Map"
// Funcs working with this struct provide some high level intelligent ways of RBs allocation
// Both physical and virtual RB assignment types supported
void rbm_init(RBMap *rbm, U32 max_rbs)
{
    U32 i;
    rbm->max_rbs = max_rbs;
    rbm->unassigned_rbs_cnt = max_rbs;
    rbm->fixed_size_cnt = 0;
    rbm->prop_cnt = 0;
    rbm->fixed_size_total = 0;
    for ( i = 0; i != max_rbs; ++i )
        rbm->ids[i] = RB_UNASSIGNED;
}

// rbm_alloc() - allocate at least prb_num of physical RBs

// "id" - resource ID. For example, RNTI number can be used as resource ID.
//      id must be != RB_UNASSIGNED

// "prb_offset" - physical resource block offset
//      if prb_offset = -1, then resorce manager takes care of offset

// "prb_num" - desired number of physical resource blocks
//      if prb_num = 0 - distribute resources proportionally (give as many as possible)
//      if prb_num = 0 - prb_offset value ignored

// "vrb" - make sure RB allocation (offset & size) compliant with virtual assignment.

// Example 1: total 3 allocations and all of them with prb_num = 0:
// entire RB space divided int 3 allocations giving 33% RBs to each allocation.

// Example 2: total 2 allocations. 1st one with prb_num = 10, 2nd one with prb_num = 0:
// 10 RBs goes to the 1st allocation and all the remaining RBs to the 2nd one.

// Order (priority) of resource allocations:
// 1. Fixed size/offset
// 2. Fixed size, variable offset
// 3. Virtual proportional & non-virtual proportional

// Return value 0: success
// !=0 : resource conflict: overlap when fixed offset and size provided

U32 rbm_alloc(RBMap *rbm, U32 id, S32 prb_offset, U32 prb_num, U8 vrb)
{
    U32 i, n_step_rb, v_prb_num;

    if ( id == RB_UNASSIGNED || prb_offset < -1 )
        return RB_UNASSIGNED;

    if ( vrb )
    {
        // recalculate offset & RB num compliant with virtual assignment
        n_step_rb = get_n_step_rb(rbm->max_rbs);
        if ( prb_num )
        {
            v_prb_num = ( prb_num / n_step_rb ) * n_step_rb;
            if ( v_prb_num < prb_num )
                v_prb_num += n_step_rb;
            prb_num = v_prb_num;
        }
        if ( prb_offset != -1 )
        {
            prb_offset = ( prb_offset / n_step_rb ) * n_step_rb;
        }
    }

    if ( prb_num )
    {
        // fixed number of RBs to be allocated
        if ( prb_offset != -1  )
        {
            // both size and offset are fixed: we can allocate RBs right away
            for ( i = 0; i != prb_num; ++i )
                if ( rbm->ids[ prb_offset + i ] != RB_UNASSIGNED )
                    return RB_UNASSIGNED;
                else
                    rbm->ids[ prb_offset + i ] = id;

            // update number of unassigned RBs
            rbm->unassigned_rbs_cnt -= prb_num;
        }
        else
        {
            // only size is fixed, but offset hasn't been provided
            // do the distribution after all fixed offset allocations complete,
            // but before proportional distribution
            rbm->fixed_size_ids[rbm->fixed_size_cnt] = id;
            rbm->fixed_size_val[rbm->fixed_size_cnt] = prb_num;
            rbm->fixed_size_cnt++;
            rbm->fixed_size_total += prb_num;
        }
    }
    else
    {
        // prb_num == 0 means proportional distribution
        // remember id for proportional distribution
        rbm->prop_ids[rbm->prop_cnt] = id;
        rbm->prop_vrb[rbm->prop_cnt] = vrb;
        rbm->prop_cnt++;
    }
    return 0;
}

// rbm_done() must be invloked after last call to rbm_alloc()
// rbm_done() finish resource allocation (variable offset with fixed size + proportional)
// Return value !=0 mean failure

U32 rbm_done(RBMap *rbm)
{
    U32 i, k, id, prb_num, prb_offset;
    U32 avg_prb_num, extra_prb_num;
    U32 n_step_rb, v_prb_num, non_virt_cnt;
    U32 first_alloc;

    // allocate RBs with fixed size 1st
    // TODO: largest blocks should be assigned 1st to reduce amount of failures
    for ( i = 0; i != rbm->fixed_size_cnt; ++i )
    {
        prb_num = rbm->fixed_size_val[i];

        prb_offset = rbm_find_gap(rbm, prb_num, 0);

        if ( prb_offset == RB_UNASSIGNED )
            return RB_UNASSIGNED;   // no gap found

        id = rbm->fixed_size_ids[i];

        for ( k = 0; k != prb_num; ++k )
            rbm->ids[ prb_offset + k ] = id;

        // update number of unassigned RBs
        rbm->unassigned_rbs_cnt -= prb_num;
    }

    // allocate RBs with proportional allocation

    if ( rbm->prop_cnt == 0 )
        return 0;

    // Average num of RBs per each proportional allocation
    avg_prb_num = rbm->unassigned_rbs_cnt / rbm->prop_cnt;
    extra_prb_num = rbm->unassigned_rbs_cnt - avg_prb_num * rbm->prop_cnt;

    // allocate virtual 1st at they may take more space then average required
    // plus they have more "strict" offset requirements

    n_step_rb = get_n_step_rb(rbm->max_rbs);
    v_prb_num = ( avg_prb_num / n_step_rb ) * n_step_rb;

    // for virtual we accept allocation a little bit smaller than average
    // as we don't v_prb_num += n_step_rb in case if v_prb_num < avg_prb_num
    // this way we make sure we don't run out of resources too soon
    if ( v_prb_num == 0 )
        v_prb_num += n_step_rb;

    non_virt_cnt = 0;
    for ( i = 0; i != rbm->prop_cnt; ++i )
    {
        if ( rbm->prop_vrb[i] == 0 ) // skip non-virtual
        {
            non_virt_cnt++;
            continue;
        }

        prb_offset = rbm_find_gap(rbm, v_prb_num, 1);
        if ( prb_offset == RB_UNASSIGNED )
            return RB_UNASSIGNED;

        for ( k = 0; k != v_prb_num; ++k )
            rbm->ids[ prb_offset + k ] = rbm->prop_ids[i];

        rbm->unassigned_rbs_cnt -= v_prb_num;
    }

    // re-evaluate average
    if ( non_virt_cnt == 0 )
        return 0;

    avg_prb_num = rbm->unassigned_rbs_cnt / non_virt_cnt;

    if ( avg_prb_num == 0 )
        return RB_UNASSIGNED; // run out of resources

    prb_num = avg_prb_num;
    first_alloc = 1;
    for ( i = 0; i != rbm->prop_cnt; ++i )
    {
        if ( rbm->prop_vrb[i] != 0 ) // skip virtual as they've been assigned already
            continue;

        if ( first_alloc == 1 )
        {
            first_alloc = 2;
            prb_num += extra_prb_num;
        }
        else if ( first_alloc == 2 )
        {
            prb_num = avg_prb_num;
            first_alloc = 3;
        }

        prb_offset = rbm_find_gap(rbm, prb_num, 0);
        if ( prb_offset == RB_UNASSIGNED )
        {
            // try smaller block
            if ( prb_num > 1 )
                while ( prb_num && prb_offset == RB_UNASSIGNED )
                {
                    prb_num -= 1;
                    prb_offset = rbm_find_gap(rbm, prb_num, 0);
                }
            if ( prb_offset == RB_UNASSIGNED || prb_num == 0 )
                return RB_UNASSIGNED;
        }
        for ( k = 0; k != prb_num; ++k )
            rbm->ids[ prb_offset + k ] = rbm->prop_ids[i];

        rbm->unassigned_rbs_cnt -= prb_num;
    }

    return 0;
}

void vue_alloc_res(VUE_CFG *vues, RBMap *dlm, RBMap *ulm)
{
    VUE_CFG *ue = vues;

    U32 pdcch_num = 0;
    U32 pdsch_num = 0;
    U32 pusch_num = 0;
    U32 pucch_num = 0;

    // calculate total counts of each type of channel
    while ( ue->rnti != 0 )
    {
        if ( ue->pdsch )
        {
            ++pdsch_num;
            ++pdcch_num;
        }
        if ( ue->pusch )
        {
            ++pusch_num;
            ++pdcch_num;
        }
        if ( ue->pucch )
            ++pucch_num;
        ++ue;
    }

    printf("PDCCH: %d, PDSCH: %d, PUSCH: %d, PUCCH: %d\n", pdcch_num, pdsch_num, pusch_num, pucch_num);

    // Allocate resources for PUCCH 1st
    if ( pucch_num )
    {

        U8 k = 0;
        U8 lo_res = 0;
        U8 hi_res = ulm->max_rbs - 1;

        ue = vues;
        while ( ue->rnti != 0 )
        {
            if ( ue->pucch )
            {
                // use different ID for PUCCH channels : ue->rnti + 0xfffff
                if ( k )
                {
                    rbm_alloc(ulm, 0xffff + ue->rnti, hi_res, 1, 0);
                    hi_res--;
                }
                else
                {
                    rbm_alloc(ulm, 0xffff + ue->rnti, lo_res, 1, 0);
                    lo_res++;
                }
                k = 1 - k;
            }
            ue++;
        }

        // reserve 1 RB if odd number of PUCCH to make sure it won't be used by PUSCH
        if ( pucch_num & 1 )
        {
            // use dummy id no one going to use
            if ( ulm->max_rbs - hi_res < lo_res )
                rbm_alloc(ulm, 0xffffffe, hi_res, 1, 0);
            else
                rbm_alloc(ulm, 0xffffffe, lo_res, 1, 0);
        }

    }

    ue = vues;
    // Allocate shared channels PDSCH & PUSCH
    while ( ue->rnti != 0 )
    {
        if ( ue->pdsch )
            rbm_alloc(dlm, ue->rnti, -1, 0, 0);
        if ( ue->pusch )
            rbm_alloc(ulm, ue->rnti, -1, 0, 0);
        ++ue;
    }
}

U8 TestMgrGetNumConsecutiveRBs(U8 **pMap, U32* num)
{
    U8 startRb = **pMap;
    U8 *pNextRb = *pMap + 1;
    U8 count = 0;

    while (startRb + (++count) == *pNextRb++)
    {
        if(--(*num) == 0)
            break;
    }

    *pMap = pNextRb - 1;
    return count;
}

U8 TestMgrIsTimAdv(BS_CONFOR_TEST_CONFIG *pTestConfConfig, U8 sf)
{
    return pTestConfConfig->TimAdvCmdMap[sf];
}

/*static S16 TestMgrGetPilotPower(BS_CONFOR_TEST_CONFIG *pTestConfConfig)
{
    PRINT_FUNCTION_NAME(__FUNCTION__);
    return pTestConfConfig->rs_epre;
}*/

/*static S16 TestMgrGetPsyncPower(BS_CONFOR_TEST_CONFIG *pTestConfConfig)
{
    PRINT_FUNCTION_NAME(__FUNCTION__);
    return pTestConfConfig->pss_ra;
}*/

/*static S16 TestMgrGetSsyncPower(BS_CONFOR_TEST_CONFIG *pTestConfConfig)
{
    PRINT_FUNCTION_NAME(__FUNCTION__);
    return pTestConfConfig->sss_ra;
}*/

/*static S16 TestMgrGetPcfichPower(BS_CONFOR_TEST_CONFIG *pTestConfConfig)
{
    PRINT_FUNCTION_NAME(__FUNCTION__);
    return (S16) pTestConfConfig->pcfichEpre;
}*/

/*static U16 TestMgrGetCellId(BS_CONFOR_TEST_CONFIG *pTestConfConfig)
{
    PRINT_FUNCTION_NAME(__FUNCTION__);
    // PHY suports only 0 now
    return pBsMacDesc->initParm.phyLayerCellIdGroup;
}*/

RESULTCODE SetRv( U8* Rv, U32 len)
{
   memcpy(&testConfConfig.arRctRv, Rv, len);
   return SUCCESS;
}

U8 PrachConfig(U8 Index, U16 frameNumber, U16 subframeNumber)
{
    U8 FrEven;
    U8 rs = PRACH_OFF;

    FrEven = ~(frameNumber & 1);

    switch(Index)
    {
        case 0:
            if(FrEven && (subframeNumber == 1))
                rs = PRACH_ON;
            break;
        case 1:
            if(FrEven && (subframeNumber == 4))
                 rs = PRACH_ON;
            break;
        case 2:
            if(FrEven && (subframeNumber == 7))
                rs = PRACH_ON;
            break;
        case 3:
            if(subframeNumber == 1)
                rs = PRACH_ON;
            break;
        case 4:
            if(subframeNumber == 4)
                rs = PRACH_ON;
            break;
        case 5:
            if(subframeNumber == 7)
                rs = PRACH_ON;
            break;
        case 6:
            if(subframeNumber == 1 || subframeNumber == 6 )
                rs = PRACH_ON;
            break;
        case 7:
            if(subframeNumber == 2 || subframeNumber == 7 )
                rs = PRACH_ON;
            break;
        case 8:
            if(subframeNumber == 3 || subframeNumber == 8 )
                rs = PRACH_ON;
            break;
        case 9:
            if( subframeNumber == 1 || subframeNumber == 4 ||
                subframeNumber == 7 )
                rs = PRACH_ON;
            break;
        case 10:
            if( subframeNumber == 2 || subframeNumber == 5 ||
                subframeNumber == 8 )
                rs = PRACH_ON;
            break;
        case 11:
            if( subframeNumber == 3 || subframeNumber == 6 ||
                subframeNumber == 9 )
                rs = PRACH_ON;
            break;
        case 12:
            if( subframeNumber == 0 || subframeNumber == 2 ||
                subframeNumber == 4 || subframeNumber == 6 ||
                subframeNumber == 8 )
                rs = PRACH_ON;
            break;
        case 13:
            if( subframeNumber == 1 || subframeNumber == 3 ||
                subframeNumber == 5 || subframeNumber == 7 ||
                subframeNumber == 9 )
                rs = PRACH_ON;
            break;
        case 14:
            rs = PRACH_ON;
            break;
        case 15:
            if(FrEven && (subframeNumber == 9))
                rs = PRACH_ON;
            break;
    case 64:
            if(subframeNumber == 0)
        rs = PRACH_ON;
        break;
        default:
            rs = PRACH_OFF;
            break;
    }

    return rs;
}

RESULTCODE SetDLDciCtx(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                       DCI_DATA_INFO* pDciInfo, /* TxVector */
                       U32 pdsch_idx)
{
    U32 i;
    U8 bw = pBsMacDesc->initParm.channelBandwidth;
    pDciInfo->dciFormat = pTestConfConfig->DciFormat;
    pDciInfo->cceIndex = 0;
    pDciInfo->aggregationLvl = pTestConfConfig->nCcePerPdcch;
    pDciInfo->codeBookIndex = 1;
    pDciInfo->resrcAllocType = 0;
    pDciInfo->harqProcessId = 0;
    pDciInfo->numAntennaPort = pTestConfConfig->nAntennaPort;
    pDciInfo->numLayer = pTestConfConfig->nLayers;
    if(bw == BW_20_0_MHZ)
    {
        pDciInfo->numOfRBs = 100;
        pDciInfo->RGBSize = 4;
    }
    else if(bw == BW_10_0_MHZ)
    {
        pDciInfo->numOfRBs = 50;
        pDciInfo->RGBSize = 3;
    }

    pDciInfo->numTB = pTestConfConfig->nCw; //cw num
    if(pDciInfo->dciFormat!= FORMAT1)
        pDciInfo->rnti = pTestConfConfig->rntiBoosted[0];
    else
        pDciInfo->rnti = pTestConfConfig->rntiPusch[0];

    pDciInfo->tbSwapFlag = 0;
    pDciInfo->transmissionMode = pTestConfConfig->transmissionMode;
    for( i = 0; i < pTestConfConfig->nCw; i++)
    {
        if(pTestConfConfig->PdschCfgEna)
           {
             pDciInfo->tbInfo[i].mcs = pTestConfConfig->PdschCfg[pdsch_idx].CwCfg[i].Mcs;
           }
    else
    {
        if(pTestConfConfig->pdschMcs_used[i])
        {
            pDciInfo->tbInfo[i].mcs = pTestConfConfig->pdschMcs[i][pdsch_idx];
        }
        else
        {
            pDciInfo->tbInfo[i].mcs = lte_modulation_get_mcs_index_dl(pTestConfConfig->pdschMod[i][0], i);
        }
        }
        pDciInfo->tbInfo[i].redundancyVer = 0;
        pDciInfo->tbInfo[i].txIndicator = 1;
    }

    return SUCCESS;
}

/* Function sets dafault test config for test execution */
RESULTCODE TestMgrSetConfig(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
    PRINT_FUNCTION_NAME(__FUNCTION__);

    pTestCfg->TestModel = E_TM_3_1;

    pTestCfg->nAntennaPort = 1;
    pTestCfg->nCw = 1;
    pTestCfg->nLayers = 1;
    pTestCfg->precodingMode = 0;
    pTestCfg->duration = 10;
    pTestCfg->transmissionMode = SINGLEANT;

    pTestCfg->rsBoosting = 1;
    pTestCfg->epreSinchro = FLOAT2Q8(0);
    pTestCfg->reservedEpre = MINUS_INF_Q8;

    /* PBCH */
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = 0;
    pTestCfg->pbchReservedEpre = MINUS_INF_Q8;

    /* PCFICH */
    pTestCfg->nCtrlSymbols = 1;

    /* PHICH */
    pTestCfg->nPhichGroup = 0;// phich disable
    pTestCfg->nPhichPerGroup = 2;
    pTestCfg->phichSymPower = FLOAT2Q8(-3.010);
    pTestCfg->phichGroupEpre = FLOAT2Q8(0);

    /* PDCCH */
    pTestCfg->nAvalableRegs = 90;
    pTestCfg->nPdcch = 5;
    pTestCfg->nCcePerPdcch = 2;
    pTestCfg->nRegPerCce = 9;
    pTestCfg->nRegAllToPdcch = 90;
    pTestCfg->nNilRegPadding = 0;
    pTestCfg->pdcchRegEpre = FLOAT2Q8(1.065);
    pTestCfg->nilRegEpre = MINUS_INF_Q8;

        /* PDSCH */
    pTestCfg->nPdsch[0] = 1;
    pTestCfg->nPdschPrb[0][0] = 50;
    pTestCfg->pdschPrbPa[0][0] = FLOAT2Q8(0);
    pTestCfg->pdschMod[0][0] = QAM64;

        pTestCfg->rntiBoosted[0] = 0;
        pTestCfg->rntiDeBoosted[0] = 0;

        /* Boosting map */
        testMgrCtx.boostingMap[0][0] = (U8**)E_TM_3_1MapIndex[BW_10_0_MHZ];

    return SUCCESS;
}

RESULTCODE TestMgrSet_4UE_10Mhz_2ANT_HARQ(BS_CONFOR_TEST_CONFIG *pTestCfg, U32 num_ant, U32 bw)
{
    PLTE_PDSCH_HARQ_STATE lte_pdsch_harq_state = get_lte_pdsch_harq_state();
    DCI_INFO    *pDciInfo = &dci_info[0];
    U32         index;

    printf("%s\n",__FUNCTION__);

    StaticConfig.tdd = 0;
    StaticConfig.bw_mode = bw;
    StaticConfig.n_antenna_port = num_ant;
    initBS(&StaticConfig);

//---------------------------------------------------------
    lte_pdsch_harq_state->ena_harq = 1;

    pTestCfg->nAntennaPort = num_ant;
    pTestCfg->nCw = num_ant;
    pTestCfg->nLayers = num_ant;
    pTestCfg->transmissionMode = OPNLPSPMUX;

    pTestCfg->epreSinchro = FLOAT2Q8(0);
//    pTestCfg->reservedEpre = MINUS_INF_Q8;


// ========================================================
// DL channels
// ========================================================
//---------------------------------------------------------
    // PDSCH
    pTestCfg->nPdsch[0] = 4;
    // PDSCH MCS
    pTestCfg->pdschMcs_used[0] = 1;
    pTestCfg->pdschMcs_used[1] = 1;
    pTestCfg->pdschMcs[0][0] = 28;
    pTestCfg->pdschMcs[1][0] = 28;
    pTestCfg->pdschMcs[0][1] = 28;
    pTestCfg->pdschMcs[1][1] = 28;
    pTestCfg->pdschMcs[0][2] = 28;
    pTestCfg->pdschMcs[1][2] = 28;
    pTestCfg->pdschMcs[0][3] = 28;
    pTestCfg->pdschMcs[1][3] = 28;

    // PDSCH RNTI (scrInitValueInput)
    pTestCfg->pdschRnti_used = 1;
    pTestCfg->pdschRnti[0] = 0;
    pTestCfg->pdschRnti[1] = 1;
    pTestCfg->pdschRnti[2] = 2;
    pTestCfg->pdschRnti[3] = 3;

    // PDSCH DL MAP
    pTestCfg->pdsch_map[0].startRes = 0;
    pTestCfg->pdsch_map[0].numRes = 13;
    pTestCfg->pdsch_map[1].startRes = 13;
    pTestCfg->pdsch_map[1].numRes = 12;
    pTestCfg->pdsch_map[2].startRes = 25;
    pTestCfg->pdsch_map[2].numRes = 12;
    pTestCfg->pdsch_map[3].startRes = 37;
    pTestCfg->pdsch_map[3].numRes = 13;

    pTestCfg->cddType = LARGECDD;
    pTestCfg->codeBookIdx = 0;

    pTestMgrGetBoostedPdsch = lte_pdsch_set_mapinfo_boosted_direct;
    pTestMgrGetDeBoostedPdsch = lte_pdsch_set_mapinfo_boosted_direct;

    // Boosting map
    // not used in this case, shouldn't be zero
    memset(testMgrCtx.boostingMap[0], 0xFF, sizeof(testMgrCtx.boostingMap[0]));

    // PDSCH Power
    pTestCfg->pdschPrbPa[0][0] = FLOAT2Q8(0);
    pTestCfg->pdschPrbPa[1][0] = FLOAT2Q8(0);
    pTestCfg->pdschPrbPa[0][1] = FLOAT2Q8(0);
    pTestCfg->pdschPrbPa[1][1] = FLOAT2Q8(0);
    pTestCfg->pdschPrbPa[0][2] = FLOAT2Q8(0);
    pTestCfg->pdschPrbPa[1][2] = FLOAT2Q8(0);
    pTestCfg->pdschPrbPa[0][3] = FLOAT2Q8(0);
    pTestCfg->pdschPrbPa[1][3] = FLOAT2Q8(0);
    // PDSCH Payload Mode
    pTestCfg->pdschPayloadMode[0][0] = 1;
    pTestCfg->pdschPayloadMode[1][0] = 0;
    pTestCfg->pdschPayloadMode[0][1] = 1;
    pTestCfg->pdschPayloadMode[1][1] = 0;
    pTestCfg->pdschPayloadMode[0][2] = 1;
    pTestCfg->pdschPayloadMode[1][2] = 0;
    pTestCfg->pdschPayloadMode[0][3] = 1;
    pTestCfg->pdschPayloadMode[1][3] = 0;


    /* PBCH */
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = FLOAT2Q8(0);
    pTestCfg->pbchRv = 3;
    pBsMacDesc->initParm.phyLayerCellIdGroup = 0x00;
    pTestCfg->payloadPbch[0] = 0xA0;
    pTestCfg->payloadPbch[1] = 0x00;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;

        pTestCfg->DontUsePuschRxSduForHiAndDci0 = 1; // don't use PUSCH RxSdu queue for HiSdu
        pTestCfg->ULEnabled = 0;    // create HiSdu permanently

        // PDCCH
        pTestCfg->nPdcchUl = 0;
        pTestCfg->nPdcch = 4;
        pTestCfg->nCcePerPdcch = 2;
        pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
        pTestCfg->rntiBoosted[0] = 20;
        pTestCfg->rntiBoosted[1] = 20;
        pTestCfg->rntiBoosted[2] = 20;
        pTestCfg->rntiBoosted[3] = 20;
        // PDCCH DL MAP
        pTestCfg->pdcch_map[0][0].startRes = 0;
        pTestCfg->pdcch_map[0][0].numRes = 2;
        pTestCfg->pdcch_map[0][1].startRes = 0;
        pTestCfg->pdcch_map[0][1].numRes = 2;
        pTestCfg->pdcch_map[1][0].startRes = 2;
        pTestCfg->pdcch_map[1][0].numRes = 2;
        pTestCfg->pdcch_map[1][1].startRes = 2;
        pTestCfg->pdcch_map[1][1].numRes = 2;
        pTestCfg->pdcch_map[2][0].startRes = 0;
        pTestCfg->pdcch_map[2][0].numRes = 2;
        pTestCfg->pdcch_map[2][1].startRes = 0;
        pTestCfg->pdcch_map[2][1].numRes = 2;
        pTestCfg->pdcch_map[3][0].startRes = 0;
        pTestCfg->pdcch_map[3][0].numRes = 2;
        pTestCfg->pdcch_map[3][1].startRes = 0;
        pTestCfg->pdcch_map[3][1].numRes = 2;

        memset(pTestCfg->pdcch_map_unified, 1, sizeof(pTestCfg->pdcch_map_unified));
        pTestMgrGetPdcch = lte_pdcch_get_mapinfo_direct;

        // DCI
        pTestCfg->DciFormat = DCI_FORMAT_HARDCODED_PAYLOAD; // 31 bit
        pTestCfg->payloadPdcch_unified = 1;

        pDciInfo->dci_format = DCI_FORMAT_2A;
        pDciInfo->rah = 1;
        pDciInfo->rbg = 131071;
        pDciInfo->tb_info[0].modulation_type = 28;
        pDciInfo->tb_info[1].modulation_type = 28;

        pDciInfo->harq_process_id = 7;
        pDciInfo->tb_info[0].ndi = 0;
        pDciInfo->tb_info[1].ndi = 0;

        pDciInfo->tb_info[0].redundancy_ver = 0;
        pDciInfo->tb_info[1].redundancy_ver = 0;

        pDciInfo->tpc = 1;

        dci_encode_2a((U32 * )&pTestCfg->payloadPdcch[0][0], (DCI_INFO *) pDciInfo, (BS_STATIC_CONFIG *) &StaticConfig, &index);
        pTestCfg->PdcchDataLen[0] = index;

// !!! need for SetDLDciCtx
        pTestCfg->pdschMod[0][0] = 28;
        pTestCfg->pdschMod[0][1] = 28;
        pTestCfg->pdschMod[0][2] = 28;
        pTestCfg->pdschMod[0][3] = 28;

        pTestCfg->pdschMod[1][0] = 28;
        pTestCfg->pdschMod[1][1] = 28;
        pTestCfg->pdschMod[1][2] = 28;
        pTestCfg->pdschMod[1][3] = 28;



    // PUSCH
    memset(pTestCfg->numPusch, 2, sizeof(pTestCfg->numPusch));
    // PUSCH MCS
    pTestCfg->ulSfrCtrl.enable64QAM = 1;
    pTestCfg->puschMcs_used = 1;
    pTestCfg->puschMcs[0] = 0;
    pTestCfg->puschMcs[1] = 0;
    // PUSCH RNTI (scrInitValueInput)
    pTestCfg->rntiPusch[0] = 0;
    pTestCfg->rntiPusch[1] = 3;
    // PUSCH UL MAP
    pTestCfg->pusch_map[0].startRes = 1;
    pTestCfg->pusch_map[0].numRes = 1;
    pTestCfg->pusch_map[1].startRes = 2;
    pTestCfg->pusch_map[1].numRes = 1;
    pTestMgrGetPusch = lte_pusch_set_mapinfo_direct;

    // PUSCH nDMRS2
    pTestCfg->pusch_nDMRS2_used = 1;
    pTestCfg->pusch_nDMRS2[0] = 0;
    pTestCfg->pusch_nDMRS2[1] = 0;

    pTestCfg->nPuschBoostedPrb = 50;
    pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);

    *((U8*)pTestCfg->Pusch_nAck + 0) = 2;
    *((U8*)pTestCfg->Pusch_nAck + 1) = 2;
    pTestCfg->Pusch_nAckUsed = 2;

    pTestCfg->PuschRiIdx[0] = 7;
    pTestCfg->PuschRiIdx[1] = 7;
    pTestCfg->PuschRiIdxUsed = 1;

    pTestCfg->PuschAckIdx[0] = 6;
    pTestCfg->PuschAckIdx[1] = 6;
    pTestCfg->PuschAckUsed = 1;

    // PUCCH
    memset(pTestCfg->numPucch, 2, sizeof(pTestCfg->numPucch));
    // PUCCH RNTI (scrInitValueInput)
    pTestCfg->rntiPucch_used = 1;
    pTestCfg->rntiPucch[0] = 0;
    pTestCfg->rntiPucch[1] = 0;
    // PUCCH UL MAP
    pTestCfg->pucch_map[0].startRes = 0;
    pTestCfg->pucch_map[0].numRes = 1;
    pTestCfg->pucch_map[1].startRes = 49;
    pTestCfg->pucch_map[1].numRes = 1;
    pTestMgrGetPucch = lte_pucch_set_mapinfo_direct;
    // PUCCH format
    pTestCfg->pucchFormat[0] = FORMAT1B;
    pTestCfg->pucchFormat[1] = FORMAT1B;
    pTestCfg->pucchFormat_used = 1;
    // PUCCH dlCqiPmiSizeBits
    pTestCfg->pucch_dlCqiPmiSizeBits[0] = 0;
    pTestCfg->pucch_dlCqiPmiSizeBits[1] = 0;
    pTestCfg->pucch_dlCqiPmiSizeBits_used = 1;
    // PUCCH harqPucchIndex
    pTestCfg->pucch_harqPucchIndex[0] = 0;
    pTestCfg->pucch_harqPucchIndex[1] = 1;
    pTestCfg->pucch_harqPucchIndex_used = 1;
    // PUCCH harqSizebits
    pTestCfg->pucch_harqSizebits[0] = 2;
    pTestCfg->pucch_harqSizebits[1] = 2;
    pTestCfg->pucch_harqSizebits_used = 1;

    pTestCfg->ulSfrCtrl.deltaPUCCHShift = 2;
    pTestCfg->ulSfrCtrl.nRBCQI = 1;
    pTestCfg->ulSfrCtrl.n1PucchAN = 11;


    /* Init param */
    pBsMacDesc->initParm.txAntennaPortCount = TXANT_TWO;
    pBsMacDesc->initParm.rxAntennaPortCount = RXANT_ONE;

    return SUCCESS;
}

RESULTCODE TestMgrSet_4UE_10Mhz_2ANT_CQI_HARQ(BS_CONFOR_TEST_CONFIG *pTestCfg, U32 num_ant, U32 bw)
{
    PLTE_PDSCH_HARQ_STATE       plte_pdsch_harq_state = get_lte_pdsch_harq_state();
    PLTE_PDSCH_CHAN_STATE       plte_pdsch_chan_state = get_lte_pdsch_chan_state();


    DCI_INFO    *pDciInfo = &dci_info[0];
    U32         index;

    printf("%s\n",__FUNCTION__);

    StaticConfig.tdd = 0;
    StaticConfig.bw_mode = bw;
    StaticConfig.n_antenna_port = num_ant;
    initBS(&StaticConfig);

//---------------------------------------------------------
    plte_pdsch_harq_state->ena_harq = 0;
    plte_pdsch_chan_state->ena_dynamic_cqi_pmi_ri = 1;

    pTestCfg->nAntennaPort = num_ant;
    pTestCfg->nCw = num_ant;
    pTestCfg->nLayers = num_ant;
    pTestCfg->transmissionMode = OPNLPSPMUX;

    pTestCfg->epreSinchro = FLOAT2Q8(0);
//    pTestCfg->reservedEpre = MINUS_INF_Q8;


// ========================================================
// DL channels
// ========================================================
//---------------------------------------------------------
    // PDSCH
    pTestCfg->nPdsch[0] = 4;
    // PDSCH MCS
    pTestCfg->pdschMcs_used[0] = 1;
    pTestCfg->pdschMcs_used[1] = 1;
    pTestCfg->pdschMcs[0][0] = 28;
    pTestCfg->pdschMcs[1][0] = 28;
    pTestCfg->pdschMcs[0][1] = 28;
    pTestCfg->pdschMcs[1][1] = 28;
    pTestCfg->pdschMcs[0][2] = 28;
    pTestCfg->pdschMcs[1][2] = 28;
    pTestCfg->pdschMcs[0][3] = 28;
    pTestCfg->pdschMcs[1][3] = 28;

    // PDSCH RNTI (scrInitValueInput)
    pTestCfg->pdschRnti_used = 1;
    pTestCfg->pdschRnti[0] = 0;
    pTestCfg->pdschRnti[1] = 1;
    pTestCfg->pdschRnti[2] = 2;
    pTestCfg->pdschRnti[3] = 3;

    // PDSCH DL MAP
    pTestCfg->pdsch_map[0].startRes = 0;
    pTestCfg->pdsch_map[0].numRes = 13;
    pTestCfg->pdsch_map[1].startRes = 13;
    pTestCfg->pdsch_map[1].numRes = 12;
    pTestCfg->pdsch_map[2].startRes = 25;
    pTestCfg->pdsch_map[2].numRes = 12;
    pTestCfg->pdsch_map[3].startRes = 37;
    pTestCfg->pdsch_map[3].numRes = 13;

    pTestCfg->cddType = LARGECDD;
    pTestCfg->codeBookIdx = 0;

    pTestMgrGetBoostedPdsch = lte_pdsch_set_mapinfo_boosted_direct;
    pTestMgrGetDeBoostedPdsch = lte_pdsch_set_mapinfo_boosted_direct;

    // Boosting map
    // not used in this case, shouldn't be zero
    memset(testMgrCtx.boostingMap[0], 0xFF, sizeof(testMgrCtx.boostingMap[0]));

    // PDSCH Power
    pTestCfg->pdschPrbPa[0][0] = FLOAT2Q8(0);
    pTestCfg->pdschPrbPa[1][0] = FLOAT2Q8(0);
    pTestCfg->pdschPrbPa[0][1] = FLOAT2Q8(0);
    pTestCfg->pdschPrbPa[1][1] = FLOAT2Q8(0);
    pTestCfg->pdschPrbPa[0][2] = FLOAT2Q8(0);
    pTestCfg->pdschPrbPa[1][2] = FLOAT2Q8(0);
    pTestCfg->pdschPrbPa[0][3] = FLOAT2Q8(0);
    pTestCfg->pdschPrbPa[1][3] = FLOAT2Q8(0);
    // PDSCH Payload Mode
    pTestCfg->pdschPayloadMode[0][0] = 1;
    pTestCfg->pdschPayloadMode[1][0] = 0;
    pTestCfg->pdschPayloadMode[0][1] = 1;
    pTestCfg->pdschPayloadMode[1][1] = 0;
    pTestCfg->pdschPayloadMode[0][2] = 1;
    pTestCfg->pdschPayloadMode[1][2] = 0;
    pTestCfg->pdschPayloadMode[0][3] = 1;
    pTestCfg->pdschPayloadMode[1][3] = 0;


    /* PBCH */
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = FLOAT2Q8(0);
    pTestCfg->pbchRv = 3;
    pBsMacDesc->initParm.phyLayerCellIdGroup = 0x00;
    pTestCfg->payloadPbch[0] = 0xA0;
    pTestCfg->payloadPbch[1] = 0x00;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;

        pTestCfg->DontUsePuschRxSduForHiAndDci0 = 1; // don't use PUSCH RxSdu queue for HiSdu
        pTestCfg->ULEnabled = 0;    // create HiSdu permanently

        // PDCCH
        pTestCfg->nPdcchUl = 0;
        pTestCfg->nPdcch = 4;
        pTestCfg->nCcePerPdcch = 2;
        pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
        pTestCfg->rntiBoosted[0] = 20;
        pTestCfg->rntiBoosted[1] = 20;
        pTestCfg->rntiBoosted[2] = 20;
        pTestCfg->rntiBoosted[3] = 20;
        // PDCCH DL MAP
        pTestCfg->pdcch_map[0][0].startRes = 0;
        pTestCfg->pdcch_map[0][0].numRes = 2;
        pTestCfg->pdcch_map[0][1].startRes = 0;
        pTestCfg->pdcch_map[0][1].numRes = 2;
        pTestCfg->pdcch_map[1][0].startRes = 2;
        pTestCfg->pdcch_map[1][0].numRes = 2;
        pTestCfg->pdcch_map[1][1].startRes = 2;
        pTestCfg->pdcch_map[1][1].numRes = 2;
        pTestCfg->pdcch_map[2][0].startRes = 0;
        pTestCfg->pdcch_map[2][0].numRes = 2;
        pTestCfg->pdcch_map[2][1].startRes = 0;
        pTestCfg->pdcch_map[2][1].numRes = 2;
        pTestCfg->pdcch_map[3][0].startRes = 0;
        pTestCfg->pdcch_map[3][0].numRes = 2;
        pTestCfg->pdcch_map[3][1].startRes = 0;
        pTestCfg->pdcch_map[3][1].numRes = 2;

        memset(pTestCfg->pdcch_map_unified, 1, sizeof(pTestCfg->pdcch_map_unified));
        pTestMgrGetPdcch = lte_pdcch_get_mapinfo_direct;

        // DCI
        pTestCfg->DciFormat = DCI_FORMAT_HARDCODED_PAYLOAD; // 31 bit
        pTestCfg->payloadPdcch_unified = 1;

        pDciInfo->dci_format = DCI_FORMAT_2A;
        pDciInfo->rah = 1;
        pDciInfo->rbg = 131071;
        pDciInfo->tb_info[0].modulation_type = 28;
        pDciInfo->tb_info[1].modulation_type = 28;

        pDciInfo->harq_process_id = 7;
        pDciInfo->tb_info[0].ndi = 0;
        pDciInfo->tb_info[1].ndi = 0;

        pDciInfo->tb_info[0].redundancy_ver = 0;
        pDciInfo->tb_info[1].redundancy_ver = 0;

        pDciInfo->tpc = 1;

        dci_encode_2a((U32 * )&pTestCfg->payloadPdcch[0][0], (DCI_INFO *) pDciInfo, (BS_STATIC_CONFIG *) &StaticConfig, &index);
        pTestCfg->PdcchDataLen[0] = index;

// !!! need for SetDLDciCtx
        pTestCfg->pdschMod[0][0] = 28;
        pTestCfg->pdschMod[0][1] = 28;
        pTestCfg->pdschMod[0][2] = 28;
        pTestCfg->pdschMod[0][3] = 28;

        pTestCfg->pdschMod[1][0] = 28;
        pTestCfg->pdschMod[1][1] = 28;
        pTestCfg->pdschMod[1][2] = 28;
        pTestCfg->pdschMod[1][3] = 28;



    // PUSCH
    memset(pTestCfg->numPusch, 2, sizeof(pTestCfg->numPusch));
    // PUSCH MCS
    pTestCfg->ulSfrCtrl.enable64QAM = 1;
    pTestCfg->puschMcs_used = 1;
    pTestCfg->puschMcs[0] = 0;
    pTestCfg->puschMcs[1] = 0;
    // PUSCH RNTI (scrInitValueInput)
    pTestCfg->rntiPusch[0] = 0;
    pTestCfg->rntiPusch[1] = 3;
    // PUSCH UL MAP
    pTestCfg->pusch_map[0].startRes = 1;
    pTestCfg->pusch_map[0].numRes = 1;
    pTestCfg->pusch_map[1].startRes = 2;
    pTestCfg->pusch_map[1].numRes = 1;
    pTestMgrGetPusch = lte_pusch_set_mapinfo_direct;

    // PUSCH nDMRS2
    pTestCfg->pusch_nDMRS2_used = 1;
    pTestCfg->pusch_nDMRS2[0] = 0;
    pTestCfg->pusch_nDMRS2[1] = 0;

    pTestCfg->nPuschBoostedPrb = 50;
    pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);

    *((U8*)pTestCfg->Pusch_nAck + 0) = 2;
    *((U8*)pTestCfg->Pusch_nAck + 1) = 2;
    pTestCfg->Pusch_nAckUsed = 2;

    pTestCfg->PuschRiIdx[0] = 7;
    pTestCfg->PuschRiIdx[1] = 7;
    pTestCfg->PuschRiIdxUsed = 1;

    pTestCfg->PuschAckIdx[0] = 6;
    pTestCfg->PuschAckIdx[1] = 6;
    pTestCfg->PuschAckUsed = 1;

    // PUCCH
    memset(pTestCfg->numPucch, 2, sizeof(pTestCfg->numPucch));
    // PUCCH RNTI (scrInitValueInput)
    pTestCfg->rntiPucch_used = 1;
    pTestCfg->rntiPucch[0] = 0;
    pTestCfg->rntiPucch[1] = 0;
    // PUCCH UL MAP
    pTestCfg->pucch_map[0].startRes = 0;
    pTestCfg->pucch_map[0].numRes = 1;
    pTestCfg->pucch_map[1].startRes = 49;
    pTestCfg->pucch_map[1].numRes = 1;
    pTestMgrGetPucch = lte_pucch_set_mapinfo_direct;
    // PUCCH format
    pTestCfg->pucchFormat[0] = FORMAT2;
    pTestCfg->pucchFormat[1] = FORMAT2;
    pTestCfg->pucchFormat_used = 1;
    // PUCCH dlCqiPmiSizeBits
    pTestCfg->pucch_dlCqiPmiSizeBits[0] = 0;
    pTestCfg->pucch_dlCqiPmiSizeBits[1] = 0;
    pTestCfg->pucch_dlCqiPmiSizeBits_used = 1;
    // PUCCH harqPucchIndex
    pTestCfg->pucch_harqPucchIndex[0] = 0;
    pTestCfg->pucch_harqPucchIndex[1] = 1;
    pTestCfg->pucch_harqPucchIndex_used = 1;
    // PUCCH harqSizebits
    pTestCfg->pucch_harqSizebits[0] = 2;
    pTestCfg->pucch_harqSizebits[1] = 2;
    pTestCfg->pucch_harqSizebits_used = 1;

    pTestCfg->ulSfrCtrl.deltaPUCCHShift = 2;
    pTestCfg->ulSfrCtrl.nRBCQI = 1;
    pTestCfg->ulSfrCtrl.n1PucchAN = 11;


    /* Init param */
    pBsMacDesc->initParm.txAntennaPortCount = TXANT_TWO;
    pBsMacDesc->initParm.rxAntennaPortCount = RXANT_ONE;

    return SUCCESS;
}



RESULTCODE TestMgrSetRtConfig10Mhz2ANT(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
    DCI_INFO    DciInfo;
    DCI_INFO    *pDciInfo = &DciInfo;
    U32                 index;

    printf("%s\n",__FUNCTION__);

    StaticConfig.bw_mode = BW_10_0_MHZ;
    StaticConfig.tdd = 0;
    StaticConfig.n_antenna_port = 2;

    initBS(&StaticConfig);

//---------------------------------------------------------------------------------------------------------------------------------------------------

    pBsMacDesc->initParm.txAntennaPortCount = TXANT_TWO;

    pTestCfg->TestModel = BE_TEST_1;
    pTestCfg->nAntennaPort = 2;
    pTestCfg->nCw = 1;
    pTestCfg->nLayers = 2;
    pTestCfg->precodingMode = 0;
    pTestCfg->duration = 10;
    pTestCfg->transmissionMode = TXDIVERSITY;//OPNLPSPMUX;

    pTestCfg->rsBoosting = 1;
    pTestCfg->epreSinchro = FLOAT2Q8(0);
    pTestCfg->reservedEpre = MINUS_INF_Q8;

    /* PBCH */
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = 0;
    pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
    pTestCfg->payloadPbch[0] = 0x60;
    pTestCfg->payloadPbch[1] = 0x60;
    pTestCfg->payloadPbch[2] = 0xB3;
    pTestCfg->payloadPbch[3] = 0x00;

    /* PCFICH */
    pTestCfg->nCtrlSymbols = 1;
    pTestCfg->nPhichGroup =  0;//2; phich disable
    pTestCfg->nPhichPerGroup = 2;
    pTestCfg->phichSymPower = FLOAT2Q8(-3.010);
    pTestCfg->phichGroupEpre = FLOAT2Q8(0);

    /* PDCCH */
    pTestCfg->nAvalableRegs = 90;
    pTestCfg->nPdcch = 1;
    pTestCfg->nCcePerPdcch = 4;
    pTestCfg->nRegPerCce = 9;
    pTestCfg->nRegAllToPdcch = 90;
    pTestCfg->nNilRegPadding = 0;
    pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
    pTestCfg->nilRegEpre = MINUS_INF_Q8;

    /* DCI */
//     7F FF D2 E4 84 00
/*    pTestCfg->payloadPdcch[0][0] = 0x7f;
    pTestCfg->payloadPdcch[0][1] = 0xff;
    pTestCfg->payloadPdcch[0][2] = 0xC0;
    pTestCfg->payloadPdcch[0][3] = 0xe4;
    pTestCfg->payloadPdcch[0][4] = 0x01;
    pTestCfg->payloadPdcch[0][5] = 0x00;*/
    pTestCfg->PdcchDataLen[0] = 43;

    pDciInfo->rah = 0;
    pDciInfo->virtul_resrc_blk_type = 0;
    pDciInfo->rbg = 0x1ffff;
    pDciInfo->tpc = 0;
    pDciInfo->harq_process_id = 0;
    pDciInfo->swap_flag = 0;
    pDciInfo->tb_info[0].modulation_type = 0x1c;
    pDciInfo->tb_info[0].ndi = 1;
    pDciInfo->tb_info[0].redundancy_ver = 0;
    pDciInfo->tb_info[1].modulation_type = 0x0;
    pDciInfo->tb_info[1].ndi = 0;
    pDciInfo->tb_info[1].redundancy_ver = 1;
    pDciInfo->precoding_info = 0;


    dci_encode_2((U32*)&pTestCfg->payloadPdcch[0][0], (DCI_INFO *)pDciInfo, (BS_STATIC_CONFIG *)&StaticConfig, &index);
    pTestCfg->PdcchDataLen[0] = index;


    /* PDSCH */
    pTestCfg->nPdsch[0] = 1;
    pTestCfg->nPdsch[1] = 1;
    pTestCfg->nPdschPrb[0][0] = 50;
    pTestCfg->pdschPrbPa[0][0] = FLOAT2Q8(0);
    pTestCfg->pdschMod[0][0] = QAM64;
    pTestCfg->nPdschPrb[1][0] = 50;
    pTestCfg->pdschPrbPa[1][0] = FLOAT2Q8(0);
    pTestCfg->pdschMod[1][0] = QAM16;
   
    pTestCfg->PdschCfgEna = 1;
    pTestCfg->PdschCfg[0].CwCfg[0].Mcs = 24;
    pTestCfg->PdschCfg[0].CwCfg[0].Mod = QAM64;
    pTestCfg->PdschCfg[0].CwCfg[0].Rvi = 0;
    pTestCfg->PdschCfg[0].CwCfg[1].Mcs = 24;
    pTestCfg->PdschCfg[0].CwCfg[1].Mod = QAM64;
    pTestCfg->PdschCfg[0].CwCfg[1].Rvi = 0;    

    pTestCfg->PdschCfg[0].Rnti = 100;
    pTestCfg->PdschCfg[0].ueCategory = 2;
    pTestCfg->PdschCfg[0].Pa = 0;
    
    pTestCfg->PdschCfg[0].Map.numberofEntries = 1;
    pTestCfg->PdschCfg[0].Map.reselmInfo[0].startRes = 0;
    pTestCfg->PdschCfg[0].Map.reselmInfo[0].numRes = 50;

    pTestCfg->codeBookIdx = 1;


    /* PUSCH */
    memset(pTestCfg->numPusch, 1, sizeof(pTestCfg->numPusch));
    pTestCfg->nPuschBoostedPrb = 45;
    pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);
    pTestCfg->puschBoostedMod = QAM16;//QPSK;

    pTestCfg->rntiPusch[0] = 100;//100;

    pTestCfg->rntiBoosted[0] = 100;//100;
    pTestCfg->rntiDeBoosted[0] = 100;//100;

    pTestCfg->rootSequenceIndex = 22;
    pTestCfg->prachConfigIndex = 0;
    pTestCfg->highSpeedFlag =  0;
    pTestCfg->zeroCorrelationZoneConfig = 1;
    pTestCfg->prachFreqOffset = 0;

    /* Boosting map */
    testMgrCtx.boostingMap[0][0] = (U8**)E_TM_3_1MapIndex[BW_10_0_MHZ];
    testMgrCtx.boostingMap[1][0] = (U8**)E_TM_3_1MapIndex[BW_10_0_MHZ];
    testMgrCtx.boostingMap[0][1] = NULL; /* No deboosted data */

    testMgrCtx.boostingUlMap = FdxULMapIndex[BW_10_0_MHZ];

    bs_static_config_update_10mhz(&pBsMacDesc->initParm);
    pBsMacDesc->initParm.txAntennaPortCount = TXANT_TWO;
    pBsMacDesc->initParm.rxAntennaPortCount = RXANT_TWO;
    return SUCCESS;
}

RESULTCODE TestMgrSetRtConfig10Mhz2TxAnt2RxAnt(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
    printf("%s\n",__FUNCTION__);

    StaticConfig.tdd = 0;
    StaticConfig.bw_mode = BW_10_0_MHZ;
    StaticConfig.n_antenna_port = TXANT_TWO;
    StaticConfig.n_antenna_port_rx = RXANT_TWO;
    initBS(&StaticConfig);    

    pTestCfg->TestModel = BE_TEST_1;
    pTestCfg->nAntennaPort = 2;
    pTestCfg->nCw = 2;
    pTestCfg->nLayers = 2;
    pTestCfg->precodingMode = 0;
    pTestCfg->duration = 10;
    pTestCfg->transmissionMode = OPNLPSPMUX;

    pTestCfg->rsBoosting = 1;
    pTestCfg->epreSinchro = FLOAT2Q8(0);
    pTestCfg->reservedEpre = MINUS_INF_Q8;

    /* PBCH */
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = 0;
    pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
    pTestCfg->payloadPbch[0] = 0x60;
    pTestCfg->payloadPbch[1] = 0xC0;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;

    /* PCFICH */
    pTestCfg->nCtrlSymbols = 3; //1;
    pTestCfg->nPhichGroup =  0;//2; phich disable
    pTestCfg->nPhichPerGroup = 2;
    pTestCfg->phichSymPower = FLOAT2Q8(-3.010);
    pTestCfg->phichGroupEpre = FLOAT2Q8(0);

    /* PDCCH */
    pTestCfg->nAvalableRegs = 90;
    pTestCfg->nPdcch = 1;
    pTestCfg->nCcePerPdcch = 1;
    pTestCfg->nRegPerCce = 9;
    pTestCfg->nRegAllToPdcch = 90;
    pTestCfg->nNilRegPadding = 0;
    pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
    pTestCfg->nilRegEpre = MINUS_INF_Q8;

    /* DCI */
    pTestCfg->payloadPdcch[0][0] = 0x8E;
    pTestCfg->payloadPdcch[0][1] = 0x14;
    pTestCfg->payloadPdcch[0][2] = 0x14;
    pTestCfg->payloadPdcch[0][3] = 0x80;
    pTestCfg->PdcchDataLen[0] = 27;


    /* PDSCH */
    pTestCfg->nPdsch[0] = 1;   
    pTestCfg->nPdschPrb[0][0] = 50;
    pTestCfg->pdschPrbPa[0][0] = FLOAT2Q8(0);
    pTestCfg->nPdschBoostedPrb = 1;

    pTestCfg->PdschCfgEna = 1;
    pTestCfg->PdschCfg[0].CwCfg[0].Mcs = 24;
    pTestCfg->PdschCfg[0].CwCfg[0].Mod = QAM64;
    pTestCfg->PdschCfg[0].CwCfg[0].Rvi = 0;
    pTestCfg->PdschCfg[0].CwCfg[1].Mcs = 24;
    pTestCfg->PdschCfg[0].CwCfg[1].Mod = QAM64;
    pTestCfg->PdschCfg[0].CwCfg[1].Rvi = 0;    

    pTestCfg->PdschCfg[0].Rnti = 100;
    pTestCfg->PdschCfg[0].ueCategory = 2;
    pTestCfg->PdschCfg[0].Pa = 0;
    
    pTestCfg->PdschCfg[0].Map.numberofEntries = 1;
    pTestCfg->PdschCfg[0].Map.reselmInfo[0].startRes = 0;
    pTestCfg->PdschCfg[0].Map.reselmInfo[0].numRes = 50;

    pTestCfg->codeBookIdx = 1;

    /* PUSCH */
    memset(pTestCfg->numPusch, 1, sizeof(pTestCfg->numPusch));
    pTestCfg->nPuschBoostedPrb = 45;
    pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);
    pTestCfg->puschBoostedMod = QAM64;//QPSK;

    pTestCfg->rntiPusch[0] = 100;//100;

    pTestCfg->rntiBoosted[0] = 100;//100;
    pTestCfg->rntiDeBoosted[0] = 100;//100;

    pTestCfg->prachMap[0] = PRACH_ON;

    /* Boosting map */
    testMgrCtx.boostingMap[0][0] = (U8**)E_TM_3_1MapIndex[BW_10_0_MHZ];
    testMgrCtx.deBoostingMap = NULL; /* No deboosted data */

    testMgrCtx.boostingUlMap = FdxULMapIndex[BW_10_0_MHZ];

    bs_static_config_update_10mhz(&pBsMacDesc->initParm);
    pBsMacDesc->initParm.txAntennaPortCount = TXANT_TWO;
    pBsMacDesc->initParm.rxAntennaPortCount = RXANT_TWO;
    
    return SUCCESS;

}


RESULTCODE TestMgrSetRtConfig10Mhz2ANTHdx(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
    printf("%s\n",__FUNCTION__);

    StaticConfig.tdd = 0;
    StaticConfig.bw_mode = BW_10_0_MHZ;
    StaticConfig.n_antenna_port = TXANT_TWO;
    StaticConfig.n_antenna_port_rx = RXANT_ONE;
    initBS(&StaticConfig);    

    pTestCfg->TestModel = BE_TEST_1;
    pTestCfg->nAntennaPort = 2;
    pTestCfg->nCw = 2;
    pTestCfg->nLayers = 2;
    pTestCfg->precodingMode = 0;
    pTestCfg->duration = 10;
    pTestCfg->transmissionMode = OPNLPSPMUX;

    pTestCfg->rsBoosting = 1;
    pTestCfg->epreSinchro = FLOAT2Q8(0);
    pTestCfg->reservedEpre = MINUS_INF_Q8;

    /* PBCH */
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = 0;
    pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
    pTestCfg->payloadPbch[0] = 0x60;
    pTestCfg->payloadPbch[1] = 0xC0;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;

    /* PCFICH */
    pTestCfg->nCtrlSymbols = 3; //1;
    pTestCfg->nPhichGroup =  0;//2; phich disable
    pTestCfg->nPhichPerGroup = 2;
    pTestCfg->phichSymPower = FLOAT2Q8(-3.010);
    pTestCfg->phichGroupEpre = FLOAT2Q8(0);

    /* PDCCH */
    pTestCfg->nAvalableRegs = 90;
    pTestCfg->nPdcch = 1;
    pTestCfg->nCcePerPdcch = 1;
    pTestCfg->nRegPerCce = 9;
    pTestCfg->nRegAllToPdcch = 90;
    pTestCfg->nNilRegPadding = 0;
    pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
    pTestCfg->nilRegEpre = MINUS_INF_Q8;

    /* DCI */
    pTestCfg->payloadPdcch[0][0] = 0x8E;
    pTestCfg->payloadPdcch[0][1] = 0x14;
    pTestCfg->payloadPdcch[0][2] = 0x14;
    pTestCfg->payloadPdcch[0][3] = 0x80;
    pTestCfg->PdcchDataLen[0] = 27;


    /* PDSCH */
    pTestCfg->nPdsch[0] = 1;
    pTestCfg->nPdschPrb[0][0] = 50;
    pTestCfg->pdschPrbPa[0][0] = FLOAT2Q8(0);
    pTestCfg->nPdschBoostedPrb = 1;
   
    pTestCfg->PdschCfgEna = 1;
    pTestCfg->PdschCfg[0].CwCfg[0].Mcs = 24;
    pTestCfg->PdschCfg[0].CwCfg[0].Mod = QAM64;
    pTestCfg->PdschCfg[0].CwCfg[0].Rvi = 0;
    pTestCfg->PdschCfg[0].CwCfg[1].Mcs = 24;
    pTestCfg->PdschCfg[0].CwCfg[1].Mod = QAM64;
    pTestCfg->PdschCfg[0].CwCfg[1].Rvi = 0;    

    pTestCfg->PdschCfg[0].Rnti = 100;
    pTestCfg->PdschCfg[0].ueCategory = 2;
    pTestCfg->PdschCfg[0].Pa = 0;
    
    pTestCfg->PdschCfg[0].Map.numberofEntries = 1;
    pTestCfg->PdschCfg[0].Map.reselmInfo[0].startRes = 0;
    pTestCfg->PdschCfg[0].Map.reselmInfo[0].numRes = 50;

    pTestCfg->codeBookIdx = 1;

    /* PUSCH */
    memset(pTestCfg->numPusch, 0, sizeof(pTestCfg->numPusch));
    pTestCfg->nPuschBoostedPrb = 45;
    pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);
    pTestCfg->puschBoostedMod = QAM64;//QPSK;

    pTestCfg->rntiPusch[0] = 100;//100;

    pTestCfg->rntiBoosted[0] = 100;//100;
    pTestCfg->rntiDeBoosted[0] = 100;//100;

    pTestCfg->prachMap[0] = PRACH_ON;

    /* Boosting map */
    testMgrCtx.boostingMap[0][0] = (U8**)E_TM_3_1MapIndex[BW_10_0_MHZ];
    testMgrCtx.deBoostingMap = NULL; /* No deboosted data */

    testMgrCtx.boostingUlMap = FdxULMapIndex[BW_10_0_MHZ];

    bs_static_config_update_10mhz(&pBsMacDesc->initParm);
    pBsMacDesc->initParm.txAntennaPortCount = TXANT_TWO;
    pBsMacDesc->initParm.rxAntennaPortCount = RXANT_ONE;
    
    return SUCCESS;
}
RESULTCODE TestMgrSetRtConfig10Mhz2TxAnt2RxAntHdx(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
    printf("%s\n",__FUNCTION__);

    StaticConfig.tdd = 0;
    StaticConfig.bw_mode = BW_10_0_MHZ;
    StaticConfig.n_antenna_port = TXANT_TWO;
    StaticConfig.n_antenna_port_rx = RXANT_TWO;
    initBS(&StaticConfig);    

    pTestCfg->TestModel = BE_TEST_1;
    pTestCfg->nAntennaPort = 2;
    pTestCfg->nCw = 2;
    pTestCfg->nLayers = 2;
    pTestCfg->precodingMode = 0;
    pTestCfg->duration = 10;
    pTestCfg->transmissionMode = OPNLPSPMUX;

    pTestCfg->rsBoosting = 1;
    pTestCfg->epreSinchro = FLOAT2Q8(0);
    pTestCfg->reservedEpre = MINUS_INF_Q8;

    /* PBCH */
    pTestCfg->pbchEna = 1;    
    pTestCfg->pbchEpre = 0;
    pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
    pTestCfg->payloadPbch[0] = 0x60;
    pTestCfg->payloadPbch[1] = 0xC0;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;

    /* PCFICH */
    pTestCfg->nCtrlSymbols = 3; //1;
    pTestCfg->nPhichGroup =  0;//2; phich disable
    pTestCfg->nPhichPerGroup = 2;
    pTestCfg->phichSymPower = FLOAT2Q8(-3.010);
    pTestCfg->phichGroupEpre = FLOAT2Q8(0);

    /* PDCCH */
    pTestCfg->nAvalableRegs = 90;
    pTestCfg->nPdcch = 1;
    pTestCfg->nCcePerPdcch = 1;
    pTestCfg->nRegPerCce = 9;
    pTestCfg->nRegAllToPdcch = 90;
    pTestCfg->nNilRegPadding = 0;
    pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
    pTestCfg->nilRegEpre = MINUS_INF_Q8;

    /* DCI */
    pTestCfg->payloadPdcch[0][0] = 0x8E;
    pTestCfg->payloadPdcch[0][1] = 0x14;
    pTestCfg->payloadPdcch[0][2] = 0x14;
    pTestCfg->payloadPdcch[0][3] = 0x80;
    pTestCfg->PdcchDataLen[0] = 27;


    /* PDSCH */
    pTestCfg->nPdsch[0] = 1;   
    pTestCfg->nPdschPrb[0][0] = 50;
    pTestCfg->pdschPrbPa[0][0] = FLOAT2Q8(0);
    pTestCfg->nPdschBoostedPrb = 1;
   
    pTestCfg->PdschCfgEna = 1;
    pTestCfg->PdschCfg[0].CwCfg[0].Mcs = 24;
    pTestCfg->PdschCfg[0].CwCfg[0].Mod = QAM64;
    pTestCfg->PdschCfg[0].CwCfg[0].Rvi = 0;
    pTestCfg->PdschCfg[0].CwCfg[1].Mcs = 24;
    pTestCfg->PdschCfg[0].CwCfg[1].Mod = QAM64;
    pTestCfg->PdschCfg[0].CwCfg[1].Rvi = 0;    

    pTestCfg->PdschCfg[0].Rnti = 100;
    pTestCfg->PdschCfg[0].ueCategory = 2;
    pTestCfg->PdschCfg[0].Pa = 0;
    
    pTestCfg->PdschCfg[0].Map.numberofEntries = 1;
    pTestCfg->PdschCfg[0].Map.reselmInfo[0].startRes = 0;
    pTestCfg->PdschCfg[0].Map.reselmInfo[0].numRes = 50;

    pTestCfg->codeBookIdx = 1;

    /* PUSCH */
    memset(pTestCfg->numPusch, 0, sizeof(pTestCfg->numPusch));
    pTestCfg->nPuschBoostedPrb = 45;
    pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);
    pTestCfg->puschBoostedMod = QAM64;//QPSK;

    pTestCfg->rntiPusch[0] = 100;//100;

    pTestCfg->rntiBoosted[0] = 100;//100;
    pTestCfg->rntiDeBoosted[0] = 100;//100;

    pTestCfg->prachMap[0] = PRACH_ON;

    /* Boosting map */
    testMgrCtx.boostingMap[0][0] = (U8**)E_TM_3_1MapIndex[BW_10_0_MHZ];
    testMgrCtx.deBoostingMap = NULL; /* No deboosted data */

    testMgrCtx.boostingUlMap = FdxULMapIndex[BW_10_0_MHZ];

    bs_static_config_update_10mhz(&pBsMacDesc->initParm);
    pBsMacDesc->initParm.txAntennaPortCount = TXANT_TWO;
    pBsMacDesc->initParm.rxAntennaPortCount = RXANT_TWO;
    
    return SUCCESS;

}


// TODO: replase with RESULTCODE TestMgrSet_Diversity_20Mhz_2ANT_2Layers
RESULTCODE TestMgrSetRtConfig20Mhz2ANT(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
#ifndef MX_ARM_ENABLED
    printf("%s\n",__FUNCTION__);

    pTestCfg->TestModel = BE_TEST_2;
    pTestCfg->nAntennaPort = 2;
    pTestCfg->nCw = 2;
    pTestCfg->nLayers = 2;
    pTestCfg->precodingMode = 0;
    pTestCfg->duration = 10;
    pTestCfg->transmissionMode = OPNLPSPMUX;

    pTestCfg->rsBoosting = 1;
    pTestCfg->epreSinchro = FLOAT2Q8(0);
    pTestCfg->reservedEpre = MINUS_INF_Q8;

    /* PBCH */
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = 0;
    pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
    pTestCfg->payloadPbch[0] = 0x60;
    pTestCfg->payloadPbch[1] = 0xC0;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;

    /* PCFICH */
    pTestCfg->nCtrlSymbols = 1;
    pTestCfg->nPhichGroup =  0;//2; phich disable
    pTestCfg->nPhichPerGroup = 2;
    pTestCfg->phichSymPower = FLOAT2Q8(-3.010);
    pTestCfg->phichGroupEpre = FLOAT2Q8(0);

    /* PDCCH */
    pTestCfg->nAvalableRegs = 90;
    pTestCfg->nPdcch = 1;
    pTestCfg->nCcePerPdcch = 1;
    pTestCfg->nRegPerCce = 9;
    pTestCfg->nRegAllToPdcch = 90;
    pTestCfg->nNilRegPadding = 0;
    pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
    pTestCfg->nilRegEpre = MINUS_INF_Q8;

    /* DCI */
    pTestCfg->payloadPdcch[0][0] = 0x9C;
    pTestCfg->payloadPdcch[0][1] = 0x24;
    pTestCfg->payloadPdcch[0][2] = 0x14;
    pTestCfg->payloadPdcch[0][3] = 0x80;
    pTestCfg->PdcchDataLen[0] = 27;

    /* PUSCH */
    memset(pTestCfg->numPusch, 1, sizeof(pTestCfg->numPusch));
    pTestCfg->nPuschBoostedPrb = 45;
    pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);
    pTestCfg->puschBoostedMod = QAM64;//QPSK;

    pTestCfg->rntiPusch[0] = 100;//100;

    pTestCfg->rntiBoosted[0] = 100;//100;
    pTestCfg->rntiDeBoosted[0] = 100;//100;

    /* Boosting map */
    testMgrCtx.boostingMap[0][0] = (U8**)E_TM_3_1MapIndex[BW_20_0_MHZ];
    testMgrCtx.deBoostingMap = NULL; /* No deboosted data */

    testMgrCtx.boostingUlMap = FdxULMapIndex[BW_20_0_MHZ];
    Configure20MhzBw(&pBsMacDesc->initParm);
#endif
    return SUCCESS;
}

RESULTCODE TestMgrSetRtConfig10MhzFdx(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
    DCI_INFO    DciInfo;
    DCI_INFO    *pDciInfo = &DciInfo;
    U32         index;

    printf("%s\n",__FUNCTION__);

    StaticConfig.tdd = 0;
    StaticConfig.bw_mode = BW_10_0_MHZ;
    StaticConfig.n_antenna_port = 1;
    initBS(&StaticConfig);

//--------------------------------------------------------------------------------------------------------------------------------------------------

    pTestCfg->TestModel = E_TM_3_1;
    pTestCfg->nAntennaPort = 1;
    pTestCfg->nCw = 1;
    pTestCfg->nLayers = 1;
    pTestCfg->precodingMode = 0;
    pTestCfg->duration = 10;
    pTestCfg->transmissionMode = SINGLEANT;

    pTestCfg->rsBoosting = 1;
    pTestCfg->epreSinchro = FLOAT2Q8(0);
    pTestCfg->reservedEpre = MINUS_INF_Q8;

    /* PBCH */
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = 0;
    pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
    pTestCfg->payloadPbch[0] = 0x60;
    pTestCfg->payloadPbch[1] = 0xC0;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;

    /* PCFICH */
    pTestCfg->nCtrlSymbols = 3; //1;
    pTestCfg->nPhichGroup = 0;
    pTestCfg->nPhichPerGroup = 2;
    pTestCfg->phichSymPower = FLOAT2Q8(-3.010);
    pTestCfg->phichGroupEpre = FLOAT2Q8(0);

    /* PDCCH */
    pTestCfg->nAvalableRegs = 90;
    pTestCfg->nPdcch = 1;
    pTestCfg->nCcePerPdcch = 1;
    pTestCfg->nRegPerCce = 9;
    pTestCfg->nRegAllToPdcch = 90;
    pTestCfg->nNilRegPadding = 0;
    pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
    pTestCfg->nilRegEpre = MINUS_INF_Q8;

    /* DCI */
    /*pTestCfg->payloadPdcch[0][0] = 0x8E;
    pTestCfg->payloadPdcch[0][1] = 0x14;
    pTestCfg->payloadPdcch[0][2] = 0x14;
    pTestCfg->payloadPdcch[0][3] = 0x80;
    pTestCfg->PdcchDataLen[0] = 27;*/

    pDciInfo->virtul_resrc_blk_type = 0;
    pDciInfo->start_rb_index = 0;
    pDciInfo->l_crb = 10;
    pDciInfo->tb_info[0].modulation_type = 16;
    pDciInfo->harq_process_id = 2;
    pDciInfo->tb_info[0].ndi = 1;
    pDciInfo->tb_info[0].redundancy_ver = 0;
    pDciInfo->tpc = 2;

    dci_encode_1a((U32*)&pTestCfg->payloadPdcch[0][0], (DCI_INFO *)pDciInfo, (BS_STATIC_CONFIG *)&StaticConfig, &index);
    pTestCfg->PdcchDataLen[0] = index;

    printf("pTestCfg->payloadPdcch[0][0] = %2.2x \npTestCfg->payloadPdcch[0][1] = %2.2x \npTestCfg->payloadPdcch[0][2] = %2.2x \npTestCfg->payloadPdcch[0][3] = %2.2x\n",
                pTestCfg->payloadPdcch[0][0], pTestCfg->payloadPdcch[0][1], pTestCfg->payloadPdcch[0][2], pTestCfg->payloadPdcch[0][3]);

    printf("pTestCfg->payloadPdcch[0][4] = %2.2x \npTestCfg->payloadPdcch[0][5] = %2.2x \npTestCfg->payloadPdcch[0][6] = %2.2x \npTestCfg->payloadPdcch[0][7] = %2.2x\n",
                pTestCfg->payloadPdcch[0][4], pTestCfg->payloadPdcch[0][5], pTestCfg->payloadPdcch[0][6], pTestCfg->payloadPdcch[0][7]);

    printf("pTestCfg->PdcchDataLen[0] = %2.2x\n",
               pTestCfg->PdcchDataLen[0]);

    /* PDSCH */
    pTestCfg->nPdsch[0] = 1;
    pTestCfg->nPdschPrb[0][0] = 10;
    pTestCfg->pdschPrbPa[0][0] = FLOAT2Q8(0);
    pTestCfg->pdschMod[0][0] = QAM16;

    /* PUSCH */
    memset(pTestCfg->numPusch, 0, sizeof(pTestCfg->numPusch));
    pTestCfg->nPuschBoostedPrb = 0;
    pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);
    pTestCfg->puschBoostedMod = QAM16;//QPSK;

    pTestCfg->rootSequenceIndex = 22;
    pTestCfg->prachConfigIndex = 12;
    pTestCfg->highSpeedFlag =  0;
    pTestCfg->zeroCorrelationZoneConfig = 1;
    pTestCfg->prachFreqOffset = 0;

    pTestCfg->rntiPusch[0] = 100;//100;

    pTestCfg->rntiBoosted[0] = 100;//100;
    pTestCfg->rntiDeBoosted[0] = 100;//100;

    /* Boosting map */
    testMgrCtx.boostingMap[0][0] = (U8**)FdxMapIndex[BW_10_0_MHZ];
    testMgrCtx.boostingMap[0][1] = NULL; /* No deboosted data */

    testMgrCtx.boostingUlMap = FdxULMapIndex[BW_10_0_MHZ];

    return SUCCESS;
}

RESULTCODE TestMgrSetRtConfig10MhzQAM16MaxFdx(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
    DCI_INFO    DciInfo;
    DCI_INFO    *pDciInfo = &DciInfo;
    U32                 index;

    printf("%s\n",__FUNCTION__);


    StaticConfig.tdd = 0;
    StaticConfig.bw_mode = BW_10_0_MHZ;
    StaticConfig.n_antenna_port = 1;
    initBS(&StaticConfig);

//--------------------------------------------------------------------------------------------------------------------------------------------------

    pTestCfg->TestModel = E_TM_3_1;
    pTestCfg->nAntennaPort = 1;
    pTestCfg->nCw = 1;
    pTestCfg->nLayers = 1;
    pTestCfg->precodingMode = 0;
    pTestCfg->duration = 10;
    pTestCfg->transmissionMode = SINGLEANT;

    pTestCfg->rsBoosting = 1;
    pTestCfg->epreSinchro = FLOAT2Q8(0);
    pTestCfg->reservedEpre = MINUS_INF_Q8;

    /* PBCH */
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = 0;
    pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
    pTestCfg->payloadPbch[0] = 0x60;
    pTestCfg->payloadPbch[1] = 0xC0;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;

    /* PCFICH */
    pTestCfg->nCtrlSymbols = 3; //1;
    pTestCfg->nPhichGroup =  0;//2; phich disable
    pTestCfg->nPhichPerGroup = 2;
    pTestCfg->phichSymPower = FLOAT2Q8(-3.010);
    pTestCfg->phichGroupEpre = FLOAT2Q8(0);

    /* PDCCH */
    pTestCfg->nAvalableRegs = 90;
    pTestCfg->nPdcch = 1;
    pTestCfg->nCcePerPdcch = 1;
    pTestCfg->nRegPerCce = 9;
    pTestCfg->nRegAllToPdcch = 90;
    pTestCfg->nNilRegPadding = 0;
    pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
    pTestCfg->nilRegEpre = MINUS_INF_Q8;

    /* DCI */
    /*pTestCfg->payloadPdcch[0][0] = 0x83;
    pTestCfg->payloadPdcch[0][1] = 0x1C;
    pTestCfg->payloadPdcch[0][2] = 0x14;
    pTestCfg->payloadPdcch[0][3] = 0x80;*/
    pTestCfg->PdcchDataLen[0] = 27;

    pDciInfo->virtul_resrc_blk_type = 0;
    pDciInfo->start_rb_index = 0;
    pDciInfo->l_crb = 50;
    pDciInfo->tb_info[0].modulation_type = 16;
    pDciInfo->harq_process_id = 2;
    pDciInfo->tb_info[0].ndi = 1;
    pDciInfo->tb_info[0].redundancy_ver = 0;
    pDciInfo->tpc = 2;

    dci_encode_1a((U32*)&pTestCfg->payloadPdcch[0][0], (DCI_INFO *)pDciInfo, (BS_STATIC_CONFIG *)&StaticConfig, &index);
    pTestCfg->PdcchDataLen[0] = index;

    /* PDSCH */
    pTestCfg->nPdsch[0] = 1;
    pTestCfg->nPdschPrb[0][0] = 10;
    pTestCfg->pdschPrbPa[0][0] = FLOAT2Q8(0);
    pTestCfg->pdschMod[0][0] = QAM16;

    /* PUSCH */
    memset(pTestCfg->numPusch, 1, sizeof(pTestCfg->numPusch));
    pTestCfg->nPuschBoostedPrb = 45;
    pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);
    pTestCfg->puschBoostedMod = QAM16;//QPSK;

    pTestCfg->rntiPusch[0] = 100;//100;

    pTestCfg->rntiBoosted[0] = 100;//100;
    pTestCfg->rntiDeBoosted[0] = 100;//100;

    pTestCfg->prachConfigIndex = 0xFF;

    /* Boosting map */
    testMgrCtx.boostingMap[0][0] = (U8**)E_TM_3_1MapIndex[BW_10_0_MHZ];
    testMgrCtx.boostingMap[0][1] = NULL; /* No deboosted data */

    testMgrCtx.boostingUlMap = FdxULMapIndex[BW_10_0_MHZ];

    return SUCCESS;
}

RESULTCODE TestMgrSetRtConfig10MhzQAM64_25_50(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
    DCI_INFO    DciInfo;
    DCI_INFO    *pDciInfo = &DciInfo;
    U32                 index;

    printf("%s\n",__FUNCTION__);


    StaticConfig.tdd = 0;
    StaticConfig.bw_mode = BW_10_0_MHZ;
    StaticConfig.n_antenna_port = 1;
    initBS(&StaticConfig);

//--------------------------------------------------------------------------------------------------------------------------------------------------

    pTestCfg->TestModel = E_TM_3_1;
    pTestCfg->nAntennaPort = 1;
    pTestCfg->nCw = 1;
    pTestCfg->nLayers = 1;
    pTestCfg->precodingMode = 0;
    pTestCfg->duration = 10;
    pTestCfg->transmissionMode = SINGLEANT;

    pTestCfg->rsBoosting = 1;
    pTestCfg->epreSinchro = FLOAT2Q8(0);
    pTestCfg->reservedEpre = MINUS_INF_Q8;

    /* PBCH */
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = 0;
    pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
    pTestCfg->payloadPbch[0] = 0x60;
    pTestCfg->payloadPbch[1] = 0xC0;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;

    /* PCFICH */
    pTestCfg->nCtrlSymbols = 1;
    pTestCfg->nPhichGroup =  0;//2; phich disable
    pTestCfg->nPhichPerGroup = 2;
    pTestCfg->phichSymPower = FLOAT2Q8(-3.010);
    pTestCfg->phichGroupEpre = FLOAT2Q8(0);

    /* PDCCH */
    pTestCfg->nAvalableRegs = 90;
    pTestCfg->nPdcch = 1;
    pTestCfg->nCcePerPdcch = 1;
    pTestCfg->nRegPerCce = 9;
    pTestCfg->nRegAllToPdcch = 90;
    pTestCfg->nNilRegPadding = 0;
    pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
    pTestCfg->nilRegEpre = MINUS_INF_Q8;

    /* DCI */
    /*pTestCfg->payloadPdcch[0][0] = 0xA6;
    pTestCfg->payloadPdcch[0][1] = 0x4F;
    pTestCfg->payloadPdcch[0][2] = 0x14;
    pTestCfg->payloadPdcch[0][3] = 0x80;*/
    pTestCfg->PdcchDataLen[0] = 27;

    pDciInfo->virtul_resrc_blk_type = 0;
    pDciInfo->start_rb_index = 25;
    pDciInfo->l_crb = 25;
    pDciInfo->tb_info[0].modulation_type = 28;
    pDciInfo->harq_process_id = 2;
    pDciInfo->tb_info[0].ndi = 1;
    pDciInfo->tb_info[0].redundancy_ver = 0;
    pDciInfo->tpc = 2;

    dci_encode_1a((U32*)&pTestCfg->payloadPdcch[0][0], (DCI_INFO *)pDciInfo, (BS_STATIC_CONFIG *)&StaticConfig, &index);
    pTestCfg->PdcchDataLen[0] = index;

    /* PDSCH */
    pTestCfg->nPdsch[0] = 1;
    pTestCfg->nPdschPrb[0][0] = 25;
    pTestCfg->pdschPrbPa[0][0] = FLOAT2Q8(0);
    pTestCfg->pdschMod[0][0] = QAM64;

    /* PUSCH */
    memset(pTestCfg->numPusch, 1, sizeof(pTestCfg->numPusch));
    pTestCfg->nPuschBoostedPrb = 45;
    pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);
    pTestCfg->puschBoostedMod = QAM16;//QPSK;

    pTestCfg->rntiPusch[0] = 100;//100;

    pTestCfg->rntiBoosted[0] = 100;//100;
    pTestCfg->rntiDeBoosted[0] = 100;//100;
    pTestCfg->prachConfigIndex = 0xFF;

    /* Boosting map */
    testMgrCtx.boostingMap[0][0] = (U8**)upperHalfMapIndex[BW_10_0_MHZ];
    testMgrCtx.boostingMap[0][1] = NULL; /* No deboosted data */

    testMgrCtx.boostingUlMap = FdxULMapIndex[BW_10_0_MHZ];

    return SUCCESS;
}

RESULTCODE TestMgrSetRtConfig10MhzQAM16_QAM64_2SDUFdx(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
    DCI_INFO    DciInfo;
    DCI_INFO    *pDciInfo = &DciInfo;
    U32                 index;

    printf("%s\n",__FUNCTION__);


    StaticConfig.tdd = 0;
    StaticConfig.bw_mode = BW_10_0_MHZ;
    StaticConfig.n_antenna_port = 1;
    initBS(&StaticConfig);

//--------------------------------------------------------------------------------------------------------------------------------------------------

    pTestCfg->TestModel = E_TM_3_1;
    pTestCfg->nAntennaPort = 1;
    pTestCfg->nCw = 1;
    pTestCfg->nLayers = 1;
    pTestCfg->precodingMode = 0;
    pTestCfg->duration = 10;
    pTestCfg->transmissionMode = SINGLEANT;

    pTestCfg->rsBoosting = 1;
    pTestCfg->epreSinchro = FLOAT2Q8(0);
    pTestCfg->reservedEpre = MINUS_INF_Q8;

    /* PBCH */
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = 0;
    pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
    pTestCfg->payloadPbch[0] = 0x60;
    pTestCfg->payloadPbch[1] = 0xC0;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;

    /* PCFICH */
    pTestCfg->nCtrlSymbols = 1;
    pTestCfg->nPhichGroup =  0;//2; phich disable
    pTestCfg->nPhichPerGroup = 2;
    pTestCfg->phichSymPower = FLOAT2Q8(-3.010);
    pTestCfg->phichGroupEpre = FLOAT2Q8(0);

    /* PDCCH */
    pTestCfg->nAvalableRegs = 90;
    pTestCfg->nPdcch = 2;
    pTestCfg->nCcePerPdcch = 1;
    pTestCfg->nRegPerCce = 9;
    pTestCfg->nRegAllToPdcch = 90;
    pTestCfg->nNilRegPadding = 0;
    pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
    pTestCfg->nilRegEpre = MINUS_INF_Q8;

    /* DCI */
//    A5841480
    /*pTestCfg->payloadPdcch[0][0] = 0xA5;
    pTestCfg->payloadPdcch[0][1] = 0x84;
    pTestCfg->payloadPdcch[0][2] = 0x14;
    pTestCfg->payloadPdcch[0][3] = 0x80;*/
    pTestCfg->PdcchDataLen[0] = 27;

    pDciInfo->virtul_resrc_blk_type = 0;
    pDciInfo->start_rb_index = 0;
    pDciInfo->l_crb = 25;
    pDciInfo->tb_info[0].modulation_type = 16;
    pDciInfo->harq_process_id = 2;
    pDciInfo->tb_info[0].ndi = 1;
    pDciInfo->tb_info[0].redundancy_ver = 0;
    pDciInfo->tpc = 2;

    dci_encode_1a((U32*)&pTestCfg->payloadPdcch[0][0], (DCI_INFO *)pDciInfo, (BS_STATIC_CONFIG *)&StaticConfig, &index);
    pTestCfg->PdcchDataLen[0] = index;

//  A64F1480
    /*pTestCfg->payloadPdcch[1][0] = 0xA6;
    pTestCfg->payloadPdcch[1][1] = 0x4F;
    pTestCfg->payloadPdcch[1][2] = 0x14;
    pTestCfg->payloadPdcch[1][3] = 0x80;*/
    pTestCfg->PdcchDataLen[1] = 27;

    pDciInfo->virtul_resrc_blk_type = 0;
    pDciInfo->start_rb_index = 25;
    pDciInfo->l_crb = 25;
    pDciInfo->tb_info[0].modulation_type = 28;
    pDciInfo->harq_process_id = 2;
    pDciInfo->tb_info[0].ndi = 1;
    pDciInfo->tb_info[0].redundancy_ver = 0;
    pDciInfo->tpc = 2;

    dci_encode_1a((U32*)&pTestCfg->payloadPdcch[1][0], (DCI_INFO *)pDciInfo, (BS_STATIC_CONFIG *)&StaticConfig, &index);
    pTestCfg->PdcchDataLen[1] = index;

    /* PDSCH */
    pTestCfg->nPdsch[0] = 2;
    pTestCfg->nPdschPrb[0][0] = 25;
    pTestCfg->pdschPrbPa[0][0] = FLOAT2Q8(0);
    pTestCfg->pdschMod[0][0] = QAM16;
    pTestCfg->nPdschPrb[0][1] = 25;
    pTestCfg->pdschPrbPa[0][1] = FLOAT2Q8(0);
    pTestCfg->pdschMod[0][1] = QAM64;

    /* PUSCH */
    memset(pTestCfg->numPusch, 1, sizeof(pTestCfg->numPusch));
    pTestCfg->nPuschBoostedPrb = 45;
    pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);
    pTestCfg->puschBoostedMod = QAM16;//QPSK;

    pTestCfg->rntiPusch[0] = 100;//100;

    pTestCfg->rntiBoosted[0] = 100;//100;
    pTestCfg->rntiDeBoosted[0] = 100;//100;

    pTestCfg->prachConfigIndex = 0xFF;

    /* Boosting map */

    testMgrCtx.boostingMap[0][0] = (U8**)HalfMapIndex[BW_10_0_MHZ];
    testMgrCtx.boostingMap[0][1] = (U8**)upperHalfMapIndex[BW_10_0_MHZ];

    testMgrCtx.boostingUlMap = FdxULMapIndex[BW_10_0_MHZ];

    return SUCCESS;
}

RESULTCODE TestMgrSetRtConfig20MhzFdx(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
    DCI_INFO    DciInfo;
    DCI_INFO    *pDciInfo = &DciInfo;
    U32         index;

    printf("%s\n",__FUNCTION__);

    StaticConfig.tdd = 0;
    StaticConfig.bw_mode = BW_20_0_MHZ;
    StaticConfig.n_antenna_port = 1;
    initBS(&StaticConfig);

//--------------------------------------------------------------------------------------------------------------------------------------------------

    pTestCfg->TestModel = E_TM_3_1;
    pTestCfg->nAntennaPort = 1;
    pTestCfg->nCw = 1;
    pTestCfg->nLayers = 1;
    pTestCfg->precodingMode = 0;
    pTestCfg->duration = 10;
    pTestCfg->transmissionMode = SINGLEANT;

    pTestCfg->rsBoosting = 1;
    pTestCfg->epreSinchro = FLOAT2Q8(0);
    pTestCfg->reservedEpre = MINUS_INF_Q8;

    /* PBCH */
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = 0;
    pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
    pTestCfg->payloadPbch[0] = 0xA0;
    pTestCfg->payloadPbch[1] = 0xC0;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;

    /* PCFICH */
    pTestCfg->nCtrlSymbols = 1;//3; //1;
    pTestCfg->nPhichGroup =  0;//2; phich disable
    pTestCfg->nPhichPerGroup = 2;
    pTestCfg->phichSymPower = FLOAT2Q8(-3.010);
    pTestCfg->phichGroupEpre = FLOAT2Q8(0);

    /* PDCCH */
    pTestCfg->nAvalableRegs = 90;
    pTestCfg->nPdcch = 1;
    pTestCfg->nCcePerPdcch = 1;
    pTestCfg->nRegPerCce = 9;
    pTestCfg->nRegAllToPdcch = 90;
    pTestCfg->nNilRegPadding = 0;
    pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
    pTestCfg->nilRegEpre = MINUS_INF_Q8;

    /* DCI */
/*    pTestCfg->payloadPdcch[0][0] = 0x9C;
    pTestCfg->payloadPdcch[0][1] = 0x24;
    pTestCfg->payloadPdcch[0][2] = 0x14;
    pTestCfg->payloadPdcch[0][3] = 0x80;*/
    pTestCfg->PdcchDataLen[0] = 27;


    pDciInfo->virtul_resrc_blk_type = 0;
    pDciInfo->start_rb_index = 0;
    pDciInfo->l_crb = 19;
    pDciInfo->tb_info[0].modulation_type = 16;
    pDciInfo->harq_process_id = 2;
    pDciInfo->tb_info[0].ndi = 1;
    pDciInfo->tb_info[0].redundancy_ver = 0;
    pDciInfo->tpc = 2;

    dci_encode_1a((U32*)&pTestCfg->payloadPdcch[0][0], (DCI_INFO *)pDciInfo, (BS_STATIC_CONFIG *)&StaticConfig, &index);
    pTestCfg->PdcchDataLen[0] = index;

    /* PDSCH */

    pTestCfg->nPdsch[0] = 1;

    pTestCfg->nPdschPrb[0][0] = 10;
    pTestCfg->pdschPrbPa[0][0] = FLOAT2Q8(0);
    pTestCfg->pdschMod[0][0] = QAM16;

    /* PUSCH */
    memset(pTestCfg->numPusch, 1, sizeof(pTestCfg->numPusch));
    pTestCfg->nPuschBoostedPrb = 45;
    pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);
    pTestCfg->puschBoostedMod = QAM16;//QPSK;

    pTestCfg->rntiPusch[0] = 100;//100;

    pTestCfg->rntiBoosted[0] = 100;//100;
    pTestCfg->rntiDeBoosted[0] = 100;//100;

    /* Boosting map */
    testMgrCtx.boostingMap[0][0] = (U8**)FdxMapIndex[BW_20_0_MHZ];

    testMgrCtx.boostingUlMap = FdxULMapIndex[BW_20_0_MHZ];

    bs_static_config_update_20mhz(&pBsMacDesc->initParm);

    return SUCCESS;
}

RESULTCODE TestMgrSetRtConfig10MhzMaxThroughFdx(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
    DCI_INFO    DciInfo;
    DCI_INFO    *pDciInfo = &DciInfo;
    U32         index;

    printf("%s\n",__FUNCTION__);
    printf("DL 36 Mbps UL 19.08 Mbps\n");

    StaticConfig.tdd = 0;
    StaticConfig.bw_mode = BW_10_0_MHZ;
    StaticConfig.n_antenna_port = 1;
    initBS(&StaticConfig);

//--------------------------------------------------------------------------------------------------------------------------------------------------

    pTestCfg->TestModel = E_TM_3_1;
    pTestCfg->nAntennaPort = 1;
    pTestCfg->nCw = 1;
    pTestCfg->nLayers = 1;
    pTestCfg->precodingMode = 0;
    pTestCfg->duration = 10;
    pTestCfg->transmissionMode = SINGLEANT;

    pTestCfg->rsBoosting = 1;
    pTestCfg->epreSinchro = FLOAT2Q8(0);
    pTestCfg->reservedEpre = MINUS_INF_Q8;

    /* PBCH */
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = 0;
    pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
    pTestCfg->payloadPbch[0] = 0x60;
    pTestCfg->payloadPbch[1] = 0xC0;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;

    /* PCFICH */
    pTestCfg->nCtrlSymbols = 1; //1;
    pTestCfg->nPhichGroup =  0;//2; phich disable
    pTestCfg->nPhichPerGroup = 2;
    pTestCfg->phichSymPower = FLOAT2Q8(-3.010);
    pTestCfg->phichGroupEpre = FLOAT2Q8(0);

    /* PDCCH */
    pTestCfg->nAvalableRegs = 90;
    pTestCfg->nPdcch = 1;
    pTestCfg->nCcePerPdcch = 1;
    pTestCfg->nRegPerCce = 9;
    pTestCfg->nRegAllToPdcch = 90;
    pTestCfg->nNilRegPadding = 0;
    pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
    pTestCfg->nilRegEpre = MINUS_INF_Q8;

    /* DCI */
/*    pTestCfg->payloadPdcch[0][0] = 0x83;
    pTestCfg->payloadPdcch[0][1] = 0x1F;
    pTestCfg->payloadPdcch[0][2] = 0x14;
    pTestCfg->payloadPdcch[0][3] = 0x80;*/
    pTestCfg->PdcchDataLen[0] = 27;

    pDciInfo->virtul_resrc_blk_type = 0;
    pDciInfo->start_rb_index = 0;
    pDciInfo->l_crb = 50;
    pDciInfo->tb_info[0].modulation_type = 0x1c;
    pDciInfo->harq_process_id = 2;
    pDciInfo->tb_info[0].ndi = 1;
    pDciInfo->tb_info[0].redundancy_ver = 0;
    pDciInfo->tpc = 2;

    dci_encode_1a((U32*)&pTestCfg->payloadPdcch[0][0], (DCI_INFO *)pDciInfo, (BS_STATIC_CONFIG *)&StaticConfig, &index);
    pTestCfg->PdcchDataLen[0] = index;

    /* PDSCH */
    pTestCfg->nPdsch[0] = 1;
    pTestCfg->nPdschPrb[0][0] = 50;
    pTestCfg->pdschPrbPa[0][0] = FLOAT2Q8(0);
    pTestCfg->pdschMod[0][0] = QAM64;

    /* PUSCH */
    memset(pTestCfg->numPusch, 1, sizeof(pTestCfg->numPusch));
    pTestCfg->nPuschBoostedPrb = 45;
    pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);
    pTestCfg->puschBoostedMod = QAM16;//QPSK;

    pTestCfg->rntiPusch[0] = 100;//100;

    pTestCfg->rntiBoosted[0] = 100;//100;
    pTestCfg->rntiDeBoosted[0] = 100;//100;

    /* Boosting map */
    testMgrCtx.boostingMap[0][0] = (U8**)E_TM_3_1MapIndex[BW_10_0_MHZ];
    testMgrCtx.boostingMap[0][1] = NULL; /* No deboosted data */

    testMgrCtx.boostingUlMap = FdxULMapIndex[BW_10_0_MHZ];
    return SUCCESS;
}

RESULTCODE TestMgrSetRtConfig10MhzMaxThroughFdxDci0Phich(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
    printf("%s\n",__FUNCTION__);

    pTestCfg->TestModel = E_TM_3_1;
    pTestCfg->nAntennaPort = 1;
    pTestCfg->nCw = 1;
    pTestCfg->nLayers = 1;
    pTestCfg->precodingMode = 0;
    pTestCfg->duration = 10;
    pTestCfg->transmissionMode = SINGLEANT;

    pTestCfg->rsBoosting = 1;
    pTestCfg->epreSinchro = FLOAT2Q8(0);
    pTestCfg->reservedEpre = MINUS_INF_Q8;

    /* PBCH */
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = 0;
    pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
    pTestCfg->payloadPbch[0] = 0x60;
    pTestCfg->payloadPbch[1] = 0xC0;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;

    /* PCFICH */
    pTestCfg->nCtrlSymbols = 1; //1;

    pTestCfg->nPhichGroup =  1;
    pTestCfg->nPhichPerGroup = 1;
    pTestCfg->DontUsePuschRxSduForHiAndDci0 = 0;
    pTestCfg->phichSymPower = FLOAT2Q8(-3.010);
    pTestCfg->phichGroupEpre = FLOAT2Q8(0);

    /* PDCCH */
    pTestCfg->nAvalableRegs = 90;
    pTestCfg->nPdcch = 1;
    pTestCfg->nCcePerPdcch = 1;
    pTestCfg->nRegPerCce = 9;
    pTestCfg->nRegAllToPdcch = 90;
    pTestCfg->nNilRegPadding = 0;
    pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
    pTestCfg->nilRegEpre = MINUS_INF_Q8;

    /* DCI */
    pTestCfg->payloadPdcch[0][0] = 0x83;
    pTestCfg->payloadPdcch[0][1] = 0x1F;
    pTestCfg->payloadPdcch[0][2] = 0x14;
    pTestCfg->payloadPdcch[0][3] = 0x80;
    pTestCfg->PdcchDataLen[0] = 27;

    pTestCfg->nPdcchUl = 1;
    /* DCI Format-0 GRANT */
    pTestCfg->payloadPdcchUl[0] = 0x07;
    pTestCfg->payloadPdcchUl[1] = 0xDD;
    pTestCfg->payloadPdcchUl[2] = 0x28;
    pTestCfg->payloadPdcchUl[3] = 0x00;
    pTestCfg->PdcchDataLenUl[0] = 27;

    /* PDSCH */
    pTestCfg->nPdsch[0] = 1;
    pTestCfg->nPdschPrb[0][0] = 50;
    pTestCfg->pdschPrbPa[0][0] = FLOAT2Q8(0);
    pTestCfg->pdschMod[0][0] = QAM64;

    /* PUSCH */
    memset(pTestCfg->numPusch, 1, sizeof(pTestCfg->numPusch));
    pTestCfg->nPuschBoostedPrb = 45;
    pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);
    pTestCfg->puschBoostedMod = QAM16;//QPSK;

    pTestCfg->rntiPusch[0] = 100;//100;

    pTestCfg->rntiBoosted[0] = 100;//100;
    pTestCfg->rntiDeBoosted[0] = 100;//100;

    /* Boosting map */
    testMgrCtx.boostingMap[0][0] = (U8**)E_TM_3_1MapIndex[BW_10_0_MHZ];
    testMgrCtx.boostingMap[0][1] = NULL; /* No deboosted data */

    testMgrCtx.boostingUlMap = FdxULMapIndex[BW_10_0_MHZ];

    return SUCCESS;
}

RESULTCODE TestMgrSetRtConfig10MhzNoRepackFdx(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
    DCI_INFO    DciInfo;
    DCI_INFO    *pDciInfo = &DciInfo;
    U32                 index;

    printf("%s\n",__FUNCTION__);


    StaticConfig.tdd = 0;
    StaticConfig.bw_mode = BW_10_0_MHZ;
    StaticConfig.n_antenna_port = 1;
    initBS(&StaticConfig);

//--------------------------------------------------------------------------------------------------------------------------------------------------

    pTestCfg->TestModel = E_TM_3_1;
    pTestCfg->nAntennaPort = 1;
    pTestCfg->nCw = 1;
    pTestCfg->nLayers = 1;
    pTestCfg->precodingMode = 0;
    pTestCfg->duration = 10;
    pTestCfg->transmissionMode = SINGLEANT;

    pTestCfg->rsBoosting = 1;
    pTestCfg->epreSinchro = FLOAT2Q8(0);
    pTestCfg->reservedEpre = MINUS_INF_Q8;

    /* PBCH */
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = 0;
    pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
    pTestCfg->payloadPbch[0] = 0x60;
    pTestCfg->payloadPbch[1] = 0xC0;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;

    /* PCFICH */
    pTestCfg->nCtrlSymbols = 1;
    pTestCfg->nPhichGroup =  0;// phich disable
    pTestCfg->nPhichPerGroup = 2;
    pTestCfg->phichSymPower = FLOAT2Q8(-3.010);
    pTestCfg->phichGroupEpre = FLOAT2Q8(0);

    /* PDCCH */
    pTestCfg->nAvalableRegs = 90;
    pTestCfg->nPdcch = 1;
    pTestCfg->nCcePerPdcch = 1;
    pTestCfg->nRegPerCce = 9;
    pTestCfg->nRegAllToPdcch = 90;
    pTestCfg->nNilRegPadding = 0;
    pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
    pTestCfg->nilRegEpre = MINUS_INF_Q8;

    /* DCI */
    /*pTestCfg->payloadPdcch[0][0] = 0x83;
    pTestCfg->payloadPdcch[0][1] = 0x1F;
    pTestCfg->payloadPdcch[0][2] = 0x14;
    pTestCfg->payloadPdcch[0][3] = 0x80;*/
    pTestCfg->PdcchDataLen[0] = 27;

    pDciInfo->virtul_resrc_blk_type = 0;
    pDciInfo->start_rb_index = 0;
    pDciInfo->l_crb = 22;
    pDciInfo->tb_info[0].modulation_type = 28;
    pDciInfo->harq_process_id = 2;
    pDciInfo->tb_info[0].ndi = 1;
    pDciInfo->tb_info[0].redundancy_ver = 0;
    pDciInfo->tpc = 2;

    dci_encode_1a((U32*)&pTestCfg->payloadPdcch[0][0], (DCI_INFO *)pDciInfo, (BS_STATIC_CONFIG *)&StaticConfig, &index);
    pTestCfg->PdcchDataLen[0] = index;

    /* PDSCH */
    pTestCfg->nPdsch[0] = 1;

    pTestCfg->nPdschPrb[0][0] = 50;
    pTestCfg->pdschPrbPa[0][0] = FLOAT2Q8(0);
    pTestCfg->pdschMod[0][0] = QAM64;

    /* PUSCH */
    memset(pTestCfg->numPusch, 1, sizeof(pTestCfg->numPusch));
    pTestCfg->nPuschBoostedPrb = 45;
    pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);
    pTestCfg->puschBoostedMod = QAM16;//QPSK;

    pTestCfg->rntiPusch[0] = 100;
    pTestCfg->rntiBoosted[0] = 100;
    pTestCfg->rntiDeBoosted[0] = 100;

    /* Boosting map */
    testMgrCtx.boostingMap[0][0] = (U8**)testMapIndex[BW_10_0_MHZ];
    testMgrCtx.boostingMap[0][1] = NULL;

    testMgrCtx.boostingUlMap = FdxULMapIndex[BW_10_0_MHZ];

    return SUCCESS;
}

RESULTCODE TestMgrSetRtConfig20MhzMaxThroughFdx(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
    DCI_INFO    DciInfo;
    DCI_INFO    *pDciInfo = &DciInfo;
    U32                 index;

    printf("%s\n",__FUNCTION__);

    StaticConfig.tdd = 0;
    StaticConfig.bw_mode = BW_20_0_MHZ;
    StaticConfig.n_antenna_port = 1;
    initBS(&StaticConfig);

//--------------------------------------------------------------------------------------------------------------------------------------------------

    pTestCfg->TestModel = E_TM_3_1;
    pTestCfg->nAntennaPort = 1;
    pTestCfg->nCw = 1;
    pTestCfg->nLayers = 1;
    pTestCfg->precodingMode = 0;
    pTestCfg->duration = 10;
    pTestCfg->transmissionMode = SINGLEANT;

    pTestCfg->rsBoosting = 1;
    pTestCfg->epreSinchro = FLOAT2Q8(0);
    pTestCfg->reservedEpre = MINUS_INF_Q8;

    /* PBCH */
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = 0;
    pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
    pTestCfg->payloadPbch[0] = 0x60;
    pTestCfg->payloadPbch[1] = 0xC0;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;

    /* PCFICH */
    pTestCfg->nCtrlSymbols = 1;
    pTestCfg->nPhichGroup =  0;// phich disable
    pTestCfg->nPhichPerGroup = 2;
    pTestCfg->phichSymPower = FLOAT2Q8(-3.010);
    pTestCfg->phichGroupEpre = FLOAT2Q8(0);

    /* PDCCH */
    pTestCfg->nAvalableRegs = 90;
    pTestCfg->nPdcch = 1;
    pTestCfg->nCcePerPdcch = 1;
    pTestCfg->nRegPerCce = 9;
    pTestCfg->nRegAllToPdcch = 90;
    pTestCfg->nNilRegPadding = 0;
    pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
    pTestCfg->nilRegEpre = MINUS_INF_Q8;

    /* DCI */
    /*pTestCfg->payloadPdcch[0][0] = 0x83;
    pTestCfg->payloadPdcch[0][1] = 0x1F;
    pTestCfg->payloadPdcch[0][2] = 0x14;
    pTestCfg->payloadPdcch[0][3] = 0x80;*/
    pTestCfg->PdcchDataLen[0] = 29;

    pDciInfo->virtul_resrc_blk_type = 0;
    pDciInfo->start_rb_index = 0;
    pDciInfo->l_crb = 50;
    pDciInfo->tb_info[0].modulation_type = 0x1c;
    pDciInfo->harq_process_id = 2;
    pDciInfo->tb_info[0].ndi = 1;
    pDciInfo->tb_info[0].redundancy_ver = 0;
    pDciInfo->tpc = 2;

    dci_encode_1a((U32*)&pTestCfg->payloadPdcch[0][0], (DCI_INFO *)pDciInfo, (BS_STATIC_CONFIG *)&StaticConfig, &index);
    pTestCfg->PdcchDataLen[0] = index;

    /* PDSCH */
    pTestCfg->nPdsch[0] = 1;
    pTestCfg->nPdschPrb[0][0] = 100;
    pTestCfg->pdschPrbPa[0][0] = FLOAT2Q8(0);
    pTestCfg->pdschMod[0][0] = QAM64;

    /* PUSCH */
    memset(pTestCfg->numPusch, 1, sizeof(pTestCfg->numPusch));
    pTestCfg->nPuschBoostedPrb = 45;
    pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);
    pTestCfg->puschBoostedMod = QAM16;

    pTestCfg->rntiPusch[0] = 100;//100;

    pTestCfg->rntiBoosted[0] = 100;//100;
    pTestCfg->rntiDeBoosted[0] = 100;//100;

    /* Boosting map */
    testMgrCtx.boostingMap[0][0] = (U8**)E_TM_3_1MapIndex[BW_20_0_MHZ];
    testMgrCtx.boostingMap[0][1] = NULL; /* No deboosted data */

    testMgrCtx.boostingUlMap = FdxULMapIndex[BW_20_0_MHZ];

    return SUCCESS;
}


RESULTCODE TestMgrSetTm500Config(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
    DCI_INFO    DciInfo;
    DCI_INFO    *pDciInfo = &DciInfo;
    U32                 index;

    printf("%s\n",__FUNCTION__);


    StaticConfig.tdd = 0;
    StaticConfig.bw_mode = BW_10_0_MHZ;
    StaticConfig.n_antenna_port = 1;
    initBS(&StaticConfig);

//--------------------------------------------------------------------------------------------------------------------------------------------------

    printf("Aeroflex TM500 HARQ mode\n");

    memset(pTestCfg, 0, sizeof(BS_CONFOR_TEST_CONFIG));

    lte_modulation_set_mcs_index_ul(QAM16, 12);

    pTestCfg->TestModel = 0;
    pTestCfg->nAntennaPort = 1;
    pTestCfg->nCw = 1;
    pTestCfg->nLayers = 1;
    pTestCfg->precodingMode = 0;
    pTestCfg->duration = 10;
    pTestCfg->transmissionMode = SINGLEANT;

    pTestCfg->rsBoosting = 1;
    pTestCfg->epreSinchro = FLOAT2Q8(0);
    pTestCfg->reservedEpre = MINUS_INF_Q8;

    /* PBCH */
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = 0;
    pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
    pTestCfg->payloadPbch[0] = 0x60;
    pTestCfg->payloadPbch[1] = 0x00;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;

    /* PCFICH */
    pTestCfg->nCtrlSymbols = 1;
    pTestCfg->nPhichGroup =  0;
    pTestCfg->nPhichPerGroup = 2;
    pTestCfg->phichSymPower = FLOAT2Q8(-3.010);
    pTestCfg->phichGroupEpre = FLOAT2Q8(0);

    /* PDCCH */
    pTestCfg->nAvalableRegs = 90;
    pTestCfg->nPdcch = 2;
    pTestCfg->nCcePerPdcch = 2;
    pTestCfg->nRegPerCce = 9;
    pTestCfg->nRegAllToPdcch = 90;
    pTestCfg->nNilRegPadding = 0;
    pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
    pTestCfg->nilRegEpre = MINUS_INF_Q8;

    /* DCI */
/*    pTestCfg->payloadPdcch[0][0] = 0x8E;
    pTestCfg->payloadPdcch[0][1] = 0x14;
    pTestCfg->payloadPdcch[0][2] = 0x14;
    pTestCfg->payloadPdcch[0][3] = 0x80;*/
    pTestCfg->PdcchDataLen[0] = 27;

    pDciInfo->virtul_resrc_blk_type = 0;
    pDciInfo->start_rb_index = 0;
    pDciInfo->l_crb = 10;
    pDciInfo->tb_info[0].modulation_type = 16;
    pDciInfo->harq_process_id = 2;
    pDciInfo->tb_info[0].ndi = 1;
    pDciInfo->tb_info[0].redundancy_ver = 0;
    pDciInfo->tpc = 2;

    dci_encode_1a((U32*)&pTestCfg->payloadPdcch[0][0], (DCI_INFO *)pDciInfo, (BS_STATIC_CONFIG *)&StaticConfig, &index);
    pTestCfg->PdcchDataLen[0] = index;

//     pDciInfo++;refill DCI
/*    pTestCfg->payloadPdcch[1][0] = 0x1D;
    pTestCfg->payloadPdcch[1][1] = 0xDB;
    pTestCfg->payloadPdcch[1][2] = 0x20;
    pTestCfg->payloadPdcch[1][3] = 0x00;*/
    pTestCfg->PdcchDataLen[1] = 27;

    pDciInfo->hopping = 0;
    pDciInfo->start_rb_index = 5;
    pDciInfo->l_crb = 20;
    pDciInfo->mcs_rv = 12;
    pDciInfo->ndi = 1;
    pDciInfo->tpc = 0;
    pDciInfo->cyclic_shift_dm_rs = 0;
    pDciInfo->cqi_request = 0;

    dci_encode_0((U32*)&pTestCfg->payloadPdcch[1][0], (DCI_INFO *)pDciInfo, (BS_STATIC_CONFIG *)&StaticConfig, &index);
    pTestCfg->PdcchDataLen[1] = index;

    /* PDSCH */
    pTestCfg->nPdsch[0] = 1;

    pTestCfg->nPdschPrb[0][0] = 10;
    pTestCfg->pdschPrbPa[0][0] = FLOAT2Q8(0);
    pTestCfg->pdschMod[0][0] = QAM16;

    /* PUSCH */
    pTestCfg->numPusch[0] = 1;
    pTestCfg->numPusch[1] = 1;
    pTestCfg->numPusch[2] = 1;
    pTestCfg->numPusch[3] = 1;
    pTestCfg->numPusch[4] = 1;
    pTestCfg->numPusch[5] = 1;
    pTestCfg->numPusch[6] = 1;
    pTestCfg->numPusch[7] = 1;
    pTestCfg->numPusch[8] = 1;
    pTestCfg->numPusch[9] = 1;

    pTestCfg->nPuschBoostedPrb = 20;
    pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);
    pTestCfg->puschBoostedMod = QAM16;

    /* PUCCH */
    pTestCfg->numPucch[0] = 0;
    pTestCfg->numPucch[1] = 0;
    pTestCfg->numPucch[2] = 0;
    pTestCfg->numPucch[3] = 0;
    pTestCfg->numPucch[4] = 0;
    pTestCfg->numPucch[5] = 0;
    pTestCfg->numPucch[6] = 0;
    pTestCfg->numPucch[7] = 0;
    pTestCfg->numPucch[8] = 0;
    pTestCfg->numPucch[9] = 0;

    /* PRACH */
    pTestCfg->rootSequenceIndex = 22;
    pTestCfg->prachConfigIndex = 12;
    pTestCfg->highSpeedFlag =  0;
    pTestCfg->zeroCorrelationZoneConfig = 1;
    pTestCfg->prachFreqOffset = 0;

    pTestCfg->rntiPusch[0] = 61;

    pTestCfg->rntiBoosted[0] = 61;
    pTestCfg->rntiDeBoosted[0] = 61;

    pTestCfg->ulSfrCtrl.deltaPUCCHShift = 2;
    pTestCfg->ulSfrCtrl.nRBCQI = 2;
    pTestCfg->ulSfrCtrl.n1PucchAN = 2;
    pTestCfg->ulSfrCtrl.nSB = 1;

    pTestCfg->betaOffsetCQIIndex = 2;

    /* Boosting map */
    testMgrCtx.boostingMap[0][0] = (U8**)Tm500DlMap;
    testMgrCtx.boostingUlMap = Tm500UlMap;

    pBsMacDesc->isMIBconfigured = FALSE;

    return SUCCESS;
}

RESULTCODE TestMgrSetRtConfig10Mhz2TxAntennas(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
    DCI_INFO    DciInfo;
    DCI_INFO    *pDciInfo = &DciInfo;
    U32                 index;

    printf("%s\n",__FUNCTION__);

    StaticConfig.tdd = 0;
    StaticConfig.bw_mode = BW_10_0_MHZ;
    StaticConfig.n_antenna_port = 1;
    initBS(&StaticConfig);

//--------------------------------------------------------------------------------------------------------------------------------------------------

    pTestCfg->TestModel = E_TM_3_1;
    pTestCfg->nAntennaPort = 1;
    pTestCfg->nCw = 1;
    pTestCfg->nLayers = 1;
    pTestCfg->precodingMode = 0;
    pTestCfg->duration = 10;
    pTestCfg->transmissionMode = SINGLEANT;

    pTestCfg->rsBoosting = 1;
    pTestCfg->epreSinchro = FLOAT2Q8(0);
    pTestCfg->reservedEpre = MINUS_INF_Q8;

    /* PBCH */
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = 0;
    pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
    pTestCfg->payloadPbch[0] = 0x60;
    pTestCfg->payloadPbch[1] = 0xC0;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;

    /* PCFICH */
    pTestCfg->nCtrlSymbols = 3; //1;
    pTestCfg->nPhichGroup =  0;//2; phich disable
    pTestCfg->nPhichPerGroup = 2;
    pTestCfg->phichSymPower = FLOAT2Q8(-3.010);
    pTestCfg->phichGroupEpre = FLOAT2Q8(0);

    /* PDCCH */
    pTestCfg->nAvalableRegs = 90;
    pTestCfg->nPdcch = 1;
    pTestCfg->nCcePerPdcch = 1;
    pTestCfg->nRegPerCce = 9;
    pTestCfg->nRegAllToPdcch = 90;
    pTestCfg->nNilRegPadding = 0;
    pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
    pTestCfg->nilRegEpre = MINUS_INF_Q8;

    /* DCI */
    pTestCfg->payloadPdcch[0][0] = 0x8E;
    pTestCfg->payloadPdcch[0][1] = 0x14;
    pTestCfg->payloadPdcch[0][2] = 0x14;
    pTestCfg->payloadPdcch[0][3] = 0x80;
    pTestCfg->PdcchDataLen[0] = 27;

    dci_decode_1a((U32*)&pTestCfg->payloadPdcch[0][0], (DCI_INFO *)pDciInfo, (BS_STATIC_CONFIG *)&StaticConfig);

    printf("pDciInfo->virtul_resrc_blk_type = %d;\n",      pDciInfo->virtul_resrc_blk_type);
    printf("pDciInfo->start_rb_index = %d;\n",             pDciInfo->start_rb_index);
    printf("pDciInfo->l_crb = %d;\n",                      pDciInfo->l_crb);
    printf("pDciInfo->tb_info[0].modulation_type = %d;\n", pDciInfo->tb_info[0].modulation_type);
    printf("pDciInfo->harq_process_id = %d;\n",            pDciInfo->harq_process_id);
    printf("pDciInfo->tb_info[0].ndi = %d;\n",             pDciInfo->tb_info[0].ndi);
    printf("pDciInfo->tb_info[0].redundancy_ver = %d;\n",  pDciInfo->tb_info[0].redundancy_ver);
    printf("pDciInfo->tpc = %d;\n",                        pDciInfo->tpc);

    dci_encode_1a((U32*)&pTestCfg->payloadPdcch[0][0], (DCI_INFO *)pDciInfo, (BS_STATIC_CONFIG *)&StaticConfig, &index);
    pTestCfg->PdcchDataLen[0] = index;

    printf("output pTestCfg->payloadPdcch[0][0] = %2.2x \npTestCfg->payloadPdcch[0][1] = %2.2x \npTestCfg->payloadPdcch[0][2] = %2.2x \npTestCfg->payloadPdcch[0][3] = %2.2x\n",
                pTestCfg->payloadPdcch[0][0], pTestCfg->payloadPdcch[0][1], pTestCfg->payloadPdcch[0][2], pTestCfg->payloadPdcch[0][3]);

    /* PDSCH */
    pTestCfg->nPdsch[0] = 1;
    pTestCfg->nPdschPrb[0][0] = 10;
    pTestCfg->pdschPrbPa[0][0] = FLOAT2Q8(0);
    pTestCfg->pdschMod[0][0] = QAM16;

    /* PUSCH */
    memset(pTestCfg->numPusch, 1, sizeof(pTestCfg->numPusch));
    pTestCfg->nPuschBoostedPrb = 45;
    pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);
    pTestCfg->puschBoostedMod = QAM16;//QPSK;

    pTestCfg->rntiPusch[0] = 100;//100;

    pTestCfg->rntiBoosted[0] = 100;//100;
    pTestCfg->rntiDeBoosted[0] = 100;//100;

    pTestCfg->rootSequenceIndex = 22;
    pTestCfg->prachConfigIndex = 12;
    pTestCfg->highSpeedFlag =  0;
    pTestCfg->zeroCorrelationZoneConfig = 1;
    pTestCfg->prachFreqOffset = 0;

    /* Boosting map */
    testMgrCtx.boostingMap[0][0] = (U8**)FdxMapIndex[BW_10_0_MHZ];
    testMgrCtx.boostingMap[0][1] = NULL; /* No deboosted data */

    testMgrCtx.boostingUlMap = FdxULMapIndex[BW_10_0_MHZ];

    return SUCCESS;
}

RESULTCODE TestHARQ(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
#define SNR_10
    DCI_INFO    DciInfo;
    DCI_INFO    *pDciInfo = &DciInfo;
    U32         index;

#ifdef SNR_10
    U8 rv[100] = {0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 2, 0, 0, 0, 0, 2,
                  0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 2, 0, 0, 0,
                  0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 2, 2, 0,
                  0, 0, 0, 0, 0, 0, 3, 0,   2, 0, 2, 0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0, 0, 2, 0,   0, 0, 0, 0, 0, 0, 0, 2,
                  2, 0, 0, 0, 0, 2, 0, 0,   0, 2, 0, 0, 0, 0, 0, 2,
                  0, 0, 0, 0};
#else
    U8 rv[100] = {0};
#endif

    StaticConfig.tdd = 0;
    StaticConfig.bw_mode = BW_10_0_MHZ;
    StaticConfig.n_antenna_port = 1;
    initBS(&StaticConfig);

//---------------------------------------------------------------------------------------------------------------------------------------------------

    printf("%s\n",__FUNCTION__);
    printf("DL 36 Mbps UL 19.08 Mbps\n");

    memcpy(pTestCfg->arRctRv, rv, sizeof(pTestCfg->arRctRv));
    pGetRvi = lte_pusch_rvi;
    lte_modulation_set_mcs_index_ul(QPSK, 6);

    pTestCfg->TestModel = E_TM_3_1;
    pTestCfg->nAntennaPort = 1;
    pTestCfg->nCw = 1;
    pTestCfg->nLayers = 1;
    pTestCfg->precodingMode = 0;
    pTestCfg->duration = 10;
    pTestCfg->transmissionMode = SINGLEANT;

    pTestCfg->rsBoosting = 1;
    pTestCfg->epreSinchro = FLOAT2Q8(0);
    pTestCfg->reservedEpre = MINUS_INF_Q8;

    /* PBCH */
    pTestCfg->pbchEna = 0;
    pTestCfg->pbchEpre = 0;
    pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
    pTestCfg->payloadPbch[0] = 0x60;
    pTestCfg->payloadPbch[1] = 0xC0;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;

    /* PCFICH */
    pTestCfg->nCtrlSymbols = 1; //1;
    pTestCfg->nPhichGroup =  0;//2; phich disable
    pTestCfg->nPhichPerGroup = 2;
    pTestCfg->phichSymPower = FLOAT2Q8(-3.010);
    pTestCfg->phichGroupEpre = FLOAT2Q8(0);

    /* PDCCH */
    pTestCfg->nAvalableRegs = 90;
    pTestCfg->nPdcch = 1;
    pTestCfg->nCcePerPdcch = 1;
    pTestCfg->nRegPerCce = 9;
    pTestCfg->nRegAllToPdcch = 90;
    pTestCfg->nNilRegPadding = 0;
    pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
    pTestCfg->nilRegEpre = MINUS_INF_Q8;

    /* DCI */
    /*pTestCfg->payloadPdcch[0][0] = 0x83;
    pTestCfg->payloadPdcch[0][1] = 0x1F;
    pTestCfg->payloadPdcch[0][2] = 0x14;
    pTestCfg->payloadPdcch[0][3] = 0x80;*/
    pTestCfg->PdcchDataLen[0] = 27;

    pDciInfo->virtul_resrc_blk_type = 0;
    pDciInfo->start_rb_index = 0;
    pDciInfo->l_crb = 50;
    pDciInfo->tb_info[0].modulation_type = 0x1c;
    pDciInfo->harq_process_id = 2;
    pDciInfo->tb_info[0].ndi = 1;
    pDciInfo->tb_info[0].redundancy_ver = 0;
    pDciInfo->tpc = 2;

    dci_encode_1a((U32*)&pTestCfg->payloadPdcch[0][0], (DCI_INFO *)pDciInfo, (BS_STATIC_CONFIG *)&StaticConfig, &index);
    pTestCfg->PdcchDataLen[0] = index;


    /* PDSCH */
    pTestCfg->nPdsch[0] = 1;
    pTestCfg->nPdschPrb[0][0] = 50;
    pTestCfg->pdschPrbPa[0][0] = FLOAT2Q8(0);
    pTestCfg->pdschMod[0][0] = QAM64;

    /* PUSCH */
    memset(pTestCfg->numPusch, 1, sizeof(pTestCfg->numPusch));
    pTestCfg->nPuschBoostedPrb = 50;
    pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);
    pTestCfg->puschBoostedMod = QPSK;
    pTestCfg->rntiPusch[0] = 0;
    pTestCfg->rntiBoosted[0] = 100;
    pTestCfg->rntiDeBoosted[0] = 100;

    /* Boosting map */
    testMgrCtx.boostingMap[0][0] = (U8**)FdxMapIndex[BW_10_0_MHZ];//E_TM_3_1MapIndex[BW_10_0_MHZ];
    testMgrCtx.boostingMap[0][1] = NULL; /* No deboosted data */

    testMgrCtx.boostingUlMap = FdxULTestMapIndex[BW_10_0_MHZ];

    return SUCCESS;
}

RESULTCODE Test10MhzHI_DCI0(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
    DCI_INFO    DciInfo;
    DCI_INFO    *pDciInfo = &DciInfo;
    U32         index;

    printf("%s\n",__FUNCTION__);
    printf("DL 36 Mbps UL 19.08 Mbps\n");

    StaticConfig.tdd = 0;
    StaticConfig.bw_mode = BW_10_0_MHZ;
    StaticConfig.n_antenna_port = 1;
    initBS(&StaticConfig);

//--------------------------------------------------------------------------------------------------------------------------------------------------

    pTestCfg->TestModel = E_TM_3_1;
    pTestCfg->nAntennaPort = 1;
    pTestCfg->nCw = 1;
    pTestCfg->nLayers = 1;
    pTestCfg->precodingMode = 0;
    pTestCfg->duration = 10;
    pTestCfg->transmissionMode = SINGLEANT;

    pTestCfg->rsBoosting = 1;
    pTestCfg->epreSinchro = FLOAT2Q8(0);
    pTestCfg->reservedEpre = MINUS_INF_Q8;

    /* PBCH */
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = 0;
    pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
    pTestCfg->payloadPbch[0] = 0x60;
    pTestCfg->payloadPbch[1] = 0xC0;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;

    /* PCFICH */
    pTestCfg->nCtrlSymbols = 1; //1;
    pTestCfg->nPhichGroup =  1;
    pTestCfg->nPhichPerGroup = 1;
    pTestCfg->phichSymPower = FLOAT2Q8(-3.010);
    pTestCfg->phichGroupEpre = FLOAT2Q8(0);

    /* PDCCH */
    pTestCfg->nAvalableRegs = 90;
    pTestCfg->nPdcch = 1;
    pTestCfg->nCcePerPdcch = 1;
    pTestCfg->nRegPerCce = 9;
    pTestCfg->nRegAllToPdcch = 90;
    pTestCfg->nNilRegPadding = 0;
    pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
    pTestCfg->nilRegEpre = MINUS_INF_Q8;


    /* DCI */
/*    pTestCfg->payloadPdcch[0][0] = 0x83;
    pTestCfg->payloadPdcch[0][1] = 0x1F;
    pTestCfg->payloadPdcch[0][2] = 0x14;
    pTestCfg->payloadPdcch[0][3] = 0x80;*/
    pTestCfg->PdcchDataLen[0] = 27;


    pDciInfo->virtul_resrc_blk_type = 0;
    pDciInfo->start_rb_index = 0;
    pDciInfo->l_crb = 50;
    pDciInfo->tb_info[0].modulation_type = 0x1c;
    pDciInfo->harq_process_id = 2;
    pDciInfo->tb_info[0].ndi = 1;
    pDciInfo->tb_info[0].redundancy_ver = 0;
    pDciInfo->tpc = 2;

    dci_encode_1a((U32*)&pTestCfg->payloadPdcch[0][0], (DCI_INFO *)pDciInfo, (BS_STATIC_CONFIG *)&StaticConfig, &index);
    pTestCfg->PdcchDataLen[0] = index;


    pTestCfg->nPdcchUl = 1;
    /* DCI Format-0 GRANT */
/*    pTestCfg->payloadPdcchUl[0] = 0x07;
    pTestCfg->payloadPdcchUl[1] = 0xDD;
    pTestCfg->payloadPdcchUl[2] = 0x28;
    pTestCfg->payloadPdcchUl[3] = 0x00;
    pTestCfg->PdcchDataLenUl[0] = 27;*/

//    pDciInfo++;    refill DCI

    pDciInfo->hopping = 0;
    pDciInfo->start_rb_index = 1;
    pDciInfo->l_crb = 6;
    pDciInfo->mcs_rv = 20;
    pDciInfo->ndi = 1;
    pDciInfo->tpc = 1;
    pDciInfo->cyclic_shift_dm_rs = 0;
    pDciInfo->cqi_request = 0;


    dci_encode_0((U32*)&pTestCfg->payloadPdcchUl[0], (DCI_INFO *)pDciInfo, (BS_STATIC_CONFIG *)&StaticConfig, &index);
    pTestCfg->PdcchDataLen[0] = index;

    /* PDSCH */

    pTestCfg->nPdsch[0] = 1;
    pTestCfg->nPdschPrb[0][0] = 50;
    pTestCfg->pdschPrbPa[0][0] = FLOAT2Q8(0);
    pTestCfg->pdschMod[0][0] = QAM64;

    /* PUSCH */
    memset(pTestCfg->numPusch, 1, sizeof(pTestCfg->numPusch));

    pTestCfg->puschMcs_used = 1;
    pTestCfg->puschMcs[0] = 20; // QAM16

    pTestCfg->nPuschBoostedPrb = 45;

    pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);

    pTestCfg->rntiPusch[0] = 100;//100

    pTestCfg->rntiBoosted[0] = 100;//100;
    pTestCfg->rntiDeBoosted[0] = 100;//100;

    /* Boosting map */
    testMgrCtx.boostingMap[0][0] = (U8**)E_TM_3_1MapIndex[BW_10_0_MHZ];//testMapIndex[BW_10_0_MHZ];

    testMgrCtx.boostingUlMap = FdxULTestMapIndex[BW_10_0_MHZ] ;//FdxULMapIndex[BW_10_0_MHZ];

    return SUCCESS;
}

RESULTCODE TestMgrSet_SpaMux_20Mhz_2ANT_2Layers_2Cw_RT39(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
    DCI_INFO    DciInfo;
    DCI_INFO    *pDciInfo = &DciInfo;
    U32         index;

    printf("%s\n",__FUNCTION__);

    StaticConfig.tdd = 0;
    StaticConfig.bw_mode = BW_20_0_MHZ;
    StaticConfig.n_antenna_port = 2;
    initBS(&StaticConfig);

//---------------------------------------------------------------------------------------------------------------------------------------------------

    lte_modulation_set_mcs_index_dl(QAM64, 0, 23);
    lte_modulation_set_mcs_index_dl(QAM64, 1, 23);
    lte_modulation_set_mcs_index_ul(QAM64, 28);

    pTestCfg->TestModel = BE_TEST_2;
    pTestCfg->nAntennaPort = 2;
    pTestCfg->nCw = 2;
    pTestCfg->nLayers = 2;
    pTestCfg->precodingMode = 0;
    pTestCfg->duration = 10;
    pTestCfg->transmissionMode = OPNLPSPMUX;

    pTestCfg->rsBoosting = 1;
    pTestCfg->epreSinchro = FLOAT2Q8(0);
    pTestCfg->reservedEpre = MINUS_INF_Q8;

    /* PBCH */
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = 0;
    pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
    pTestCfg->payloadPbch[0] = 0xA0;
    pTestCfg->payloadPbch[1] = 0x00;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;

    /* PCFICH */
    pTestCfg->nCtrlSymbols = 1;
    pTestCfg->nPhichGroup =  0;//2; phich disable
    pTestCfg->nPhichPerGroup = 2;
    pTestCfg->phichSymPower = FLOAT2Q8(-3.010);
    pTestCfg->phichGroupEpre = FLOAT2Q8(0);

    /* PDCCH */
    pTestCfg->nAvalableRegs = 90;
    pTestCfg->nPdcch = 1;
    pTestCfg->nCcePerPdcch = 4;
    pTestCfg->nRegPerCce = 9;
    pTestCfg->nRegAllToPdcch = 90;
    pTestCfg->nNilRegPadding = 0;
    pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
    pTestCfg->nilRegEpre = MINUS_INF_Q8;

    /* DCI */
    pTestCfg->DciFormat = DCI_FORMAT_HARDCODED_PAYLOAD;
    //     7F FF  FF  d2  E4 80 40

/*    pTestCfg->payloadPdcch[0][0] = 0x7f;
    pTestCfg->payloadPdcch[0][1] = 0xff;
    pTestCfg->payloadPdcch[0][2] = 0xff;
    pTestCfg->payloadPdcch[0][3] = 0xc2;
    pTestCfg->payloadPdcch[0][4] = 0xe4;
    pTestCfg->payloadPdcch[0][5] = 0x84;
    pTestCfg->payloadPdcch[0][6] = 0x40;*/
    pTestCfg->PdcchDataLen[0] = 51;

    pDciInfo->dci_format = DCI_FORMAT_2;
    pDciInfo->rah = 0;
    pDciInfo->virtul_resrc_blk_type = 0;
    pDciInfo->rbg = 0x1ffffff;
    pDciInfo->tpc = 0;
    pDciInfo->harq_process_id = 1;
    pDciInfo->swap_flag = 0;
    pDciInfo->tb_info[0].modulation_type = 28;
    pDciInfo->tb_info[0].ndi = 1;
    pDciInfo->tb_info[0].redundancy_ver = 0;
    pDciInfo->tb_info[1].modulation_type = 16;
    pDciInfo->tb_info[1].ndi = 1;
    pDciInfo->tb_info[1].redundancy_ver = 0;
    pDciInfo->precoding_info = 2;


    dci_encode_2((U32*)&pTestCfg->payloadPdcch[0][0], (DCI_INFO *)pDciInfo, (BS_STATIC_CONFIG *)&StaticConfig, &index);
    pTestCfg->PdcchDataLen[0] = index;


    /* PDSCH */
    pTestCfg->nPdsch[0] = 1;
    pTestCfg->nPdsch[1] = 1;

    pTestCfg->nPdschPrb[0][0] = 100;
    pTestCfg->pdschPrbPa[0][0] = FLOAT2Q8(0);
    pTestCfg->pdschMod[0][0] = QAM64;
    pTestCfg->pdschPayloadMode[0][0] = 0;

    pTestCfg->nPdschPrb[1][0] = 100;
    pTestCfg->pdschPrbPa[1][0] = FLOAT2Q8(0);
    pTestCfg->pdschMod[1][0] = QAM64;
    pTestCfg->pdschPayloadMode[1][0] = 1;

    /* PUSCH */
    memset(pTestCfg->numPusch, 1, sizeof(pTestCfg->numPusch));
    pTestCfg->nPuschBoostedPrb = 72;
    pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);
    pTestCfg->puschBoostedMod = QAM64;

    pTestCfg->puschDed.nDMRS2 = 5;

    pTestCfg->rntiPusch[0] = 10;

    pTestCfg->rntiBoosted[0] = 100;//100;
    pTestCfg->rntiDeBoosted[0] = 100;//100;

    /* Boosting map */
    testMgrCtx.boostingMap[0][0] = (U8**)E_TM_3_1MapIndex[BW_20_0_MHZ];

    testMgrCtx.boostingUlMap = FdxULMapIndex[BW_20_0_MHZ];

    /* Init param */
    bs_static_config_update_20mhz(&pBsMacDesc->initParm);
    pBsMacDesc->initParm.txAntennaPortCount = TXANT_TWO;
    pBsMacDesc->initParm.rxAntennaPortCount = RXANT_TWO;

    return SUCCESS;
}

extern U32 global_chanIdx;

RESULTCODE TestMgrValidateTestConfig(BS_CONFOR_TEST_CONFIG *pTestConfig)
{
    U8 bw = pBsMacDesc->initParm.channelBandwidth;

        if (
            bw != BW_1_4_MHZ &&
            bw != BW_3_0_MHZ &&
            bw != BW_5_0_MHZ &&
            bw != BW_10_0_MHZ &&
            bw != BW_20_0_MHZ)
        {
            printf("Error in function %s (%s:%d)\n",
                      __FUNCTION__, __FILE__, __LINE__);
            return FAILURE;
        }

    return SUCCESS;
}

RESULTCODE RctMacAllocationULMap(U16 sfn, U8* map, U32 len)
{
    U8* BoostMapUl = NULL;
    U32 i = (sfn < testConfConfig.duration)? sfn : 0;

    if(testMgrCtx.boostingUlMap == NULL)
        return FAILURE;
    if(len <= GetMapSize(pBsMacDesc->initParm.channelBandwidth))
    {
        for(; i < testConfConfig.duration; i++)
        {
            BoostMapUl = (U8*)testMgrCtx.boostingUlMap[i];
            memcpy(BoostMapUl, map, len);
            BoostMapUl[len] = 0xFF;
            if(sfn < testConfConfig.duration)
                break;
        }
    }
    else
        return FAILURE;
    return SUCCESS;
}

/* Function parses incoming message with test configuration */
RESULTCODE TestMgrReconfigure(BS_CONFOR_TEST_CONFIG *pTestConfig)
{
    RESULTCODE ret = SUCCESS;
    U8 idx;
    U8 bw = pBsMacDesc->initParm.channelBandwidth;
    PRINT_FUNCTION_NAME(__FUNCTION__);

    if (TestMgrValidateTestConfig(pTestConfig) != SUCCESS)
        return FAILURE;

    if(pBsMacDesc->isMIBconfigured == FALSE)
    {
        memcpy(pTestConfig->payloadPbch, testConfConfig.payloadPbch,
            sizeof(pTestConfig->payloadPbch));
    }
    memcpy(pTestConfig->timAdvErrInfo, testConfConfig.timAdvErrInfo, sizeof(pTestConfig->timAdvErrInfo));

    memcpy(&testConfConfig, pTestConfig, sizeof(BS_CONFOR_TEST_CONFIG));
    
#if 0
#ifdef MX_ARM_ENABLED //ETM Lib
    uart_printf("testConfConfig.TestModel = %d, bw = %d\n", (U32)testConfConfig.TestModel, bw);
    etm_cfg.model = (E_TM_INT_MODEL)testConfConfig.TestModel;
    etm_cfg.bw = bw;
    etm_cfg.ant0 = 1;
    etm_cfg.ant1 = 0;
    etm_cfg.num_layers = 1;
    etm_cfg.num_cw = 1;
    etm_cfg.e_rs = 0;
    etm_cfg.tdd = 0;
    etm_init(etm_cfg);
#endif
#endif

    
#ifndef AC_MSPD_TDD
    switch (testConfConfig.TestModel)
    {
        case E_TM_1_1:
            testMgrCtx.boostingMap[0][0] = (U8**)E_TM_1_1MapIndex[bw];
            testMgrCtx.boostingMap[0][1] = NULL; /* No deboosted data */
            testConfConfig.nPdsch[0] = 1;
            break;
        case E_TM_1_2:
            testMgrCtx.boostingMap[0][0] = (U8**)E_TM_1_2MapIndex[bw];
            testMgrCtx.boostingMap[0][1] = (U8**)E_TM_1_2MapIndex[bw];
            testConfConfig.nPdsch[0] = 2;
            break;
        case E_TM_2:
            testMgrCtx.boostingMap[0][0] = (U8**)E_TM_2MapIndex[bw];
            testMgrCtx.boostingMap[0][1] = NULL; /* No deboosted data */
            testConfConfig.nPdsch[0] = 1;
            break;
        case E_TM_3_1:
            testMgrCtx.boostingMap[0][0] = (U8**)E_TM_3_1MapIndex[bw];
            testMgrCtx.boostingMap[0][1] = NULL; /* No deboosted data */
            testConfConfig.nPdsch[0] = 1;
            break;
        case E_TM_3_2:
            testMgrCtx.boostingMap[0][0] = (U8**)E_TM_3_2MapIndex[bw];
            testMgrCtx.boostingMap[0][1] = (U8**)E_TM_3_2MapIndex[bw];
            testConfConfig.nPdsch[0] = 2;
            break;
        case E_TM_3_3:
            testMgrCtx.boostingMap[0][0] = (U8**)E_TM_3_3MapIndex[bw];
            testMgrCtx.boostingMap[0][1] = (U8**)E_TM_3_3MapIndex[bw];
            testConfConfig.nPdsch[0] = 2;
            break;
        case SWEEP_TEST_1:
            testMgrCtx.boostingMap[0][0] = (U8**)testConfConfig.SwpTst_MapIdx_1;
            testMgrCtx.boostingMap[0][1] = (U8**)testConfConfig.SwpTst_MapIdx_2;
            testConfConfig.nPdsch[0] = 2;
            pTestMgrGetBoostedPdsch   = lte_pdsch_set_mapinfo_boosted_ex;
            pTestMgrGetDeBoostedPdsch = lte_pdsch_set_mapinfo_boosted_ex;
            break;
        default:
            return FAILURE;
    }
#else
    switch (testConfConfig.TestModel)
    {
        case E_TM_1_1:
            testMgrCtx.boostingMap[0][0] = (U8**)E_TM_x_1MapTDD[bw];
            testMgrCtx.boostingMap[0][1] = NULL; /* No deboosted data */
            testConfConfig.nPdsch[0] = 1;
            break;
        case E_TM_1_2:
            testMgrCtx.boostingMap[0][0] = (U8**)E_TM_1_2MapTDD[bw];
            testMgrCtx.boostingMap[0][1] = (U8**)E_TM_1_2MapTDD_UE2[bw];
            testConfConfig.nPdsch[0] = 2;
            break;
        case E_TM_2:
            testMgrCtx.boostingMap[0][0] = (U8**)E_TM_2MapTDD[bw];
            testMgrCtx.boostingMap[0][1] = NULL;
            testConfConfig.nPdsch[0] = 1;
            break;
        case E_TM_3_1:
            testMgrCtx.boostingMap[0][0] = (U8**)E_TM_x_1MapTDD[bw];
            testMgrCtx.boostingMap[0][1] = NULL; /* No deboosted data */
            testConfConfig.nPdsch[0] = 1;
            break;
        case E_TM_3_2:
            testMgrCtx.boostingMap[0][0] = (U8**)E_TM_3_2MapTDD[bw];
            testMgrCtx.boostingMap[0][1] = (U8**)E_TM_3_2MapTDD_UE2[bw];
            testConfConfig.nPdsch[0] = 2;
            break;
        case E_TM_3_3:
            testMgrCtx.boostingMap[0][0] = (U8**)E_TM_3_3MapTDD[bw];
            testMgrCtx.boostingMap[0][1] = (U8**)E_TM_3_3MapTDD_UE2[bw];
            testConfConfig.nPdsch[0] = 2;
            break;
        default:
            return FAILURE;
    }
#endif /*AC_MSPD_TDD*/

        testMgrCtx.boostingUlMap = RctULMapIndex[bw];
        testConfConfig.pbchEna = 1;

// TODO:
#ifndef MX_ARM_ENABLED
    /* Set conformance test scheduler */
    pBsMacDesc->schedulingSchema = RCT_SCHEMA;
    if(testConfConfig.transmissionMode == 0)
        testConfConfig.transmissionMode = SINGLEANT; // set default if no configured
    memcpy(&pBsMacDesc->RxNumBytes, testConfConfig.RxNumBytes,
        sizeof(pBsMacDesc->RxNumBytes));
    pBsMacDesc->DirMode = (U8)testConfConfig.DirMode;
#endif

    idx = 0;
    while((ret = lte_modulation_set_mcs_index_dl((U16)(testConfConfig.RctMcsDl[idx]>>16), 0,
                             (U8)(testConfConfig.RctMcsDl[idx]&0xFFFF)))== SUCCESS)
    {
        lte_modulation_set_mcs_index_dl((U16)(testConfConfig.RctMcsDl[idx]>>16), 1, (U8)(testConfConfig.RctMcsDl[idx]&0xFFFF));
        idx++;
    }

    idx = 0;
    while((ret =lte_modulation_set_mcs_index_ul((U16)(testConfConfig.RctMcsUl[idx]>>16),
                        (U8)(testConfConfig.RctMcsUl[idx]&0xFFFF)))== SUCCESS)
           idx++;

    pGetRvi = lte_pusch_rvi;

// TODO:
#ifdef MX_ARM_ENABLED

    testConfConfig.curRctRv = 0;
    testConfConfig.nRctRv = sizeof(testConfConfig.arRctRv);

    /* Set conformance test scheduler */
    pBsMacDesc->schedulingSchema = RCT_SCHEMA;
    if(testConfConfig.transmissionMode == 0)
        testConfConfig.transmissionMode = SINGLEANT;

    ret = LteBsMacUlSchedulerReconfig(pBsMacDesc);

    testConfConfig.ULEnabled = 0;
/*    if(testConfConfig.prachConfigIndex == 0xFF)
    {
        for(idx = 0; idx<MIN(sizeof(testConfConfig.numPucch), sizeof(testConfConfig.numPusch)); idx++)
            testConfConfig.ULEnabled += testConfConfig.numPucch[idx] + testConfConfig.numPusch[idx];
        if(!testConfConfig.ULEnabled)
             uart_printf(">>>>>>>>>>>>>>>>>>Downlink enabled; Uplink disabled %d\n",testConfConfig.ULEnabled);
    }
    else
        testConfConfig.ULEnabled = 1;*/
    testConfConfig.DontUsePuschRxSduForHiAndDci0 = 1;

#else

    ret = LteBsMacDlSchedulerReconfig(pBsMacDesc);
    ret = LteBsMacUlSchedulerReconfig(pBsMacDesc);

    testConfConfig.ULEnabled = 0;

#endif

    return ret;
}

RESULTCODE TestMgrSet_Diversity_20Mhz_2ANT_2Layers(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
    DCI_INFO    DciInfo;
    DCI_INFO    *pDciInfo = &DciInfo;
    U32                 index;

    printf("%s\n",__FUNCTION__);

    StaticConfig.tdd = 0;
    StaticConfig.bw_mode = BW_20_0_MHZ;
    StaticConfig.n_antenna_port = 2;
    initBS(&StaticConfig);
//---------------------------------------------------------------------------------------------------------------------------------------------------

    pTestCfg->TestModel = BE_TEST_2;
    pTestCfg->nAntennaPort = 2;
    pTestCfg->nCw = 1;
    pTestCfg->nLayers = 2;
    pTestCfg->precodingMode = 0;
    pTestCfg->duration = 10;
    pTestCfg->transmissionMode = TXDIVERSITY;//OPNLPSPMUX;

    pTestCfg->rsBoosting = 1;
    pTestCfg->epreSinchro = FLOAT2Q8(0);
    pTestCfg->reservedEpre = MINUS_INF_Q8;

    /* PBCH */
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = 0;
    pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
    pTestCfg->payloadPbch[0] = 0xA0;
    pTestCfg->payloadPbch[1] = 0x00;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;

    /* PCFICH */
    pTestCfg->nCtrlSymbols = 1;
    pTestCfg->nPhichGroup =  0;//2; phich disable
    pTestCfg->nPhichPerGroup = 2;
    pTestCfg->phichSymPower = FLOAT2Q8(-3.010);
    pTestCfg->phichGroupEpre = FLOAT2Q8(0);

    /* PDCCH */
    pTestCfg->nAvalableRegs = 90;
    pTestCfg->nPdcch = 1;
    pTestCfg->nCcePerPdcch = 4;
    pTestCfg->nRegPerCce = 9;
    pTestCfg->nRegAllToPdcch = 90;
    pTestCfg->nNilRegPadding = 0;
    pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
    pTestCfg->nilRegEpre = MINUS_INF_Q8;

    /* DCI */
    pTestCfg->DciFormat = DCI_FORMAT_HARDCODED_PAYLOAD;

    //     7F FF  FF  d2  E4 80 40
/*    pTestCfg->payloadPdcch[0][0] = 0x7f;
    pTestCfg->payloadPdcch[0][1] = 0xff;
    pTestCfg->payloadPdcch[0][2] = 0xff;
    pTestCfg->payloadPdcch[0][3] = 0xc0;
    pTestCfg->payloadPdcch[0][4] = 0xe4;
    pTestCfg->payloadPdcch[0][5] = 0x01;
    pTestCfg->payloadPdcch[0][6] = 0x00;*/
    pTestCfg->PdcchDataLen[0] = 51;

    pDciInfo->dci_format = DCI_FORMAT_2;
    pDciInfo->rah = 0;
    pDciInfo->virtul_resrc_blk_type = 0;
    pDciInfo->rbg = 0x1ffffff;
    pDciInfo->tpc = 0;
    pDciInfo->harq_process_id = 0;
    pDciInfo->swap_flag = 0;
    pDciInfo->tb_info[0].modulation_type = 0x1c;
    pDciInfo->tb_info[0].ndi = 1;
    pDciInfo->tb_info[0].redundancy_ver = 0;
    pDciInfo->tb_info[1].modulation_type = 0x0;
    pDciInfo->tb_info[1].ndi = 0;
    pDciInfo->tb_info[1].redundancy_ver = 1;
    pDciInfo->precoding_info = 0;


    dci_encode_2((U32*)&pTestCfg->payloadPdcch[0][0], (DCI_INFO *)pDciInfo, (BS_STATIC_CONFIG *)&StaticConfig, &index);
    pTestCfg->PdcchDataLen[0] = index;


    /* PDSCH */
    pTestCfg->nPdsch[0] = 1;

    pTestCfg->nPdschPrb[0][0] = 100;
    pTestCfg->pdschPrbPa[0][0] = FLOAT2Q8(0);
    pTestCfg->pdschMod[0][0] = QAM64;

    /* PUSCH */
    memset(pTestCfg->numPusch, 1, sizeof(pTestCfg->numPusch));
    pTestCfg->nPuschBoostedPrb = 45;
    pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);
    pTestCfg->puschBoostedMod = QAM64;//QPSK;

    pTestCfg->rntiPusch[0] = 100;//100;

    pTestCfg->rntiBoosted[0] = 100;//100;
    pTestCfg->rntiDeBoosted[0] = 100;//100;

    /* Boosting map */
    testMgrCtx.boostingMap[0][0] = (U8**)E_TM_3_1MapIndex[BW_20_0_MHZ];

    testMgrCtx.boostingUlMap = FdxULMapIndex[BW_20_0_MHZ];
    bs_static_config_update_20mhz(&pBsMacDesc->initParm);
    pBsMacDesc->initParm.txAntennaPortCount = TXANT_TWO;

    return SUCCESS;
}

RESULTCODE TestMgrSet_BCH_20Mhz_2ANT(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
    printf("%s\n",__FUNCTION__);

    pTestCfg->TestModel = BE_TEST_2;
    pTestCfg->nAntennaPort = 2;
    pTestCfg->nCw = 1;
    pTestCfg->nLayers = 2;
    pTestCfg->precodingMode = 0;
    pTestCfg->duration = 10;
    pTestCfg->transmissionMode = TXDIVERSITY;

    pTestCfg->rsBoosting = 1;
    pTestCfg->epreSinchro = FLOAT2Q8(0);
    pTestCfg->reservedEpre = MINUS_INF_Q8;

    /* PBCH */
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = 0;
    pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
    pTestCfg->payloadPbch[0] = 0xA0;
    pTestCfg->payloadPbch[1] = 0x00;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;

    /* PCFICH */
    pTestCfg->nCtrlSymbols = 1;
    pTestCfg->nPhichGroup =  0;//2; phich disable
    pTestCfg->nPhichPerGroup = 2;
    pTestCfg->phichSymPower = FLOAT2Q8(-3.010);
    pTestCfg->phichGroupEpre = FLOAT2Q8(0);

    /* PDCCH */
    pTestCfg->nPdcch = 0;

    /* PDSCH */
    pTestCfg->nPdschPrb[0][0] = 0;

    /* PUSCH */
    memset(pTestCfg->numPusch, 1, sizeof(pTestCfg->numPusch));
    pTestCfg->nPuschBoostedPrb = 45;
    pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);
    pTestCfg->puschBoostedMod = QAM64;//QPSK;

    pTestCfg->rntiPusch[0] = 100;//100;

    pTestCfg->rntiBoosted[0] = 100;//100;
    pTestCfg->rntiDeBoosted[0] = 100;//100;

    /* Boosting map */
    testMgrCtx.boostingMap[0][0] = NULL;

    testMgrCtx.boostingUlMap = FdxULMapIndex[BW_20_0_MHZ];
    bs_static_config_update_20mhz(&pBsMacDesc->initParm);
    pBsMacDesc->initParm.txAntennaPortCount = TXANT_TWO;

    return SUCCESS;
}

RESULTCODE TestMgrSetRtConfigBCH_10MHz_2ANT(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
    DCI_INFO    DciInfo;
    DCI_INFO    *pDciInfo = &DciInfo;
    U32                 index;

    printf("%s\n",__FUNCTION__);

    StaticConfig.bw_mode = BW_10_0_MHZ;
    StaticConfig.tdd = 0;
    StaticConfig.n_antenna_port = 2;

    initBS(&StaticConfig);
//---------------------------------------------------------------------------------------------------------------------------------------------------


    pBsMacDesc->initParm.txAntennaPortCount = TXANT_TWO;

    pTestCfg->TestModel = BE_TEST_1;
    pTestCfg->nAntennaPort = 2;
    pTestCfg->nCw = 2;
    pTestCfg->nLayers = 2;
    pTestCfg->precodingMode = 0;
    pTestCfg->duration = 10;
    pTestCfg->transmissionMode = OPNLPSPMUX;

    pTestCfg->rsBoosting = 1;
    pTestCfg->epreSinchro = FLOAT2Q8(0);
    pTestCfg->reservedEpre = MINUS_INF_Q8;

    /* PBCH */
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = 0;
    pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
    pTestCfg->payloadPbch[0] = 0xB5;
    pTestCfg->payloadPbch[1] = 0x60;
    pTestCfg->payloadPbch[2] = 0xB3;
    pTestCfg->payloadPbch[3] = 0x00;

    /* PCFICH */
    pTestCfg->nCtrlSymbols = 1; //1;
    pTestCfg->nPhichGroup =  0;//2; phich disable
    pTestCfg->nPhichPerGroup = 2;
    pTestCfg->phichSymPower = FLOAT2Q8(-3.010);
    pTestCfg->phichGroupEpre = FLOAT2Q8(0);

    /* PDCCH */
    pTestCfg->nAvalableRegs = 90;
    pTestCfg->nPdcch = 0;
    pTestCfg->nCcePerPdcch = 1;
    pTestCfg->nRegPerCce = 9;
    pTestCfg->nRegAllToPdcch = 90;
    pTestCfg->nNilRegPadding = 0;
    pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
    pTestCfg->nilRegEpre = MINUS_INF_Q8;

    /* DCI */
//     7F FF D2 E4 80 00
/*    pTestCfg->payloadPdcch[0][0] = 0x0;
    pTestCfg->payloadPdcch[0][1] = 0x0;
    pTestCfg->payloadPdcch[0][2] = 0x0;
    pTestCfg->payloadPdcch[0][3] = 0xe4;
    pTestCfg->payloadPdcch[0][4] = 0x84;
    pTestCfg->payloadPdcch[0][5] = 0x40;*/
    pTestCfg->PdcchDataLen[0] = 43;

    pDciInfo->rah = 0;
    pDciInfo->virtul_resrc_blk_type = 0;
    pDciInfo->rbg = 0x0;
    pDciInfo->tpc = 0;
    pDciInfo->harq_process_id = 0;
    pDciInfo->swap_flag = 0;
    pDciInfo->tb_info[0].modulation_type = 0x1c;
    pDciInfo->tb_info[0].ndi = 1;
    pDciInfo->tb_info[0].redundancy_ver = 0;
    pDciInfo->tb_info[1].modulation_type = 0x10;
    pDciInfo->tb_info[1].ndi = 1;
    pDciInfo->tb_info[1].redundancy_ver = 0;
    pDciInfo->precoding_info = 2;


    dci_encode_2((U32*)&pTestCfg->payloadPdcch[0][0], (DCI_INFO *)pDciInfo, (BS_STATIC_CONFIG *)&StaticConfig, &index);
    pTestCfg->PdcchDataLen[0] = index;


    /* PDSCH */

    pTestCfg->nPdsch[0] = 0;
    pTestCfg->nPdsch[1] = 0;

    pTestCfg->nPdschPrb[0][0] = 0;
    pTestCfg->pdschPrbPa[0][0] = FLOAT2Q8(0);
    pTestCfg->pdschMod[0][0] = QAM16;

    pTestCfg->nPdschPrb[1][0] = 0;
    pTestCfg->pdschPrbPa[1][0] = FLOAT2Q8(0);
    pTestCfg->pdschMod[1][0] = QAM64;


    /* PUSCH */
    memset(pTestCfg->numPusch, 1, sizeof(pTestCfg->numPusch));
    pTestCfg->nPuschBoostedPrb = 45;
    pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);
    pTestCfg->puschBoostedMod = QAM16;//QPSK;

    pTestCfg->rntiPusch[0] = 100;//100;

    pTestCfg->rntiBoosted[0] = 100;//100;
    pTestCfg->rntiDeBoosted[0] = 100;//100;

    pTestCfg->rootSequenceIndex = 22;
    pTestCfg->prachConfigIndex = 0;
    pTestCfg->highSpeedFlag =  0;
    pTestCfg->zeroCorrelationZoneConfig = 1;
    pTestCfg->prachFreqOffset = 0;

    /* Boosting map */
    testMgrCtx.boostingMap[0][0] = (U8**)E_TM_3_1MapIndex[BW_10_0_MHZ];
    testMgrCtx.boostingMap[1][0] = (U8**)E_TM_3_1MapIndex[BW_10_0_MHZ];


    testMgrCtx.boostingUlMap = FdxULMapIndex[BW_10_0_MHZ];
    return SUCCESS;
}

RESULTCODE TestMgrSet_BCH_PDCCH_20Mhz_2ANT(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
    DCI_INFO    DciInfo;
    DCI_INFO    *pDciInfo = &DciInfo;
    U32                 index;

    printf("%s\n",__FUNCTION__);

    StaticConfig.tdd = 0;
    StaticConfig.bw_mode = BW_20_0_MHZ;
    StaticConfig.n_antenna_port = 2;
    initBS(&StaticConfig);
//---------------------------------------------------------------------------------------------------------------------------------------------------


    pTestCfg->TestModel = BE_TEST_2;
    pTestCfg->nAntennaPort = 2;
    pTestCfg->nCw = 1;
    pTestCfg->nLayers = 2;
    pTestCfg->precodingMode = 0;
    pTestCfg->duration = 10;
    pTestCfg->transmissionMode = TXDIVERSITY;//OPNLPSPMUX;

    pTestCfg->rsBoosting = 1;
    pTestCfg->epreSinchro = FLOAT2Q8(0);
    pTestCfg->reservedEpre = MINUS_INF_Q8;

    /* PBCH */
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = 0;
    pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
    pTestCfg->payloadPbch[0] = 0xA0;
    pTestCfg->payloadPbch[1] = 0x00;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;

    /* PCFICH */
    pTestCfg->nCtrlSymbols = 1;
    pTestCfg->nPhichGroup =  0;//2; phich disable
    pTestCfg->nPhichPerGroup = 2;
    pTestCfg->phichSymPower = FLOAT2Q8(-3.010);
    pTestCfg->phichGroupEpre = FLOAT2Q8(0);

    /* PDCCH */
    pTestCfg->nAvalableRegs = 90;
    pTestCfg->nPdcch = 1;
    pTestCfg->nCcePerPdcch = 1;
    pTestCfg->nRegPerCce = 9;
    pTestCfg->nRegAllToPdcch = 90;
    pTestCfg->nNilRegPadding = 0;
    pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
    pTestCfg->nilRegEpre = MINUS_INF_Q8;

    /* DCI */

/*    pTestCfg->payloadPdcch[0][0] = 0x7f;
    pTestCfg->payloadPdcch[0][1] = 0xff;
    pTestCfg->payloadPdcch[0][2] = 0xff;
    pTestCfg->payloadPdcch[0][3] = 0xc0;
    pTestCfg->payloadPdcch[0][4] = 0xe4;
    pTestCfg->payloadPdcch[0][5] = 0x01;
    pTestCfg->payloadPdcch[0][6] = 0x00;*/
    pTestCfg->PdcchDataLen[0] = 48;


    pDciInfo->rah = 0;
    pDciInfo->virtul_resrc_blk_type = 0;
    pDciInfo->rbg = 0x1ffffff;
    pDciInfo->tpc = 0;
    pDciInfo->harq_process_id = 0;
    pDciInfo->scrambl_indentity = 0;
    pDciInfo->tb_info[0].modulation_type = 0x1c;
    pDciInfo->tb_info[0].ndi = 1;
    pDciInfo->tb_info[0].redundancy_ver = 0;
    pDciInfo->tb_info[1].modulation_type = 0x0;
    pDciInfo->tb_info[1].ndi = 0;
    pDciInfo->tb_info[1].redundancy_ver = 1;


    dci_encode_2b((U32*)&pTestCfg->payloadPdcch[0][0], (DCI_INFO *)pDciInfo, (BS_STATIC_CONFIG *)&StaticConfig, &index);
    pTestCfg->PdcchDataLen[0] = index;


    /* PDSCH */
    pTestCfg->nPdsch[0] = 1;
    pTestCfg->nPdschPrb[0][0] = 0;
    pTestCfg->pdschPrbPa[0][0] = FLOAT2Q8(0);
    pTestCfg->pdschMod[0][0] = QAM64;

    /* PUSCH */
    memset(pTestCfg->numPusch, 1, sizeof(pTestCfg->numPusch));
    pTestCfg->nPuschBoostedPrb = 45;
    pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);
    pTestCfg->puschBoostedMod = QAM64;//QPSK;

    pTestCfg->rntiPusch[0] = 100;//100;

    pTestCfg->rntiBoosted[0] = 100;//100;
    pTestCfg->rntiDeBoosted[0] = 100;//100;

    /* Boosting map */
    testMgrCtx.boostingMap[0][0] = (U8**)E_TM_3_1MapIndex[BW_20_0_MHZ];

    testMgrCtx.boostingUlMap = FdxULMapIndex[BW_20_0_MHZ];
    bs_static_config_update_20mhz(&pBsMacDesc->initParm);
    pBsMacDesc->initParm.txAntennaPortCount = TXANT_TWO;

    return SUCCESS;
}

RESULTCODE TestMgrSet_SpaMux_20Mhz_2ANT_2Layers_2Cw(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
    DCI_INFO    DciInfo;
    DCI_INFO    *pDciInfo = &DciInfo;
    U32         index;

    printf("%s\n",__FUNCTION__);

    StaticConfig.tdd = 0;
    StaticConfig.bw_mode = BW_20_0_MHZ;
    StaticConfig.n_antenna_port = 2;
    initBS(&StaticConfig);
//---------------------------------------------------------------------------------------------------------------------------------------------------

    pTestCfg->TestModel = BE_TEST_2;
    pTestCfg->nAntennaPort = 2;
    pTestCfg->nCw = 2;
    pTestCfg->nLayers = 2;
    pTestCfg->precodingMode = 0;
    pTestCfg->duration = 10;
    pTestCfg->transmissionMode = OPNLPSPMUX;

    pTestCfg->rsBoosting = 1;
    pTestCfg->epreSinchro = FLOAT2Q8(0);
    pTestCfg->reservedEpre = MINUS_INF_Q8;

    /* PBCH */
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = 0;
    pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
    pTestCfg->payloadPbch[0] = 0xA0;
    pTestCfg->payloadPbch[1] = 0x00;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;

    /* PCFICH */
    pTestCfg->nCtrlSymbols = 1;
    pTestCfg->nPhichGroup =  0;//2; phich disable
    pTestCfg->nPhichPerGroup = 2;
    pTestCfg->phichSymPower = FLOAT2Q8(-3.010);
    pTestCfg->phichGroupEpre = FLOAT2Q8(0);

    /* PDCCH */
    pTestCfg->nAvalableRegs = 90;
    pTestCfg->nPdcch = 1;
    pTestCfg->nCcePerPdcch = 4;
    pTestCfg->nRegPerCce = 9;
    pTestCfg->nRegAllToPdcch = 90;
    pTestCfg->nNilRegPadding = 0;
    pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
    pTestCfg->nilRegEpre = MINUS_INF_Q8;

    /* DCI */
    pTestCfg->DciFormat = DCI_FORMAT_HARDCODED_PAYLOAD;
    //     7F FF  FF  d2  E4 80 40

/*    pTestCfg->payloadPdcch[0][0] = 0x7f;
    pTestCfg->payloadPdcch[0][1] = 0xff;
    pTestCfg->payloadPdcch[0][2] = 0xff;
    pTestCfg->payloadPdcch[0][3] = 0xc2;
    pTestCfg->payloadPdcch[0][4] = 0xe4;
    pTestCfg->payloadPdcch[0][5] = 0x84;
    pTestCfg->payloadPdcch[0][6] = 0x40;
    pTestCfg->PdcchDataLen[0] = 51;*/

    pDciInfo->dci_format = DCI_FORMAT_2;
    pDciInfo->rah = 0;
    pDciInfo->virtul_resrc_blk_type = 0;
    pDciInfo->rbg = 0x1ffffff;
    pDciInfo->tpc = 0;
    pDciInfo->harq_process_id = 1;
    pDciInfo->swap_flag = 0;
    pDciInfo->tb_info[0].modulation_type = 28;
    pDciInfo->tb_info[0].ndi = 1;
    pDciInfo->tb_info[0].redundancy_ver = 0;
    pDciInfo->tb_info[1].modulation_type = 16;
    pDciInfo->tb_info[1].ndi = 1;
    pDciInfo->tb_info[1].redundancy_ver = 0;
    pDciInfo->precoding_info = 2;

    dci_encode_2((U32*)&pTestCfg->payloadPdcch[0][0], (DCI_INFO *)pDciInfo, (BS_STATIC_CONFIG *)&StaticConfig, &index);
    pTestCfg->PdcchDataLen[0] = index;

    /* PDSCH */
    pTestCfg->nPdsch[0] = 1;
    pTestCfg->nPdsch[1] = 1;

    pTestCfg->nPdschPrb[0][0] = 100;
    pTestCfg->pdschPrbPa[0][0] = FLOAT2Q8(0);
    pTestCfg->pdschMod[0][0] = QAM64;

    pTestCfg->nPdschPrb[1][0] = 60;
    pTestCfg->pdschPrbPa[1][0] = FLOAT2Q8(0);
    pTestCfg->pdschMod[1][0] = QAM16;

    /* PUSCH */
    memset(pTestCfg->numPusch, 1, sizeof(pTestCfg->numPusch));
    pTestCfg->nPuschBoostedPrb = 45;
    pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);
    pTestCfg->puschBoostedMod = QAM64;//QPSK;

    pTestCfg->rntiPusch[0] = 100;//100;

    pTestCfg->rntiBoosted[0] = 100;//100;
    pTestCfg->rntiDeBoosted[0] = 100;//100;

    /* Boosting map */
    testMgrCtx.boostingMap[0][0] = (U8**)E_TM_3_1MapIndex[BW_20_0_MHZ];

    testMgrCtx.boostingUlMap = FdxULMapIndex[BW_20_0_MHZ];
    bs_static_config_update_20mhz(&pBsMacDesc->initParm);
    pBsMacDesc->initParm.txAntennaPortCount = TXANT_TWO;

    return SUCCESS;
}

RESULTCODE TestMgrSet_SpaMux_20Mhz_2ANT_2Layers_2Cw_RT43(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
    DCI_INFO    DciInfo;
    DCI_INFO    *pDciInfo = &DciInfo;
    U32                 index;

    printf("%s\n",__FUNCTION__);

    StaticConfig.tdd = 0;
    StaticConfig.bw_mode = BW_20_0_MHZ;
    StaticConfig.n_antenna_port = 2;
    initBS(&StaticConfig);
//---------------------------------------------------------------------------------------------------------------------------------------------------

    lte_modulation_set_mcs_index_dl(QAM64, 0, 23);
    lte_modulation_set_mcs_index_dl(QAM64, 1, 23);
    lte_modulation_set_mcs_index_ul(QAM64, 28);

    pTestCfg->TestModel = BE_TEST_2;
    pTestCfg->nAntennaPort = 2;
    pTestCfg->nCw = 2;
    pTestCfg->nLayers = 2;
    pTestCfg->precodingMode = 0;
    pTestCfg->duration = 10;
    pTestCfg->transmissionMode = OPNLPSPMUX;

    pTestCfg->rsBoosting = 1;
    pTestCfg->epreSinchro = FLOAT2Q8(0);
    pTestCfg->reservedEpre = MINUS_INF_Q8;

    /* PBCH */
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = 0;
    pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
    pTestCfg->payloadPbch[0] = 0xA0;
    pTestCfg->payloadPbch[1] = 0x00;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;

    /* PCFICH */
    pTestCfg->nCtrlSymbols = 1;
    pTestCfg->nPhichGroup =  0;//2; phich disable
    pTestCfg->nPhichPerGroup = 2;
    pTestCfg->phichSymPower = FLOAT2Q8(-3.010);
    pTestCfg->phichGroupEpre = FLOAT2Q8(0);

    /* PDCCH */
    pTestCfg->nAvalableRegs = 90;
    pTestCfg->nPdcch = 1;
    pTestCfg->nCcePerPdcch = 4;
    pTestCfg->nRegPerCce = 9;
    pTestCfg->nRegAllToPdcch = 90;
    pTestCfg->nNilRegPadding = 0;
    pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
    pTestCfg->nilRegEpre = MINUS_INF_Q8;

    /* DCI */
    pTestCfg->DciFormat = DCI_FORMAT_HARDCODED_PAYLOAD;
    //     7F FF  FF  d2  E4 80 40

/*    pTestCfg->payloadPdcch[0][0] = 0x7f;
    pTestCfg->payloadPdcch[0][1] = 0xff;
    pTestCfg->payloadPdcch[0][2] = 0xff;
    pTestCfg->payloadPdcch[0][3] = 0xc2;
    pTestCfg->payloadPdcch[0][4] = 0xe4;
    pTestCfg->payloadPdcch[0][5] = 0x84;
    pTestCfg->payloadPdcch[0][6] = 0x40;*/
    pTestCfg->PdcchDataLen[0] = 51;

    pDciInfo->dci_format = DCI_FORMAT_2;
    pDciInfo->rah = 0;
    pDciInfo->virtul_resrc_blk_type = 0;
    pDciInfo->rbg = 0x1ffffff;
    pDciInfo->tpc = 0;
    pDciInfo->harq_process_id = 1;
    pDciInfo->swap_flag = 0;
    pDciInfo->tb_info[0].modulation_type = 28;
    pDciInfo->tb_info[0].ndi = 1;
    pDciInfo->tb_info[0].redundancy_ver = 0;
    pDciInfo->tb_info[1].modulation_type = 16;
    pDciInfo->tb_info[1].ndi = 1;
    pDciInfo->tb_info[1].redundancy_ver = 0;
    pDciInfo->precoding_info = 2;


    dci_encode_2((U32*)&pTestCfg->payloadPdcch[0][0], (DCI_INFO *)pDciInfo, (BS_STATIC_CONFIG *)&StaticConfig, &index);
    pTestCfg->PdcchDataLen[0] = index;


    /* PDSCH */
    pTestCfg->nPdsch[0] = 1;
    pTestCfg->nPdsch[1] = 1;

    pTestCfg->nPdschPrb[0][0] = 100;
    pTestCfg->pdschPrbPa[0][0] = FLOAT2Q8(0);
    pTestCfg->pdschMod[0][0] = QAM64;
    pTestCfg->pdschPayloadMode[0][0] = 0;

    pTestCfg->nPdschPrb[1][0] = 100;
    pTestCfg->pdschPrbPa[1][0] = FLOAT2Q8(0);
    pTestCfg->pdschMod[1][0] = QAM64;
    pTestCfg->pdschPayloadMode[1][0] = 1;

    /* PUSCH */
    memset(pTestCfg->numPusch, 1, sizeof(pTestCfg->numPusch));
    pTestCfg->nPuschBoostedPrb = 50;
    pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);
    pTestCfg->puschBoostedMod = QAM64;

    pTestCfg->puschDed.nDMRS2 = 5;

    pTestCfg->rntiPusch[0] = 10;

    pTestCfg->rntiBoosted[0] = 100;//100;
    pTestCfg->rntiDeBoosted[0] = 100;//100;

    /* Boosting map */
    testMgrCtx.boostingMap[0][0] = (U8**)E_TM_3_1MapIndex[BW_20_0_MHZ];

    testMgrCtx.boostingUlMap = FdxULMapIndex[BW_20_0_MHZ];

    /* Init param */
    bs_static_config_update_20mhz(&pBsMacDesc->initParm);
    pBsMacDesc->initParm.txAntennaPortCount = TXANT_TWO;
    pBsMacDesc->initParm.rxAntennaPortCount = RXANT_TWO;

    return SUCCESS;
}

RESULTCODE TestMgrSet_10MHz_xDL_xUL_HARQ_RT45(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
    DCI_INFO    DciInfo;
    DCI_INFO    *pDciInfo = &DciInfo;
    U32                 index;

    printf("%s\n",__FUNCTION__);

    StaticConfig.tdd = 0;
    StaticConfig.bw_mode = BW_10_0_MHZ;
    StaticConfig.n_antenna_port = 2;
    initBS(&StaticConfig);
//---------------------------------------------------------------------------------------------------------------------------------------------------

    pTestCfg->nAntennaPort = 1;
    pTestCfg->nCw = 1;
    pTestCfg->nLayers = 1;
    pTestCfg->duration = 10;
    pTestCfg->transmissionMode = SINGLEANT;

    pTestCfg->epreSinchro = FLOAT2Q8(0);

    // --------------------------------
    // DL channels
    // --------------------------------

    /* PDSCH */
    pTestCfg->nPdsch[0] = 8;
    // PDSCH MCS
    pTestCfg->pdschMcs_used[0] = 1;
    pTestCfg->pdschMcs[0][0] = 8;
    pTestCfg->pdschMcs[0][1] = 13;
    pTestCfg->pdschMcs[0][2] = 25;
    pTestCfg->pdschMcs[0][3] = 3;
    pTestCfg->pdschMcs[0][4] = 16;
    pTestCfg->pdschMcs[0][5] = 28;
    pTestCfg->pdschMcs[0][6] = 2;
    pTestCfg->pdschMcs[0][7] = 11;
    // PDSCH RNTI (scrInitValueInput)
    pTestCfg->pdschRnti_used = 1;
    pTestCfg->pdschRnti[0] = 64;
    pTestCfg->pdschRnti[1] = 65;
    pTestCfg->pdschRnti[2] = 66;
    pTestCfg->pdschRnti[3] = 67;
    pTestCfg->pdschRnti[4] = 68;
    pTestCfg->pdschRnti[5] = 69;
    pTestCfg->pdschRnti[6] = 70;
    pTestCfg->pdschRnti[7] = 71;
    // PDSCH DL MAP
    pTestCfg->pdsch_map[0].startRes = 0;
    pTestCfg->pdsch_map[0].numRes = 3;
    pTestCfg->pdsch_map[1].startRes = 5;
    pTestCfg->pdsch_map[1].numRes = 4;
    pTestCfg->pdsch_map[2].startRes = 10;
    pTestCfg->pdsch_map[2].numRes = 5;
    pTestCfg->pdsch_map[3].startRes = 15;
    pTestCfg->pdsch_map[3].numRes = 3;
    pTestCfg->pdsch_map[4].startRes = 20;
    pTestCfg->pdsch_map[4].numRes = 4;
    pTestCfg->pdsch_map[5].startRes = 25;
    pTestCfg->pdsch_map[5].numRes = 5;
    pTestCfg->pdsch_map[6].startRes = 32;
    pTestCfg->pdsch_map[6].numRes = 3;
    pTestCfg->pdsch_map[7].startRes = 40;
    pTestCfg->pdsch_map[7].numRes = 4;
    pTestMgrGetBoostedPdsch = lte_pdsch_set_mapinfo_boosted_direct;
    pTestMgrGetDeBoostedPdsch = lte_pdsch_set_mapinfo_boosted_direct;
    /* Boosting map */
    // not used in this case, shouldn't be zero
    memset(testMgrCtx.boostingMap[0], 0xFF, sizeof(testMgrCtx.boostingMap[0]));
    // PDSCH Power
    pTestCfg->pdschPrbPa[0][0] = FLOAT2Q8(0);
    pTestCfg->pdschPrbPa[0][1] = FLOAT2Q8(0);
    pTestCfg->pdschPrbPa[0][2] = FLOAT2Q8(0);
    pTestCfg->pdschPrbPa[0][3] = FLOAT2Q8(0);
    pTestCfg->pdschPrbPa[0][4] = FLOAT2Q8(0);
    pTestCfg->pdschPrbPa[0][5] = FLOAT2Q8(0);
    pTestCfg->pdschPrbPa[0][6] = FLOAT2Q8(0);
    pTestCfg->pdschPrbPa[0][7] = FLOAT2Q8(0);
    // PDSCH Payload Mode
    pTestCfg->pdschPayloadMode[0][0] = 1;
    pTestCfg->pdschPayloadMode[0][1] = 0;
    pTestCfg->pdschPayloadMode[0][2] = 1;
    pTestCfg->pdschPayloadMode[0][3] = 0;
    pTestCfg->pdschPayloadMode[0][4] = 1;
    pTestCfg->pdschPayloadMode[0][5] = 0;
    pTestCfg->pdschPayloadMode[0][6] = 1;
    pTestCfg->pdschPayloadMode[0][7] = 0;


    /* PBCH */
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = FLOAT2Q8(0);
    pTestCfg->pbchRv = 3;
    pBsMacDesc->initParm.phyLayerCellIdGroup = 0xFF;
    pTestCfg->payloadPbch[0] = 0xA0;
    pTestCfg->payloadPbch[1] = 0x00;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;

    /* PCFICH */

    /* PHICH */
    pTestCfg->nCtrlSymbols = 1;
    pTestCfg->nPhichGroup =  3;
    pTestCfg->nPhichPerGroup = 2;
    pTestCfg->phichSymPower = FLOAT2Q8(-3.010);
    // only new PHICH API supported (use "createphy 2") !!!
    pTestCfg->DontUsePuschRxSduForHiAndDci0 = 1; // don't use PUSCH RxSdu queue for HiSdu
    pTestCfg->ULEnabled = 0;    // create HiSdu permanently

    /* PDCCH */
    pTestCfg->nPdcchUl = 0;
    pTestCfg->nPdcch = 8;
    pTestCfg->nCcePerPdcch = 2;
    pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
    pTestCfg->rntiBoosted[0] = 20;
    pTestCfg->rntiBoosted[1] = 20;
    pTestCfg->rntiBoosted[2] = 20;
    pTestCfg->rntiBoosted[3] = 20;
    pTestCfg->rntiBoosted[4] = 20;
    pTestCfg->rntiBoosted[5] = 20;
    pTestCfg->rntiBoosted[6] = 20;
    pTestCfg->rntiBoosted[7] = 20;
    // PDCCH DL MAP
    pTestCfg->pdcch_map[0][0].startRes = 0;
    pTestCfg->pdcch_map[0][0].numRes = 2;
    pTestCfg->pdcch_map[1][0].startRes = 2;
    pTestCfg->pdcch_map[1][0].numRes = 2;
    pTestCfg->pdcch_map[2][0].startRes = 0;
    pTestCfg->pdcch_map[2][0].numRes = 2;
    pTestCfg->pdcch_map[3][0].startRes = 0;
    pTestCfg->pdcch_map[3][0].numRes = 2;
    pTestCfg->pdcch_map[4][0].startRes = 0;
    pTestCfg->pdcch_map[4][0].numRes = 2;
    pTestCfg->pdcch_map[5][0].startRes = 0;
    pTestCfg->pdcch_map[5][0].numRes = 2;
    pTestCfg->pdcch_map[6][0].startRes = 0;
    pTestCfg->pdcch_map[6][0].numRes = 2;
    pTestCfg->pdcch_map[7][0].startRes = 0;
    pTestCfg->pdcch_map[7][0].numRes = 2;
    memset(pTestCfg->pdcch_map_unified, 1, sizeof(pTestCfg->pdcch_map_unified));
    pTestMgrGetPdcch = lte_pdcch_get_mapinfo_direct;

    /* DCI */
    pTestCfg->DciFormat = DCI_FORMAT_HARDCODED_PAYLOAD; // 31 bit
    //     AC 4D  15  DB
/*    pTestCfg->payloadPdcch[0][0] = 0xAC;
    pTestCfg->payloadPdcch[0][1] = 0x4D;
    pTestCfg->payloadPdcch[0][2] = 0x15;
    pTestCfg->payloadPdcch[0][3] = 0xDB;*/
    pTestCfg->PdcchDataLen[0] = 31;
    pTestCfg->payloadPdcch_unified = 1;

    pDciInfo->dci_format = DCI_FORMAT_1;
    pDciInfo->rah = 1;
    pDciInfo->rbg = 45364;
    pDciInfo->tb_info[0].modulation_type = 10;
    pDciInfo->harq_process_id = 7;
    pDciInfo->tb_info[0].ndi = 0;
    pDciInfo->tb_info[0].redundancy_ver = 3;
    pDciInfo->tpc = 1;

    dci_encode_1((U32 * )&pTestCfg->payloadPdcch[0][0], (DCI_INFO *) pDciInfo, (BS_STATIC_CONFIG *) &StaticConfig, &index);
    pTestCfg->PdcchDataLen[0] = index;


    // Just to shut up GetMcsIndexDl()  - used to create payload for formats 0 and 2 by TestMAC
    pTestCfg->pdschMod[0][0] = QPSK;
    // --------------------------------
    // UL channels
    // --------------------------------

    // PUSCH
    memset(pTestCfg->numPusch, 4, sizeof(pTestCfg->numPusch));
    // PUSCH MCS
    pTestCfg->ulSfrCtrl.enable64QAM = 1;
    pTestCfg->puschMcs_used = 1;
    pTestCfg->puschMcs[0] = 23;
    pTestCfg->puschMcs[1] = 12;
    pTestCfg->puschMcs[2] = 4;
    pTestCfg->puschMcs[3] = 15;
    // PUSCH RNTI (scrInitValueInput)
    pTestCfg->rntiPusch[0] = 20;
    pTestCfg->rntiPusch[1] = 21;
    pTestCfg->rntiPusch[2] = 21;
    pTestCfg->rntiPusch[3] = 23;
    // PUSCH UL MAP
    pTestCfg->pusch_map[0].startRes = 6;
    pTestCfg->pusch_map[0].numRes = 6;
    pTestCfg->pusch_map[1].startRes = 12;
    pTestCfg->pusch_map[1].numRes = 4;
    pTestCfg->pusch_map[2].startRes = 20;
    pTestCfg->pusch_map[2].numRes = 3;
    pTestCfg->pusch_map[3].startRes = 24;
    pTestCfg->pusch_map[3].numRes = 15;
    pTestMgrGetPusch = lte_pusch_set_mapinfo_direct;
    // PUSCH nDMRS2
    pTestCfg->pusch_nDMRS2_used = 1;
    pTestCfg->pusch_nDMRS2[0] = 0;
    pTestCfg->pusch_nDMRS2[1] = 5;
    pTestCfg->pusch_nDMRS2[2] = 5;
    pTestCfg->pusch_nDMRS2[3] = 5;

    pTestCfg->nPuschBoostedPrb = 50;
    pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);

    // PUCCH
    memset(pTestCfg->numPucch, 4, sizeof(pTestCfg->numPucch));
    // PUCCH RNTI (scrInitValueInput)
    pTestCfg->rntiPucch_used = 1;
    pTestCfg->rntiPucch[0] = 24;
    pTestCfg->rntiPucch[1] = 25;
    pTestCfg->rntiPucch[2] = 26;
    pTestCfg->rntiPucch[3] = 62;
    // PUCCH UL MAP
    pTestCfg->pucch_map[0].startRes = 0;
    pTestCfg->pucch_map[0].numRes = 1;
    pTestCfg->pucch_map[1].startRes = 49;
    pTestCfg->pucch_map[1].numRes = 1;
    pTestCfg->pucch_map[2].startRes = 1;
    pTestCfg->pucch_map[2].numRes = 1;
    pTestCfg->pucch_map[3].startRes = 48;
    pTestCfg->pucch_map[3].numRes = 1;
    pTestMgrGetPucch = lte_pucch_set_mapinfo_direct;
    // PUCCH format
    pTestCfg->pucchFormat_used = 1;
    pTestCfg->pucchFormat[0] = FORMAT2;
    pTestCfg->pucchFormat[1] = FORMAT1A;
    pTestCfg->pucchFormat[2] = FORMAT1A;
    pTestCfg->pucchFormat[3] = FORMAT1A;
    // PUCCH dlCqiPmiSizeBits
    pTestCfg->pucch_dlCqiPmiSizeBits_used = 1;
    pTestCfg->pucch_dlCqiPmiSizeBits[0] = 11;
    pTestCfg->pucch_dlCqiPmiSizeBits[1] = 0;
    pTestCfg->pucch_dlCqiPmiSizeBits[2] = 0;
    pTestCfg->pucch_dlCqiPmiSizeBits[3] = 0;
    // PUCCH harqPucchIndex
    pTestCfg->pucch_harqPucchIndex_used = 1;
    pTestCfg->pucch_harqPucchIndex[0] = 0;
    pTestCfg->pucch_harqPucchIndex[1] = 11;
    pTestCfg->pucch_harqPucchIndex[2] = 11;
    pTestCfg->pucch_harqPucchIndex[3] = 11;

    pTestCfg->ulSfrCtrl.deltaPUCCHShift = 2;
    pTestCfg->ulSfrCtrl.nRBCQI = 1;
    pTestCfg->ulSfrCtrl.n1PucchAN = 11;


    // PRACH
    pTestCfg->prachEnableAllSfs = 1;
    pTestCfg->prachConfigIndex = 12;
    pTestCfg->rootSequenceIndex = 22;
    pTestCfg->zeroCorrelationZoneConfig = 1;
    pTestCfg->prachFreqOffset = 40;


    /* Init param */
    pBsMacDesc->initParm.txAntennaPortCount = TXANT_ONE;
    pBsMacDesc->initParm.rxAntennaPortCount = RXANT_ONE;

    return SUCCESS;
}


RESULTCODE TestMgrSetTm500CtrlConfig(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
    DCI_INFO    DciInfo;
    DCI_INFO    *pDciInfo = &DciInfo;
    U32                 index;

    printf("%s\n",__FUNCTION__);


    StaticConfig.tdd = 0;
    StaticConfig.bw_mode = BW_10_0_MHZ;
    StaticConfig.n_antenna_port = 1;
    initBS(&StaticConfig);

//--------------------------------------------------------------------------------------------------------------------------------------------------

    printf("Aeroflex TM500 HARQ mode (PUCCH)\n");

    memset(pTestCfg, 0, sizeof(BS_CONFOR_TEST_CONFIG));

    lte_modulation_set_mcs_index_ul(QAM16, 12);

    pTestCfg->TestModel = 0;
    pTestCfg->nAntennaPort = 1;
    pTestCfg->nCw = 1;
    pTestCfg->nLayers = 1;
    pTestCfg->precodingMode = 0;
    pTestCfg->duration = 10;
    pTestCfg->transmissionMode = SINGLEANT;

    pTestCfg->rsBoosting = 1;
    pTestCfg->epreSinchro = FLOAT2Q8(0);
    pTestCfg->reservedEpre = MINUS_INF_Q8;

    /* PBCH */
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = 0;
    pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
    pTestCfg->payloadPbch[0] = 0x60;
    pTestCfg->payloadPbch[1] = 0x00;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;

    /* PCFICH */
    pTestCfg->nCtrlSymbols = 1;
    pTestCfg->nPhichGroup =  0;
    pTestCfg->nPhichPerGroup = 2;
    pTestCfg->phichSymPower = FLOAT2Q8(-3.010);
    pTestCfg->phichGroupEpre = FLOAT2Q8(0);

    /* PDCCH */
    pTestCfg->nAvalableRegs = 90;
    pTestCfg->nPdcch = 2;
    pTestCfg->nCcePerPdcch = 2;
    pTestCfg->nRegPerCce = 9;
    pTestCfg->nRegAllToPdcch = 90;
    pTestCfg->nNilRegPadding = 0;
    pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
    pTestCfg->nilRegEpre = MINUS_INF_Q8;

    /* DCI */
/*    pTestCfg->payloadPdcch[0][0] = 0x8E;
    pTestCfg->payloadPdcch[0][1] = 0x14;
    pTestCfg->payloadPdcch[0][2] = 0x14;
    pTestCfg->payloadPdcch[0][3] = 0x80;*/
    pTestCfg->PdcchDataLen[0] = 27;

    pDciInfo->virtul_resrc_blk_type = 0;
    pDciInfo->start_rb_index = 0;
    pDciInfo->l_crb = 10;
    pDciInfo->tb_info[0].modulation_type = 16;
    pDciInfo->harq_process_id = 2;
    pDciInfo->tb_info[0].ndi = 1;
    pDciInfo->tb_info[0].redundancy_ver = 0;
    pDciInfo->tpc = 2;

    dci_encode_1a((U32*)&pTestCfg->payloadPdcch[0][0], (DCI_INFO *)pDciInfo, (BS_STATIC_CONFIG *)&StaticConfig, &index);
    pTestCfg->PdcchDataLen[0] = index;

/*    pTestCfg->payloadPdcch[1][0] = 0x1D;
    pTestCfg->payloadPdcch[1][1] = 0xDB;
    pTestCfg->payloadPdcch[1][2] = 0x20;
    pTestCfg->payloadPdcch[1][3] = 0x00;*/
    pTestCfg->PdcchDataLen[1] = 27;

    pDciInfo->hopping = 0;
    pDciInfo->start_rb_index = 5;
    pDciInfo->l_crb = 20;
    pDciInfo->mcs_rv = 12;
    pDciInfo->ndi = 1;
    pDciInfo->tpc = 0;
    pDciInfo->cyclic_shift_dm_rs = 0;
    pDciInfo->cqi_request = 0;

//    pDciInfo++; refill DCi

    dci_encode_0((U32*)&pTestCfg->payloadPdcch[1][0], (DCI_INFO *)pDciInfo, (BS_STATIC_CONFIG *)&StaticConfig, &index);
    pTestCfg->PdcchDataLen[1] = index;

    /* PDSCH */
    pTestCfg->nPdsch[0] = 1;

    pTestCfg->nPdschPrb[0][0] = 10;
    pTestCfg->pdschPrbPa[0][0] = FLOAT2Q8(0);
    pTestCfg->pdschMod[0][0] = QAM16;

    /* PUSCH */
    pTestCfg->nPuschBoostedPrb = 20;

    pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);
    pTestCfg->puschBoostedMod = QAM16;

    /* PRACH */
    pTestCfg->rootSequenceIndex = 22;
    pTestCfg->prachConfigIndex = 12;
    pTestCfg->highSpeedFlag =  0;
    pTestCfg->zeroCorrelationZoneConfig = 1;
    pTestCfg->prachFreqOffset = 0;

    pTestCfg->rntiPusch[0] = 61;

    pTestCfg->rntiBoosted[0] = 61;
    pTestCfg->rntiDeBoosted[0] = 61;

    pTestCfg->ulSfrCtrl.deltaPUCCHShift = 2;
    pTestCfg->ulSfrCtrl.nRBCQI = 2;
    pTestCfg->ulSfrCtrl.n1PucchAN = 2;
    pTestCfg->ulSfrCtrl.nSB = 1;

    pTestCfg->betaOffsetCQIIndex = 2;

    /* Boosting map */
    testMgrCtx.boostingMap[0][0] = (U8**)Tm500DlMap;
    testMgrCtx.boostingUlMap = Tm500UlMap;

    pBsMacDesc->isMIBconfigured = TRUE;

    return SUCCESS;
}

RESULTCODE TestMgrSet_xUEs_xAnt_xLayers_xBw_MaxThrough
    (BS_CONFOR_TEST_CONFIG *pTestCfg, U8 ues, U8 numLayers, U8 numAnt, enum ChBwOptions bw, U8 phich)
{
    U32 rbs = 0;
    U8 rbs_per_ue = 0;
    U8 extra_rbs = 0;
    U8 uec = 0;
    U8 layer = 0;
    U32 pdsch_res_off = 0;
    U32 pdcch_res_off = 0;
    U32 pusch_res_off = 0;
    U32 pusch_rbs_per_ue = 0;
    U32 pddch_index = 0;
    DCI_DATA_INFO dci = {0};
    DCI_INFO    DciInfo;
    DCI_INFO *pDciInfo = &DciInfo;
    U8 *dci_payload;
    U32 idx;
    U32 index;

    printf("%s\n",__FUNCTION__);

    pTestCfg->TestModel = E_TM_3_1;

    if ( bw != BW_10_0_MHZ && bw != BW_20_0_MHZ )
    {
        return MAC_FAILURE;
    }
    rbs = GetMapSize(bw);

    rbs_per_ue = rbs / ues;
    extra_rbs = rbs - rbs_per_ue * ues;

    pTestCfg->nAntennaPort = numAnt;
    pTestCfg->nCw = numLayers;
    pTestCfg->nLayers = numLayers;

    if ( numAnt == 1 )
    {
        pBsMacDesc->initParm.txAntennaPortCount = TXANT_ONE;
        pBsMacDesc->initParm.rxAntennaPortCount = RXANT_ONE;
        pTestCfg->transmissionMode = SINGLEANT;
    }
    else if ( numAnt == 2 )
    {
        pBsMacDesc->initParm.txAntennaPortCount = TXANT_TWO;
        pBsMacDesc->initParm.rxAntennaPortCount = RXANT_ONE;
        pTestCfg->transmissionMode = OPNLPSPMUX;
    }
    else
    {
        return MAC_FAILURE;
    }

    pTestCfg->precodingMode = 0;
    pTestCfg->duration = 10;

    pTestCfg->epreSinchro = FLOAT2Q8(0);
    pTestCfg->reservedEpre = MINUS_INF_Q8;

    // PBCH  (to be overwritten anyway?)
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = 0;
    pTestCfg->pbchRv = 0;
    pBsMacDesc->initParm.phyLayerCellIdGroup = 0;
    pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
    pTestCfg->payloadPbch[0] = 0x60; // TODO: correct only for 10 MHz
    pTestCfg->payloadPbch[1] = 0xC0;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;

    // PCFICH
    pTestCfg->nCtrlSymbols = 1;

    // PHICH
    if ( phich )
    {
        pTestCfg->nPhichGroup = 1;
        pTestCfg->nPhichPerGroup = ues;
        pTestCfg->phichSymPower = FLOAT2Q8(-3.010);
        // only new PHICH API supported (use "createphy 2") !!!
        pTestCfg->DontUsePuschRxSduForHiAndDci0 = 1; // don't use PUSCH RxSdu queue for HiSdu
        pTestCfg->ULEnabled = 0;    // create HiSdu permanently
    }
    else
        pTestCfg->nPhichGroup = 0;


    // PDSCH basic init
    pTestCfg->nPdsch[0] = ues;
    if ( numLayers == 2 )
        pTestCfg->nPdsch[1] = ues;

    //pTestCfg->nPdschPrb[0][0] = 50;
    //pTestCfg->pdschPrbPa[0][0] = FLOAT2Q8(0);
    pTestCfg->pdschMcs_used[0] = 1;
    if ( pTestCfg->nCw == 2 )
    {
        pTestCfg->pdschMcs_used[1] = 1;
    }
    pTestCfg->pdschRnti_used = 1;
    pTestMgrGetBoostedPdsch = lte_pdsch_set_mapinfo_boosted_direct;
    pTestMgrGetDeBoostedPdsch = lte_pdsch_set_mapinfo_boosted_direct;
    memset(testMgrCtx.boostingMap[0], 0xFF, sizeof(testMgrCtx.boostingMap[0]));

    // PDCCH basic init
    pTestCfg->nPdcchUl = ues;
    pTestCfg->nPdcch = ues * 2;
    pTestCfg->nAvalableRegs = 90;
    pTestCfg->nCcePerPdcch = numAnt; // ?!
    pTestCfg->nRegPerCce = 9;
    pTestCfg->nRegAllToPdcch = 90;
    pTestCfg->nNilRegPadding = 0;
    pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
    pTestMgrGetPdcch = lte_pdcch_get_mapinfo_direct;
    memset(pTestCfg->pdcch_map_unified, 1, sizeof(pTestCfg->pdcch_map_unified));

    // PRACH
    pTestCfg->rootSequenceIndex = 22;
    pTestCfg->prachConfigIndex = 0;
    pTestCfg->highSpeedFlag =  0;
    pTestCfg->zeroCorrelationZoneConfig = 1;
    pTestCfg->prachFreqOffset = 0;

/*
    // No PUCCH requested

    // PUCCH
    memset(pTestCfg->numPucch, ues, sizeof(pTestCfg->numPucch));
    pTestMgrGetPucch = TestMgrGetPucchDirect;
    // PUCCH RNTI (scrInitValueInput)
    pTestCfg->rntiPucch_used = 1;
    // PUCCH format
    pTestCfg->pucchFormat_used = 1;
    // PUCCH dlCqiPmiSizeBits
    pTestCfg->pucch_dlCqiPmiSizeBits_used = 1;
    // PUCCH harqPucchIndex
    pTestCfg->pucch_harqPucchIndex_used = 1;
    pTestCfg->ulSfrCtrl.deltaPUCCHShift = 2;
    pTestCfg->ulSfrCtrl.nRBCQI = 1;
    pTestCfg->ulSfrCtrl.n1PucchAN = 11;
*/
    // PUSCH
    memset(pTestCfg->numPusch, ues, sizeof(pTestCfg->numPusch));
    // PUSCH MCS
    pTestCfg->ulSfrCtrl.enable64QAM = 1;
    pTestCfg->puschMcs_used = 1;
    pTestMgrGetPusch = lte_pusch_set_mapinfo_direct;
    // PUSCH nDMRS2
    pTestCfg->pusch_nDMRS2_used = 0;
    //pTestCfg->nPuschBoostedPrb = 50;
    //pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);

    pTestCfg->DciFormat = DCI_FORMAT_HARDCODED_PAYLOAD; // NULL callback in order to use hardcoded dci
    pTestCfg->payloadPdcch_unified = 1;

    pdsch_res_off = 0;
    pdcch_res_off = 0;
    pusch_res_off = 4; // reserve some space for control once needed

    pusch_rbs_per_ue = (rbs - (pusch_res_off * 2))/ues;

    dci.MaxRb = rbs;

    StaticConfig.bw_mode = GetBwSize(rbs);
    StaticConfig.n_antenna_port = numAnt;
    StaticConfig.tdd = 0;

    initBS(&StaticConfig);

    dci.harqProcessId = 0;
    pDciInfo->virtul_resrc_blk_type = 0;
    pDciInfo->rah = 0;
    pDciInfo->rbg = 0x1C000;
    pDciInfo->swap_flag = 0;
    pDciInfo->harq_process_id = 0;


    dci.numAntennaPort = numAnt;
    dci.resrcAllocType = 0;


    // Different PrepareDCI funcs use different fields for "modulation and coding scheme"
    dci.tbInfo[0].modulationType = 28; // 64QAM
    pDciInfo->mcs_rv = 28; // 64QAM

    dci.tbInfo[0].mcs = 28; // 64QAM
    pDciInfo->tb_info[0].modulation_type = 28; // 64QAM
    pDciInfo->tb_info[1].modulation_type = 28; // 64QAM


    dci.tbInfo[0].redundancyVer = 0;
    pDciInfo->tb_info[0].redundancy_ver = 1;
    pDciInfo->tb_info[1].redundancy_ver = 1;


    dci.tpc = 0;
    pDciInfo->tpc = 0;

    pDciInfo->tb_info[0].ndi = 1;
    pDciInfo->tb_info[1].ndi = 1;
    pDciInfo->precoding_info = 1;


    if(bw == BW_20_0_MHZ)
    {
        dci.numOfRBs = 100;
        dci.RGBSize = 4;
    }
    else if(bw == BW_10_0_MHZ)
    {
        dci.numOfRBs = 50;
        dci.RGBSize = 3;
    }

    for ( uec = 0; uec != ues; ++uec )
    {
        for ( layer = 0; layer < numLayers; ++layer )
        {
            // PDSCH num MCS (well, "cw" is index here, not layer)
            pTestCfg->pdschMcs[layer][uec] = 28;

            // PDSCH Payload Mode
            pTestCfg->pdschPayloadMode[layer][uec] = uec & 1; // try different payloads

            // PDSCH Power
            pTestCfg->pdschPrbPa[layer][uec] = FLOAT2Q8(0);

            // PDSCH Just to shut up GetMcsIndexDl()  - used to create payload for formats 0 and 2 by TestMAC
            pTestCfg->pdschMod[layer][uec] = QAM64;
        }

        // PDSCH RNTI
        pTestCfg->pdschRnti[uec] = 64 + uec;
        // PDSCH DL MAP
        pTestCfg->pdsch_map[uec].startRes = pdsch_res_off;
        pdsch_res_off += rbs_per_ue;
        pTestCfg->pdsch_map[uec].numRes = rbs_per_ue;
        // PDSCH DL MAP last UE to have some extra RBs
        if ( uec == (ues-1) )
            pTestCfg->pdsch_map[uec].numRes += extra_rbs;
        // PDCCH boosted (not used)
        pTestCfg->rntiBoosted[uec] = 64 + uec;


        // PUSCH
        // PUSCH MCS
        pTestCfg->puschMcs[uec] = 28;
        // PUSCH RNTI
        pTestCfg->rntiPusch[uec] = 64 + uec;
        // PUSCH UL MAP
        pTestCfg->pusch_map[uec].startRes = pusch_res_off;
        pTestCfg->pusch_map[uec].numRes = pusch_rbs_per_ue;
        pusch_res_off += pusch_rbs_per_ue;
        // PUSCH nDMRS2
        pTestCfg->pusch_nDMRS2[uec] = 0; // not used

        //  ========= PDCCH DL MAP
        pddch_index = uec*2; // mul UE index by 2 in order to have 1 PDCCH for DL and next one for UL

        // update resource map. Every PDDCH assigned as many CCE as required
        pTestCfg->pdcch_map[pddch_index][0].startRes = pdcch_res_off;
        pTestCfg->pdcch_map[pddch_index][0].numRes = pTestCfg->nCcePerPdcch;

        // prepare dci info based on resource map
        dci_payload = &pTestCfg->payloadPdcch[pddch_index][0];
        dci.Lcrb = pTestCfg->pdsch_map[uec].numRes;
        pDciInfo->l_crb = pTestCfg->pdsch_map[uec].numRes;
        dci.startRBIndex = pTestCfg->pdsch_map[uec].startRes;
        pDciInfo->start_rb_index = pTestCfg->pdsch_map[uec].startRes;



        // use FORMAT_1A for 1 ant and FORMAT_2  for 2 ant
        idx = 0;
        if ( numAnt == 1 )
        {
//            DciPrepareFormat1a_autoRBA(dci_payload, &dci, &idx);
            pDciInfo->dci_format = DCI_FORMAT_1A;
            dci_encode_1a((U32 *) dci_payload, (DCI_INFO *)pDciInfo, (BS_STATIC_CONFIG *) &StaticConfig, &index);
            idx = StaticConfig.DCI_size.format1a;
        }
        else if ( numAnt == 2 )
        {
//            DciPrepareFormat2_autoRes(dci_payload, &dci, &idx);
            pDciInfo->dci_format = DCI_FORMAT_2;
            dci_encode_2((U32 *) dci_payload, (DCI_INFO *)pDciInfo, (BS_STATIC_CONFIG *) &StaticConfig, &index);
            idx = StaticConfig.DCI_size.format2;
        }

        pTestCfg->PdcchDataLen[pddch_index] = idx;

        pdcch_res_off += pTestCfg->nCcePerPdcch;
        pddch_index += 1;
        //  ========= PDCCH DL MAP END


        // ========= PDCCH UL MAP
        pTestCfg->pdcch_map[pddch_index][0].startRes = pdcch_res_off;
        pTestCfg->pdcch_map[pddch_index][0].numRes = 1;

        dci_payload = &pTestCfg->payloadPdcch[pddch_index][0];
        dci.Lcrb = pTestCfg->pusch_map[uec].numRes;
        dci.startRBIndex = pTestCfg->pusch_map[uec].startRes;

        idx = 0;
//        DciPrepareFormat0(dci_payload, &dci, &idx);
        dci_encode_0((U32 *)dci_payload, pDciInfo, (BS_STATIC_CONFIG *)&StaticConfig, &idx);

        pTestCfg->PdcchDataLen[pddch_index] = idx;
        pdcch_res_off += 1;
        // ========= PDCCH UL MAP END

/*
        PUCCH not requested

        // PUCCH RNTI (scrInitValueInput)
        pTestCfg->rntiPucch[uec] = 64 + uec;
        pTestCfg->pucch_map[uec].startRes = (uec & 1) ? uec : rbs - uec - 1;
        pTestCfg->pucch_map[uec].numRes = 1;
        // PUCCH format
        pTestCfg->pucchFormat[uec] = FORMAT2;
        // PUCCH dlCqiPmiSizeBits
        pTestCfg->pucch_dlCqiPmiSizeBits[uec] = 11;
        // PUCCH harqPucchIndex
        pTestCfg->pucch_harqPucchIndex[uec] = 0;
*/
    }

    return SUCCESS;
}

PDSCH_CONFIG     pdsch_default                                    = {{DCI_FORMAT_HARDCODED_PAYLOAD, {0},0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 0, 0, {0}, NULL, 28};

PUSCH_CONFIG     pusch_default                                    = {{DCI_FORMAT_HARDCODED_PAYLOAD, {0},0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 0, 0, {0}, NULL, 28, 0, 0, 0,     0,  0,  0,                      0, 0};
PUSCH_CONFIG     pusch_enamult_ri0x0_cqi_32x32_convoenc_harq_2x13 = {{DCI_FORMAT_HARDCODED_PAYLOAD, {0},0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 0, 0, {0}, NULL, 28, 1, 0, 0,     32, 32, CS_TAIL_BITTING,        2, 13};
PUSCH_CONFIG     pusch_enamult_ri2x4_cqi_11x16_blockenc_harq_0x00 = {{DCI_FORMAT_HARDCODED_PAYLOAD, {0},0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 0, 0, {0}, NULL, 28, 1, 2, 4,     11, 16, CS_BLOCK_CODE_CQI_32_2, 0, 0};

PDCCH_CONFIG     pdcch_default = {0};

PUCCH_CONFIG     pucch_format1       = {FORMAT1,  0, 0};
PUCCH_CONFIG     pucch_format1a      = {FORMAT1A, 0, 0};
PUCCH_CONFIG     pucch_format2_4cqi  = {FORMAT2,  0, 4};
PUCCH_CONFIG     pucch_format2_6cqi  = {FORMAT2,  0, 6};
PUCCH_CONFIG     pucch_format2_8cqi  = {FORMAT2,  0, 8};
PUCCH_CONFIG     pucch_format2_13cqi = {FORMAT2,  0, 13};



VUE_CFG test_cfg_8xpdsch_1xpusch_nomultri_2xformat1_2xformat1a_4xformat2_6cqi[] =
{
    { 64, &pdsch_default, &pusch_default, &pdcch_default, &pucch_format1      },
    { 65, &pdsch_default, 0,              0,              &pucch_format1      },
    { 66, &pdsch_default, 0,              0,              &pucch_format1a     },
    { 67, &pdsch_default, 0,              0,              &pucch_format1a     },
    { 68, &pdsch_default, 0,              0,              &pucch_format2_6cqi },
    { 69, &pdsch_default, 0,              0,              &pucch_format2_6cqi },
    { 70, &pdsch_default, 0,              0,              &pucch_format2_6cqi },
    { 71, &pdsch_default, 0,              0,              &pucch_format2_6cqi },
    { 0,  0,              0,              0,              NULL                }
};

VUE_CFG test_cfg_5xpdsch_1xpusch_multri_2xformat1_2xformat1a_4xformat2_13cqi[] =
{
    { 64, &pdsch_default, &pusch_enamult_ri2x4_cqi_11x16_blockenc_harq_0x00, &pdcch_default, &pucch_format1      },
    { 65, &pdsch_default, 0,                                                 0,              &pucch_format1      },
    { 66, &pdsch_default, 0,                                                 0,              &pucch_format1a     },
    { 67, &pdsch_default, 0,                                                 0,              &pucch_format1a     },
    { 68, &pdsch_default, 0,                                                 0,              &pucch_format2_13cqi},
    { 0,  0,              0,                                                 0,              NULL                }
};



VUE_CFG test_cfg_5xpdsch_1xpusch_multri1_2xformat1_2xformat1a_1xformat2_13cqi[] =
{
    { 64, &pdsch_default, &pusch_enamult_ri0x0_cqi_32x32_convoenc_harq_2x13, &pdcch_default, &pucch_format1      },
    { 65, &pdsch_default, 0,                                                 0,              &pucch_format1      },
    { 66, &pdsch_default, 0,                                                 0,              &pucch_format1a     },
    { 67, &pdsch_default, 0,                                                 0,              &pucch_format1a     },
    { 68, &pdsch_default, 0,                                                 0,              &pucch_format2_6cqi },
    { 0,  0,              0,                                                 0,              NULL                }
};



VUE_CFG test_cfg_4_ue_1[] =
{
    { 64, &pdsch_default, &pusch_default, 0, 0},
    { 65, &pdsch_default, &pusch_default, 0, 0},
    { 66, &pdsch_default, &pusch_default, 0, 0},
    { 67, &pdsch_default, &pusch_default, 0, 0},
    { 0,  0,                      0,              0, 0 }
};

VUE_CFG test_cfg_4_ue_2[] =
{
    { 64, &pdsch_default, 0, 0, &pucch_format1 },
    { 65, &pdsch_default, 0, 0, &pucch_format1 },
    { 66, &pdsch_default, 0, 0, &pucch_format1 },
    { 67, &pdsch_default, 0, 0, &pucch_format1 },
    { 0,  0,                      0, 0, 0 }
};

VUE_CFG test_cfg_4_ue_3[] =
{
    { 64, &pdsch_default, &pusch_default, 0, 0 },
    { 65, &pdsch_default, &pusch_default, 0, 0 },
    { 66, &pdsch_default, 0,              0, &pucch_format1 },
    { 67, &pdsch_default, 0,              0, &pucch_format1 },
    { 0,  0,                      0,              0, 0 }
};

VUE_CFG test_cfg_4_ue_4[] =
{
    { 64, &pdsch_default, &pusch_default, 0, 0 },
    { 65, &pdsch_default, &pusch_default, 0, 0 },
    { 66, &pdsch_default, &pusch_default, 0, 0 },
    { 67, &pdsch_default, &pusch_default, 0, 0 },
    { 0,  0,                      0,              0, 0 }
};

VUE_CFG test_cfg_3_ue_1[] =
{
    { 64, &pdsch_default, &pusch_default, 0, 0 },
    { 65, &pdsch_default, &pusch_default, 0, 0 },
    { 66, &pdsch_default, &pusch_default, 0, 0 },
    { 0,  0,                      0,              0, 0 }
};

VUE_CFG test_cfg_3_ue_2[] =
{
    { 64, &pdsch_default, 0, 0, &pucch_format1},
    { 65, &pdsch_default, 0, 0, &pucch_format1},
    { 66, &pdsch_default, 0, 0, &pucch_format1},
    { 0,  0,                      0, 0, 0 }
};

VUE_CFG test_cfg_3_ue_3[] =
{
    { 64, &pdsch_default, &pusch_default, 0, 0},
    { 65, &pdsch_default, &pusch_default, 0, 0},
    { 66, &pdsch_default, 0,              0, &pucch_format1},
    { 0,  0,                      0,              0, 0 }
};

VUE_CFG test_cfg_3_ue_4[] =
{
    { 64, &pdsch_default, &pusch_default, 0, 0 },
    { 65, &pdsch_default, &pusch_default, 0, 0 },
    { 66, &pdsch_default, &pusch_default, 0, 0 },
    { 0,  0,                      0,              0, 0 }
};

VUE_CFG test_cfg_2_ue_1[] =
{
    { 64, &pdsch_default, &pusch_default, 0, 0 },
    { 65, &pdsch_default, &pusch_default, 0, 0 },
    { 0,  0,                      0,              0, 0 }
};

VUE_CFG test_cfg_2_ue_2[] =
{
    { 64, &pdsch_default, 0, 0, &pucch_format1 },
    { 65, &pdsch_default, 0, 0, &pucch_format1 },
    { 0,  0,                      0, 0, 0 }
};

VUE_CFG test_cfg_2_ue_3[] =
{
    { 64, &pdsch_default, &pusch_default, 0, 0 },
    { 65, &pdsch_default, &pusch_default, 0, 0 },
    { 0,  0,                      0,              0, 0 }
};

VUE_CFG test_cfg_2_ue_4[] =
{
    { 64, &pdsch_default, &pusch_default, 0, 0 },
    { 65, &pdsch_default, &pusch_default, 0, 0 },
    { 0,  0,                      0,              0, 0 }
};

U16 test_subframes_1_start[] = { 0, DYN_CFG_END };
U16 test_subframes_1_end[]  = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, DYN_CFG_END };
U16 test_subframes_all[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, DYN_CFG_END };

static void TestMgr_Reset_Subrames(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
   memset(&pTestCfg->dyn_pdcch[0][0], DYN_CFG_END, sizeof(pTestCfg->dyn_pdcch));
   memset(&pTestCfg->dyn_pdsch[0][0], DYN_CFG_END, sizeof(pTestCfg->dyn_pdsch));
}

static void TestMgr_Assign_PDCCH_Subframes(BS_CONFOR_TEST_CONFIG *pTestCfg, U16 *subframes, U8 chanIdx)
{
    U16 i = 0, k, subframe_idx;

    // walk all subframes and add channel to dyn_pdcch
    while ( (subframe_idx = subframes[i]) != DYN_CFG_END )
    {
        k = 0;

        // find free element
        while ( pTestCfg->dyn_pdcch[subframe_idx][k] != DYN_CFG_END ) ++k;

        // add PDCCH to corresponding subframe
        pTestCfg->dyn_pdcch[subframe_idx][k] = chanIdx;

        ++i;
    }
}

static void TestMgr_Assign_PDSCH_Subframes(BS_CONFOR_TEST_CONFIG *pTestCfg, U16 *subframes, U8 chanIdx)
{
    U16 i = 0, k, subframe_idx;

    // walk all subframes and add channel to dyn_pdsch
    while ( (subframe_idx = subframes[i]) != DYN_CFG_END )
    {
        k = 0;
        // find free element
        while ( pTestCfg->dyn_pdsch[subframe_idx][k] != DYN_CFG_END ) ++k;

        // add PDSCH to corresponding subframe
        pTestCfg->dyn_pdsch[subframe_idx][k] = chanIdx;

        ++i;
    }
}


static void TestMgr_Alloc_Res(BS_CONFOR_TEST_CONFIG *pTestCfg, U8 rbs, U16 *subframes, VUE_CFG *vues, U16 sib)
{
    RBMap *ulm, *dlm;
    VUE_CFG *ue = vues;
    DCI_INFO DciInfo;
    U8 *dci_payload, layer;
    U32 pdcch_res_off = 0;
    U32 pdsch_index = 0, pucch_index = 0, pdcch_index = 0, pusch_index;
    S32 dl_rb_offset = 0, ul_rb_offset = 0;
    U32 dl_rb_size = 0, ul_rb_size = 0;
    U32 si = 0;
    U32 index;

    printf("------ SFs: ");
    while ( subframes[si] != DYN_CFG_END )
    {
        printf("%d ", subframes[si] );
        ++si;
    }
    printf("------\n");


    StaticConfig.bw_mode = GetBwSize(rbs);
    StaticConfig.n_antenna_port = pTestCfg->nAntennaPort;
    StaticConfig.tdd = 0;
    initBS(&StaticConfig);

    ulm = &pTestCfg->ul_rb_map;
    dlm = &pTestCfg->dl_rb_map;

    rbm_init(dlm, rbs);
    rbm_init(ulm, rbs);

    vue_alloc_res(vues, dlm, ulm);

    // allocate SIB
    if ( sib )
    {
        rbm_alloc(dlm, sib, 24, 4, 1);
        printf("SIB 1 PDDCH / 1 PDSCH\n");
    }

    rbm_done(dlm);
    rbm_done(ulm);

    pdcch_index = pTestCfg->nPdcch;
    pdsch_index = pTestCfg->nPdsch[0];
    pusch_index = 0;

    ue = vues;
    while ( ue->rnti )
    {
        if (ue->pdsch)
        {
            PDSCH_CONFIG *pPDSCH = ue->pdsch;
            DCI_INFO     *pDCI_INFO = &ue->pdsch->dci_info;

            rbm_get_prb(dlm, ue->rnti, &dl_rb_offset, &dl_rb_size);

            // add DL PDDCH
            pTestCfg->pdcch_map[pdcch_index][0].startRes = pdcch_res_off;
            pTestCfg->pdcch_map[pdcch_index][0].numRes = pTestCfg->nCcePerPdcch;

            // prepare dci info based on resource map
            dci_payload = (U8*)&ue->pdsch->dci_payload[0];
            pDCI_INFO->hopping = 0;
            pDCI_INFO->virtul_resrc_blk_type = 0;
            pDCI_INFO->l_crb = dl_rb_size;
            pDCI_INFO->start_rb_index = dl_rb_offset;
            pDCI_INFO->tb_info[0].modulation_type = pPDSCH->mcs;
            pDCI_INFO->harq_process_id = 0;
            pDCI_INFO->tb_info[0].ndi = 1;
            pDCI_INFO->tb_info[0].redundancy_ver = 0;
            pDCI_INFO->tpc = 0;
            pPDSCH->dci_encode_function = dci_encode_1a;

            // use DCI_FORMAT_1A for 1 ant
            pPDSCH->dci_encode_function((U32 *) dci_payload, (DCI_INFO *)pDCI_INFO, (BS_STATIC_CONFIG *) &StaticConfig, &index);
            pPDSCH->dci_length_bits = index;

            // for save old capability - NRF
            memcpy((U8*)&pTestCfg->payloadPdcch[pdcch_index][0], (U8*)dci_payload, DCI_MAX_SIZE);
            pTestCfg->PdcchDataLen[pdcch_index] =  index;

            //printf("pdsch: %d\n", pdcch_index);
            TestMgr_Assign_PDCCH_Subframes(pTestCfg, subframes, pdcch_index);

            // add PDSCH
            for ( layer = 0; layer <  pTestCfg->nLayers; ++layer )
            {
                // PDSCH num MCS (well, "cw" is index here, not layer)
                pTestCfg->pdschMcs[layer][pdsch_index] = pPDSCH->mcs;

                // PDSCH Payload Mode
                pTestCfg->pdschPayloadMode[layer][pdsch_index] = pdsch_index & 1; // try different payloads

                // PDSCH Power
                pTestCfg->pdschPrbPa[layer][pdsch_index] = FLOAT2Q8(0);

                // PDSCH Just to shut up GetMcsIndexDl()  - used to create payload for formats 0 and 2 by TestMAC
                pTestCfg->pdschMod[layer][pdsch_index] = predef_mcs_index_table_dl[pPDSCH->mcs].mod_order;
            }

            pTestCfg->pdschRnti[pdsch_index] = ue->rnti;
            pTestCfg->pdsch_map[pdsch_index].startRes = dl_rb_offset;
            pTestCfg->pdsch_map[pdsch_index].numRes = dl_rb_size;

            TestMgr_Assign_PDSCH_Subframes(pTestCfg, subframes, pdsch_index);

            pdcch_res_off += pTestCfg->nCcePerPdcch;

            pdcch_index++;
            pdsch_index++;

        }

        if (ue->pusch)
        {
            PUSCH_CONFIG *pPUSCH = ue->pusch;
            DCI_INFO     *pDCI_INFO = &ue->pusch->dci_info;

            rbm_get_prb(ulm, ue->rnti, &ul_rb_offset, &ul_rb_size);

            // add UL PDDCH
            pTestCfg->pdcch_map[pdcch_index][0].startRes = pdcch_res_off;
            pTestCfg->pdcch_map[pdcch_index][0].numRes = pTestCfg->nCcePerPdcch;

            // prepare dci info based on resource map
            dci_payload = (U8*)&pPUSCH->dci_payload[0];

            pDCI_INFO->hopping = 0;
            pDCI_INFO->l_crb = ul_rb_size;
            pDCI_INFO->start_rb_index = ul_rb_offset;
            pDCI_INFO->mcs_rv = ue->pusch->mcs;
            pDCI_INFO->ndi = 0;
            pDCI_INFO->tpc = 0;
            pDCI_INFO->cyclic_shift_dm_rs = 0;
            pDCI_INFO->cqi_request = 0;
            pPUSCH->dci_encode_function = dci_encode_0;

            pPUSCH->dci_encode_function((U32 *) dci_payload, (DCI_INFO *)&(ue->pusch->dci_info), (BS_STATIC_CONFIG *) &StaticConfig, &index);
            pPUSCH->dci_length_bits = index;

            // for save old capability - NRF
            memcpy((U8*)&pTestCfg->payloadPdcch[pdcch_index][0], (U8*)dci_payload, DCI_MAX_SIZE);
            pTestCfg->PdcchDataLen[pdcch_index] =  index;

            //printf("pusch: %d\n", pdcch_index);
            TestMgr_Assign_PDCCH_Subframes(pTestCfg, subframes, pdcch_index);

            // add PUSCH
            pTestCfg->puschMcs[pusch_index] = ue->pusch->mcs;
            pTestCfg->rntiPusch[pusch_index] = ue->rnti;
            pTestCfg->pusch_map[pusch_index].startRes = ul_rb_offset;
            pTestCfg->pusch_map[pusch_index].numRes = ul_rb_size;
            pTestCfg->pusch_nDMRS2[pusch_index] = 0; // not used
            pTestCfg->pusch_ri[pusch_index] = ue->pusch->num_ri_bits;
            pTestCfg->pusch_nCQI[pusch_index] = ue->pusch->num_cqi_bits;
            pTestCfg->pusch_nack[pusch_index] = ue->pusch->num_harq;


            pdcch_res_off += pTestCfg->nCcePerPdcch;

            pdcch_index++;
            pusch_index++;
        }

        if (ue->pucch)
        {
            // so far PUCCH will be simply overwritten for all frames

            // use +0xffff for PUCCH
            rbm_get_prb(ulm, ue->rnti + 0xffff, &ul_rb_offset, &ul_rb_size);

            // PUCCH RNTI (scrInitValueInput)
            pTestCfg->rntiPucch[pucch_index] = ue->rnti;
            pTestCfg->pucch_map[pucch_index].startRes = ul_rb_offset;
            pTestCfg->pucch_map[pucch_index].numRes = ul_rb_size;
            // PUCCH format
            pTestCfg->pucchFormat[pucch_index] = ue->pucch->pucch_format;
            // PUCCH dlCqiPmiSizeBits
            pTestCfg->pucch_dlCqiPmiSizeBits[pucch_index] = ue->pucch->dlCqiPmiSizeBits;
            // PUCCH harqPucchIndex
            pTestCfg->pucch_harqPucchIndex[pucch_index] = ue->pucch->harqPucchIndex;
            ++pucch_index;
        }

        ++ue;
    }

// add SIB
    if ( sib )
    {
        DCI_INFO *pDciInfo = &DciInfo;

        rbm_get_prb(dlm, sib, &dl_rb_offset, &dl_rb_size);

        // add DL PDDCH
        pTestCfg->pdcch_map[pdcch_index][0].startRes = pdcch_res_off;
        pTestCfg->pdcch_map[pdcch_index][0].numRes = pTestCfg->nCcePerPdcch;

        // prepare dci info based on resource map
        dci_payload = &pTestCfg->payloadPdcch[pdcch_index][0];

        pDciInfo->l_crb = dl_rb_size;
        pDciInfo->start_rb_index = dl_rb_offset;
        pDciInfo->index_ngap = 0;
        pDciInfo->tb_info[0].modulation_type = 28;
        pDciInfo->scheduling_pdsch_codeword = 1;


        // use FORMAT_1C for SIB
        dci_encode_1c((U32 *) dci_payload, (DCI_INFO *)pDciInfo, (BS_STATIC_CONFIG *) &StaticConfig, &index);
        pTestCfg->PdcchDataLen[pdcch_index] = index;

        TestMgr_Assign_PDCCH_Subframes(pTestCfg, subframes, pdcch_index);

        // add PDSCH
        for ( layer = 0; layer <  pTestCfg->nLayers; ++layer )
        {
            // PDSCH num MCS (well, "cw" is index here, not layer)
            pTestCfg->pdschMcs[layer][pdsch_index] = 28;

            // PDSCH Payload Mode
            pTestCfg->pdschPayloadMode[layer][pdsch_index] = pdsch_index & 1; // try different payloads

            // PDSCH Power
            pTestCfg->pdschPrbPa[layer][pdsch_index] = FLOAT2Q8(0);

            // PDSCH Just to shut up GetMcsIndexDl()  - used to create payload for formats 0 and 2 by TestMAC
            pTestCfg->pdschMod[layer][pdsch_index] = predef_mcs_index_table_dl[pTestCfg->pdschMcs[layer][pdsch_index]].mod_order;
        }

        pTestCfg->pdschRnti[pdsch_index] = 0xFFFF; // SIB RNTI
        pTestCfg->pdsch_map[pdsch_index].startRes = dl_rb_offset;
        pTestCfg->pdsch_map[pdsch_index].numRes = dl_rb_size;

        TestMgr_Assign_PDSCH_Subframes(pTestCfg, subframes, pdsch_index);

        pdcch_index++;
        pdsch_index++;
        pdcch_res_off += pTestCfg->nCcePerPdcch;
    }

    // PUCCH & PUSCH are constant cfg all over entire dyn_period
    memset(pTestCfg->numPucch, pucch_index, sizeof(pTestCfg->numPucch));
    memset(pTestCfg->numPusch, pusch_index, sizeof(pTestCfg->numPusch));

    pTestCfg->nPdcch = pdcch_index;
    pTestCfg->nPdsch[0] = pdsch_index;

}

RESULTCODE recreate_vue_cfg(U8 *cfg_array, VUE_CFG *vues)
{
    VUE_CFG *ue = vues;
    U32     start_addr, align_addr = 0;
    U32     max_size = ROUND(sizeof(PDSCH_CONFIG), 4) + ROUND(sizeof(PUSCH_CONFIG), 4) + ROUND(sizeof(PDCCH_CONFIG), 4) + ROUND(sizeof(PUCCH_CONFIG), 4);

    if (cfg_array == NULL)
        return MAC_FAILURE;


    align_addr = (U32)cfg_array;
    start_addr = align_addr;

    while (ue->rnti)
    {
        if (ue->pdsch)
        {
            memcpy(cfg_array, ue->pdsch, sizeof(PDSCH_CONFIG));
            ue->pdsch = (PDSCH_CONFIG *)align_addr;
            align_addr += ROUND(sizeof(PDSCH_CONFIG), 4);
            cfg_array = (U8*)align_addr;
        }

        if (ue->pusch)
        {
            memcpy(cfg_array, ue->pusch, sizeof(PUSCH_CONFIG));
            ue->pusch = (PUSCH_CONFIG *)align_addr;
            align_addr += ROUND(sizeof(PUSCH_CONFIG), 4);
            cfg_array = (U8*)align_addr;
        }


        if (ue->pdcch)
        {
            memcpy(cfg_array, ue->pdcch, sizeof(PDCCH_CONFIG));
            ue->pdcch = (PDCCH_CONFIG *)align_addr;
            align_addr += ROUND(sizeof(PDCCH_CONFIG), 4);
            cfg_array = (U8*)align_addr;
        }

        if (ue->pucch)
        {
            memcpy(cfg_array, ue->pucch, sizeof(PUCCH_CONFIG));
            ue->pucch = (PUCCH_CONFIG *)align_addr;
            align_addr += ROUND(sizeof(PUCCH_CONFIG), 4);
            cfg_array = (U8*)align_addr;
        }
        ++ue;

        if (((U32)(align_addr - start_addr))>=(16*1024-max_size))
        {

// TODO:
#ifdef MX_ARM_ENABLED
            SvsrFreeMsg((U8*)start_addr);
#else
            MsgFreeBuffer((PMSGHEADER)start_addr);
#endif

            printf("memory for cfg_array is finished...\n");
            return MAC_FAILURE;
        }
    }

    return SUCCESS;
}

RESULTCODE TestMgrSet_xUE_MaxThrough(BS_CONFOR_TEST_CONFIG *pTestCfg, VUE_CFG *vues)
{
    U8 numLayers = 1;
    U8 numAnt = 1;
    enum ChBwOptions bw = BW_10_0_MHZ;
    U8 phich = 0;
    U8 rbs;
    U8 *cfg_array;

    printf("%s\n",__FUNCTION__);

    pTestCfg->TestModel = E_TM_3_1;

    if ( bw != BW_10_0_MHZ && bw != BW_20_0_MHZ )
    {
        return MAC_FAILURE;
    }
    rbs = GetMapSize(bw);

    pTestCfg->nAntennaPort = numAnt;
    pTestCfg->nCw = numLayers;
    pTestCfg->nLayers = numLayers;

    if ( numAnt == 1 )
    {
        pBsMacDesc->initParm.txAntennaPortCount = TXANT_ONE;
        pBsMacDesc->initParm.rxAntennaPortCount = RXANT_ONE;
        pTestCfg->transmissionMode = SINGLEANT;
    }
    else if ( numAnt == 2 )
    {
        pBsMacDesc->initParm.txAntennaPortCount = TXANT_TWO;
        pBsMacDesc->initParm.rxAntennaPortCount = RXANT_ONE;
        pTestCfg->transmissionMode = OPNLPSPMUX;
    }
    else
    {
        return MAC_FAILURE;
    }

    pTestCfg->precodingMode = 0;
    pTestCfg->duration = 10;

    pTestCfg->epreSinchro = FLOAT2Q8(0);
    pTestCfg->reservedEpre = MINUS_INF_Q8;

    // PBCH  (to be overwritten anyway?)
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = 0;
    pTestCfg->pbchRv = 0;
    pBsMacDesc->initParm.phyLayerCellIdGroup = 0;
    pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
    pTestCfg->payloadPbch[0] = 0x60; // TODO: correct only for 10 MHz
    pTestCfg->payloadPbch[1] = 0xC0;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;

    // PCFICH
    pTestCfg->nCtrlSymbols = 1;

    // PHICH
    if ( phich )
    {
        pTestCfg->nPhichGroup = 1;
        pTestCfg->nPhichPerGroup = 1; // TODO

        pTestCfg->phichSymPower = FLOAT2Q8(-3.010);
        // only new PHICH API supported (use "createphy 2") !!!
        pTestCfg->DontUsePuschRxSduForHiAndDci0 = 1; // don't use PUSCH RxSdu queue for HiSdu
        pTestCfg->ULEnabled = 0;    // create HiSdu permanently
    }
    else
        pTestCfg->nPhichGroup = 0;


    // PDSCH basic init
    pTestCfg->nPdsch[0] = 0;

    //pTestCfg->nPdschPrb[0][0] = 50;
    //pTestCfg->pdschPrbPa[0][0] = FLOAT2Q8(0);
    pTestCfg->pdschMcs_used[0] = 1;
    if ( pTestCfg->nCw == 2 )
    {
        pTestCfg->pdschMcs_used[1] = 1;
    }
    pTestCfg->pdschRnti_used = 1;
    pTestMgrGetBoostedPdsch = lte_pdsch_set_mapinfo_boosted_direct;
    pTestMgrGetDeBoostedPdsch = lte_pdsch_set_mapinfo_boosted_direct;
    memset(testMgrCtx.boostingMap[0], 0xFF, sizeof(testMgrCtx.boostingMap[0]));

    // PDCCH basic init
    pTestCfg->nPdcchUl = 0;
    pTestCfg->nPdcch = 0;       // number will be incremented each time new PDDCH added
    pTestCfg->nAvalableRegs = 90;
    pTestCfg->nCcePerPdcch = numAnt; // ?!
    pTestCfg->nRegPerCce = 9;
    pTestCfg->nRegAllToPdcch = 90;
    pTestCfg->nNilRegPadding = 0;
    pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
    pTestMgrGetPdcch = lte_pdcch_get_mapinfo_direct;
    pTestCfg->payloadPdcch_unified = 0;
    memset(pTestCfg->pdcch_map_unified, 1, sizeof(pTestCfg->pdcch_map_unified));
    pTestCfg->DciFormat = DCI_FORMAT_HARDCODED_PAYLOAD; // NULL callback in order to use hardcoded dci

    // PRACH
    pTestCfg->rootSequenceIndex = 22;
    pTestCfg->prachConfigIndex = 0;
    pTestCfg->highSpeedFlag =  0;
    pTestCfg->zeroCorrelationZoneConfig = 1;
    pTestCfg->prachFreqOffset = 0;

    // PUCCH
    pTestMgrGetPucch = lte_pucch_set_mapinfo_direct;
    // PUCCH RNTI (scrInitValueInput)
    pTestCfg->rntiPucch_used = 1;
    // PUCCH format
    pTestCfg->pucchFormat_used = 1;
    // PUCCH dlCqiPmiSizeBits
    pTestCfg->pucch_dlCqiPmiSizeBits_used = 1;
    // PUCCH harqPucchIndex
    pTestCfg->pucch_harqPucchIndex_used = 1;
    pTestCfg->ulSfrCtrl.deltaPUCCHShift = 2;
    pTestCfg->ulSfrCtrl.nRBCQI = 1;
    pTestCfg->ulSfrCtrl.n1PucchAN = 11;


    // PUSCH
    // PUSCH MCS
    pTestCfg->ulSfrCtrl.enable64QAM = 1;
    pTestCfg->puschMcs_used = 1;
    pTestMgrGetPusch = lte_pusch_set_mapinfo_direct;
    // PUSCH nDMRS2
    pTestCfg->pusch_nDMRS2_used = 0;
    //pTestCfg->nPuschBoostedPrb = 50;
    //pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);

    // Reset subframe info
    TestMgr_Reset_Subrames(pTestCfg);

    // dymamic cfg period is 20 TTI (different PDCCH/PDSCH layout every TTI)
    pTestCfg->dyn_period = 20;

// TODO:
#ifdef MX_ARM_ENABLED
    cfg_array = (U8*)SvsrAllocMsgEx(16*1024);
#else
    cfg_array = (U8*)MsgAllocBuffer(MSG_COMMON);
#endif

    if (!cfg_array)
    {
        return MAC_FAILURE;
    }
    memset(cfg_array, 0x00, 16*1024);

    if (recreate_vue_cfg(cfg_array, vues) != MAC_FAILURE)
    {
        // Cfg all physical channels + SIB PDCCH & PDSCH once every 20ms (0, 20, 40, ...)
        TestMgr_Alloc_Res(pTestCfg, rbs, &test_subframes_1_start[0], (VUE_CFG*)vues, 0xaf);

        // Cfg all physicall channels, but no SIB for the rest of 19 frames
        TestMgr_Alloc_Res(pTestCfg, rbs, &test_subframes_1_end[0], (VUE_CFG*)vues, 0);
        return SUCCESS;
    }

    /*
    for ( i = 0 ; i != (MAX_DYN_PERIOD + 1); ++i )
    {
        Delay(10);
        printf("\n-----: %d\n", i);
        for ( k = 0; k != MAX_PDCCH_CHANNELS; ++k )
            printf("[%d=%d]", k, pTestCfg->dyn_pdcch[i][k]);
    }*/
    return FAILURE;

}


RESULTCODE TestMgrSet_xUE_MaxThrough_configuring(BS_CONFOR_TEST_CONFIG *pTestCfg, VUE_CFG *vues, U8 numAnt, U8 numLayers, U8 bw)
{
    U8 phich = 0;
    U8 rbs;
    U8 *cfg_array;

    printf("%s\n",__FUNCTION__);

    pTestCfg->TestModel = E_TM_3_1;

    if ( bw != BW_10_0_MHZ && bw != BW_20_0_MHZ )
    {
        return MAC_FAILURE;
    }
    rbs = GetMapSize(bw);

    pTestCfg->nAntennaPort = numAnt;
    pTestCfg->nCw = 1;
    pTestCfg->nLayers = numLayers;

    if ( numAnt == 1 )
    {
        pBsMacDesc->initParm.txAntennaPortCount = TXANT_ONE;
        pBsMacDesc->initParm.rxAntennaPortCount = RXANT_ONE;
        pTestCfg->transmissionMode = SINGLEANT;
    }
    else if ( numAnt == 2 )
    {
        pBsMacDesc->initParm.txAntennaPortCount = TXANT_TWO;
        pBsMacDesc->initParm.rxAntennaPortCount = RXANT_ONE;
        pTestCfg->transmissionMode = TXDIVERSITY;//OPNLPSPMUX;
    }
    else
    {
        return MAC_FAILURE;
    }

    pTestCfg->precodingMode = 0;
    pTestCfg->duration = 10;

    pTestCfg->epreSinchro = FLOAT2Q8(0);
    pTestCfg->reservedEpre = MINUS_INF_Q8;


    // PBCH  (to be overwritten anyway?)
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = 0;
    pTestCfg->pbchRv = 0;
    pBsMacDesc->initParm.phyLayerCellIdGroup = 0;
    pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
    pTestCfg->payloadPbch[0] = 0x60; // TODO: correct only for 10 MHz
    pTestCfg->payloadPbch[1] = 0xC0;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;

    // PCFICH
    pTestCfg->nCtrlSymbols = 1;

    // PHICH
    if ( phich )
    {
        pTestCfg->nPhichGroup = 1;
        pTestCfg->nPhichPerGroup = 1; // TODO

        pTestCfg->phichSymPower = FLOAT2Q8(-3.010);
        // only new PHICH API supported (use "createphy 2") !!!
        pTestCfg->DontUsePuschRxSduForHiAndDci0 = 1; // don't use PUSCH RxSdu queue for HiSdu
        pTestCfg->ULEnabled = 0;    // create HiSdu permanently
    }
    else
        pTestCfg->nPhichGroup = 0;


    // PDSCH basic init
    pTestCfg->nPdsch[0] = 0;

    //pTestCfg->nPdschPrb[0][0] = 50;
    //pTestCfg->pdschPrbPa[0][0] = FLOAT2Q8(0);
    pTestCfg->pdschMcs_used[0] = 1;
    if ( pTestCfg->nCw == 2 )
    {
        pTestCfg->pdschMcs_used[1] = 1;
    }
    pTestCfg->pdschRnti_used = 1;
    pTestMgrGetBoostedPdsch = lte_pdsch_set_mapinfo_boosted_direct;
    pTestMgrGetDeBoostedPdsch = lte_pdsch_set_mapinfo_boosted_direct;
    memset(testMgrCtx.boostingMap[0], 0xFF, sizeof(testMgrCtx.boostingMap[0]));

    // PDCCH basic init
    pTestCfg->nPdcchUl = 0;
    pTestCfg->nPdcch = 0;       // number will be incremented each time new PDDCH added
    pTestCfg->nAvalableRegs = 90;
    pTestCfg->nCcePerPdcch = numAnt; // ?!
    pTestCfg->nRegPerCce = 9;
    pTestCfg->nRegAllToPdcch = 90;
    pTestCfg->nNilRegPadding = 0;
    pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
    pTestMgrGetPdcch = lte_pdcch_get_mapinfo_direct;
    pTestCfg->payloadPdcch_unified = 0;
    memset(pTestCfg->pdcch_map_unified, 1, sizeof(pTestCfg->pdcch_map_unified));
    pTestCfg->DciFormat = DCI_FORMAT_HARDCODED_PAYLOAD; // NULL callback in order to use hardcoded dci

    // PRACH
    pTestCfg->rootSequenceIndex = 22;
    pTestCfg->prachConfigIndex = 0;
    pTestCfg->highSpeedFlag =  0;
    pTestCfg->zeroCorrelationZoneConfig = 1;
    pTestCfg->prachFreqOffset = 0;

    // PUCCH
    pTestMgrGetPucch = lte_pucch_set_mapinfo_direct;
    // PUCCH RNTI (scrInitValueInput)
    pTestCfg->rntiPucch_used = 1;
    // PUCCH format
    pTestCfg->pucchFormat_used = 1;
    // PUCCH dlCqiPmiSizeBits
    pTestCfg->pucch_dlCqiPmiSizeBits_used = 1;
    // PUCCH harqPucchIndex
    pTestCfg->pucch_harqPucchIndex_used = 1;
    pTestCfg->ulSfrCtrl.deltaPUCCHShift = 2;
    pTestCfg->ulSfrCtrl.nRBCQI = 1;
    pTestCfg->ulSfrCtrl.n1PucchAN = 11;


    // PUSCH
    // PUSCH MCS
    pTestCfg->ulSfrCtrl.enable64QAM = 1;
    pTestCfg->puschMcs_used = 1;
    pTestMgrGetPusch = lte_pusch_set_mapinfo_direct;
    // PUSCH nDMRS2
    pTestCfg->pusch_nDMRS2_used = 0;
    //pTestCfg->nPuschBoostedPrb = 50;
    //pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);

    // Reset subframe info
    TestMgr_Reset_Subrames(pTestCfg);

    // dymamic cfg period is 20 TTI (different PDCCH/PDSCH layout every TTI)
    pTestCfg->dyn_period = 20;

    // TODO:
#ifdef MX_ARM_ENABLED
    cfg_array = (U8*)SvsrAllocMsgEx(16*1024);
#else
    cfg_array = (U8*)MsgAllocBuffer(MSG_COMMON);
#endif

    if (!cfg_array)
    {
        return MAC_FAILURE;
    }
    memset(cfg_array, 0x00, 16*1024);

    if (recreate_vue_cfg(cfg_array, vues) != MAC_FAILURE)
    {
        pTestCfg->nPdcch = 0;
        TestMgr_Alloc_Res(pTestCfg, rbs, &test_subframes_all[0], (VUE_CFG*)vues, 0);
        return SUCCESS;
    }
    return MAC_FAILURE;
}

RESULTCODE TestMgrSet_4UE_10Mhz_1ANT_UDP_MPEG2(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
#ifndef MX_ARM_ENABLED
    PLTE_PDSCH_HARQ_STATE plte_pdsch_harq_state = get_lte_pdsch_harq_state();
    DCI_INFO    *pDciInfo = &dci_info[0];
    U32         index;

    printf("%s\n",__FUNCTION__);

    StaticConfig.tdd = 0;
    StaticConfig.bw_mode = BW_10_0_MHZ;
    StaticConfig.n_antenna_port = 1;
    initBS(&StaticConfig);

//--------------------------------------------------------------------------------------------------------------------------------------------------
    plte_pdsch_harq_state->ena_harq = 1;
    plte_pdsch_harq_state->ena_udp_stream = 1;

    /* Boosting map */
    testMgrCtx.boostingMap[0][0] = (U8**)E_TM_3_1MapIndex[BW_10_0_MHZ];

    testMgrCtx.deBoostingMap = NULL;// E_TM_3_2MapIndex[BW_10_0_MHZ]; /* No deboosted data */
    testMgrCtx.boostingUlMap = FdxULMapIndex[BW_10_0_MHZ];

    pTestCfg->PdschCfg[0].ueCategory = 3;

    pTestCfg->TestModel = E_TM_3_1;
    pTestCfg->nAntennaPort = 1;
    pTestCfg->nCw = 1;
    pTestCfg->nLayers = 1;
    pTestCfg->precodingMode = 0;
    pTestCfg->duration = 10;
    pTestCfg->transmissionMode = SINGLEANT;

    pTestCfg->rsBoosting = 1;
    pTestCfg->epreSinchro = FLOAT2Q8(0);
    pTestCfg->reservedEpre = MINUS_INF_Q8;

    /* PBCH */
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = FLOAT2Q8(0);
    pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
    pTestCfg->payloadPbch[0] = 0x60;
    pTestCfg->payloadPbch[1] = 0xC0;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;

    /* PUCCH */

    /* PCFICH */
    pTestCfg->nCtrlSymbols = 1; //1;
//    pTestCfg->pcfichEpre = FLOAT2Q8(0);
    pTestCfg->nPhichGroup =  1; //2;// phich disable
    pTestCfg->nPhichPerGroup = 1;
    pTestCfg->phichSymPower = FLOAT2Q8(-3.010);//FLOAT2Q8(-3.010);
    pTestCfg->phichGroupEpre = FLOAT2Q8(0);

    /* PDCCH */
    pTestCfg->nAvalableRegs = 90;
    pTestCfg->nPdcch = 1;
    pTestCfg->nCcePerPdcch = 1;
    pTestCfg->nRegPerCce = 9;
    pTestCfg->nRegAllToPdcch = 90;
    pTestCfg->nNilRegPadding = 0;
    pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
    pTestCfg->nilRegEpre = MINUS_INF_Q8;
    pTestMgrGetPdcch = lte_pdcch_set_mapinfo; // need to calculate CCE-table

    /* PDSCH */
    pTestCfg->nPdsch[0] = 1;
    pTestCfg->nPdschPrb[0][0] = 50;
    pTestCfg->pdschPrbPa[0][0] = FLOAT2Q8(-5);
//    pTestCfg->pdschDeBoostedPrbPa = FLOAT2Q8(-128); // need for LtePhyUpdatePDSCH->...DeBoostedPower
    pTestCfg->nPdschBoostedPrb = 1;
    pTestCfg->pdschMod[0][0] = QAM16;

    pTestCfg->pdschMcs_used[0] = 1;
    pTestCfg->pdschMcs[0][0] = 13;

    /* DCI Format-1A GRANT */
    {
        U8 **pBoostingMap;
        const U8 *pMap;
        U32 cur_num;
        U8 start_rb_index, l_crb;

        pBoostingMap = testMgrCtx.boostingMap[0][0];
        pMap = pBoostingMap[0];
        cur_num = 50;

        start_rb_index = *pMap;
        l_crb = TestMgrGetNumConsecutiveRBs((U8**)&pMap, &cur_num);

        printf("PDSCH: start_rb_index=%d, l_crb=%d\n", start_rb_index, l_crb);

        pDciInfo->dci_format = DCI_FORMAT_1A;
        pDciInfo->virtul_resrc_blk_type = 0;
        pDciInfo->start_rb_index = start_rb_index;
        pDciInfo->l_crb = l_crb;
        pDciInfo->harq_process_id = 2;
        pDciInfo->tb_info[0].ndi = 1;
        pDciInfo->tb_info[0].redundancy_ver = 0;
        pDciInfo->tpc = 2;

        dci_encode_1a((U32*)&pTestCfg->payloadPdcch[0][0], (DCI_INFO *)pDciInfo, (BS_STATIC_CONFIG *)&StaticConfig, &index);
        pTestCfg->PdcchDataLen[0] = index;
    }


    /* DCI Format-0 GRANT */
    {

        U8 **pBoostingMap;
        const U8 *pMap;
        U32 cur_num;
        U8 start_rb_index, l_crb;

        pBoostingMap = (U8**)testMgrCtx.boostingUlMap;
        pMap = pBoostingMap[0];
        cur_num = 50;

        start_rb_index = *pMap;
        l_crb = TestMgrGetNumConsecutiveRBs((U8**)&pMap, &cur_num);

        printf("PUSCH: start_rb_index=%d, l_crb=%d\n", start_rb_index, l_crb);

        pDciInfo = &dci_info[1];

        pDciInfo->dci_format = DCI_FORMAT_0;
        pDciInfo->hopping = 0;
        pDciInfo->start_rb_index = start_rb_index;
        pDciInfo->l_crb = l_crb;
        pDciInfo->mcs_rv = 14;
        pDciInfo->ndi = 1;
        pDciInfo->tpc = 0;
        pDciInfo->cyclic_shift_dm_rs = 0;
        pDciInfo->cqi_request = 0;

        dci_encode_0((U32*)&pTestCfg->payloadPdcchUl[0], (DCI_INFO *)pDciInfo, (BS_STATIC_CONFIG *)&StaticConfig, &index);
        pTestCfg->PdcchDataLenUl[0] = 27;//index;
        pTestCfg->nPdcchUl = 1;
    }
    testConfConfig.DontUsePuschRxSduForHiAndDci0 = 0;


    /* PUSCH */
    memset(pTestCfg->numPusch, 0, sizeof(pTestCfg->numPusch)); // all sf schedule for UL

    pTestCfg->nPuschBoostedPrb = 45;
    pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);
//    pTestCfg->nPuschDeBoostedPrb = 0;
//    pTestCfg->puschDeBoostedPrbPa = FLOAT2Q8(-128);
    pTestCfg->puschBoostedMod = QPSK;//QAM16;//QPSK;
//    pTestCfg->puschDeBoostedMod = QPSK;//QAM16;//QPSK;

    pTestCfg->rntiPusch[0] = 100;//100;
    pTestCfg->rntiBoosted[0] = 100;//100;
    pTestCfg->rntiDeBoosted[0] = 100;//100;

//    pTestCfg->rntiPusch[0] = 51;//100;
//    pTestCfg->rntiBoosted[0] = 51;//100;
//    pTestCfg->rntiDeBoosted[0] = 51;//100;

    pTestCfg->puschMcs_used = 1;
    memset(pTestCfg->puschMcs, 14, sizeof(pTestCfg->puschMcs));

    pTestCfg->ulSfrCtrl.deltaPUCCHShift = 2;
    pTestCfg->ulSfrCtrl.nRBCQI = 4;
    pTestCfg->ulSfrCtrl.n1PucchAN = 0;
    pTestCfg->ulSfrCtrl.nSB = 2;

    pTestCfg->betaOffsetCQIIndex = 1;

    pTestCfg->puschDed.nACK = 1; // max 2 - Number of HARQ ACK bits.
    pTestCfg->puschDed.betaOffsetACKIndex = 9;
    pTestCfg->puschDed.nDMRS2 = 0;

// ----------------------------------------------------------------------------

    plte_pdsch_set_payload = lte_pdsch_copy_payload_from_udp;

    udp_stream_rxinit();
    udp_stream_txinit();

#endif //#ifdef MX_ARM_ENABLED


    return SUCCESS;
}

RESULTCODE TestMgrSet_xUE_10MHz(BS_CONFOR_TEST_CONFIG *pTestCfg, PTESTMGRINIT pTestMgrInit)
{
    U32* pVals = pTestMgrInit->ConsoleVals;
    U32 i, numUE = 0;

    printf("%s\n",__FUNCTION__);

    if(pTestMgrInit->ConsoleValsNum < 4)
    {
        printf("Usage: %d [[UE1_mcs] [UE1_startRB] [UE1_numRB] ...]\n",
            pTestMgrInit->configId);
        return FAILURE;
    }
    else
    {
        numUE = (pTestMgrInit->ConsoleValsNum - 1) / 3;
        printf("Configured UL test for:\n");
        for (i = 0; i < numUE; i++)
        {
            printf("UE%d: MCS=%d startRB=%d numRB=%d\n", i+1,
                (U8)pVals[i*3 + 1], (U8)pVals[i*3 + 2], (U8)pVals[i*3 + 3]);
        }
    }

    StaticConfig.tdd = 0;
    StaticConfig.bw_mode = BW_10_0_MHZ;
    StaticConfig.n_antenna_port = 2;
    initBS(&StaticConfig);
//---------------------------------------------------------------------------------------------------------------------------------------------------

    pTestCfg->nAntennaPort = 1;
    pTestCfg->nCw = 1;
    pTestCfg->nLayers = 1;
    pTestCfg->duration = 10;
    pTestCfg->transmissionMode = SINGLEANT;

    pTestCfg->epreSinchro = FLOAT2Q8(0);

    // --------------------------------
    // DL channels
    // --------------------------------

    /* PDSCH */
    pTestCfg->nPdsch[0] = 0;
    /* Boosting map */
    // not used in this case, shouldn't be zero
    pTestCfg->nPdschPrb[0][0] = 0;
    memset(testMgrCtx.boostingMap[0], 0xFF, sizeof(testMgrCtx.boostingMap[0]));

    /* PBCH */
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = FLOAT2Q8(0);
    pTestCfg->pbchRv = 3;
    pBsMacDesc->initParm.phyLayerCellIdGroup = 0xFF;
    pTestCfg->payloadPbch[0] = 0xA0;
    pTestCfg->payloadPbch[1] = 0x00;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;

    /* PCFICH */

    /* PHICH */
    pTestCfg->nCtrlSymbols = 1;
    pTestCfg->nPhichGroup =  0;
    pTestCfg->nPhichPerGroup = 2;
    pTestCfg->phichSymPower = FLOAT2Q8(-3.010);
    // only new PHICH API supported (use "createphy 2") !!!
    pTestCfg->DontUsePuschRxSduForHiAndDci0 = 1; // don't use PUSCH RxSdu queue for HiSdu
    pTestCfg->ULEnabled = 0;    // create HiSdu permanently

    /* PDCCH */
    pTestCfg->nPdcchUl = 0;
    pTestCfg->nPdcch = 0;
    // --------------------------------
    // UL channels
    // --------------------------------

    // PUSCH
    memset(pTestCfg->numPusch, numUE, sizeof(pTestCfg->numPusch));

    pTestCfg->ulSfrCtrl.enable64QAM = 1;
    pTestCfg->pusch_nDMRS2_used = 1;
    pTestCfg->puschMcs_used = 1;

    pTestMgrGetPusch = lte_pusch_set_mapinfo_direct;

    pTestCfg->nPuschBoostedPrb = 50;
    pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);

    for(i=0; i<numUE; i++)
    {
        // PUSCH RNTI (scrInitValueInput)
        pTestCfg->rntiPusch[i] = 33 + i;

        // PUSCH MCS
        pTestCfg->puschMcs[i] = (U8)pVals[i*3 + 1];

        // PUSCH UL MAP
        pTestCfg->pusch_map[i].startRes = (U8)pVals[i*3 + 2];
        pTestCfg->pusch_map[i].numRes = (U8)pVals[i*3 + 3];

        // PUSCH nDMRS2
        pTestCfg->pusch_nDMRS2[i] = 0;
    }


    // PUCCH
    memset(pTestCfg->numPucch, 0, sizeof(pTestCfg->numPucch));

    pTestCfg->ulSfrCtrl.deltaPUCCHShift = 0;
    pTestCfg->ulSfrCtrl.nRBCQI = 0;
    pTestCfg->ulSfrCtrl.n1PucchAN = 0;

    // PRACH
    pTestCfg->prachConfigIndex = 65;

    /* Init param */
    pBsMacDesc->initParm.txAntennaPortCount = TXANT_ONE;
    pBsMacDesc->initParm.rxAntennaPortCount = RXANT_ONE;

    return SUCCESS;
}


RESULTCODE TestMgrSet_SpaMux_10Mhz_2ANT_2Layers_2Cw(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
    DCI_INFO    DciInfo;
    DCI_INFO    *pDciInfo = &DciInfo;
    U32                 index;

    printf("%s\n",__FUNCTION__);

    StaticConfig.tdd = 0;
    StaticConfig.bw_mode = BW_10_0_MHZ;
    StaticConfig.n_antenna_port = 2;
    initBS(&StaticConfig);
//---------------------------------------------------------------------------------------------------------------------------------------------------


    pTestCfg->TestModel = BE_TEST_1;
    pTestCfg->nAntennaPort = 2;
    pTestCfg->nCw = 2;
    pTestCfg->nLayers = 2;
    pTestCfg->precodingMode = 0;
    pTestCfg->duration = 10;
    pTestCfg->transmissionMode = OPNLPSPMUX;

    pTestCfg->rsBoosting = 1;
    pTestCfg->epreSinchro = FLOAT2Q8(0);
    pTestCfg->reservedEpre = MINUS_INF_Q8;

    /* PBCH */
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = 0;
    pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
    pTestCfg->payloadPbch[0] = 0xA0;
    pTestCfg->payloadPbch[1] = 0x00;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;

    /* PCFICH */
    pTestCfg->nCtrlSymbols = 1;
    pTestCfg->nPhichGroup =  0;//2; phich disable
    pTestCfg->nPhichPerGroup = 2;
    pTestCfg->phichSymPower = FLOAT2Q8(-3.010);
    pTestCfg->phichGroupEpre = FLOAT2Q8(0);

    /* PDCCH */
    pTestCfg->nAvalableRegs = 90;
    pTestCfg->nPdcch = 1;
    pTestCfg->nCcePerPdcch = 4;
    pTestCfg->nRegPerCce = 9;
    pTestCfg->nRegAllToPdcch = 90;
    pTestCfg->nNilRegPadding = 0;
    pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
    pTestCfg->nilRegEpre = MINUS_INF_Q8;

    /* DCI */
    pTestCfg->DciFormat = DCI_FORMAT_HARDCODED_PAYLOAD;
    //     7F FF  FF  d2  E4 80 40

/*      pTestCfg->payloadPdcch[0][0] = 0x7f;
    pTestCfg->payloadPdcch[0][1] = 0xff;
    pTestCfg->payloadPdcch[0][2] = 0xC0;
    pTestCfg->payloadPdcch[0][3] = 0xe4;
    pTestCfg->payloadPdcch[0][4] = 0x01;
    pTestCfg->payloadPdcch[0][5] = 0x00;*/
    pTestCfg->PdcchDataLen[0] = 43;

    pDciInfo->dci_format = DCI_FORMAT_2;
    pDciInfo->rah = 0;
    pDciInfo->virtul_resrc_blk_type = 0;
    pDciInfo->rbg = 0x1ffff;
    pDciInfo->tpc = 0;
    pDciInfo->harq_process_id = 0;
    pDciInfo->swap_flag = 0;
    pDciInfo->tb_info[0].modulation_type = 0x1c;
    pDciInfo->tb_info[0].ndi = 1;
    pDciInfo->tb_info[0].redundancy_ver = 0;
    pDciInfo->tb_info[1].modulation_type = 0x0;
    pDciInfo->tb_info[1].ndi = 0;
    pDciInfo->tb_info[1].redundancy_ver = 1;
    pDciInfo->precoding_info = 0;


    dci_encode_2((U32*)&pTestCfg->payloadPdcch[0][0], (DCI_INFO *)pDciInfo, (BS_STATIC_CONFIG *)&StaticConfig, &index);
    pTestCfg->PdcchDataLen[0] = index;


    /* PDSCH */
    pTestCfg->nPdsch[0] = 1;
    pTestCfg->nPdsch[1] = 1;

    pTestCfg->nPdschPrb[0][0] = 50;
    pTestCfg->pdschPrbPa[0][0] = FLOAT2Q8(0);
    pTestCfg->pdschMod[0][0] = QAM64;

    pTestCfg->nPdschPrb[1][0] = 50;
    pTestCfg->pdschPrbPa[1][0] = FLOAT2Q8(0);
    pTestCfg->pdschMod[1][0] = QAM16;

    /* PUSCH */
    memset(pTestCfg->numPusch, 1, sizeof(pTestCfg->numPusch));
    pTestCfg->nPuschBoostedPrb = 45;
    pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);
    pTestCfg->puschBoostedMod = QAM64;//QPSK;

    pTestCfg->rntiPusch[0] = 100;//100;

    pTestCfg->rntiBoosted[0] = 100;//100;
    pTestCfg->rntiDeBoosted[0] = 100;//100;

    /* Boosting map */
    testMgrCtx.boostingMap[0][0] = (U8**)E_TM_3_1MapIndex[BW_10_0_MHZ];

    testMgrCtx.boostingUlMap = FdxULMapIndex[BW_10_0_MHZ];
    bs_static_config_update_10mhz(&pBsMacDesc->initParm);
    pBsMacDesc->initParm.txAntennaPortCount = TXANT_TWO;
    pBsMacDesc->initParm.rxAntennaPortCount = RXANT_TWO;

    return SUCCESS;
}

RESULTCODE TestHarqOptimizationQam64(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
#define SNR_10
    DCI_INFO    DciInfo;
    DCI_INFO    *pDciInfo = &DciInfo;
    U32                 index;


#ifdef SNR_10
    U8 rv[100] = {0, 0, 0, 0, 0, 0, 0, 0,      0, 0, 2, 0, 0, 0, 0, 2,
                  0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 2, 0, 0, 0,
                  0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 2, 2, 0,
                  0, 0, 0, 0, 0, 0, 3, 0,   2, 0, 2, 0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0, 0, 2, 0,   0, 0, 0, 0, 0, 0, 0, 2,
                  2, 0, 0, 0, 0, 2, 0, 0,   0, 2, 0, 0, 0, 0, 0, 2,
                  0, 0, 0, 0};
#else
    U8 rv[100] = {0};
#endif

    printf("%s\n",__FUNCTION__);
    printf("DL PBCH, PHICH, PDCCH(grant) UL 36 Mbps\n");

    StaticConfig.tdd = 0;
    StaticConfig.bw_mode = BW_10_0_MHZ;
    StaticConfig.n_antenna_port = 1;
    initBS(&StaticConfig);

//---------------------------------------------------------------------------------------------------------------------------------------------------

    memcpy(pTestCfg->arRctRv, rv, sizeof(pTestCfg->arRctRv));
    pGetRvi = lte_pusch_rvi;
    lte_modulation_set_mcs_index_ul(QAM64, 28);

    pTestCfg->TestModel = E_TM_3_1;
    pTestCfg->nAntennaPort = 1;
    pTestCfg->nCw = 1;
    pTestCfg->nLayers = 1;
    pTestCfg->precodingMode = 0;
    pTestCfg->duration = 10;
    pTestCfg->transmissionMode = SINGLEANT;

    pTestCfg->rsBoosting = 1;
    pTestCfg->epreSinchro = FLOAT2Q8(0);
    pTestCfg->reservedEpre = MINUS_INF_Q8;

    /* PBCH */
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = 0;
    pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
    pTestCfg->payloadPbch[0] = 0x60;
    pTestCfg->payloadPbch[1] = 0xC0;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;

    /* PCFICH */
    pTestCfg->nCtrlSymbols = 1; //1;
    pTestCfg->nPhichGroup =  1; //phich disable
    pTestCfg->nPhichPerGroup = 1;
    pTestCfg->phichSymPower = FLOAT2Q8(-3.010);
    pTestCfg->phichGroupEpre = FLOAT2Q8(0);

    /* PDCCH */
    pTestCfg->nAvalableRegs = 90;
    pTestCfg->nPdcch = 1;
    pTestCfg->nCcePerPdcch = 1;
    pTestCfg->nRegPerCce = 9;
    pTestCfg->nRegAllToPdcch = 90;
    pTestCfg->nNilRegPadding = 0;
    pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
    pTestCfg->nilRegEpre = MINUS_INF_Q8;

    /* DCI Format-0 GRANT */
/*    pTestCfg->payloadPdcch[0][0] = 0x06;
    pTestCfg->payloadPdcch[0][1] = 0x52;
    pTestCfg->payloadPdcch[0][2] = 0x28;
    pTestCfg->payloadPdcch[0][3] = 0x00;*/
    pTestCfg->PdcchDataLen[0] = 27;


    pDciInfo->hopping = 0;
    pDciInfo->start_rb_index = 2;
    pDciInfo->l_crb = 5;
    pDciInfo->mcs_rv = 0x8;
    pDciInfo->ndi = 1;
    pDciInfo->tpc = 1;
    pDciInfo->cyclic_shift_dm_rs = 0;
    pDciInfo->cqi_request = 0;

    dci_encode_0((U32*)&pTestCfg->payloadPdcch[0][0], (DCI_INFO *)pDciInfo, (BS_STATIC_CONFIG *)&StaticConfig, &index);
    pTestCfg->PdcchDataLen[0] = index;


    /* PDSCH */
    pTestCfg->nPdsch[0] = 1;
    pTestCfg->nPdschPrb[0][0] = 50;
    pTestCfg->pdschPrbPa[0][0] = FLOAT2Q8(0);
    pTestCfg->pdschMod[0][0] = QAM64;

    /* PUSCH */
    memset(pTestCfg->numPusch, 1, sizeof(pTestCfg->numPusch));
    pTestCfg->nPuschBoostedPrb = 45;

    pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);
    pTestCfg->puschBoostedMod = QAM64;

    pTestCfg->rntiPusch[0] = 0;
    pTestCfg->rntiBoosted[0] = 100;
    pTestCfg->rntiDeBoosted[0] = 100;

    /* Boosting map */
    testMgrCtx.boostingMap[0][0] = (U8**)E_TM_3_1MapIndex[BW_10_0_MHZ];


    testMgrCtx.boostingUlMap = FdxULMapIndex[BW_10_0_MHZ];

    pBsMacDesc->isMIBconfigured = TRUE;

    return SUCCESS;
}


RESULTCODE TestHarqOptimizationQam16(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
#define SNR_10
    DCI_INFO    DciInfo;
    DCI_INFO    *pDciInfo = &DciInfo;
    U32         index;


#ifdef SNR_10
    U8 rv[100] = {0, 0, 0, 0, 0, 0, 0, 0,      0, 0, 2, 0, 0, 0, 0, 2,
                  0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 2, 0, 0, 0,
                  0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 2, 2, 0,
                  0, 0, 0, 0, 0, 0, 3, 0,   2, 0, 2, 0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0, 0, 2, 0,   0, 0, 0, 0, 0, 0, 0, 2,
                  2, 0, 0, 0, 0, 2, 0, 0,   0, 2, 0, 0, 0, 0, 0, 2,
                  0, 0, 0, 0};
#else
    U8 rv[100] = {0};
#endif

    printf("%s\n",__FUNCTION__);
    printf("DL PBCH, PHICH, PDCCH(grant) UL 36 Mbps\n");


    StaticConfig.tdd = 0;
    StaticConfig.bw_mode = BW_10_0_MHZ;
    StaticConfig.n_antenna_port = 1;
    initBS(&StaticConfig);

//---------------------------------------------------------------------------------------------------------------------------------------------------


    memcpy(pTestCfg->arRctRv, rv, sizeof(pTestCfg->arRctRv));
    pGetRvi = lte_pusch_rvi;
    lte_modulation_set_mcs_index_ul(QAM16, 16);

    pTestCfg->TestModel = E_TM_3_1;
    pTestCfg->nAntennaPort = 1;
    pTestCfg->nCw = 1;
    pTestCfg->nLayers = 1;
    pTestCfg->precodingMode = 0;
    pTestCfg->duration = 10;
    pTestCfg->transmissionMode = SINGLEANT;

    pTestCfg->rsBoosting = 1;
    pTestCfg->epreSinchro = FLOAT2Q8(0);
    pTestCfg->reservedEpre = MINUS_INF_Q8;

    /* PBCH */
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = 0;
    pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
    pTestCfg->payloadPbch[0] = 0x60;
    pTestCfg->payloadPbch[1] = 0xC0;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;

    /* PCFICH */
    pTestCfg->nCtrlSymbols = 1; //1;
    pTestCfg->nPhichGroup =  1; //phich disable
    pTestCfg->nPhichPerGroup = 1;
    pTestCfg->phichSymPower = FLOAT2Q8(-3.010);
    pTestCfg->phichGroupEpre = FLOAT2Q8(0);

    /* PDCCH */
    pTestCfg->nAvalableRegs = 90;
    pTestCfg->nPdcch = 1;
    pTestCfg->nCcePerPdcch = 1;
    pTestCfg->nRegPerCce = 9;
    pTestCfg->nRegAllToPdcch = 90;
    pTestCfg->nNilRegPadding = 0;
    pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
    pTestCfg->nilRegEpre = MINUS_INF_Q8;

    /* DCI Format-0 GRANT */
/*    pTestCfg->payloadPdcch[0][0] = 0x06;
    pTestCfg->payloadPdcch[0][1] = 0x52;
    pTestCfg->payloadPdcch[0][2] = 0x28;
    pTestCfg->payloadPdcch[0][3] = 0x00;*/
    pTestCfg->PdcchDataLen[0] = 27;

    pDciInfo->hopping = 0;
    pDciInfo->start_rb_index = 2;
    pDciInfo->l_crb = 5;
    pDciInfo->mcs_rv = 0x8;
    pDciInfo->ndi = 1;
    pDciInfo->tpc = 1;
    pDciInfo->cyclic_shift_dm_rs = 0;
    pDciInfo->cqi_request = 0;

    dci_encode_0((U32*)&pTestCfg->payloadPdcch[0][0], (DCI_INFO *)pDciInfo, (BS_STATIC_CONFIG *)&StaticConfig, &index);
    pTestCfg->PdcchDataLen[0] = index;

    /* PDSCH */
    pTestCfg->nPdsch[0] = 1;

    pTestCfg->nPdschPrb[0][0] = 50;
    pTestCfg->pdschPrbPa[0][0] = FLOAT2Q8(0);
    pTestCfg->pdschMod[0][0] = QAM16;


    /* PUSCH */
    memset(pTestCfg->numPusch, 1, sizeof(pTestCfg->numPusch));
    pTestCfg->nPuschBoostedPrb = 45;

    pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);
    pTestCfg->puschBoostedMod = QAM16;

    pTestCfg->rntiPusch[0] = 0;
    pTestCfg->rntiBoosted[0] = 100;
    pTestCfg->rntiDeBoosted[0] = 100;

    /* Boosting map */
    testMgrCtx.boostingMap[0][0] = (U8**)E_TM_3_1MapIndex[BW_10_0_MHZ];

    testMgrCtx.boostingUlMap = FdxULMapIndex[BW_10_0_MHZ];

    pBsMacDesc->isMIBconfigured = TRUE;

    return SUCCESS;
}

///New APIs tests cases
RESULTCODE Test10MhzHI(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
    DCI_INFO    DciInfo;
    DCI_INFO    *pDciInfo = &DciInfo;
    U32         index;

    printf("%s\n",__FUNCTION__);
    printf("DL 36 Mbps UL 19.08 Mbps\n");

    StaticConfig.tdd = 0;
    StaticConfig.bw_mode = BW_10_0_MHZ;
    StaticConfig.n_antenna_port = 1;
    initBS(&StaticConfig);

//---------------------------------------------------------------------------------------------------------------------------------------------------


    pTestCfg->TestModel = E_TM_3_1;
    pTestCfg->nAntennaPort = 1;
    pTestCfg->nCw = 1;
    pTestCfg->nLayers = 1;
    pTestCfg->precodingMode = 0;
    pTestCfg->duration = 10;
    pTestCfg->transmissionMode = SINGLEANT;

    pTestCfg->rsBoosting = 1;
    pTestCfg->epreSinchro = FLOAT2Q8(0);
    pTestCfg->reservedEpre = MINUS_INF_Q8;

    /* PBCH */
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = 0;
    pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
    pTestCfg->payloadPbch[0] = 0x60;
    pTestCfg->payloadPbch[1] = 0xC0;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;

    /* PCFICH */
    pTestCfg->nCtrlSymbols = 1; //1;
    pTestCfg->nPhichGroup =  1;
    pTestCfg->nPhichPerGroup = 1;
    pTestCfg->phichSymPower = FLOAT2Q8(-3.010);
    pTestCfg->phichGroupEpre = FLOAT2Q8(0);

    /* PDCCH */
    pTestCfg->nAvalableRegs = 90;
    pTestCfg->nPdcch = 1;
    pTestCfg->nCcePerPdcch = 1;
    pTestCfg->nRegPerCce = 9;
    pTestCfg->nRegAllToPdcch = 90;
    pTestCfg->nNilRegPadding = 0;
    pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
    pTestCfg->nilRegEpre = MINUS_INF_Q8;
    pTestCfg->nPdcchUl = 0;

    /* DCI */
/*
    pTestCfg->payloadPdcch[0][0] = 0x83;
    pTestCfg->payloadPdcch[0][1] = 0x1F;
    pTestCfg->payloadPdcch[0][2] = 0x14;
    pTestCfg->payloadPdcch[0][3] = 0x80;*/
    pTestCfg->PdcchDataLen[0] = 27;


    pDciInfo->virtul_resrc_blk_type = 0;
    pDciInfo->start_rb_index = 0;
    pDciInfo->l_crb = 50;
    pDciInfo->tb_info[0].modulation_type = 0x1c;
    pDciInfo->harq_process_id = 2;
    pDciInfo->tb_info[0].ndi = 1;
    pDciInfo->tb_info[0].redundancy_ver = 0;
    pDciInfo->tpc = 2;

    dci_encode_1a((U32*)&pTestCfg->payloadPdcch[0][0], (DCI_INFO *)pDciInfo, (BS_STATIC_CONFIG *)&StaticConfig, &index);
    pTestCfg->PdcchDataLen[0] = index;


    /* PDSCH */
    pTestCfg->nPdsch[0] = 1;

    pTestCfg->nPdschPrb[0][0] = 50;
    pTestCfg->pdschPrbPa[0][0] = FLOAT2Q8(0);
    pTestCfg->pdschMod[0][0] = QAM16;


    /* PUSCH */
    memset(pTestCfg->numPusch, 1, sizeof(pTestCfg->numPusch));
    pTestCfg->nPuschBoostedPrb = 45;

    pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);
    pTestCfg->puschBoostedMod = QAM16;

    pTestCfg->rntiPusch[0] = 100;

    pTestCfg->rntiBoosted[0] = 100;
    pTestCfg->rntiDeBoosted[0] = 100;

    /* Boosting map */

    /* Boosting map */
    testMgrCtx.boostingMap[0][0] = (U8**)testMapIndex[BW_10_0_MHZ];

    testMgrCtx.boostingUlMap = FdxULMapIndex[BW_10_0_MHZ];

    return SUCCESS;
}


RESULTCODE Test10MhzDCI0(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
    DCI_INFO    DciInfo;
    DCI_INFO    *pDciInfo = &DciInfo;
    U32         index;

    printf("%s\n",__FUNCTION__);
    printf("DL 36 Mbps UL 19.08 Mbps\n");

    StaticConfig.tdd = 0;
    StaticConfig.bw_mode = BW_10_0_MHZ;
    StaticConfig.n_antenna_port = 1;
    initBS(&StaticConfig);

//---------------------------------------------------------------------------------------------------------------------------------------------------

    pTestCfg->TestModel = E_TM_3_1;
    pTestCfg->nAntennaPort = 1;
    pTestCfg->nCw = 1;
    pTestCfg->nLayers = 1;
    pTestCfg->precodingMode = 0;
    pTestCfg->duration = 10;
    pTestCfg->transmissionMode = SINGLEANT;

    pTestCfg->rsBoosting = 1;
    pTestCfg->epreSinchro = FLOAT2Q8(0);
    pTestCfg->reservedEpre = MINUS_INF_Q8;

    /* PBCH */
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = 0;
    pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
    pTestCfg->payloadPbch[0] = 0x60;
    pTestCfg->payloadPbch[1] = 0xC0;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;

    /* PCFICH */
    pTestCfg->nCtrlSymbols = 1; //1;
    pTestCfg->nPhichGroup =  0; //phich disable
    pTestCfg->nPhichPerGroup = 2;
    pTestCfg->phichSymPower = FLOAT2Q8(-3.010);
    pTestCfg->phichGroupEpre = FLOAT2Q8(0);

    /* PDCCH */
    pTestCfg->nAvalableRegs = 90;
    pTestCfg->nPdcch = 1;
    pTestCfg->nCcePerPdcch = 1;
    pTestCfg->nRegPerCce = 9;
    pTestCfg->nRegAllToPdcch = 90;
    pTestCfg->nNilRegPadding = 0;
    pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
    pTestCfg->nilRegEpre = MINUS_INF_Q8;

    /* DCI */
/*    pTestCfg->payloadPdcch[0][0] = 0xA0;//0x83;
    pTestCfg->payloadPdcch[0][1] = 0xD7;//0x1F;
    pTestCfg->payloadPdcch[0][2] = 0x14;
    pTestCfg->payloadPdcch[0][3] = 0x80;*/
    pTestCfg->PdcchDataLen[0] = 27;


    pDciInfo->virtul_resrc_blk_type = 0;
    pDciInfo->start_rb_index = 0;
    pDciInfo->l_crb = 22;
    pDciInfo->tb_info[0].modulation_type = 0x1c;
    pDciInfo->harq_process_id = 2;
    pDciInfo->tb_info[0].ndi = 1;
    pDciInfo->tb_info[0].redundancy_ver = 0;
    pDciInfo->tpc = 2;

    dci_encode_1a((U32*)&pTestCfg->payloadPdcch[0][0], (DCI_INFO *)pDciInfo, (BS_STATIC_CONFIG *)&StaticConfig, &index);
    pTestCfg->PdcchDataLen[0] = index;



    pTestCfg->nPdcchUl = 1;
    /* DCI Format-0 GRANT */
/*    pTestCfg->payloadPdcchUl[0] = 0x06;
    pTestCfg->payloadPdcchUl[1] = 0x52;
    pTestCfg->payloadPdcchUl[2] = 0x28;
    pTestCfg->payloadPdcchUl[3] = 0x00;*/
    pTestCfg->PdcchDataLenUl[0] = 27;

//    pDciInfo++; refill DCI

    pDciInfo->virtul_resrc_blk_type = 0;
    pDciInfo->start_rb_index = 2;
    pDciInfo->l_crb = 5;
    pDciInfo->mcs_rv = 0x8;
    pDciInfo->ndi = 1;
    pDciInfo->tpc = 1;
    pDciInfo->cyclic_shift_dm_rs = 0;
    pDciInfo->cqi_request = 0;

    dci_encode_0((U32*)&pTestCfg->payloadPdcchUl[0], (DCI_INFO *)pDciInfo, (BS_STATIC_CONFIG *)&StaticConfig, &index);
    pTestCfg->PdcchDataLen[0] = index;

    /* PDSCH */

    pTestCfg->nPdsch[0] = 1;

    pTestCfg->nPdschPrb[0][0] = 50;
    pTestCfg->pdschPrbPa[0][0] = FLOAT2Q8(0);
    pTestCfg->pdschMod[0][0] = QAM64;

    /* PUSCH */
    memset(pTestCfg->numPusch, 1, sizeof(pTestCfg->numPusch));
    pTestCfg->nPuschBoostedPrb = 45;

    pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);
    pTestCfg->puschBoostedMod = QAM16;

    pTestCfg->rntiPusch[0] = 100;
    pTestCfg->rntiBoosted[0] = 100;
    pTestCfg->rntiDeBoosted[0] = 100;

    /* Boosting map */
    testMgrCtx.boostingMap[0][0] = (U8**)testMapIndex[BW_10_0_MHZ];

    testMgrCtx.boostingUlMap = FdxULMapIndex[BW_10_0_MHZ];

    return SUCCESS;
}

RESULTCODE TestMgrSet_4UE_10MHz_1ANT_DLQ64_UL16(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
    U32         index;
    printf("%s\n",__FUNCTION__);

    StaticConfig.tdd = 0;
    StaticConfig.bw_mode = BW_10_0_MHZ;
    pBsMacDesc->initParm.txAntennaPortCount = 2;
    pBsMacDesc->initParm.rxAntennaPortCount = 2;

    initBS(&StaticConfig);
    
    memset(pTestCfg->arRctRv, 0, sizeof(pTestCfg->arRctRv));
    pGetRvi = lte_pusch_rvi;

    pTestCfg->TestModel = E_TM_3_1;
    pTestCfg->nAntennaPort = 1;
    pTestCfg->nCw = 2;
    pTestCfg->nLayers = 2;
    pTestCfg->precodingMode = 3;
    pTestCfg->duration = 10;
    pTestCfg->transmissionMode = OPNLPSPMUX;
    pTestCfg->cddType = 1;
    
    pTestCfg->rsBoosting = 0;
    pTestCfg->epreSinchro = 0;
    pTestCfg->reservedEpre = MINUS_INF_Q8;
    
    /* PBCH */
    pTestCfg->pbchEna = 1;
    pTestCfg->pbchEpre = 0;
    pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
    pTestCfg->payloadPbch[0] = 0x60;
    pTestCfg->payloadPbch[1] = 0xC0;
    pTestCfg->payloadPbch[2] = 0x00;
    pTestCfg->payloadPbch[3] = 0x00;
    
    /* PCFICH */
    pTestCfg->nCtrlSymbols = 2;
    pTestCfg->nPhichGroup =  4;
    pTestCfg->nPhichPerGroup = 3;
    pTestCfg->phichSymPower = 0;
    pTestCfg->phichGroupEpre = 0;
    
    /* PDCCH */
    pTestCfg->nAvalableRegs = 90;
    pTestCfg->nPdcch = 4;
    pTestCfg->nCcePerPdcch = 1;
    pTestCfg->nRegPerCce = 9;
    pTestCfg->nRegAllToPdcch = 18;
    pTestCfg->nNilRegPadding = 72;
    pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
    pTestCfg->nilRegEpre = MINUS_INF_Q8;

    for(index = 0; index < 12 ; index++)
    {
        pTestCfg->PdcchDataLen[index] = 41;
        pTestCfg->payloadPdcch[index][0] = 127;
        pTestCfg->payloadPdcch[index][1] = 255;
        pTestCfg->payloadPdcch[index][2] = 194;
        pTestCfg->payloadPdcch[index][3] = 228;
        pTestCfg->payloadPdcch[index][4] = 228;
        index++;
        pTestCfg->PdcchDataLen[index] = 27;
        pTestCfg->payloadPdcch[index][0] = 0;
        pTestCfg->payloadPdcch[index][1] = 0;
        pTestCfg->payloadPdcch[index][2] = 32;
    }
    
    pTestCfg->PdcchRnti[0] = 1;
    pTestCfg->PdcchRnti[1] = 8;
    pTestCfg->PdcchRnti[2] = 15;
    pTestCfg->PdcchRnti[3] = 22;    
    pTestCfg->PdcchRnti[4] = 29;
    pTestCfg->PdcchRnti[5] = 36;
    pTestCfg->PdcchRnti[6] = 43;
    pTestCfg->PdcchRnti[7] = 53;
    pTestCfg->PdcchRnti[8] = 60;
    pTestCfg->PdcchRnti[9] = 67;
    pTestCfg->PdcchRnti[10] = 74;
    pTestCfg->PdcchRnti[11] = 119;
    
    pTestCfg->DciFormat = DCI_FORMAT_HARDCODED_PAYLOAD;
    
    pTestCfg->nPdcchUl = 0;
    
    /* PDSCH */
    
    pTestCfg->nPdsch[0] = 4;
    pTestCfg->PdschCfgEna = 1;

    //PDSCH1
    pTestCfg->PdschCfg[0].CwCfg[0].Mcs = 24;
    pTestCfg->PdschCfg[0].CwCfg[0].Mod = QAM64;
    pTestCfg->PdschCfg[0].CwCfg[0].Rvi = 0;
    
    pTestCfg->PdschCfg[0].CwCfg[1].Mcs = 24;
    pTestCfg->PdschCfg[0].CwCfg[1].Mod = QAM64;
    pTestCfg->PdschCfg[0].CwCfg[1].Rvi = 0;
    
    pTestCfg->PdschCfg[0].Pa = 0;
    pTestCfg->PdschCfg[0].Rnti = 1;
    pTestCfg->PdschCfg[0].ueCategory = 5;
    pTestCfg->PdschCfg[0].Map.numberofEntries = 1;
    pTestCfg->PdschCfg[0].Map.reselmInfo[0].startRes = 0;
    pTestCfg->PdschCfg[0].Map.reselmInfo[0].numRes = 12;


    //PDSCH2
    pTestCfg->PdschCfg[1].CwCfg[0].Mcs = 24;
    pTestCfg->PdschCfg[1].CwCfg[0].Mod = QAM64;
    pTestCfg->PdschCfg[1].CwCfg[0].Rvi = 0;
    
    pTestCfg->PdschCfg[1].CwCfg[1].Mcs = 24;
    pTestCfg->PdschCfg[1].CwCfg[1].Mod = QAM64;
    pTestCfg->PdschCfg[1].CwCfg[1].Rvi = 0;
    
    pTestCfg->PdschCfg[1].Pa = 0;
    pTestCfg->PdschCfg[1].Rnti = 8;
    pTestCfg->PdschCfg[1].ueCategory = 5;
    pTestCfg->PdschCfg[1].Map.numberofEntries = 1;
    pTestCfg->PdschCfg[1].Map.reselmInfo[0].startRes = 12;
    pTestCfg->PdschCfg[1].Map.reselmInfo[0].numRes = 24;

    //PDSCH3
    pTestCfg->PdschCfg[2].CwCfg[0].Mcs = 24;
    pTestCfg->PdschCfg[2].CwCfg[0].Mod = QAM64;
    pTestCfg->PdschCfg[2].CwCfg[0].Rvi = 0;
    
    pTestCfg->PdschCfg[2].CwCfg[1].Mcs = 24;
    pTestCfg->PdschCfg[2].CwCfg[1].Mod = QAM64;
    pTestCfg->PdschCfg[2].CwCfg[1].Rvi = 0;
    
    pTestCfg->PdschCfg[2].Pa = 0;
    pTestCfg->PdschCfg[2].Rnti = 15;
    pTestCfg->PdschCfg[2].ueCategory = 5;
    pTestCfg->PdschCfg[2].Map.numberofEntries = 1;
    pTestCfg->PdschCfg[2].Map.reselmInfo[0].startRes = 24;
    pTestCfg->PdschCfg[2].Map.reselmInfo[0].numRes = 36;

    //PDSCH4
    pTestCfg->PdschCfg[3].CwCfg[0].Mcs = 24;
    pTestCfg->PdschCfg[3].CwCfg[0].Mod = QAM64;
    pTestCfg->PdschCfg[3].CwCfg[0].Rvi = 0;
    
    pTestCfg->PdschCfg[3].CwCfg[1].Mcs = 24;
    pTestCfg->PdschCfg[3].CwCfg[1].Mod = QAM64;
    pTestCfg->PdschCfg[3].CwCfg[1].Rvi = 0;
    
    pTestCfg->PdschCfg[3].Pa = 0;
    pTestCfg->PdschCfg[3].Rnti = 22;
    pTestCfg->PdschCfg[3].ueCategory = 5;
    pTestCfg->PdschCfg[3].Map.numberofEntries = 1;
    pTestCfg->PdschCfg[3].Map.reselmInfo[0].startRes = 36;
    pTestCfg->PdschCfg[3].Map.reselmInfo[0].numRes = 48;
    
    
    /*PUCCH*/
    pTestCfg->ulSfrCtrl.deltaPUCCHShift = 1;
    
    /* PUSCH */
    
    pTestCfg->ulSfrCtrl.enable64QAM = 1;
    pTestCfg->puschDed.nACK = 2;
    pTestCfg->puschDed.betaOffsetCQIIndex = 5;
    pTestCfg->puschDed.betaOffsetACKIndex = 10;
    
    
    pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);
    pTestCfg->puschBoostedMod = QAM16;
    pTestCfg->pusch_map_used = 1;
    pTestMgrGetPusch = lte_pusch_set_mapinfo_direct;


    /*UE0*/

    pTestCfg->pusch_map[0].startRes = 0;
    pTestCfg->pusch_map[0].numRes = 12;
    pTestCfg->rntiPusch[0] = 0;

    /*UE1*/

    pTestCfg->pusch_map[1].startRes = 12;
        pTestCfg->pusch_map[1].numRes = 20;
    pTestCfg->rntiPusch[1] = 1;
    
    /*UE2*/

    pTestCfg->pusch_map[2].startRes = 32;
    pTestCfg->pusch_map[2].numRes = 10;
    pTestCfg->rntiPusch[2] = 2;

    /*UE3*/

    pTestCfg->pusch_map[3].startRes = 48;
    pTestCfg->pusch_map[3].numRes = 2;
    pTestCfg->rntiPusch[3] = 3;


    memset(pTestCfg->numPusch, 4, sizeof(pTestCfg->numPusch));
    pTestCfg->nPuschBoostedPrb = 0;
    
    /* Boosting map */
//    testMgrCtx.boostingMap[0][0] = (U8**)testMapIndex[BW_10_0_MHZ];
    

    return SUCCESS;
}
RESULTCODE TestMgrSet_3UE_10MHz_1ANT_DLQ64_UL16(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
        U32         index;
        printf("%s\n",__FUNCTION__);
    
        StaticConfig.tdd = 0;
        StaticConfig.bw_mode = BW_10_0_MHZ;
        pBsMacDesc->initParm.txAntennaPortCount = 2;
        pBsMacDesc->initParm.rxAntennaPortCount = 2;

        initBS(&StaticConfig);
        
        memset(pTestCfg->arRctRv, 0, sizeof(pTestCfg->arRctRv));
        pGetRvi = lte_pusch_rvi;
    
        pTestCfg->TestModel = E_TM_3_1;
        pTestCfg->nAntennaPort = 1;
        pTestCfg->nCw = 2;
        pTestCfg->nLayers = 2;
        pTestCfg->precodingMode = 3;
        pTestCfg->duration = 10;
        pTestCfg->transmissionMode = OPNLPSPMUX;
        pTestCfg->cddType = 1;
        
        pTestCfg->rsBoosting = 0;
        pTestCfg->epreSinchro = 0;
        pTestCfg->reservedEpre = MINUS_INF_Q8;
        
        /* PBCH */
        pTestCfg->pbchEna = 1;
        pTestCfg->pbchEpre = 0;
        pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
        pTestCfg->payloadPbch[0] = 0x60;
        pTestCfg->payloadPbch[1] = 0xC0;
        pTestCfg->payloadPbch[2] = 0x00;
        pTestCfg->payloadPbch[3] = 0x00;
        
        /* PCFICH */
        pTestCfg->nCtrlSymbols = 2;
        pTestCfg->nPhichGroup =  2;
        pTestCfg->nPhichPerGroup = 2;
        pTestCfg->phichSymPower = 0;
        pTestCfg->phichGroupEpre = 0;
        
        /* PDCCH */
        pTestCfg->nAvalableRegs = 90;
        pTestCfg->nPdcch = 3;
        pTestCfg->nCcePerPdcch = 1;
        pTestCfg->nRegPerCce = 9;
        pTestCfg->nRegAllToPdcch = 18;
        pTestCfg->nNilRegPadding = 72;
        pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
        pTestCfg->nilRegEpre = MINUS_INF_Q8;
    
        for(index = 0; index < pTestCfg->nPdcch ; index++)
        {
            pTestCfg->PdcchDataLen[index] = 41;
            pTestCfg->payloadPdcch[index][0] = 127;
            pTestCfg->payloadPdcch[index][1] = 255;
            pTestCfg->payloadPdcch[index][2] = 194;
            pTestCfg->payloadPdcch[index][3] = 228;
            pTestCfg->payloadPdcch[index][4] = 228;
            index++;
            pTestCfg->PdcchDataLen[index] = 27;
            pTestCfg->payloadPdcch[index][0] = 0;
            pTestCfg->payloadPdcch[index][1] = 0;
            pTestCfg->payloadPdcch[index][2] = 32;
        }
        
        pTestCfg->PdcchRnti[0] = 1;
        pTestCfg->PdcchRnti[1] = 8;
        pTestCfg->PdcchRnti[2] = 15;
        
        pTestCfg->DciFormat = DCI_FORMAT_HARDCODED_PAYLOAD;
        
        pTestCfg->nPdcchUl = 0;
        
        /* PDSCH */
        
        pTestCfg->nPdsch[0] = 3;
        pTestCfg->PdschCfgEna = 1;
    
        //PDSCH1
        pTestCfg->PdschCfg[0].CwCfg[0].Mcs = 24;
        pTestCfg->PdschCfg[0].CwCfg[0].Mod = QAM64;
        pTestCfg->PdschCfg[0].CwCfg[0].Rvi = 0;
        
        pTestCfg->PdschCfg[0].CwCfg[1].Mcs = 24;
        pTestCfg->PdschCfg[0].CwCfg[1].Mod = QAM64;
        pTestCfg->PdschCfg[0].CwCfg[1].Rvi = 0;
        
        pTestCfg->PdschCfg[0].Pa = 0;
        pTestCfg->PdschCfg[0].Rnti = 1;
        pTestCfg->PdschCfg[0].ueCategory = 5;
        pTestCfg->PdschCfg[0].Map.numberofEntries = 1;
        pTestCfg->PdschCfg[0].Map.reselmInfo[0].startRes = 0;
        pTestCfg->PdschCfg[0].Map.reselmInfo[0].numRes = 12;
    
    
        //PDSCH2
        pTestCfg->PdschCfg[1].CwCfg[0].Mcs = 24;
        pTestCfg->PdschCfg[1].CwCfg[0].Mod = QAM64;
        pTestCfg->PdschCfg[1].CwCfg[0].Rvi = 0;
        
        pTestCfg->PdschCfg[1].CwCfg[1].Mcs = 24;
        pTestCfg->PdschCfg[1].CwCfg[1].Mod = QAM64;
        pTestCfg->PdschCfg[1].CwCfg[1].Rvi = 0;
        
        pTestCfg->PdschCfg[1].Pa = 0;
        pTestCfg->PdschCfg[1].Rnti = 8;
        pTestCfg->PdschCfg[1].ueCategory = 5;
        pTestCfg->PdschCfg[1].Map.numberofEntries = 1;
        pTestCfg->PdschCfg[1].Map.reselmInfo[0].startRes = 12;
        pTestCfg->PdschCfg[1].Map.reselmInfo[0].numRes = 24;
    
        //PDSCH3
        pTestCfg->PdschCfg[2].CwCfg[0].Mcs = 24;
        pTestCfg->PdschCfg[2].CwCfg[0].Mod = QAM64;
        pTestCfg->PdschCfg[2].CwCfg[0].Rvi = 0;
        
        pTestCfg->PdschCfg[2].CwCfg[1].Mcs = 24;
        pTestCfg->PdschCfg[2].CwCfg[1].Mod = QAM64;
        pTestCfg->PdschCfg[2].CwCfg[1].Rvi = 0;
        
        pTestCfg->PdschCfg[2].Pa = 0;
        pTestCfg->PdschCfg[2].Rnti = 15;
        pTestCfg->PdschCfg[2].ueCategory = 5;
        pTestCfg->PdschCfg[2].Map.numberofEntries = 1;
        pTestCfg->PdschCfg[2].Map.reselmInfo[0].startRes = 24;
        pTestCfg->PdschCfg[2].Map.reselmInfo[0].numRes = 36;
    
        
        /*PUCCH*/
        pTestCfg->ulSfrCtrl.deltaPUCCHShift = 1;
        
        /* PUSCH */
        
        pTestCfg->ulSfrCtrl.enable64QAM = 1;
        pTestCfg->puschDed.nACK = 2;
        pTestCfg->puschDed.betaOffsetCQIIndex = 5;
        pTestCfg->puschDed.betaOffsetACKIndex = 10;
        
        pTestCfg->pusch_map_used = 1;
        pTestMgrGetPusch = lte_pusch_set_mapinfo_direct;
    
        pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);
        pTestCfg->puschBoostedMod = QAM16;
    
        /*UE0*/
    
        pTestCfg->pusch_map[0].startRes = 0;
        pTestCfg->pusch_map[0].numRes = 12;
        pTestCfg->rntiPusch[0] = 0;
    
        /*UE1*/
    
        pTestCfg->pusch_map[1].startRes = 12;
        pTestCfg->pusch_map[1].numRes = 20;
        pTestCfg->rntiPusch[1] = 1;
        
        /*UE2*/
    
        pTestCfg->pusch_map[2].startRes = 32;
        pTestCfg->pusch_map[2].numRes = 10;
        pTestCfg->rntiPusch[2] = 2;
    
    
        memset(pTestCfg->numPusch, 3, sizeof(pTestCfg->numPusch));
        pTestCfg->nPuschBoostedPrb = 0;
        
        /* Boosting map */
    //    testMgrCtx.boostingMap[0][0] = (U8**)testMapIndex[BW_10_0_MHZ];
        

    return SUCCESS;

}
RESULTCODE TestMgrSet_2UE_10MHz_1ANT_DLQ64_UL16(BS_CONFOR_TEST_CONFIG *pTestCfg)
{
        U32         index;
        printf("%s\n",__FUNCTION__);
    
        StaticConfig.tdd = 0;
        StaticConfig.bw_mode = BW_10_0_MHZ;
        StaticConfig.n_antenna_port = 2;
        StaticConfig.n_antenna_port_rx = 2;
        pBsMacDesc->initParm.txAntennaPortCount = 2;
        pBsMacDesc->initParm.rxAntennaPortCount = 2;
        //pBsMacDesc->initParm.phyCfg = 2;
        initBS(&StaticConfig);
        
        memset(pTestCfg->arRctRv, 0, sizeof(pTestCfg->arRctRv));
        pGetRvi = lte_pusch_rvi;
    
        pTestCfg->TestModel = E_TM_3_1;
        pTestCfg->nAntennaPort = 1;
        pTestCfg->nCw = 2;
        pTestCfg->nLayers = 2;
        pTestCfg->precodingMode = 3;
        pTestCfg->duration = 10;
        pTestCfg->transmissionMode = OPNLPSPMUX;
        pTestCfg->cddType = 1;
        
        pTestCfg->rsBoosting = 0;
        pTestCfg->epreSinchro = 0;
        pTestCfg->reservedEpre = MINUS_INF_Q8;
        
        /* PBCH */
        pTestCfg->pbchEna = 1;
        pTestCfg->pbchEpre = 0;
        pTestCfg->pbchRv = 0;
        pTestCfg->pbchReservedEpre = MINUS_INF_Q8;
        pTestCfg->payloadPbch[0] = 0x60;
        pTestCfg->payloadPbch[1] = 0xC0;
        pTestCfg->payloadPbch[2] = 0x00;
        pTestCfg->payloadPbch[3] = 0x00;
        
        /* PCFICH */
        pTestCfg->nCtrlSymbols = 2;
        pTestCfg->nPhichGroup =  2;
        pTestCfg->nPhichPerGroup = 2;
        pTestCfg->phichSymPower = 0;
        pTestCfg->phichGroupEpre = 0;
        
        /* PDCCH */
        pTestCfg->nAvalableRegs = 90;
        pTestCfg->nPdcch = 2;
        pTestCfg->nCcePerPdcch = 1;
        pTestCfg->nRegPerCce = 9;
        pTestCfg->nRegAllToPdcch = 18;
        pTestCfg->nNilRegPadding = 72;
        pTestCfg->pdcchRegEpre = FLOAT2Q8(0);
        pTestCfg->nilRegEpre = MINUS_INF_Q8;
    
        for(index = 0; index < pTestCfg->nPdcch ; index++)
        {
            pTestCfg->PdcchDataLen[index] = 41;
            pTestCfg->payloadPdcch[index][0] = 127;
            pTestCfg->payloadPdcch[index][1] = 255;
            pTestCfg->payloadPdcch[index][2] = 194;
            pTestCfg->payloadPdcch[index][3] = 228;
            pTestCfg->payloadPdcch[index][4] = 228;
            index++;
            pTestCfg->PdcchDataLen[index] = 27;
            pTestCfg->payloadPdcch[index][0] = 0;
            pTestCfg->payloadPdcch[index][1] = 0;
            pTestCfg->payloadPdcch[index][2] = 32;
        }
        pTestCfg->PdcchRntiEna = 1;
        pTestCfg->PdcchRnti[0] = 1;
        pTestCfg->PdcchRnti[1] = 8;
        
        pTestCfg->DciFormat = DCI_FORMAT_HARDCODED_PAYLOAD;
        
        pTestCfg->nPdcchUl = 0;
        
        /* PDSCH */
        
        pTestCfg->nPdsch[0] = 2;
        pTestCfg->PdschCfgEna = 1;
    
        //PDSCH1
        pTestCfg->PdschCfg[0].CwCfg[0].Mcs = 24;
        pTestCfg->PdschCfg[0].CwCfg[0].Mod = QAM64;
        pTestCfg->PdschCfg[0].CwCfg[0].Rvi = 0;
        
        pTestCfg->PdschCfg[0].CwCfg[1].Mcs = 24;
        pTestCfg->PdschCfg[0].CwCfg[1].Mod = QAM64;
        pTestCfg->PdschCfg[0].CwCfg[1].Rvi = 0;
        
        pTestCfg->PdschCfg[0].Pa = 0;
        pTestCfg->PdschCfg[0].Rnti = 1;
        pTestCfg->PdschCfg[0].ueCategory = 5;
        pTestCfg->PdschCfg[0].Map.numberofEntries = 1;
        pTestCfg->PdschCfg[0].Map.reselmInfo[0].startRes = 0;
        pTestCfg->PdschCfg[0].Map.reselmInfo[0].numRes = 24;
    
    
        //PDSCH2
        pTestCfg->PdschCfg[1].CwCfg[0].Mcs = 24;
        pTestCfg->PdschCfg[1].CwCfg[0].Mod = QAM64;
        pTestCfg->PdschCfg[1].CwCfg[0].Rvi = 0;
        
        pTestCfg->PdschCfg[1].CwCfg[1].Mcs = 24;
        pTestCfg->PdschCfg[1].CwCfg[1].Mod = QAM64;
        pTestCfg->PdschCfg[1].CwCfg[1].Rvi = 0;
        
        pTestCfg->PdschCfg[1].Pa = 0;
        pTestCfg->PdschCfg[1].Rnti = 8;
        pTestCfg->PdschCfg[1].ueCategory = 5;
        pTestCfg->PdschCfg[1].Map.numberofEntries = 1;
        pTestCfg->PdschCfg[1].Map.reselmInfo[0].startRes = 24;
        pTestCfg->PdschCfg[1].Map.reselmInfo[0].numRes = 24;


        /*PUCCH*/
        pTestCfg->ulSfrCtrl.deltaPUCCHShift = 1;
        
        /* PUSCH */
        
        pTestCfg->ulSfrCtrl.enable64QAM = 1;
        pTestCfg->puschDed.nACK = 2;
        pTestCfg->puschDed.betaOffsetCQIIndex = 5;
        pTestCfg->puschDed.betaOffsetACKIndex = 10;
        
        
        pTestCfg->puschBoostedPrbPa = FLOAT2Q8(0);
        pTestCfg->puschBoostedMod = QAM16;
        pTestCfg->pusch_map_used = 1;
        pTestMgrGetPusch = lte_pusch_set_mapinfo_direct;
    
    
        /*UE0*/
    
        pTestCfg->pusch_map[0].startRes = 0;
        pTestCfg->pusch_map[0].numRes = 12;
        pTestCfg->rntiPusch[0] = 0;
    
        /*UE1*/
    
        pTestCfg->pusch_map[1].startRes = 12;
        pTestCfg->pusch_map[1].numRes = 20;
        pTestCfg->rntiPusch[1] = 1;
        
        memset(pTestCfg->numPusch, 2, sizeof(pTestCfg->numPusch));
        pTestCfg->nPuschBoostedPrb = 0;
        
        /* Boosting map */
    //    testMgrCtx.boostingMap[0][0] = (U8**)testMapIndex[BW_10_0_MHZ];
        

    return SUCCESS;

}

RESULTCODE TestMgrReconfigureRxVec(BS_CONFOR_TEST_CONFIG *pTestConfig)
{
    RESULTCODE ret = SUCCESS;

    memcpy(testConfConfig.timAdvErrDflt_pucch, pTestConfig->timAdvErrDflt_pucch,
                testConfConfig.timAdvErrDflt_pucch_len);

    memcpy(testConfConfig.timAdvErrDflt_pusch, pTestConfig->timAdvErrDflt_pusch,
                testConfConfig.timAdvErrDflt_pusch_len);

    memcpy(&testConfConfig.srsInfo, &pTestConfig->srsInfo, sizeof(testConfConfig.srsInfo));
    testConfConfig.srsBandwitdhConfig = pTestConfig->srsBandwitdhConfig;
    testConfConfig.srsSubframeConfig = pTestConfig->srsSubframeConfig;
    testConfConfig.srsNumber = pTestConfig->srsNumber;
    testConfConfig.ackNackSRSSimultaneousTransmission = pTestConfig->ackNackSRSSimultaneousTransmission;

    if(pTestConfig->puschMcs_used)
    {
        memcpy(testConfConfig.puschMcs, pTestConfig->puschMcs, sizeof(testConfConfig.puschMcs));
        testConfConfig.puschMcs_used = 1;
    }

    if(pTestConfig->rntiPusch_used)
    {
        memcpy(testConfConfig.rntiPusch, pTestConfig->rntiPusch, sizeof(testConfConfig.rntiPusch));
        testConfConfig.rntiPusch_used = 1;
    }

    if(pTestConfig->pusch_map_used)
    {
        memcpy(testConfConfig.pusch_map, pTestConfig->pusch_map, sizeof(testConfConfig.pusch_map));
        pTestMgrGetPusch = lte_pusch_set_mapinfo_direct;
        testConfConfig.pusch_map_used = 1;
    }

    if(pTestConfig->rntiPucch_used)
    {
        memcpy(testConfConfig.rntiPucch, pTestConfig->rntiPucch, sizeof(testConfConfig.rntiPucch));
        testConfConfig.rntiPucch_used = 1;
    }

    if(pTestConfig->pucchFormat_used)
    {
        memcpy(testConfConfig.pucchFormat, pTestConfig->pucchFormat, sizeof(testConfConfig.pucchFormat));
        testConfConfig.pucchFormat_used = 1;
    }

    if(pTestConfig->pucch_map_used)
    {
        memcpy(testConfConfig.pucch_map, pTestConfig->pucch_map, sizeof(testConfConfig.pucch_map));
        pTestMgrGetPucch = lte_pucch_set_mapinfo_direct;
        testConfConfig.pucch_map_used = 1;
    }

    if(pTestConfig->pucch_dlCqiPmiSizeBits_used)
    {
        memcpy(testConfConfig.pucch_dlCqiPmiSizeBits, pTestConfig->pucch_dlCqiPmiSizeBits,
            sizeof(testConfConfig.pucch_dlCqiPmiSizeBits));
        testConfConfig.pucch_dlCqiPmiSizeBits_used = 1;
    }

    if(pTestConfig->pucch_harqSizebits_used)
    {
        memcpy(testConfConfig.pucch_harqSizebits, pTestConfig->pucch_harqSizebits,
            sizeof(testConfConfig.pucch_harqSizebits));
        testConfConfig.pucch_harqSizebits_used = 1;
    }

    if(pTestConfig->pucch_harqPucchIndex_used)
    {
        memcpy(testConfConfig.pucch_harqPucchIndex, pTestConfig->pucch_harqPucchIndex,
            sizeof(testConfConfig.pucch_harqPucchIndex));
        testConfConfig.pucch_harqPucchIndex_used = 1;
    }

    if(pTestConfig->pucch_cqirsrcidx_used)
    {
        memcpy(testConfConfig.pucch_cqirsrcidx, pTestConfig->pucch_cqirsrcidx,
            sizeof(testConfConfig.pucch_cqirsrcidx));
        testConfConfig.pucch_cqirsrcidx_used = 1;
    }


    if(pTestConfig->PuschRiIdxUsed)
    {
        memcpy(testConfConfig.PuschRiIdx,pTestConfig->PuschRiIdx, sizeof(testConfConfig.PuschRiIdx));
        testConfConfig.PuschRiIdxUsed = 1;
    }

    if(pTestConfig->PuschCqiIdxUsed)
    {
        memcpy(testConfConfig.PuschCqiIdx,pTestConfig->PuschCqiIdx, sizeof(testConfConfig.PuschCqiIdx));
        testConfConfig.PuschCqiIdxUsed = 1;
    }

    if(pTestConfig->Pusch_nRiUsed)
    {
            testConfConfig.Pusch_nRiUsed = pTestConfig->Pusch_nRiUsed;
        memcpy(testConfConfig.Pusch_nRi,pTestConfig->Pusch_nRi, testConfConfig.Pusch_nRiUsed);    
    }

    if(pTestConfig->Pusch_nrlCqiUsed)
    {
        testConfConfig.Pusch_nrlCqiUsed = pTestConfig->Pusch_nrlCqiUsed;
        memcpy(testConfConfig.Pusch_nrlCqi, pTestConfig->Pusch_nrlCqi, testConfConfig.Pusch_nrlCqiUsed);
    }

    if(pTestConfig->Pusch_nrglCqiUsed)
    {
        testConfConfig.Pusch_nrglCqiUsed = pTestConfig->Pusch_nrglCqiUsed;    
        memcpy(testConfConfig.Pusch_nrglCqi,pTestConfig->Pusch_nrglCqi, testConfConfig.Pusch_nrglCqiUsed);
    }

    if(pTestConfig->Pusch_nAckUsed)
    {
        testConfConfig.Pusch_nAckUsed = pTestConfig->Pusch_nAckUsed;
        memcpy(testConfConfig.Pusch_nAck, pTestConfig->Pusch_nAck, testConfConfig.Pusch_nAckUsed);
    }

    if(pTestConfig->PuschAckUsed)
    {
        testConfConfig.PuschAckUsed = 1;
        memcpy(testConfConfig.PuschAckIdx,pTestConfig->PuschAckIdx, sizeof(testConfConfig.PuschAckIdx));
    }

#ifdef AC_MSPD_TDD

    if(pTestConfig->ackPUCCHResourceIndex[0] == 1)
    {
        memcpy(testConfConfig.ackPUCCHResourceIndex0, pTestConfig->ackPUCCHResourceIndex0,
                 sizeof(testConfConfig.ackPUCCHResourceIndex0));

    }
    if(pTestConfig->ackPUCCHResourceIndex[1] == 1)
    {
        memcpy(testConfConfig.ackPUCCHResourceIndex1, pTestConfig->ackPUCCHResourceIndex1,
               sizeof(testConfConfig.ackPUCCHResourceIndex1));
    }
    if(pTestConfig->ackPUCCHResourceIndex[2] == 1)
    {
        memcpy(testConfConfig.ackPUCCHResourceIndex2, pTestConfig->ackPUCCHResourceIndex2,
                sizeof(testConfConfig.ackPUCCHResourceIndex2));
    }
    if(pTestConfig->ackPUCCHResourceIndex[3] == 1)
    {
        memcpy(testConfConfig.ackPUCCHResourceIndex3, pTestConfig->ackPUCCHResourceIndex3,
                 sizeof(testConfConfig.ackPUCCHResourceIndex3));
    }
#endif
    return ret;
}

RESULTCODE TestMgrReconfigureTxVec(BS_CONFOR_TEST_CONFIG *pTestConfig)
{
    RESULTCODE ret = SUCCESS;

    if(pTestConfig->pdschMcs_used[0])
    {
        memcpy(testConfConfig.pdschMcs[0], pTestConfig->pdschMcs[0], sizeof(testConfConfig.pdschMcs[0]));
        testConfConfig.pdschMcs_used[0] = 1;
    }

    if(pTestConfig->pdschMcs_used[1])
    {
        memcpy(testConfConfig.pdschMcs[1], pTestConfig->pdschMcs[1], sizeof(testConfConfig.pdschMcs[1]));
        testConfConfig.pdschMcs_used[1] = 1;
    }

    if(pTestConfig->pdschRnti_used)
    {
        memcpy(testConfConfig.pdschRnti, pTestConfig->pdschRnti, sizeof(testConfConfig.pdschRnti));
        testConfConfig.pdschRnti_used = 1;
        testConfConfig.nPdsch[0] = pTestConfig->pdschRnti_used;
    }

    if(pTestConfig->pdsch_map_used)
    {
        memcpy(testConfConfig.pdsch_map, pTestConfig->pdsch_map, sizeof(testConfConfig.pdsch_map));
        pTestMgrGetBoostedPdsch   = lte_pdsch_set_mapinfo_boosted_direct;
        pTestMgrGetDeBoostedPdsch = lte_pdsch_set_mapinfo_boosted_direct;
        // not used in this case, shouldn't be zero
        memset(testMgrCtx.boostingMap[0], 0xFF, sizeof(testMgrCtx.boostingMap[0]));
        testConfConfig.pdsch_map_used = 1;
    }

    if(pTestConfig->pdcch_map_used)
    {
        memcpy(testConfConfig.pdcch_map, pTestConfig->pdcch_map, sizeof(testConfConfig.pdcch_map));
        pTestMgrGetPdcch = lte_pdcch_get_mapinfo_direct;
        testConfConfig.pdcch_map_used = 1;
    }

    if(pTestConfig->PdschCfgEna == 1)
    {
        U32 i;
        pTestMgrGetBoostedPdsch = lte_pdsch_set_mapinfo_boosted_direct;
        pTestMgrGetDeBoostedPdsch = lte_pdsch_set_mapinfo_boosted_direct;
        testConfConfig.nPdsch[0] = pTestConfig->nPdsch[0];
        testConfConfig.PdschCfgEna = 1;
        for(i = 0; i < testConfConfig.nPdsch[0]; i++)
          memcpy(&testConfConfig.PdschCfg[i], &pTestConfig->PdschCfg[i], sizeof(PDSCH_CFG));
        testConfConfig.pdsch_map_used = 1;
    }

    return ret;
}

extern MAC_INIT_CONFIG macInitConfig;
RESULTCODE TestMgrInit(PTESTMGRINIT pTestMgrCfg)
{
    PLTE_PDSCH_HARQ_STATE plte_pdsch_harq_state = get_lte_pdsch_harq_state();
    PLTE_PDCCH_CHAN_STATE plte_pdcch_chan_state = get_lte_pdcch_chan_state();

    PRINT_FUNCTION_NAME(__FUNCTION__);

    
    memset((char *)&dci_info, 0x00, sizeof(DCI_INFO));

    lte_harq_init();

    //init params by default

    pGetRvi = lte_pusch_rvi_default;
    memset(&testConfConfig, 0, sizeof(testConfConfig));
    testConfConfig.prachConfigIndex = 0xFF;

    testConfConfig.configId = pTestMgrCfg->configId;

    pTestMgrGetBoostedPdsch   = lte_pdsch_set_mapinfo_boosted;
    pTestMgrGetDeBoostedPdsch = lte_pdsch_set_mapinfo_deboosted;
    pTestMgrGetPusch = lte_pusch_set_mapinfo;
    pTestMgrGetPucch = lte_pucch_set_mapinfo;
    pTestMgrGetPdcch = lte_pdcch_get_mapinfo_from_cce;
    testConfConfig.dyn_period = 0;

    testConfConfig.nsymi = 12;

#ifdef MX_ARM_ENABLED
    lte_modulation_set_mcs_index_dl(QPSK, 0, 9);
    lte_modulation_set_mcs_index_dl(QAM16, 0, 16);
    lte_modulation_set_mcs_index_dl(QAM64, 0, 28);
    lte_modulation_set_mcs_index_dl(QPSK, 1, 9);
    lte_modulation_set_mcs_index_dl(QAM16, 1, 16);
    lte_modulation_set_mcs_index_dl(QAM64, 1, 28);

    lte_modulation_set_mcs_index_ul(QPSK, 9);
    lte_modulation_set_mcs_index_ul(QAM16, 20);
    lte_modulation_set_mcs_index_ul(QAM64, 28);
#else
    lte_modulation_set_mcs_index_dl(QPSK, 0, 9);
    lte_modulation_set_mcs_index_dl(QAM16, 0, 16);
    lte_modulation_set_mcs_index_dl(QAM64, 0, 24);
    lte_modulation_set_mcs_index_dl(QPSK, 1, 9);
    lte_modulation_set_mcs_index_dl(QAM16, 1, 16);
    lte_modulation_set_mcs_index_dl(QAM64, 1, 24);


    lte_modulation_set_mcs_index_ul(QPSK, 9);
    lte_modulation_set_mcs_index_ul(QAM16, 20);
    lte_modulation_set_mcs_index_ul(QAM64, 24);

    testConfConfig.transmissionMode = SINGLEANT;
    MacMaxChanDescs = 39;

    testConfConfig.nPdsch[0] = 2;
    testConfConfig.pbchEna = 1; // for save old capability

    if (!pTestMgrCfg->configId)
        testConfConfig.DontUsePuschRxSduForHiAndDci0 = 1;

    testMgrCtx.boostingMap[0][0] = (U8**)E_TM_3_1MapIndex[BW_10_0_MHZ];
    testMgrCtx.boostingMap[0][1] = (U8**)E_TM_3_1MapIndex[BW_10_0_MHZ];

#endif

    switch(pTestMgrCfg->configId)
    {
        case TEST_MGR_NO_CONFIG:
            /* config should be sent by Host */
            break;
        case TEST_MGR_10MHZ_1ANT_RT:
            TestMgrSetRtConfig10MhzFdx(&testConfConfig);
            break;
        case TEST_MGR_10MHZ_1ANT_RT_MAX_THROUGHPUT: //2
            TestMgrSetRtConfig10MhzMaxThroughFdx(&testConfConfig);
            break;
        case TEST_MGR_10MHZ_1ANT_RT_NO_REPACK: //3
            TestMgrSetRtConfig10MhzNoRepackFdx(&testConfConfig);
            break;
        case TEST_MGR_20MHZ_1ANT_RT: //4
            TestMgrSetRtConfig20MhzFdx(&testConfConfig);
            break;
        case TEST_MGR_20MHZ_1ANT_RT_MAX_THROUGHPUT: //5
            TestMgrSetRtConfig20MhzMaxThroughFdx(&testConfConfig);
            break;
        case TEST_MGR_TM500: //6
            TestMgrSetTm500Config(&testConfConfig);
            break;
        case TEST_MGR_1_10MHz_2ANT: //7
            TestMgrSetRtConfig10Mhz2ANT(&testConfConfig);
            break;
        case TEST_MGR_1_20MHz_2ANT: //8
            //TestMgrSetRtConfig20Mhz2ANT(&testConfConfig); //LTE
            TestMgrSet_Diversity_20Mhz_2ANT_2Layers(&testConfConfig);
            break;
        case TEST_MGR_10MHZ_1ANT_RT_MAX_QAM16: //9
            TestMgrSetRtConfig10MhzQAM16MaxFdx(&testConfConfig);
            break;
        case TEST_MGR_10MHZ_1ANT_RT_QAM16_QAM64_2SDU: // 10
            TestMgrSetRtConfig10MhzQAM16_QAM64_2SDUFdx(&testConfConfig);
            break;
        case TEST_MGR_10MHZ_1ANT_RT_QAM64_25_50: // 11
            TestMgrSetRtConfig10MhzQAM64_25_50(&testConfConfig);
            break;
        case TEST_MGR_10MHz_2ANT_1CW: //12
            TestMgrSetRtConfigBCH_10MHz_2ANT(&testConfConfig);
            break;
        case TEST_MGR_20MHz_2ANT_BCH: //13
            TestMgrSet_BCH_20Mhz_2ANT(&testConfConfig);
            break;
        case TEST_MGR_20MHz_2ANT_BCH_PDCCH: //14
            TestMgrSet_BCH_PDCCH_20Mhz_2ANT(&testConfConfig);
            break;
        case TEST_MGR_20MHz_2ANT_SPMUX: //15
            TestMgrSet_SpaMux_20Mhz_2ANT_2Layers_2Cw(&testConfConfig);
            break;
        case TEST_MGR_20MHz_2ANT_DIVERSITY: //16
             TestMgrSet_Diversity_20Mhz_2ANT_2Layers(&testConfConfig);
             break;
        case TEST_HARQ: //17
            TestHARQ(&testConfConfig);
            break;
        case TEST_HARQ_OPTIMIZATION_QAM64: //18
             TestHarqOptimizationQam64(&testConfConfig);
             break;
        case TEST_HARQ_OPTIMIZATION_QAM16: //19
             TestHarqOptimizationQam16(&testConfConfig);
             break;
        case TEST_1TXHISDU: //20
            Test10MhzHI(&testConfConfig);
            break;
        case TEST_1TXDCIULSDU: //21
            Test10MhzDCI0(&testConfConfig);
            break;
        case TEST_4HISDU_R1TXDCIULSDU: //22
            Test10MhzHI_DCI0(&testConfConfig);
            break;
        case TEST_MGR_20MHz_2ANT_SPMUX_RT39: //23
            TestMgrSet_SpaMux_20Mhz_2ANT_2Layers_2Cw_RT39(&testConfConfig);
            break;
        case TEST_MGR_20MHz_2ANT_SPMUX_RT43: //24
            TestMgrSet_SpaMux_20Mhz_2ANT_2Layers_2Cw_RT43(&testConfConfig);
            break;
        case TEST_10MHz_XDL_XUL_HARQ_RT45://25
            TestMgrSet_10MHz_xDL_xUL_HARQ_RT45(&testConfConfig);
            break;
        case TEST_6UE_1ANT_1LAYER_10MHz_MAX_THROUGHPUT://26
            TestMgrSet_xUEs_xAnt_xLayers_xBw_MaxThrough(&testConfConfig, 6, 1, 1, BW_10_0_MHZ, 1);
            break;
        case TEST_6UE_1ANT_1LAYER_20MHz_MAX_THROUGHPUT://27
            TestMgrSet_xUEs_xAnt_xLayers_xBw_MaxThrough(&testConfConfig, 6, 1, 1, BW_20_0_MHZ, 1);
            break;
        case TEST_6UE_2ANT_2LAYER_10MHz_MAX_THROUGHPUT://28
            TestMgrSet_xUEs_xAnt_xLayers_xBw_MaxThrough(&testConfConfig, 6, 2, 2, BW_10_0_MHZ, 1);
            break;
        case TEST_6UE_2ANT_2LAYER_20MHz_MAX_THROUGHPUT://29
            TestMgrSet_xUEs_xAnt_xLayers_xBw_MaxThrough(&testConfConfig, 6, 2, 2, BW_20_0_MHZ, 1);
            break;
        case TEST_MGR_10MHz_2ANT_SPMUX: //31
            TestMgrSet_SpaMux_10Mhz_2ANT_2Layers_2Cw(&testConfConfig);
            break;
        case TEST_4UE_10MHz_1ANT_MAX_THROUGHPUT://32
            TestMgrSet_xUEs_xAnt_xLayers_xBw_MaxThrough(&testConfConfig, 4, 1, 1, BW_10_0_MHZ, 0);
            break;
        case TEST_4UE_10Mhz_1ANT_MAX_THROUGHPUT_1://34
            TestMgrSet_xUE_MaxThrough(&testConfConfig, test_cfg_4_ue_1);
            break;
        case TEST_4UE_10Mhz_1ANT_MAX_THROUGHPUT_2://35
            TestMgrSet_xUE_MaxThrough(&testConfConfig, test_cfg_4_ue_2);
            break;
        case TEST_4UE_10Mhz_1ANT_MAX_THROUGHPUT_3://36
            TestMgrSet_xUE_MaxThrough(&testConfConfig, test_cfg_4_ue_3);
            break;
        case TEST_4UE_10Mhz_1ANT_MAX_THROUGHPUT_4://37
            TestMgrSet_xUE_MaxThrough(&testConfConfig, test_cfg_4_ue_4);
            break;
        case TEST_3UE_10Mhz_1ANT_MAX_THROUGHPUT_1://38
            TestMgrSet_xUE_MaxThrough(&testConfConfig, test_cfg_3_ue_1);
            break;
        case TEST_3UE_10Mhz_1ANT_MAX_THROUGHPUT_2://39
            TestMgrSet_xUE_MaxThrough(&testConfConfig, test_cfg_3_ue_2);
            break;
        case TEST_3UE_10Mhz_1ANT_MAX_THROUGHPUT_3://40
            TestMgrSet_xUE_MaxThrough(&testConfConfig, test_cfg_3_ue_3);
            break;
        case TEST_3UE_10Mhz_1ANT_MAX_THROUGHPUT_4://41
            TestMgrSet_xUE_MaxThrough(&testConfConfig, test_cfg_3_ue_4);
            break;
        case TEST_2UE_10Mhz_1ANT_MAX_THROUGHPUT_1://42
            TestMgrSet_xUE_MaxThrough(&testConfConfig, test_cfg_2_ue_1);
            break;
        case TEST_2UE_10Mhz_1ANT_MAX_THROUGHPUT_2://43
            TestMgrSet_xUE_MaxThrough(&testConfConfig, test_cfg_2_ue_2);
            break;
        case TEST_2UE_10Mhz_1ANT_MAX_THROUGHPUT_3://44
            TestMgrSet_xUE_MaxThrough(&testConfConfig, test_cfg_2_ue_3);
            break;
        case TEST_2UE_10Mhz_1ANT_MAX_THROUGHPUT_4://45
            TestMgrSet_xUE_MaxThrough(&testConfConfig, test_cfg_2_ue_4);
            break;
        case TEST_8UE_10Mhz_2ANT_MAX_THROUGHPUT_1://46
            TestMgrSet_xUE_MaxThrough_configuring(&testConfConfig, test_cfg_8xpdsch_1xpusch_nomultri_2xformat1_2xformat1a_4xformat2_6cqi, 2, 2, BW_10_0_MHZ);
            break;
        case TEST_5UE_10Mhz_2ANT_MAX_THROUGHPUT_2://47
             TestMgrSet_xUE_MaxThrough_configuring(&testConfConfig, test_cfg_5xpdsch_1xpusch_multri_2xformat1_2xformat1a_4xformat2_13cqi, 2, 2, BW_10_0_MHZ);
            break;

        case TEST_4UE_10Mhz_2ANT_CQI_HARQ://48
             TestMgrSet_4UE_10Mhz_2ANT_CQI_HARQ(&testConfConfig, 2, BW_10_0_MHZ);
            break;
        case TEST_4UE_10Mhz_2ANT_HARQ://49
             TestMgrSet_4UE_10Mhz_2ANT_HARQ(&testConfConfig, 2, BW_10_0_MHZ);
            break;

        case TEST_1UE_10Mhz_1ANT_UDP_MPEG2://50
             TestMgrSet_4UE_10Mhz_1ANT_UDP_MPEG2(&testConfConfig);
            break;

        case TEST_xUE_10Mhz_1ANT_VAR_THROUGHPUT://52
             TestMgrSet_xUE_10MHz(&testConfConfig, pTestMgrCfg);
            break;
        case TEST_2Pdsch2AntSpatialMuxQAM64_2Pusch2AntQAM16_10MHZ: //53
            TestMgrSet_2UE_10MHz_1ANT_DLQ64_UL16(&testConfConfig);
            break;
        case TEST_3Pdsch2AntSpatialMuxQAM64_3Pusch2AntQAM16_10MHZ: //54
            TestMgrSet_3UE_10MHz_1ANT_DLQ64_UL16(&testConfConfig);
            break;
        case TEST_4Pdsch2AntSpatialMuxQAM64_4Pusch2AntQAM16_10MHZ: //55
            TestMgrSet_4UE_10MHz_1ANT_DLQ64_UL16(&testConfConfig);
            break;

        case TEST_MGR_1_10MHz_2TXANT_2RXANT: //57
            TestMgrSetRtConfig10Mhz2TxAnt2RxAnt(&testConfConfig); // fdx
            break;
        case TEST_MGR_1_10MHz_2ANT_HDX: //58
            TestMgrSetRtConfig10Mhz2ANTHdx(&testConfConfig); //hdx
            break;
        case TEST_MGR_1_10MHz_2TXANT_2RXANT_HDX: //59
            TestMgrSetRtConfig10Mhz2TxAnt2RxAntHdx(&testConfConfig); // hdx
            break;

        case TEST_ETM_LIB:
#ifdef MX_ARM_ENABLED //ETM Lib
            {
                U8 bw = BW_10_0_MHZ;
                U8 etm_id = E_TM_INT_1_1;
                U8 nums = get_mac_console_vals_num();
                PBS_STATIC_CONFIG pbs_static_config = get_static_config();

                if (nums > 1)
                {
                    etm_id = get_mac_console_vals(1);
                    if (etm_id > E_TM_INT_3_3)
                    {
                        uart_printf("WARNING!!!! etm_id outed of diapason\r\n");
                        etm_id = E_TM_INT_1_1;
                    }
                }
                
                if (nums > 2)
                {
                    bw = get_mac_console_vals(2);
                    if (bw >BW_20_0_MHZ)
                    {
                        uart_printf("WARNING!!!! bw outed of diapason\r\n");
                        bw = BW_10_0_MHZ;
                    }
                }

                uart_printf("init internal etm with etm_id = %d, bw = %d\r\n", etm_id, bw);

                etm_cfg.model = etm_id;
                etm_cfg.bw = bw;
                etm_cfg.ant0 = 1;
                etm_cfg.ant1 = 0;
                etm_cfg.num_layers = 1;
                etm_cfg.num_cw = 1;
                etm_cfg.e_rs = 0;
                etm_cfg.tdd = 0;
                etm_init(etm_cfg);
                pbs_static_config->use_internal_etm_test = 1;
                
            }
#endif

            break;

        default:
            printf("ERROR: Incorrect config Id [%s,%s:%d]\n",
                   __FUNCTION__, __FILE__, __LINE__);
            return FAILURE;
    }


    if (plte_pdsch_harq_state->ena_udp_stream)
        lte_pdcch_set_dci_alloc(testConfConfig.rntiPusch[0], 1, 1, GetMapSize(StaticConfig.bw_mode), (GetMapSize(StaticConfig.bw_mode)/6 + 7)>>3, StaticConfig.n_antenna_port, 0, 0, &plte_pdcch_chan_state->sCCE[0][0]); // calculate startCCE


    /* Set conformance test scheduler */
// TODO:
#ifndef MX_ARM_ENABLED
    if(pTestMgrCfg->configId != TEST_MGR_NO_CONFIG)
#endif
    {// do we need it here?
        pBsMacDesc->schedulingSchema = RCT_SCHEMA;
        LteBsMacDlSchedulerReconfig(pBsMacDesc);
        LteBsMacUlSchedulerReconfig(pBsMacDesc);
    }

    {// we're going to create real MIB if no SQV config, sure it overwrites above configured constants
        MIB_INFO* pMib = (MIB_INFO*)testConfConfig.payloadPbch;

        pMib->dl_Bandwidth = pBsMacDesc->initParm.channelBandwidth;
// TODO:
#ifdef MX_ARM_ENABLED
        pMib->phich_duration = PHICHDUR_NORMAL;
#else
        pMib->phich_duration = PHICHDUR_MIB_NORMAL;
#endif
        pMib->phich_resource = PHICHRES_ONE_SIXTH;
        pMib->spareHi = 0;
        pMib->spareLo = 0;
        pMib->systemFrameNumberHi = (getFrameNum() >> (2+6)) & 0x3;
        pMib->systemFrameNumberLo = (getFrameNum() >> 2) & 0x3F;
    }

    memset(testConfConfig.TimAdvCmdMap, 0, sizeof(testConfConfig.TimAdvCmdMap));
    if(pBsMacDesc->initParm.rxSduLineMonitorInfoenable)
    {// debug purposes
//        testConfConfig.TimAdvCmdMap[1] = 5; // 5 * 16Ts (80 samples)
    }

    {
        U32 i, j;
        for(j=0; j<3; j++)
        {
            testConfConfig.timAdvErrInfo[j].mSetDefaults = 1;
            testConfConfig.timAdvErrInfo[j].mErrAvgCh[0] = 1;
            testConfConfig.timAdvErrInfo[j].mErrAvgCh[1] = 0;
            testConfConfig.timAdvErrInfo[j].mErrExpo = -32768;
        }

        for(i=0; i<TIMERRBUFDEPTH; i++)
        {
            for(j=0; j<16; j++)
            {
                testConfConfig.timAdvErrInfoArrFSF[i][j].mSetDefaults = 1;
            }
            }
        }

// TODO:
#ifdef MX_ARM_ENABLED
/*    testConfConfig.ULEnabled = 0;
    if(testConfConfig.prachConfigIndex == 0xFF)
    {
        U32 idx;
        for(idx = 0; idx < MIN(sizeof(testConfConfig.numPucch), sizeof(testConfConfig.numPusch)); idx++)
            testConfConfig.ULEnabled += testConfConfig.numPucch[idx] + testConfConfig.numPusch[idx];
        if(!testConfConfig.ULEnabled)
             uart_printf(">>>>>>>>>>>>>>>>>>Downlink enabled; Uplink disabled %d\n",testConfConfig.ULEnabled);
    }
    else
        testConfConfig.ULEnabled = 1;*/
#endif

    return SUCCESS;
}

RESULTCODE TestMgrInitRt(PTESTMGRINIT pTestMgrCfg)
{
    PRINT_FUNCTION_NAME(__FUNCTION__);

    lte_modulation_set_mcs_index_dl(QPSK, 0, 9);
    lte_modulation_set_mcs_index_dl(QAM16, 0, 16);
    lte_modulation_set_mcs_index_dl(QAM64, 0, 28);
    lte_modulation_set_mcs_index_dl(QPSK, 1, 9);
    lte_modulation_set_mcs_index_dl(QAM16, 1, 16);
    lte_modulation_set_mcs_index_dl(QAM64, 1, 28);


    lte_modulation_set_mcs_index_ul(QPSK, 9);
    lte_modulation_set_mcs_index_ul(QAM16, 20);
    lte_modulation_set_mcs_index_ul(QAM64, 28);

    return SUCCESS;
}
/*
void Vpr(BS_CONFOR_TEST_CONFIG *pTestCfg)
{

    printf("%d \n",   pBsMacDesc->initParm.channelBandwidth);
    printf("%d \n",   pBsMacDesc->initParm.numberResourceBlocksperSlot);
    printf("%d \n",   pBsMacDesc->initParm.fftSize);
    printf("%d \n",   pBsMacDesc->initParm.numberUsedSubcarriers);
    printf("%d \n",   pBsMacDesc->initParm.nMaxDlRb);
    printf("%d \n",   pBsMacDesc->initParm.nMaxUlRb);

    printf("%d \n",   pTestCfg->nAntennaPort);
    printf("%d \n",   pTestCfg->nCw);
    printf("%d \n",   pTestCfg->nLayers);
    printf("%d \n",   pTestCfg->precodingMode);;
    printf("%d \n",   pTestCfg->duration );
    printf("%d \n",   pTestCfg->transmissionMode );

    printf("%d \n",   pTestCfg->rsBoosting );
    printf("%d \n",   pTestCfg->epreSinchro );
    printf("%d \n",   pTestCfg->reservedEpre );

    // PDSCH
    printf("%d \n",   pTestCfg->pbchEpre);
    printf("%d \n",   pTestCfg->pbchReservedEpre);
    // PCFICH
    printf("%d \n",   pTestCfg->nCtrlSymbols);
    printf("%d \n",   pTestCfg->nPhichGroup);
    printf("%d \n",   pTestCfg->nPhichPerGroup);
    printf("%d \n",   pTestCfg->phichSymPower );
    printf("%d \n",   pTestCfg->phichGroupEpre);

    // PDCCH/
    printf("%d \n",   pTestCfg->nAvalableRegs);
    printf("%d \n",   pTestCfg->nPdcch);
    printf("%d \n",   pTestCfg->nCcePerPdcch);
    printf("%d \n",   pTestCfg->nRegPerCce);;
    printf("%d \n",   pTestCfg->nRegAllToPdcch);
    printf("%d \n",   pTestCfg->nNilRegPadding);
    printf("%d \n",   pTestCfg->pdcchRegEpre );
    printf("%d \n",   pTestCfg->nilRegEpre );


    // PDSCH
    printf("%d \n",   pTestCfg->nPdschBoostedPrb );
    printf("%d \n",   pTestCfg->nPdschDeBoostedPrb);

    // PUSCH
    printf("%d \n",   pTestCfg->nPuschBoostedPrb);
    printf("%d \n",   pTestCfg->puschBoostedPrbPa);
    printf("%d \n",   pTestCfg->puschBoostedMod );

    printf("%d \n",   pTestCfg->rntiPusch[0]);

    printf("%d \n",   pTestCfg->rntiBoosted[0] );
    printf("%d \n",   pTestCfg->rntiDeBoosted[0]);

}*/

/*RESULTCODE TestFileCfgMgr(BS_CONFOR_TEST_CONFIG *pTestConfig)
{
    RESULTCODE ret = SUCCESS;
    PRINT_FUNCTION_NAME(__FUNCTION__);

    if (TestMgrValidateTestConfig(pTestConfig) != SUCCESS)
        return FAILURE;

    pBsMacDesc->schedulingSchema = RCT_SCHEMA;
//Vpr(pTestConfig);
    ret = LteBsMacDlSchedulerReconfig(pBsMacDesc);
    ret = LteBsMacUlSchedulerReconfig(pBsMacDesc);

    return ret;
}*/


RESULTCODE TestMgrClean(void)
{
    PRINT_FUNCTION_NAME(__FUNCTION__);

    return SUCCESS;
}

