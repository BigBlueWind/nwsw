//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL clkrst.c
 *
 * @brief Clock and reset control block
 * @author Mindspeed Technologies
 * @version $Revision: 1.7 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include <stdio.h>
#include "clkrst.h"
#include "hal.h"
#include "mmumgr.h"

/*******************************************************************************
*                  Local functions                                             *
*******************************************************************************/

INLINE UINT32 GetRefClock (void)
{
    if (REG32(GP_STAT) & 1)
        return CLK_RST_REF_FREQ;

    return CLK_RST_ALT_FREQ;
}

UINT32 GetPllFreq (UINT32 nPllID)
{
    UINT32 nCtrl = REG32(PLL_CNTRL(nPllID));
    UINT64 nFrq;
    UINT32 nVal;

    // if in reset or in PowerDown mode
    if ((nCtrl & (1 << 0)) || (nCtrl & (1 << 3)))
        return 0;

    nFrq = GetRefClock();

    nVal = REG32(PLL_NF_0 (nPllID)) & 0xFF;
    nVal |= (REG32(PLL_NF_1 (nPllID)) & 0x1F) << 8;
    nVal += 1;

    nFrq *= nVal;
    nFrq /= (REG32(PLL_NR (nPllID)) & 0x3F) + 1;
    nFrq /= (REG32(PLL_OD (nPllID)) & 0xF) + 1;

    // if PLL is in TEST MODE
    if (nCtrl & (1 << 7))
    {
        nFrq /= 2;

        return (UINT32)nFrq;
    }

    // if bypass mode
    if (nCtrl & (1 << 4))
        return GetRefClock();

    // in this case the PLL output freq. = REF * (NF + 1) / (NR + 1) / (OD + 1)

    return (UINT32)nFrq;
}

/** @brief This function checks PLL_reset and PLL_powerdown modes

    @param nDevID [in] - the PLL id

    @return [BOOL]*/

BOOL IsPllInReset (UINT32 nDevID)
{
    UINT32 nCtrl;

    nCtrl = REG32(PLL_CNTRL(nDevID));

    if (nCtrl & (1 << 0) || nCtrl & (1 << 3))
        return TRUE;

    return FALSE;
}


/** @brief This function returns an AXI block frequency

    @param nID [in] - see xxx_AXI

    @return [UINT32] frequency or 0 */

UINT32 GetAXIFreq (UINT32 nID, PUINT32 pPllID)
{
    UINT32 nCtrl, nSrcPLL, PLLFreq, nDiv;

    nCtrl = REG32(AXI_CLK_CNTRL_0);

    // =============================================
    //              SYSTEM AXI CLOCK
    // =============================================

    if (nID == SYS_AXI)
    {
        // if ref clock disabled

        if ((nCtrl & 1) == 0)
            return 0;

        nSrcPLL = (nCtrl >> 1) & 7;

        if (nSrcPLL == 0)
        {
            PLLFreq = GetRefClock ();

            if (pPllID != NULL)
                *pPllID = CLK_RST_REF_PLL;
        }
        else
        {
            PLLFreq = GetPllFreq ( nSrcPLL - 1 );

            if (pPllID != NULL)
                *pPllID = nSrcPLL - 1;
        }

        // if divider is not used, just to bypassed a freq

        if (REG32(AXI_CLK_DIV_CNTRL_2) & 1)
        {
            nDiv = 1;
        }
        else
        {
            nDiv = REG32(AXI_CLK_DIV_CNTRL_0) & 0xF;
        }

        return PLLFreq / nDiv;
    }

    // =============================================
    //              XP AXI CLOCK
    // =============================================

    if (nID == XP_AXI)
    {
        // if ref clock disabled

        if ((nCtrl & (1 << 4)) == 0)
            return 0;

        nSrcPLL = (nCtrl >> 5) & 7;

        if (nSrcPLL == 0)
        {
            PLLFreq = GetRefClock ();

            if (pPllID != NULL)
                *pPllID = CLK_RST_REF_PLL;
        }
        else
        {
            PLLFreq = GetPllFreq ( nSrcPLL - 1 );

            if (pPllID != NULL)
                *pPllID = nSrcPLL - 1;
        }

        nDiv = 1;

        // if divider is not used, just to bypassed a freq

        if (REG32(AXI_CLK_DIV_CNTRL_2) & (1 << 5))
        {
            // XP AXI Clock = Reference Clock divided by 2
            nDiv = 2;
        }
        else
        {
            nDiv = (REG32(AXI_CLK_DIV_CNTRL_0) >> 4) & 0xF;
        }

        return PLLFreq / nDiv;
    }



    nCtrl = REG32(AXI_CLK_CNTRL_1);

    // =============================================
    //              SPU AXI CLOCK
    // =============================================

    if (nID == SPU_AXI)
    {
        // if ref clock disabled

        if ((nCtrl & (1 << 4)) == 0)
            return 0;

        nSrcPLL = (nCtrl >> 5) & 7;

        if (nSrcPLL == 0)
        {
            PLLFreq = GetRefClock ();

            if (pPllID != NULL)
                *pPllID = CLK_RST_REF_PLL;
        }
        else
        {
            PLLFreq = GetPllFreq ( nSrcPLL - 1 );

            if (pPllID != NULL)
                *pPllID = nSrcPLL - 1;
        }

        // if divider is not used, just to bypassed a freq

        if (REG32(AXI_CLK_DIV_CNTRL_2) & (1 << 3))
        {
            // Clock divider output is bypassed. (Clock Out = Clock In)
            nDiv = 1;
        }
        else
        {
            nDiv = (REG32(AXI_CLK_DIV_CNTRL_1) >> 4) & 0xF;
        }

        return PLLFreq / nDiv;
    }

    // =============================================
    //              RAD AXI CLOCK
    // =============================================

    if (nID == RAD_AXI)
    {
        // if ref clock disabled

        if ((nCtrl & (1 << 0)) == 0)
            return 0;

        nSrcPLL = (nCtrl >> 1) & 7;

        if (nSrcPLL == 0)
        {
            PLLFreq = GetRefClock ();

            if (pPllID != NULL)
                *pPllID = CLK_RST_REF_PLL;
        }
        else
        {
            PLLFreq = GetPllFreq ( nSrcPLL - 1 );

            if (pPllID != NULL)
                *pPllID = nSrcPLL - 1;
        }

        // if divider is not used, just to bypassed a freq

        if (REG32(AXI_CLK_DIV_CNTRL_2) & (1 << 2))
        {
            // Clock divider output is bypassed. (Clock Out = Clock In)
            nDiv = 1;
        }
        else
        {
            nDiv = (REG32(AXI_CLK_DIV_CNTRL_1)) & 0xF;
        }

        return PLLFreq / nDiv;
    }

    return 0;
}

