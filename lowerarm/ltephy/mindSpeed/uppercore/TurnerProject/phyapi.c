/** @file phyapi.c
 *
 * @brief Intel PHY SAP API
 * @author Mindspeed Technologies
 * @version $Revision: 1.7 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#if defined (BSMAC) || defined (MSMAC)

#include "phyapi.h"
#include "ctrlmsg.h"
#include "apidefs.h"

/** Parse messages received from PHY
 *
 * @param data
 * @return Standard result code
 */
RESULTCODE PhySapDecode(PTR data)
{
    PHY_TXSTART_REQUEST *pHdr = (PHY_TXSTART_REQUEST *) data;

    // TODO: Add segmentation and entity ID

    // All structures start from same sequence
    // so we can pick any and cast
    switch (pHdr->messageType)
    {
        case PHY_TXSTART_REQ:
            return PhyTxStart((PHY_TXSTART_REQUEST *) data);

        case PHY_TXSTART_ACK:
            return PhyTxStartAck((PHY_TXSTART_CONFIRMATION*) data);

        case PHY_TXSTART_IND:
            return PhyTxStartInd((PHY_TXSTART_INDICATION*) data);

        case PHY_TXSDU_REQ:
            return PhyTxSdu((PHY_TXSDU_REQUEST *) data);

        case PHY_TXSDU_ACK:
            return PhyTxSduAck((PHY_TXSDU_CONFIRMATION*) data);

        case PHY_TXEND_IND:
            return PhyTxEndInd((PHY_TXEND_INDICATION*) data);

        case PHY_RXSTART_REQ:
            return PhyRxStart((PHY_RXSTART_REQUEST *) data);

        case PHY_RXSTART_ACK:
            return PhyRxStartAck((PHY_RXSTART_CONFIRMATION*)data);

        case PHY_RXSTART_IND:
            return PhyRxStartInd((PHY_RXSTART_INDICATION*)data);

        case PHY_RXSDU_IND:
            return PhyRxSduInd((PHY_RXSDU_INDICATION*)data);

        case PHY_RXEND_IND:
            return PhyRxEndInd((PHY_RXEND_INDICATION*)data);

        case PHY_RESERVED1:
        case PHY_RESERVED2:
        case PHY_RESERVED3:
            break;

        case PHY_RXCDMA_IND:
            return PhyRxCdmaInd((PHY_RXCDMA_INDICATION*)data);
    }
    return FAILURE;             // Not processed
}

/** PHY_TXSTART.request primitive
 *
 * @param data
 * @return Standard result code
 */
RESULTCODE PhyTxStart(PHY_TXSTART_REQUEST *data)
{
    return SUCCESS;
}

/** PHY_TXSTART.confirmation primitive
 *
 * @param data
 * @return Standard result code
 */
RESULTCODE PhyTxStartAck(PHY_TXSTART_CONFIRMATION *data)
{
    return SUCCESS;
}

/** PHY_TXSTART.indication primitive
 *
 * @param data
 * @return Standard result code
 */
RESULTCODE PhyTxStartInd(PHY_TXSTART_INDICATION *data)
{
    if (data->status == ERR_SUCCESS)
    {
        // TODO: Decode
        return SUCCESS;
    }
    return data->status;
}

/** PHY_TXSDU.request primitive
 *
 * @param data
 * @return Standard result code
 */
RESULTCODE PhyTxSdu(PHY_TXSDU_REQUEST *data)
{
    return SUCCESS;
}

/** PHY_TXSDU.confirmation primitive
 *
 * @param data
 * @return Standard result code
 */
RESULTCODE PhyTxSduAck(PHY_TXSDU_CONFIRMATION *data)
{
    return SUCCESS;
}

/** PHY_TXEND.indication primitive
 *
 * @param data
 * @return Standard result code
 */
RESULTCODE PhyTxEndInd(PHY_TXEND_INDICATION *data)
{
    if (data->status == ERR_SUCCESS)
    {
        // TODO: Decode
        return SUCCESS;
    }
    return data->status;
}

/** PHY_RXSTART.request primitive
 *
 * @param data
 * @return Standard result code
 */
RESULTCODE PhyRxStart(PHY_RXSTART_REQUEST *data)
{
    return SUCCESS;
}

/** PHY_RXSTART.confirmation primitive
 *
 * @param data
 * @return Standard result code
 */
RESULTCODE PhyRxStartAck(PHY_RXSTART_CONFIRMATION *data)
{
    return SUCCESS;
}

/** PHY_RXSTART.indication primitive
 *
 * @param data
 * @return Standard result code
 */
RESULTCODE PhyRxStartInd(PHY_RXSTART_INDICATION *data)
{
    if (data->status == ERR_SUCCESS)
    {
        // TODO: Decode
        return SUCCESS;
    }
    return data->status;
}

/** PHY_RXSDU.indication primitive
 *
 * @param data
 * @return Standard result code
 */
RESULTCODE PhyRxSduInd(PHY_RXSDU_INDICATION *data)
{
    PHY_RXSDU_INDICATION *dataPtr = data + 1;

    switch (data->indicationType)
    {
        case IND_DATABURST:
            return PhySdu(data, dataPtr);

        case IND_HARQACK:
            return PhyHarqAck(data, (HARQ_ACK_DATA*)dataPtr);

        case IND_FASTFEEDBACK:
            return PhyFastFeedback(data, (FAST_FEEDBACK_DATA*)dataPtr);

        case IND_HARQDATABURST:
            return 0;
        case IND_MIMODATABURST:
            return 0;
        case IND_MIMOHARQBURST:
            return 0;
    }
    return FAILURE;             // Reserved value used
}

/** PHY_RXEND.indication primitive
 *
 * @param data
 * @return Standard result code
 */
RESULTCODE PhyRxEndInd(PHY_RXEND_INDICATION *data)
{
    if (data->status == ERR_SUCCESS)
    {
        // TODO: Decode
        return SUCCESS;
    }
    return data->status;
}

/**
 *
 * @param data
 * @param dataPtr
 * @return Standard result code
 */
RESULTCODE PhyHarqAck(PHY_RXSDU_INDICATION *data, HARQ_ACK_DATA *dataPtr)
{
    return SUCCESS;
}

/**
 *
 * @param data
 * @param dataPtr
 * @return Standard result code
 */
RESULTCODE PhyFastFeedback(PHY_RXSDU_INDICATION *data, FAST_FEEDBACK_DATA *dataPtr)
{
    return SUCCESS;
}

/** PHY_RXCDMA.indication primitive
 *
 * @param data
 * @return Standard result code
 */
RESULTCODE PhyRxCdmaInd(PHY_RXCDMA_INDICATION *data)
{
    return SUCCESS;
}

/**
 *
 * @param data
 * @param dataPtr
 * @return Standard result code
 */
RESULTCODE PhySdu(PHY_RXSDU_INDICATION *data, PHY_RXSDU_INDICATION *dataPtr)
{
    return SUCCESS;
}

/** For debugging purposes or example
 *
 * @param size Size of message
 * @param data Pinter to the message
 * @return Standard result code
 */
RESULTCODE LarmDispatch(U32 size, PTR data)
{
    PMSGHEADER pHdr = (PMSGHEADER) data;
    RESULTCODE res = SUCCESS;

    if (pHdr->control & CTRL_BYPASS)
    {
        res = PhySapDecode(&pHdr->param);
    }
    return res;
}

#endif /* BSMAC || MSMAC */
