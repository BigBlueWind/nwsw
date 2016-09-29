/** @file phystub.c
 *
 * @brief Dummy PHY stub
 * @author Mindspeed Technologies
 * @version $Revision: 1.7 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef STUB

#include "osal.h"
#include "phyiface.h"
#include "apidefs.h"

//
// This ID is present in all MAC types
//
const int Domain4G = PAR_LTE;
const int Mac4G = PAR_STUB;

OSAL_SEMAPHORE PhyTickSema;

PTR pIMSG_TO_PHY = NULL;
PTR pIMSG_FROM_PHY = NULL;

RESULTCODE PhyInit(void)
{
    return SUCCESS;
}

RESULTCODE PhyEnable(U16 *instID)
{
    if (instID != NULL)
    {
        // TODO: Another ID?
        *instID = 0;
    }
    return SUCCESS;
}

RESULTCODE PhyDone(void)
{
    return SUCCESS;
}

RESULTCODE PhySendMsg(HANDLE h, PTR pMsg, OSAL_SIZE msgSize)
{
    return FAILURE;
}

/*! \brief Get next message from basket
 *
 *  \param pMsgBasket [IN] Pointer to message basket (mail box) handler
 *  \param pMsg [IN/OUT] Pointer to the memory area which will receive data
 *  \param pMsgSize [OUT] Data size
 *  \return Standard result code
*/
RESULTCODE PhyGetMsg(HANDLE h, PTR pMsg, OSAL_SIZE *pMsgSize)
{
    return FAILURE;
}

RESULTCODE PhyDispatch(U32 size, PTR msg)
{
    return SUCCESS;
}

#endif /* STUB */