/** @brief This function checks AXI reset mode

    @param nDevID [in] - the AXI device id

    @return [BOOL]*/

BOOL IsAXIInReset (UINT32 nDevID)
{
    UINT32 nCtrl;

    nCtrl = REG32 (SYS_AXI_RESET_1);

    if (nDevID == RAD_AXI)
    {
        return (nCtrl & (1 << 7));
    }
    else if (nDevID == SPU_AXI)
    {
        return (nCtrl & (1 << 5));
    }
    else if (nDevID == XP_AXI)
    {
        return (nCtrl & (1 << 6));
    }

    return FALSE;
}

/** @brief This function returns a frequency of SPAcc device or 0 if
           the device is not clocked

    @return [UINT32] */

UINT32 GetSpaccFreq (PUINT32 pPllID)
{
    UINT32 nCtrl;

    nCtrl = REG32(SYS_AXI_CLK_CNTRL_0);

    // global clock enable

    if ((nCtrl & 1) == 0)
        return 0;

    // local clock enable

    if ((nCtrl & (1 << 6)) == 0)
        return 0;

    return GetAXIFreq (SYS_AXI, pPllID);
}

/** @brief This function checks SPAcc reset mode

    @return [BOOL] */

BOOL IsSpaccInReset (void)
{
    UINT32 nCtrl;

    nCtrl = REG32(SYS_AXI_RESET_0);

    if (nCtrl & 1)
        return TRUE;

    return (nCtrl & (1 << 6));
}

/** @brief This function returs a frequency of ARMx4 procesor or 0
           if frequency is not enabled

    @return [UINT32] */

UINT32 GetArmx4Freq (UINT32 nDevID, PUINT32 pPllID)
{
    UINT32 nCtrl, nFreq, nDiv;

    nCtrl = REG32(SYS_A9QP_CORE_CLK_CNTRL);

    if ((nCtrl & 1) == 0)
        return 0;

    if (!(nCtrl & (1 << 4 + nDevID)))
        return 0;

    nCtrl = REG32(ARMQP_CLK_CNTRL);

    if ((nCtrl & 1) == 0)
        return 0;

    nCtrl = (nCtrl >> 1) & 0x7;

    if (nCtrl == 0 )
    {
        nFreq = GetRefClock ();

        if (pPllID!=NULL)
            *pPllID = CLK_RST_REF_PLL;
    }
    else
    {
        nFreq = GetPllFreq (nCtrl - 1);

        if (pPllID!=NULL)
            *pPllID = nCtrl - 1;
    }

    nDiv = REG32 (ARMQP_CLK_DIV_CNTRL);

    // if bypass

    if (nDiv & (1 << 7))
        return nFreq;


    if ((nDiv & 0x1F) == 0)
        return 0;

    return nFreq / (nDiv & 0x1F);
}

/** @brief This function returns reset status for ARMx4cores

    @param nDevID [in] - ARMx4 core ID

    @return [BOOL] */

BOOL IsArmx4InReset (UINT32 nDevID)
{
    UINT32 nCtrl;

    nCtrl = REG32(SYS_A9QP_RESET_0);

    if (nCtrl & 1)
        return TRUE;

    if (nCtrl & (1 << (4 + nDevID)))
        return TRUE;

    return FALSE;
}

/** @brief This function returs a frequency of ARMx2 procesor or 0
           if frequency is not enabled

    @return [UINT32] */

