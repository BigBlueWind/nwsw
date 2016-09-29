//-------------------------------------------------------------------------------------------
/** @file dl_Pilot.c
 *
 * @brief LTE base station Reference Signal Generator (Pilot)
 * @author Mindspeed Technologies
 * @version $Revision: 1.41 $
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
#define MAX_NDLRB	100
extern U8 *DataBuff_Bank1;
#else
#include "basic_op_cc.h"
#endif

/*
 ;ARM-Ceva interface for LTE TX DlPilot function
 ;
 ;ARM and Ceva use the data structure struct tTCB for passing parameters and data.
 ;The parameters for the LTE TX DlPilot are described in the followings.
 ;
 ;When calling the LTE TX DlPilot, ARM should create the data structure and
 ;allocate memories to pointers. The usage of the data structure is described in
 ;the followings.
 ;
 ;(1) TaskID
 ;    TASKID_TX_DlPilot
 ;
 ;(2) Total of 9 input/output parameters:
 ;void DlPilot(Ncell_id, CPType, Ndl_rb, Ndl_symb, Nrb_sc,
 ;           Ntx_antennas, pilotScaleFactor, slotNumber, a);
 ;
 ;(3) Input Parameters:
 ;    InParam    - Input Data Structure, length = 6 bytes
 ;    Ntx_ant    - Input, length = 2 bytes, S16
 ;    slotNumber - Input, length = 2 bytes, S16
 ;
 ;(4) Input/Output Parameters:
 ;    a[]           - Input/Output Stream, length = row_a*col_a*2*2, S16 (IQ Samples)
 */

