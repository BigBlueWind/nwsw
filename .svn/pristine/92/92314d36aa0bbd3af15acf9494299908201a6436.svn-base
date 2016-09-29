//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL Phyprofiler.h
 *
 * @brief
 * @author Mindspeed Technologies
 * @version $Revision: 1.4 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef __PHYPROFILER_H
#define __PHYPROFILER_H

#include "4gmx.h"
#include "apimgr.h"
#include "hal.h"

#define PHYPROF_DBG 0

#define RC_PHY_PROF_NO_VALID_DATA_ERROR		DEF_USRRC(APP_MODULE, PHYPROF, 1)

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
#define PROF_INDEXOFFSET        6           // Relative to Parameter ID

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
    UINT32 ProfileEnaMask;
    UINT32 FrmProcTime[PROF_MAXCNTRS];
    VUINT32 FrmProcTimeStart[PROF_MAXCNTRS];
} GENPROFILECTX, *PGENPROFILECTX;

extern GENPROFILECTX gGenProfileCtx;

INLINE UINT8 PhyProfMode2Idx(UINT32 mode)
{
    switch(mode)
    {
        case PROF_MASK_TX:
            return PROF_IDX_TX;
        case PROF_MASK_FULL:
            return PROF_IDX_FULL;
        case PROF_MASK_RX:
            return PROF_IDX_RX;
        case PROF_MASK_TXVECT:
            return PROF_IDX_TXVECT;
        case PROF_MASK_RXVECT:
            return PROF_IDX_RXVECT;
        case PROF_MASK_TXSDU:
            return PROF_IDX_TXSDU;
        case PROF_MASK_TX1:
            return PROF_IDX_TX1;
        case PROF_MASK_TX2:
            return PROF_IDX_TX2;
        case PROF_MASK_TX3:
            return PROF_IDX_TX3;
        case PROF_MASK_TX4:
            return PROF_IDX_TX4;
        default:
            return PROF_IDX_FULL;
    }
}

INLINE UINT32 PhyProfParam2Mode(UINT16 paramId)
{
    switch(paramId)
    {
        case PAR_DIAG_TXEXETIME:
            return PROF_MASK_TX;
        case PAR_DIAG_FULLEXETIME:
            return PROF_MASK_FULL;
        case PAR_DIAG_RXEXETIME:
            return PROF_MASK_RX;
        case PAR_DIAG_TXVECT:
            return PROF_MASK_TXVECT;
        case PAR_DIAG_RXVECT:
            return PROF_MASK_RXVECT;
        case PAR_DIAG_TXSDU:
            return PROF_MASK_TXSDU;
        case PAR_DIAG_TX1:
            return PROF_MASK_TX1;
        case PAR_DIAG_TX2:
            return PROF_MASK_TX2;
        case PAR_DIAG_TX3:
            return PROF_MASK_TX3;
        case PAR_DIAG_TX4:
            return PROF_MASK_TX4;
        default:
            return PROF_MASK_FULL;
    }
}

INLINE UINT32 PhyProfParam2Idx(UINT16 paramId)
{
    return PhyProfMode2Idx(PhyProfParam2Mode(paramId));
}

INLINE void ProcTimeBegin(UINT32 mode)
{
    if (mode == 0)
        return;

    gGenProfileCtx.FrmProcTimeStart[PhyProfMode2Idx(mode)] = GetTIMETICK();
    gGenProfileCtx.FrmProcTime[PhyProfMode2Idx(mode)] = 0;
}

INLINE void ProcTimeStart(UINT32 mode)
{
    if (mode == 0)
        return;

    gGenProfileCtx.FrmProcTimeStart[PhyProfMode2Idx(mode)] = GetTIMETICK();
}

INLINE void ProcTimeStop(UINT32 mode)
{
    VUINT32 Curr;

    if (mode == 0)
        return;

    Curr = GetTIMETICK();

    gGenProfileCtx.FrmProcTime[PhyProfMode2Idx(mode)] +=
        (Curr - gGenProfileCtx.FrmProcTimeStart[PhyProfMode2Idx(mode)]);
}

MXRC PhyProfilerApiProc(ApiHeader * pApi, ApiHeader * pResp);

#endif /* __PHYPROFILER_H */

#ifdef __cplusplus
}
#endif

