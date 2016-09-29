//-------------------------------------------------------------------------------------------
/** @file bs_rx_ChEstimator_pucch.c
 *
 * @brief LTE Base Station Receiver, Channel Estimation for PUCCH
 * @author Mindspeed Technologies
 * @version $Revision: 1.64.8.1 $
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

#ifdef _WIN32
#define WIN_BLD 1
#else
#define WIN_BLD 0
#endif

#include "typedef.h"
#include "ltephydef.h"
#include "prototypes.h"
#include "ext_phy.h"
#include "math_ext32.h"

#ifdef CEVA_INTRINSICS
#define CEVA_BUILD 1
#include <basic_op.h>
#include <asm-insn.h>

#else
#define CEVA_BUILD 0
#include "basic_op_cc.h"

#define round(x)    round_s(x)

#endif

//-------------------------------------------------------------------------------------------
/** @brief LTE Base Station Rx, Channel Estimation in PUCCH for a 1/2ms slot
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param *adet              - input from FFT to ChanEst
 *  @param *rs_grid           - Array of pointers containing beginning locations of all symbols
 *  @param *aP_temp           - reference signals (pilots) sent in the pilotCarrierSymbol of the adet array
 *  @param pilotCarrierSymbol - Location of Pilot symbols
 *  @param num_pilots         - number of pilots
 *  @param MI                 - number of TX antennas
 *  @param row_a              - number of rows of adet
 *  @param col_a              - number of columns of adet
 *  @param pucch_format       - PUCCH Format (format 1,1a,2,2a,2b)
 *  @param ChanEstPar         - Channel Estimate Structure containing configuration variables
 *  @param chEst              - pointer to channel estimate
 *  @param expo_fac           - scale factor of adet
 *  @param *chMeasurements    - Pointer to Channel Measurements results, only Timing advance currently supported
 *  @param slot               - Slot number
 *  @param Msc                - number of sub carriers used
 *  @param Mrb                - number of resource blocks used
 *  @param start_address      - starting RB address

 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------
 *  |Name        |Name in  |Direction     |Length     |Type          |Data Format |Valid Range |Units |
 *  |            |Standard |              |           |              |            |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |adet         |         |pInput        |Mpucch_sc* |pComplex Array|(1:0:15)    |Full Range  |     |
 *  |             |         |              |Nul_symb   |              |            |            |     |
 *  --------------------------------------------------------------------------------------------------
 *  |rs_grid      |         |pInput        |2*Nul_symb |Pointers      |            |            |     |
 *  --------------------------------------------------------------------------------------------------
 *  |aP_temp      |         |pInput        |Mpucch_sc  |pComplex Array|(1:0:15)    |Full Range  |     |
 *  --------------------------------------------------------------------------------------------------
 *  |pilotCarrier |         |pnput         |num_pilots |pReal Array   |(1:15:0)    |[0:13]      |     |
 *  |Symbol       |         |              |           |              |            |            |     |
 *  --------------------------------------------------------------------------------------------------
 *  |num_pilots   |         |Input         |1          |Real          |(1:15:0)    |[1,2,3]     |     |
 *  --------------------------------------------------------------------------------------------------
 *  |MI           |         |Input         |1          |Real          |(1:15:0)    |[1,2,4]     |     |
 *  --------------------------------------------------------------------------------------------------
 *  |row_a        |         |Input         |1          |Real          |(1:15:0)    |Full Range  |     |
 *  --------------------------------------------------------------------------------------------------
 *  |col_a        |         |Input         |1          |Real          |(1:15:0)    |Full Range  |     |
 *  --------------------------------------------------------------------------------------------------
 *  |ChanEstPar   |         |Input         |1          |Structure     |            |            |     |
 *  --------------------------------------------------------------------------------------------------
 *  |chEst        |         |pOutput       |Mpucch_sc* |pComplex Array|(1:0:15)    |Full Range  |     |
 *  |             |         |              |Nul_symb   |              |            |            |     |
 *  --------------------------------------------------------------------------------------------------
 *  |expo_fac     |         |pInput        |1          |Real          |(1:15:0)    |[-16:16)    |     |
 *  ---------------------------------------------------------------------------------------------------
 *  |chMeasurements|        |pOutput       |1          |Real          |(1:15:0)    |[0,63]      |     |                                                                        |
 *  ---------------------------------------------------------------------------------------------------
 *  |slot         |         |Input         |1          |Real          |(1:15:0)    |[0:19]      |     |
 *  --------------------------------------------------------------------------------------------------
 *  |Msc          |         |Input         |1          |Real          |(1:15:0)    |[0:1200]    |     |
 *  --------------------------------------------------------------------------------------------------
 *  |Mrb          |         |Input         |1          |Real          |(1:15:0)    |[0:100]     |     |
 *  --------------------------------------------------------------------------------------------------
 *  |start_address|         |Input         |1          |Real          |(1:15:0)    |[0:100]     |     |
 *  --------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------------
 *  |Block Name          |bs_rx_ChEstimator_PUCCH            |
 *  ---------------------------------------------------------
 *  |Input Streams       |adet,aP_temp                       |
 *  ---------------------------------------------------------
 *  |Output Streams      |adet,chEst,chMeasurements          |
 *  ---------------------------------------------------------
 *  |Config Parameters   |ChanEstPar,MI,row_a,col_a,expo_fac |
 *  |                    |pilotCarrierSymbol,pucch_format,   |
 *  |                    |expo_fac,slot,Msc,Mrb,start_address|
 *  ---------------------------------------------------------
 *  |Status Parameters   |none                               |
 *  ---------------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Channel Estimator for PUCCH:
 * -# For the entire frequency band, do direct calculation of channel estimate at each pilot
 *    point in the pilotCarrierSymbol of the slot, by division of
 *    the received IQ sample by the known pilot data symbol value.
 * -# Do fine timing adjustment to remove the phase rotation with frequency.
 * -# Then extrapolate these channel estimates across all symbols,
 *    to every subcarrier across the complete slot. Note, there is a pilot value
 *    for every frequency point, so no need to interpolate across frequency. Since there is
 *    only one pilot symbol in a slot, there is no opportunity to interpolate across time.
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * [1] 3GPP TS 36.211 v8.7, Sec.5.4, "Uplink: Physical Uplink Control Channel"
 *
 * \ingroup group_lte_rx_uplink_demodulator
 *
 **/