UINT32 GetArmx2Freq (UINT32 nDevID, PUINT32 pPllID)
{
    UINT32 nCtrl, nFreq, nDiv;

    nCtrl = REG32(SYS_A9DP_CORE_CLK_CNTRL);

    if ((nCtrl & 1) == 0)
        return 0;

    if (!(nCtrl & (1 << 4 + nDevID)))
        return 0;

    nCtrl = REG32(ARMDP_CLK_CNTRL);

    if ((nCtrl & 1) == 0)
        return 0;

    nCtrl = (nCtrl >> 1) & 0x7;

    if (nCtrl == 0 )
    {
        nFreq = GetRefClock ();
        
        if (pPllID!=NULL)
            *pPllID = CLK_RST_REF_PLL;
    }
    else
    {
        nFreq = GetPllFreq (nCtrl - 1);

        if (pPllID!=NULL)
            *pPllID = nCtrl - 1;
    }

    nDiv = REG32 (ARMDP_CLK_DIV_CNTRL);

    // if bypass

    if (nDiv & (1 << 7))
        return nFreq;


    if ((nDiv & 0x1F) == 0)
        return 0;

    return nFreq / (nDiv & 0x1F);
}

/** @brief This function returns reset status for ARMx4cores

    @param nDevID [in] - ARMx2 core ID

    @return [BOOL] */

BOOL IsArmx2InReset (UINT32 nDevID)
{
    UINT32 nCtrl;

    nCtrl = REG32(SYS_A9DP_RESET_0);

    if (nCtrl & 1)
        return TRUE;

    if (nCtrl & (1 << (4 + nDevID)))
        return TRUE;

    return FALSE;
}

UINT32 GetPCIeFreq (PUINT32 pPllID)
{
    UINT32 Val;

    Val = REG32 (RAD_PCIE_CLK_CNTRL);

    if ((Val & 1) == 0)
        return 0;

    if ((Val & (1 << 4)) == 0)
        return 0;

    Val = REG32 (RAD_CLUSTER_CNTRL);

    if (Val & (1 << 2) == 0)
        return 0;

    Val = REG32 (RAD_AXI_CLK_CNTRL_0);

    if (Val & (1 << 0) == 0)
        return 0;

    // Clock branch

    Val = REG32 (RAD_AXI_CLK_CNTRL_1);

    if (Val & (1 << 0) == 0)
        return 0;

    return GetAXIFreq (RAD_AXI, pPllID);
}

BOOL IsPCIeInReset (void)
{
    // Clock branch

    UINT32 Val = REG32 (RAD_AXI_RESET_0);

    if (Val & (1 << 0))
        return TRUE;

    Val = REG32 (RAD_AXI_RESET_1);

    if (Val & (1 << 0))
        return TRUE;

    return REG32 (RAD_PCIE_RESET) & 1;
}

/** @brief This function returns a CEVA frequency or 0 if the CEVA in reset mode

    @param nCevaID [in] - the CEVA id [0 .. 9]

    @return [UINT32] frequency or 0 */

UINT32 GetCevaFreq (UINT32 nCevaID, PUINT32 pPllID)
{
    UINT32 nCtrl, nFreq, nDiv;

    // Disables/Enables the clock domain (Global and Block Clock Branches)
    if (REG32(CEVA_CLK_CNTRL_0) & 1 == 0)
        return 0;

    // SPU cluster
    // to check the clock enable for the CEVA device

    if (nCevaID < 4)
    {
        if ( !((REG32(CEVA_CLK_CNTRL_0) >> 4) & (1 << nCevaID) ) )
            return 0;
    }
    else
    {
        if ( !(REG32(CEVA_CLK_CNTRL_1) & (1 << (nCevaID - 4)) ) )
            return 0;
    }

    // to get the source clock for the CEVA

    nCtrl = REG32(CEVA_CLK_CNTRL_0);

    // if SPU_AXI clock is a source clock
    if (nCtrl & (1 << 1))
    {
        // The clock domain is turned off
        // the source clock is derived from  CEVA-AXI block

        if (REG32(CV_AXI_CLK_CNTRL_0) & 1 == 0)
            return 0;

        if (nCevaID < 4)
        {
            if ((REG32(CV_AXI_CLK_CNTRL_0) >> 4) & (1 << nCevaID) == 0)
                return 0;
        }
        else
        {
            if ((REG32(CV_AXI_CLK_CNTRL_1)) & (1 << (nCevaID - 4)) == 0)
                return 0;
        }

        nFreq = GetAXIFreq (SPU_AXI, pPllID);

        if (REG32(CV_AXI_CLK_DIV_CNTRL) & 1 << 7)
        {
            nDiv = 1;
        }
        else
        {
            nDiv = REG32(CV_AXI_CLK_DIV_CNTRL) & 0x1F;
        }

    }
    // if CEVA clock is used (TOP clock register map is used )
    else
    {
        // TOP CLOCK

        nCtrl = REG32(CEVA_CLK_CNTRL);

        // Disbales/Enables the clock domain
        if ((nCtrl & 1) == 0)
            return 0;

        if (((nCtrl >> 1) & 0x7) == 0)
        {
            nFreq = GetRefClock ();

            if (pPllID!=NULL)
                *pPllID = CLK_RST_REF_PLL;
        }
        else
        {
            nFreq = GetPllFreq ( ((nCtrl >> 1) & 0x7) - 1 );

            if (pPllID!=NULL)
                *pPllID = (((nCtrl >> 1) & 0x7) - 1);
        }

        if (REG32(CEVA_CLK_DIV_CNTRL) & (1 << 7))
        {
            nDiv = 1;
        }
        else
        {
            nDiv = REG32(CEVA_CLK_DIV_CNTRL) & 0x1F;
        }
    }

    if (!nDiv)
        return 0;

    return nFreq / nDiv;
}

