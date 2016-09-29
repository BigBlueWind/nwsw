/** @file lte_pdsch.h
 *
 * @brief Defenitions and prototypes for creating PDSCH message
 * @author Mindspeed Technologies
 * @version $Revision: 1.6 $
 *
 * COPYRIGHT(c) 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef LTE_PDSCH_H
#define LTE_PDSCH_H

typedef RESULTCODE (*tTestMgrGetPdschMap)(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                          MAPPINGINFO *pMapInfo, 
                                          U16 ChanId, 
                                          U8 LayerIdx); 

typedef U32        (*lte_pdsch_set_payload_cb)(U8 *buf, U32 num);


PLTE_PDSCH_CHAN_STATE      get_lte_pdsch_chan_state(void);
PLTE_PDSCH_CHAN_DESC       get_lte_pdsch_chan_desc(U32 chan_id);

U8  lte_pdsch_calc_mcs_via_cqi(U8 cqi);

U32 lte_pdsch_copy_payload_from_udp(U8 *buf, U32 num);
U32 lte_pdsch_set_payload(U8 *buf, U32 num);

U32 lte_pdsch_get_tb_size(U32 mcsIndex, U32 nbRB, U8 TxMode);
U32 lte_pdsch_get_sdu_size(PDLSUBCHINFO pSubCh, MAPPINGINFO *pMapInfo, U8 TxMode);

RESULTCODE lte_pdsch_set_mapinfo_boosted(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                         MAPPINGINFO *pMapInfo, 
                                         U16 ChanId, 
                                         U8 LayerIdx);

RESULTCODE lte_pdsch_set_mapinfo_boosted_ex(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                            MAPPINGINFO *pMapInfo, 
                                            U16 ChanId, 
                                            U8 LayerIdx);

RESULTCODE lte_pdsch_set_mapinfo_boosted_direct(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                                MAPPINGINFO *pMapInfo, 
                                                U16 ChanId, 
                                                U8 LayerIdx);

RESULTCODE lte_pdsch_set_mapinfo_deboosted(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                           MAPPINGINFO *pMapInfo, 
                                           U16 Pdsch_idx, 
                                           U8 LayerIdx);


RESULTCODE lte_pdsch_create(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                            PDLSUBFRDESC pDlSubFrame,
                            PMAPPINGINFO pmapInfo,
                            U8 *chanIdx,
                            U8 Pdsch_idx,
                            U8 boosted);

RESULTCODE lte_pdsch_add(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                         PDLCHANDESC pCh,
                         U32 id,
                         U32 pdsch_idx,
                         MAPPINGINFO *pMapInfo,
                         U16 rnti,
                         S16 power);


#endif //LTE_PDSCH_H

