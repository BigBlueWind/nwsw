/** @file lte_rnti.c
 *
 * @brief Defenitions and prototypes for getting rnti from differrnt part of TestConfConfig-structure
 * @author Mindspeed Technologies
 * @version $Revision: 1.2 $
 *
 * COPYRIGHT(c) 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef _LTE_RNTI_H
#define _LTE_RNTI_H

U16 lte_rnti_get_pusch_rnti(BS_CONFOR_TEST_CONFIG *pTestConfConfig, U8 idx);
U16 lte_rnti_get_boosted_rnti(BS_CONFOR_TEST_CONFIG *pTestConfConfig, U8 idx);
U16 lte_rnti_get_deboosted_rnti(BS_CONFOR_TEST_CONFIG *pTestConfConfig, U8 idx);

#endif //_LTE_RNTI_H