/** @brief This function checks CEVA_reset mode

    @param nDevID [in] - the CEVA id

    @return [BOOL]*/

BOOL IsCevaInReset (UINT32 nDevID)
{
    UINT32 nCtrl;

    nCtrl = REG32(CEVA_RESET_0);

    // if global reset is set

    if (nCtrl & 1)
        return TRUE;

    if (nDevID < 4)
    {
        if ((nCtrl >> 4) & (1 << nDevID))
            return TRUE;
    }
    else
    {
        nCtrl = REG32(CEVA_RESET_1);

        if (nCtrl & (1 << (nDevID - 4)))
            return TRUE;
    }

    return FALSE;
}

/** @brief This function returns FFT frequency or 0 if the FFT in reset mode

    @param nFftID [in] - the FFT id [0 .. 9]

    @return [UINT32] frequency or 0 */

UINT32 GetFFTFreq (UINT32 nFftID, PUINT32 pPllID)
{
    UINT32 nCtrl, nFreq, nDiv;

    // Disables/Enables the clock domain (Global and Block Clock Branches)
    if (REG32(FFT_CLK_CNTRL_0) & 1 == 0)
        return 0;

    // SPU cluster
    // to check the clock enable for the FFT device

    if (nFftID < 4)
    {
        if ( !((REG32(FFT_CLK_CNTRL_0) >> 4) & (1 << nFftID) ) )
            return 0;
    }
    else
    {
        if ( !(REG32(FFT_CLK_CNTRL_1) & (1 << (nFftID - 4)) ) )
            return 0;
    }

    // to get the source clock for the FFT

    nCtrl = REG32(FFT_CLK_CNTRL_0);

    // if SPU_AXI clock is a source clock
    if (nCtrl & (1 << 1))
    {
        nFreq = GetAXIFreq (SPU_AXI, pPllID);

        if (REG32(FFT_CLK_DIV_CNTRL) & (1 << 7))
        {
            nDiv = 1;
        }
        else
        {
            nDiv = REG32(FFT_CLK_DIV_CNTRL) & 0x1F;
        }

    }
    // if FFT clock is used (TOP clock register map is used )
    else
    {
        // TOP CLOCK

        nCtrl = REG32(FFT_CLK_CNTRL);

        // Disbales/Enables the clock domain
        if ((nCtrl & 1) == 0)
            return 0;

        if (((nCtrl >> 1) & 0x7) == 0)
        {
            nFreq = GetRefClock ();

            if (pPllID!=NULL)
                *pPllID = CLK_RST_REF_PLL;
        }
        else
        {
            nFreq = GetPllFreq ( ((nCtrl >> 1) & 0x7) - 1 );

            if (pPllID!=NULL)
                *pPllID = (((nCtrl >> 1) & 0x7) - 1);
        }

        if (REG32(FFT_CLK_DIV_CNTRL) & (1 << 7))
        {
            nDiv = 1;
        }
        else
        {
            nDiv = REG32(FFT_CLK_DIV_CNTRL) & 0x1F;
        }
    }

    if (!nDiv)
        return 0;

    return nFreq / nDiv;
}

/** @brief This function checks FFT_reset mode

    @param nDevID [in] - the FFT id

    @return [BOOL]*/

BOOL IsFFTInReset (UINT32 nDevID)
{
    UINT32 nCtrl;

    nCtrl = REG32(FFT_RESET_0);

    // if global reset is set

    if (nCtrl & 1)
        return TRUE;

    if (nDevID < 4)
    {
        if ((nCtrl >> 4) & (1 << nDevID))
            return TRUE;
    }
    else
    {
        nCtrl = REG32(FFT_RESET_1);

        if (nCtrl & (1 << (nDevID - 4)))
            return TRUE;
    }

    return FALSE;
}

/** @brief This function returns FEC (UL/DL) frequency or 0 if the FEC in reset mode

    @param nDevID [in] - the FEC dev id

    @return [UINT32] frequency or 0 */

