//-------------------------------------------------------------------------------------------
/** @file LteBsPhyinit.c
 *
 * @brief This file implements the initialization of the LTE BS PHY
 * @author Mindspeed Technologies
 * @version $Revision: 1.270 $
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

#include "gputil_fx.h"
#include "mlog.h"
#include "cd_data_objects.h"

#if defined(FDX_RT)
#include "LteBsApiThread.h"
#include "LteBsRxPrachThread.h"
#include "LteBsRxSrsThread.h"
#include "LteBsRxThread.h"
#include "LteBsTxThread.h"
#include "LteBsApiErrorCheck.h"
#include "lte_bs_phy_nmm_thread.h"
#include "lte_bs_phy_nmm_fdd.h"
#include "svsrlogger.h"
#include "pm.h"
#include "phydi.h"
#include "LteBsPhyStats.h"
#include "clkrst.h"
#include "syscorecfg.h"
#include "devinfo.h"
#else
#include "svsrlogger_c_code.h"
#endif

#ifndef _WIN32
typedef struct tPhyInitCtx
{
    SYSCORECFG cfg;
    FEC_CONFIG_REGS FecConfigRegs;
}PHYINTCTX, *PPHYINITCTX;

PHYINTCTX phyInitCtx;
#endif

//-------------------------------------------------------------------------------------------
/** @brief Initializes base parameters in RxInputParams for all Ceva Functions
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param pPhy        Defines the particular PHY that is going to perform the
 *                     processing of the messages and input samples
 *  @return status     A value of zero indicate success a value of 1 indicates
 *                     failure (We can expand to indicate the specific failure
 *                     reason)
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -----------------------------------------------------------------------------------------------------
 *  |Name          |Name in   |Direction      |Length  |Type          |Data Format |Valid Range |Units  |
 *  |              |Standard  |               |        |              |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |pPhy          |          |pInput/pOutput |        |structure     |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |SetRxInputParams            |
 *  ---------------------------------------------------
 *  |Inputs Stream       |pPhy                        |
 *  ---------------------------------------------------
 *  |Output Stream       |pPhy                        |
 *  ---------------------------------------------------
 *  |Config Parameters   |none                        |
 *  ---------------------------------------------------
 *  |Status Parameters   |status                      |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 *  This function initializes variables in RxInputParams for parameters
 *  passed to the Ceva Functions.  Only set values which are known during
 *  Phy Init so that they do not need to be set again during RxVectorProc.
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * none
 *
 *  \ingroup group_lte_api
 *
 **/
//-------------------------------------------------------------------------------------------
static U32 SetRxInputParams(PLTEPHYDESC pPhy)
{
    ULRXINPUTPARAMS *pRxInputParams;
    ULRXFECPARAMS   *pRxFecParams[2];
    PBS pBs;
    PBS_RX pBsRx;
    S16 K, L, CP;
    U16 isym;
    S32 i;

    ///////////////////////////////////////////////////////////////////////////////
    //New Rx Modulation Input Structure, code around this area will need to be
    //cleaned up later on.    Don't really need to copy things to two different
    //structures...
    ///////////////////////////////////////////////////////////////////////////////
    pBs = pPhy->pBs;
    pBsRx = pPhy->pBsRx;
    pRxInputParams = pBsRx->pUlRxInputParams;
    pRxFecParams[0]   = pBsRx->pUlRxFecParams[0];
    pRxFecParams[1]   = pBsRx->pUlRxFecParams[1];

    K = pBsRx->Nul_rb * pBs->Nrb_sc;    //total number of frequency
    L = 2 * pBsRx->Nul_symb;            //total number of time indices
    CP = pBs->CPType;                   //Cyclic Prefix Type

    pBsRx->RxFecParam_Context = 0;
    pBsRx->MuxControlTcbCtx = 0;
    pBsRx->RxPrach_Context = 0;

    pRxInputParams->Ncell_id = (S16) (pBs->Ncell_id);
    pRxInputParams->Nrb_sc   = pBs->Nrb_sc;
    pRxInputParams->Nul_symb = pBsRx->Nul_symb;
    pRxInputParams->K        = K;
    pRxInputParams->L        = L;
    pRxInputParams->MI       = pBsRx->numMS;
    pRxInputParams->MO       = pBsRx->Nrx_antennas;
    pRxInputParams->MO_1     = pBsRx->Nrx_antennas;
    pRxInputParams->CPType   = CP;

    //setting up variables for ChanEst
    pRxInputParams->df = pBs->df;
    pRxInputParams->Fs = pBs->Fs;
    pRxInputParams->NusedE = pBs->Nused;
    pRxInputParams->Nfft = pBs->Nfft;
    pRxInputParams->Nul_rb = pBsRx->Nul_rb;
    pRxInputParams->pilotCarrierSymbol = CA_ULPilotCarrierSymbol[pBs->CPType];

    //Setup sequences for Uplink PUCCH pilots
    gen_ul_Pilot_info(pRxInputParams);

    ////////////////////////////////////////////////////////////////
    //Initialization for Multiplex control and Data parameters
    ////////////////////////////////////////////////////////////////
    for(i = 0; i < NUM_SYM_ACK_RI; i++)
    {
        //Need to determine soft decision locations for each of the symbols in the subframe which
        //has ACK information.
        isym = (U16)ColSetACK[CP][i];
        pRxFecParams[0]->symPosACK[isym] = (U16)i;
        pRxFecParams[1]->symPosACK[isym] = (U16)i;

        //Need to determine soft decision locations for each of the symbols in the subframe which
        //has RI information.
        isym = (U16)ColSetRI[CP][i];
        pRxFecParams[0]->symPosRI[isym] = (U16)i;
        pRxFecParams[1]->symPosRI[isym] = (U16)i;
    }

    //Starting symbol for all Multiplex CQI bits
    for(i = 0; i < MAX_PUSCH_DATASYMB_PER_SUBF; i++)
    {
        pRxFecParams[0]->symPosCQI[i] = (U16)i;
        pRxFecParams[1]->symPosCQI[i] = (U16)i;
    }

    pRxFecParams[0]->genBuf = pPhy->genBuf;
    pRxFecParams[1]->genBuf = pPhy->genBuf;

    return SUCCESS;
}

//-------------------------------------------------------------------------------------------
/** @brief Initializes base parameters in SetTxInputParams for all Ceva Functions
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param pPhy        Defines the particular PHY that is going to perform the
 *                     processing of the messages and input samples
 *  @return status     A value of zero indicate success a value of 1 indicates
 *                     failure (We can expand to indicate the specific failure
 *                     reason)
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -----------------------------------------------------------------------------------------------------
 *  |Name          |Name in   |Direction      |Length  |Type          |Data Format |Valid Range |Units  |
 *  |              |Standard  |               |        |              |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |pPhy          |          |pInput/pOutput |        |structure     |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |SetTxInputParams            |
 *  ---------------------------------------------------
 *  |Inputs Stream       |pPhy                        |
 *  ---------------------------------------------------
 *  |Output Stream       |pPhy                        |
 *  ---------------------------------------------------
 *  |Config Parameters   |none                        |
 *  ---------------------------------------------------
 *  |Status Parameters   |status                      |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 *  This function initializes variables in RxInputParams for parameters
 *  passed to the Ceva Functions.  Only set values which are known during
 *  Phy Init so that they do not need to be set again during RxVectorProc.
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * none
 *
 *  \ingroup group_lte_api
 *
 **/
//-------------------------------------------------------------------------------------------
static U32 SetTxInputParams(PLTEPHYDESC pPhy)
{
    DLTXINPUTPARAMS *pTxInputParams;
    IOParams_DlControl *pDlCntrl_Pars;
    PBS pBs;
    PBS_TX pBsTx;
    S16 ii, Ndl_rb;
    S32 cond1;

    ///////////////////////////////////////////////////////////////////////////////
    //New Tx Modulation Input Structure, code around this area will need to be
    //cleaned up later on.    Don't really need to copy things to two different
    //structures...
    ///////////////////////////////////////////////////////////////////////////////
    pBs = pPhy->pBs;
    pBsTx = pPhy->pBsTx;
    pTxInputParams = pBsTx->pTxInputParams;
    pDlCntrl_Pars  = pBsTx->pDlCntrl_TxInPars;
    Ndl_rb = pBsTx->Ndl_rb;

    pTxInputParams->pBchDataBuffer = pBsTx->pBsDlInitMemBuf->pBchDataBuffer;

    g_totalsizescr[0][0] = g_totalsizescr[0][1] = 0;
    g_totalsizescr[1][0] = g_totalsizescr[1][1] = 0;
    pDlCntrl_Pars->ptrInputPDCCH = pDlCntrl_Pars->inputPDCCH;

    pTxInputParams->cellSector = pBs->cellSector;
    pTxInputParams->CPType     = pBs->CPType;
    pTxInputParams->Ncell_id   = pBs->Ncell_id;
    pTxInputParams->Ndl_rb     = Ndl_rb;
    pTxInputParams->Nrb_sc     = pBs->Nrb_sc;
    pTxInputParams->Ndl_symb   = pBsTx->Ndl_symb;
    pTxInputParams->Ntx_antennas  = pBsTx->Ntx_antennas;
    pTxInputParams->RBIndexMid6Hi = pBsTx->RBIndexMid6Hi;
    pTxInputParams->RBIndexMid6Lo = pBsTx->RBIndexMid6Lo;
    pTxInputParams->CE_SyncTxAntennaNum = (S16)pBsTx->SyncTxAntennaNum;
    pTxInputParams->NumSubCars = RUP32B(pBsTx->Ndl_rb * pBs->Nrb_sc); //total number of frequency indices in resource grid
    pTxInputParams->NumSubCars = pTxInputParams->NumSubCars << 2;     //for IQ and S16
    pTxInputParams->frameDuplexType = pBs->frameDuplexType;
    pTxInputParams->Pb              = pBs->P_b;

    ///////////////////////////////////////////////////////////////////////////////
    //Downlink Tx input parameter structure for Control Symbols
    ///////////////////////////////////////////////////////////////////////////////
    pDlCntrl_Pars->CPType     = pBs->CPType;
    pDlCntrl_Pars->Ncell_id   = pBs->Ncell_id;
    pDlCntrl_Pars->Ndl_rb     = Ndl_rb;
    pDlCntrl_Pars->Nrb_sc     = pBs->Nrb_sc;
    pDlCntrl_Pars->stepsize_a = RUP32B(pBsTx->Ndl_rb * pBs->Nrb_sc);
    pDlCntrl_Pars->numTxAntennas = pBsTx->Ntx_antennas;
    pDlCntrl_Pars->scrSeq        = pBsTx->pBsDlInitMemBuf->pDLCellSpecificScrSeq;

    ii = 0;
    cond1 = pTxInputParams->Ntx_antennas >> 1;
    if (Ndl_rb == 6)
    {
        ii = 0;
        pTxInputParams->nVRB_Table = nVRB_BW_1_4;
        pTxInputParams->CA_NumREinRBPairsMid6_Sync = CA_NumREinRBPairsMid6_Sync_BW_1_4;
        pTxInputParams->CA_NumREinRBPairsMid6_Bcst = CA_NumREinRBPairsMid6_Bcst_BW_1_4[cond1];
    }
    else if (Ndl_rb == 15)
    {
        ii = 1;
        pTxInputParams->nVRB_Table = nVRB_BW_3;
        pTxInputParams->CA_NumREinRBPairsMid6_Sync = CA_NumREinRBPairsMid6_Sync_BW_3;
        pTxInputParams->CA_NumREinRBPairsMid6_Bcst = CA_NumREinRBPairsMid6_Bcst_BW_3[cond1];
    }
    else if (Ndl_rb == 25)
    {
        ii = 2;
        pTxInputParams->nVRB_Table = nVRB_BW_5;
        pTxInputParams->CA_NumREinRBPairsMid6_Sync = CA_NumREinRBPairsMid6_Sync_BW_5;
        pTxInputParams->CA_NumREinRBPairsMid6_Bcst = CA_NumREinRBPairsMid6_Bcst_BW_5[cond1];
    }
    else if (Ndl_rb == 50)
    {
        ii = 3;
        pTxInputParams->nVRB_Table = nVRB_BW_10;
        pTxInputParams->CA_NumREinRBPairsMid6_Sync = CA_NumREinRBPairsMid6_Sync_BW_10;
        pTxInputParams->CA_NumREinRBPairsMid6_Bcst = CA_NumREinRBPairsMid6_Bcst_BW_10[cond1];
    }
    else if (Ndl_rb == 75)
    {
        ii = 4;
        pTxInputParams->nVRB_Table = nVRB_BW_15;
        pTxInputParams->CA_NumREinRBPairsMid6_Sync = CA_NumREinRBPairsMid6_Sync_BW_15;
        pTxInputParams->CA_NumREinRBPairsMid6_Bcst = CA_NumREinRBPairsMid6_Bcst_BW_15[cond1];
    }
    else if (Ndl_rb == 100)
    {
        ii = 5;
        pTxInputParams->nVRB_Table = nVRB_BW_20;
        pTxInputParams->CA_NumREinRBPairsMid6_Sync = CA_NumREinRBPairsMid6_Sync_BW_20;
        pTxInputParams->CA_NumREinRBPairsMid6_Bcst = CA_NumREinRBPairsMid6_Bcst_BW_20[cond1];
    }
    pTxInputParams->CA_NumREinRBPairsMid6_Bcst_all = CA_NumREinRBPairsMid6_Bcst_all[cond1];
    pDlCntrl_Pars->PHICH_Grp_Idx1 = ii;


    //Need to save off configuration of PBCH so that we can restore it when we
    //insert PBCH into configuration for frame number not divisable by 4
    pTxInputParams->channelType[MAXCHSUBFRAME-1]  = PBCH;
    pTxInputParams->phyChanScale_factor[MAXCHSUBFRAME-1] = 16384;
    if (pDlCntrl_Pars->numTxAntennas == 1)
    {
        pTxInputParams->transmissionMode[MAXCHSUBFRAME-1] = SINGLEANT;
        pTxInputParams->numTxLayers[MAXCHSUBFRAME-1]  = SINGLEANT;
    }
    else if (pDlCntrl_Pars->numTxAntennas > 1)
    {
        pTxInputParams->transmissionMode[MAXCHSUBFRAME-1] = TXDIVERSITY;
        pTxInputParams->numTxLayers[MAXCHSUBFRAME-1]  = TXDIVERSITY;
    }

    pTxInputParams->numCodeWords[MAXCHSUBFRAME-1] = 1;
    pTxInputParams->cddType[MAXCHSUBFRAME-1]      = 0;
    pTxInputParams->codeBookIdx[MAXCHSUBFRAME-1]  = 0;
    pTxInputParams->modType0[MAXCHSUBFRAME-1] = 1;
    pTxInputParams->modType1[MAXCHSUBFRAME-1] = 0;
    pTxInputParams->Qm0[MAXCHSUBFRAME-1] = 2;
    pTxInputParams->Qm1[MAXCHSUBFRAME-1] = 0;

    pTxInputParams->mcsType0[MAXCHSUBFRAME-1] = 0;
    pTxInputParams->mcsType1[MAXCHSUBFRAME-1] = 0;

    pTxInputParams->numberofEntries[MAXCHSUBFRAME-1] = 1;
    pTxInputParams->reselmInfo[MAXCHSUBFRAME-1][0].startRes = (U8)pTxInputParams->RBIndexMid6Lo;
    cond1 = ((pTxInputParams->RBIndexMid6Hi - pTxInputParams->RBIndexMid6Lo) == 6);
    pTxInputParams->reselmInfo[MAXCHSUBFRAME-1][0].numRes = 6;
    if (cond1)
        pTxInputParams->reselmInfo[MAXCHSUBFRAME-1][0].numRes = 7;

    return SUCCESS;
}

//-------------------------------------------------------------------------------------------
/** @brief LtePhyBsGenSecondarySyncSignal, generate primary and secondary sync signals
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param pBs         Structure containing base parameters used to generate
 *                     sync signals
 *  @param pBsTx       Downlink Tx structure containing array to copy primary
 *                     and secondary sync signals
 *  @param contextNum  Context number
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -----------------------------------------------------------------------------------------------------
 *  |Name          |Name in   |Direction      |Length  |Type          |Data Format |Valid Range |Units  |
 *  |              |Standard  |               |        |              |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |pBs           |          |pInput         |        |structure     |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |pBsTx         |          |pInput/pOutput |        |structure     |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |contextNum    |          |input          |        |real          |(32:0:0)    |[0,1]       |       |
 *  -----------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------
 *  |Block Name          |LtePhyBsGenSecondarySyncSignal |
 *  ------------------------------------------------------
 *  |Inputs Stream       |pBs                            |
 *  ------------------------------------------------------
 *  |Output Stream       |pBsTx                          |
 *  ------------------------------------------------------
 *  |Config Parameters   |contextNum                     |
 *  ------------------------------------------------------
 *  |Status Parameters   |                               |
 *  ------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 *  This function will generate primary and secondary Syncronization
 *  signals for the Downlink Tx.
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * none
 *
 *  \ingroup group_lte_api
 *
 **/
