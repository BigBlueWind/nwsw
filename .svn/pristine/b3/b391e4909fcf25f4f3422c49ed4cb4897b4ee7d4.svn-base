/** @file lte_pdcch.h
 *
 * @brief Defenitions and prototypes for creating PDCCH message
 * @author Mindspeed Technologies
 * @version $Revision: 1.6 $
 *
 * COPYRIGHT(c) 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef LTE_PDCCH_H
#define LTE_PDCCH_H

typedef RESULTCODE (*tTestMgrGetPdcch)(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                       U8 pdcchIdx,
                                       MAPPINGINFO *pMapInfo);


int lte_pdcch_get_N_regs(int n_prb,     // max RB (50)
                         int ctrl,      // 1
                         int num_ant,   // numAnt
                         int cp);       // cycle-prefix, 0-normal, 1-extended

void lte_pdcch_set_dci_alloc(U16 rnti, 
                             U8 ctrl, 
                             U8 agreg_level, 
                             U8 nprb, 
                             int phich_ng, 
                             U8 nRs, 
                             U8 cp, 
                             U8 dci_id, 
                             U32 *scce);

RESULTCODE lte_pdcch_set_mapinfo(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                 U8 pdcch_idx,
                                 MAPPINGINFO *pmapinfo);
RESULTCODE lte_pdcch_get_mapinfo(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                 PDCICHANNELDESC pch,
                                 U32 ch_id,
                                 MAPPINGINFO *pmapinfo);
RESULTCODE lte_pdcch_get_mapinfo_from_cce(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                          U8 pdcchIdx,
                                          MAPPINGINFO *pMapInfo);
RESULTCODE lte_pdcch_get_mapinfo_direct(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                        U8 pdcchIdx,
                                        MAPPINGINFO *pMapInfo);



RESULTCODE lte_pdcch_update_dci(U32 max_cw, U32 max_ch_idx, U32 current_harq_tick);

RESULTCODE lte_pdcch_create_sdu(DCI_DATA_INFO* pDciInfo, 
                                U32 numBits, 
                                U8 chId, 
                                U8 channelType, 
                                U8* pDci);

RESULTCODE lte_add_pdcch(PDCICHANNELDESC pCh,
                         U32 id,
                         MAPPINGINFO *pMapInfo,
                         U32 dci_idx);

#endif //LTE_PDCCH_H