//-------------------------------------------------------------------------------------------
void bs_rx_ChEstimator_PUCCH(S16 *adet,  ULRXINPUTPARAMS *pChanEstPar, S16 *chEst,
                             S16 *expo_fac, S16 slot, S16 *temp_vec1,
                             S32 ChanNum, S16 *r_alpha_uv, S16 *Sdescramb,
                             S16 *nOc, S16 srsPilotSym, S16 *pAvgReDet_pucchACK, S16 *pAvgReDet_pucchCQI,
                             S16 *pChEst_pucch,
                             S32 *pRxInPwrAvg_PerRbPilot, S16 *pPwrAvg_Pilot_expo,
                             S32 *pRxInPwrAvg_PerRbDataSym, S16 *pPwrAvg_Data_expo, S32 *ErrAvg, S16 startRB)
{
    S32 i;
    S16 Inv_num_pilots;
    S16 p;
    S16 max_expo;
    S16 Mpucch_sc, Msc;
    S16 Nul_symb;
    S16 num_pilots;
    S16 pucch_format;
    S16 tmpR0, tmpI0;
    S16 SrsEnable;
    S32 Pwr_PilotSym;
    S16 *pilotCarrierSymbol;
    S32 L_temp1 = 0, L_temp2 = 0;

   
    S16 *derotation;
    
    S16 df,Nrb_sc, NusedE, Nfft;
    U32 Fs;
    S16 Nul_rb;
    S16 phaseOffset;

    S16 dummyNoffset, dummy_expo;

    S16 start_address;
    S16 timeAdjust, index;

    S32 I;
    S16 isym;
    
    S16 tmpR1, tmpI1;
    S16 tmpR2, tmpI2;
    S16 tmpR3, tmpI3;
    S16 temp;

    S16 flagDoTOC, Ncp0, DelayOffset;

#if 0
    static FILE *fmyAVG2;
    static short first_open_avg =1;

    if(first_open_avg)
    {
        fmyAVG2 = fopen("MYAVG2.txt","wt");
        first_open_avg = 0;
    }
#endif

    //Copy parameters back out of structure so I don't have to
    //change the code below.
    Mpucch_sc    = pChanEstPar->Nrb_sc;
    Msc          = Mpucch_sc;
    num_pilots   = pChanEstPar->num_Pilots_slot[ChanNum];
    pucch_format = pChanEstPar->pucchFormat[ChanNum];
    Nul_symb     = pChanEstPar->Nul_symb;

    df = pChanEstPar->df;
    Fs = pChanEstPar->Fs;
   
    Nrb_sc = pChanEstPar->Nrb_sc;
    NusedE = pChanEstPar->NusedE;
    Nfft = pChanEstPar->Nfft;
    Nul_rb = pChanEstPar->Nul_rb;
    Ncp0 = (S16) (pChanEstPar->Ncp0);
        
    start_address = startRB * Nrb_sc;
    if(slot == 1)
        start_address = (Nul_rb - 1 - startRB) * Nrb_sc;

    Inv_num_pilots = 32767; // num_pilots = 1
    if (num_pilots == 2)
        Inv_num_pilots = 16384;
    else if (num_pilots == 3)
        Inv_num_pilots = 10923;

    if (pucch_format < FORMAT2)// Format 1s
        pilotCarrierSymbol = &Pilot_symbol_loc[0];
    else if (pucch_format == FORMAT2)  //Format 2
        pilotCarrierSymbol = &Pilot_symbol_loc[3];
    else //Format 2a and 2b
        pilotCarrierSymbol = &Pilot_symbol_loc[5];

    max_expo = -32768;
    for (i = 0; i < Nul_symb; i++)
    {
        if (max_expo < expo_fac[i])
        {
            max_expo = expo_fac[i];
        }
    }
    *pPwrAvg_Pilot_expo  = max_expo;
    *pPwrAvg_Data_expo   = max_expo;
    *pRxInPwrAvg_PerRbPilot   = 0;
    *pRxInPwrAvg_PerRbDataSym = 0;
    
    for (p = 0; p < num_pilots; p++)
    {
        temp = (S16)(2*(expo_fac[pilotCarrierSymbol[p]]-max_expo));
        Pwr_PilotSym = ComputePwrAvg_PerRbPilot(&adet[2 * Msc * pilotCarrierSymbol[p]], Mpucch_sc);

        //average over number of pilots and bring all symbols to the same exponent
        
        L_temp1 = L_mpy_ls(Pwr_PilotSym, Inv_num_pilots);
        L_temp2 = L_shl(L_temp1, temp);
        *pRxInPwrAvg_PerRbPilot = L_add(*pRxInPwrAvg_PerRbPilot, L_temp2);
    }

    // Step 2) Do fine timing adjustment to remove the phase rotation with frequency.
    // !!!!!!!!!!!!!!move to function FineTimeAdjust(), since re-useable
    //calculate time offset
    //Err = 0;  %init error. Accumulate below, %!!!!!!!independently for all
    //Resource Blocks, for now. Whether consecutive in frequency or not...
    //kre= [0:Nrb_sc-1];
    pChanEstPar->BsRxChMeas[ChanNum].TimingAdvance = 31; 
    flagDoTOC = 0;

    //only 1 user in this RB and valid timing offset
    if(temp_vec1[2*Msc + startRB] == 1 && pChanEstPar->PucchValidTimeOffset[ChanNum] == 1) 
    {
        derotation = temp_vec1;
        phaseOffset = get_angle_fx(ErrAvg[0],ErrAvg[1]);
        Compute_Derotation(Fs, phaseOffset, df, Nfft, Mpucch_sc, NusedE, derotation,
                       start_address, slot, &dummyNoffset, &dummy_expo);
      
        //If timing offset measured is bigger then the CP, 
        //do not apply it.
 
#ifndef CEVA_INTRINSICS
    DelayOffset = shift_r(dummyNoffset, negate(dummy_expo));
#else
    DelayOffset = shr_r(dummyNoffset , dummy_expo);
#endif
        if( abs_s(DelayOffset) < Ncp0)     //Second qualifier 
            flagDoTOC = 1;  
    
        if (flagDoTOC == 1)
        {
            index = 0;
            switch (pChanEstPar->Nul_rb)
            {
                case 6:
                    index = 0;
                    break;
                case 15:
                    index = 1;
                    break;
                case 25:
                    index = 2;
                    break;
                case 50:
                    index = 3;
                    break;
                case 75:
                    index = 4;
                    break;
                case 100:
                    index = 5;
                    break;
            }

           //ratio_fs = Fs / 30720000.0;
           //timeAdjust = (S16) (-ceil(DelayOffset / (16 * ratio_fs)));
           timeAdjust = divide_ceil(DelayOffset,timeAdjust_TAB1[index]);
           if(DelayOffset <=0)
           {
              timeAdjust = shr(negate(DelayOffset),timeAdjust_TAB2[index]);
              if(index == 4)
                 timeAdjust = mult(timeAdjust,10923);
           }

           if(DelayOffset >0)
              timeAdjust = negate(timeAdjust);
        
            timeAdjust += 31;
            if (timeAdjust > 63)
                timeAdjust = 63; //upper bound
            else if (timeAdjust < 0)
                timeAdjust = 0; //lower bound
            pChanEstPar->BsRxChMeas[ChanNum].TimingAdvance = timeAdjust;

            for (p = 0; p < num_pilots; p++)
            {
#ifdef CEVA_INTRINSICS
                Remove_HalfCyclicShift_A(derotation, chEst + 2 * (p * Mpucch_sc), chEst + 2 * (p * Mpucch_sc), Mpucch_sc);
#else
                Remove_HalfCyclicShift(derotation, chEst + 2 * (p * Mpucch_sc),   chEst + 2 * (p * Mpucch_sc), Mpucch_sc);
#endif
            }

            //Scale down derotation for derotation processing
            for (i = 0; i < Mpucch_sc / 4; i++)
            {
                I = i << 1;

                tmpR0 = (S16) mult(derotation[4 * I], 16384);
                tmpI0 = (S16) mult(derotation[4 * I + 1], 16384);

                tmpR1 = (S16) mult(derotation[4 * I + 2], 16384);
                tmpI1 = (S16) mult(derotation[4 * I + 3], 16384);

                tmpR2 = (S16) mult(derotation[4 * I + 4], 16384);
                tmpI2 = (S16) mult(derotation[4 * I + 5], 16384);

                tmpR3 = (S16) mult(derotation[4 * I + 6], 16384);
                tmpI3 = (S16) mult(derotation[4 * I + 7], 16384);

                derotation[4 * I] = tmpR0;
                derotation[4 * I + 1] = tmpI0;
                derotation[4 * I + 2] = tmpR1;
                derotation[4 * I + 3] = tmpI1;
                derotation[4 * I + 4] = tmpR2;
                derotation[4 * I + 5] = tmpI2;
                derotation[4 * I + 6] = tmpR3;
                derotation[4 * I + 7] = tmpI3;
            }
            for (isym = 0; isym < Nul_symb; isym++)
            {
#ifdef CEVA_INTRINSICS
                Remove_HalfCyclicShift_A(derotation, adet+ 2*Msc*isym, adet+ 2*Msc*isym, Mpucch_sc);
#else
                Remove_HalfCyclicShift(derotation, adet + 2 * Msc * isym, adet + 2 * Msc * isym, Mpucch_sc);
#endif
           }
        }   //flagDoTOC
    }

    pChEst_pucch[0] = 0;
    pChEst_pucch[1] = 0;

    //Initialize average
    for (p = 0; p < num_pilots; p++)
    {  
        //Average across subcarriers
        temp_vec1[0] = 0;
        temp_vec1[1] = 0;
        temp = (S16)(expo_fac[pilotCarrierSymbol[p]]-max_expo);
#ifdef CEVA_INTRINSICS
        ComputeChestAvg_PerRbPilot_A(&chEst[2 * Msc * p], Mpucch_sc, temp_vec1);
#else
        ComputeChestAvg_PerRbPilot(&chEst[2 * Msc * p], Mpucch_sc, temp_vec1);
#endif

        //Bring each channel estimate to common exponent
        tmpR0 = shl(temp_vec1[0], temp);
        tmpI0 = shl(temp_vec1[1], temp);

        //Averaging across pilot symbols and Return one channel estimate per slot/per antenna
        pChEst_pucch[0] = add(pChEst_pucch[0], mult(tmpR0, Inv_num_pilots));
        pChEst_pucch[1] = add(pChEst_pucch[1], mult(tmpI0, Inv_num_pilots));
    }

    SrsEnable = (srsPilotSym != -1);

    if(flagDoTOC == 1)
        max_expo = max_expo - 1;

    CazacCorr(adet, r_alpha_uv, Mpucch_sc, Nul_symb, Sdescramb, slot, nOc, SrsEnable, expo_fac, 
              max_expo, pAvgReDet_pucchACK,  pAvgReDet_pucchCQI, pRxInPwrAvg_PerRbDataSym, pucch_format);

    return;
}