//-------------------------------------------------------------------------------------------
static void LtePhyBsGenSecondarySyncSignal ( PBS pBs, PBS_TX pBsTx, U32 contextNum )
{
    // Sync computation
    S16 temp, temp1, temp2, i, n;
    S16 m0, m1;
    S16 x[31];
    S16 s_mseq[31];
    S16 c_mseq[31];
    S16 z_mseq[31];
    S16 c0[31];
    S16 c1[31];
    S16 sm0_0[31];
    S16 sm1_1[31];
    S16 zm0_1[31];
    S16 zm1_1[31];


    //=========================================================================
    //Define here, non-constant parameters that are cell-specific and are
    // unchanged between subframes / users / codewords etc.
    //

    //-------------------------------------------------------------------------
    // Synchronization signals
    //

    //
    // Secondary sync signal is entirely defined by the CellId, so calculated
    // here, with device config, not per frame.

    //Step 1: Get cyclic shift values from pre-calculated table

    //cyclic shift value m0
    m0 = CA_Sync2CellIDTable[pBs->cellGroup][CE_m0Col];
    //cyclic shift value m1
    m1 = CA_Sync2CellIDTable[pBs->cellGroup][CE_m1Col];

    //Step 2: Generate 31-point m-sequence, s_mseq(n), for n=0,1,...,30
    for (i = 0; i < 31; i++) //initialize temporary array to zeros
    {
        x[i] = 0;
    }
    //set one bit to start the sequence going
    x[4] = 1;

    for (n = 0; n <= 25; n++)
    {
        temp = x[n + 2] + x[n];
        //generate temp sequence, as bit values {0,1}
        x[n + 5] = 0;
        if (temp == 1)
        {
            //generate temp sequence, as bit values {0,1}
            x[n + 5] = 1;
        }
    }
    for (i = 0; i < 31; i++)
    {
        //s_mseq is this temp bit sequence, converted from {0,1} to {-1,+1}
        s_mseq[i] = 1 - 2 * x[i];
    }

    //Step 3: Generate the two sequences s0 and s1 as two different cyclic
    //shifts of the m-sequence s_mseq
    for (n = 0; n <= 30; n++)
    {
        temp = (n + m0) % 31;
        sm0_0[n] = s_mseq[temp];
        temp = (n + m1) % 31;
        sm1_1[n] = s_mseq[temp];
    }

    //Step 4: Generate 31-point m-sequence, c_mseq(n), for n=0,1,...,30
    for (i = 0; i < 31; i++)
    {
        //initialize temporary array to zeros
        x[i] = 0;
    }
    //set one bit to start the sequence going
    x[4] = 1;

    for (n = 0; n <= 25; n++)
    {
        temp = x[n + 3] + x[n];
        //generate temp sequence, as bit values {0,1}
        x[n + 5] = 0;
        if (temp == 1)
        {
            //generate temp sequence, as bit values {0,1
            x[n + 5] = 1;
        }
    }

    for (i = 0; i < 31; i++)
    {
        //c_mseq is this temp bit sequence, converted from {0,1} to {-1,+1}
        c_mseq[i] = 1 - 2 * x[i];
    }

    //Step 5: Generate two scrambling sequences c0 and c1 as two different
    // cyclic shifts of the m-sequence c_mseq
    for (n = 0; n <= 30; n++)
    {
        temp = (n + pBs->cellSector) % 31;
        c0[n] = c_mseq[temp];
        temp = (n + pBs->cellSector + 3) % 31;
        c1[n] = c_mseq[temp];
    }

    //Step 6: Generate 31-point m-sequence, z_mseq(n), for n=0,1,...,30
    for (i = 0; i < 31; i++)
    {
        //initialize temporary array to zeros
        x[i] = 0;
    }
    //set one bit to start the sequence going
    x[4] = 1;
    for (n = 0; n <= 25; n++)
    {
        temp = x[n + 4] + x[n + 2] + x[n + 1] + x[n];
        //generate temp sequence, as bit values {0,1}
        x[n + 5] = 0;
        if (temp == 1 || temp == 3)
    {
            //generate temp sequence, as bit values {0,1}
            x[n + 5] = 1;
        }
    }

    for (i = 0; i < 31; i++)
    {
        //z_mseq is this temp bit sequence, converted from {0,1} to {-1,+1}
        z_mseq[i] = 1 - 2 * x[i];
    }

    //Step 7: Generate two scrambling sequences z0 and z1 as two different
    // cyclic shifts of the m-sequence z_mseq
    for (n = 0; n <= 30; n++)
    {
        temp2 = m0 % 8;
        temp1 = (n + temp2) % 31;
        zm0_1[n] = z_mseq[temp1];

        temp2 = m1 % 8;
        temp1 = (n + temp2) % 31;
        zm1_1[n] = z_mseq[temp1];
    }

    //Last Sequence Generation Step 8: Generate the Secondary Sync Signal d(0),d(1),...,d(61)
    //for Subframe 0 (slot 0)
    for (n = 0; n <= 30; n++)
    {
        // Q14
        pBsTx->sync2_d[0][2 * n] = sm0_0[n] * c0[n] * 16384;
        pBsTx->sync2_d[0][2 * n + 1] = sm1_1[n] * c1[n] * zm0_1[n] * 16384;
    }

    //for Subframe 5 (slot 10)
    for (n = 0; n <= 30; n++)
    {
        // Q14
        pBsTx->sync2_d[1][2 * n] = sm1_1[n] * c0[n] * 16384;
        pBsTx->sync2_d[1][2 * n + 1] = sm0_0[n] * c1[n] * zm1_1[n] * 16384;
    }
}

//-------------------------------------------------------------------------------------------
/** @brief Initialize common phy varialbes used for UL and DL
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   pPhy        Pointer to the current active PHY structure
 *  @param   contextNum  Structure number for the N/N+1 Real Time scheme
 *  @param   pPhy0       Pointer to the first active PHY structure (subframe N)
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -----------------------------------------------------------------------------------------------------
 *  |Name          |Name in   |Direction      |Length  |Type          |Data Format |Valid Range |Units  |
 *  |              |Standard  |               |        |              |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |pPhy          |          |pInput/pOutput |        |structure     |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |contextNum    |          |input          |1       |unsigned int  |(0:31:0)    |[0:1]       |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |pPhy0         |          |pInput         |        |structure     |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------------
 *  |Block Name          |LteBsPhyInitCommonMemBuffers  |
 *  -----------------------------------------------------
 *  |Input Stream        |pPhy, pPhy0                   |
 *  -----------------------------------------------------
 *  |Output Stream       |pPhy                          |
 *  -----------------------------------------------------
 *  |Config Parameters   |contextNum                    |
 *  -----------------------------------------------------
 *  |Status Parameters   |none                          |
 *  -----------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Initialize common phy varialbes used for UL and DL. It allocates memory and initializes varialbes
 *  needed for the N/N+1 scheme of running realtime PHY
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *  none
 *
 *
 *  \ingroup group_lte_api
 *
 **/