//-------------------------------------------------------------------------------------------
/** @brief Add Downlink Reference (pilot) signals to the resource grid
 *
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param Ncell_id         = Cell ID
 *  @param CPType           = Cyclic Prefix Type (Normal = 0 or Extended = 1)
 *  @param Ndl_rb           = Number of DL resource Blocks
 *  @param Ndl_symb         = Number of DL symbols per slot
 *  @param Nrb_sc           = Number of sub carriers per resource block
 *  @param Ntx_antennas     = Number of TX Antennas
 *  @param pilotScaleFactor = Pilot Scale Factor
 *  @param isym             = Current Symbol Number
 *  @param slotNumber       = Current Slot Number
 *  @param a                = Resource Grid (final output structure)
 *
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name             |Name in  |Direction     |Length        |Type          |Data Format|Valid Range |Units |
 *  |                 |Standard |              |              |              |           |            |      |
 *  ----------------------------------------------------------------------------------------------------------
 *  |Ncell_id         |         |Input         |1             |signed short  |(1:15:0)   |Full Range  |      |
 *  ----------------------------------------------------------------------------------------------------------
 *  |CPType           |         |Input         |1             |signed short  |(1:15:0)   |[0,1]       |      |
 *  ----------------------------------------------------------------------------------------------------------
 *  |Ndl_rb           |         |Input         |1             |signed short  |(1:15:0)   |[0,100]     |      |
 *  ----------------------------------------------------------------------------------------------------------
 *  |Ndl_symb         |         |Input         |1             |signed short  |(1:15:0)   |[0,7]       |      |
 *  ----------------------------------------------------------------------------------------------------------
 *  |Nrb_sc           |         |Input         |1             |signed short  |(1:15:0)   |[0,12]      |      |
 *  ----------------------------------------------------------------------------------------------------------
 *  |Ntx_antennas     |         |Input         |1             |signed short  |(1:15:0)   |[1,2,4]     |      |
 *  ----------------------------------------------------------------------------------------------------------
 *  |pilotScaleFactor |         |Input         |1             |signed short  |(1:15:0)   |Full Range  |      |
 *  ----------------------------------------------------------------------------------------------------------
 *  |isym             |         |Input         |1             |signed short  |(1:15:0)   |[0,13]      |      |
 *  ----------------------------------------------------------------------------------------------------------
 *  |slotNumber       |         |Input         |1             |signed short  |(1:15:0)   |[0,19]      |      |
 *  ----------------------------------------------------------------------------------------------------------
 *  |a                |         |pInput/pOutput|Nrb_sc*Ndl_rb |pComplex Array|(1:1:14)   |Full Range  |      |
 *  |                 |         |              |Ndl_symb*2    |              |           |            |      | 
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |DlPilot                          |
 *  --------------------------------------------------------
 *  |Input Streams       |a                                |
 *  --------------------------------------------------------
 *  |Output Streams      |a                                |
 *  --------------------------------------------------------
 *  |Config Parameters   |Ncell_id,CPType,Ndl_rb,          |
 *  |                    |Ndl_symb,Nrb_sc,Ntx_antennas,    |
 *  |                    |pilotScaleFactor,isym,slotNumber |
 *  --------------------------------------------------------
 *  |Status Parameters   |none                             |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Add Downlink Reference (pilot) signals to the resource grid [3GPP TS 36.211 v8.7, Sec.6.10]
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * [1] 3GPP TS 36.211 v8.7, Sec.6.10, "Downlink: Reference Signals"
 *
 * \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-------------------------------------------------------------------------------------------
void DlPilot(S16 Ncell_id, S16 CPType, S16 Ndl_rb, S16 Ndl_symb, S16 Nrb_sc, S16 Ntx_antennas,
             S16 pilotScaleFactor, S16 isym, S16 slotNumber, S16* a[])
{

    S32 p;
    S16 vshift, v;
    S16 cond1, cond2;
    U8* c;
    S32 index4, const1, const3;
    U32 cinit;
    S32 const4, const5;
    S16 lpLim = Ntx_antennas;
    S16 scaleFactor = pilotScaleFactor;

#ifndef CEVA_INTRINSICS
    S16 result1;
    S32 kIndex, mdash;
    S32 m;
#endif
	
#ifdef CEVA_INTRINSICS
    c = (U8*)(DataBuff_Bank1);
#else
    c = MemAlloc(4 * (CE_NmaxDL_rb), CRAM_MEM, 0);
#endif

    //init array size variables
    vshift = Ncell_id % 6;

    scaleFactor = mult_r(scaleFactor, -11585);

    index4 = 0;
    if (isym >= Ndl_symb)
    {
        isym = isym - Ndl_symb;
        index4 = 1;
        slotNumber++;
    }

    //!!!!!!!!! need to add outer loop
    //For each tx antenna
    // generate reference signals
    //tx antenna

    //Loop to generate reference signals for every OFDM symbol in the 1ms
    // subframe, where reference signals are sent
    const3 = CE_NmaxDL_rb - Ndl_rb;

    const4 = (1 << 10) * (2 * Ncell_id + 1);
    const5 = 1 - CPType + 2 * Ncell_id;

    //
    //(Step 1 of 3) Bit Sequence Generation
    //[3GPP TS 36.211 Sec.6.10.1.1]
    //
    //init pseudo random sequence generator at the start of every
    // OFDM symbol within the subframe
    //depends on CellID, slotNumber, symbol number, CP Type
    cinit = (const4) * (7 * (slotNumber + 1) + isym + 1) + const5;
    //cinit = (1 << 10) * (7 * (ns + 1) + lIndex + 1) * (2 * Ncell_id + 1)
    //       + 2 * Ncell_id + !CPType;
    PseudoRandomSeqGen(4 * (CE_NmaxDL_rb), cinit, c);

    //(Step 2 of 3) Complex Sequence Generation
    //
    //(Step 3 of 3) Map to resource elements [3GPP TS 36.211
    // Sec.6.10.1.2]
    //
    for (p = 0; p < lpLim; p++)
    {
        cond1 = ((isym == 0) || (isym == (Ndl_symb - 3))) && (p <= 1);
        cond2 = (isym == 1) && (p >= 2);
        if (cond1 || cond2)
        {
            //confirmed, valid combination of lIndex and p
            //set v
            if (p <= 1)
            {
                v = (S16) (3 * p);
                if (isym)
                    v = 3 - v;
            }
            else
                v = (S16) (3 * (p - 2) + 3 * (index4));

            //map to resource elements
            const1 = (v + vshift) % 6;
#ifdef CEVA_INTRINSICS
            DlPilot_part1_A(Ndl_rb, scaleFactor, &c[2 * const3], &a[p][2 * const1]);
#else
            for (m = 0; m < 2 * Ndl_rb; m++)
            {
                //output index into a()
                kIndex = 2 * (6 * m + const1);

                //input index into r() mdash	45
                mdash = m + const3;

                result1 = c[2 * mdash] * 2 - 1;
                a[p][kIndex++] = result1 * scaleFactor - c[2 * mdash];

                result1 = c[2 * mdash + 1] * 2 - 1;
                a[p][kIndex] = result1 * scaleFactor - c[2 * mdash + 1];
            }//m
#endif
        }
    } //p
#ifndef CEVA_INTRINSICS
    MemFree(c, CRAM_MEM);
#endif

}

