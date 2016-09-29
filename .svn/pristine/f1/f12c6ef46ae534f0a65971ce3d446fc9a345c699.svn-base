/** @file lte_modulation.c
 *
 * @brief Implemenataion for set/get modulation
 * @author Mindspeed Technologies
 * @version $Revision: 1.1 $
 *
 * COPYRIGHT(c) 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#include "LtePhyL2Api.h"
#include "resultcodes.h"
#include "LteMacTestCfgMgr.h"
#include "bsmac.h"

#include "basetypes.h"

#include "lte_mac_typedef.h"

U8 MCSIndex_TableDl[4][2] = {0};
U8 MCSIndex_TableUl[4] = {0};


/* Function set mcs type for current modulation value for uplink
 *
 * @param modulation
 * @param mcs
 * @return Standard result code
 */
U8 lte_modulation_set_mcs_index_ul(U16 modulation, U8 value)
{
    if(modulation>=QPSK && modulation<=QAM64)
    {
        MCSIndex_TableUl[modulation] = value;
        return SUCCESS;
    }
    else
        return 0xFF;
}

/* Function get mcs type for current modulation value for uplink
 *
 * @param modulation
 * @return mcs type
 */
U8 lte_modulation_get_mcs_index_ul(U16 modulation)
{
    PRINT_FUNCTION_NAME(__FUNCTION__);

    if(modulation>=QPSK && modulation<=QAM64)
        return MCSIndex_TableUl[modulation];

    printf("Error in funtion %s (%s:%d)\n",
            __FUNCTION__, __FILE__, __LINE__);
    return 0xFF;
}

//-----------------------------------------------------------------------------------------------------
/* Function set mcs type for current modulation value for downlink
 *
 * @param modulation
 * @param code word index
 * @param mcs
 * @return Standard result code
 */
U8 lte_modulation_set_mcs_index_dl(U16 modulation, U8 cwId, U8 value)
{
    if(modulation>=QPSK && modulation<=QAM64)
    {
        MCSIndex_TableDl[modulation][cwId] = value;
        return SUCCESS;
    }
    else
        return 0xFF;
}


/* Function get mcs type for current modulation value for uplink
 *
 * @param modulation
 * @param code word index
 * @return mcs type
 */
U8 lte_modulation_get_mcs_index_dl(U16 modulation, U16 cwId)
{
    PRINT_FUNCTION_NAME(__FUNCTION__);

    if(modulation>=QPSK && modulation<=QAM64)
        return MCSIndex_TableDl[modulation][cwId];

    printf("Error in funtion %s (%s:%d)\n",
            __FUNCTION__, __FILE__, __LINE__);
    return 0xFF;
}