//-------------------------------------------------------------------------------------------
static U32 LteBsPhyInitCommonMemBuffers(PLTEPHYDESC pPhy, U32 contextNum, PLTEPHYDESC pPhy0)
{
    PBS pBs = pPhy->pBs;
    PBS_TX pBsTx = pPhy->pBsTx;
    PBS_RX pBsRx = pPhy->pBsRx, pBsRx0;
    S16 MO, MI, L, K;
    S16 Nul_symb = pBsRx->Nul_symb;
    S16 Ndl_rb = pBsTx->Ndl_rb;
    S16 Ndl_symb = pBsTx->Ndl_symb;
    PBSDLINITMEMBUF pBsDlInitMemBuf;
    PBS_RX_POINTERS pRxPtr = pBsRx->pBsRxPointers;
    PBS_RX_POINTERS pRxPtr0;
    U8 *p;

    U32 offset, TxOffset, TxOffset1;
    S16 i = 0;
    S32 step_aDet, step_reDet;
    S32 length_adc, len_r_alpha_uv, _size;

#ifndef _WIN32
    PLTE_PHY_SDU_DESC pSduDesc;
#endif

    //-------------------------------------------------
    // Initialize buffers for DSP functions running in ARM or host PC
    //-------------------------------------------------
    TxOffset = 0x8000; // Bank2 Starts at address 32K = 32868
    TxOffset1 = 0x200; // To point to memory in between banks for downlink control
    DataBuff_Bank1  = (U8 *) &CevaDataSectionBase; // Starts at address 0
    DataBuff_Bank1a = (U8 *) &DataBuff_Bank1[TxOffset1];
    DataBuff_Bank2  = (U8 *) &DataBuff_Bank1[TxOffset];
    DataBuff_Bank3  = (U8 *) &DataBuff_Bank2[TxOffset];
    TxOffset1 = 0x3000;
    DataBuff_Bank3a = (U8 *) &DataBuff_Bank3[TxOffset1];

    TxOffset = (sizeof(TXLAYERMAPPERMULTICHANOUT) + 100);
    TxOffset = (TxOffset + 7) & (~7); // To Make it 8 byte aligned
    DataBuff_Bank1_Off = &DataBuff_Bank1[TxOffset];

    TxOffset = sizeof(DLRESELEMMAPPERMULTICHANOUT);
    TxOffset = (TxOffset + 7) & (~7); // To Make it 8 byte aligned
    DataBuff_Bank2_Off = &DataBuff_Bank2[TxOffset]; // Used by TASKID_LTE_MULTICHAN as temp space

    TxOffset = 0x8000 - TxOffset - 1200 * 4 * 4;
    TxOffset = sizeof(TXPRECODERMULTICHANOUT) - TxOffset;
    TxOffset = (TxOffset + 7) & (~7); // To Make it 8 byte aligned
    DataBuff_Bank3_Off = &DataBuff_Bank3[TxOffset]; // Used by TASKID_TX_MULTICHAN_MODULATION  as temp space
    /* End Mapping */

    //-------------------------------------------------
    // Local Variable Initializations
    //-------------------------------------------------
    length_adc = 2 * ((pBs->Nfft + pBs->Ncp0) + 6 * (pBs->Nfft + pBs->Ncp)) * sizeof(S16);  // Ex: 15360 * 2 = 30720 samples for 10MHz
    MI = pBsRx->numMS; // it is supposed each MS has one TX antenna
    MO = pBsRx->Nrx_antennas;
    K = pBsRx->Nul_rb * pBs->Nrb_sc; //total number of frequency indices in resource grid
    L = 2 * Nul_symb; //total number of time indices in resource grid subframe
    len_r_alpha_uv = 7 * NUM_SLOTS_SUBFRM * 2 * PRB_12;
    step_aDet = 2 * RUP8B(K+2);
    step_reDet = 2 * RUP8B(K+4*MAXCHSUBFRAME);


    //-------------------------------------------------
    // Check for Sizes and stop if we do not support it
    //-------------------------------------------------
#ifndef _WIN32
    if (length_adc*2*2*pBsTx->Ntx_antennas > CPRI_DMA_BUF_SIZE)   // 2*2  for bytes and for 2 contexts
    {
        uart_printf("Stopping!!!\r\n");
        uart_printf("LteBsPhyInitCommonMemBuffers: We Do Not support this in code yet (BandWidth: %d MHz, nTxAnt: %d)\r\n", pBs->Nfft, pBsTx->Ntx_antennas);
        while(1);
    }
    if (length_adc*2*2*pBsRx->Nrx_antennas > CPRI_DMA_BUF_SIZE)   // 2*2  for bytes and for 2 contexts
    {
        uart_printf("Stopping!!!\r\n");
        uart_printf("LteBsPhyInitCommonMemBuffers: We Do Not support this in code yet (BandWidth: %d MHz, nRxAnt: %d)\r\n", pBs->Nfft, pBsRx->Nrx_antennas);
        while(1);
    }
#endif

    //-------------------------------------------------
    // DL MEMORY ALLOCATIONS
    //-------------------------------------------------
    // DDR
    //-------------------------------------------------
    pBsTx->row_a = Ndl_rb * pBs->Nrb_sc; //total number of frequency indices in resource grid
    pBsTx->col_a = Ndl_symb << 1; //total number of time indices in resource grid subframe
    pBsDlInitMemBuf = pBsTx->pBsDlInitMemBuf;

#ifndef WIN32
    gpSduDesc[contextNum] = MemAlloc(sizeof(LTE_PHY_SDU_DESC), CRAM_MEM, 1);
#endif

    if ( contextNum == 0 )
    {
        /* allocate memory for BCH data */
        pBsDlInitMemBuf->pBchDataBuffer = (U32 *) MemAlloc ( sizeof(U8)*PBCH_INIT_MEM_BUFFER_SIZE,
                                                             NCNB_MEM, 0);
        _ASSERT_PTR(pBsDlInitMemBuf->pBchDataBuffer);
        addObj((U8*)pBsDlInitMemBuf->pBchDataBuffer, sizeof(U8)*PBCH_INIT_MEM_BUFFER_SIZE, OBJECTNUMBER_PBCH_INIT_MEM_BUFFER);

        /* Allocate memory for DL scrambling sequences */
        pBsDlInitMemBuf->pDLCellSpecificScrSeq = (U8 *) MemAlloc (10 * 800, NCNB_MEM, 0 );
        _ASSERT_PTR(pBsDlInitMemBuf->pDLCellSpecificScrSeq);
        addObj((U8*)pBsDlInitMemBuf->pDLCellSpecificScrSeq, 10 * 800, OBJECTNUMBER_DLCELLSPECIFICSCRSEQ);
    }

    //-------------------------------------------------
    // DL MEMORY ALLOCATIONS
    //-------------------------------------------------
    // IRAM
    //-------------------------------------------------
    gCRC24A_buf[contextNum] = MemAlloc(MAX_BYTES_PER_ANTENNA_SCRAMBLER_OUT, IRAM_MEM, 0);
    _ASSERT_PTR(gCRC24A_buf[contextNum]);
    addObj((U8*)gCRC24A_buf[contextNum], MAX_BYTES_PER_ANTENNA_SCRAMBLER_OUT, OBJECTNUMBER_CRC24ABUF);
    gScrInBufs[contextNum][0] = MemAlloc(MAX_BYTES_PER_ANTENNA_SCRAMBLER_OUT, IRAM_MEM, 0);
    _ASSERT_PTR(gScrInBufs[contextNum][0]);
    addObj((U8*)gScrInBufs[contextNum][0], MAX_BYTES_PER_ANTENNA_SCRAMBLER_OUT, OBJECTNUMBER_SCRBUF0);
    gScrInBufs[contextNum][1] = MemAlloc(MAX_BYTES_PER_ANTENNA_SCRAMBLER_OUT, IRAM_MEM, 0);
    _ASSERT_PTR(gScrInBufs[contextNum][1]);
    addObj((U8*)gScrInBufs[contextNum][1], MAX_BYTES_PER_ANTENNA_SCRAMBLER_OUT, OBJECTNUMBER_SCRBUF1);


    //-------------------------------------------------
    // DL MEMORY ALLOCATIONS
    //-------------------------------------------------
    // CRAM
    //-------------------------------------------------

    _size = 2 * RUP32B(pBsTx->col_a * sizeof(PTXSYSTEMINFO *))  // pBsTx->pTxSys
          + pBsTx->col_a * (RUP32B( sizeof(TXSYMBUF)*MAXCHSUBFRAME )
          + RUP32B(sizeof(TXSYSTEMINFO))                        // pBsTx->pTxSys[i]
          + RUP32B(sizeof(TXSYMBUFS)));

    if ( contextNum == 0 )
    {
       pBsTx->GenDLCramBufSize =
                             RUP32B(sizeof(S16)*(PCFICH_INIT_MEM_BUFFER_SIZE))                          // pBsDlInitMemBuf->pCfiMemBuffer
                           + RUP32B(sizeof(S16)*(PCFICH_INIT_RE_BUFFER_SIZE))                           // pBsDlInitMemBuf->pCfiReMapBuffer
                           + RUP32B(sizeof(S16)*(PHICH_INIT_MEM_BUFFER_SIZE))                           // pBsDlInitMemBuf->pHIMemBuffer
                           + RUP32B(sizeof(S16)*(PHICH_INIT_RE_BUFFER_SIZE))                            // pBsDlInitMemBuf->pHIREMapTable
                           + RUP32B(sizeof(S16) * (PDCCH_INIT_PERMUTE_TABLE_BUFFER_SIZE))               // pBsDlInitMemBuf->pPDCCHPermuteTable
                           + RUP32B(sizeof(S16) * (PDCCH_INIT_RE_BUFFER_SIZE))                          // pBsDlInitMemBuf->pPDCCHReMapTable
                           + RUP32B(sizeof(S16) * (PDCCH_INIT_NUM_QUADRUPLETS_BUFFER_SIZE))             // pBsDlInitMemBuf->pPDCCHQuadrupletsLen
                           + RUP32B(sizeof(S16) * (DL_CRS_INIT_SYM_BUFFER_SIZE))                        // pBsDlInitMemBuf->pDLCellSpecificRefSeq
                           + RUP32B(_size)                                                              // pBsTx->pTxSys and co
                           + RUP32B(pBsTx->col_a * RUP32B(pBsTx->row_a) * pBsTx->Ntx_antennas * 2 * 2)  // pBsTx->a
                           + 2 * RUP32B(64 * sizeof (S16))                                              // pBsTx->sync2_d[2]
                           + 2 * RUP32B(sizeof(U32) * MAXCHSUBFRAME)                                    // pPhy->scrIndex[2]
                           + 2 * RUP32B(MAX_BYTES_PER_ANTENNA_SCRAMBLER_OUT)                            // pPhy->scrOutBufs[2]
                           + RUP32B(sizeof(IOParams_DlControl))                                         // pBsTx->pDlCntrl_TxInPars
                           + RUP32B(sizeof(DLTXINPUTPARAMS))                                            // pBsTx->pTxInputParams
                           + RUP32B(MAXCHSUBFRAME*128)                                                  // pBsTx->inputPDCCH
                           + RUP32B(MAXCHSUBFRAME*sizeof(U16))                                          // pBsTx->pDlCntrl_TxInPars->pdcchRNTI
                           + RUP32B(240)                                                                // pBsTx->startPBCH
                           + RUP32B(MAX_PDCCH_BUFFER_SIZE)                               //(800)        // pBsTx->startPDCCH
                           ;


        pBsTx->GenDLCramBuf = MemAlloc(pBsTx->GenDLCramBufSize, CRAM_MEM, 1);
        _ASSERT_PTR(pBsTx->GenDLCramBuf);

        offset = 0;
        p      = pBsTx->GenDLCramBuf;

        /* Buffer for PCFICH data - should be in CRAM */
        pBsDlInitMemBuf->pCfiMemBuffer = MemAllocFrom(p, &offset, RUP32B(sizeof(S16)*(PCFICH_INIT_MEM_BUFFER_SIZE)));
        addObj((U8*)pBsDlInitMemBuf->pCfiMemBuffer, RUP32B(sizeof(S16)*(PCFICH_INIT_MEM_BUFFER_SIZE)), OBJECTNUMBER_PCFICH_INIT_MEM_BUFFER);

        /* Buffer for Resource element allocation for PCFICH = 16 symbol locations */
        pBsDlInitMemBuf->pCfiReMapBuffer = MemAllocFrom(p, &offset, RUP32B(sizeof(S16)*(PCFICH_INIT_RE_BUFFER_SIZE)));
        addObj((U8*)pBsDlInitMemBuf->pCfiReMapBuffer, RUP32B(sizeof(S16)*(PCFICH_INIT_RE_BUFFER_SIZE)), OBJECTNUMBER_PCFICH_INIT_RE_BUFFER);

        /* Buffer for PHICH symbols = 12 symbols * 8 seqIndices *10 sub-frames * 4 bytes * numBsTxAntPorts */
        pBsDlInitMemBuf->pHIMemBuffer = MemAllocFrom(p, &offset, RUP32B(sizeof(S16)*(PHICH_INIT_MEM_BUFFER_SIZE)));
        addObj((U8*)pBsDlInitMemBuf->pHIMemBuffer, RUP32B(sizeof(S16)*(PHICH_INIT_MEM_BUFFER_SIZE)), OBJECTNUMBER_PHICH_INIT_MEM_BUFFER);

        /* Allocate memory fo resource element mapping table for PHICH */
        pBsDlInitMemBuf->pHIREMapTable = MemAllocFrom(p, &offset, RUP32B(sizeof(S16)*(PHICH_INIT_RE_BUFFER_SIZE)));
        addObj((U8*)pBsDlInitMemBuf->pHIREMapTable, RUP32B(sizeof(S16)*(PHICH_INIT_RE_BUFFER_SIZE)), OBJECTNUMBER_PHICH_INIT_RE_BUFFER);

        /* Allocate memory for PDCCH Permute Table buffer */
        pBsDlInitMemBuf->pPDCCHPermuteTable = MemAllocFrom(p, &offset, RUP32B(sizeof(S16) * (PDCCH_INIT_PERMUTE_TABLE_BUFFER_SIZE)));
        addObj((U8*)pBsDlInitMemBuf->pPDCCHPermuteTable, RUP32B(sizeof(S16)*(PDCCH_INIT_PERMUTE_TABLE_BUFFER_SIZE)), OBJECTNUMBER_PDCCH_INIT_PERMUTE_TABLE_BUFFER);

        /* Allocate memory for PDCCH Resource Element Table */
        pBsDlInitMemBuf->pPDCCHReMapTable = MemAllocFrom(p, &offset, RUP32B(sizeof(S16) * (PDCCH_INIT_RE_BUFFER_SIZE)));
        addObj((U8*)pBsDlInitMemBuf->pPDCCHReMapTable, RUP32B(sizeof(S16)*(PDCCH_INIT_RE_BUFFER_SIZE)), OBJECTNUMBER_PDCCH_INIT_RE_BUFFER);

        /* Allocate memory for saving Quadruplets len per control symbol */
        pBsDlInitMemBuf->pPDCCHQuadrupletsLen = MemAllocFrom(p, &offset, RUP32B(sizeof(S16) * (PDCCH_INIT_NUM_QUADRUPLETS_BUFFER_SIZE)));
        addObj((U8*)pBsDlInitMemBuf->pPDCCHQuadrupletsLen, RUP32B(sizeof(S16)*(PDCCH_INIT_NUM_QUADRUPLETS_BUFFER_SIZE)), OBJECTNUMBER_PDCCH_INIT_NUM_QUADRUPLETS_BUFFER);

        /* Allocate memory for DL cell specific reference sequence generation */
        pBsDlInitMemBuf->pDLCellSpecificRefSeq = MemAllocFrom(p, &offset, RUP32B(sizeof(S16) * (DL_CRS_INIT_SYM_BUFFER_SIZE)));
        addObj((U8*)pBsDlInitMemBuf->pDLCellSpecificRefSeq, RUP32B(sizeof(S16)*(DL_CRS_INIT_SYM_BUFFER_SIZE)), OBJECTNUMBER_DL_CRS_INIT_SYM_BUFFER);

        pBsTx->pTxSys               = MemAllocFrom(p, &offset, RUP32B(_size));
        addObj((U8*)pBsTx->pTxSys , RUP32B(_size), OBJECTNUMBER_TXSYS);
        pBsTx->a                    = MemAllocFrom(p, &offset, RUP32B(pBsTx->col_a * RUP32B(pBsTx->row_a) * pBsTx->Ntx_antennas * 2 * 2));
        addObj((U8*)pBsTx->a , RUP32B(pBsTx->col_a * RUP32B(pBsTx->row_a) * pBsTx->Ntx_antennas * 2 * 2), OBJECTNUMBER_A);
        pBsTx->sync2_d[0]           = MemAllocFrom(p, &offset, RUP32B(64 * sizeof (S16)));
        addObj((U8*)pBsTx->sync2_d[0] , RUP32B(64 * sizeof (S16)), OBJECTNUMBER_SYNC2_D0);
        pBsTx->sync2_d[1]           = MemAllocFrom(p, &offset, RUP32B(64 * sizeof (S16)));
        addObj((U8*)pBsTx->sync2_d[1] , RUP32B(64 * sizeof (S16)), OBJECTNUMBER_SYNC2_D1);
        gScrIndex[contextNum][0]           = MemAllocFrom(p, &offset, RUP32B(sizeof(U32) * MAXCHSUBFRAME));
        addObj((U8*)gScrIndex[contextNum][0] , RUP32B(sizeof(U32) * MAXCHSUBFRAME), OBJECTNUMBER_SCR_INDEX_0);
        gScrIndex[contextNum][1]           = MemAllocFrom(p, &offset, RUP32B(sizeof(U32) * MAXCHSUBFRAME));
        addObj((U8*)gScrIndex[contextNum][1] , RUP32B(sizeof(U32) * MAXCHSUBFRAME), OBJECTNUMBER_SCR_INDEX_1);
        gScrOutBufs[contextNum][0]         = MemAllocFrom(p, &offset, RUP32B(MAX_BYTES_PER_ANTENNA_SCRAMBLER_OUT));
        addObj((U8*)gScrOutBufs[contextNum][0] , RUP32B(MAX_BYTES_PER_ANTENNA_SCRAMBLER_OUT), OBJECTNUMBER_SCR_OUT_0);
        gScrOutBufs[contextNum][1]         = MemAllocFrom(p, &offset, RUP32B(MAX_BYTES_PER_ANTENNA_SCRAMBLER_OUT));
        addObj((U8*)gScrOutBufs[contextNum][1] , RUP32B(MAX_BYTES_PER_ANTENNA_SCRAMBLER_OUT), OBJECTNUMBER_SCR_OUT_1);
        pBsTx->pDlCntrl_TxInPars    = MemAllocFrom(p, &offset, RUP32B(sizeof(IOParams_DlControl)));
        addObj((U8*)pBsTx->pDlCntrl_TxInPars , RUP32B(sizeof(IOParams_DlControl)), OBJECTNUMBER_IOPARAMS_DLCONTROL);
        pBsTx->pTxInputParams       = MemAllocFrom(p, &offset, RUP32B(sizeof(DLTXINPUTPARAMS)));
        addObj((U8*)pBsTx->pTxInputParams , RUP32B(sizeof(DLTXINPUTPARAMS)), OBJECTNUMBER_DLTXINPUTPARAMS);
        pBsTx->pDlCntrl_TxInPars->inputPDCCH   = MemAllocFrom(p, &offset, RUP32B(MAXCHSUBFRAME*128));
        addObj((U8*)pBsTx->pDlCntrl_TxInPars->inputPDCCH , RUP32B(MAXCHSUBFRAME*128), OBJECTNUMBER_INPUTPDCCH);
        pBsTx->pDlCntrl_TxInPars->pdcchRNTI = MemAllocFrom(p, &offset, RUP32B(MAXCHSUBFRAME*sizeof(U16)));
        addObj((U8*)pBsTx->pDlCntrl_TxInPars->pdcchRNTI , RUP32B(MAXCHSUBFRAME*sizeof(U16)), OBJECTNUMBER_PDCCHRNTI);
        pBsTx->pTxInputParams->startPBCH            = MemAllocFrom(p, &offset, RUP32B(240));
        addObj((U8*)pBsTx->pTxInputParams->startPBCH, RUP32B(240), OBJECTNUMBER_STARTPBCH);
        pBsTx->pDlCntrl_TxInPars->startPDCCH        = MemAllocFrom(p, &offset, RUP32B(MAX_PDCCH_BUFFER_SIZE)); //RUP32B(800));
        addObj((U8*)pBsTx->pDlCntrl_TxInPars->startPDCCH, RUP32B(MAX_PDCCH_BUFFER_SIZE), OBJECTNUMBER_STARTPDCCH);
        if (offset != pBsTx->GenDLCramBufSize)
        {
            phy_printf("LteBsPhyInitCommomMemBuffers(DL 1): Something Fishy!!! offet: %d, GenDLCramBufSize: %d\n",
                offset, pBsTx->GenDLCramBufSize);
            while(1);
        }
    }
    else
    {
        pBsTx->GenDLCramBufSize =
                           RUP32B(_size)                                                                // pBsTx->pTxSys and co
                           + 2 * RUP32B(64 * sizeof (S16))                                              // pBsTx->sync2_d[2]
                           + 2 * RUP32B(sizeof(U32) * MAXCHSUBFRAME)                                    // pPhy->scrIndex[2]
                           + 2 * RUP32B(MAX_BYTES_PER_ANTENNA_SCRAMBLER_OUT)                            // pPhy->scrOutBufs[2]
                           + RUP32B(sizeof(IOParams_DlControl))                                         // pBsTx->pDlCntrl_TxInPars
                           + RUP32B(sizeof(DLTXINPUTPARAMS))                                            // pBsTx->pTxInputParams
                           + RUP32B(MAXCHSUBFRAME*128)                                                  // pBsTx->inputPDCCH
                           + RUP32B(MAXCHSUBFRAME*sizeof(U16))                                          // pBsTx->pDlCntrl_TxInPars->pdcchRNTI
                           + RUP32B(240)                                                                // pBsTx->startPBCH
                           + RUP32B(MAX_PDCCH_BUFFER_SIZE)                  //800)                      // pBsTx->startPDCCH
                           ;

        pBsTx->GenDLCramBuf = MemAlloc(pBsTx->GenDLCramBufSize, CRAM_MEM, 1);
        _ASSERT_PTR(pBsTx->GenDLCramBuf);

        offset = 0;
        p      = pBsTx->GenDLCramBuf;

        pBsTx->a                    = pPhy0->pBsTx->a;
        pBsTx->pTxSys               = MemAllocFrom(p, &offset, RUP32B(_size));
        addObj((U8*)pBsTx->pTxSys , RUP32B(_size), OBJECTNUMBER_TXSYS);
        pBsTx->sync2_d[0]           = MemAllocFrom(p, &offset, RUP32B(64 * sizeof (S16)));
        addObj((U8*)pBsTx->sync2_d[0] , RUP32B(64 * sizeof (S16)), OBJECTNUMBER_SYNC2_D0);
        pBsTx->sync2_d[1]           = MemAllocFrom(p, &offset, RUP32B(64 * sizeof (S16)));
        addObj((U8*)pBsTx->sync2_d[1] , RUP32B(64 * sizeof (S16)), OBJECTNUMBER_SYNC2_D1);
        gScrIndex[contextNum][0]           = MemAllocFrom(p, &offset, RUP32B(sizeof(U32) * MAXCHSUBFRAME));
        addObj((U8*)gScrIndex[contextNum][0] , RUP32B(sizeof(U32) * MAXCHSUBFRAME), OBJECTNUMBER_SCR_INDEX_0);
        gScrIndex[contextNum][1]           = MemAllocFrom(p, &offset, RUP32B(sizeof(U32) * MAXCHSUBFRAME));
        addObj((U8*)gScrIndex[contextNum][1] , RUP32B(sizeof(U32) * MAXCHSUBFRAME), OBJECTNUMBER_SCR_INDEX_1);
        gScrOutBufs[contextNum][0]         = MemAllocFrom(p, &offset, RUP32B(MAX_BYTES_PER_ANTENNA_SCRAMBLER_OUT));
        addObj((U8*)gScrOutBufs[contextNum][0] , RUP32B(MAX_BYTES_PER_ANTENNA_SCRAMBLER_OUT), OBJECTNUMBER_SCR_OUT_0);
        gScrOutBufs[contextNum][1]         = MemAllocFrom(p, &offset, RUP32B(MAX_BYTES_PER_ANTENNA_SCRAMBLER_OUT));
        addObj((U8*)gScrOutBufs[contextNum][1] , RUP32B(MAX_BYTES_PER_ANTENNA_SCRAMBLER_OUT), OBJECTNUMBER_SCR_OUT_1);
        pBsTx->pDlCntrl_TxInPars    = MemAllocFrom(p, &offset, RUP32B(sizeof(IOParams_DlControl)));
        addObj((U8*)pBsTx->pDlCntrl_TxInPars , RUP32B(sizeof(IOParams_DlControl)), OBJECTNUMBER_IOPARAMS_DLCONTROL);
        pBsTx->pTxInputParams       = MemAllocFrom(p, &offset, RUP32B(sizeof(DLTXINPUTPARAMS)));
        addObj((U8*)pBsTx->pTxInputParams , RUP32B(sizeof(DLTXINPUTPARAMS)), OBJECTNUMBER_DLTXINPUTPARAMS);
        pBsTx->pDlCntrl_TxInPars->inputPDCCH   = MemAllocFrom(p, &offset, RUP32B(MAXCHSUBFRAME*128));
        addObj((U8*)pBsTx->pDlCntrl_TxInPars->inputPDCCH , RUP32B(MAXCHSUBFRAME*128), OBJECTNUMBER_INPUTPDCCH);
        pBsTx->pDlCntrl_TxInPars->pdcchRNTI = MemAllocFrom(p, &offset, RUP32B(MAXCHSUBFRAME*sizeof(U16)));
        addObj((U8*)pBsTx->pDlCntrl_TxInPars->pdcchRNTI , RUP32B(MAXCHSUBFRAME*sizeof(U16)), OBJECTNUMBER_PDCCHRNTI);
        pBsTx->pTxInputParams->startPBCH            = MemAllocFrom(p, &offset, RUP32B(240));
        addObj((U8*)pBsTx->pTxInputParams->startPBCH, RUP32B(240), OBJECTNUMBER_STARTPBCH);
        pBsTx->pDlCntrl_TxInPars->startPDCCH        = MemAllocFrom(p, &offset, RUP32B(MAX_PDCCH_BUFFER_SIZE)); //RUP32B(800));
        addObj((U8*)pBsTx->pDlCntrl_TxInPars->startPDCCH, RUP32B(MAX_PDCCH_BUFFER_SIZE), OBJECTNUMBER_STARTPDCCH);
        if (offset != pBsTx->GenDLCramBufSize)
        {
            phy_printf("LteBsPhyInitCommomMemBuffers(DL 2): Something Fishy!!! offet: %d, GenDLCramBufSize: %d\n",
                offset, pBsTx->GenDLCramBufSize);
            while(1);
        }
    }

    // (IFFT Output to CRAM)
#ifdef _WIN32
    if (contextNum == 0)
    {
        for(i = 0; i < pBsTx->Ntx_antennas; i++)
            pBsTx->ifftOut[i] = MemAlloc(2*length_adc, CRAM_MEM, 0);    // Multiply by 2 for bytes

        pPhy->sigOut[0] = MemAlloc(LTEMAXIQ, CRAM_MEM, 0);
    }
    else
    {
        for(i = 0; i < pBsTx->Ntx_antennas; i++)
            pBsTx->ifftOut[i] = pPhy0->pBsTx->ifftOut[i] + length_adc;

        pPhy->sigOut[0] = pPhy0->sigOut[0];
    }
#endif

    //-------------------------------------------------
    // END DL MEMORY ALLOCATIONS
    //-------------------------------------------------

    SetTxInputParams(pPhy);

    /* Generate all the tables necessary for DL control */
    if ( contextNum == 0 )
    {
        LteBsPhyDLCommonInit ( pPhy->pBsTx->pBsDlInitParam, pBsDlInitMemBuf );
    }

    /* Generate secondary sync signal - should be moved into INIT -
       not sure why context number is needed ?? */
    LtePhyBsGenSecondarySyncSignal ( pBs, pBsTx, contextNum );

    // Set predefined FEC processing setup fields
#ifndef WIN32
    pSduDesc = gpSduDesc[contextNum];
    pSduDesc->CtxDlFec                          = contextNum;
    pSduDesc->devX7                             = DevInfoIsT4Kx7();
    pSduDesc->armCB                             = (U32)LtePhySduEncMdmaFecCbDone1;
    pSduDesc->pbchCB                            = (U32)PBCHScramblerDone;
    pSduDesc->gUseTxSduPointer                  = (U32)gUseTxSduPointer;
    pSduDesc->gLowLatencyFlag                   = (U32)gLowLatencyFlag;
    pSduDesc->gPhyInternalBch                   = gPhyInternalBch;
    pSduDesc->pTxInputParams                    = pBsTx->pTxInputParams;
    pSduDesc->pDlCntrl_Pars                     = pBsTx->pDlCntrl_TxInPars;
    pSduDesc->ScrInBufs[0]                      = gScrInBufs[contextNum][0];
    pSduDesc->ScrInBufs[1]                      = gScrInBufs[contextNum][1];
    pSduDesc->ScrOutBufs[0]                     = gScrOutBufs[contextNum][0];
    pSduDesc->ScrOutBufs[1]                     = gScrOutBufs[contextNum][1];
    pSduDesc->CRC24A_buf                        = gCRC24A_buf[contextNum];
    pSduDesc->ScrIndex[0]                       = gScrIndex[contextNum][0];
    pSduDesc->ScrIndex[1]                       = gScrIndex[contextNum][1];
    pSduDesc->totalsizescr[0]                   = 0;
    pSduDesc->totalsizescr[1]                   = 0;
    pSduDesc->scrsize[0]                        = 0;
    pSduDesc->scrsize[1]                        = 0;
    pSduDesc->pTable_DL_FEC_CTC_CB              = Table_DL_FEC_CTC_CB;
    pSduDesc->pUE_NIR_Table                     = UE_NIR_Table;
    pSduDesc->pBlkSizTab                        = BlkSizTab;
    pSduDesc->unlockPdschTcb                    = UNLOCK_DLSCH_TCB_INIT;
#endif


    //-------------------------------------------------
    // UL MEMORY ALLOCATIONS
    //-------------------------------------------------
    // DDR
    //-------------------------------------------------

    if (contextNum == 0)
    {
        pPhy->genBuf = MemAlloc(MAX_INPUT_TRANSPORT_BYTE_SIZE, CRAM_MEM, 0);
        _ASSERT_PTR(pPhy->genBuf);
        memset(pPhy->genBuf, 0, MAX_INPUT_TRANSPORT_BYTE_SIZE);

        addObj((U8*)pPhy->genBuf, RUP32B(MAX_INPUT_TRANSPORT_BYTE_SIZE), OBJECTNUMBER_GENBUF);

    }
    else
    {
        pPhy->genBuf = pPhy0->genBuf;
    }

    pBsRx->pBsRxPointers = MemAlloc(sizeof(BS_RX_POINTERS), PHY_MEM, 1);
    _ASSERT_PTR(pBsRx->pBsRxPointers);

    addObj((U8*)pBsRx->pBsRxPointers, sizeof(BS_RX_POINTERS), OBJECTNUMBER_BS_RX_POINTERS);

#if defined (RT_API_PROC)
    //  Rx Sdu Processing
    if (contextNum == 0)
    {
        int j;
        for (i = 0; i < MAXCHSUBFRAME; i++)
        {
            for (j = 0; j < 4; j++)
            {
                pRxSduDesc[j][i] = MemAlloc(sizeof(LTE_PHY_UL_SDU_DESC), PHY_MEM, 1);
                _ASSERT_PTR(pRxSduDesc[j][i]);

                addObj((U8*)pRxSduDesc[j][i], sizeof(LTE_PHY_UL_SDU_DESC), OBJECTNUMBER_LTE_PHY_UL_SDU_DESC);
            }
        }
    }
    gRxSduCtx = 0;
#endif

    //-------------------------------------------------
    // UL MEMORY ALLOCATIONS
    //-------------------------------------------------
    // CRAM
    //-------------------------------------------------
    // CONTEXT 0
    //-------------------------------------------------
    if (contextNum == 0)
    {
        pBsRx->GenULCramBufSize =
                      RUP32B(L * MAXCHSUBFRAME * sizeof(U32))           // pBsRx->rs_grid
                    + MAX_DEPENDENCIES * sizeof(S16)                    // pDependencyArray
                    + MAX_DEPENDENCIES * sizeof(S16)                    // pDependencyArray1
                    + MAX_DEPENDENCIES * sizeof(S16)                    // pDependencyArray2
                    + MAX_DEPENDENCIES * sizeof(S16)                    // pDependencyArray3
                    + MAX_DEPENDENCIES * sizeof(S16)                    // pDependencyArray4
                    + RUP32B(2 * MO * Nul_symb * sizeof(S16))           // pExpo_mat
                    + RUP32B(2 * MO * Nul_symb * sizeof(S16))           // pExpo_ch_pusch
                    + RUP32B(2 * MO * Nul_symb * sizeof(S16))           // pExpo_ch_pucch
                    + RUP32B(2*Nul_symb*sizeof(S16)*MAXCHSUBFRAME)      // pExpo_norm_reEst
                    + RUP32B(MO*MI*L*K*2*sizeof(S16))                   // pChEst_ul
                    + RUP32B(2*2*sizeof(S32)*MAXCHSUBFRAME)               // pErrAvgOut
                    + RUP32B(MO*2*sizeof(S16)*MAXCHSUBFRAME)            // pErrExpo
                    + RUP32B(2*sizeof(S32)*MAXCHSUBFRAME)               // mErrTemp
                    + RUP32B(sizeof(S16)*MAXCHSUBFRAME)                 // mErrExpoTemp
                    + RUP32B(8*2*sizeof(S32*))                          // ppSnrPerRb_Pilot x
                    + RUP32B(100*2*2*MO*sizeof(S32))                    // pSnrPerRb x
                    + RUP32B(MO * 2 *sizeof(S32)*MAXCHSUBFRAME)         // pnoiseVar
                    + NUM_USER_PUCCH_TCBS*
                      RUP32B(NUM_PUCCH_PER_TCB*sizeof(S16))             // pucchChan[NUM_USER_PUCCH_TCBS]
                    + RUP32B(step_reDet * (L - 2) * sizeof(S16))        // pReDet0
                    + RUP32B(MAX_MUXCTRL_SOFT_DEC * sizeof(S8))         // pMuxCtrl_SoftDec
                    + RUP32B(sizeof(ULRXINPUTPARAMS))                   // pBsRx->pUlRxInputParams
                    + RUP32B(sizeof(ULRXSDUPARAMS))                     // pBsRx->pUlRxSduParams
                    + RUP32B(15 * sizeof(S16))                          // pBsRx->isym_eff
                    + RUP32B(2 * 2 * K * sizeof(S16))                   // pBsRx->bs_aPilots
                    + RUP32B(sizeof(BS_PUSCH))                          // pBsRx->pBsPusch
                    + RUP32B(MI * MO * step_aDet * L * sizeof(S16))     // pBsRx->aDet
                    //FDD
                    + RUP32B(2 * MAXSUBCARRIERS * sizeof(S16))              // pBsRx->derotation
                    + RUP32B(2*2*2*3*PRB_12*MAXCHSUBFRAME*sizeof(S16))      // bs_aPilots_pucch x
                    + RUP32B(2*len_r_alpha_uv*MAXCHSUBFRAME*sizeof(S16))    // r_alpha_uv x
                    + RUP32B(2*2*2*MO*sizeof(S32)*MAXCHSUBFRAME)            // pErrAvg x
                    + RUP32B(2*2*MO*MAXCHSUBFRAME*sizeof(S32))              // rxInPwrAvg_PerRbPilot
                    + RUP32B(2*2*MO*MAXCHSUBFRAME*sizeof(S16))              // pPwrAvg_Pilot_expo
                    + RUP32B(2*2*MO*MAXCHSUBFRAME*sizeof(S32))              // rxInPwrAvg_PerRbDataSym
                    + RUP32B(2*2*MO*MAXCHSUBFRAME*sizeof(S16))              // pPwrAvg_Data_expo
                    + RUP32B(8*2*sizeof(S32)*MAXCHSUBFRAME)                 // pSnrPilot x
                    + RUP32B(2*NUM_SLOTS_SUBFRM * sizeof(S16) * MAXCHSUBFRAME)           // nOc x
                    + RUP32B(2*NUM_SLOTS_SUBFRM * sizeof(S16) * MAXCHSUBFRAME)           // Sdescramb x
                    + RUP32B(2*2*2*2*MO*MAXCHSUBFRAME*sizeof(S16))                       // pAvgReDet_pucchACK  x
                    + RUP32B(2*MAX_PUCCH_DATASYMB_PER_SUBF*MO*MAXCHSUBFRAME*sizeof(S16)) // pAvgReDet_pucchCQI  x
                    + RUP32B(2*2*2*2*MO*MAXCHSUBFRAME*sizeof(S16))                       // pChEst_pucch
                    + RUP32B(2*NUM_SLOTS_PER_SUBF*MO*MAXCHSUBFRAME*sizeof(S32))          // pAllPucchPwrAvg_AntSlot extra 2 for SR stuff
                    + RUP32B(2*NUM_SLOTS_PER_SUBF*MO*MAXCHSUBFRAME*sizeof(S32))          // pPucchPwrAvg

                    //For SRS
                    + RUP32B(MO*2*K*sizeof(S16)*MAX_UL_SRS)             // pSrsChEst
                    + RUP32B(2*sizeof(S16))                             // pRawSrsChEst
                    + RUP32B(2*K*MAX_UL_SRS)                            // pSrsPilots
                    + RUP32B(MAX_UL_SRS*2)                              // pLengthSrsPilots
                    + RUP32B(MAX_UL_SRS*2)                              // nCsSrs
                    + RUP32B(MAX_UL_SRS*pBsRx->Nul_rb*MO*sizeof(U32))   // pSrsSnrPerRb
                    + RUP32B(MAX_UL_SRS*pBsRx->Nul_rb*sizeof(U8))       // pSrsAvgSnrPerRbdB
                    + RUP32B(MAX_UL_SRS*sizeof(BSRXCHMeas))             // pSrsChMeas
                    + RUP32B(MI * MO * 2 * MAXRESOURCEBLOCKS * sizeof(S32)) // pRxRbPwrAvg_AntSlot
                    + RUP32B(MAXRESOURCEBLOCKS * sizeof(S32))               // pRxRip
                    + RUP32B(MI * MO * 2 * MAXRESOURCEBLOCKS * sizeof(U8))  // pRbUsedFlag
                    + RUP32B(sizeof(S16))                                    // pInitFlag
                    + RUP32B(MI * MO * 2 * MAXCHSUBFRAME * sizeof(S32))     // rssi for pusch and pucch
                    + RUP32B(sizeof(BS_RX_CHAN_SRS))                      // pBsRx->pBsRxChanSrs
                    + 2 * RUP32B(sizeof(ULRXFECPARAMS))                   // pBsRx->pUlRxFecParams[2]
                    + 2 * RUP32B(sizeof(BS_PRACH))                        // pBs->pBsRxPrachCram[2]
                    + RUP32B(sizeof(BS_SRS))                              // pBs->pBsSrs
                    + RUP32B(64*4*2)                                      // CA_srsBandwidthConfigTable_cram
                    + RUP32B(sizeof(U32) * MAXCHSUBFRAME)                 // pPhy->descrIndex
                    + RUP32B(sizeof(U32) * MAXCHSUBFRAME)                 // pPhy->demapOutSize
                    + RUP32B(MAX_INPUT_TRANSPORT_BYTE_SIZE)               // pPhy->descrInBufs
                    + RUP32B(2*2*PRB_12*MAX_PUCCH_PILOTSYMB_PER_SUBF*MO*MAXCHSUBFRAME*sizeof(S16))                   // pChEst_ul_pucch
                    + RUP32B(2*2*2*MO*MAXCHSUBFRAME*sizeof(S32))          // pErrSub_pucch
                    + RUP32B(2*2*MO*MAXCHSUBFRAME*sizeof(S16))            // pErrSubExpo_pucch
                    + RUP32B(sizeof(MAP_SETUP_DESCRIPTOR))                // pBsRx->pMapSetupDesc
                    + RUP32B((IDFT_CONFIG_TABLE_SIZE+IDFT_MULT_TABLE_SIZE)*2) // pIdftDriverBuf
                    ;

        pBsRx->GenULCramBuf = MemAlloc(pBsRx->GenULCramBufSize, CRAM_MEM, 1);
        _ASSERT_PTR(pBsRx->GenULCramBuf);

        pRxPtr = pBsRx->pBsRxPointers;

        offset = 0;
        p      = pBsRx->GenULCramBuf;

        pBsRx->rs_grid              = MemAllocFrom(p, &offset, RUP32B(L * MAXCHSUBFRAME * sizeof(U32)));
        addObj((U8*)pBsRx->rs_grid, RUP32B(L * MAXCHSUBFRAME * sizeof(U32)), OBJECTNUMBER_RS_GRID);
        pRxPtr->pDependencyArray    = MemAllocFrom(p, &offset, MAX_DEPENDENCIES * sizeof(S16));
        addObj((U8*)pRxPtr->pDependencyArray, MAX_DEPENDENCIES * sizeof(S16), OBJECTNUMBER_DEPENDENCYARRAY);
        pRxPtr->pDependencyArray1   = MemAllocFrom(p, &offset, MAX_DEPENDENCIES * sizeof(S16));
        addObj((U8*)pRxPtr->pDependencyArray1, MAX_DEPENDENCIES * sizeof(S16), OBJECTNUMBER_DEPENDENCYARRAY1);
        pRxPtr->pDependencyArray2   = MemAllocFrom(p, &offset, MAX_DEPENDENCIES * sizeof(S16));
        addObj((U8*)pRxPtr->pDependencyArray2, MAX_DEPENDENCIES * sizeof(S16), OBJECTNUMBER_DEPENDENCYARRAY2);
        pRxPtr->pDependencyArray3   = MemAllocFrom(p, &offset, MAX_DEPENDENCIES * sizeof(S16));
        addObj((U8*)pRxPtr->pDependencyArray3, MAX_DEPENDENCIES * sizeof(S16), OBJECTNUMBER_DEPENDENCYARRAY3);
        pRxPtr->pDependencyArray4   = MemAllocFrom(p, &offset, MAX_DEPENDENCIES * sizeof(S16));
        addObj((U8*)pRxPtr->pDependencyArray4, MAX_DEPENDENCIES * sizeof(S16), OBJECTNUMBER_DEPENDENCYARRAY4);
        pRxPtr->pExpo_mat           = MemAllocFrom(p, &offset, RUP32B(2 * MO * Nul_symb * sizeof(S16)));
        addObj((U8*)pRxPtr->pExpo_mat, RUP32B(2 * MO * Nul_symb * sizeof(S16)), OBJECTNUMBER_EXPO_MAT);
        pRxPtr->pExpo_ch_pusch      = MemAllocFrom(p, &offset, RUP32B(2 * MO * Nul_symb * sizeof(S16)));
        addObj((U8*)pRxPtr->pExpo_ch_pusch, RUP32B(2 * MO * Nul_symb * sizeof(S16)), OBJECTNUMBER_EXPO_CH_PUSCH);
        pRxPtr->pExpo_ch_pucch      = MemAllocFrom(p, &offset, RUP32B(2 * MO * Nul_symb * sizeof(S16)));
        addObj((U8*)pRxPtr->pExpo_ch_pucch, RUP32B(2 * MO * Nul_symb * sizeof(S16)), OBJECTNUMBER_EXPO_CH_PUCCH);
        pRxPtr->pExpo_norm_reEst    = MemAllocFrom(p, &offset, RUP32B(2 * Nul_symb * sizeof(S16) * MAXCHSUBFRAME));
        addObj((U8*)pRxPtr->pExpo_norm_reEst, RUP32B(2 * Nul_symb * sizeof(S16) * MAXCHSUBFRAME), OBJECTNUMBER_EXPO_NORM_REEST);
        pRxPtr->pChEst_ul           = MemAllocFrom(p, &offset, RUP32B(MO*MI*L*K*2*sizeof(S16)));
        addObj((U8*)pRxPtr->pChEst_ul, RUP32B(MO*MI*L*K*2*sizeof(S16)), OBJECTNUMBER_CHEST_UL);
        pRxPtr->pErrAvgOut          = MemAllocFrom(p, &offset, RUP32B(2*2*sizeof(S32)*MAXCHSUBFRAME));
        addObj((U8*)pRxPtr->pErrAvgOut, RUP32B(2*2*sizeof(S32)*MAXCHSUBFRAME), OBJECTNUMBER_ERRAVGOUT);
        pRxPtr->pErrExpo            = MemAllocFrom(p, &offset, RUP32B(MO*2*sizeof(S16)*MAXCHSUBFRAME));
        addObj((U8*)pRxPtr->pErrExpo, RUP32B(MO*2*sizeof(S16)*MAXCHSUBFRAME), OBJECTNUMBER_ERREXPO);
        pRxPtr->mErrTemp            = MemAllocFrom(p, &offset, RUP32B(2*sizeof(S32)*MAXCHSUBFRAME));
        addObj((U8*)pRxPtr->mErrTemp, RUP32B(2*sizeof(S32)*MAXCHSUBFRAME), OBJECTNUMBER_MPERRTEMP);
        pRxPtr->mErrExpoTemp        = MemAllocFrom(p, &offset, RUP32B(sizeof(S16)*MAXCHSUBFRAME));
        addObj((U8*)pRxPtr->mErrExpoTemp, RUP32B(2*sizeof(S16)*MAXCHSUBFRAME), OBJECTNUMBER_MPERREXPOTEMP);
        pRxPtr->ppSnrPerRb_Pilot    = MemAllocFrom(p, &offset, RUP32B(2*8*sizeof(S32*))); //x
        addObj((U8*)pRxPtr->ppSnrPerRb_Pilot, RUP32B(2*8*sizeof(S32*)), OBJECTNUMBER_SNRPERRB_PILOTS);
        pRxPtr->pSnrPerRb           = MemAllocFrom(p, &offset, RUP32B(2*100*2*MO*sizeof(S32))); //x
        addObj((U8*)pRxPtr->pSnrPerRb, RUP32B(2*100*2*MO*sizeof(S32)), OBJECTNUMBER_SNRPERRB);
        pRxPtr->pnoiseVar           = MemAllocFrom(p, &offset, RUP32B(MO * 2 * sizeof(S32)*MAXCHSUBFRAME));
        addObj((U8*)pRxPtr->pnoiseVar, RUP32B(MO * 2 * sizeof(S32)*MAXCHSUBFRAME), OBJECTNUMBER_NOISEVAR);

        for(i=0; i < NUM_USER_PUCCH_TCBS; i++)
        {
            pRxPtr->pucchChan[i]    = MemAllocFrom(p, &offset, RUP32B(NUM_PUCCH_PER_TCB*sizeof(S16)));
            addObj((U8*)pRxPtr->pucchChan[i], RUP32B(NUM_PUCCH_PER_TCB*sizeof(S16)), OBJECTNUMBER_PUCCHCHAN);
        }
        pRxPtr->pReDet0             = MemAllocFrom(p, &offset, RUP32B(step_reDet * (L - 2) * sizeof(S16)));
        addObj((U8*)pRxPtr->pReDet0, RUP32B(step_reDet * (L - 2) * sizeof(S16)), OBJECTNUMBER_REDET0);
        pRxPtr->pMuxCtrl_SoftDec    = MemAllocFrom(p, &offset, RUP32B(MAX_MUXCTRL_SOFT_DEC * sizeof(S8)));
        addObj((U8*)pRxPtr->pMuxCtrl_SoftDec, RUP32B(MAX_MUXCTRL_SOFT_DEC * sizeof(S8)), OBJECTNUMBER_MUXCTRL_SOFTDEC);
        pBsRx->pUlRxInputParams     = MemAllocFrom(p, &offset, RUP32B(sizeof(ULRXINPUTPARAMS)));
        addObj((U8*)pBsRx->pUlRxInputParams, RUP32B(sizeof(ULRXINPUTPARAMS)), OBJECTNUMBER_ULRXINPUTPARAMS);
        pBsRx->pUlRxSduParams       = MemAllocFrom(p, &offset, RUP32B(sizeof(ULRXSDUPARAMS)));
        addObj((U8*)pBsRx->pUlRxSduParams, RUP32B(sizeof(ULRXSDUPARAMS)), OBJECTNUMBER_ULRXSDUPARAMS);
        pBsRx->isym_eff             = MemAllocFrom(p, &offset, RUP32B(15 * sizeof(S16)));
        addObj((U8*)pBsRx->isym_eff, RUP32B(15 * sizeof(S16)), OBJECTNUMBER_ISYM_EFF);
        pBsRx->bs_aPilots           = MemAllocFrom(p, &offset, RUP32B(2 * 2 * K * sizeof(S16)));
        addObj((U8*)pBsRx->bs_aPilots, RUP32B(2 * 2 * K * sizeof(S16)), OBJECTNUMBER_BS_APILOTS);
        pBsRx->pBsPusch             = MemAllocFrom(p, &offset, RUP32B(sizeof(BS_PUSCH)));
        addObj((U8*)pBsRx->pBsPusch, RUP32B(sizeof(BS_PUSCH)), OBJECTNUMBER_BS_PUSCH);
        // FFT Output (in CRAM)
        pBsRx->aDet                 = MemAllocFrom(p, &offset, RUP32B(MI * MO * step_aDet * L * sizeof(S16)));
        addObj((U8*)pBsRx->aDet, RUP32B(MI * MO * step_aDet * L * sizeof(S16)), OBJECTNUMBER_ADET);

        //FDD
        pBsRx->derotation               = MemAllocFrom(p, &offset, RUP32B(2 * MAXSUBCARRIERS * sizeof(S16)));
        addObj((U8*)pBsRx->derotation, RUP32B(2 * MAXSUBCARRIERS * sizeof(S16)), OBJECTNUMBER_DEROTATION);
        pRxPtr->bs_aPilots_pucch        = MemAllocFrom(p, &offset, RUP32B(2*2*2*3*PRB_12*MAXCHSUBFRAME*sizeof(S16))); //x
        addObj((U8*)pRxPtr->bs_aPilots_pucch, RUP32B(2*2*2*3*PRB_12*MAXCHSUBFRAME*sizeof(S16)), OBJECTNUMBER_BS_APILOTS_PUCCH);
        pRxPtr->r_alpha_uv              = MemAllocFrom(p, &offset, RUP32B(2*len_r_alpha_uv*MAXCHSUBFRAME*sizeof(S16))); //x
        addObj((U8*)pRxPtr->r_alpha_uv, RUP32B(2*len_r_alpha_uv*MAXCHSUBFRAME*sizeof(S16)), OBJECTNUMBER_R_ALPHA_UV);
        pRxPtr->pErrAvg                 = MemAllocFrom(p, &offset, RUP32B(2*2*2*MO*sizeof(S32)*MAXCHSUBFRAME)); //x
        addObj((U8*)pRxPtr->pErrAvg, RUP32B(2*2*2*MO*sizeof(S32)*MAXCHSUBFRAME), OBJECTNUMBER_ERRAVG);
        pRxPtr->rxInPwrAvg_PerRbPilot   = MemAllocFrom(p, &offset, RUP32B(2*2*MO*MAXCHSUBFRAME*sizeof(S32)));
        addObj((U8*)pRxPtr->rxInPwrAvg_PerRbPilot, RUP32B(2*2*MO*sizeof(S32)*MAXCHSUBFRAME), OBJECTNUMBER_RXINPWRAVG_PERRBPILOT);
        pRxPtr->pPwrAvg_Pilot_expo      = MemAllocFrom(p, &offset, RUP32B(2*2*MO*MAXCHSUBFRAME*sizeof(S16)));
        addObj((U8*)pRxPtr->pPwrAvg_Pilot_expo, RUP32B(2*2*MO*sizeof(S16)*MAXCHSUBFRAME), OBJECTNUMBER_PWRAVG_PILOT_EXPO);
        pRxPtr->rxInPwrAvg_PerRbDataSym = MemAllocFrom(p, &offset, RUP32B(2*2*MO*MAXCHSUBFRAME*sizeof(S32)));
        addObj((U8*)pRxPtr->rxInPwrAvg_PerRbDataSym, RUP32B(2*2*MO*sizeof(S32)*MAXCHSUBFRAME), OBJECTNUMBER_RXINPWRAVG_PERRBDATASYM);
        pRxPtr->pPwrAvg_Data_expo       = MemAllocFrom(p, &offset, RUP32B(2*2*MO*MAXCHSUBFRAME*sizeof(S16)));
        addObj((U8*)pRxPtr->pPwrAvg_Data_expo, RUP32B(2*2*MO*sizeof(S16)*MAXCHSUBFRAME), OBJECTNUMBER_PWRAVG_DATA_EXPO);
        pRxPtr->pSnrPilot               = MemAllocFrom(p, &offset, RUP32B(2*8*sizeof(S32)*MAXCHSUBFRAME)); //x
        addObj((U8*)pRxPtr->pSnrPilot, RUP32B(2*8*sizeof(S32)*MAXCHSUBFRAME), OBJECTNUMBER_SNRPILOT);
        pRxPtr->nOc                     = MemAllocFrom(p, &offset, RUP32B(2*NUM_SLOTS_SUBFRM * sizeof(S16) * MAXCHSUBFRAME)); //x
        addObj((U8*)pRxPtr->nOc, RUP32B(2*NUM_SLOTS_SUBFRM * sizeof(S16) * MAXCHSUBFRAME), OBJECTNUMBER_NOC);
        pRxPtr->Sdescramb               = MemAllocFrom(p, &offset, RUP32B(2*NUM_SLOTS_SUBFRM * sizeof(S16) * MAXCHSUBFRAME)); //x
        addObj((U8*)pRxPtr->Sdescramb, RUP32B(2*NUM_SLOTS_SUBFRM * sizeof(S16) * MAXCHSUBFRAME), OBJECTNUMBER_SDESCRAMB);
        pRxPtr->pAvgReDet_pucchACK     = MemAllocFrom(p, &offset, RUP32B(2*2*2*2*MO*MAXCHSUBFRAME*sizeof(S16))); //x
        addObj((U8*)pRxPtr->pAvgReDet_pucchACK, RUP32B(2*2*2*2*MO*MAXCHSUBFRAME*sizeof(S16)), OBJECTNUMBER_PAVGREDET_PUCCHACK);
        pRxPtr->pAvgReDet_pucchCQI     = MemAllocFrom(p, &offset, RUP32B(2*MAX_PUCCH_DATASYMB_PER_SUBF*MO*MAXCHSUBFRAME*sizeof(S16))); //x
        addObj((U8*)pRxPtr->pAvgReDet_pucchCQI, RUP32B(2*MAX_PUCCH_DATASYMB_PER_SUBF*MO*MAXCHSUBFRAME*sizeof(S16)), OBJECTNUMBER_PAVGREDET_PUCCHCQI);
        pRxPtr->pChEst_pucch      = MemAllocFrom(p, &offset, RUP32B(2*2*2*2*MO*MAXCHSUBFRAME*sizeof(S16)));
        addObj((U8*)pRxPtr->pChEst_pucch, RUP32B(2*2*2*2*MO*MAXCHSUBFRAME*sizeof(S16)), OBJECTNUMBER_CHEST_PUCCH);
        pRxPtr->pAllPucchPwrAvg_AntSlot   = MemAllocFrom(p, &offset, RUP32B(2*NUM_SLOTS_PER_SUBF*MO*MAXCHSUBFRAME*sizeof(S32)));
        addObj((U8*)pRxPtr->pAllPucchPwrAvg_AntSlot, RUP32B(2*NUM_SLOTS_PER_SUBF*MO*MAXCHSUBFRAME*sizeof(S32)), OBJECTNUMBER_ALLPUCCHPWRAVG_ANTSLOT);
        pRxPtr->pPucchPwrAvg   = MemAllocFrom(p, &offset, RUP32B(2*NUM_SLOTS_PER_SUBF*MO*MAXCHSUBFRAME*sizeof(S32)));
        addObj((U8*)pRxPtr->pPucchPwrAvg, RUP32B(2*NUM_SLOTS_PER_SUBF*MO*MAXCHSUBFRAME*sizeof(S32)), OBJECTNUMBER_PUCCHPWRAVG);
        pRxPtr->pSrsChEst           = MemAllocFrom(p, &offset, RUP32B(MO*2*K*sizeof(S16)*MAX_UL_SRS));
        addObj((U8*)pRxPtr->pSrsChEst, RUP32B(MO*2*K*sizeof(S16)*MAX_UL_SRS), OBJECTNUMBER_SRSCHEST);
        pRxPtr->pRawSrsChEst        = MemAllocFrom(p, &offset, RUP32B(2*sizeof(S16)));
        addObj((U8*)pRxPtr->pRawSrsChEst, RUP32B(2*sizeof(S16)), OBJECTNUMBER_RAWSRSCHEST);
        pRxPtr->pSrsPilots          = MemAllocFrom(p, &offset, RUP32B(2*K*MAX_UL_SRS));
        addObj((U8*)pRxPtr->pSrsPilots, RUP32B(2*K*MAX_UL_SRS), OBJECTNUMBER_SRSPILOTS);
        pRxPtr->pLengthSrsPilots    = MemAllocFrom(p, &offset, RUP32B(MAX_UL_SRS*2));
        addObj((U8*)pRxPtr->pLengthSrsPilots, RUP32B(2*MAX_UL_SRS), OBJECTNUMBER_LENGTHSRSPILOTS);
        pRxPtr->nCsSrs              = MemAllocFrom(p, &offset, RUP32B(MAX_UL_SRS*2));
        addObj((U8*)pRxPtr->nCsSrs, RUP32B(2*MAX_UL_SRS), OBJECTNUMBER_NCSSRS);
        pRxPtr->pSrsSnrPerRb        = MemAllocFrom(p, &offset, RUP32B(MAX_UL_SRS*pBsRx->Nul_rb*MO*sizeof(U32)));
        addObj((U8*)pRxPtr->pSrsSnrPerRb, RUP32B(MAX_UL_SRS*pBsRx->Nul_rb*MO*sizeof(U32)), OBJECTNUMBER_SRSSNRPERRB);
        pRxPtr->pSrsAvgSnrPerRbdB   = MemAllocFrom(p, &offset, RUP32B(MAX_UL_SRS*pBsRx->Nul_rb*sizeof(U8)));
        addObj((U8*)pRxPtr->pSrsAvgSnrPerRbdB, RUP32B(MAX_UL_SRS*pBsRx->Nul_rb*sizeof(U8)), OBJECTNUMBER_SRSAVGSNRPERRBDB);
        pRxPtr->pSrsChMeas          = MemAllocFrom(p, &offset, RUP32B(MAX_UL_SRS*sizeof(BSRXCHMeas)));
        addObj((U8*)pRxPtr->pSrsChMeas, RUP32B(MAX_UL_SRS*pBsRx->Nul_rb*MO*sizeof(U32)), OBJECTNUMBER_SRSCHMEAS);
        pRxPtr->pRxInRbPwrAvg_AntSlot = MemAllocFrom(p, &offset, RUP32B(MO*2*MAXRESOURCEBLOCKS*sizeof(S32)));
        pRxPtr->pRxInRip            = MemAllocFrom(p, &offset, RUP32B(MAXRESOURCEBLOCKS*sizeof(S32)));
        pRxPtr->pRbUsedFlag         = MemAllocFrom(p, &offset, RUP32B(MO*2*MAXRESOURCEBLOCKS*sizeof(U8)));
        pRxPtr->pInitFlag           = MemAllocFrom(p, &offset, RUP32B(sizeof(S16)));
        pRxPtr->pRxRssi_AntSlot     = MemAllocFrom(p, &offset, RUP32B(MO*2*MAXCHSUBFRAME*sizeof(S32)));
        memset(pRxPtr->pInitFlag, 0, sizeof(S16));
        //End
        pBsRx->pBsRxChanSrs         = MemAllocFrom(p, &offset, RUP32B(sizeof(BS_RX_CHAN_SRS)));
        addObj((U8*)pBsRx->pBsRxChanSrs, RUP32B(sizeof(BS_RX_CHAN_SRS)), OBJECTNUMBER_BS_RX_CHAN_SRS);
        pBsRx->pUlRxFecParams[0]    = MemAllocFrom(p, &offset, RUP32B(sizeof(ULRXFECPARAMS)));
        addObj((U8*)pBsRx->pUlRxFecParams[0], RUP32B(sizeof(ULRXFECPARAMS)), OBJECTNUMBER_ULRXFECPARAMS);
        pBsRx->pUlRxFecParams[1]    = MemAllocFrom(p, &offset, RUP32B(sizeof(ULRXFECPARAMS)));
        addObj((U8*)pBsRx->pUlRxFecParams[1], RUP32B(sizeof(ULRXFECPARAMS)), OBJECTNUMBER_ULRXFECPARAMS);
        pBs->pBsRxPrachCram[0]      = MemAllocFrom(p, &offset, RUP32B(sizeof(BS_PRACH)));
        addObj((U8*)pBs->pBsRxPrachCram[0], RUP32B(sizeof(BS_PRACH)), OBJECTNUMBER_BS_PRACH);
        pBs->pBsRxPrachCram[1]      = MemAllocFrom(p, &offset, RUP32B(sizeof(BS_PRACH)));
        addObj((U8*)pBs->pBsRxPrachCram[1], RUP32B(sizeof(BS_PRACH)), OBJECTNUMBER_BS_PRACH);
        pBs->pBsSrs                 = MemAllocFrom(p, &offset, RUP32B(sizeof(BS_SRS)));
        addObj((U8*)pBs->pBsSrs, RUP32B(sizeof(BS_SRS)), OBJECTNUMBER_BS_SRS);
        CA_srsBandwidthConfigTable_cram = MemAllocFrom(p, &offset, RUP32B(64*4*2));
        addObj((U8*)CA_srsBandwidthConfigTable_cram, RUP32B(64*4*2), OBJECTNUMBER_SRSBANDWIDTHCONFIGTABLE);
        pPhy->descrIndex            = MemAllocFrom(p, &offset, RUP32B(sizeof(U32) * MAXCHSUBFRAME));
        addObj((U8*)pPhy->descrIndex, RUP32B(sizeof(U32) * MAXCHSUBFRAME), OBJECTNUMBER_DESCRINDEX);
        pPhy->demapOutSize          = MemAllocFrom(p, &offset, RUP32B(sizeof(U32) * MAXCHSUBFRAME));
        addObj((U8*)pPhy->demapOutSize, RUP32B(sizeof(U32) * MAXCHSUBFRAME), OBJECTNUMBER_DEMAPOUTSIZE);
        pPhy->descrInBufs           = MemAllocFrom(p, &offset, RUP32B(MAX_INPUT_TRANSPORT_BYTE_SIZE));
        addObj((U8*)pPhy->descrInBufs, RUP32B(MAX_INPUT_TRANSPORT_BYTE_SIZE), OBJECTNUMBER_DESCRINBUFS);
        pRxPtr->pChEst_ul_pucch     = MemAllocFrom(p, &offset, RUP32B(2*2*PRB_12*MAX_PUCCH_PILOTSYMB_PER_SUBF*MO*MAXCHSUBFRAME*sizeof(S16)));
        addObj((U8*)pRxPtr->pChEst_ul_pucch, RUP32B(2*2*PRB_12*MAX_PUCCH_PILOTSYMB_PER_SUBF*MO*MAXCHSUBFRAME*sizeof(S16)), OBJECTNUMBER_CHEST_UL_PUCCH);
        pRxPtr->pErrSub_pucch     = MemAllocFrom(p, &offset, RUP32B(2*2*2*MO*MAXCHSUBFRAME*sizeof(S32)));
        addObj((U8*)pRxPtr->pErrSub_pucch, RUP32B(2*2*2*MO*MAXCHSUBFRAME*sizeof(S32)), OBJECTNUMBER_ERRSUB_PUCCH);
        pRxPtr->pErrSubExpo_pucch     = MemAllocFrom(p, &offset, RUP32B(2*2*MO*MAXCHSUBFRAME*sizeof(S16)));
        addObj((U8*)pRxPtr->pErrSubExpo_pucch, RUP32B(2*2*MO*MAXCHSUBFRAME*sizeof(S16)), OBJECTNUMBER_ERRSUBEXPO_PUCCH);
        pBsRx->pMapSetupDesc          = MemAllocFrom(p, &offset, RUP32B(sizeof(MAP_SETUP_DESCRIPTOR)));
        addObj((U8*)pBsRx->pMapSetupDesc, RUP32B(sizeof(MAP_SETUP_DESCRIPTOR)), OBJECTNUMBER_MAP_SETUP_DESCRIPTOR);
        pBsRx->pIdftDriverBuf         = MemAllocFrom(p, &offset, RUP32B((IDFT_CONFIG_TABLE_SIZE+IDFT_MULT_TABLE_SIZE)*2));
        addObj((U8*)pBsRx->pIdftDriverBuf, RUP32B((IDFT_CONFIG_TABLE_SIZE+IDFT_MULT_TABLE_SIZE)*2), OBJECTNUMBER_CEVA_IDFT_TABLES);


        // FFT Input (in CRAM)
#ifdef _WIN32
        for(i = 0; i < pBsRx->Nrx_antennas; i++)
            pBsRx->adcbuf[i] = MemAlloc(2*length_adc, CRAM_MEM, 0);    // Multiply by 2 for bytes
#endif

        // FFT Input (in DDR)
        for(i = 0; i < pBsRx->Nrx_antennas; i++)
            pPhy->sigIn[0] = NULL;

        if (offset != pBsRx->GenULCramBufSize)
        {
            phy_printf("LteBsPhyInitCommomMemBuffers(UL 1): Something Fishy!!! offet: %d, GenULCramBufSize: %d\n",
                offset, pBsRx->GenULCramBufSize);
            while(1);
        }
    }

    //-------------------------------------------------
    // UL MEMORY ALLOCATIONS
    //-------------------------------------------------
    // CRAM
    //-------------------------------------------------
    // CONTEXT 1
    //-------------------------------------------------
    else
    {
        pBsRx->GenULCramBufSize =
                        + RUP32B(sizeof(ULRXINPUTPARAMS))                   // pBsRx->pUlRxInputParams
                        + RUP32B(sizeof(ULRXSDUPARAMS))                     // pBsRx->pUlRxSduParams
                        + NUM_USER_PUCCH_TCBS*
                          RUP32B(NUM_PUCCH_PER_TCB*sizeof(S16))             // pucchChan[NUM_USER_PUCCH_TCBS]
                        + RUP32B(MAX_UL_SRS*2)                              // pLengthSrsPilots
                        + RUP32B(MAX_UL_SRS*2)                              // nCsSrs
                        + RUP32B(MAX_UL_SRS*pBsRx->Nul_rb*MO*sizeof(U32))   // pSrsSnrPerRb
                        + RUP32B(MAX_UL_SRS*pBsRx->Nul_rb*sizeof(U8))       // pSrsAvgSnrPerRbdB
                        + RUP32B(MAX_UL_SRS*sizeof(BSRXCHMeas))             // pSrsChMeas
                        + RUP32B(sizeof(BS_RX_CHAN_SRS))                    // pBsRx->pBsRxChanSrs
                        + 2* RUP32B(sizeof(ULRXFECPARAMS))                  // pBsRx->pUlRxFecParams[2]
                        + 2 * RUP32B(sizeof(BS_PRACH))                      // pBs->pBsRxPrachCram[2]
                        + RUP32B(sizeof(BS_SRS))                            // pBs->pBsSrs
                        + RUP32B(sizeof(U32) * MAXCHSUBFRAME)               // pPhy->descrIndex
                        + RUP32B(sizeof(U32) * MAXCHSUBFRAME)               // pPhy->demapOutSize
                        + RUP32B(MAX_OUTPUT_TRANSPORT_BIT_SIZE)             // pPhy->descrInBufs
                        + RUP32B(sizeof(MAP_SETUP_DESCRIPTOR))              // pBsRx->pMapSetupDesc
                        + RUP32B((IDFT_CONFIG_TABLE_SIZE+IDFT_MULT_TABLE_SIZE)*2) // pIdftDriverBuf
                        + RUP32B(MI * MO * step_aDet * L * sizeof(S16))     // pBsRx->aDet
                        ;

        pBsRx->GenULCramBuf = MemAlloc(pBsRx->GenULCramBufSize, CRAM_MEM, 1);
        _ASSERT_PTR(pBsRx->GenULCramBuf);

        pRxPtr = pBsRx->pBsRxPointers;
        pBsRx0 = pPhy0->pBsRx;
        pRxPtr0 = pBsRx0->pBsRxPointers;

        offset = 0;
        p      = pBsRx->GenULCramBuf;

        // New Allocations for this context
        pBsRx->pUlRxInputParams     = MemAllocFrom(p, &offset, RUP32B(sizeof(ULRXINPUTPARAMS)));
        addObj((U8*)pBsRx->pUlRxInputParams, RUP32B(sizeof(ULRXINPUTPARAMS)), OBJECTNUMBER_ULRXINPUTPARAMS);
        pBsRx->pUlRxSduParams       = MemAllocFrom(p, &offset, RUP32B(sizeof(ULRXSDUPARAMS)));
        addObj((U8*)pBsRx->pUlRxSduParams, RUP32B(sizeof(ULRXSDUPARAMS)), OBJECTNUMBER_ULRXSDUPARAMS);

        for(i=0; i < NUM_USER_PUCCH_TCBS; i++)
        {
            pRxPtr->pucchChan[i]    = MemAllocFrom(p, &offset, RUP32B(NUM_PUCCH_PER_TCB*sizeof(S16)));
            addObj((U8*)pRxPtr->pucchChan[i], RUP32B(NUM_PUCCH_PER_TCB*sizeof(S16)), OBJECTNUMBER_PUCCHCHAN);
        }

        pRxPtr->pLengthSrsPilots    = MemAllocFrom(p, &offset, RUP32B(MAX_UL_SRS*2));
        addObj((U8*)pRxPtr->pLengthSrsPilots, RUP32B(2*MAX_UL_SRS), OBJECTNUMBER_LENGTHSRSPILOTS);
        pRxPtr->nCsSrs              = MemAllocFrom(p, &offset, RUP32B(MAX_UL_SRS*2));
        addObj((U8*)pRxPtr->nCsSrs, RUP32B(2*MAX_UL_SRS), OBJECTNUMBER_NCSSRS);
        pRxPtr->pSrsSnrPerRb        = MemAllocFrom(p, &offset, RUP32B(MAX_UL_SRS*pBsRx->Nul_rb*MO*sizeof(U32)));
        addObj((U8*)pRxPtr->pSrsSnrPerRb, RUP32B(MAX_UL_SRS*pBsRx->Nul_rb*MO*sizeof(U32)), OBJECTNUMBER_SRSSNRPERRB);
        pRxPtr->pSrsAvgSnrPerRbdB   = MemAllocFrom(p, &offset, RUP32B(MAX_UL_SRS*pBsRx->Nul_rb*sizeof(U8)));
        addObj((U8*)pRxPtr->pSrsAvgSnrPerRbdB, RUP32B(MAX_UL_SRS*pBsRx->Nul_rb*sizeof(U8)), OBJECTNUMBER_SRSAVGSNRPERRBDB);
        pRxPtr->pSrsChMeas          = MemAllocFrom(p, &offset, RUP32B(MAX_UL_SRS*sizeof(BSRXCHMeas)));
        addObj((U8*)pRxPtr->pSrsChMeas, RUP32B(MAX_UL_SRS*sizeof(BSRXCHMeas)), OBJECTNUMBER_SRSCHMEAS);
        pBsRx->pBsRxChanSrs         = MemAllocFrom(p, &offset, RUP32B(sizeof(BS_RX_CHAN_SRS)));
        addObj((U8*)pBsRx->pBsRxChanSrs, RUP32B(sizeof(BS_RX_CHAN_SRS)), OBJECTNUMBER_BS_RX_CHAN_SRS);
        pBsRx->pUlRxFecParams[0]    = MemAllocFrom(p, &offset, RUP32B(sizeof(ULRXFECPARAMS)));
        addObj((U8*)pBsRx->pUlRxFecParams[0], RUP32B(sizeof(ULRXFECPARAMS)), OBJECTNUMBER_ULRXFECPARAMS);
        pBsRx->pUlRxFecParams[1]    = MemAllocFrom(p, &offset, RUP32B(sizeof(ULRXFECPARAMS)));
        addObj((U8*)pBsRx->pUlRxFecParams[1], RUP32B(sizeof(ULRXFECPARAMS)), OBJECTNUMBER_ULRXFECPARAMS);
        pBs->pBsRxPrachCram[0]      = MemAllocFrom(p, &offset, RUP32B(sizeof(BS_PRACH)));
        addObj((U8*)pBs->pBsRxPrachCram[0], RUP32B(sizeof(BS_PRACH)), OBJECTNUMBER_BS_PRACH);
        pBs->pBsRxPrachCram[1]      = MemAllocFrom(p, &offset, RUP32B(sizeof(BS_PRACH)));
        addObj((U8*)pBs->pBsRxPrachCram[1], RUP32B(sizeof(BS_PRACH)), OBJECTNUMBER_BS_PRACH);
        pBs->pBsSrs                 = MemAllocFrom(p, &offset, RUP32B(sizeof(BS_SRS)));
        addObj((U8*)pBs->pBsRxPrachCram[1], RUP32B(sizeof(BS_SRS)), OBJECTNUMBER_BS_SRS);
        pPhy->descrIndex            = MemAllocFrom(p, &offset, RUP32B(sizeof(U32) * MAXCHSUBFRAME));
        addObj((U8*)pPhy->descrIndex, RUP32B(sizeof(U32) * MAXCHSUBFRAME), OBJECTNUMBER_DESCRINDEX);
        pPhy->demapOutSize          = MemAllocFrom(p, &offset, RUP32B(sizeof(U32) * MAXCHSUBFRAME));
        addObj((U8*)pPhy->demapOutSize, RUP32B(sizeof(U32) * MAXCHSUBFRAME), OBJECTNUMBER_DEMAPOUTSIZE);
        pPhy->descrInBufs           = MemAllocFrom(p, &offset, RUP32B(MAX_OUTPUT_TRANSPORT_BIT_SIZE));
        addObj((U8*)pPhy->descrIndex, RUP32B(MAX_INPUT_TRANSPORT_BYTE_SIZE), OBJECTNUMBER_DESCRINBUFS);
        pBsRx->pMapSetupDesc        = MemAllocFrom(p, &offset, RUP32B(sizeof(MAP_SETUP_DESCRIPTOR)));
        addObj((U8*)pBsRx->pMapSetupDesc, RUP32B(sizeof(MAP_SETUP_DESCRIPTOR)), OBJECTNUMBER_MAP_SETUP_DESCRIPTOR);
        pBsRx->pIdftDriverBuf         = MemAllocFrom(p, &offset, RUP32B((IDFT_CONFIG_TABLE_SIZE+IDFT_MULT_TABLE_SIZE)*2));
        addObj((U8*)pBsRx->pIdftDriverBuf, RUP32B((IDFT_CONFIG_TABLE_SIZE+IDFT_MULT_TABLE_SIZE)*2), OBJECTNUMBER_CEVA_IDFT_TABLES);

        // FFT Output (in CRAM)
        pBsRx->aDet                 = MemAllocFrom(p, &offset, RUP32B(MI * MO * step_aDet * L * sizeof(S16)));
        addObj((U8*)pBsRx->aDet, RUP32B(MI * MO * step_aDet * L * sizeof(S16)), OBJECTNUMBER_ADET);

        if (offset != pBsRx->GenULCramBufSize)
        {
            printf("LteBsPhyInitCommomMemBuffers(UL 2): Something Fishy!!! offet: %d, GenULCramBufSize: %d\n",
                offset, pBsRx->GenULCramBufSize);
            while(1);
        }

        // Re-use Old Context
        pBsRx->rs_grid              = pBsRx0->rs_grid;
        pBsRx->derotation           = pBsRx0->derotation;
        pRxPtr->pDependencyArray    = pRxPtr0->pDependencyArray;
        pRxPtr->pDependencyArray1   = pRxPtr0->pDependencyArray1;
        pRxPtr->pDependencyArray2   = pRxPtr0->pDependencyArray2;
        pRxPtr->pDependencyArray3   = pRxPtr0->pDependencyArray3;
        pRxPtr->pDependencyArray4   = pRxPtr0->pDependencyArray4;
        pRxPtr->bs_aPilots_pucch    = pRxPtr0->bs_aPilots_pucch;
        pRxPtr->pExpo_mat           = pRxPtr0->pExpo_mat;
        pRxPtr->pExpo_ch_pusch      = pRxPtr0->pExpo_ch_pusch;
        pRxPtr->pExpo_ch_pucch      = pRxPtr0->pExpo_ch_pucch;
        pRxPtr->pExpo_norm_reEst    = pRxPtr0->pExpo_norm_reEst;
        pRxPtr->pChEst_ul           = pRxPtr0->pChEst_ul;
        pRxPtr->r_alpha_uv          = pRxPtr0->r_alpha_uv;
        pRxPtr->pErrAvg             = pRxPtr0->pErrAvg;
        pRxPtr->pErrAvgOut          = pRxPtr0->pErrAvgOut;
        pRxPtr->pErrExpo            = pRxPtr0->pErrExpo;
        pRxPtr->mErrTemp            = pRxPtr0->mErrTemp;
        pRxPtr->mErrExpoTemp        = pRxPtr0->mErrExpoTemp;
        pRxPtr->rxInPwrAvg_PerRbPilot   = pRxPtr0->rxInPwrAvg_PerRbPilot;
        pRxPtr->pPwrAvg_Pilot_expo      = pRxPtr0->pPwrAvg_Pilot_expo;
        pRxPtr->rxInPwrAvg_PerRbDataSym = pRxPtr0->rxInPwrAvg_PerRbDataSym;
        pRxPtr->pPwrAvg_Data_expo       = pRxPtr0->pPwrAvg_Data_expo;
        pRxPtr->pSnrPilot           = pRxPtr0->pSnrPilot;
        pRxPtr->ppSnrPerRb_Pilot    = pRxPtr0->ppSnrPerRb_Pilot;
        pRxPtr->pSnrPerRb           = pRxPtr0->pSnrPerRb;
        pRxPtr->pnoiseVar           = pRxPtr0->pnoiseVar;
        pRxPtr->nOc                 = pRxPtr0->nOc;
        pRxPtr->Sdescramb           = pRxPtr0->Sdescramb;
        pRxPtr->pReDet0             = pRxPtr0->pReDet0;
        pRxPtr->pMuxCtrl_SoftDec    = pRxPtr0->pMuxCtrl_SoftDec;

        pBsRx->isym_eff             = pBsRx0->isym_eff;
        pBsRx->bs_aPilots           = pBsRx0->bs_aPilots;

        pBsRx->pBsPusch             = pBsRx0->pBsPusch;
        pRxPtr->pAvgReDet_pucchACK  = pRxPtr0->pAvgReDet_pucchACK;
        pRxPtr->pAvgReDet_pucchCQI  = pRxPtr0->pAvgReDet_pucchCQI;
        pRxPtr->pChEst_pucch        = pRxPtr0->pChEst_pucch;
        pRxPtr->pAllPucchPwrAvg_AntSlot       = pRxPtr0->pAllPucchPwrAvg_AntSlot;
        pRxPtr->pPucchPwrAvg       = pRxPtr0->pPucchPwrAvg;
        pRxPtr->pSrsChEst           = pRxPtr0->pSrsChEst;
        pRxPtr->pRawSrsChEst        = pRxPtr0->pRawSrsChEst;
        pRxPtr->pSrsPilots          = pRxPtr0->pSrsPilots;
        pRxPtr->pChEst_ul_pucch     = pRxPtr0->pChEst_ul_pucch;
        pRxPtr->pErrSub_pucch       = pRxPtr0->pErrSub_pucch;
        pRxPtr->pErrSubExpo_pucch   = pRxPtr0->pErrSubExpo_pucch;
        pRxPtr->pRxInRip            = pRxPtr0->pRxInRip;
        pRxPtr->pRbUsedFlag         = pRxPtr0->pRbUsedFlag;
        pRxPtr->pInitFlag           = pRxPtr0->pInitFlag;
        pRxPtr->pRxRssi_AntSlot     = pRxPtr0->pRxRssi_AntSlot;
        pRxPtr->pRxInRbPwrAvg_AntSlot = pRxPtr0->pRxInRbPwrAvg_AntSlot;

        // FFT Input (in CRAM)
        for(i=0; i < pBsRx->Nrx_antennas; i++)
            pBsRx->adcbuf[i]    = pBsRx0->adcbuf[i] + length_adc; // Allocated enough space for 2 contexts in the first one

        // FFT Input (in DDR)
        for(i = 0; i < pBsRx->Nrx_antennas; i++)
            pPhy->sigIn[0] = NULL;

    }

    //-------------------------------------------------
    // END UL MEMORY ALLOCATIONS
    //-------------------------------------------------


    //Setup Base Input parameters which do not change for 1st context
    SetRxInputParams(pPhy);
    if (contextNum == 0)
       memcpy(CA_srsBandwidthConfigTable_cram, CA_srsBandwidthConfigTable, 64*4*2);


    //-------------------------------------------------
    // TASK LIST INITIALIZATION
    //-------------------------------------------------
    MLogSetMask(0xFFFFFFFF);
    pBsTx->pExeTaskList = CreateTaskList((U16)(DL_TASKLIST_START_IDX+contextNum), NULL);
    _ASSERT_PTR(pBsTx->pExeTaskList);

    pBsRx->pExeTaskList[0] = CreateTaskList((U16)(UL_TASKLIST_START_IDX+contextNum), NULL);
    _ASSERT_PTR(pBsRx->pExeTaskList[0]);

    pBsRx->pExePrachTaskList[0] = CreateTaskList((U16)(PRACH_TASKLIST_START_IDX+contextNum), NULL);
    _ASSERT_PTR(pBsRx->pExePrachTaskList[0]);

    pBsRx->pExePrachTaskList[1] = CreateTaskList((U16)(PRACH_TASKLIST_START_IDX+2+contextNum), NULL);
    _ASSERT_PTR(pBsRx->pExePrachTaskList[1]);

    pBsRx->pExeSrsTaskList[0] = CreateTaskList((U16)(SRS_TASKLIST_START_IDX+contextNum), NULL);
    _ASSERT_PTR(pBsRx->pExeSrsTaskList[0]);

    if (pBsTx->pExeTaskList == NULL ||
        pBsRx->pExeTaskList[0]    == NULL || pBsRx->pExePrachTaskList[0] == NULL ||
        pBsRx->pExeSrsTaskList[0] == NULL || pBsRx->pExePrachTaskList[1] == NULL)
    {
        phy_printf("Failed to create Task List!\nPDSCH: 0x%08x\nDL_SET: 0x%08x\nPUSCH/PUCCH: 0x%08x\nPRACH0: 0x%08x\nPRACH1: 0x%08x\nSRS: 0x%08x\n",
        pBsTx->pExeTaskList,
        pBsRx->pExeTaskList[0],
        pBsRx->pExePrachTaskList[0],
        pBsRx->pExePrachTaskList[1],
        pBsRx->pExeSrsTaskList[0]);
        while (1);
    }
    MLogSetMask(0);

    return 0;
}

