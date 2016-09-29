//-------------------------------------------------------------------------------------------
/** @file bs_tx_PhysicalSignals.c
 *
 * @brief Base Station Transmitter, includes generators for; Reference signals (pilots), Primary
 *        and secondary synchronization signals
 * @author Mindspeed Technologies
 * @version $Revision: 1.45 $
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

extern U8 *DataBuff_Bank1;
extern void BsTxSyncCH_Part1_A(S16* a, S16* d, S16 scaleP);

#else
#include "basic_op_cc.h"
#endif

//-------------------------------------------------------------------------------------------
// BsTxSyncCH()
//
/** @brief Base Station transmitter, Primary and Secondary Synchronization signals
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param cellSector       - Cell Sector of current active PHY
 *  @param Ndl_rb           - Number of downlink resource blocks
 *  @param Nrb_sc           - Number of subcarriers per resource block
 *  @param scaleP           - Power scaling factor for Primary Syncronization Signal
 *  @param scaleS           - Power scaling factor for Secondary Syncronization Signal
 *  @param syncTxAntennaNum - antenna port used for transmission of the synchronization sequence
 *  @param slotNumber       - slot number of this 1/2ms slot within the 10ms radio frame
 *  @param isym             - Current symbol being processed
 *  @param a                - Pointer to full resource grid for the subframe
 *  @param d0               - Input buffer containing Primary Syncronization signal sequence
 *  @param d1               - Input buffer containing Secondary Syncronization signal sequence
 *  @param frameDuplexType  - Input the duplex mode: FDD or TDD.
 *
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -------------------------------------------------------------------------------------------------------------------------
 *  |Name            |Name in   |Direction     |Length                    |Type           |Data Format |Valid Range |Units  |
 *  |                |Standard  |              |                          |               |            |            |       |
 *  -------------------------------------------------------------------------------------------------------------------------
 *  |cellSector      |          |input         |1                         |real           |(1:15:0)    |Full Range  |       |
 *  -------------------------------------------------------------------------------------------------------------------------
 *  |Ndl_rb          |          |input         |1                         |real           |(1:15:0)    |[0:99]      |       |
 *  -------------------------------------------------------------------------------------------------------------------------
 *  |Nrb_sc          |          |input         |1                         |real           |(1:15:0)    |12          |       |
 *  -------------------------------------------------------------------------------------------------------------------------
 *  |scaleP          |          |input         |1                         |real           |(1:1:14)    |Full range  |       |
 *  -------------------------------------------------------------------------------------------------------------------------
 *  |scaleS          |          |input         |1                         |real           |(1:1:14)    |Full range  |       |
 *  -------------------------------------------------------------------------------------------------------------------------
 *  |syncTxAntennaNum|p         |input         |1                         |real           |(1:15:0)    |[0:3]       |       |
 *  -------------------------------------------------------------------------------------------------------------------------
 *  |slotNumber      |n_s       |input         |1                         |real           |(1:15:0)    |[0:19]      |       |
 *  -------------------------------------------------------------------------------------------------------------------------
 *  |a               |a         |input/output  |pPhy->bsTx.Ndl_rb         |pComplex Array |(1:1:14)    |Full Range  |       |
 *  |                |          |              |* pPhy->bs.Nrb_sc         |               |            |            |       |
 *  |                |          |              |* pPhy->bsTx.Ndl_symb     |               |            |            |       |
 *  |                |          |              |* pPhy->bsTx.Ntx_antennas |               |            |            |       |
 *  -------------------------------------------------------------------------------------------------------------------------
 *  |d0              |          |input         |1                         |real           |(1:1:14)    |Full Range  |       |
 *  -------------------------------------------------------------------------------------------------------------------------
 *  |d1              |          |input         |1                         |real           |(1:1:14)    |Full Range  |       |
 *  -------------------------------------------------------------------------------------------------------------------------
 *  |frameDuplexType |          |input         |1                         |real           |(1:15:0)    |[0, 1]      |       |
 *  -------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------------------
 *  |Block Name          |BsTxSyncCH                          |
 *  -----------------------------------------------------------
 *  |Input Streams       |a,d0,d1                             |
 *  -----------------------------------------------------------
 *  |Output Streams      |a                                   |
 *  -----------------------------------------------------------
 *  |Config Parameters   |cellSector,Ndl_rb,Nrb_sc,Ndl_symb,  |
 *  |                    |scaleP,scaleS,SyncTxAntennaNum,     |
 *  |                    |slotNumber,isym, frameDuplexType    |
 *  -----------------------------------------------------------
 *  |Status Parameters   |none                                |
 *  -----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * Base Station transmitter, Primary and Secondary Synchronization Signal.
 * To generate Primary Synchronization Signal this function reads the Zadoff-Chu
 * sequence from a table (which was previously constructed based on the cell physical
 * layer id during BS Initialization) and adds these signals to the resource grid,
 * according to [1].
 * To generate Secondary Synchronization Signal this function reads the sequence from
 * a table (which was previously constructed based on the cell physical layer id during
 * BS Initialization) and adds these signals to the resource grid, according to [1].
 *
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * [1] 3GPP TS 36.211 Rel.8, Sec.6.11.2, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-------------------------------------------------------------------------------------------
void BsTxSyncCH(S16 cellSector, S16 Ndl_rb, S16 Nrb_sc, S16 Ndl_symb, S16 scaleP, S16 scaleS,
                S16 SyncTxAntennaNum, S16 slotNumber, S16 isym, S16 *a[], S16 d0[], S16 d1[])
{
    S16 i;
    S16 *src;
    S16 row_a1 = Ndl_rb * Nrb_sc;
    S16 startInd;

    S16 *d;

    d = (S16*)(DataBuff_Bank1);

    //
    // Sequence Generation
    //
    // i.e. Read ZadoffChu sequence from table, depending on the cell physical layer identity
    // CA_ZadoffChu is (3*124) (real/imag) or (3*62) Complex
    // d = CA_ZadoffChu(N2_id+CE_MI1, :);
    // d = [0 0 0 0 0 d 0 0 0 0 0];  add reserved symbols around the synch signals
    if (isym == (S16) (Ndl_symb - 1))
    {
        memset(d, 0, 10 * 2);
        memset(&d[134], 0, 10 * 2);
        memcpy(&d[10], &CA_ZadoffChu[cellSector * 124], 124 * 2);

        // Mapping to resource elements
        // send synch signal only in the last symbol of the first slot

        startInd = 2 * ((row_a1 >> 1) - 36);
        if(SyncTxAntennaNum == 2) // both antennas
        {
          scaleP = mult(scaleP,23170); // divide by 1/sqrt(2) (-3dB)
         
#ifdef CEVA_INTRINSICS
          BsTxSyncCH_Part1_A(&a[0][startInd], d, scaleP);
#else
          for (i = 0; i < 144; i++)
          {
               a[0][startInd + i] = mult(d[i], scaleP);
          }
#endif
          //Copy to second antenna
          memcpy(&a[1][startInd], &a[0][startInd], 144 * 2);
          
        }
        else // antenna 0 or 1
        {
#ifdef CEVA_INTRINSICS
           BsTxSyncCH_Part1_A(&a[SyncTxAntennaNum][startInd], d, scaleP);
#else
           for (i = 0; i < 144; i++)
           {
              a[SyncTxAntennaNum][startInd + i] = mult(d[i], scaleP);
           }
#endif
        }
        
    }

    // Note: Reserved resource elements, set to zero, 5 kIndex values each side of
    // the ZadoffChu sequence written above.
    // Nothing to do here, since the resource grid is intialized to zero. No data
    // should have been written to these locations.
    // Also, this full range of resource elements should be reserved for all antennas.
    // Send pre-calculated Synch2 Sequence, add reserved symbols before
    if (isym == (S16) (Ndl_symb - 2))
    {
        memset(d, 0, 144 * 2);
        src = d0;
        if (slotNumber == 10)
        {
            src = d1;
        }
        for (i = 5; i < 67; i++)
        {
            // Real
            d[2 * i] = *src++;// Q14
        }

        //Mapping to resource elements
        //send synch signal only in the 2nd-last symbol of the first slot
        //of this subframe (which could be subframe 0 or 5 - slot 0 or 10)
        startInd = 2 * ((row_a1 >> 1) - 36);
        if(SyncTxAntennaNum == 2) // both antennas
        {
          scaleS = mult(scaleS,23170); // divide by 1/sqrt(2) (-3dB)
#ifdef CEVA_INTRINSICS
          BsTxSyncCH_Part1_A(&a[0][startInd], d, scaleS);
#else
          for (i = 0; i < 144; i++)
          {
              a[0][startInd + i] = mult(d[i], scaleS);
          }
#endif
          //Copy to second antenna
          memcpy(&a[1][startInd], &a[0][startInd], 144 * 2);
        }
        else // antenna 0 or 1
        {
#ifdef CEVA_INTRINSICS
           BsTxSyncCH_Part1_A(&a[SyncTxAntennaNum][startInd], d, scaleS);
#else
           for (i = 0; i < 144; i++)
           {
              a[SyncTxAntennaNum][startInd + i] = mult(d[i], scaleS);
           }
#endif
        }

    }

    //Note: Reserved resource elements, set to zero, 5 kIndex values each side of
    //the sync sequence written above.
    //Nothing to do here, since the resource grid is intialized to zero. No
    //data should have been written to these locations.
    //Also, this full range of resource elements should be reserved (I assume) for
    //all antennas.
    return;
}
