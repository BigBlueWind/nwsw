#if !defined _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

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

//#include "basic_op_cc.h"
#ifdef CEVA_INTRINSICS
    #include <basic_op.h>
    #include <asm-insn.h>
    #include "global.h"
#else
    #include "basic_op_cc.h"
#endif

#define CEVA_BUILD 0


//-------------------------------------------------------------------------------------------
/** @brief Main function to do SRS receiver
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *
 *  @param   df                  - frequency spacing between subcarriers
 *  @param   Fs                  - sampling frequency
 *  @param   Nrb_sc              - Number of subcarriers in resource block
 *  @param   NusedE              - Number of subcarriers used
 *  @param   Nfft                - Size of FFT
 *  @param   *aDet               - Input IQ samples
 *  @param   numSrs              - Number of SRS signals present in subframe
 *  @param   mo                  - Number of Rx Antennas
 *  @param   K                   - Total number of Subcarriers in symbol
 *  @param   L                   - Total number of symbols in subframe
 *  @param   Nul_rb              - Number of resource blocks allocated
 *  @param   srsPilotSymbol      - Location of SRS Pilots
 *  @param   *srsChEst           - Output of SRS Channel Estimate
 *  @param   *rawSrsChEst        - Output of Raw SRS channel Estimate
 *  @param   *srsPilots          - SRS Pilots
 *  @param   *length_srsPilots   - Length of each SRS Pilots
 *  @param   stepsize_aDet       - Number of samples to skip to reach IQ samples of next Rx Antenna
 *  @param   *cyclicShift        - nCs_srs, in [3GPP TS 36.211, Sec.5.5.3.1]
 *  @param   *bandwidth          - B_srs, Tables 5.5.3.2-x in [3GPP TS 36.211, Sec.5.5.3]
 *  @param   *m_srs              - pointer to CA_srsBandwidthConfigTable.
 *  @param   *N                  - pointer to CA_srsBandwidthConfigTable.
 *  @param   *transmissionComb   - UE specific offset value based on SRS transmission comb, k_tc in [3GPP TS 36.211, Sec.5.5.3.2]
 *  @param   *freqDomainPosition - first resource block where SRS starts, n_rrc, in [3GPP TS 36.211, Sec.5.5.3.2]
 *  @param   *hoppingBandwidth   - HoppingBandwidth configuration for each srs channel, b_hop, in [3GPP TS 36.211, Sec.5.5.3.2]
 *  @param   *cdmEnable          - 1= multiple MS are sharing this frequency allocation
 *  @param   *cdmGroupIndex      - index to group of channels combined on the same frequency-time resources,
 *                                 where cdm = code division multiplexed
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------------
 *  |Name                |Name in  |Direction     |Length          |Type          |Data Format |Valid Range |Units |
 *  |                    |Standard |              |                |              |            |            |      |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |df                  |         |Input         |1               |short         |(1:15:0)    |Full Range  |      |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |Fs                  |         |Input         |1               |short         |(1:15:0)    |Full Range  |      |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |Nrb_sc              |         |Input         |1               |short         |(1:15:0)    |Full Range  |      |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |NusedE              |         |Input         |1               |short         |(1:15:0)    |Full Range  |      |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |Nfft                |         |Input         |1               |short         |(1:15:0)    |Full Range  |      |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |*aDet               |         |pInput        |1               |pComplex      |(1:0:15)    |Full Range  |      |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |numSrs              |         |Input         |1               |short         |(1:15:0)    |[0:13]      |      |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |mo                  |         |Input         |1               |short         |(1:15:0)    |[1,2,4]     |      |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |K                   |         |Input         |1               |short         |(1:15:0)    |Full Range  |      |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |L                   |         |Input         |1               |short         |(1:15:0)    |[0:13]      |      |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |Nul_rb              |         |Input         |1               |short         |(1:15:0)    |Full Range  |      |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |srsPilotSymbol      |         |Input         |1               |short         |(1:15:0)    |[0:13]      |      |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |*srsChEst           |         |pOutput       |1200*mo         |pComplex      |(1:0:15)    |Full Range  |      |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |*rawSrsChEst        |         |pOutput       |1200*mo         |pComplex      |(1:0:15)    |Full Range  |      |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |*srsPilots          |         |pOutput       |length_srsPilots|short         |(1:0:15)    |Full Range  |      |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |*length_srsPilots   |         |pInput        |numSRS          |short         |(1:15:0)    |Full Range  |      |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |stepsize_aDet       |         |Input         |1               |short         |(1:0:15)    |Full Range  |      |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |*cyclicShift        |         |pInput        |2*numSrs        |short         |(1:0:15)    |[0:7]      |      |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |*bandwidth          |         |pInput        |2*numSrs        |short         |(1:0:15)    |[0:3]      |      |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |*m_srs              |         |pInput        |2*numSrs        |short         |(1:0:15)    |[4:96]     |      |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |*N                  |         |pInput        |2*numSrs        |short         |(1:0:15)    |[0:6]      |      |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |*transmissionComb   |         |pInput        |2*numSrs        |short         |(1:0:15)    |[0:1]      |      |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |*freqDomainPosition |         |pInput        |2*numSrs        |short         |(1:0:15)    |[0 1188]   |      |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |*hoppingBandwidth   |         |pInput        |2*numSrs        |short         |(1:0:15)    |[0:3]      |      |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |*cdmEnable          |         |pInput        |1               |short         |(1:0:15)    |[0:1]      |      |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |*cdmGroupIndex      |         |pInput        |1               |short         |(1:0:15)    |[0:8]      |      |
 *  ----------------------------------------------------------------------------------------------------------------
 *  TABLE_END *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------------------------
 *  |Block Name          |SRSReceiverProcessing                         |
 *  ---------------------------------------------------------------------
 *  |Input Stream        |aDet,srsPilots                                |
 *  ---------------------------------------------------------------------
 *  |Output Stream       |srsChEst,rawSrsChEst,                         |
 *  |                    |rawSrsChEst                                   |
 *  ---------------------------------------------------------------------
 *  |Config Parameters   |df,Fs,Nrb_sc,NusedE,Nfft,numSrs,              |
 *  |                    |mo,,K,L,Nul_rb,                               |
 *  |                    |srsPilotSymbol,length_srsPilots,stepsize_aDet,|
 *  |                    |cyclicShift,bandwidth,m_srs,N,                |
 *  |                    |transmissionComb,freqDomainPosition,          |
 *  |                    |hoppingBandwidth,cdmEnable,cdmGroupIndex      |
 *  ---------------------------------------------------------------------
 *  |Status Parameters   |none                                          |
 *  ---------------------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Top level function to do Sounding Reference Signal Receiver for BaseStation Uplink.
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.5.5.3, "Sounding Reference Signal"<BR>
 *
 *
 *  \ingroup group_lte_rx_uplink_demodulator
 *
 **/