//-------------------------------------------------------------------------------------------
/** @brief Destroys all common buffers created in Phy Init
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   pPhy        Pointer to the current active PHY structure
 *  @param   contextNum  Structure number for the N/N+1 Real Time scheme
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -----------------------------------------------------------------------------------------------------
 *  |Name          |Name in   |Direction      |Length  |Type          |Data Format |Valid Range |Units  |
 *  |              |Standard  |               |        |              |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |pPhy          |          |pInput/pOutput |        |structure     |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |contextNum    |          |input          |1       |unsigned int  |(0:31:0)    |[0:1]       |       |
 *  -----------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |LteBsPhyDestroyCommonMemBuffers  |
 *  --------------------------------------------------------
 *  |Input Stream        |pPhy                             |
 *  --------------------------------------------------------
 *  |Output Stream       |                                 |
 *  --------------------------------------------------------
 *  |Config Parameters   |contextNum                       |
 *  --------------------------------------------------------
 *  |Status Parameters   |none                             |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Destroys all common buffers created during Phy Init
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *  none
 *
 *
 *  \ingroup group_lte_api
 *
 **/
//-------------------------------------------------------------------------------------------
static LteBsPhyDestroyCommonMemBuffers(PLTEPHYDESC pPhy, U32 contextNum)
{
    PBS_TX pBsTx = pPhy->pBsTx;
    PBS_RX pBsRx = pPhy->pBsRx;
    PBSDLINITMEMBUF pBsDlInitMemBuf = pBsTx->pBsDlInitMemBuf;
    U32 i, j = 0;

    //-------------------------------------------------
    // DL MEMORY ALLOCATIONS
    //-------------------------------------------------
    // DDR
    //-------------------------------------------------
    if (contextNum == 0)
    {
        if (pBsDlInitMemBuf->pBchDataBuffer)
        {
            MemFree(pBsDlInitMemBuf->pBchDataBuffer, NCNB_MEM);
            pBsDlInitMemBuf->pBchDataBuffer = NULL;
        }
        if (pBsDlInitMemBuf->pDLCellSpecificScrSeq)
        {
            MemFree(pBsDlInitMemBuf->pDLCellSpecificScrSeq, NCNB_MEM);
            pBsDlInitMemBuf->pDLCellSpecificScrSeq = NULL;
        }
    }

    //-------------------------------------------------
    // DL MEMORY ALLOCATIONS
    //-------------------------------------------------
    // IRAM
    //-------------------------------------------------
    if (gCRC24A_buf[contextNum])
    {
        MemFree(gCRC24A_buf[contextNum], IRAM_MEM);
        gCRC24A_buf[contextNum] = NULL;
    }
    if (gScrInBufs[contextNum][0])
    {
        MemFree(gScrInBufs[contextNum][0], IRAM_MEM);
        gScrInBufs[contextNum][0] = NULL;
    }
    if (gScrInBufs[contextNum][1])
    {
        MemFree(gScrInBufs[contextNum][1], IRAM_MEM);
        gScrInBufs[contextNum][1] = NULL;
    }

    //-------------------------------------------------
    // DL MEMORY ALLOCATIONS
    //-------------------------------------------------
    // CRAM
    //-------------------------------------------------
#ifndef WIN32
    if (gpSduDesc[contextNum])
    {
        MemFree(gpSduDesc[contextNum], CRAM_MEM);
        gpSduDesc[contextNum] = NULL;
    }
#endif

    if (pBsTx->GenDLCramBuf)
    {
        MemFree(pBsTx->GenDLCramBuf, CRAM_MEM);
        pBsTx->GenDLCramBuf = NULL;
    }

#ifdef _WIN32
    if (contextNum == 0)
    {
        for(i = 0; i < pBsTx->Ntx_antennas; i++)
        {
            if (pBsTx->ifftOut[i])
            {
                MemFree(pBsTx->ifftOut[i], CRAM_MEM);
                pBsTx->ifftOut[i] = NULL;
            }
        }

        if (pPhy->sigOut[0])
        {
            MemFree(pPhy->sigOut[0], CRAM_MEM);
            pPhy->sigOut[0] = NULL;
        }
    }
    else
    {
        for(i = 0; i < pBsTx->Ntx_antennas; i++)
            pBsTx->ifftOut[i] = NULL;

        pPhy->sigOut[0] = NULL;
    }
#endif

    //-------------------------------------------------
    // END DL MEMORY ALLOCATIONS
    //-------------------------------------------------



    //-------------------------------------------------
    // UL MEMORY ALLOCATIONS
    //-------------------------------------------------
    // DDR
    //-------------------------------------------------
    if (contextNum == 0)
    {
        if (pPhy->genBuf)
        {
            MemFree(pPhy->genBuf, CRAM_MEM);
            pPhy->genBuf = NULL;
        }
    }
    else
    {
        pPhy->genBuf = NULL;
    }

    if (pBsRx->pBsRxPointers)
    {
        MemFree(pBsRx->pBsRxPointers, PHY_MEM);
        pBsRx->pBsRxPointers = NULL;
    }

    //-------------------------------------------------
    // UL MEMORY ALLOCATIONS
    //-------------------------------------------------
    // CRAM
    //-------------------------------------------------
    if (pBsRx->GenULCramBuf)
    {
        MemFree(pBsRx->GenULCramBuf, CRAM_MEM);
        pBsRx->GenULCramBuf = NULL;
    }

#ifdef _WIN32
    if (contextNum == 0)
    {
        for(i = 0; i < (U32)pBsRx->Nrx_antennas; i++)
        {
            if (pBsRx->adcbuf[i])
            {
                MemFree(pBsRx->adcbuf[i], CRAM_MEM);
                pBsRx->adcbuf[i] = NULL;
            }
        }
    }
#endif


#ifndef _WIN32
    if (contextNum == 0)
    {
        for (i = 0; i < MAXCHSUBFRAME; i++)
        {
            for (j = 0; j < NUM_UL_SDU_CONTEXT; j++)
            {
                if (pRxSduDesc[j][i])
                {
                    MemFree(pRxSduDesc[j][i], PHY_MEM);
                    pRxSduDesc[j][i] = NULL;
                }
            }
        }
    }
#endif



    //-------------------------------------------------
    // END UL MEMORY ALLOCATIONS
    //-------------------------------------------------


    //-------------------------------------------------
    // TASK LIST INITIALIZATION
    //-------------------------------------------------
    DestroyTaskList(pBsTx->pExeTaskList);
    pBsTx->pExeTaskList = NULL;

    DestroyTaskList(pBsRx->pExeTaskList[0]);
    pBsRx->pExeTaskList[0] = NULL;

    DestroyTaskList(pBsRx->pExePrachTaskList[0]);
    pBsRx->pExePrachTaskList[0] = NULL;

    DestroyTaskList(pBsRx->pExePrachTaskList[1]);
    pBsRx->pExePrachTaskList[1] = NULL;

    DestroyTaskList(pBsRx->pExeSrsTaskList[0]);
    pBsRx->pExeSrsTaskList[0] = NULL;

}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   sfCxtNum DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |sfCxtNum      |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
static U32 LteBsPhySetPointers(U32 sfCxtNum)
{
    Inst0[sfCxtNum].pBs = &bs[sfCxtNum];
    Inst0[sfCxtNum].pBsTx = &bsTx[sfCxtNum];
    Inst0[sfCxtNum].pBsRx = &bsRx[sfCxtNum];
    Inst0[sfCxtNum].pBs->pBsRxPrach[0] = &bsRxPrach[2*sfCxtNum];
    Inst0[sfCxtNum].pBs->pBsRxPrach[1] = &bsRxPrach[2*sfCxtNum+1];
    Inst0[sfCxtNum].pBs->pBsRxPrachDet[0] = &bsRxPrachDet[2*sfCxtNum];
    Inst0[sfCxtNum].pBs->pBsRxPrachDet[1] = &bsRxPrachDet[2*sfCxtNum+1];

    addObj((U8*) &Inst0[sfCxtNum], sizeof(LTEPHYDESC), OBJECTNUMBER_LTEPHYDESC);
    addObj((U8*) &bs[sfCxtNum], sizeof(BS), OBJECTNUMBER_BS);
    addObj((U8*) &bsTx[sfCxtNum], sizeof(BS_TX), OBJECTNUMBER_BS_TX);
    addObj((U8*) &bsRx[sfCxtNum], sizeof(BS_RX), OBJECTNUMBER_BS_RX);
    addObj((U8*) &bsRxPrach[2*sfCxtNum], sizeof(BS_RX_PRACH), OBJECTNUMBER_BS_RX_PRACH);
    addObj((U8*) &bsRxPrach[2*sfCxtNum+1], sizeof(BS_RX_PRACH), OBJECTNUMBER_BS_RX_PRACH);
    addObj((U8*) &bsRxPrachDet[2*sfCxtNum], sizeof(BS_RX_PRACHDet), OBJECTNUMBER_BS_RX_PRACHDet);
    addObj((U8*) &bsRxPrachDet[2*sfCxtNum+1], sizeof(BS_RX_PRACHDet), OBJECTNUMBER_BS_RX_PRACHDet);

    if (sfCxtNum == 0)
    {
        Inst0[sfCxtNum].pBsTx->pBsDlInitMemBuf = &glb_bsDlInitMemBuf[sfCxtNum];
        Inst0[sfCxtNum].pBsTx->pBsDlInitParam = &glb_bsDlInitParam[sfCxtNum];

        addObj((U8*) &glb_bsDlInitMemBuf[sfCxtNum], sizeof(BS_DL_INIT_MEMBUF), OBJECTNUMBER_BS_DL_INIT_MEMBUF);
        addObj((U8*) &glb_bsDlInitParam[sfCxtNum], sizeof(BS_DL_INIT_PARAM), OBJECTNUMBER_BS_DL_INIT_PARAM);
    }
    else
    {
        Inst0[sfCxtNum].pBsTx->pBsDlInitMemBuf = Inst0[0].pBsTx->pBsDlInitMemBuf;
        Inst0[sfCxtNum].pBsTx->pBsDlInitParam = Inst0[0].pBsTx->pBsDlInitParam;
    }

    setPhyCtx(0, sfCxtNum, &Inst0[sfCxtNum]);

    return 0;
}

