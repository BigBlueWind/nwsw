//-------------------------------------------------------------------------------------------
/** @file ul_SrsResourceElementAllocation.c
 *
 * @brief Receiver. Physical Uplink Sounding Reference Signal, Resource Element Assignment
 * @author Mindspeed Technologies
 * @version $Revision: 1.20 $
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
#include "prototypes.h"
#include "ext_phy.h"
//#include "heaps.h"

#ifdef CEVA_INTRINSICS
#include <basic_op.h>
#else
#include "basic_op_cc.h"
#endif

//-------------------------------------------------------------------------------------------
/** @brief ul_SrsResourceElementAllocation: Resource Element Allocation for <BR>
 *                                          Sounding Reference Signal.
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param Nul_rb             - Number of Uplink Resource Blocks
 *  @param Nrb_sc             - Number of sub-carriers in resource block
 *  @param k_l_ul_row0        - pointer to row 0 array of (k,l) coordinates for <BR>
 *                              consecutive complex modulation symbols
 *  @param k_l_ul_row1        - pointer to row 1 array of (k,l) coordinates for <BR>
 *                              consecutive complex modulation symbols
 *  @param length_k_l         - length of output arrays
 *  @param pilotSymbol        - Symbol where Pilots are located
 *  @param bandwidth          - B_srs, Tables 5.5.3.2-x in [3GPP TS 36.211, Sec.5.5.3]
 *  @param m_srs_ptr          - pointer to CA_srsBandwidthConfigTable.
 *  @param N_srs_ptr          - pointer to CA_srsBandwidthConfigTable.
 *  @param transmissionComb   - k_tc in [3GPP TS 36.211, Sec.5.5.3.2]
 *  @param freqDomainPosition - n_rrc, in [3GPP TS 36.211, Sec.5.5.3.2]
 *  @param hoppingBandwidth   - b_hop, in [3GPP TS 36.211, Sec.5.5.3.2]
 *  @param UpPtsFlag          - flag of UpPts

 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name              |Name in  |Direction |Length     |Type       |Data Format |Valid Range |Units |
 *  |                  |Standard |          |           |           |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |Nul_rb            |         |Input     |1          |Real       |(1:15:0)    |[0:99]      |      |
 *  -------------------------------------------------------------------------------------------------
 *  |Nrb_sc            |         |pInput    |1          |pStructure |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |k_l_ul_row0       |         |pOutput   |length_k_l |pReal Array|(1:15:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |k_l_ul_row1       |         |pOutput   |length_k_l |pReal Array|(1:15:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |length_k_l        |         |Output    |1          |pReal      |(1:15:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |pilotSymbol       |         |Output    |1          |pReal      |(1:15:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |m_srs_ptr         |         |Output    |1          |pReal      |(1:15:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |N_srs_ptr         |         |Output    |1          |pReal      |(1:15:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |transmissionComb  |         |Output    |1          |pReal      |(1:15:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |freqDomainPosition|         |Output    |1          |pReal      |(1:15:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |hoppingBandwidth  |         |Output    |1          |pReal      |(1:15:0)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------------------
 *  |Block Name          |ul_SrsResourceElementAllocation         |
 *  ---------------------------------------------------------------
 *  |Input Streams       |none                                    |
 *  ---------------------------------------------------------------
 *  |Output Streams      |k_l_ul_row0,k_l_ul_row1                 |
 *  ---------------------------------------------------------------
 *  |Config Parameters   |iSig,ul_srs,ulChan_srs,Msymb,Nul_rb,    |
 *  |                    |Nrb_sc,Nul_symb                         |
 *  ---------------------------------------------------------------
 *  |Status Parameters   |length_kl                               |
 *  ---------------------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Resource Element Allocation for Sounding Reference Signal.
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * [1] 3GPP TS 36.211, Rel.8, Sec.5.5.3.2, "Sounding reference signal: <BR>
 *     Mapping to physical resources"
 *
 * \ingroup group_lte_rx_uplink_demodulator
 *
 **/
