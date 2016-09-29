
// -------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL lte_bs_phy_nmm_radio_itf.h
 *
 * @brief LTE listening RFIC itf (HAL)
 * @author Mindspeed Technologies
 * @version $Revision: 1.1.4.1.2.7 $
 *
 * COPYRIGHT&copy; 2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
// -------------------------------------------------------------------------------------------

#ifndef LTE_BS_LISTENING_RADIO_ITF
#define LTE_BS_LISTENING_RADIO_ITF

#include "LtePhyL2Api.h"
#include "ltephydef.h"
#include "ltephy.h"
#ifdef RT_MODE
#include "ad9361radio.h"
#endif

#define MIN_EUTRA_BAND						1
#define MAX_EUTRA_BAND 						41
#define IS_FREQ_BAND_AVAILABLE(x)		 	((x >=MIN_EUTRA_BAND) && (x <= MAX_EUTRA_BAND) && (x != 6) && (x != 15) &&(x != 16) && !((x >21) && (x<33)))
/*
 * 36.101 5.5.1
 *
 * E UTRA
 * Operating Band  Uplink
 *   (UL)            (DL)                                Duplex Mode
 *   FUL_low   ? FUL_high       FDL_low   ? FDL_high
 *--------------------------------------------------------------------
 * 1   1920 MHz ?1980 MHz         2110 MHz ?2170 MHz     FDD
 * 2   1850 MHz ?1910  MHz        1930 MHz ?1990 MHz     FDD
 * 3   1710 MHz ?1785 MHz         1805 MHz ?1880 MHz     FDD
 * 4   1710 MHz ?1755 MHz         2110 MHz ?2155 MHz     FDD
 * 5   824 MHz ?849 MHz           869 MHz ?894MHz        FDD
 * 6(*1) 830 MHz ? 840  MHz       875 MHz - 885 MHz       FDD
 * 7   2500 MHz ?2570 MHz         2620 MHz ?2690 MHz     FDD
 * 8   880 MHz ?915 MHz           925 MHz ?960 MHz       FDD
 * 9   1749.9 MHz  ?1784.9 MHz    1844.9 MHz ?1879.9 MHz FDD
 * 10  1710 MHz ?1770 MHz         2110 MHz ?2170 MHz     FDD
 * 11  1427.9 MHz ?1447.9 MHz     1475.9 MHz ?1495.9 MHz FDD
 * 12  698 MHz ?716 MHz           728 MHz ?746 MHz       FDD
 * 13  777 MHz ?787 MHz           746 MHz ?756 MHz       FDD
 * 14  788 MHz ?798 MHz           758 MHz ?768 MHz       FDD
 * 15  Reserved                    Reserved                FDD
 * 16  Reserved                    Reserved                FDD
 * 17  704 MHz ?716 MHz           734 MHz ?746 MHz       FDD
 * 18  815 MHz ?830 MHz           860 MHz ?875 MHz       FDD
 * 19  830 MHz ?845 MHz           875 MHz ?890 MHz       FDD
 * 20  832 MHz ?862 MHz           791 MHz ?821 MHz       FDD
 * 21  1447.9 MHz ?1462.9 MHz     1495.9 MHz ?1510.9 MHz FDD
 * ...
 * 33  1900 MHz ? 1920 MHz        1900 MHz ?1920 MHz     TDD
 * 34  2010 MHz ? 2025 MHz        2010 MHz - 2025 MHz     TDD
 * 35  1850 MHz ? 1910 MHz        1850 MHz ?1910 MHz     TDD
 * 36  1930 MHz ? 1990 MHz        1930 MHz ?1990 MHz     TDD
 * 37  1910 MHz ? 1930 MHz        1910 MHz ?1930 MHz     TDD
 * 38  2570 MHz -  2620 MHz        2570 MHz ?2620 MHz     TDD
 * 39  1880 MHz ? 1920 MHz        1880 MHz ?1920 MHz     TDD
 * 40  2300 MHz ? 2400 MHz        2300 MHz ?2400 MHz     TDD
 * 41  2496 MHz - 2690 MHz         2496 MHz - 2690 MHz     TDD
 *
 *(*) 1: Band 6 is not applicable
 */

typedef struct {
    /* FDL_low (MHz) */
    U16             FDLlow;
    /* NOffs-DL */
    U16             Noffs_DL;
    /* Range of NDL */
    U16             range_min;
    U16             range_max;
} EARFNC_item;


/*
 * 36.101 5.7.3.1
 */
extern EARFNC_item EARFNC_TABLE[MAX_EUTRA_BAND + 1];


#define BAND_EARFCN_TO_KHZ(freq_band_indicator, earfcn)     (1000*EARFNC_TABLE[freq_band_indicator].FDLlow + 100*(earfcn - EARFNC_TABLE[freq_band_indicator].Noffs_DL))
#define EARFCNIDX_TO_KHZ(freq_band_indicator, idx)  (1000*EARFNC_TABLE[freq_band_indicator].FDLlow + 100*(EARFNC_TABLE[freq_band_indicator].range_min + idx - EARFNC_TABLE[freq_band_indicator].Noffs_DL))
#define NB_EARFCN_PER_BAND(freq_band_indicator)     (EARFNC_TABLE[freq_band_indicator].range_max - EARFNC_TABLE[freq_band_indicator].range_min)


/* FDL = FDL_low + 0.1(NDL - NOffs-DL) */
#ifdef RT_MODE

#define USE_UL_RF_PATH 0

#if USE_UL_RF_PATH
#define SWITCH_RFIC_DL_FREQ(freq) if(pListening->di_mode == DI_PCIE) \
    {PhyDiStopNmm(NULL); Ad9361RadioInit(7,1,1,0,2535,0);PhyDiStart(0, &pListening->phystart_req, 0);}\
    nmm_dbg_printf(("SWITCH_RFIC_DL_FREQ to %d Khz\n",freq));
#else
#define SWITCH_RFIC_DL_FREQ(freq) /*if(pListening->di_mode == DI_PCIE) \
    {PhyDiStopNmm(NULL); Ad9361RadioInit(107,1,1,0,freq,0);PhyDiStart(0, &pListening->phystart_req, 0);}\
    nmm_dbg_printf(("SWITCH_RFIC_DL_FREQ to %d Khz\n",freq));*/
#endif


#define READ_RSSI pListening->di_mode == DI_CPRI_EMULATOR ? (543) : Ad9361GetRSSI(0)

#else
#define SWITCH_RFIC_DL_FREQ(freq) nmm_dbg_printf(("SWITCH_RFIC_DL_FREQ to %d Khz\n",freq));
#define READ_RSSI (543)         /* fake */
#endif

#define IS_BAND_FDD(band) (band<33)





void            earfcn_to_khz(U32, U32 *, U8 *);
void            lte_bs_phy_nmm_radio_itf_query_available_band(U8 *, U8 *);
void            lte_bs_phy_nmm_radio_register_radio(void);

#endif