//-------------------------------------------------------------------------------------------
/** @brief LTE Base Station Rx, Channel Estimation in PUCCH for a 1/2ms slot
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param *adet              - input from FFT to ChanEst
 *  @param *rs_grid           - Array of pointers containing beginning locations of all symbols
 *  @param *aP_temp           - reference signals (pilots) sent in the pilotCarrierSymbol of the adet array
 *  @param pilotCarrierSymbol - Location of Pilot symbols
 *  @param num_pilots         - number of pilots
 *  @param MI                 - number of TX antennas
 *  @param row_a              - number of rows of adet
 *  @param col_a              - number of columns of adet
 *  @param pucch_format       - PUCCH Format (format 1,1a,2,2a,2b)
 *  @param ChanEstPar         - Channel Estimate Structure containing configuration variables
 *  @param chEst              - pointer to channel estimate
 *  @param expo_fac           - scale factor of adet
 *  @param *chMeasurements    - Pointer to Channel Measurements results, only Timing advance currently supported
 *  @param slot               - Slot number
 *  @param Msc                - number of sub carriers used
 *  @param Mrb                - number of resource blocks used
 *  @param start_address      - starting RB address

 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------
 *  |Name        |Name in  |Direction     |Length     |Type          |Data Format |Valid Range |Units |
 *  |            |Standard |              |           |              |            |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |adet         |         |pInput        |Mpucch_sc* |pComplex Array|(1:0:15)    |Full Range  |     |
 *  |             |         |              |Nul_symb   |              |            |            |     |
 *  --------------------------------------------------------------------------------------------------
 *  |rs_grid      |         |pInput        |2*Nul_symb |Pointers      |            |            |     |
 *  --------------------------------------------------------------------------------------------------
 *  |aP_temp      |         |pInput        |Mpucch_sc  |pComplex Array|(1:0:15)    |Full Range  |     |
 *  --------------------------------------------------------------------------------------------------
 *  |pilotCarrier |         |pnput         |num_pilots |pReal Array   |(1:15:0)    |[0:13]      |     |
 *  |Symbol       |         |              |           |              |            |            |     |
 *  --------------------------------------------------------------------------------------------------
 *  |num_pilots   |         |Input         |1          |Real          |(1:15:0)    |[1,2,3]     |     |
 *  --------------------------------------------------------------------------------------------------
 *  |MI           |         |Input         |1          |Real          |(1:15:0)    |[1,2,4]     |     |
 *  --------------------------------------------------------------------------------------------------
 *  |row_a        |         |Input         |1          |Real          |(1:15:0)    |Full Range  |     |
 *  --------------------------------------------------------------------------------------------------
 *  |col_a        |         |Input         |1          |Real          |(1:15:0)    |Full Range  |     |
 *  --------------------------------------------------------------------------------------------------
 *  |ChanEstPar   |         |Input         |1          |Structure     |            |            |     |
 *  --------------------------------------------------------------------------------------------------
 *  |chEst        |         |pOutput       |Mpucch_sc* |pComplex Array|(1:0:15)    |Full Range  |     |
 *  |             |         |              |Nul_symb   |              |            |            |     |
 *  --------------------------------------------------------------------------------------------------
 *  |expo_fac     |         |pInput        |1          |Real          |(1:15:0)    |[-16:16)    |     |
 *  ---------------------------------------------------------------------------------------------------
 *  |chMeasurements|        |pOutput       |1          |Real          |(1:15:0)    |[0,63]      |     |                                                                        |
 *  ---------------------------------------------------------------------------------------------------
 *  |slot         |         |Input         |1          |Real          |(1:15:0)    |[0:19]      |     |
 *  --------------------------------------------------------------------------------------------------
 *  |Msc          |         |Input         |1          |Real          |(1:15:0)    |[0:1200]    |     |
 *  --------------------------------------------------------------------------------------------------
 *  |Mrb          |         |Input         |1          |Real          |(1:15:0)    |[0:100]     |     |
 *  --------------------------------------------------------------------------------------------------
 *  |start_address|         |Input         |1          |Real          |(1:15:0)    |[0:100]     |     |
 *  --------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------------
 *  |Block Name          |bs_rx_ChEstimator_PUCCH            |
 *  ---------------------------------------------------------
 *  |Input Streams       |adet,aP_temp                       |
 *  ---------------------------------------------------------
 *  |Output Streams      |adet,chEst,chMeasurements          |
 *  ---------------------------------------------------------
 *  |Config Parameters   |ChanEstPar,MI,row_a,col_a,expo_fac |
 *  |                    |pilotCarrierSymbol,pucch_format,   |
 *  |                    |expo_fac,slot,Msc,Mrb,start_address|
 *  ---------------------------------------------------------
 *  |Status Parameters   |none                               |
 *  ---------------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Channel Estimator for PUCCH:
 * -# For the entire frequency band, do direct calculation of channel estimate at each pilot
 *    point in the pilotCarrierSymbol of the slot, by division of
 *    the received IQ sample by the known pilot data symbol value.
 * -# Do fine timing adjustment to remove the phase rotation with frequency.
 * -# Then extrapolate these channel estimates across all symbols,
 *    to every subcarrier across the complete slot. Note, there is a pilot value
 *    for every frequency point, so no need to interpolate across frequency. Since there is
 *    only one pilot symbol in a slot, there is no opportunity to interpolate across time.
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * [1] 3GPP TS 36.211 v8.7, Sec.5.4, "Uplink: Physical Uplink Control Channel"
 *
 * \ingroup group_lte_rx_uplink_demodulator
 *
 **/
