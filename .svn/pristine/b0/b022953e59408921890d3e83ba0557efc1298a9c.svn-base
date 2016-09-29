/** @file lte_phich.h
 *
 * @brief Defenitions and prototypes for creating PHICH message
 * @author Mindspeed Technologies
 * @version $Revision: 1.4 $
 *
 * COPYRIGHT(c) 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef LTE_PHICH_H
#define LTE_PHICH_H

#include "resultcodes.h"

void lte_phich_calc_n_group(BS_STATIC_CONFIG *pbs_static_config);
int lte_phich_get_pcfich_regs(void);
int lte_phich_get_regs(int ng, 
                       int frame_mode, // 0 - fdd, 1 - tdd
                       int uldlcfg, 
                       int sfn);
U16 lte_phich_get_lower_rb(PMAPPINGINFO pmapinfo);
U16 lte_phich_get_i(void);

S16 lte_phich_get_power(void);

RESULTCODE lte_phich_create_sdu(U8 chId, U8 groupId, U8 seqIdx, U8 channelType, U8 nackAck);

RESULTCODE lte_add_phich(PDCICHANNELDESC pCh,
                         U32 id,
                         U8 groupId,
                         U8 seqIdx);

U32 lte_phich_calc(PDLSUBFRDESC pDlSubFrame,
                   PULSUBFRDESC pUlSubFrame,
                   PRXSDUIND pRxSdu);

typedef RESULTCODE (*pLtePhyHiDciAPISet)(PDLSUBFRDESC pDlSubFrame, PULSUBFRDESC pUlSubFrame,
                                         PRXSDUIND pRxSdu);
extern pLtePhyHiDciAPISet pHiDciApiSet;

RESULTCODE lte_phich_dci_fake(PDLSUBFRDESC pDlSubFrame, PULSUBFRDESC pUlSubFrame, PRXSDUIND pRxSdu);
RESULTCODE lte_phich_dci_set (PDLSUBFRDESC pDlSubFrame, PULSUBFRDESC pUlSubFrame, PRXSDUIND pRxSdu);



#endif //LTE_PHICH_H
