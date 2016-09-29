/** @file lte_if.h
 *
 * @brief Defenitions and prototypes of interface to Message based PHY interface
 * @author Mindspeed Technologies
 * @version $Revision: 1.2 $
 *
 * COPYRIGHT(c) 2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef LTE_IF_H
#define LTE_IF_H

U32 lte_if_copy_tx_vector(U8* pDest, PDLSUBFRDESC pVec);
U32 lte_if_copy_rx_vector(U8* pDest, PULSUBFRDESC pVec);

RESULTCODE LteMacCheckMsgIntegrity(void);

#endif // LTE_IF_H