//-------------------------------------------------------------------------------------------
void bs_rx_ChEstimator_PUCCH_part0(S16 *adet, S16 *aPilots, ULRXINPUTPARAMS *pChanEstPar, S16 *chEst,
                             S16 *expo_fac, S16 slot, S32 ChanNum,  S32 *ErrAvg,  
                             S16 *pScaleComputeErr_Tab, S32 *ErrSubAvg, S16 *ErrSubExpo, S32 *ErrVec)
{
    S32 i;
    S16 length_krange;
    S16 Inv_num_pilots;
    S16 p;
    S16 max_expo, min_expo, expo[3];
    S16 Mpucch_sc, Msc;
    S16 Nul_symb;
    S16 num_pilots;
    S16 pucch_format;
    
    S16 *pilotCarrierSymbol;
    S32 L_temp1 = 0, L_temp2 = 0;

    S32 Err[2], ErrUnWt[2];
    S32 *ErrSub, *diffErr,  *temp32Arry;

    S16 Mpucch_rb;
    S32  L_acc1,  L_acc2;

    S16 indexScale, scale1, scale2;
    S16 temp;
    
#if 0
    static FILE *fmyAVG2;
    static short first_open_avg =1;

    if(first_open_avg)
    {
        fmyAVG2 = fopen("MYAVG1.txt","wt");
        first_open_avg = 0;
    }
#endif

    //Copy parameters back out of structure so I don't have to
    //change the code below.
    Mpucch_sc    = pChanEstPar->Nrb_sc;
    Msc          = Mpucch_sc;
    num_pilots   = pChanEstPar->num_Pilots_slot[ChanNum];
    pucch_format = pChanEstPar->pucchFormat[ChanNum];
    Nul_symb     = pChanEstPar->Nul_symb;

    Mpucch_rb = 1;

    Inv_num_pilots = 32767; // num_pilots = 1
    if (num_pilots == 2)
        Inv_num_pilots = 16384;
    else if (num_pilots == 3)
        Inv_num_pilots = 10923;

    if (pucch_format < FORMAT2)// Format 1s
        pilotCarrierSymbol = &Pilot_symbol_loc[0];
    else if (pucch_format == FORMAT2)  //Format 2
        pilotCarrierSymbol = &Pilot_symbol_loc[3];
    else //Format 2a and 2b
        pilotCarrierSymbol = &Pilot_symbol_loc[5];

    length_krange = Mpucch_sc;
    max_expo = -32768;
    for (i = 0; i < Nul_symb; i++)
    {
        if (max_expo < expo_fac[i])
        {
            max_expo = expo_fac[i];
        }
    }
    
    ErrSub = &ErrVec[0];
    diffErr = &ErrVec[2*12];
    temp32Arry = &ErrVec[2*2*12];

    ErrAvg[0] = 0;
    ErrAvg[1] = 0;
    ErrSubAvg[0] = 0;
    ErrSubAvg[1] = 0;

    indexScale = Mpucch_rb - 1;
    scale1 = inverse_nrb[indexScale];
    scale2 = inverse_nrb_expo[indexScale];
    
    for (p = 0; p < num_pilots; p++)
    {
#ifdef CEVA_INTRINSICS
        ComputeComplexConjMult_A(adet + 2 * Msc * pilotCarrierSymbol[p], aPilots + 2*p * Mpucch_sc,
                                    chEst + 2 * p * Mpucch_sc, length_krange);
        Compute_phase_errorPUCCH_A(chEst + 2 * p * Mpucch_sc, Err, Mpucch_rb, length_krange, pScaleComputeErr_Tab, ErrSub, ErrUnWt);
#else
        ComputeComplexConjMult(adet + 2 * Msc * pilotCarrierSymbol[p], aPilots + 2 * p * Mpucch_sc,
                               chEst + 2 * p * Mpucch_sc, length_krange);
        Compute_phase_errorPUCCH(chEst + 2 * p * Mpucch_sc, Err, Mpucch_rb, length_krange, pScaleComputeErr_Tab, ErrSub, ErrUnWt);
#endif

        //Bring to same exponent, before averaging, 2 comes from squaring in Compute_phase_error
        temp = (S16) 2 * (max_expo - expo_fac[pilotCarrierSymbol[p]]);
        Err[0] = L_shr(Err[0], temp);
        Err[1] = L_shr(Err[1], temp);

        //Bring to same exponent, before averaging, 2 comes from squaring in Compute_phase_error
        ErrUnWt[0] = L_shr(ErrUnWt[0], temp);
        ErrUnWt[1] = L_shr(ErrUnWt[1], temp);

        // average over num_pilots
        ErrAvg[0] = L_add(ErrAvg[0], L_mpy_ls(Err[0], Inv_num_pilots));
        ErrAvg[1] = L_add(ErrAvg[1], L_mpy_ls(Err[1], Inv_num_pilots));

        //Same for ErrSub
        for (i = 0; i < Mpucch_sc - 1; i++)
        {
            //Bring to same exponent, before averaging, 2 comes from squaring in Compute_phase_error
             ErrSub[2 * i] = L_shr(ErrSub[2 * i], temp);
             ErrSub[2 * i + 1] = L_shr(ErrSub[2 * i + 1], temp);
        }

        //Normalize by 1/(Mpucch_sc-1)
        L_temp1 = L_shr(L_mpy_ls(ErrUnWt[0], scale1), scale2);
        L_temp2 = L_shr(L_mpy_ls(ErrUnWt[1], scale1), scale2);
        
        for (i = 0; i < Mpucch_sc - 1; i++)
        {
            diffErr[2 * i] = L_sub(L_temp1 , ErrSub[2 * i]);
            diffErr[2 * i + 1] = L_sub(L_temp2 , ErrSub[2 * i + 1]);
        }
        expo[p] = block_lnorm(diffErr, 2*(Mpucch_sc-1), 5); // Get the normalization factor with 5 bit headroom for the summation
#ifdef CEVA_INTRINSICS
        L_acc1 = ComputePhaseDifferencePower_A(diffErr, expo[p], Mpucch_sc - 1);
#else
        L_acc1 = ComputePhaseDifferencePower(diffErr, expo[p], Mpucch_sc - 1);
#endif

        //Bringing ErrAvg to same Q factor and compute its magnitude
#ifdef CEVA_INTRINSICS
        L_acc2 = ComputePhaseDifferencePower_A(ErrUnWt, expo[p], 1);
#else
        L_acc2 = ComputePhaseDifferencePower(ErrUnWt, expo[p], 1);
#endif
        
        //Saving in Temporary storage
        temp32Arry[2 * p] = L_acc1;
        temp32Arry[2 * p + 1] = L_acc2;
    }

    //Since Expo[p] is from normalization, need to take smallest
    min_expo = 32767;
    for (p = 0; p < num_pilots; p++)
    {
        if (min_expo > expo[p])
        {
            min_expo = expo[p];
        }
    }
    
    *ErrSubExpo = min_expo;
    for (p = 0; p < num_pilots; p++)
    {
        //Bring to same exponent, before averaging
        temp = (S16)  2*(expo[p] - min_expo);
        temp32Arry[2 * p] = L_shr(temp32Arry[2 * p], temp);
        temp32Arry[2 * p + 1] = L_shr(temp32Arry[2 * p + 1], temp);

        // average over num_pilots
        ErrSubAvg[0] = L_add(ErrSubAvg[0], L_mpy_ls(temp32Arry[2 * p], Inv_num_pilots));
        ErrSubAvg[1] = L_add(ErrSubAvg[1], L_mpy_ls(temp32Arry[2 * p + 1], Inv_num_pilots));
    }
   
    return;
}
