/** @file profiler.h
 *
 * @brief PHY profiler on a Win32 (stub)
 * @author Mindspeed Technologies
 * @version $Revision: 1.1 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef _WIN32

#pragma once

#include "basetypes.h"

typedef enum
{
    PAR_DIAG_MIN = 6,
    PAR_DIAG_FULLEXETIME =  PAR_DIAG_MIN,
    PAR_DIAG_TXEXETIME, /*7*/
    PAR_DIAG_RXEXETIME, /*8*/
    PAR_DIAG_TXVECT,    /*9*/
    PAR_DIAG_RXVECT,    /*10*/
    PAR_DIAG_TXSDU,     /*11*/
    PAR_DIAG_TX1,           /*12*/
    PAR_DIAG_TX2,           /*13*/
    PAR_DIAG_TX3,           /*14*/
    PAR_DIAG_TX4,           /*15*/
    PAR_DIAG_MAX
}PHYPROF_PARAMS;

#define PROF_MAXCNTRS           32
#define PROF_INDEXOFFSET        PAR_DIAG_MIN

#define PROF_MASK_FULL          0x00000001
#define PROF_MASK_TX            0x00000002  // LteBsTx
#define PROF_MASK_RX            0x00000004  // LteBsRx
#define PROF_MASK_TXVECT        0x00000008  // LteBsPhyTxVectorProcessing
#define PROF_MASK_RXVECT        0x00000010  // LteBsPhyRxVectorProcessing
#define PROF_MASK_TXSDU         0x00000020  // LteBsTxSduProcessing
#define PROF_MASK_TX1           0x00000040  // LteBsTx-1
#define PROF_MASK_TX2           0x00000080  // LteBsTx-2
#define PROF_MASK_TX3           0x00000100  // LteBsTx-3
#define PROF_MASK_TX4           0x00000200  // LteBsTx-4

#define PROF_IDX_FULL           0
#define PROF_IDX_TX             1           // LteBsTx
#define PROF_IDX_RX             2           // LteBsRx
#define PROF_IDX_TXVECT         3           // LteBsPhyTxVectorProcessing
#define PROF_IDX_RXVECT         4           // LteBsPhyRxVectorProcessing
#define PROF_IDX_TXSDU          5           // LteBsTxSduProcessing
#define PROF_IDX_TX1            6           // LteBsTx-1
#define PROF_IDX_TX2            7           // LteBsTx-2
#define PROF_IDX_TX3            8           // LteBsTx-3
#define PROF_IDX_TX4            9           // LteBsTx-4

typedef struct tagGENPROFILECTX
{
    U32 ProfileEnaMask;
    U64 FrmProcTime[PROF_MAXCNTRS];
    U64 FrmProcTimeStart[PROF_MAXCNTRS];
} GENPROFILECTX, *PGENPROFILECTX;

extern GENPROFILECTX gGenProfileCtx;

void ProcTimeBegin(U32 mode);
void ProcTimeStart(U32 mode);
void ProcTimeStop(U32 mode);

#endif /* _WIN32 */