UINT32 GetFECFreq (UINT32 nDevID, PUINT32 pPllID)
{
    UINT32 nCtrl, nClkDiv, nFreq;

    if (nDevID == CR_DEV_FEC_DL)
    {
        nCtrl   = REG32(FEC_DL_CLK_CNTRL);
        nClkDiv = REG32(FEC_DL_CLK_DIV_CNTRL);
    }
    else if (nDevID == CR_DEV_FEC_UL)
    {
        nCtrl   = REG32(FEC_UL_CLK_CNTRL);
        nClkDiv = REG32(FEC_UL_CLK_DIV_CNTRL);
    }
    else
    {
        return 0;
    }

    if ((nCtrl & 1) == 0)
        return 0;

    if (((nCtrl >> 1) & 0x7) == 0)
    {
        nFreq = GetRefClock ();

        if (pPllID!=NULL)
            *pPllID = CLK_RST_REF_PLL;
    }
    else
    {
        nFreq = GetPllFreq (((nCtrl >> 1) & 0x7) - 1);

        if (pPllID!=NULL)
            *pPllID = (((nCtrl >> 1) & 0x7) - 1);
    }

    // if clock is bypassed

    if (nClkDiv & (1 << 7))
        return nFreq;

    if ((nClkDiv & 0x1F) == 0)
        return 0;

    return nFreq / (nClkDiv & 0x1F);
}

/** @brief This function checks DL and UL reset mode

    @param nDevID [in] - the FEC devices id

    @return [BOOL]*/

BOOL IsFECInReset (UINT32 nDevID)
{
    UINT32 nCtrl;

    nCtrl = REG32 (SYS_AXI_RESET_0);

    // AXI FEC reset bit:5
    if (nCtrl & (1 << 5))
        return TRUE;

    if (nCtrl & (1 << 0))
        return TRUE;

    nCtrl = REG32 (SYS_FEC_RESET);

    // FEC global reset
    if (nCtrl & 1 )
        return TRUE;

    if (nDevID == CR_DEV_FEC_DL)
    {
        return nCtrl & (1 << 4);
    }
    else if (nDevID == CR_DEV_FEC_UL)
    {
        return nCtrl & (1 << 5);
    }

    return FALSE;
}


UINT32 GetDDRFreq (UINT32 nID, UINT32 * pPllID)
{
    UINT32 nPllID = (REG32(DDR_CLK_CNTRL) >> 1);
    UINT32 nFreq;

    if ((REG32(DDR_CLK_CNTRL) & 1) == 0)
        return 0;

    if (nPllID == 0)
    {
        if (pPllID)
            *pPllID = CLK_RST_REF_PLL;
        
        return GetRefClock();
    }

    nFreq = GetPllFreq(nPllID - 1);

    if (pPllID)
       *pPllID = (nPllID - 1);

    // if divider is turned off 
    if (REG32(DDR_CLK_DIV_CNTRL) & (1 << 7))
        return nFreq;

    return nFreq / (REG32(DDR_CLK_DIV_CNTRL) & 0x1F);
}

/*******************************************************************************
*                  Interface functions                                         *
*******************************************************************************/


/** @brif This function checks out of reset bit and clock enable bit

    @param nDevID [in] - the device ID

    @return [BOOL]  TRUE - if device is enabled, 0 - if device is disabled*/

BOOL ClkRstIsDevEnabled (UINT32 nDevID)
{
    if (ClkRstIsInReset (nDevID))
        return FALSE;

    return ClkRstGetFreq (nDevID) != 0;
}

/** @brief This function returns a frequency of control object,
           if the frequency is equal to 0, this device is in reset mode

    @param nDevID [in] - the device ID, see: ClkRstDev enum

    @return [UINT32] the frequency */

UINT32 ClkRstGetFreq (UINT32 nDevID)
{
    if (nDevID >= CR_PLL_0 && nDevID <= CR_PLL_5)
        return GetPllFreq (nDevID - CR_PLL_0);

    if (nDevID >= CR_DEV_CEVA_0 && nDevID <= CR_DEV_CEVA_9)
        return GetCevaFreq (nDevID - CR_DEV_CEVA_0, NULL);

    if (nDevID >= CR_DEV_FP_0 && nDevID <= CR_DEV_FP_9)
        return GetFFTFreq (nDevID - CR_DEV_FP_0, NULL);

    if (nDevID == CR_DEV_FEC_DL || nDevID == CR_DEV_FEC_UL)
        return GetFECFreq (nDevID, NULL);

    if (nDevID == SYS_AXI || nDevID == SPU_AXI || nDevID == XP_AXI || nDevID == RAD_AXI)
        return GetAXIFreq (nDevID, NULL);

    if (nDevID == CR_DEV_SPACC)
        return GetSpaccFreq (NULL);

    if (nDevID >= ARMx4_CORE0 && nDevID <= ARMx4_CORE3)
        return GetArmx4Freq (nDevID - ARMx4_CORE0, NULL);

    if (nDevID >= ARMx2_CORE0 && nDevID <= ARMx2_CORE1)
        return GetArmx2Freq (nDevID - ARMx2_CORE0, NULL);

    if (nDevID == CR_PCI)
        return GetPCIeFreq (NULL);

    if (nDevID == CR_DDR0)
        return GetDDRFreq (0, NULL);

    return 0;
}

/** @brief This function returns the ID of source PLL used to clock specified device

    @param nDevID [in] - the device ID, see: ClkRstDev enum

    @return [UINT32] the frequency */