void ul_SrsResourceElementAllocation(S16 Nul_rb, S16 Nrb_sc, S16 frameNumber,S16 subframeNumber,  S16 *startRE,  S16 *numRE,  S16 bandwidth,
                                     S16 *m_srs_ptr, S16 *N_srs_ptr, S16 transmissionComb,
                                     S16 freqDomainPosition, S16 hoppingBandwidth, S16 srsConfigIndex)
{
    S16 B_srs;
    S16 m_srs[4], Mrs_sc[4]; // since Max B_srs = 3;
    S16 N_srs[4], n[4];
    S16 k_tc;
    S16 n_rrc;
    S16 b_hop;
   
    S16 i,temp_nb;
    S16 kDash_0, k0, sum, b;
    S16 n_srs, Fb_srs;
    S16 T_srs; // 3GPP TS 36.213, Rel.8, Sec.8.2


    //S16 frameNumber = 0;
    //S16 subframeNumber = 0;

    // Extract SRS parameters from input structures
    B_srs = bandwidth;
    k_tc = transmissionComb;
    n_rrc = freqDomainPosition; //first resource block where SRS starts
    b_hop = hoppingBandwidth;

    // T_srs: SRS Periodicity, 3GPP TS 36.213, Rel.8, Sec.8.2 
    if(srsConfigIndex <= 1)
        T_srs = 2;
    else if ((srsConfigIndex >= 2) && (srsConfigIndex <= 6))
        T_srs = 5;
    else if ((srsConfigIndex >= 7) && (srsConfigIndex <= 16))
        T_srs = 10;
    else if ((srsConfigIndex >= 17) && (srsConfigIndex <= 36))
        T_srs = 20;
    else if ((srsConfigIndex >= 37) && (srsConfigIndex <= 76))
        T_srs = 40;
    else if ((srsConfigIndex >= 77) && (srsConfigIndex <= 156))
        T_srs = 80;
    else if ((srsConfigIndex >= 157) && (srsConfigIndex <= 316))
        T_srs = 160;
   // else if ((srsConfigIndex => 317) && (srsConfigIndex <= 636))
    else
        T_srs = 320;

    for (b = 0; b <= B_srs; b++)
    {
        m_srs[b] = m_srs_ptr[2 * b];
        N_srs[b] = N_srs_ptr[2 * b];
    }

    //init fixed SRS location parameters
   // lIndex = pilotSymbol; //symbol index in subframe (0 to 13). SRS is only sent in one symbol in a subframe

    for (b = 0; b <= B_srs; b++)
    {
        Mrs_sc[b] = m_srs[b] * Nrb_sc / 2;
    }

    if (b_hop >= B_srs)
    {
        //frequency hopping of SRS is disabled
        for (b = 0; b <= B_srs; b++)
        {
            n[b] = divide_floor((short)(4*n_rrc),(short)m_srs[b]) % N_srs[b];

        }
    }
    else //frequency hopping of SRS is enabled
    {
        for (b = 0; b <= B_srs; b++)
        {
            if(b <= b_hop)
            {
                n[b] = divide_floor((short)(4*n_rrc),(short)m_srs[b]) % N_srs[b];
            }
            else
            {
                n_srs = divide_floor((short) (10*frameNumber + subframeNumber), (short) (T_srs));

                temp_nb = 1;
                for(i = b_hop+1; i<b; i++)
                    temp_nb *= N_srs[i];

                // Fb_srs
                if((N_srs[b] % 2) == 0) // Nb is even
                {
                    Fb_srs = shr(N_srs[b],1) * divide_floor((short)(n_srs % (temp_nb*N_srs[b])),(short) temp_nb) + divide_floor((short)(n_srs % (temp_nb*N_srs[b])),(short) (2*temp_nb));
                }
                else
                {
                    Fb_srs = divide_floor((short)(n_srs),(short) temp_nb);
                    Fb_srs *= shr(N_srs[b],1); 
                }

                n[b] = (divide_floor((short)(4*n_rrc),(short)m_srs[b]) + Fb_srs) % N_srs[b];
            }
        }
    }

    kDash_0 = (divide_floor(Nul_rb, 2) - m_srs[0] / 2) * Nrb_sc + k_tc;
    sum = 0;
    for (b = 0; b <= B_srs; b++)
    {
        sum += 2 * Mrs_sc[b] * n[b]; // sum(2 * Mrs_sc .* n)
    }
    k0 = kDash_0 + sum; //frequency index for first resource element in
    //the first resource block for this SRS allocation
    if (k0 < 0)
    {
        k0 = 0; // ????
    }

	*startRE = k0;

    *numRE = Mrs_sc[B_srs];

   // MemFree(k_Vec, CACHEABLE);

    //Check for errors
    //    if (Msymb != length_kVec)
    //    {
    //	    printf("Mrs_sc=%hd, length_kVec=%hd\n", Msymb, length_kVec);
    //        printf("error in PUSCHResourceElementAssignment(): Number of ResourceElements\
//            in Physical Channel, length_kVec =Mrs_sc, does not match Msymb");
    //    }

    return;
}

