/** @file lte_if.c
 *
 * @brief Implemenataion of interface to Message based PHY interface
 * @author Mindspeed Technologies
 * @version $Revision: 1.5 $
 *
 * COPYRIGHT(c) 2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef MX_ARM_ENABLED
#include "osal.h"
#include "tl.h"
#endif
#include "LtePhyL2Api.h"
#include "resultcodes.h"

#include "LteMacTestCfgMgr.h"
#include "lte_mac_state.h"
#include "lte_printf.h"

typedef U32 (*tAddToTail)(U8 MessageType, U32 MessageLen, U8* tempo);
typedef struct stMac2PhyQueue
{
    PMAC2PHY_QUEUE_EL StartQueueCurr;
    PMAC2PHY_QUEUE_EL TailQueueCurr;
    tAddToTail pAddToTail;
} MAC2PHY_QUEUE, *PMAC2PHY_QUEUE;

extern MAC2PHY_QUEUE Mac2PhyQueue;


/** Compresses TXVECTOR structure by removing gaps and adjusting offsets
 *
 * @param pDest Destination buffer
 * @param pVec Original Downlink vector
 * @return New Downlink vector size
 */
U32 lte_if_copy_tx_vector(U8* pDest, PDLSUBFRDESC pVec)
{
    U32 len = 0;
    U32 num_ctrl_chan = pVec->numCtrlChannels;
    U32 num_data_chan = pVec->numberofChannelDescriptors - num_ctrl_chan;

    if (pDest == NULL)
    {
        return 0;
    }
    
    // Copy header
    memcpy(pDest, pVec, (U32)&pVec->dlCh[0] - (U32)pVec);
    len += (U32)&pVec->dlCh - (U32)pVec;
    // Copy dlCh
    memcpy(pDest + len, pVec->dlCh, num_data_chan * sizeof(pVec->dlCh[0]));
    len += num_data_chan * sizeof(pVec->dlCh[0]);
    // Copy dciCh
    memcpy(pDest + len, pVec->dciCh,  num_ctrl_chan* sizeof(pVec->dciCh[0]));
    len +=  num_ctrl_chan * sizeof(pVec->dciCh[0]);
    // Copy dlCmnPwrCtl
    memcpy(pDest + len, &pVec->dlCmnPwrCtl, sizeof(pVec->dlCmnPwrCtl));
    len += sizeof(pVec->dlCmnPwrCtl);

    // Overwrite offsets
    {
        PDLSUBFRDESC pDestVect = (PDLSUBFRDESC)pDest;

        pDestVect->offsetDCIChannels =
            (U32)&pDestVect->dlCh + num_data_chan * sizeof(pVec->dlCh[0]) - (U32)pDestVect;
        pDestVect->offsetPowerCtrl =
            pDestVect->offsetDCIChannels + num_ctrl_chan * sizeof(pVec->dciCh[0]);
    }

    return len;
}


/** Compresses RXVECTOR structure by removing gaps and adjusting offsets
 *
 * @param pDest Destination buffer
 * @param pVec Original Uplink vector
 * @return New Uplink vector size
 */
U32 lte_if_copy_rx_vector(U8* pDest, PULSUBFRDESC pVec)
{
    U32 len = 0;
    U32 num_ctrl_chan = pVec->numberOfCtrlChannelDescriptors;
    U32 num_data_chan = pVec->numberofChannelDescriptors - num_ctrl_chan;
    U32 num_srs_chan = pVec->numberSrsinSf;

    // Copy header
    memcpy(pDest, pVec, (U32) &pVec->ulCh[0] - (U32) pVec);
    len += (U32) &pVec->ulCh[0] - (U32) pVec;
    // Copy ulCh
    memcpy(pDest + len, pVec->ulCh, num_data_chan * sizeof(pVec->ulCh[0]));
    len += num_data_chan * sizeof(pVec->ulCh[0]);
    // Copy ulCtlCh
    memcpy(pDest + len, pVec->ulCtlCh, num_ctrl_chan * sizeof(pVec->ulCtlCh[0]));
    len += num_ctrl_chan * sizeof(pVec->ulCtlCh[0]);
    // Copy srsInfo
    memcpy(pDest + len, pVec->srsInfo, num_srs_chan * sizeof(pVec->srsInfo[0]));
    len += num_srs_chan * sizeof(pVec->srsInfo[0]);
    // Copy rachCtrl
    memcpy(pDest + len, &pVec->rachCtrl, sizeof(pVec->rachCtrl));
    len += sizeof(pVec->rachCtrl);

    // Overwrite offsets
    {
        PULSUBFRDESC pDestVect = (PULSUBFRDESC) pDest;

    pDestVect->offsetULCtrlChannels = 
        (U32) &pDestVect->ulCh[0] + num_data_chan * sizeof(pVec->ulCh[0]) - (U32) pDestVect;

    pDestVect->offsetsrsInfo = 
        pDestVect->offsetULCtrlChannels + num_ctrl_chan * sizeof(pVec->ulCtlCh[0]);

    pDestVect->offsetRachCtrlStruct = 
        pDestVect->offsetsrsInfo + num_srs_chan * sizeof(pVec->srsInfo[0]);
    }

    return len;
}