UINT32 ClkGetSrcPll(UINT32 nDevID)
{
    UINT32 nPllID = CLK_RST_REF_PLL;

    if (nDevID >= CR_PLL_0 && nDevID <= CR_PLL_5)
        return (nDevID - CR_PLL_0);

    if (nDevID >= CR_DEV_CEVA_0 && nDevID <= CR_DEV_CEVA_9)
    {
        GetCevaFreq (nDevID - CR_DEV_CEVA_0, &nPllID);
        return nPllID;
    }

    if (nDevID >= CR_DEV_FP_0 && nDevID <= CR_DEV_FP_9)
    {
        GetFFTFreq (nDevID - CR_DEV_FP_0, &nPllID);
        return nPllID;
    }

    if (nDevID == CR_DEV_FEC_DL || nDevID == CR_DEV_FEC_UL)
    {
        GetFECFreq (nDevID, &nPllID);
        return nPllID;
    }

    if (nDevID == SYS_AXI || nDevID == SPU_AXI || nDevID == XP_AXI || nDevID == RAD_AXI)
    {
        GetAXIFreq (nDevID, &nPllID);
        return nPllID;
    }

    if (nDevID == CR_DEV_SPACC)
    {
        GetSpaccFreq (&nPllID);
        return nPllID;
    }

    if (nDevID >= ARMx4_CORE0 && nDevID <= ARMx4_CORE3)
    {
        GetArmx4Freq (nDevID - ARMx4_CORE0, &nPllID);
        return nPllID;
    }

    if (nDevID >= ARMx2_CORE0 && nDevID <= ARMx2_CORE1)
    {
        GetArmx2Freq (nDevID - ARMx2_CORE0, &nPllID);
        return nPllID;
    }

    if (nDevID == CR_PCI)
    {
        GetPCIeFreq (&nPllID);
        return nPllID;
    }

    if (nDevID == CR_DDR0)
    {
        GetDDRFreq (0, &nPllID);
        return nPllID;
    }

    return nPllID;
}

/** @brief This function returns TRUE is a device is in reset mode

    @param nDevID [in] - the device ID , see ClkRstDev enum type

    @return [BOOL] */

BOOL ClkRstIsInReset (UINT32 nDevID)
{
    if (nDevID >= CR_PLL_0 && nDevID <= CR_PLL_5)
        return IsPllInReset (nDevID - CR_PLL_0);

    if (nDevID >= CR_DEV_CEVA_0 && nDevID <= CR_DEV_CEVA_9)
        return IsCevaInReset (nDevID - CR_DEV_CEVA_0);

    if (nDevID >= CR_DEV_FP_0 && nDevID <= CR_DEV_FP_9)
        return IsFFTInReset (nDevID - CR_DEV_FP_0);

    if (nDevID == CR_DEV_FEC_DL || nDevID == CR_DEV_FEC_UL)
        return IsFECInReset (nDevID);

    if (nDevID == SYS_AXI || nDevID == SPU_AXI || nDevID == XP_AXI || nDevID == RAD_AXI)
        return IsAXIInReset (nDevID);

    if (nDevID == CR_DEV_SPACC)
        return IsSpaccInReset ();

    if (nDevID >= ARMx4_CORE0 && nDevID <= ARMx4_CORE3)
        return IsArmx4InReset (nDevID - ARMx4_CORE0);

    if (nDevID >= ARMx4_CORE0 && nDevID <= ARMx4_CORE3)
        return IsArmx2InReset (nDevID - ARMx4_CORE0);

    if (nDevID == CR_PCI)
        return IsPCIeInReset ();

    return FALSE;
}

/** @brief This function enables device in clock and reset module

    @param nDevID [in] - the device ID , see ClkRstDev enum type

    @return [MXRC] */

MXRC ClkRstEnableDev (UINT32 nDevID)
{
    // todo:

    return 1;
}

/** @brief This function returns device frequency in ASCIIZ

    @param [nDevID] - the device ID, see ClkRstDev enum type

    @return [char *] */

char * ClkRstGetAsciizFreq (UINT32 nDevID)
{
    UINT32 nFrac;
    UINT32 nFrq = ClkRstGetFreq (nDevID);

    static char buf [50];

    nFrac = nFrq - ((nFrq / 1000000) * 1000000);

    if (nFrac == 0)
    {
        sprintf (buf, "%d MHz", nFrq / 1000000);
    }
    else
    {
        while (TRUE)
        {
            if ((nFrac % 10) == 0)
                nFrac /= 10;
            else
                break;
        }

        sprintf (buf, "%d.%d MHz", nFrq / 1000000, nFrac);
    }

    return buf;
}

UINT32 CalcFreqParam (UINT32 freq, UINT32 * _nf, UINT32 * _nr, UINT32 * _od)
{
	UINT32 nf, nr, od;
	UINT32 ref = GetRefClock() / 1000000;

	*_nf = 0;
	*_nr = 0;
	*_od = 0;

	for (od = 0; od < 14; od++)
	{
		for (nr = 0; nr < 63; nr++)
		{
			for (nf = 0; nf < 0x2000; nf++)
			{
				if ((ref * (nf+1) / (nr+1) / (od + 1)) == freq)
				{
					*_nf = nf;
					*_nr = nr;
					*_od = od;
					return 0;
				}
			}
		}
	}
	
	return 1;
}