//-------------------------------------------------------------------------------------------
/** @brief Initialize Context parameters used for UL and DL in PHY code
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   pPhy        Pointer to the current active PHY structure
 *  @param   parmPtr     Pointer to a INITPARM structure that contains the
 *                       information required to initialize the Phy
 *  @param   contextNum  Structure number for the N/N+1 Real Time scheme
 *  @param   pPhy0       Pointer to the first active PHY structure (subframe N)
 *
 *  @return  status    A value of zero indicate success a value of 1 indicates
 *                     failure (We can expand to indicate the specific failure
 *                     reason)
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -----------------------------------------------------------------------------------------------------
 *  |Name          |Name in   |Direction      |Length  |Type          |Data Format |Valid Range |Units  |
 *  |              |Standard  |               |        |              |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |pPhy          |          |pInput/pOutput |        |structure     |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |parmPtr       |          |pInput         |        |structure     |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |contextNum    |          |input          |1       |unsigned int  |(0:31:0)    |[0:1]       |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |pPhy0         |          |pInput         |        |structure     |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |status        |          |Output         |        |unsigned int  |(0:31:0)    |[0:1]       |       |
 *  -----------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -------------------------------------------
 *  |Block Name          |LteBsPhyInitContext |
 *  -------------------------------------------
 *  |Input Stream        |pPhy, pPhy0, parmPtr|
 *  -------------------------------------------
 *  |Output Stream       |pPhy                |
 *  -------------------------------------------
 *  |Config Parameters   |contextNum          |
 *  -------------------------------------------
 *  |Status Parameters   |status              |
 *  -------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Initialize Context parameters used for UL and DL in PHY code. These variables are initializes
 *  with data that is passed from upper MAC layer.
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *  none
 *
 *
 *  \ingroup group_lte_api
 *
 **/
