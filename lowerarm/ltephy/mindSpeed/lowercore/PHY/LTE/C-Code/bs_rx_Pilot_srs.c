//-------------------------------------------------------------------------------------------
/** @file bs_rx_Pilot_srs.c
 *
 * @brief Generate Uplink SRS (sounding reference signals)
 * @author Mindspeed Technologies
 * @version $Revision: 1.24 $
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

#ifdef CEVA_INTRINSICS
#include <basic_op.h>
#else
#include "basic_op_cc.h"
#endif

//-------------------------------------------------------------------------------------------
/** @brief Generate Uplink SRS (sounding reference signals)
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param subframeNumber          - INPUT, current subframeNumber
 *  @param Nrb_sc                  - INPUT, Number of resource element in a RB
 *  @param Ncell_id                - INPUT, Sector Cell_id
 *  @param groupHoppingEnabled     - INPUT, group Hopping Enabled flag
 *  @param u                       - INPUT, Sequence-group hopping number,u(0:1) = cell-specific base sequence value
 *  @param rSrs                    - OUTPUT, SRS Pilots
 *  @param m_srs                   - INPUT, pointer to m_srs value in srsBandwidthConfigTable
 *  @param N                       - INPUT, pointer to N value in srsBandwidthConfigTable
 *  @param nCs_srs                 - INPUT, pointer to UE dependent cyclic shift
 *  @param bandwidth               - INPUT, pointer to srs bandwidth value in srsBandwidthConfigTable
 *  @param numChan                 - INPUT, number of SRS channels


 *
 *  @return  None
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------------
 *  |Name                   |Name in  |Direction     |Length      |Type         |Data Format |Valid Range |Units |
 *  |                       |Standard |              |            |             |            |            |      |
 *  |subframeNumber         |         |Input        |1            |short        |            |            |      |
 *  --------------------------------------------------------------------------------------------------------------
 *  |Nrb_sc                 |         |Input        |1            |short        |            |            |      |
 *  --------------------------------------------------------------------------------------------------------------
 *  |Ncell_id               |         |Input        |1            |short        |            |            |      |
 *  --------------------------------------------------------------------------------------------------------------
 *  |groupHoppingEnabled    |         |Input        |1            |short        |            |            |      |
 *  --------------------------------------------------------------------------------------------------------------
 *  |u                      |         |pInput        |2           |Array Pointer|(0:16:0)    |            |      |
 *  --------------------------------------------------------------------------------------------------------------
 *  |rSrs                   |         |pOutput       |length_rSrs |Complex Array|(1:15:0)    |            |      |
 *  --------------------------------------------------------------------------------------------------------------
 *  |m_srs                  |         |pInput        |2*numSrs    |short        |(1:0:15)    |[4:96]      |      |
 *  --------------------------------------------------------------------------------------------------------------
 *  |N                      |         |pInput        |2*numSrs    |short        |(1:0:15)    |[0:6]       |      |
 *  --------------------------------------------------------------------------------------------------------------
 *  |nCs_srs                |         |pInput        |2*numSrs    |short        |(1:0:15)    |[0:7]       |      |
 *  --------------------------------------------------------------------------------------------------------------
 *  |bandwidth              |         |pInput        |2*numSrs    |short        |(1:0:15)    |[0:3]       |      |
 *  --------------------------------------------------------------------------------------------------------------
 *  |numChan                |         |Input         |1           |short        |            |            |      |
 *  --------------------------------------------------------------------------------------------------------------
 *
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------------------
 *  |Block Name          |bs_rx_Pilot_srs                         |
 *  ---------------------------------------------------------------
 *  |Input Streams       |u, m_srs, N, nCs_srs, bandwidth         |
 *  ---------------------------------------------------------------
 *  |Output Streams      |rSrs                                    |
 *  ---------------------------------------------------------------
 *  |Config Parameters   |subframeNumber, Nrb_sc, Ncell_id,       |
 *  |                    |groupHoppingEnabled, numChan            |
 *  ---------------------------------------------------------------
 *  |Status Parameters   |length_rSrs                             |
 *  ---------------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Generate Uplink SRS (sounding reference signals)
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.5.5.3, "Sounding Reference Signal"<BR>
 *
 *  \ingroup group_lte_rx_uplink_demodulator
 */
//-------------------------------------------------------------------------------------------
void bs_rx_Pilot_srs(S16 subframeNumber, S16 Nrb_sc, U32 Ncell_id, S16 groupHoppingEnabled, S16 sequenceHopping, S16 *u, S16 *v, S16 delta_ss, 
                     S16 *rSrs, S16 *m_srs, S16 *N, S16 *nCs_srs, S16 *bandwidth, S16 numChan, S16 pUlPilots_QthRootTable[], U8 cvec[], S16 fgh[])
{
    S16 iChan, i;
    
    S32 length_cvec;
    S16 ns;
    S16 fPUCCH_ss, fPUSCH_ss , fss;
	U32 cinit;
	S16 SlotNum, temp;
    S16 Mrs_sc;

    temp = 6 * Nrb_sc;
	SlotNum = 2*subframeNumber;
    
    //        gen_ul_SequenceGroupNumber_U((S16) Ncell_id, groupHoppingEnabled, u);
    fPUCCH_ss = Ncell_id % 30;
    fss = fPUCCH_ss;
	fPUSCH_ss = (fPUCCH_ss + delta_ss) % 30;
	memset(v, 0, numChan * NUM_SLOTS * 2);

    // Generate Random sequence for entire frame of length 8*7*20
    if (groupHoppingEnabled == 0) //Group Hopping is disabled
    {
		length_cvec = NUM_SLOTS;
		cinit = 32 *(Ncell_id / 30) + fPUSCH_ss;
		PseudoRandomSeqGen(length_cvec, cinit, cvec);
        for (ns = SlotNum; ns < SlotNum + 2; ns++)
		{
            fgh[ns] = 0;
		    
            for (iChan = 0; iChan < numChan; iChan++)
            {     
                 i = 2 * bandwidth[iChan];
                 Mrs_sc = m_srs[i] * Nrb_sc / 2; //Mrs_sc is always an integer multiple of Nrb_sc 
		        //Group Hopping is disabled && sequenceHopping enabled
                if ((sequenceHopping == 1) && (Mrs_sc >= temp)) 
                {
                    v[ns + NUM_SLOTS * iChan] = cvec[ns];
                }
            }
            
		}
    }
    else
    {
        length_cvec = 8 * 7 * NUM_SLOTS;
        PseudoRandomSeqGen(length_cvec, Ncell_id / 30, cvec);
        for (ns = SlotNum; ns < SlotNum + 2; ns++)
        {
            fgh[ns] = 0;
            for (i = 0; i <= 7; i++)
            {
                fgh[ns] += cvec[8 * ns + i] * (shl(1, i));
            }
            fgh[ns] = fgh[ns] % 30;
        }
    }

    for (ns = SlotNum; ns < SlotNum + 2; ns++)
    {
        u[ns] = (fgh[ns] + fss) % 30; //Sequence-group hopping number [TS 36.211 v.8.4.0, Sec.5.5.1.3]
    }
    // End modified the calculation of "u" to support TDD
    for (iChan = 0; iChan < numChan; iChan++)
    {
        i = 2 * bandwidth[iChan];
        gen_ul_Pilot_srs_info(Nrb_sc, m_srs[i], nCs_srs[iChan], u + SlotNum,
                              v + SlotNum + NUM_SLOTS * iChan, rSrs + m_srs[i] * Nrb_sc * iChan, pUlPilots_QthRootTable);
    } //ichan

    return;
}