/** @brief This function sets PLL frequency 
    @param nPllID [in] - the PLL id [0..4]
    @param nFreq  [in] - the PLL frequency specified in MHz
    @return [MXRC] an error code */

MXRC ClkRstSetPll (UINT32 nPllID, UINT32 nFreq)
{
    UINT32 nf, nr, od;

    nPllID -= CR_PLL_0;

    if (nPllID > CR_PLL_5)
        return RC_CLKRST_ID_ERROR;

    if (CalcFreqParam (nFreq, &nf, &nr, &od))
        return RC_CLKRST_FREQ_ERROR;

    REG32(PLL_NF_0(nPllID)) = (nf & 0xFF);
    REG32(PLL_NF_1(nPllID)) = ((nf >> 8) & 0x1F);
    REG32(PLL_BWADJ_0(nPllID)) = (nf & 0xFF);;
    REG32(PLL_BWADJ_1(nPllID)) = ((nf >> 8) & 0x1F);
    REG32(PLL_NR(nPllID))  = (nr & 0x3F);
    REG32(PLL_OD(nPllID))  = (od & 0xF);
    REG32(PLL_CNTRL(nPllID))   = 0;
    REG32(TOP_GEN_CNTRL) &= ~(1<<nPllID);

    return MX_OK;
}

UINT32 ClkRstFindSrcPll(UINT32 nFreq, UINT32 * nDiv)
{
    UINT32 nPllID;
    UINT32 nPllFreq;
    UINT32 nDivVal;

    _ASSERT_PTR(nDiv);

    for (nPllID = 0; nPllID < 6; nPllID++)
    {
        // to convert to Mhz
        nPllFreq = GetPllFreq(nPllID) / 1000000;

        if (nPllFreq == 0)
            continue;

        nDivVal = nPllFreq / nFreq;

        if ((nDivVal * nFreq) == nPllFreq)
        {
            *nDiv = nDivVal;
            return nPllID;
        }
    }

    return 0xFF;
}

