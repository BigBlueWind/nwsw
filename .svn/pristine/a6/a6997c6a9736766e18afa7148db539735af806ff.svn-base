//-------------------------------------------------------------------------------------------
/** @file LteBsTx.c
 *
 * @brief This file implements the transmitter function for an LTE
 *        Base Station
 * @author Mindspeed Technologies
 * @version $Revision: 1.53 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifdef _WIN32

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#include "typedef.h"
#include "ltephydef.h"
#include "prototypes.h"
#include "ext_phy.h"
#include "basic_op_cc.h"


//-------------------------------------------------------------------------------------------
/** @brief Implements the Transmitter for an LTE Base Station
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param phyInstance Defines the particular PHY that is going to perform the
 *                     processing of the messages and input samples
 *  @param sigOut      Pointer to the Tx IQ Sample output memory
 *  @param outMsg	   Pointer to the output LTE L2-PHY API Message
 *  @param outMsgSize  Pointer to Output LTE L2-PHY message size in long words
 *
 *  @return status     A value of zero indicate success a value of 1 indicates
 *                     failure (We can expand to indicate the specific failure
 *                     reason)
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------------
 *  |Name       |Name in   |Direction      |Length  |Type                 |Data Format |Valid Range |Units  |
 *  |           |Standard  |               |        |                     |            |            |       |
 *  ---------------------------------------------------------------------------------------------------------
 *  |phyInstance|          |Input          |1       |unsigned int         |(1:31:0)    |[0:2]       |       |
 *  ---------------------------------------------------------------------------------------------------------
 *  |sigOut     |          |pInput/pOutput |1       |pComplex             |(1:1:14)    |(0,2^32)    |       |
 *  ---------------------------------------------------------------------------------------------------------
 *  |outMsg     |          |pInput/pOutput |1       |punsigned int        |(1:31:0)    |(0,2^32)    |       |
 *  ---------------------------------------------------------------------------------------------------------
 *  |outMsgSize |          |pOutput        |1       |punsigned int        |(1:31:0)    |(0,2^32)    |       |
 *  ---------------------------------------------------------------------------------------------------------
 *  |status     |          |Output         |1       |unsigned int         |(1:31:0)    |[0,1]       |       |
 *  ---------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |LteBsTx                     |
 *  ---------------------------------------------------
 *  |Input Streams       |phyInstance                 |
 *  ---------------------------------------------------
 *  |Output Streams      |outMsg,outMsgsize           |
 *  ---------------------------------------------------
 *  |Config Parameters   |phyInstance                 |
 *  ---------------------------------------------------
 *  |Status Parameters   |status                      |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * Function LteBsTx invokes the main LTE Transmitter function, generates the TXEND API
 * indication and return an execution status
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * MSA-81613
 *
 *  \ingroup group_lte_api
 *
 **/
//-------------------------------------------------------------------------------------------
U32 LteBsTx(U32 phyInstance, S16* sigOut, U32* outMsg, U32* outMsgSize)
{
    U32 status = 0;
    PBS pBs;
    PBS_TX pBsTx;
    PLTEPHYDESC pPhy = getPhyCtxApiProc(phyInstance);
    U32 i, length_adc;
    DLTXINPUTPARAMS *pDlTxInPars;

    if (!pPhy)
        return 0;

    pBsTx = pPhy->pBsTx;
    pBs = pPhy->pBs;
    if (!pBsTx)
        return 0;

    pDlTxInPars = pBsTx->pTxInputParams;

    //For C-Code and Ceva Sim, we only have 1 context so
    //gCtxDlFec should always point to first context
    gCtxDlFec = 0;

    // This function should be invoked after a semaphore gets updated from
    // an isr that corresponds to the 1 ms subframe period. Per 3GPP 36.211
    // section 8.1 the Uplink Subframe preceeds the Downlink subframe by a
    // number between 0 and 0.66 ms. So for example the Rx Samples clock could
    // be used as a reference for the timer event that leads to the Tx and Rx
    // sub-frame processing

    // This message also needed for PC-based FDX tests
    // To enable VS6 support WIN32 must be changed to _WIN32
    LteGenerateAPIMsg(phyInstance, outMsg, outMsgSize, PHY_TXSTART_IND, status);

#ifndef LTE_PHY_RT
    //For C-Code
    status = PhyBsTx(pPhy);

#else
    //For Ceva Sim
    UpdateDlControlTaskList(pPhy);

    if (gLowLatencyFlag == 0)
        pBsTx->isExeTaskListReady = TRUE;

    PhyBsTxExeTaskList(pPhy);

#endif

    //Need to make sure ptrInputPDCCH points back to the
    //beginning of the buffer.  This is done in LteBsApiThread for
    //RT project since all TxSDUs come at the same time
    pBsTx->pDlCntrl_TxInPars->ptrInputPDCCH = pBsTx->pDlCntrl_TxInPars->inputPDCCH;

    length_adc = 2 * ((pBs->Nfft + pBs->Ncp0) + 6 * (pBs->Nfft + pBs->Ncp)) * sizeof(S16);  // Ex: 15360 * 2 = 30720 samples for 10MHz

    for(i = 0; i < (U32)pBsTx->Ntx_antennas; i++)
        memcpy(&sigOut[i*length_adc], pPhy->pBsTx->ifftOut[i], length_adc*2);

    // End close the file pointer.
    return (status);
}

#endif
