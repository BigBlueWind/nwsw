//-------------------------------------------------------------------------------------------
/** @file LteBsTxDataSduThread.h
 *
 * @brief This file implements the DL SDU processing for the LTE BS
 * @author Mindspeed Technologies
 * @version $Revision: 1.19 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */
//-------------------------------------------------------------------------------------------

#ifndef LTEBSTXDATASDU_H
#define LTEBSTXDATASDU_H

#include "ltephydef.h"

#define UNLOCK_DLSCH_TCB_INIT                   (0<<0)
#define UNLOCK_DLSCH_TCB_BCH_COMPLETE           (1<<0)
#define UNLOCK_DLSCH_TCB_PDSCH_DONE             (1<<1)
#define UNLOCK_DLSCH_TCB_SETUPSYM_DONE          (1<<2)

#define UNLOCK_DLSCH_TCB                        (UNLOCK_DLSCH_TCB_BCH_COMPLETE | UNLOCK_DLSCH_TCB_PDSCH_DONE | UNLOCK_DLSCH_TCB_SETUPSYM_DONE)

typedef enum tLtePhySduProcessingState
{
    SDU_NEW = 0,
    SDU_MDMA_DONE,
    SDU_FEC_DONE,
    SDU_SCRAMBLER_DONE,
    SDU_SCRAMBLER_INIT_DONE,
    SDU_PROCESSING_STATE_MAX
} LTE_PHY_SDU_PROCESSING_STATE;

typedef struct tDlSduDesc
{
    U32 gUseTxSduPointer;
    U32 gLowLatencyFlag;

    U32 g_scrsize;
    U32 g_totalsizescr;

    // PBCH
    U8 *pBchSdu;
    U8 *pBchScrOut;
    U32 gPhyInternalBch;
    U16 bchNumBytes;
    U32 pbchCB;
    U32 tBchStart;

    U32 numSDU;
    U32 startTime;
    U32 devX7;
    U32 scrInit;
    U32 armCB;
    PDLTXINPUTPARAMS pTxInputParams;
    PIOParams_DlControl pDlCntrl_Pars;
    PIOParams_DlControl pDlCntrl_Pars_List;
    U32 inDatPtr[MAXCHSUBFRAME*2];
    S32 num_in_bytes[MAXCHSUBFRAME*2];
    U32 cwId[MAXCHSUBFRAME*2];
    U32 chanId[MAXCHSUBFRAME*2];
    U8 *CRC24A_output_array[MAXCHSUBFRAME*2];

    U32 CtxDlFec;
    U8* ScrInBufs[2];
    U8* CRC24A_buf;
    U8* ScrOutBufs[2];
    U32* ScrIndex[2];
    U32 totalsizescr[2];
    U32 scrsize[2];
    U32* pTable_DL_FEC_CTC_CB;
    S32* pUE_NIR_Table;
    S16* pBlkSizTab;
    U16  dlCrcDone;
    U16  dlFecDescriptorDone;

    PTCB modTask[7];
    PTCB pSetupBufsTCB;
    U32* pIsExeTaskListReady;
    U32  unlockPdschTcb;

    PFEC_DL_FDESC pFecDlStart;
    PMDMA_FDESC pMdmaScrStart;
    volatile LTE_PHY_SDU_PROCESSING_STATE sduState;
    volatile U32 t_arm;
    volatile U32 t_mdma;
    volatile U32 t_mdma_scr;
    volatile U32 t_fec;

} LTE_PHY_SDU_DESC, *PLTE_PHY_SDU_DESC;

void LtePhySduEncMdmaFecCbDone1(LPVOID pCtx);
void PBCHScramblerDone(void* pParam);
void DispatchDlFec(PLTE_PHY_SDU_DESC pSduDesc);
void UnlockModTcb(U32 mask, PLTE_PHY_SDU_DESC pSduDesc);

#endif