//-------------------------------------------------------------------------------------------
static U32 LteBsPhyInitContext(PLTEPHYDESC pPhy, PINITPARM parmPtr, U32 contextNum, PLTEPHYDESC pPhy0)
{
    U32 status = 0;
    PBS pBs;
    PBS_TX pBsTx;
    PBS_RX pBsRx;
    PBS_SRS pBsSrs;
    PBS_RX_PRACH pBsRxPrach[2];
    PBS_RX_PRACHDet pBsRxPrachDet[2];
    U16 frameNumber;
    S16 subframeNumber, i;
    PBSDLINITPARAM  pBsDlInitParam;

    if (pPhy == NULL)
        return FAILURE;

    // Now initialize the PHY instance with the information contained
    // in the INITPARM structure
    pBs = pPhy->pBs;
    pBsTx = pPhy->pBsTx;
    pBsRx = pPhy->pBsRx;
    pBsRxPrach[0] = pBs->pBsRxPrach[0];
    pBsRxPrach[1] = pBs->pBsRxPrach[1];
    pBsRxPrachDet[0] = pBs->pBsRxPrachDet[0];
    pBsRxPrachDet[1] = pBs->pBsRxPrachDet[1];

    pBsTx->Ntx_antennas = parmPtr->txAntennaPortCount;
    pBsRx->Nrx_antennas = parmPtr->rxAntennaPortCount;
    pBs->cellGroup = parmPtr->phyLayerCellIdGroup;
    pBs->cellSector = parmPtr->phyLayerId;
    pBs->Ncell_id = (U16) ((pBs->cellGroup << 1) + pBs->cellGroup + pBs->cellSector);
    pBs->frameDuplexType = parmPtr->frtypeDuplexmode;
    pBs->P_b = (S8) (parmPtr->pb);

    pBsTx->SyncTxAntennaNum = (parmPtr->reserved & 0x3); // mask LS 2 bits
    if(pBsTx->SyncTxAntennaNum > 2 || (pBsTx->SyncTxAntennaNum == 1 && pBsTx->Ntx_antennas == 1)) // invalid
    {
       pBsTx->SyncTxAntennaNum = 0; // forced
    }
    pBs->Nused = parmPtr->numberUsedSubcarriers;
    pBs->CPType = parmPtr->cyclicPrefixType;
    pBs->Nrb_sc = parmPtr->physicalResourceBandwidth;
    pBsTx->Ndl_symb = LteDetermineSymbolsPerSlot(pBs->CPType, pBs->Nrb_sc);

    pBsTx->Ndl_rb = parmPtr->nMaxDlRb;
    subframeNumber = (parmPtr->subframeNumber + contextNum) % 10;
    frameNumber = (parmPtr->radioFrameNumber + ((parmPtr->subframeNumber+contextNum > 9) ? 1:0)) % 1024;
    pPhy->txFrameNumber = frameNumber;
    pBsTx->subframeNumber = subframeNumber;
    pPhy->rxFrameNumber = frameNumber;
    pBsRx->subframeNumber = subframeNumber;
    pBs->Nfft = parmPtr->fftSize;
    pBs->Nrb = parmPtr->nMaxDlRb;
    pBs->expoNfft = CEXPFFT[parmPtr->channelBandwidth];
    pPhy->dacTxScale = DACSCALE[parmPtr->channelBandwidth];

    // Setup pointer to low and high frequency index to the middle six
    // resource blocks in the frequency domain. Depends on signal bandwidth
    // (Profile), since each bandwidth has a different number of Resource
    // Blocks
    pBsTx->RBIndexMid6Lo = CA_RBIndexMid6Lo[parmPtr->channelBandwidth];
    pBsTx->RBIndexMid6Hi = CA_RBIndexMid6Hi[parmPtr->channelBandwidth];

    LteDetermineCpsandSampling(parmPtr, pPhy);

    if (parmPtr->physicalResourceBandwidth == PRB_12)
    {
        pBs->df = CS_15K;
    }
    else
    {
        pBs->df = CS_7500;
    }

    // API confirmations (all disabled by default)
    pPhy->confEnableMask = 0;
    if (parmPtr->txSduConfEnable)
        pPhy->confEnableMask |= CONF_TXSDU;
    if (parmPtr->rxStartConfDisable == 0)
        pPhy->confEnableMask |= CONF_RXSTART;
    if (parmPtr->txStartConfDisable == 0)
        pPhy->confEnableMask |= CONF_TXSTART;

    pBsRx->numMS = 1;

    // The default is zero channels as this info comes from the RxVector
    pBsRx->numPUCCHSymbols = 0;
    pBsRx->Nul_rb = pBs->Nrb;
    pBsRx->Nul_symb = pBsTx->Ndl_symb;

    // Need to figure out a better place to initialize
    //   pBsRx->n1_DMRS=0;
    //   pBsRx->groupHoppingEnabled=0;
    pBsRx->Ndata_rb = CA_NumULDataCarriersPerResourceBlock[parmPtr->cyclicPrefixType];
    pBsRx->slotNumber = 2 * pBsRx->subframeNumber;

#ifndef _WIN32
    memset (pBsRx->adcbuf, 0, sizeof (pBsRx->adcbuf));
#endif

    for(i=0; i<2; i++)
    {
        pBsRxPrach[i]->bs_prach_enable = 1;
        if (pBsRxPrach[i]->bs_prach_enable == 1)
        {
            pBsRxPrachDet[i]->bs_prachDet_firLpf_length = 8;
            pBsRxPrachDet[i]->bs_prachDet_downsampleRate = 3;
            pBsRxPrachDet[i]->bs_prachDet_lpfDelay = 5;

            pBsRxPrach[i]->bs_prach_configurationIndex = parmPtr->prachConfigurationIndex;
            pBsRxPrach[i]->bs_prach_subcSpacing
                = LteDeterminePrachSubcSpacing(parmPtr->prachConfigurationIndex);
            pBsRxPrach[i]->bs_prach_frequencyOffset = parmPtr->prachFrequencyOffset;
            pBsRxPrach[i]->bs_prach_highSpeedFlag = parmPtr->prachHighSpeedFlag;
            pBsRxPrach[i]->bs_prach_cyclicShiftConfig = parmPtr->prachCyclicShiftConfig;
            pBsRxPrach[i]->bs_prach_rootSequenceIndex = parmPtr->prachRootSequenceIndex;
        }
    }

    /* Generate all the BCH data for a given MIB */
    /* Need to call this again if MIB is reconfigured */
    if ( contextNum == 0 )
    {
        /* setup the DL parameter information */
        /* set the parameters */
        pBsDlInitParam = pBsTx->pBsDlInitParam;
        pBsDlInitParam->dlBandwidth = parmPtr->channelBandwidth;
        pBsDlInitParam->cellId = pBs->Ncell_id;
        pBsDlInitParam->cyclicPrefixType = parmPtr->cyclicPrefixType;
        pBsDlInitParam->numBsTxAntPort = parmPtr->txAntennaPortCount;
        pBsDlInitParam->phichDuration = parmPtr->phichDuration;
        pBsDlInitParam->phichResource = parmPtr->phichResource;
        pBsDlInitParam->numDlRb = pBsTx->Ndl_rb;
    }

    LteBsPhyInitCommonMemBuffers(pPhy, contextNum, pPhy0);

    pBsSrs = pPhy->pBs->pBsSrs;

    pBsSrs->srsEnable = ((parmPtr->srsSubframeConfig != 15) ? 1 : 0);
    if (pBsSrs->srsEnable == 1)
    {
        pBsSrs->srsBandwidthConfig = parmPtr->srsBandwidthConfig; //2; //1 (option8);    //C_srs, Tables 5.5.3.2-x [3GPP TS 36.211, Sec.5.5.3]
        pBsSrs->srsSubframeConfig = parmPtr->srsSubframeConfig; //7;     //Table 5.5.3.3-1 [3GPP TS 36.211, Sec.5.5.3]
        pBsSrs->srsSimultaneousANandSRS = parmPtr->srsSimultaneousANandSRS; //0;  //[3GPP TS 36.213, Sec.8.2]
    }

    return status;
}

