/** @file lte_pbch.c
 *
 * @brief Defines and prototypes for static configuration enb
 * @author Mindspeed Technologies
 * @version $Revision: 1.11 $
 *
 * COPYRIGHT(c) 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef _BS_STATIC_CONFIG_H
#define _BS_STATIC_CONFIG_H

#include "typedef.h"
#include "LtePhyL2Api.h"

typedef struct
{
    U8 format0;
    U8 format1;
    U8 format1a;
    U8 format1b;
    U8 format1c;
    U8 format1d;
    U8 format2;
    U8 format2a;
    U8 format2b;
    U8 format3;
    U8 format3a;
} DCI_SIZE_t;

// eNodeB static configuration.
// Part of it assigned manually during eNodeB configuration.
// The rest of it calculated automatically during eNodeB startup (based on info provided during eNodeB configuration).

typedef struct BsStaticConfig
{

    U8 n_antenna_port;    //!< number of antena ports (1, 2, 4)
    U8 n_antenna_port_rx; //!< number of antena ports for rx (1, 2, 4)

    U8 tdd;               //!< TDD=1, FDD=0
    U8 tdd_configuration; //!< 0..6 - Table 7-1: Maximum number of DL HARQ processes for TDD in the 36.213-930
    U8 cp;                //!< cyclip prefix (CP_NORMAL or CP_EXTENDED)
    U8 bw_mode;           //!< 5Mhz, 10Mhz, 20Mhz

// Copy each tti to DLCommonTxPowerControl struct of TxVector
    DLCMNTXPWRCTL   dl_cmn_pwr_ctl;  //!< structure of power values


//----------------------------------------------------------------------------------------------
//                      Parameters below to be auto-calculated, don't set them manually
//----------------------------------------------------------------------------------------------

    U8  n_step_rb;
    U8  n_dl_rb;
    U8  n_ul_rb;
    U8  n_dl_vrb;

    U8  P;                  //!< RBG Size. Table 7.1.6.1-1: Type 0 Resource Allocation RBG Size vs. Downlink System Bandwidth in the 36.213-930

    U8  size_rba_pusch;
    U8  size_rba_pdsch;
    U8  size_rba_vrb;  
    U8  size_rbg_pusch;
    U8  n_dl_vrb_gap1;
    U8  distributed_flag;
    U8  n_gap_flag;

    // PRS
    U8 prs_num_rb;          //!< Bandwidth available for prs generation (Always smaller than system bandwidth (1,..100)    
    S16 prs_ra;             //!< prs Signal Power ratio relative to Cell Specific reference signals power (db)
    U16 i_prs;              //!< 
    U8 prs_enable;          //!< 0 no positioning reference signals are generated in this subframe, 1 prs is generated in this subframe

    DCI_SIZE_t DCI_size;    //!< different format sizes for DCI payloads

    // PHICH
    U8  six_ng;
    U8  n_phich_group;
    U8  two_nsf_phich;

    U32 phy_instance_id;
    U32 phy_on_linux_rt;

    U32 use_internal_etm_test;
} BS_STATIC_CONFIG, *PBS_STATIC_CONFIG;

PBS_STATIC_CONFIG clr_static_config(void);
PBS_STATIC_CONFIG get_static_config(void);

void calc_nGPhich(BS_STATIC_CONFIG *pTestCfg);
void initBS(BS_STATIC_CONFIG *pBsStaticConfig);

U32 GetMapSize(U32 bw);
U32 GetBwSize(U32 n_rbs);
U32 calc_rbg(U32 l_crb, U32 start_rb_index, U8 P, U8 len);
U8  get_n_step_rb(U8 dl_rbs);
U8  get_P(U8 n_dl_rb);
U8  get_gap_1(U8 dl_rbs);

RESULTCODE bs_static_config_update_10mhz(PINITPARM pParm);
RESULTCODE bs_static_config_update_20mhz(PINITPARM pParm);

#endif //_BS_STATIC_CONFIG_H
