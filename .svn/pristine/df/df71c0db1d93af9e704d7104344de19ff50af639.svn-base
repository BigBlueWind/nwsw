/** @file e_tm_ables.h
 *
 * @brief Header file to tables
 * @author Mindspeed Technologies
 * @version $Revision: 1.3 $
 *
 * COPYRIGHT(c) 2008-2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef _E_TM_TABLES_H
#define _E_TM_TABLES_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "e_tm_defines.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

#define RCI_MAX_SIZE 7 //UNDEFINED, 1.1, 1.2, 2, 3.1, 3.2, 3.3
#define BCH_MAX_SIZE 6	
#define MAX_PDCCH_CHANNELS      20
#define MAX_PDSCH_CHANNELS      20
#define MAX_SDU_CHANNELS        10
#define MAX_LAYERS              4
#define TIMERRBUFDEPTH          6
#define TIMERRINFO_DELAY_SF     3
#define MAX_DYN_PERIOD          20

#define RB_UNASSIGNED           0xffffffff
#define MAX_RBS                 110
#define DYN_CFG_END             0xff

#define DCI_MAX_SIZE            128

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

extern const U32 rs_boosting[RCI_MAX_SIZE][BCH_MAX_SIZE];
extern const S16 epre_sinchro[RCI_MAX_SIZE][BCH_MAX_SIZE];		/* Synchronisation signal EPRE / ERS [dB] */
extern const S32 reserved_epre[RCI_MAX_SIZE][BCH_MAX_SIZE];		/* Reserved EPRE / ERS [dB] */
/* PBCH */
extern const S16 pbch_epre[RCI_MAX_SIZE][BCH_MAX_SIZE];			/* PBCH EPRE / ERS [dB] */
extern const S32 pbch_reserved_epre[RCI_MAX_SIZE][BCH_MAX_SIZE];/* ReservedEPRE / ERS [dB]/ ERS [dB] */
/* PCFICH */
extern const U8  n_ctrl_symbols[RCI_MAX_SIZE][BCH_MAX_SIZE];		/* # of symbols used for control channels */
extern const S16 pcfich_epre[RCI_MAX_SIZE][BCH_MAX_SIZE]; 		/* PCFICH EPRE / ERS [dB] */
/* PHICH */
extern const U8  n_phich_group[RCI_MAX_SIZE][BCH_MAX_SIZE]; 		/* # of PHICH groups */
extern const U8  n_phich_per_group[RCI_MAX_SIZE][BCH_MAX_SIZE];	/* # of PHICH per group */
extern const U8  phich_group_sfn[RCI_MAX_SIZE][BCH_MAX_SIZE];		//for Tdd
extern const S16 phich_sym_power[RCI_MAX_SIZE][BCH_MAX_SIZE];	/* PHICH BPSK symbol power / ERS [dB] */
extern const S32 phich_group_epre[RCI_MAX_SIZE][BCH_MAX_SIZE];  /* PHICH group EPRE / ERS */
/* PDCCH */
extern const U8  n_avalable_regs[RCI_MAX_SIZE][BCH_MAX_SIZE];	/* # of available REGs */
extern const U8  n_pdcch[RCI_MAX_SIZE][BCH_MAX_SIZE];			/* # of PDCCH */
extern const U8  n_cce_per_pdcch[RCI_MAX_SIZE][BCH_MAX_SIZE];	/* # of CCEs per PDCCH */
extern const U8  n_reg_per_cce[RCI_MAX_SIZE][BCH_MAX_SIZE];		/* # of REGs per CCE */
extern const U8  n_reg_all_to_pdcch[RCI_MAX_SIZE][BCH_MAX_SIZE];	/* # of REGs allocated to PDCCH */
extern const U8  n_nil_reg_padding[RCI_MAX_SIZE][BCH_MAX_SIZE];	/* # of <NIL> REGs added for padding */
extern const S16 pdcch_reg_epre[RCI_MAX_SIZE][BCH_MAX_SIZE];	/* PDCCH REG EPRE / ERS [dB] */
extern const S32 nil_reg_epre[RCI_MAX_SIZE][BCH_MAX_SIZE]; 		/* <NIL> REG EPRE / ERS [dB] */
/* PDSCH */
extern const U8  n_pdsch_boosted_prb[RCI_MAX_SIZE][BCH_MAX_SIZE];	/* # of QPSK PDSCH PRBs which are boosted */
extern const S16 pdsch_boosted_prb_pa[RCI_MAX_SIZE][BCH_MAX_SIZE];	/* PRB PA = EA/ERS [dB] */
extern const U8  n_pdsch_de_boosted_prb[RCI_MAX_SIZE][BCH_MAX_SIZE]; /* # of QPSK PDSCH PRBs which are de-boosted */
extern const S16 pdsch_de_boosted_prb_pa[RCI_MAX_SIZE][BCH_MAX_SIZE];	/* PRB PA = EA/ERS [dB] */

/* RCT */
extern const U8 **E_TM_1_1MapIdx[];
extern const U8 **E_TM_1_2MapIdx[];
extern const U8 **E_TM_1_2MapIdxDeBoosted[];
extern const U8 **E_TM_2MapIdx[];
extern const U8 **E_TM_3_1MapIdx[];
extern const U8 **E_TM_3_2MapIdx[];
extern const U8 **E_TM_3_3MapIdx[];

/****************************************************************************
 * Exported Functions
 ****************************************************************************/



#endif /* _E_TM_TABLES_H */