//-------------------------------------------------------------------------------------------
/** @brief Initialize HARQ structures used by PHY code
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   pPhy      Pointer to the current active PHY structure
 *  @param   contextNum  Structure number for the N/N+1 Real Time scheme
 *  @param   pPhy0       Pointer to the first active PHY structure (subframe N)
 *
 *  @return  status    A value of zero indicate success a value of 1 indicates
 *                     failure (We can expand to indicate the specific failure
 *                     reason)
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -----------------------------------------------------------------------------------------------------
 *  |Name          |Name in   |Direction      |Length  |Type          |Data Format |Valid Range |Units  |
 *  |              |Standard  |               |        |              |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |pPhy          |          |pInput/pOutput |        |structure     |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |contextNum    |          |input          |1       |unsigned int  |(0:31:0)    |[0:1]       |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |pPhy0         |          |pInput         |        |structure     |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |status        |          |Output         |        |unsigned int  |(0:31:0)    |[0:1]       |       |
 *  -----------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -------------------------------------------
 *  |Block Name          |LteBsPhyInitUlHarq  |
 *  -------------------------------------------
 *  |Input Stream        |pPhy, pPhy0, contextNum                |
 *  -------------------------------------------
 *  |Output Stream       |pPhy                |
 *  -------------------------------------------
 *  |Config Parameters   |none                |
 *  -------------------------------------------
 *  |Status Parameters   |status              |
 *  -------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Initialize HARQ structures used by PHY code. Allocates memory to buffers used by HARQ processes
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *  none
 *
 *
 *  \ingroup group_lte_api
 *
 **/
//-------------------------------------------------------------------------------------------
static U32 LteBsPhyInitUlHarq(PLTEPHYDESC pPhy, U32 contextNum, PLTEPHYDESC pPhy0)
{
    U32 status = 0;
    PHARQS pharq;
    PHARQ_DESCRIPTOR_T pHarqDescriptor;
    U32 i, j;

    //*****************************************************************************
    //**********                        UPLINK HARQ                      **********
    //*****************************************************************************
#ifndef _WIN32
    if (pPhy->pharq)
    {
        MemFree(pPhy->pharq, DDR1_NCNB_MEM);
        pPhy->pharq = NULL;
    }
#endif

    // Allocate only 1 instance of this
    if (contextNum == 0)
    {
        // allocate memory
        pHarqDescriptor = MemAlloc( sizeof(HARQ_DESCRIPTOR_STRUCT), DDR1_NCNB_MEM, 1 );
        _ASSERT_PTR(pHarqDescriptor);

        // Allocate temporary space for uplink HARQ structure
        pPhy->pharq = MemAlloc(sizeof(HARQS), DDR1_NCNB_MEM, 1);
        _ASSERT_PTR(pPhy->pharq);
        // set pointer to harq descriptor
        pPhy->pharq->pHarqDescriptor = pHarqDescriptor;

#ifndef _WIN32
        _ASSERT_PTR(pPhy->pharq);
#endif

        pharq = pPhy->pharq;

        // Initialize HARQ process.
        pharq->harq_prc = P_HARQ_PROC - 1;

        // Initialize HARQ group indices.
        for (i = 0; i < P_HARQ_PROC; i++)
        {
            pharq->harq_grp[i] = P_HARQ_COMB - 1;
        }

        // Initialize HARQ iterations number.
        for (i = 0; i < P_HARQ_PROC; i++)
        {
            for (j = 0; j < P_HARQ_USERS; j++)
            {
                pharq->harq_iter[i][j] = 0;
            }
        }

        // Allocate and Initialize space for uplink Rnti Mapping structure
        pPhy->pRntiMapping = MemAlloc(sizeof(BS_RX_RNTI_MAPPING), DDR1_NCNB_MEM, 1);
        _ASSERT_PTR(pPhy->pRntiMapping);
        for(i = 0; i < NUM_UNIQUE_RNTI; i++)
        {
            pPhy->pRntiMapping->rnti_map[i] = RNTI_MAP_NOT_USED;
        }
        for(i = 0; i < NUM_CONNECTED_USERS; i++)
        {
            pPhy->pRntiMapping->used_index[i].index = RNTI_VARS_NOT_USED;
            pPhy->pRntiMapping->used_index[i].last_frame_used = 0;
            pPhy->pRntiMapping->used_index[i].RNTI = 0;
        }
#ifdef _WIN32
        //Ceva Sim needs these to have valid address
        for(i = 0; i < MAXCHSUBFRAME; i++)
        {
            pPhy->pBsRx->pUlRxInputParams->usedIndexVars[i] = &pPhy->pRntiMapping->used_index[i];
        }
#endif
    }
    else
    {
        pPhy->pharq = pPhy0->pharq;
        pPhy->pRntiMapping = pPhy0->pRntiMapping;

#ifdef _WIN32
        //Ceva Sim needs these to have valid address
        for(i = 0; i < MAXCHSUBFRAME; i++)
        {
            pPhy->pBsRx->pUlRxInputParams->usedIndexVars[i] = &pPhy->pRntiMapping->used_index[i];
        }
#endif
    }

    return status;
}

//-------------------------------------------------------------------------------------------
/** @brief Destroys all Uplink HARQ Buffers created in Phy Init
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   pPhy        Pointer to the current active PHY structure
 *  @param   contextNum  Structure number for the N/N+1 Real Time scheme
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -----------------------------------------------------------------------------------------------------
 *  |Name          |Name in   |Direction      |Length  |Type          |Data Format |Valid Range |Units  |
 *  |              |Standard  |               |        |              |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |pPhy          |          |pInput/pOutput |        |structure     |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |contextNum    |          |input          |1       |unsigned int  |(0:31:0)    |[0:1]       |       |
 *  -----------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |LteBsPhyDestroyUlHarq            |
 *  --------------------------------------------------------
 *  |Input Stream        |pPhy                             |
 *  --------------------------------------------------------
 *  |Output Stream       |                                 |
 *  --------------------------------------------------------
 *  |Config Parameters   |contextNum                       |
 *  --------------------------------------------------------
 *  |Status Parameters   |none                             |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Destroys all Uplink HARQ Buffers created during Phy Init
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *  none
 *
 *
 *  \ingroup group_lte_api
 *
 **/
//-------------------------------------------------------------------------------------------
static U32 LteBsPhyDestroyUlHarq(PLTEPHYDESC pPhy, U32 contextNum)
{
    U32 status = 0;

    if (contextNum == 0)
    {
        if (pPhy->pharq->pHarqDescriptor)
        {
            MemFree(pPhy->pharq->pHarqDescriptor, DDR1_NCNB_MEM);
            pPhy->pharq->pHarqDescriptor = NULL;
        }
        if (pPhy->pharq)
        {
            MemFree(pPhy->pharq, DDR1_NCNB_MEM);
            pPhy->pharq = NULL;
        }
        if (pPhy->pRntiMapping)
        {
            MemFree(pPhy->pRntiMapping, DDR1_NCNB_MEM);
            pPhy->pRntiMapping = NULL;
        }
    }
    else
    {
        pPhy->pharq = NULL;
        pPhy->pRntiMapping = NULL;
    }

    return status;
}

//-------------------------------------------------------------------------------------------
/** @brief Print the amount of free memory on the device
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   ptr           String to print
 *  @param   printAntInfo  Flag to print Antenna information
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -----------------------------------------------------------------------------------------------------
 *  |Name          |Name in   |Direction      |Length  |Type          |Data Format |Valid Range |Units  |
 *  |              |Standard  |               |        |              |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |ptr           |          |pInput         |        |char          |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |printAntInfo  |          |input          |1       |unsigned int  |(0:31:0)    |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |LteBsPhyDisplayFreeSpace         |
 *  --------------------------------------------------------
 *  |Input Stream        |ptr                              |
 *  --------------------------------------------------------
 *  |Output Stream       |                                 |
 *  --------------------------------------------------------
 *  |Config Parameters   |printAntInfo                     |
 *  --------------------------------------------------------
 *  |Status Parameters   |none                             |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Print the amount of free memory on the device
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *  none
 *
 *
 *  \ingroup group_lte_api
 *
 **/