MXRC ClkRstSetDevFreq (UINT32 nDevID, UINT32 nFreq, UINT32 nPllID)
{
    UINT32 nDiv = 1;
    UINT32 nPllFreq = 0;
    MXRC rc = RC_CLKRST_DEV_ERROR;

    if (nPllID == 0xFF)
    {
        nPllID = ClkRstFindSrcPll (nFreq, &nDiv);

        if (nPllID == 0xFF)
            return RC_CLKRST_NO_PLL;
    }
    else
    {
        // to convert to specified range
        nPllID -= CR_PLL_0;
    
        nPllFreq = GetPllFreq(nPllID) / 1000000;

        if (nPllFreq == 0)
            return RC_CLKRST_FREQ_ERROR;

        nDiv = nPllFreq / nFreq;

        if ((nFreq * nDiv) != nPllFreq)
            return RC_CLKRST_FREQ_ERROR;
    }

    if (nDevID == ARMx4_CORE0)
    {
        if (nDiv == 1)
            REG32(ARMQP_CLK_DIV_CNTRL) = (1 << 7) | 2;
        else
            REG32(ARMQP_CLK_DIV_CNTRL) = nDiv & 0x1F;

        REG32(ARMQP_CLK_CNTRL) = PLL_SOURCE(nPllID);

        rc = MX_OK;
    }

    if (nDevID == ARMx2_CORE0)
    {
        if (nDiv == 1)
            REG32(ARMDP_CLK_DIV_CNTRL) = (1 << 7) | 2;
        else
            REG32(ARMDP_CLK_DIV_CNTRL) = nDiv & 0x1F;

        REG32(ARMDP_CLK_CNTRL) = PLL_SOURCE(nPllID);

        rc = MX_OK;
    }

    if (nDevID >= CR_DEV_CEVA_0 && nDevID <= CR_DEV_CEVA_9)
    {
        if (nDiv == 1)
            REG32(CEVA_CLK_DIV_CNTRL) = (1 << 7) | 2;
        else
            REG32(CEVA_CLK_DIV_CNTRL) = nDiv & 0x1F;

        REG32(CEVA_CLK_CNTRL) = PLL_SOURCE(nPllID);

        rc = MX_OK;
    }

    if (nDevID >= CR_DEV_FP_0 && nDevID <= CR_DEV_FP_9)
    {
        if (nDiv == 1)
            REG32(FFT_CLK_DIV_CNTRL) = (1 << 7) | 2;
        else
            REG32(FFT_CLK_DIV_CNTRL) = nDiv & 0x1F;

        REG32(FFT_CLK_CNTRL) = PLL_SOURCE(nPllID);

        rc = MX_OK;
    }

    if (nDevID == CR_DEV_FEC_DL)
    {
        if (nDiv == 1)
            REG32(FEC_DL_CLK_DIV_CNTRL) = (1 << 7) | 2;
        else
            REG32(FEC_DL_CLK_DIV_CNTRL) = nDiv & 0x1F;

        REG32(FEC_DL_CLK_CNTRL) = PLL_SOURCE(nPllID);

        rc = MX_OK;
    }

    if (nDevID == CR_DEV_FEC_UL)
    {
        if (nDiv == 1)
            REG32(FEC_UL_CLK_DIV_CNTRL) = (1 << 7) | 2;
        else
            REG32(FEC_UL_CLK_DIV_CNTRL) = nDiv & 0x1F;

        REG32(FEC_UL_CLK_CNTRL) = PLL_SOURCE(nPllID);

        rc = MX_OK;
    }

    if (nDevID == SYS_AXI)
    {
        UINT32 tmp1, tmp2;
    
        tmp1 = REG32(AXI_CLK_CNTRL_0);
        tmp2 = REG32(AXI_CLK_DIV_CNTRL_0);

        if (nDiv == 1)
        {
            REG32(AXI_CLK_CNTRL_0) = (tmp1 & 0xF0) | PLL_SOURCE(nPllID);

            // the divider is turned off
            REG32(AXI_CLK_DIV_CNTRL_2) |= 1; 
        }
        else
        {
            REG32(AXI_CLK_CNTRL_0) = (tmp1 & 0xF0) | PLL_SOURCE(nPllID);
            REG32(AXI_CLK_DIV_CNTRL_0) = (tmp2 & 0xF0) | (nDiv & 0xF);

            // the divider is turned off
            REG32(AXI_CLK_DIV_CNTRL_2) &= ~1; 
        }
        
        rc = MX_OK;
    }

    if (nDevID == XP_AXI)
    {
        UINT32 tmp1, tmp2;
    
        tmp1 = REG32(AXI_CLK_CNTRL_0);
        tmp2 = REG32(AXI_CLK_DIV_CNTRL_0);

        if (nDiv == 1)
        {
            REG32(AXI_CLK_CNTRL_0) = (tmp1 & 0x0F) | (PLL_SOURCE(nPllID) << 4);

            // the divider is turned off
            REG32(AXI_CLK_DIV_CNTRL_2) |= (1 << 1); 
        }
        else
        {
            REG32(AXI_CLK_CNTRL_0) = (tmp1 & 0x0F) | (PLL_SOURCE(nPllID) << 4);
            REG32(AXI_CLK_DIV_CNTRL_0) = (tmp2 & 0x0F) | ((nDiv & 0xF) << 4);

            // the divider is turned off
            REG32(AXI_CLK_DIV_CNTRL_2) &= ~(1 << 1);
        }
        
        rc = MX_OK;
    }

    if (nDevID == RAD_AXI)
    {
        UINT32 tmp1, tmp2;
    
        tmp1 = REG32(AXI_CLK_CNTRL_1);
        tmp2 = REG32(AXI_CLK_DIV_CNTRL_1);

        if (nDiv == 1)
        {
            REG32(AXI_CLK_CNTRL_1) = (tmp1 & 0xF0) | PLL_SOURCE(nPllID);

            // the divider is turned off
            REG32(AXI_CLK_DIV_CNTRL_2) |= 1; 
        }
        else
        {
            REG32(AXI_CLK_CNTRL_1) = (tmp1 & 0xF0) | PLL_SOURCE(nPllID);
            REG32(AXI_CLK_DIV_CNTRL_1) = (tmp2 & 0xF0) | (nDiv & 0xF);

            // the divider is turned off
            REG32(AXI_CLK_DIV_CNTRL_2) &= ~1; 
        }
        
        rc = MX_OK;
    }

    if (nDevID == SPU_AXI)
    {
        UINT32 tmp1, tmp2;
    
        tmp1 = REG32(AXI_CLK_CNTRL_1);
        tmp2 = REG32(AXI_CLK_DIV_CNTRL_1);

        if (nDiv == 1)
        {
            REG32(AXI_CLK_CNTRL_1) = (tmp1 & 0x0F) | (PLL_SOURCE(nPllID) << 4);

            // the divider is turned off
            REG32(AXI_CLK_DIV_CNTRL_2) |= (1 << 1); 
        }
        else
        {
            REG32(AXI_CLK_CNTRL_1) = (tmp1 & 0x0F) | (PLL_SOURCE(nPllID) << 4);
            REG32(AXI_CLK_DIV_CNTRL_1) = (tmp2 & 0x0F) | ((nDiv & 0xF) << 4);

            // the divider is turned off
            REG32(AXI_CLK_DIV_CNTRL_2) &= ~(1 << 1);
        }
        
        rc = MX_OK;
    }

    return rc;
}

void SysClkMmuCfg(UINT32 nCpuID, UINT32 nParam)
{
    if (nCpuID == 0)
    {
        MmuMgrCfgMemAP(TOPCLKRST_BASEADDR, 4*1024, AP_MGR_READ_WRITE, AP_MGR_READ_ONLY);
        MmuMgrCfgMemAP(XPCLKRST_BASEADDR,  4*1024, AP_MGR_READ_WRITE, AP_MGR_READ_ONLY);
        MmuMgrCfgMemAP(CFG_SYS_BASEADDR,   4*1024, AP_MGR_READ_WRITE, AP_MGR_READ_ONLY);
    }
}