//-------------------------------------------------------------------------------------------
void SRSReceiverProcessing(S16 df, U32 Fs, S16 Nrb_sc, S16 NusedE, S16 Nfft,
                           S16 srsBandwidthTab, S16 srsBandwidthConfig, S16 *psrsBandwidthConfigTable,
                           S16 *aDet, S16 numSrs, S16 MO,
                           S16 K, S16 L, S16 Nul_rb, S16 srsPilotSymbol, S16 *srsChEst,
                           S16 *rawSrsChEst, S16 *srsPilots,
                           S16 *length_srsPilots, S32 stepsize_aDet, S16 *cyclicShift,
                           S16 *bandwidth, S16 *transmissionComb,
                           S16 *freqDomainPosition, S16 *hoppingBandwidth, S16 *cdmEnable,
                           S16 *cdmGroupIndex, S16 *temp_vec, S16 *pScaleComputeErr_Tab,
                           S32 *snrPerRb, S16 *StartRb, S16 *EndRb, PBSRXCHMeas pSrsChMeas,
                           S16 **pTable_DFT, S16 *srsConfigIndex,S16 frameNumber,S16 subframeNumber)
{
    S32 iSig, iSigEnd, lenCdmGroup;
    S32 i, k, length_iSigGroup;
    S16 numRE[MAX_UL_SRS], startRE[MAX_UL_SRS], Cdm_group_pair[MAX_UL_SRS];
    S16 *iSigGroup, *aEstSig;
    S16 *p;

    S16 *m_srs_ptr, *N_ptr;
    S16 numRbSrs, RbLocation;
    S16 *IDFT_Table_Array;
    S16 pilot_length;
    //S32 noise_power_total[MAX_UL_SRS];
    S16 numb_SRS_group;
    //S32 noise_power_temp,noise_power_temp1;
    S32 noise_power_ave;
    S16 Msrs_sc;
    S32 Ltmp0;
    S32 snrPilot;
    S16 Cdm_group_total;
    S16 temp_subc_index;
    S16 empty_sub_indicator;
    S32 srs_start_point;

    S16 *srs_ploits_internal;
    S16 *raw_data_internal;
    S16 *rawSrsChEst_internal;
    S16 *srsChEst_internal;

    S32 xNRB_fx;
    S16 xN_fx;
    S16 mo;
    S16 noise_norm_exp = 0;

    srs_start_point = stepsize_aDet * srsPilotSymbol;
    iSigGroup = temp_vec;
    aEstSig = (S16 *) &(temp_vec[MAX_UL_SRS]);
    p = aDet;

    pilot_length = 0;
    numRbSrs = K/Nrb_sc;
    m_srs_ptr = &psrsBandwidthConfigTable[64 * srsBandwidthTab + 8 * srsBandwidthConfig];
    N_ptr = &psrsBandwidthConfigTable[64 * srsBandwidthTab + 8 * srsBandwidthConfig + 1];

    //---------------------------------------------------------------------------
    // allocate memory
    srs_ploits_internal = (S16 *) &(aEstSig[MAXSUBCARRIERS]);
    raw_data_internal = (S16 *) &(srs_ploits_internal[MAXSUBCARRIERS]);
    rawSrsChEst_internal = (S16 *) &(raw_data_internal[2 * MAXSUBCARRIERS]);
    srsChEst_internal = (S16 *) &(rawSrsChEst_internal[MAXSUBCARRIERS]);

    // allocate memory for DFT talbe and DMA data in;
    IDFT_Table_Array = (S16 *) &(srsChEst_internal[MAX_UL_SRS * MAXSUBCARRIERS/2]);
    IDFT_Ceva_Setup_Tables(IDFT_Table_Array,pTable_DFT, length_srsPilots[0]);

    //---------------------------------------------------------------------------
    // find all SRS's location and group CDM SRS together;
    for(iSig = 0; iSig < numSrs; iSig++)
    {
        ul_SrsResourceElementAllocation(Nul_rb, Nrb_sc,frameNumber, subframeNumber, startRE+iSig, numRE+iSig,
                                        bandwidth[iSig], m_srs_ptr, N_ptr,
                                        transmissionComb[iSig], freqDomainPosition[iSig],
                                        hoppingBandwidth[iSig],srsConfigIndex[iSig]);
    }

    cdmGroupIndex[0] = 0;
    Cdm_group_pair[0] = 0;
    Cdm_group_total = 0;
    for(i = 0; i < numSrs; i++)
    {
        if(Cdm_group_pair[i] != 1)
        {
            cdmGroupIndex[i] = Cdm_group_total;
            for(k = i+1; k < numSrs; k++)
            {
                if(startRE[i] == startRE[k])
                {
                    cdmGroupIndex[k] = cdmGroupIndex[i];
                    Cdm_group_pair[k] = 1;
                    cdmEnable[k] = 1;
                    cdmEnable[i] = 1;
                }
                else
                    break;
            }
            Cdm_group_pair[i] = 1;
            Cdm_group_total++;
        }
    }

    //---------------------------------------------------------------------------
    // Do SRS Receiver Processing. These following functions are repeated independently for each rx antenna
    // loop for each antenna
    for (mo = 0; mo < MO; mo++)
    {
        iSig = 0;
        numb_SRS_group = 0;
        while (iSig <= numSrs - 1)
        {
            //Determine CDM Group information
            lenCdmGroup = 0;
            iSigEnd = iSig;
            if (cdmEnable[iSig] == 1)
            {
                while ((iSigEnd+1 <= numSrs - 1) && (cdmGroupIndex[iSigEnd+1] == cdmGroupIndex[iSig]))
                {
                    lenCdmGroup = lenCdmGroup + 1;
                    iSigEnd = iSigEnd + 1;
                }
            }

            // DMA  SRS pilot in
            MemCpyFromExt((void*) srs_ploits_internal, (
                           void*) &(srsPilots[2 * iSig * length_srsPilots[iSig]]), numRE[iSig]*4);

            length_iSigGroup = 0;
            for (i = 0; i <= iSigEnd - iSig; i++)
            {
                iSigGroup[i] = (S16) (iSig + i); //iSig:iSigEnd;
                length_iSigGroup++;
            }

            RbLocation = startRE[iSig]/Nrb_sc;
            for(k = iSig; k < iSig + length_iSigGroup ; k++)
            {
                StartRb[k] = RbLocation;
                EndRb[k] = RbLocation + numRE[iSig]/(Nrb_sc / 2);
            }

            pilot_length = length_srsPilots[0];

            //Demux IQ demod samples for this SRS CSM Group, for this rx antenna
            //DMA data in
            p = &(aDet[mo * stepsize_aDet * L + srs_start_point + 2 *(startRE[iSig])]);
            MemCpyFromExt((void*) raw_data_internal, (void*) p, numRE[iSig]*8);

            for (i = 0; i < numRE[iSig]; i++)
            {
                aEstSig[2 * i] = raw_data_internal[4 * i];
                aEstSig[2 * i + 1] = raw_data_internal[ 4 *i + 1];
            }

            // srsChEst_internal is for calculating channel power, edge subcarriers are replaced by center subcarriers
            // rawSrsChEst is Rx * conj(srs_pilot)
            bs_rx_ChEstimatorSrs(iSigGroup[0], (S16) (length_iSigGroup), cyclicShift, aEstSig,
                                 srs_ploits_internal, df, Fs,
                                 Nrb_sc, NusedE, Nfft, startRE[iSig], (short) (-2), Nul_rb,
                                 length_srsPilots[iSig],
                                 &srsChEst_internal[ 2*iSig*pilot_length], rawSrsChEst_internal, pScaleComputeErr_Tab,
                                 pSrsChMeas,IDFT_Table_Array);

            // DMA the channel estimation results out
            MemCpyToExt((void*) &(srsChEst[(mo * 2 * K * numSrs) + 2*iSig*pilot_length]), (void*) &(srsChEst_internal[2*iSig*pilot_length]), numRE[iSig]*4*length_iSigGroup);

            iSig = iSigEnd+1; //update iSig for the CDM group of channels processed
            numb_SRS_group = numb_SRS_group + 1;
        } //iSig, srs channel

        // calculate noise power from empty frequency domain subcarriers
        // find empty subcarriers
        empty_sub_indicator = 0;
        temp_subc_index = 0;
        for(i = 0; i < numSrs; i++)
        {
            empty_sub_indicator = 0;
            if(transmissionComb[i] == 1)
                temp_subc_index = startRE[i] - 1;
            else
                temp_subc_index = startRE[i] + 1;

            for(k = 0; k < numSrs; k++)
            {
                if(temp_subc_index == startRE[k])
                {
                    empty_sub_indicator = 1;
                    break;
                }
            }

            if((k == numSrs) && (empty_sub_indicator == 0))
            {
                empty_sub_indicator = 0;
                break;
            }
        }

        //  noise power in freq domain if we have empty subcarriers
        p = &(aDet[mo * stepsize_aDet * L]);
        noise_power_ave = 1;
        if(empty_sub_indicator == 0)
        {
            // read in data from CRAM to rawSrsChEst_internal; rawSrsChEst_internal will be pure noise
            for (i = 0; i < pilot_length; i++)
            {
                 rawSrsChEst_internal[2*i] = p[srs_start_point + 2 *(temp_subc_index + 2*i)];
                 rawSrsChEst_internal[2*i + 1] =  p[srs_start_point + 2 *(temp_subc_index + 2*i) + 1];
            }

            // block norm rawSrsChEst_internal
            noise_norm_exp = block_norm(rawSrsChEst_internal,rawSrsChEst_internal,pilot_length*2,6);

            xNRB_fx = 0;
            for (i = 0; i < pilot_length; i++)
            {
                xN_fx = rawSrsChEst_internal[2*i];
                xNRB_fx = L_mac(xNRB_fx, xN_fx, xN_fx);
                xN_fx = rawSrsChEst_internal[2*i + 1];
                xNRB_fx = L_mac(xNRB_fx, xN_fx, xN_fx);
            }
            noise_power_ave = L_shl(xNRB_fx,-2);
        }

        noise_power_ave = noise_power_ave / (pilot_length/24);
        if(noise_power_ave == 0)
        {
            noise_power_ave = 1;
            noise_norm_exp = 0;
        }

        // SNR calculation
        for (iSig = 0; iSig < numSrs; iSig++)
        {
            RbLocation = startRE[iSig]/Nrb_sc;
            Msrs_sc = (pilot_length);
            memset(&snrPerRb[RbLocation + iSig*numRbSrs + (mo * numRbSrs * numSrs)], 0, 4 * (Msrs_sc/6));

            ComputeSnrReportingInfoSRS(rawSrsChEst, srsChEst_internal + 2 * iSig * Msrs_sc,
                1, Msrs_sc, &snrPerRb[RbLocation + iSig*numRbSrs + (mo * numRbSrs * numSrs)],
                &snrPilot, &Ltmp0, 6, MIN_SRS_RB, temp_vec, pilot_length/6,
                noise_power_ave ,noise_norm_exp); // Ltmp0 is dummy noiseVar not used for SRS yet
         }
     } // mo

     // debug output to files
#if 0
    {
        FILE *fp;
        fp = (FILE *) fopen( "srs_data_raw_in_noise.dat", "at+");
        for ( i=0; i < pilot_length; i++ )
        {
             fprintf( fp, "%d %d\n",  rawSrsChEst_internal[2 * i], rawSrsChEst_internal[2 * i+1]);
        }
        fclose(fp);
    }
#endif
}
