/** @file lte_pusch.c
 *
 * @brief Defenitions and prototypes for processing PUSCH message & some service functions for mapinfo
 * @author Mindspeed Technologies
 * @version $Revision: 1.4 $
 *
 * COPYRIGHT(c) 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef LTE_PUSCH_H
#define LTE_PUSCH_H

RESULTCODE lte_pusch_update_cqi_pmi_ri_harq(PRX_CQIRIHI_STATUS_EVENT pmsg, PRXSTATUSIND prx_status_ind);


typedef RESULTCODE (*tTestMgrGetPusch)(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                       MAPPINGINFO *pMapInfo,
                                       MAPPINGINFO *pMapInfoStore);

typedef U8 (*tGetRvi)(BS_CONFOR_TEST_CONFIG *pTestConfConfig);


extern tTestMgrGetPusch pTestMgrGetPusch;

S16 lte_pusch_get_power(BS_CONFOR_TEST_CONFIG *pTestConfConfig);

RESULTCODE lte_pusch_set_mapinfo(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                 MAPPINGINFO *pMapInfo, 
                                 MAPPINGINFO *pMapInfoStore);

RESULTCODE lte_pusch_set_mapinfo_direct(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                        MAPPINGINFO *pMapInfo, 
                                        MAPPINGINFO *pMapInfoStore);

RESULTCODE lte_pusch_fill_sub_channel(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                      PULSUBCHINFO pSubCh,
                                      U8 modulation,
                                      U8 mcsType,
                                      MAPPINGINFO *pMapInfo,
                                      U8 idx);

U8 lte_pusch_conv_mcs_to_modulation(U8 Mcs);
U8 lte_pusch_get_modulation(BS_CONFOR_TEST_CONFIG *pTestConfConfig);

U8 lte_pusch_rvi(BS_CONFOR_TEST_CONFIG *pTestConfConfig);
U8 lte_pusch_rvi_default(BS_CONFOR_TEST_CONFIG *pTestConfConfig);

RESULTCODE lte_pusch_add(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                         PULCHANDESC pCh,
                         U32 id,
                         S16 power);



#endif //LTE_PUSCH_H

