/** @file bs_static_config.c
 *
 * @brief Functions for static configuration enb
 * @author Mindspeed Technologies
 * @version $Revision: 1.9 $
 *
 * COPYRIGHT(c) 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#include "resultcodes.h"
#include "dci.h"
#include "bs_static_config.h"
#include "MacUtils.h"
#include "ltemaccore.h"

#include "lte_phich.h"

U32 GetMapSize(U32 bw)
{
    switch(bw)
    {
    case BW_1_4_MHZ:
        return RB_SLOT_6;
    case BW_3_0_MHZ:
        return RB_SLOT_15;
    case BW_5_0_MHZ:
        return RB_SLOT_25;
    case BW_10_0_MHZ:
        return RB_SLOT_50;
    case BW_20_0_MHZ:
        return RB_SLOT_100;
    default:
        return 0;
    }
}

U32 GetBwSize(U32 n_rbs)
{
    switch(n_rbs)
    {
    case RB_SLOT_6:
        return BW_1_4_MHZ;
    case RB_SLOT_15:
        return BW_3_0_MHZ;
    case RB_SLOT_25:
        return BW_5_0_MHZ;
    case RB_SLOT_50:
        return BW_10_0_MHZ;
    case RB_SLOT_100:
        return BW_20_0_MHZ;
    default:
        return 0;
    }
}


U32 calc_rbg(U32 l_crb, U32 start_rb_index, U8 P, U8 len)
{
    U32 rbg=0;
    U8  i, pos=len-1-start_rb_index/P;

    for (i=0; i<l_crb/P; i++)
    {
        rbg |= 1<<pos;
        pos--;
    }
    
    return rbg;
}


U8 get_n_step_rb(U8 dl_rbs)
{
    if ( dl_rbs < 50 )
        return 2;
    else
        return 4;
}

U8 get_P(U8 n_dl_rb)
{
    if ( n_dl_rb <= 10 )
        return 1;

    else if (( n_dl_rb >10 ) && (n_dl_rb<=26))
        return 2;

    else if (( n_dl_rb >26 ) && (n_dl_rb<=63))
        return 3;

    else if (( n_dl_rb >63 ) && (n_dl_rb<=110))
        return 4;

    return 0;
}

U8 get_gap_1(U8 dl_rbs)
{
    int n_gap_1 = 0;
    if (dl_rbs <= 10)
    {
        n_gap_1 = dl_rbs/2;
    }
    else if (dl_rbs == 11)
    {
        n_gap_1 = 4;
    }
    else if (dl_rbs <= 19)
    {
        n_gap_1 = 8;
    }
    else if (dl_rbs <= 26)
    {
        n_gap_1 = 12;
    }
    else if (dl_rbs <= 44)
    {
        n_gap_1 = 18;
    }
    else if (dl_rbs <= 49)
    {
        n_gap_1 = 27;
    }
    else if (dl_rbs <= 63)
    {
        n_gap_1 = 27;
    }
    else if (dl_rbs <= 79)
    {
        n_gap_1 = 32;
    }
    else if (dl_rbs <= 110)
    {
        n_gap_1 = 48;
    }
    return n_gap_1;
}


void initBS(BS_STATIC_CONFIG *pBsStaticConfig)
{    
    pBsStaticConfig->n_dl_rb = GetMapSize(pBsStaticConfig->bw_mode);
    pBsStaticConfig->n_ul_rb = GetMapSize(pBsStaticConfig->bw_mode);
    pBsStaticConfig->n_dl_vrb= GetMapSize(pBsStaticConfig->bw_mode);

    pBsStaticConfig->P = get_P(pBsStaticConfig->n_dl_rb);

    pBsStaticConfig->size_rba_pusch = Log2(pBsStaticConfig->n_ul_rb*(pBsStaticConfig->n_ul_rb+1)/2);
    pBsStaticConfig->size_rba_pdsch = Log2(pBsStaticConfig->n_dl_rb*(pBsStaticConfig->n_dl_rb+1)/2);
    pBsStaticConfig->size_rbg_pusch = (pBsStaticConfig->n_dl_rb + pBsStaticConfig->P - 1)/pBsStaticConfig->P;

    pBsStaticConfig->n_step_rb      = get_n_step_rb(pBsStaticConfig->n_dl_rb);
    pBsStaticConfig->n_dl_vrb_gap1  = get_gap_1(pBsStaticConfig->n_dl_rb);
    pBsStaticConfig->size_rba_vrb   = Log2((pBsStaticConfig->n_dl_vrb_gap1/pBsStaticConfig->n_step_rb)*(pBsStaticConfig->n_dl_vrb_gap1/pBsStaticConfig->n_step_rb+1)/2);

    calc_dci_size((DCI_SIZE_t *)&pBsStaticConfig->DCI_size.format0, pBsStaticConfig);

    pBsStaticConfig->six_ng = 1; // It means 1/6
    lte_phich_calc_n_group(pBsStaticConfig);
    return;
}


/** Reconfigure PHY initialization values to match 20 MHz bandwidth
 *
 * @param pParm Pointer to preallocated block holding PHY initialization values
 * @return Standard result code
 */
RESULTCODE bs_static_config_update_20mhz(PINITPARM pParm)
{
    pParm->channelBandwidth = BW_20_0_MHZ;
    pParm->numberResourceBlocksperSlot = RB_SLOT_100;
    pParm->fftSize = FFTSZ_2048;
    pParm->numberUsedSubcarriers = NUSC_1200;
    pParm->nMaxDlRb = NMAXRB_100;
    pParm->nMaxUlRb = NMAXRB_100;

    return SUCCESS;
}

/** Reconfigure PHY initialization values to match 10 MHz bandwidth
 *
 * @param pParm Pointer to preallocated block holding PHY initialization values
 * @return Standard result code
 */
RESULTCODE bs_static_config_update_10mhz(PINITPARM pParm)
{
    pParm->channelBandwidth = BW_10_0_MHZ;
    pParm->numberResourceBlocksperSlot = RB_SLOT_50;
    pParm->fftSize = FFTSZ_1024;
    pParm->numberUsedSubcarriers = NUSC_600;
    pParm->nMaxDlRb = NMAXRB_50;
    pParm->nMaxUlRb = NMAXRB_50;

    return SUCCESS;
}

