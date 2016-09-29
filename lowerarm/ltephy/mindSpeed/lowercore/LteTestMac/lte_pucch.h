/** @file lte_pucch.c
 *
 * @brief Defenitions and prototypes for creating PUCCH message
 * @author Mindspeed Technologies
 * @version $Revision: 1.3 $
 *
 * COPYRIGHT(c) 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef LTE_PUCCH_H
#define LTE_PUCCH_H

typedef RESULTCODE (*tTestMgrGetPucch)(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                       MAPPINGINFO *pMapInfo,
                                       U32 id);

RESULTCODE lte_pucch_set_mapinfo(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                 MAPPINGINFO *pMapInfo, 
                                 U32 id);

RESULTCODE lte_pucch_set_mapinfo_direct(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                        MAPPINGINFO *pMapInfo, 
                                        U32 id);

RESULTCODE lte_pucch_update_cqi_harq(PRXSDUIND prx_sdu_ind);

RESULTCODE lte_pucch_add(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                         PULCTRLCHDESC pCh,
                         U32 id,
                         MAPPINGINFO *pMapInfo,
                         U32 idx);


#endif //LTE_PUCCH_H

