/** @file lte_pbch.h
 *
 * @brief Defenitions and prototypes for creating PBCH message
 * @author Mindspeed Technologies
 * @version $Revision: 1.3 $
 *
 * COPYRIGHT(c) 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef LTE_PBCH_H
#define LTE_PBCH_H

RESULTCODE lte_pbch_create_sdu(U32 num_bits, U8 ch_id, U8 channel_type, U8* pbch_payload);

RESULTCODE lte_add_pbch(PDLCHANDESC pCh,
                        U32 id);

#endif //LTE_PBCH_H