//-------------------------------------------------------------------------------------------
void LteBsPhyDisplayFreeSpace(char *ptr, int printAntInfo)
{
#ifndef _WIN32

    UINT32 nHeapNum, i, nSize, nOrgSize, id;
    UINT64 Val;
    HANDLE h;
    char * pName;
    HEAPINFO info;

    nHeapNum = MxGetObjCount (OBJ_TYPE_HEAP);

    phy_printf("\r\n--------------------------------------------------------------------------------------------------\r\n");
    phy_printf(  "%s:\r\n", ptr);
    phy_printf(  "------------------------------------\r\n");
    if (printAntInfo)
    {
        phy_printf(  "Bandwidth:                                                     %9d\r\n", getPhyCtxByNum(0, 0)->pBs->Nfft);
        phy_printf(  "Number of Tx Antennas:                                         %9d\r\n", getPhyCtxByNum(0, 0)->pBsTx->Ntx_antennas);
        phy_printf(  "Number of Rx Antennas:                                         %9d\r\n", getPhyCtxByNum(0, 0)->pBsRx->Nrx_antennas);
    }

    for (i = 0; i < nHeapNum; i++)
    {
        h = MxGetObjByIndex (OBJ_TYPE_HEAP, i);

        if (HANDLE_TOINT(h) == HANDLE_TOINT(AppHeap))
        {
            id = ID_HEAP_APP;
            pName = "AppHeap     ";
        }
        else if (HANDLE_TOINT(h) == HANDLE_TOINT(DspHeap))
        {
            id = ID_HEAP_DSP;
            pName = "DspHeap     ";
        }
        else if (HANDLE_TOINT(h) == HANDLE_TOINT(NcnbHeap))
        {
            id = ID_HEAP_NCNB;
            pName = "NcnbHeap    ";
        }
        else if (HANDLE_TOINT(h) == HANDLE_TOINT(IramHeap))
        {
            id = ID_HEAP_IRAM;
            pName = "IramHeap    ";
        }
        else if (HANDLE_TOINT(h) == HANDLE_TOINT(DDR1Heap))
        {
            id = ID_HEAP_DDR1;
            pName = "DDR1Heap    ";
        }
        else if (HANDLE_TOINT(h) == HANDLE_TOINT(DDR1NcnbHeap))
        {
            id = ID_HEAP_DDR1_NCNB;
            pName = "DDR1NcnbHeap";
        }
        else if (HANDLE_TOINT(h) == HANDLE_TOINT(PRV_CORE_HEAP))
        {
            id = ID_HEAP_PRV_CORE0;
            pName = "PrvCore0Heap";
        }
        else
        {
            id = ~0;
            pName = "DEFCORE-HEAP";
        }

        nOrgSize = MxHeapGetTotalSize(h);
        nSize    = MxHeapGetFreeSpace (h);

        GetHeapInfo (id, &info);

        Val = nSize;
        Val *= 100;
        Val /= nOrgSize;

        phy_printf (" - %s [h=0x%08x]{0x%08x..0x%08x}, size = %9d / %9d (%4d%% free )\r\n",
            pName,
            HANDLE_TOINT(h),
            info.BaseAddr,
            info.BaseAddr + info.Size,
            nSize,
            nOrgSize,
            (UINT32)Val);
    }
    phy_printf(  "--------------------------------------------------------------------------------------------------\r\n\r\n");
#endif
    return;
}


//-------------------------------------------------------------------------------------------
/** @brief Initializes the Physical Layer of an LTE BS
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param phyInstance Defines the particular PHY that is going to perform the
 *                     processing of the messages and input samples
 *  @param parmPtr     Pointer to a INITPARM structure that contains the
 *                     information required to initialize the Phy
 *
 *  @return status     A value of zero indicate success a value of 1 indicates
 *                     failure (We can expand to indicate the specific failure
 *                     reason)
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -----------------------------------------------------------------------------------------------------
 *  |Name          |Name in   |Direction      |Length  |Type          |Data Format |Valid Range |Units  |
 *  |              |Standard  |               |        |              |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |pPhy          |          |pInput/pOutput |        |structure     |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |parmPtr       |          |pInput         |        |structure     |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |status        |          |Output         |        |unsigned int  |(0:31:0)    |[0:1]       |       |
 *  -----------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |LteBsPhyInit                |
 *  ---------------------------------------------------
 *  |Inputs Stream       |pPhy, parmPtr               |
 *  ---------------------------------------------------
 *  |Output Stream       |pPhy                        |
 *  ---------------------------------------------------
 *  |Config Parameters   |none                        |
 *  ---------------------------------------------------
 *  |Status Parameters   |status                      |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 *  This function performs the initialization of an LTE BS PHY according to the
 *  information contained in the INITPARM structure that the second parameter
 *  indicates and for the phyInstance stated in the first parameter.
 *  Once the initialization completes a PHY_INIT.indication message is issued.
 *  This message needs to be embedded into the Mindspeed generic message
 *  format described in MSA-81479 using MessageType 0x0005 (Indication Message)
 *  and MessageID 0x7000 Foreign API Message
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * none
 *
 *  \ingroup group_lte_api
 *
 **/
//-------------------------------------------------------------------------------------------
U32 LteBsPhyInit(U32 phyInstance, PINITPARM parmPtr)
{
    U32 i;
    U32 status = 0;
#ifndef _WIN32
    PLTE_PHY_SDU_DESC pSduDesc;
#endif

    LteBsPhyDisplayFreeSpace("Memory Status before LteBsPhyInit\0", 0);

#ifndef _WIN32
    // Driver Init
    {
        MXRC rc = MX_OK;
        PPHYINITCTX pPhyInitCtx;

        pPhyInitCtx = &phyInitCtx;

        memset(pPhyInitCtx, 0, sizeof(phyInitCtx));

        pPhyInitCtx->FecConfigRegs.wimax_lte = FEC_HWCONF_LTE;
        pPhyInitCtx->FecConfigRegs.bp_rate_match = 0;
        pPhyInitCtx->FecConfigRegs.bp_channel_interleaver = 0;
        pPhyInitCtx->FecConfigRegs.ci_symbol_number = 0; // 12

        if (MxGetARMID() == 0)
        { /* Quad core */

            pPhyInitCtx->cfg.CpuDrivers [0] =
                SYSCORE_FEC_DL_DRV      |
                SYSCORE_FEC_UL_DRV      |
                SYSCORE_FEC_ROBUST_CTRL |
                SYSCORE_SYS_MDMA_DRV    |
                SYSCORE_SPU_MDMA_DRV    |
                SYSCORE_RAD_MDMA_DRV;


            #if 1
            pPhyInitCtx->cfg.CpuDrivers [0] |=
                SYSCORE_SYS_MDMA_ROBUST_CTRL |
                SYSCORE_SPU_MDMA_ROBUST_CTRL |
                SYSCORE_RAD_MDMA_ROBUST_CTRL
            ;
            #endif

            pPhyInitCtx->cfg.CpuDrivers [0] |=
                SYSCORE_CEVA_DRV        |
                SYSCORE_FP_DRV          |
                SYSCORE_SCHEDULER;
        }
        else
        { /* Dual core */
            pPhyInitCtx->cfg.CpuDrivers [0] =
                SYSCORE_FEC_DL_DRV      |
                SYSCORE_FEC_UL_DRV      |
                SYSCORE_FEC_ROBUST_CTRL |
                SYSCORE_SYS_MDMA_DRV    |
                SYSCORE_SPU_MDMA_DRV    |
                SYSCORE_RAD_MDMA_DRV;

            pPhyInitCtx->cfg.CpuDrivers [0] |=
                SYSCORE_CEVA_DRV        |
                SYSCORE_FP_DRV          |
                SYSCORE_SCHEDULER;
        }

        pPhyInitCtx->cfg.PtrFecCfg = &pPhyInitCtx->FecConfigRegs;
        pPhyInitCtx->cfg.FpMode = CEXPFFT[parmPtr->channelBandwidth];
        pPhyInitCtx->cfg.PtrSchedCfg = &gInitScheduler;

        pPhyInitCtx->cfg.RexCfg |= REX_CFG_IDFT_ON_CEVA;

        if (FAILED(rc = SysCoreConfig (&pPhyInitCtx->cfg)))
        {
            _ASSERT_RC(rc);
            uart_printf("Syscore config error [%d]\n", rc);
            return rc;
        }
    }
#endif // _WIN32

    //LteBsPhyDisplayFreeSpace("Memory Status after Driver Init\0", 0);

    MLogSetMask(0x0);
    for (i = 0; i < MAX_NUM_OF_SF_CTX; i++)
    {
        LteBsPhySetPointers(i);
        LteBsPhyInitContext(getPhyCtxByNum(phyInstance, i), parmPtr, i,
                            getPhyCtxByNum(phyInstance, 0));
        LteBsPhyInitUlHarq(getPhyCtxByNum(phyInstance, i), i, getPhyCtxByNum(phyInstance, 0));
    }
    MLogSetMask(0xFFFFFFFF);

    for (i = 0; i < MAX_NUM_OF_SF_CTX; i++)
    {
        CreateDlTxTaskList(getPhyCtxByNum(phyInstance, i));
        CreateUlRxTaskList(getPhyCtxByNum(phyInstance, i));
    }

#ifndef _WIN32
    for (i = 0; i < MAX_NUM_OF_SF_CTX; i++)
    {
        pSduDesc = gpSduDesc[i];
        memcpy(pSduDesc->modTask, getPhyCtxByNum(phyInstance, i)->pBsTx->pModTCB, 7 * sizeof(PTCB));
        pSduDesc->pSetupBufsTCB = getPhyCtxByNum(phyInstance, i)->pBsTx->pSetupBufsTCB;
        pSduDesc->pIsExeTaskListReady = &getPhyCtxByNum(phyInstance, i)->pBsTx->isExeTaskListReady;
        gpSduDesc[i]->pDlCntrl_Pars_List  = gpSduDesc[1-i]->pDlCntrl_Pars;
    }

    PhyDiInit(getPhyCtxByNum(phyInstance, 0));
    LtePhyInitThreadPrach();
    LtePhyInitThreadSrs();
    LtePhyInitApiThread();
    lte_bs_phy_nmm_thread_init();

    LteBsInitPhyStats();
    LtePhyApiErrorCheckInitEmptyChannels(getPhyCtxByNum(phyInstance, 0));

    MLogSetMaskProg(0x0);
    MLogActivateMaskProg();

    SvsrLoggerSetTTICycleLimit(200);   // 1000 Ticks
//    SvsrLoggerSetMask(0);

    // Calculate Cache stats and Log it in
#ifdef PM_ENABLED
    for (i = 0; i < MLOG_BUFFERS_COUNT; i++)
    {
        PMStartCalcDataCacheMissEx(i);
        PMStartCalcDataCacheHitEx(i);
        PMStartCalcCycleEx(i);
    }
#endif

#endif  // _WIN32

    LteBsPhyDisplayFreeSpace("Memory Status after PhyInit\0", 1);
    uart_printf("PHY INIT Complete. Running on LARM 0 (3x1 mode)\r\n");


    //LteBsPhyDestroy(phyInstance);

    return (status);
}

//-------------------------------------------------------------------------------------------
/** @brief Destroys the Physical Layer of an LTE BS
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param phyInstance Defines the particular PHY that is going to perform the
 *                     processing of the messages and input samples
 *  @return status     A value of zero indicate success a value of 1 indicates
 *                     failure (We can expand to indicate the specific failure
 *                     reason)
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -----------------------------------------------------------------------------------------------------
 *  |Name          |Name in   |Direction      |Length  |Type          |Data Format |Valid Range |Units  |
 *  |              |Standard  |               |        |              |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |pPhy          |          |pInput/pOutput |        |structure     |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |status        |          |Output         |        |unsigned int  |(0:31:0)    |[0:1]       |       |
 *  -----------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |LteBsPhyDestroy                |
 *  ---------------------------------------------------
 *  |Inputs Stream       |phyInstance               |
 *  ---------------------------------------------------
 *  |Output Stream       |none                        |
 *  ---------------------------------------------------
 *  |Config Parameters   |none                        |
 *  ---------------------------------------------------
 *  |Status Parameters   |status                      |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 *  This function destroys an isntance of the LTE BS PHY that was previous initialized
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * none
 *
 *  \ingroup group_lte_api
 *
 **/
//-------------------------------------------------------------------------------------------
U32 LteBsPhyDestroy(U32 phyInstance)
{
    U32 i;
    U32 rc = 0;

    //LteBsPhyDisplayFreeSpace("Memory Status before PhyShutdown\0", 0);

    for (i = 0; i < MAX_NUM_OF_SF_CTX; i++)
    {
        LteBsPhyDestroyUlHarq(getPhyCtxByNum(phyInstance, i), i);
        LteBsPhyDestroyCommonMemBuffers(getPhyCtxByNum(phyInstance, i), i);
    }

#ifndef _WIN32
    PhyDiDestroy();
    //LtePhyDestroyApiThread();
    LteBsDestroyPhyStats();
    LtePhyApiErrorCheckDestroyEmptyChannels();
    lte_bs_phy_nmm_destroy_buffers();
    lte_bs_phy_nmm_thread_kill();

    if (PhyDiIsStopPrintf() == 0)
    {
        PPHYINITCTX pPhyInitCtx;
        pPhyInitCtx = &phyInitCtx;

        if (FAILED(rc = SysCoreShutdown (&pPhyInitCtx->cfg)))
        {
            _ASSERT_RC(rc);
            uart_printf("Syscore config error [%d]\n", rc);
            return rc;
        }
    }

    // Needed because next message will come normally to PHY for PHYINIT
    gFirstMessageArrived = 0;
#endif

    LteBsPhyDisplayFreeSpace("Memory Status after PhyShutdown\0", 0);
    uart_printf("PHY Destroy Complete.\r\n");

    return rc;

}


// Clean up function when PhyDi is called
#if defined (RT_API_PROC)
//-------------------------------------------------------------------------------------------
/** @brief Destroys the Physical Layer of an LTE BS
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -----------------------------------------------------------------------------------------------------
 *  |Name          |Name in   |Direction      |Length  |Type          |Data Format |Valid Range |Units  |
 *  |              |Standard  |               |        |              |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |LteBsPhyStop                |
 *  ---------------------------------------------------
 *  |Inputs Stream       |none                        |
 *  ---------------------------------------------------
 *  |Output Stream       |none                        |
 *  ---------------------------------------------------
 *  |Config Parameters   |none                        |
 *  ---------------------------------------------------
 *  |Status Parameters   |                            |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 *  Function called when Phy Stop command is invoked by the MAC.  Will clean
 *  up memory and prepare PHY to be restarted.
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * none
 *
 *  \ingroup group_lte_api
 *
 **/
//-------------------------------------------------------------------------------------------
U32 LteBsPhyStop(void)
{
    PLTEPHYDESC pPhy;
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();
    U32 i, j;

    // clean up for next start
    gStartSubfrTimeMark = 0;
    gFirstMessageArrived = 0;

    pPhy = getPhyCtxListProc(0);
    MLogMarkAndRegisterFrameSubframe(MLOG_MARK_FRAMEBORDER, gStartSubfrTimeMark, pPhy->txFrameNumber, pPhy->pBsTx->subframeNumber);
    RexNewTTINotification();

    for (i = 0; i < 2; i++)
    {
        if (i == 0)
            pPhy = getPhyCtxListProc(0);
        else
            pPhy = getPhyCtxApiProc(0);

        pPhy->pBsRx->isExeTaskListReady         = 0;
        pPhy->pBsRx->isExePrachTaskListReady[0] = 0;
        pPhy->pBsRx->isExePrachTaskListReady[1] = 0;
        pPhy->pBsRx->isExeSrsTaskListReady      = 0;
        pPhy->pBsRx->numSrsChan                 = 0;
        pPhy->pBsRx->RxPrach_Context            = 0;

        ReinitTaskList(pPhy->pBsRx->pExeTaskList[0], NULL);
        ReinitTaskList(pPhy->pBsRx->pExePrachTaskList[0], NULL);
        ReinitTaskList(pPhy->pBsRx->pExePrachTaskList[1], NULL);
        ReinitTaskList(pPhy->pBsRx->pExeSrsTaskList[0], NULL);

        pPhy->pBsTx->isExeTaskListReady         = 0;
        ReinitTaskList(pPhy->pBsTx->pExeTaskList, NULL);
    }

    for (i = 0; i < MAXCHSUBFRAME; i++)
    {
        for (j = 0; j < NUM_UL_SDU_CONTEXT; j++)
        {
            if (pRxSduDesc[j][i])
            {
                PLTE_PHY_UL_SDU_DESC Curr;
                Curr = (PLTE_PHY_UL_SDU_DESC) pRxSduDesc[j][i];
                if (Curr->out && (Curr->sduType == PUSCH))
                {
                    MemFree(Curr->out, IRAM_MEM);
                    Curr->out = NULL;
                }
                if (Curr->pListElement)
                {
                    if (Curr->pListElement->MessagePtr)
                    {
                        SvsrFreeIMem(Curr->pListElement->MessagePtr);
                        Curr->pListElement->MessagePtr = NULL;
                    }
                    SvsrFreeIMem(Curr->pListElement);
                    Curr->pListElement = NULL;
                }
                if (Curr->pListElementCqiRi)
                {
                    if (Curr->pListElementCqiRi->MessagePtr)
                    {
                        SvsrFreeIMem(Curr->pListElementCqiRi->MessagePtr);
                        Curr->pListElementCqiRi->MessagePtr = NULL;
                    }
                    SvsrFreeIMem(Curr->pListElementCqiRi);
                    Curr->pListElementCqiRi = NULL;
                }
                if (Curr->pListElementHi)
                {
                    if (Curr->pListElementHi->MessagePtr)
                    {
                        SvsrFreeIMem(Curr->pListElementHi->MessagePtr);
                        Curr->pListElementHi->MessagePtr = NULL;
                    }
                    SvsrFreeIMem(Curr->pListElementHi);
                    Curr->pListElementHi = NULL;
                }

                Curr->pListRxEnd = NULL;
            }
        }
    }

    for (i = 0; i < NUM_UL_SDU_CONTEXT; i++)
    {
        if (pRxSduRxEnd[i])
        {
            if (pRxSduRxEnd[i]->MessagePtr)
            {
                SvsrFreeIMem(pRxSduRxEnd[i]->MessagePtr);
                pRxSduRxEnd[i]->MessagePtr = NULL;
            }
            SvsrFreeIMem(pRxSduRxEnd[i]);
            pRxSduRxEnd[i] = NULL;
        }
    }

    for (j = 0; j < NUM_UL_SDU_CONTEXT; j++)
    {
        pRxSduFirstPusch[j] = NULL;
        pRxSduFirstPucch[j] = NULL;
        pRxSduFirstMuxCtrlAckNack[j] = NULL;
        pRxSduFirstMuxCtrlCqiRi[j] = NULL;
    }

    // Swap PHY Context
    swapPhyCtx(0);
    gCtxDlFec = 1 - gCtxDlFec;

#ifdef SLOT_BASED_FFT
    pPhy = getPhyCtxListProc(0);
    if (pMapDrvCtx->UpdateQueueFftIfft[pMapDrvCtx->QueueContext[1]][1])
        MapFftDrvRun(pPhy->pBsRx->pFftTcb[1]);
#endif

    LtePhyStartApiFree();

    return 0;
}
#endif