/** Checks for internal message buffer corruption
 * @param  pMsg - pointer to list
 * @return Standard result code
 */
RESULTCODE LteMacCheckMsgIntegrity(void)
{
    U32 TxVectorQidx = 0, RxVectorQidx = 0, CurrMsgCount = 0;
    U32 TxHiADciUlQidx = 0, TxHiSduQidx = 0, TxDciUlSduQidx = 0;

    PMAC2PHY_QUEUE_EL pMsg = Mac2PhyQueue.StartQueueCurr;
    PMAC2PHY_QUEUE_EL CurrTxVector = NULL;
    PMAC2PHY_QUEUE_EL CurrRxVector = NULL;
    PMAC2PHY_QUEUE_EL CurrTxHiADciUl = NULL, CurrTxHiSdu = NULL, CurrTxDciUlSdu = NULL;


    while (pMsg)
    {
        if (pMsg->MessageType == PHY_TXSTART_REQ)
        {
            CurrTxVector = pMsg;
            TxVectorQidx = CurrMsgCount;
        }
        else if (pMsg->MessageType == PHY_RXSTART_REQ)
        {
            CurrRxVector = pMsg;
            RxVectorQidx = CurrMsgCount;
        }
        else if (pMsg->MessageType == PHY_TXHIADCIUL_REQ)
        {
            CurrTxHiADciUl = pMsg;
            TxHiADciUlQidx++;
        }
        else if (pMsg->MessageType == PHY_TXHISDU_REQ)
        {
            CurrTxHiSdu = pMsg;
            TxHiSduQidx++;
        }
        else if (pMsg->MessageType == PHY_TXDCIULSDU_REQ)
        {
            CurrTxDciUlSdu = pMsg;
            TxDciUlSduQidx++;
        }

        CurrMsgCount++;
        pMsg = pMsg->Next;
    }


    if (TxHiADciUlQidx || TxHiSduQidx || TxDciUlSduQidx)
    {
// vector contains hi-sdu
        if (TxHiADciUlQidx!=1)
        {
            printf("%s:[err]: wrong number of TxHiADciUl: %d at sfn:fn [%d:%d] (0x%X)\n", __FUNCTION__, TxHiADciUlQidx, getFrameNum(), getSubFrameNum(), (U32)CurrTxHiADciUl);
        }
        else
        {
            if ((TxHiSduQidx+TxDciUlSduQidx) == 0)
            {
                printf("%s:[err]: wrong number of TxHiSduQidx+TxDciUlSduQidx: %d at sfn:fn [%d:%d] (0x%X, 0x%X)\n", __FUNCTION__, TxHiSduQidx+TxDciUlSduQidx, getFrameNum(), getSubFrameNum(), (U32)CurrTxHiSdu, (U32)CurrTxDciUlSdu);
            }
        }
    }

    if (CurrMsgCount)
    {
        if ((CurrTxVector == NULL && CurrRxVector != NULL) || (CurrRxVector == NULL && CurrTxVector != NULL))
        {
            printf("WARNING: L2: Tx/Rx vectors corrupted: 0x%X (%d), 0x%X (%d)\r\n", (U32)CurrTxVector, TxVectorQidx,
                    (U32)CurrRxVector, RxVectorQidx);
            return FAILURE;
        }
    }
    return SUCCESS;
}


