//-------------------------------------------------------------------------------------------
/** @file bs_rx_prach.h
 *
 * @brief This file defines the data structure that is used
 *        in the implementation of PRACH
 * @author Mindspeed Technologies
 * @version $Revision: 1.2 $
 *
 * COPYRIGHT&copy; 2009-2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#ifndef BS_RX_PRACH_H
#define BS_RX_PRACH_H

#include "typedef.h"
#include "ltephydef.h"
#include "prototypes.h"
#include "ext_phy.h"
#include "math_ext32.h"
#include "basic_op.h"
#include "tcbscheduler.h"
#include "dft_table16.h"

#include "gputil_fx.h"

#define SEGMSIZE 3840
#define EXTDSIZE 16      // Extended array size, 2*FIR_length
#define NZC      839
#define NZC2     1678

typedef struct PRACH_Info
{
    S16 MO;
    S16 iAntenna;
    S16 Nzc;
    S16 K;
    S16 phi;
    S16 k0;
    S16 Nseq;
    S16 Ncp;
    S16 FIRLength;
	S16 dummy;
    S32 length_adcPrach;

    S32 SegmSize;
    S32 nSegm;
    S32 iSegm;
    S32 AdcNextIdx;  // Idx to the pAdcBuf[] for the next segment

    S16 demod_start;
    S16 demod_end;
    S16 demod_istart;
    S16 demod_idx;
    S16 demod_inv;
    S16 demod_temp;
    S32 demod_L_temp1;

    S16 DSR;   // Down sampling rate
    S16 Nf;    // FIR filter length
    S16 lpfDelay;
    S16 IdxDown;
    S16 nStart;

    S16 numRootSeq;
    S16 u[32];
    S16 Cv[32];
    S16 vLen[32];
    S16 Ncs[32];

	S16 dummy2;
    S16 nPeakDetected;
    S16 iseqPeakDetected[64];
    S16 locPeakDetected[64];

} BS_PRACH, *PBS_PRACH;


void PRACH_Rx_Init( BS_PRACH *pbs_prach, BS_RX_PRACH *bsRxPrach, BS_RX_PRACHDet *bsRxPrachDet,
                    S32 length_adcPrach, S16 MO);

void PRACH_Rx(BS_RX *bsRx, BS_RX_CHAN *bsRxChan, BS_RX_PRACH *bsRxPrach,
              BS_RX_PRACHDet *bsRxPrachDet, S16 MO, S16 **adcbufPrach,
              S32 length_adcPrach, S16 *pTablesInCRAM, BS_PRACH *pbs_prach_cram);


#endif  // BS_RX_PRACH_H
