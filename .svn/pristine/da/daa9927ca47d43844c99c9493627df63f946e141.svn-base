/** @file lte_rnti.c
 *
 * @brief Functions for getting rnti from differrnt part of TestConfConfig-structure
 * @author Mindspeed Technologies
 * @version $Revision: 1.4 $
 *
 * COPYRIGHT(c) 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */
#include "resultcodes.h"

#include "LteMacTestCfgMgr.h"

/* Get RNTI for PUSCH
 *
 * @param pTestConfConfig Pointer to pTestConfConfig
 * @param idx channel index
 * @return RNTI value
 */
U16 lte_rnti_get_pusch_rnti(BS_CONFOR_TEST_CONFIG *pTestConfConfig, U8 idx)
{
    PRINT_FUNCTION_NAME(__FUNCTION__);
    return pTestConfConfig->rntiPusch[idx];
}


/* Get RNTI for PDSCH (Boosted)
 *
 * @param pTestConfConfig Pointer to pTestConfConfig
 * @param idx channel index
 * @return RNTI value
 */
U16 lte_rnti_get_boosted_rnti(BS_CONFOR_TEST_CONFIG *pTestConfConfig, U8 idx)
{
    PRINT_FUNCTION_NAME(__FUNCTION__);
    return pTestConfConfig->rntiBoosted[idx];
}


/* Get RNTI for PDSCH (deBoosted)
 *
 * @param pTestConfConfig Pointer to pTestConfConfig
 * @param idx channel index
 * @return RNTI value
 */
U16 lte_rnti_get_deboosted_rnti(BS_CONFOR_TEST_CONFIG *pTestConfConfig, U8 idx)
{
    PRINT_FUNCTION_NAME(__FUNCTION__);
    return pTestConfConfig->rntiDeBoosted[idx];
}

