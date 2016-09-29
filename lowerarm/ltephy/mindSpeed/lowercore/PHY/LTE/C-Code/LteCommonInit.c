//-------------------------------------------------------------------------------------------
/** @file LteCommonInit.c
 *
 * @brief This file implements common initialization functions 
 * between Lte BS PHY and MS PHY 
 * @author Mindspeed Technologies
 * @version $Revision: 1.15 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#include "typedef.h"
#include "ltephydef.h"
#include "ext_phy.h"
#include "prototypes.h"

#ifdef CEVA_INTRINSICS
#include <basic_op.h>
#else
#include "basic_op_cc.h"
#endif

//-------------------------------------------------------------------------------------------
/** @brief LteDetermineSymbolsPerSlot:  return number of synbols per slot
 *
 *  <1> Hardware Resource:
 *  ARM/Ceva
 *
 *  @param cpType - INPUT, 16bit signed number
 *  @param nRbSc  - INPUT, 16bit signed number
 
 *
 *  @return (numSymbols, 16bit signed number)
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction     |Length   |Type          |Data Format |Valid Range     |Units |
 *  |             |Standard |              |         |              |            |                |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |cpType       |         |Input         |1        |Real          |(1:15:0)    |Normal/Extemded |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |nRbSc        |         |Input         |1        |Real          |(1:15:0)    |12 or 24        |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |return       |         |Output        |1        |Real          |(1:15:0)    |3, 6 or 7       |      |
 *  ------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |LteDetermineSymbolsPerSlot           |
 *  ------------------------------------------------------------
 *  |Input Streams       |cpType, nRbSc                        |
 *  ------------------------------------------------------------
 *  |Output Streams      |return value                         |
 *  ------------------------------------------------------------
 *  |Config Parameters   |none                                 |
 *  ------------------------------------------------------------
 *  |Status Parameters   |none                                 |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  LteDetermineSymbolsPerSlot:  return number of synbols per slot given CPType and nRbSc
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
S16 LteDetermineSymbolsPerSlot(S16 cpType, S16 nRbSc)
{
    S16 ans;

    if (nRbSc == PRB_12)
    {
        if (cpType == NORMAL)
        {
            ans = SL_SEVEN;
        }
        else
        {
            ans = SL_SIX;
        }
    }
    else
    {
        ans = SL_THREE;
    }
    return (ans);
}

//-------------------------------------------------------------------------------------------
/** @brief LteDetermineCpsandSampling:  return length of cyclic prefix and sampling frequency
 *
 *  <1> Hardware Resource:
 *  ARM/Ceva
 *
 *  @param pInit   Pointer to PINITPARM parmeter init structure
 *  @param pPhy     Pointer to PLTEPHYDESC for this Phy Instance
 
 *
 *  @return (status)
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name        |Name in  |Direction     |Length   |Type                    |Data Format |Valid Range |Units |
 *  |            |Standard |              |         |                        |            |            |      |
 *  -----------------------------------------------------------------------------------------------------------
 *  |pInit       |         |Input         |1        |pStructure(PINITPARM)   |            |            |      |
 *  -----------------------------------------------------------------------------------------------------------
 *  |pPhy        |         |Input         |1        |pStructure(PLTEPHYDESC) |            |            |      |
 *  -----------------------------------------------------------------------------------------------------------
 *  |return      |         |Output        |1        |pStructure(pBs)         |            |            |
 *  -----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |LteDetermineCpsandSampling           |
 *  ------------------------------------------------------------
 *  |Input Streams       |pInit, pPhy                          |
 *  ------------------------------------------------------------
 *  |Output Streams      |pBs->Fs, pBs->Ncp, pBs->Ncp0         |
 *  ------------------------------------------------------------
 *  |Config Parameters   |none                                 |
 *  ------------------------------------------------------------
 *  |Status Parameters   |0 or 1                               |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  LteDetermineCpsandSampling:  return length of cyclic prefix and sampling frequency given
 *                               channel bandwidth and cyclic prefix type
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
U32 LteDetermineCpsandSampling(PINITPARM pInit, PLTEPHYDESC pPhy)
{
    U32 status = 0;
    PBS pBs = pPhy->pBs;

    pBs->Fs = SAMPLINGFREQ[pInit->channelBandwidth];
    if (pInit->cyclicPrefixType == NORMAL)
    {
        pBs->Ncp = CYCLICPRE_NRML[pInit->channelBandwidth];
        pBs->Ncp0 = CYCLICPRE0_NRML[pInit->channelBandwidth];
    }
    else
    {
        pBs->Ncp = CYCLICPRE_EXT[pInit->channelBandwidth];
        pBs->Ncp0 = CYCLICPRE0_EXT[pInit->channelBandwidth];
    }
    return (status);
}

//-------------------------------------------------------------------------------------------
/** @brief LteDeterminePrachSubcSpacing:  return PRACH SubCarrier Spacing
 *
 *  <1> Hardware Resource:
 *  ARM/Ceva
 *
 *  @param prachConfigurationIndex - INPUT, 16bit signed number
 
 *
 *  @return (prachSubcSpacing, 16bit signed number)
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------
 *  |Name                    |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |                        |Standard |              |         |              |            |            |      |
 *  ------------------------------------------------------------------------------------------------------------
 *  |prachConfigurationIndex |         |Input         |1        |Real          |(1:15:0)    |            |      |
 *  ------------------------------------------------------------------------------------------------------------
 *  |return                  |         |Output        |1        |Real          |(1:15:0)    |            |      |
 *  ------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |LteDeterminePrachSubcSpacing         |
 *  ------------------------------------------------------------
 *  |Input Streams       |prachConfigurationIndex              |
 *  ------------------------------------------------------------
 *  |Output Streams      |return value                         |
 *  ------------------------------------------------------------
 *  |Config Parameters   |none                                 |
 *  ------------------------------------------------------------
 *  |Status Parameters   |none                                 |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  LteDeterminePrachSubcSpacing:  return PRACH SubCarrier Spacing given prachConfigurationIndex
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
S16 LteDeterminePrachSubcSpacing(S16 prachConfigurationIndex)
{
    S16 prachPreambleFormat = 0;
    S16 prachSubcSpacing = 0;

    // 1) From prachConfigurationIndex determine prachPreambleFormat
    if (prachConfigurationIndex < 16)
    {
        prachPreambleFormat = 0;
    }
    else if ((prachConfigurationIndex >= 16) && (prachConfigurationIndex < 32))
    {
        prachPreambleFormat = 1;
    }
    else if ((prachConfigurationIndex >= 32) && (prachConfigurationIndex < 48))
    {
        prachPreambleFormat = 2;
    }
    else if ((prachConfigurationIndex >= 48) && (prachConfigurationIndex < 64))
    {
        prachPreambleFormat = 3;
    }

    // Then determine subc spacing
    if (prachPreambleFormat < 4)
    {
        prachSubcSpacing = 1250;
    }
    else
    {
        prachSubcSpacing = 7500;
    }

    return prachSubcSpacing;
}

