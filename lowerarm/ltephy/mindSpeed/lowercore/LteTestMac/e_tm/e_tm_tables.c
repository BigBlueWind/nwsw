/** @file table.c
 *
 * @brief Tables
 * @author Mindspeed Technologies
 * @version $Revision: 1.3 $
 *
 * COPYRIGHT(c) 2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */
#include "typedef.h"

#include "e_tm_defines.h"
#include "e_tm_tables.h"
#include "LtePhyL2Api.h"

/*
 * Private Limits
 */

#define DL_MAP_SIZE             101
#define UL_MAP_SIZE             101

/****************************************************************************
 * Privat Definitions
 ****************************************************************************/
// round +0.5 if x>0 and-0.5, if x<=0
#define FLOAT2Q8(x)     (S16) ( (x>0.0) ? ((x+(float)1/512.0) * 256) : ((x-(float)1/512.0) * 256) )

#define MINUS_INF_Q8    (-128 * 256)
#define NAQ8 0xFF
#define UDQ8 0xFF

//----------------------------------------------------- Referance, Syncronisation Signals -----------------------------------------------------

/* RS boosting, PB = EB/EA */
const U32 rs_boosting[RCI_MAX_SIZE][BCH_MAX_SIZE] = {
    //{1.4, 3, 5, 10, 15, 20} MHz
    {UDQ8},                // E_TM_UNDEFINED
    {1, 1, 1, 1, 1, 1},    // E-TM 1.1
    {1, 1, 1, 1, 1, 1},    // E-TM 1.2
    {1, 1, 1, 1, 1, 1},    // E-TM 2
    {1, 1, 1, 1, 1, 1},    // E-TM 3.1
    {1, 1, 1, 1, 1, 1},    // E-TM 3.2
    {1, 1, 1, 1, 1, 1}     // E-TM 3.3
};     

/* Synchronisation signal EPRE / ERS [dB] */
const S16 epre_sinchro[RCI_MAX_SIZE][BCH_MAX_SIZE] = {
    //{1.4, 3, 5, 10, 15, 20} MHz
    {UDQ8},                                                                                                    // E_TM_UNDEFINED
    {FLOAT2Q8(0), FLOAT2Q8(0),      FLOAT2Q8(0),      FLOAT2Q8(0),      FLOAT2Q8(0),      FLOAT2Q8(0)},        // E-TM 1.1
    {FLOAT2Q8(0), FLOAT2Q8(-4.730), FLOAT2Q8(-4.730), FLOAT2Q8(-4.730), FLOAT2Q8(-4.730), FLOAT2Q8(-4.730)},   // E-TM 1.2
    {FLOAT2Q8(0), FLOAT2Q8(0),      FLOAT2Q8(0),      FLOAT2Q8(0),      FLOAT2Q8(0),      FLOAT2Q8(0)},        // E-TM 2
    {FLOAT2Q8(0), FLOAT2Q8(0),      FLOAT2Q8(0),      FLOAT2Q8(0),      FLOAT2Q8(0),      FLOAT2Q8(0)},        // E-TM 3.1
    {FLOAT2Q8(0), FLOAT2Q8(1.573),  FLOAT2Q8(2.426),  FLOAT2Q8(2.426),  FLOAT2Q8(3.005),  FLOAT2Q8(2.426)},    // E-TM 3.2
    {FLOAT2Q8(0), FLOAT2Q8(2.189),  FLOAT2Q8(2.580),  FLOAT2Q8(2.427),  FLOAT2Q8(2.477),  FLOAT2Q8(2.427)}     // E-TM 3.3
};

/* Reserved EPRE / ERS [dB] */
const S32 reserved_epre[RCI_MAX_SIZE][BCH_MAX_SIZE] = {
    //{1.4, 3, 5, 10, 15, 20} MHz
    {UDQ8},                                                                                  // E_TM_UNDEFINED
    {MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8},    // E-TM 1.1
    {MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8},    // E-TM 1.2
    {MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8},    // E-TM 2
    {MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8},    // E-TM 3.1
    {MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8},    // E-TM 3.2
    {MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8}     // E-TM 3.3
};

//----------------------------------------------------- PBCH -----------------------------------------------------

/* PBCH EPRE / ERS [dB] */
const S16 pbch_epre[RCI_MAX_SIZE][BCH_MAX_SIZE] = {
    //{1.4, 3, 5, 10, 15, 20} MHz
    {UDQ8},                                                                                                  // E_TM_UNDEFINED
    {FLOAT2Q8(0), FLOAT2Q8(2),      FLOAT2Q8(2),      FLOAT2Q8(2),      FLOAT2Q8(0),      FLOAT2Q8(0)},      // E-TM 1.1
    {FLOAT2Q8(0), FLOAT2Q8(-4.730), FLOAT2Q8(-4.730), FLOAT2Q8(-4.730), FLOAT2Q8(-4.730), FLOAT2Q8(-4.730)}, // E-TM 1.2
    {FLOAT2Q8(0), FLOAT2Q8(0),      FLOAT2Q8(0),      FLOAT2Q8(0),      FLOAT2Q8(0),      FLOAT2Q8(0)},      // E-TM 2
    {FLOAT2Q8(0), FLOAT2Q8(0),      FLOAT2Q8(0),      FLOAT2Q8(0),      FLOAT2Q8(0),      FLOAT2Q8(0)},      // E-TM 3.1
    {FLOAT2Q8(0), FLOAT2Q8(1.573),  FLOAT2Q8(2.426),  FLOAT2Q8(2.426),  FLOAT2Q8(3.005),  FLOAT2Q8(2.426)},  // E-TM 3.2
    {FLOAT2Q8(0), FLOAT2Q8(2.189),  FLOAT2Q8(2.580),  FLOAT2Q8(2.427),  FLOAT2Q8(2.477),  FLOAT2Q8(2.427)}   // E-TM 3.3
};

/* ReservedEPRE / ERS [dB]/ ERS [dB] */
const S32 pbch_reserved_epre[RCI_MAX_SIZE][BCH_MAX_SIZE] = {
    //{1.4, 3, 5, 10, 15, 20} MHz
    {UDQ8},                                                                                  // E_TM_UNDEFINED
    {MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8},    // E-TM 1.1
    {MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8},    // E-TM 1.2
    {MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8},    // E-TM 2
    {MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8},    // E-TM 3.1
    {MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8},    // E-TM 3.2
    {MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8}     // E-TM 3.3
};

//----------------------------------------------------- PCFICH -----------------------------------------------------

/* # of symbols used for control channels */
const U8 n_ctrl_symbols[RCI_MAX_SIZE][BCH_MAX_SIZE] = {
    //{1.4, 3, 5, 10, 15, 20} MHz
    {UDQ8},                // E_TM_UNDEFINED
    {2, 1, 1, 1, 1, 1},    // E-TM 1.1
    {2, 1, 1, 1, 1, 1},    // E-TM 1.2
    {2, 1, 1, 1, 1, 1},    // E-TM 2
    {2, 1, 1, 1, 1, 1},    // E-TM 3.1
    {2, 1, 1, 1, 1, 1},    // E-TM 3.2
    {2, 1, 1, 1, 1, 1}     // E-TM 3.3
};     

/* PCFICH EPRE / ERS [dB] */
const S16 pcfich_epre[RCI_MAX_SIZE][BCH_MAX_SIZE] = {
    //{1.4, 3, 5, 10, 15, 20} MHz
    {UDQ8},                              // E_TM_UNDEFINED
    {FLOAT2Q8(3.222), 0, 0, 0, 0, 0},    // E-TM 1.1
    {FLOAT2Q8(3.222), 0, 0, 0, 0, 0},    // E-TM 1.2
    {FLOAT2Q8(0.0),   0, 0, 0, 0, 0},    // E-TM 2
    {FLOAT2Q8(3.222), 0, 0, 0, 0, 0},    // E-TM 3.1
    {FLOAT2Q8(3.222), 0, 0, 0, 0, 0},    // E-TM 3.2
    {FLOAT2Q8(3.222), 0, 0, 0, 0, 0}     // E-TM 3.3
};
    
//----------------------------------------------------- PHICH -----------------------------------------------------

/* # of PHICH groups */
const U8 n_phich_group[RCI_MAX_SIZE][BCH_MAX_SIZE] = {
    //{1.4, 3, 5, 10, 15, 20} MHz
    {UDQ8},                // E_TM_UNDEFINED
    {1, 1, 1, 2, 2, 3},    // E-TM 1.1
    {1, 1, 1, 2, 2, 2},    // E-TM 1.2
    {1, 1, 1, 2, 2, 2},    // E-TM 2
    {1, 1, 1, 2, 2, 3},    // E-TM 3.1
    {1, 1, 1, 2, 2, 3},    // E-TM 3.2
    {1, 1, 1, 2, 2, 3}     // E-TM 3.3
}; 

/* # of PHICH per group */
const U8 n_phich_per_group[RCI_MAX_SIZE][BCH_MAX_SIZE] = {
    //{1.4, 3, 5, 10, 15, 20} MHz
    {UDQ8},                // E_TM_UNDEFINED
    {2, 2, 2, 2, 2, 2},    // E-TM 1.1
    {2, 2, 2, 2, 2, 2},    // E-TM 1.2
    {2, 2, 2, 2, 2, 2},    // E-TM 2
    {2, 2, 2, 2, 2, 2},    // E-TM 3.1
    {2, 2, 2, 2, 2, 2},    // E-TM 3.2
    {2, 2, 2, 2, 2, 2}     // E-TM 3.3
}; 

//for Tdd
const U8 phich_group_sfn[RCI_MAX_SIZE][BCH_MAX_SIZE] = {
    //{1.4, 3, 5, 10, 15, 20} MHz
    {UDQ8},                // E_TM_UNDEFINED
    {1, 1, 1, 1, 1, 1},    // E-TM 1.1
    {1, 1, 1, 1, 1, 1},    // E-TM 1.2
    {1, 1, 1, 1, 1, 1},    // E-TM 2
    {1, 1, 1, 1, 1, 1},    // E-TM 3.1
    {1, 1, 1, 1, 1, 1},    // E-TM 3.2
    {1, 1, 1, 1, 1, 1}     // E-TM 3.3
}; 

/* PHICH BPSK symbol power / ERS [dB] */
const S16 phich_sym_power[RCI_MAX_SIZE][BCH_MAX_SIZE] = {
    //{1.4, 3, 5, 10, 15, 20} MHz
    {UDQ8},                                                                                                      // E_TM_UNDEFINED
    {FLOAT2Q8(-3.010), FLOAT2Q8(-3.010), FLOAT2Q8(-3.010), FLOAT2Q8(-3.010), FLOAT2Q8(-3.010), FLOAT2Q8(-3.010)},// E-TM 1.1
    {FLOAT2Q8(-3.010), FLOAT2Q8(-3.010), FLOAT2Q8(-3.010), FLOAT2Q8(-3.010), FLOAT2Q8(-3.010), FLOAT2Q8(-3.010)},// E-TM 1.2
    {FLOAT2Q8(-3.010), FLOAT2Q8(-3.010), FLOAT2Q8(-3.010), FLOAT2Q8(-3.010), FLOAT2Q8(-3.010), FLOAT2Q8(-3.010)},// E-TM 2
    {FLOAT2Q8(-3.010), FLOAT2Q8(-3.010), FLOAT2Q8(-3.010), FLOAT2Q8(-3.010), FLOAT2Q8(-3.010), FLOAT2Q8(-3.010)},// E-TM 3.1
    {FLOAT2Q8(-3.010), FLOAT2Q8(-3.010), FLOAT2Q8(-3.010), FLOAT2Q8(-3.010), FLOAT2Q8(-3.010), FLOAT2Q8(-3.010)},// E-TM 3.2
    {FLOAT2Q8(-3.010), FLOAT2Q8(-3.010), FLOAT2Q8(-3.010), FLOAT2Q8(-3.010), FLOAT2Q8(-3.010), FLOAT2Q8(-3.010)},// E-TM 3.3
}; 

/* PHICH group EPRE / ERS */
const S32 phich_group_epre[RCI_MAX_SIZE][BCH_MAX_SIZE] = {
    //{1.4, 3, 5, 10, 15, 20} MHz
    {UDQ8},                // E_TM_UNDEFINED
    {0, 0, 0, 0, 0, 0},    // E-TM 1.1
    {0, 0, 0, 0, 0, 0},    // E-TM 1.2
    {0, 0, 0, 0, 0, 0},    // E-TM 2
    {0, 0, 0, 0, 0, 0},    // E-TM 3.1
    {0, 0, 0, 0, 0, 0},    // E-TM 3.2
    {0, 0, 0, 0, 0, 0}     // E-TM 3.3
}; 

//----------------------------------------------------- PDCCH -----------------------------------------------------

/* # of available REGs */
const U8 n_avalable_regs[RCI_MAX_SIZE][BCH_MAX_SIZE] = {
    //{1.4, 3, 5, 10, 15, 20} MHz
    {UDQ8},                        // E_TM_UNDEFINED
    {23, 23, 43, 90, 140, 187},    // E-TM 1.1
    {23, 23, 43, 90, 140, 187},    // E-TM 1.2
    {23, 23, 43, 90, 140, 187},    // E-TM 2
    {23, 23, 43, 90, 140, 187},    // E-TM 3.1
    {23, 23, 43, 90, 140, 187},    // E-TM 3.2
    {23, 23, 43, 90, 140, 187}     // E-TM 3.3
}; 

/* # of PDCCH */
const U8 n_pdcch[RCI_MAX_SIZE][BCH_MAX_SIZE] = {
    //{1.4, 3, 5, 10, 15, 20} MHz
    {UDQ8},                // E_TM_UNDEFINED
    {2, 2, 2, 5, 7, 10},   // E-TM 1.1
    {2, 2, 2, 5, 7, 10},   // E-TM 1.2
    {1, 1, 1, 1, 1, 1},    // E-TM 2
    {2, 2, 2, 5, 7, 10},   // E-TM 3.1
    {2, 2, 2, 5, 7, 10},   // E-TM 3.2
    {2, 2, 2, 5, 7, 10}    // E-TM 3.3
}; 

/* # of CCEs per PDCCH */
const U8 n_cce_per_pdcch[RCI_MAX_SIZE][BCH_MAX_SIZE] = {
    //{1.4, 3, 5, 10, 15, 20} MHz
    {UDQ8},                // E_TM_UNDEFINED
    {1, 1, 2, 2, 2, 2},    // E-TM 1.1
    {1, 1, 2, 2, 2, 2},    // E-TM 1.2
    {1, 1, 2, 2, 2, 2},    // E-TM 2
    {1, 1, 2, 2, 2, 2},    // E-TM 3.1
    {1, 1, 2, 2, 2, 2},    // E-TM 3.2
    {1, 1, 2, 2, 2, 2}     // E-TM 3.3
}; 

/* # of REGs per CCE */
const U8 n_reg_per_cce[RCI_MAX_SIZE][BCH_MAX_SIZE] = {
    //{1.4, 3, 5, 10, 15, 20} MHz
    {UDQ8},                // E_TM_UNDEFINED
    {9, 9, 9, 9, 9, 9},    // E-TM 1.1
    {9, 9, 9, 9, 9, 9},    // E-TM 1.2
    {9, 9, 9, 9, 9, 9},    // E-TM 2
    {9, 9, 9, 9, 9, 9},    // E-TM 3.1
    {9, 9, 9, 9, 9, 9},    // E-TM 3.2
    {9, 9, 9, 9, 9, 9}     // E-TM 3.3
}; 

/* # of REGs allocated to PDCCH */
const U8 n_reg_all_to_pdcch[RCI_MAX_SIZE][BCH_MAX_SIZE] = {
    //{1.4, 3, 5, 10, 15, 20} MHz
    {UDQ8},                        // E_TM_UNDEFINED
    {18, 18, 36, 90, 126, 180},    // E-TM 1.1
    {18, 18, 36, 90, 126, 180},    // E-TM 1.2
    {9,  9,  18, 18, 18,  18},     // E-TM 2
    {18, 18, 36, 90, 126, 180},    // E-TM 3.1
    {18, 18, 36, 90, 126, 180},    // E-TM 3.2
    {18, 18, 36, 90, 126, 180}     // E-TM 3.3
};

/* # of <NIL> REGs added for padding */
const U8 n_nil_reg_padding[RCI_MAX_SIZE][BCH_MAX_SIZE] = {
    //{1.4, 3, 5, 10, 15, 20} MHz
    {UDQ8},                        // E_TM_UNDEFINED
    {5,  5,  7,  0,  14,  7},      // E-TM 1.1
    {5,  5,  7,  0,  14,  7},      // E-TM 1.2
    {14, 14, 25, 72, 122, 169},    // E-TM 2
    {5,  5,  7,  0,  14,  7},      // E-TM 3.1
    {5,  5,  7,  0,  14,  7},      // E-TM 3.2
    {5,  5,  7,  0,  14,  7}       // E-TM 3.3
};

/* PDCCH REG EPRE / ERS [dB] */
const S16 pdcch_reg_epre[RCI_MAX_SIZE][BCH_MAX_SIZE] = {
    //{1.4, 3, 5, 10, 15, 20} MHz
    {UDQ8},                                                                                                    // E_TM_UNDEFINED
    {FLOAT2Q8(0.792), FLOAT2Q8(2.290), FLOAT2Q8(1.880), FLOAT2Q8(1.065), FLOAT2Q8(1.488), FLOAT2Q8(1.195)},    // E-TM 1.1
    {FLOAT2Q8(0.792), FLOAT2Q8(2.290), FLOAT2Q8(1.880), FLOAT2Q8(1.065), FLOAT2Q8(1.488), FLOAT2Q8(1.195)},    // E-TM 1.2
    {FLOAT2Q8(0.0),   FLOAT2Q8(0.0),   FLOAT2Q8(0.0),   FLOAT2Q8(0.0),   FLOAT2Q8(0.0),   FLOAT2Q8(0.0)  },    // E-TM 2
    {FLOAT2Q8(0.792), FLOAT2Q8(2.290), FLOAT2Q8(1.880), FLOAT2Q8(1.065), FLOAT2Q8(1.488), FLOAT2Q8(1.195)},    // E-TM 3.1
    {FLOAT2Q8(0.792), FLOAT2Q8(2.290), FLOAT2Q8(1.880), FLOAT2Q8(1.065), FLOAT2Q8(1.488), FLOAT2Q8(1.195)},    // E-TM 3.2
    {FLOAT2Q8(0.792), FLOAT2Q8(2.290), FLOAT2Q8(1.880), FLOAT2Q8(1.065), FLOAT2Q8(1.488), FLOAT2Q8(1.195)}     // E-TM 3.3
};

/* <NIL> REG EPRE / ERS [dB] */
const S32 nil_reg_epre[RCI_MAX_SIZE][BCH_MAX_SIZE] = {
    //{1.4, 3, 5, 10, 15, 20} MHz
    {UDQ8},                                                                                  // E_TM_UNDEFINED
    {MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8},    // E-TM 1.1
    {MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8},    // E-TM 1.2
    {MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8},    // E-TM 2
    {MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8},    // E-TM 3.1
    {MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8},    // E-TM 3.2
    {MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8, MINUS_INF_Q8}     // E-TM 3.3
};

//----------------------------------------------------- PDSCH -----------------------------------------------------

/* # of QPSK PDSCH PRBs which are boosted */
const U8 n_pdsch_boosted_prb[RCI_MAX_SIZE][BCH_MAX_SIZE] = {
    //{1.4, 3, 5, 10, 15, 20} MHz
    {UDQ8},                     // E_TM_UNDEFINED
    {6, 15, 25, 50, 75, 100},   // E-TM 1.1
    {2, 6,  10, 20, 30, 40},    // E-TM 1.2
    {1, 1,  1,  1,  1,  1},     // E-TM 2
    {6, 15, 25, 50, 75, 100},   // E-TM 3.1
    {4, 7,  15, 30, 50, 60},    // E-TM 3.2
    {3, 7,  13, 25, 38, 50}     // E-TM 3.3
};

/* PRB PA = EA/ERS [dB] */
const S16 pdsch_boosted_prb_pa[RCI_MAX_SIZE][BCH_MAX_SIZE] = {
    //{1.4, 3, 5, 10, 15, 20} MHz
    {UDQ8},                // E_TM_UNDEFINED
    {FLOAT2Q8(0),  FLOAT2Q8(0),  FLOAT2Q8(0),  FLOAT2Q8(0),  FLOAT2Q8(0),  FLOAT2Q8(0)},     // E-TM 1.1
    {FLOAT2Q8(3),  FLOAT2Q8(3),  FLOAT2Q8(3),  FLOAT2Q8(3),  FLOAT2Q8(3),  FLOAT2Q8(3)},     // E-TM 1.2
    {FLOAT2Q8(0),  FLOAT2Q8(0),  FLOAT2Q8(0),  FLOAT2Q8(0),  FLOAT2Q8(0),  FLOAT2Q8(0)},     // E-TM 2 
    {FLOAT2Q8(0),  FLOAT2Q8(0),  FLOAT2Q8(0),  FLOAT2Q8(0),  FLOAT2Q8(0),  FLOAT2Q8(0)},     // E-TM 3.1
    {FLOAT2Q8(-3), FLOAT2Q8(-3), FLOAT2Q8(-3), FLOAT2Q8(-3), FLOAT2Q8(-3), FLOAT2Q8(-3)},    // E-TM 3.2
    {FLOAT2Q8(-6), FLOAT2Q8(-6), FLOAT2Q8(-6), FLOAT2Q8(-6), FLOAT2Q8(-6), FLOAT2Q8(-6)}     // E-TM 3.3
};
// TODO: E-TM3.2 and 3.3 1.4Mhz in subframes containing PBCH or synchronisation signal REs, 
// TODO: no PRB boosting/deboosting shall be applied, i.e. PRB PA = EA/ERS  = 0 [dB]

/* # of QPSK PDSCH PRBs which are de-boosted */
const U8 n_pdsch_de_boosted_prb[RCI_MAX_SIZE][BCH_MAX_SIZE] = {
    //{1.4, 3, 5, 10, 15, 20} MHz
    {UDQ8},                    // E_TM_UNDEFINED
    {0, 0,  0,  0,  0,  0},    // E-TM 1.1
    {4, 9,  15, 30, 45, 60},   // E-TM 1.2
    {5, 14, 24, 49, 74, 99},   // E-TM 2
    {0, 0,  0,  0,  0,  0},    // E-TM 3.1
    {2, 8,  10, 20, 25, 40},   // E-TM 3.2
    {3, 8,  12, 25, 37, 50}    // E-TM 3.3
};        
    
/* PRB PA = EA/ERS [dB] */ 
const S16 pdsch_de_boosted_prb_pa[RCI_MAX_SIZE][BCH_MAX_SIZE] = {
    //{1.4, 3, 5, 10, 15, 20} MHz
    {UDQ8},                                                                                                       // E_TM_UNDEFINED
    {NAQ8,             NAQ8,             NAQ8,             NAQ8,             NAQ8,             NAQ8},             // E-TM 1.1
    {FLOAT2Q8(-2.990), FLOAT2Q8(-4.730), FLOAT2Q8(-4.730), FLOAT2Q8(-4.730), FLOAT2Q8(-4.730), FLOAT2Q8(-4.730)}, // E-TM 1.2
    {MINUS_INF_Q8,     MINUS_INF_Q8,     MINUS_INF_Q8,     MINUS_INF_Q8,     MINUS_INF_Q8,     MINUS_INF_Q8},     // E-TM 2
    {NAQ8,             NAQ8,             NAQ8,             NAQ8,             NAQ8,             NAQ8},             // E-TM 3.1
    {FLOAT2Q8(3.005),  FLOAT2Q8(1.573),  FLOAT2Q8(2.426),  FLOAT2Q8(2.426),  FLOAT2Q8(3.005),  FLOAT2Q8(2.426)},  // E-TM 3.2
    {FLOAT2Q8(2.427),  FLOAT2Q8(2.189),  FLOAT2Q8(2.580),  FLOAT2Q8(2.427),  FLOAT2Q8(2.477),  FLOAT2Q8(2.427)}   // E-TM 3.3
};    

// ----------------------------------------------------- Tables for allocate rbs ----------------------------------------------------

//-----------------------------------------------------          E-TM 6.1.1        -----------------------------------------------------
static const U8 noboosting1_4mhz_subframe[] = { 0, 1, 2, 3, 4, 5, 0xFF };

static const U8 *noboosted1_4MhzMapIndex[10] =
{
  noboosting1_4mhz_subframe,
  noboosting1_4mhz_subframe,
  noboosting1_4mhz_subframe,
  noboosting1_4mhz_subframe,
  noboosting1_4mhz_subframe,
  noboosting1_4mhz_subframe,
  noboosting1_4mhz_subframe,
  noboosting1_4mhz_subframe,
  noboosting1_4mhz_subframe,
  noboosting1_4mhz_subframe
};

static const U8 noboosting3mhz_subframe[] =
{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 0xFF};

static const U8 *noboosted3MhzMapIndex[10] =
{
  noboosting3mhz_subframe,
  noboosting3mhz_subframe,
  noboosting3mhz_subframe,
  noboosting3mhz_subframe,
  noboosting3mhz_subframe,
  noboosting3mhz_subframe,
  noboosting3mhz_subframe,
  noboosting3mhz_subframe,
  noboosting3mhz_subframe,
  noboosting3mhz_subframe
};

static const U8 noboosting5mhz_subframe[] =
{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
  22, 23, 24, 0xFF};

static const U8 *noboosted5MhzMapIndex[10] =
{
  noboosting5mhz_subframe,
  noboosting5mhz_subframe,
  noboosting5mhz_subframe,
  noboosting5mhz_subframe,
  noboosting5mhz_subframe,
  noboosting5mhz_subframe,
  noboosting5mhz_subframe,
  noboosting5mhz_subframe,
  noboosting5mhz_subframe,
  noboosting5mhz_subframe
};


static const U8 noboosting10mhz_subframe[] =
{ 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 
  20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 ,33, 34, 35, 36, 37, 38, 39, 
  40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 0xFF};

static const U8 *noboosted10MhzMapIndex[10] =
{
  noboosting10mhz_subframe,
  noboosting10mhz_subframe,
  noboosting10mhz_subframe,
  noboosting10mhz_subframe,
  noboosting10mhz_subframe,
  noboosting10mhz_subframe,
  noboosting10mhz_subframe,
  noboosting10mhz_subframe,
  noboosting10mhz_subframe,
  noboosting10mhz_subframe
};

static const U8 noboosting20mhz_subframe[] =
{ 
  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 
  20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 ,33, 34, 35, 36, 37, 38, 39, 
  40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
  60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 
  80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 0xFF};

static const U8 *noboosted20MhzMapIndex[10] =
{
    noboosting20mhz_subframe,
    noboosting20mhz_subframe,
    noboosting20mhz_subframe,
    noboosting20mhz_subframe,
    noboosting20mhz_subframe,
    noboosting20mhz_subframe,
    noboosting20mhz_subframe,
    noboosting20mhz_subframe,
    noboosting20mhz_subframe,
    noboosting20mhz_subframe,
};

const U8 **E_TM_1_1MapIdx[6] =
{
    noboosted1_4MhzMapIndex,
    noboosted3MhzMapIndex,
    noboosted5MhzMapIndex,
    noboosted10MhzMapIndex,
    NULL,
    noboosted20MhzMapIndex
};

//-----------------------------------------------------          E-TM 6.1.2        -----------------------------------------------------
//1_4Mhz
static const U8 boosted1_4mhz_NA[] = { 0xFF };
static const U8 boosted1_4mhz_subframe1[] = { 1, 3, 0xFF };
static const U8 boosted1_4mhz_subframe2[] = { 1, 3, 0xFF };
static const U8 boosted1_4mhz_subframe3[] = { 2, 3, 0xFF };
static const U8 boosted1_4mhz_subframe4[] = { 2, 5, 0xFF };
static const U8 boosted1_4mhz_subframe6[] = { 0, 2, 0xFF };
static const U8 boosted1_4mhz_subframe7[] = { 0, 5, 0xFF };
static const U8 boosted1_4mhz_subframe8[] = { 2, 5, 0xFF };
static const U8 boosted1_4mhz_subframe9[] = { 1, 5, 0xFF };

static const U8 *boosted1_4MhzMapIndex[10] =
{
  boosted1_4mhz_NA, /*noboosting1_4mhz_subframe // Do not add PDSCH for 6.1.1 E-TM 1.2 1.4 MHz SF#0*/
  boosted1_4mhz_subframe1,
  boosted1_4mhz_subframe2,
  boosted1_4mhz_subframe3,
  boosted1_4mhz_subframe4,
  boosted1_4mhz_NA, /*noboosting1_4mhz_subframe // Do not add PDSCH for 6.1.1 E-TM 1.2 1.4 MHz SF#0*/
  boosted1_4mhz_subframe6,
  boosted1_4mhz_subframe7,
  boosted1_4mhz_subframe8,
  boosted1_4mhz_subframe9
};

static const U8 *deboosted1_4MhzMapIndex[10] =
{
  noboosting1_4mhz_subframe,
  boosted1_4mhz_subframe1,
  boosted1_4mhz_subframe2,
  boosted1_4mhz_subframe3,
  boosted1_4mhz_subframe4,
  noboosting1_4mhz_subframe,
  boosted1_4mhz_subframe6,
  boosted1_4mhz_subframe7,
  boosted1_4mhz_subframe8,
  boosted1_4mhz_subframe9
};

//3Mhz
static const U8 boosted3mhz_subframe0[] = { 0, 1, 2,  11, 12, 13, 0xFF };
static const U8 boosted3mhz_subframe1[] = { 0, 4, 10, 11, 12, 13, 0xFF };
static const U8 boosted3mhz_subframe2[] = { 0, 3, 5,  6,  11, 13, 0xFF };
static const U8 boosted3mhz_subframe3[] = { 0, 1, 4,  5,  7,  12, 0xFF };
static const U8 boosted3mhz_subframe4[] = { 0, 2, 3,  4,  9,  10, 0xFF };
static const U8 boosted3mhz_subframe5[] = { 1, 2, 3,  11, 12, 14, 0xFF };
static const U8 boosted3mhz_subframe6[] = { 4, 6, 8,  11, 13, 14, 0xFF };
static const U8 boosted3mhz_subframe7[] = { 2, 5, 6,  12, 13, 14, 0xFF };
static const U8 boosted3mhz_subframe8[] = { 0, 3, 4,  7,  8,  11, 0xFF };
static const U8 boosted3mhz_subframe9[] = { 1, 3, 4,  5,  11, 12, 0xFF };

static const U8 *boosted3MhzMapIndex[10] =
{
  boosted3mhz_subframe0,
  boosted3mhz_subframe1,
  boosted3mhz_subframe2,
  boosted3mhz_subframe3,
  boosted3mhz_subframe4,
  boosted3mhz_subframe5,
  boosted3mhz_subframe6,
  boosted3mhz_subframe7,
  boosted3mhz_subframe8,
  boosted3mhz_subframe9
};

//5Mhz
static const U8 boosted5mhz_subframe0[] = { 0, 1, 3, 6,  7,  8,  16, 18, 20, 21, 0xFF };
static const U8 boosted5mhz_subframe1[] = { 0, 1, 4, 5,  9,  10, 12, 17, 18, 24, 0xFF };
static const U8 boosted5mhz_subframe2[] = { 0, 1, 2, 12, 13, 14, 19, 20, 23, 24, 0xFF};
static const U8 boosted5mhz_subframe3[] = { 0, 5, 8, 12, 13, 15, 17, 20, 21, 24, 0xFF };
static const U8 boosted5mhz_subframe4[] = { 0, 4, 6, 7,  12, 13, 15, 16, 22, 23, 0xFF };
static const U8 boosted5mhz_subframe5[] = { 0, 1, 2, 3,  8,  16, 18, 21, 22, 24, 0xFF };
static const U8 boosted5mhz_subframe6[] = { 1, 3, 5, 7,  9,  10, 12, 15, 21, 22, 0xFF };
static const U8 boosted5mhz_subframe7[] = { 0, 1, 2, 3,  7,  10, 14, 18, 20, 21, 0xFF };
static const U8 boosted5mhz_subframe8[] = { 1, 4, 8, 9,  10, 12, 15, 16, 18, 20, 0xFF };
static const U8 boosted5mhz_subframe9[] = { 1, 2, 3, 5,  6,  9,  10, 13, 16, 17, 0xFF};

static const U8 *boosted5MhzMapIndex[10] =
{
  boosted5mhz_subframe0,
  boosted5mhz_subframe1,
  boosted5mhz_subframe2,
  boosted5mhz_subframe3,
  boosted5mhz_subframe4,
  boosted5mhz_subframe5,
  boosted5mhz_subframe6,
  boosted5mhz_subframe7,
  boosted5mhz_subframe8,
  boosted5mhz_subframe9
};

//10Mhz
static const U8 boosted10mhz_subframe0[] =
{ 1,  2,  7,  8,  9,  10, 11, 16, 20, 31, 32, 33, 35, 36, 39, 40, 42, 46, 47, 48, 0xFF };
static const U8 boosted10mhz_subframe1[] =
{ 5,  6,  7,  9,  11, 15, 20, 21, 22, 24, 25, 27, 34, 35, 36, 37, 40, 44, 46, 49, 0xFF };
static const U8 boosted10mhz_subframe2[] =
{ 3,  5,  11, 12, 14, 17, 18, 19, 20, 22, 26, 27, 28, 29, 31, 34, 38, 41, 42, 49, 0xFF };
static const U8 boosted10mhz_subframe3[] =
{ 1,  2,  3,  5,  8,  14, 16, 22, 23, 26, 28, 30, 32, 34, 38, 41, 42, 45, 46, 47, 0xFF };
static const U8 boosted10mhz_subframe4[] =
{ 0,  3,  6,  7,  8,  9,  10, 12, 13, 16, 18, 21, 23, 31, 33, 41, 42, 45, 46, 47, 0xFF };
static const U8 boosted10mhz_subframe5[] =
{ 0,  2,  3,  4,  5,  7,   9, 10, 11, 12, 15, 19, 20, 28, 29, 30, 31, 34, 36, 48,  0xFF };
static const U8 boosted10mhz_subframe6[] =
{ 0,  1,  4,  6,  8,  9,  10, 16, 17, 19, 20, 21, 24, 29, 30, 31, 35, 37, 38, 47,  0xFF };
static const U8 boosted10mhz_subframe7[] =
{ 2,  3,  4,  5,  6,  9,  10, 12, 16, 17, 19, 22, 24, 25, 26, 30, 34, 37, 42, 48, 0xFF };
static const U8 boosted10mhz_subframe8[] =
{ 7,  9,  14, 15, 16, 21, 22, 28, 30, 31, 32, 34, 35, 41, 42, 43, 44, 46, 48, 49, 0xFF };
static const U8 boosted10mhz_subframe9[] =
{ 11, 13, 16, 17, 18, 21, 24, 27, 28, 29, 30, 32, 37, 38, 40, 42, 45, 47, 48, 49, 0xFF };

static const U8 *boosted10MhzMapIndex[10] =
{
  boosted10mhz_subframe0,
  boosted10mhz_subframe1,
  boosted10mhz_subframe2,
  boosted10mhz_subframe3,
  boosted10mhz_subframe4,
  boosted10mhz_subframe5,
  boosted10mhz_subframe6,
  boosted10mhz_subframe7,
  boosted10mhz_subframe8,
  boosted10mhz_subframe9
};

//20Mhz
static const U8 boosted20mhz_subframe0[] =
{ 
    0, 6, 10, 13, 15, 16, 20, 23, 25, 28, 29, 30, 31, 32, 33, 39, 41, 42, 44, 45, 54, 56, 57, 63, 
    66, 67, 68, 76, 77, 79, 82, 84, 85, 88, 92, 94, 95, 97, 98, 99, 0xFF 
};
static const U8 boosted20mhz_subframe1[] =
{ 
    1, 3, 7, 9, 10, 13, 19, 20, 21, 22, 23, 25, 26, 27, 30, 33, 34, 35, 36, 47, 49, 50, 51, 53, 55, 
    57, 60, 61, 64, 68, 76, 77, 80, 83, 84, 86, 87, 89, 95, 99, 0xFF 
};
static const U8 boosted20mhz_subframe2[] =
{ 
    1, 2, 3, 6, 8, 10, 11, 15, 16, 17, 19, 21, 25, 26, 28, 29, 32, 35, 39, 41, 42, 43, 44, 51, 52, 
    54, 60, 64, 69, 76, 79, 81, 84, 86, 88, 89, 90, 93, 94, 99, 0xFF 
};
static const U8 boosted20mhz_subframe3[] =
{
    5, 7, 9, 10, 12, 15, 16, 21, 23, 24, 27, 28, 29, 30, 33, 34, 35, 36, 39, 47, 49, 54, 55, 56, 
    57, 64, 66, 70, 72, 76, 77, 80, 81, 86, 87, 90, 91, 92, 98, 99, 0xFF 
};
static const U8 boosted20mhz_subframe4[] =
{  
    2, 3, 4, 5, 6, 7, 14, 17, 19, 21, 22, 24, 26, 37, 42, 44, 47, 49, 51, 56, 57, 62, 63, 65, 67, 
    70, 71, 73, 76, 77, 81, 83, 85, 86, 87, 89, 94, 95, 97, 99, 0xFF 
};
static const U8 boosted20mhz_subframe5[] =
{ 
    1, 2, 5, 6, 8, 9, 12, 13, 21, 22, 25, 26, 28, 32, 35, 39, 40, 43, 45, 46, 57, 59, 61, 62, 64, 
    66, 68, 71, 73, 77, 78, 84, 85, 86, 93, 94, 95, 96, 97, 99, 0xFF 
};
static const U8 boosted20mhz_subframe6[] =
{ 
    0, 1, 2, 3, 5, 8, 9, 10, 12, 22, 25, 26, 27, 29, 31, 32, 33, 36, 38, 39, 43, 45, 49, 53, 55, 
    59, 62, 63, 64, 71, 72, 73, 75, 77, 78, 81, 84, 89, 97, 98, 0xFF 
};

static const U8 boosted20mhz_subframe7[] =
{ 
    0, 1, 3, 4, 5, 7, 11, 18, 19, 20, 21, 26, 27, 29, 30, 31, 33, 35, 39, 40, 41, 43, 44, 46, 47, 
    50, 53, 55, 56, 62, 64, 66, 67, 69, 70, 72, 74, 92, 93, 98, 0xFF 
};
static const U8 boosted20mhz_subframe8[] =
{ 
    2, 3, 4, 7, 11, 13, 15, 16, 24, 25, 27, 29, 35, 36, 40, 43, 44, 45, 46, 51, 52, 55, 56, 57, 63, 
    64, 65, 68, 71, 77, 78, 81, 82, 83, 84, 85, 86, 90, 94, 98, 0xFF 
};
static const U8 boosted20mhz_subframe9[] =
{ 
    0, 4, 7, 8, 10, 11, 16, 18, 22, 26, 29, 32, 35, 37, 43, 44, 46, 47, 48, 49, 53, 54, 57, 59, 60, 
    61, 64, 66, 67, 69, 70, 76, 78, 81, 84, 87, 89, 91, 95, 96, 0xFF 
};

static const U8 *boosted20MhzMapIndex[10] =
{
  boosted20mhz_subframe0,
  boosted20mhz_subframe1,
  boosted20mhz_subframe2,
  boosted20mhz_subframe3,
  boosted20mhz_subframe4,
  boosted20mhz_subframe5,
  boosted20mhz_subframe6,
  boosted20mhz_subframe7,
  boosted20mhz_subframe8,
  boosted20mhz_subframe9
};

const U8 **E_TM_1_2MapIdx[6] =
{
    boosted1_4MhzMapIndex,
    boosted3MhzMapIndex,
    boosted5MhzMapIndex,
    boosted10MhzMapIndex,
    NULL,
    boosted20MhzMapIndex
};

const U8 **E_TM_1_2MapIdxDeBoosted[6] =
{
    deboosted1_4MhzMapIndex,
    boosted3MhzMapIndex,
    boosted5MhzMapIndex,
    boosted10MhzMapIndex,
    NULL,
    boosted20MhzMapIndex
};

//-----------------------------------------------------           E-TM 6.2         -----------------------------------------------------

//1_4Mhz
static const U8 allocated1_4mhz_subframe0[] = { 4, 0xFF };
static const U8 allocated1_4mhz_subframe1[] = { 1, 0xFF };
static const U8 allocated1_4mhz_subframe2[] = { 1, 0xFF };
static const U8 allocated1_4mhz_subframe3[] = { 2, 0xFF };
static const U8 allocated1_4mhz_subframe4[] = { 5, 0xFF };
static const U8 allocated1_4mhz_subframe5[] = { 3, 0xFF };
static const U8 allocated1_4mhz_subframe6[] = { 0, 0xFF };
static const U8 allocated1_4mhz_subframe7[] = { 0, 0xFF };
static const U8 allocated1_4mhz_subframe8[] = { 5, 0xFF };
static const U8 allocated1_4mhz_subframe9[] = { 4, 0xFF };

static const U8 *allocated1_4MhzMapIndex[10] =
{
  allocated1_4mhz_subframe0,
  allocated1_4mhz_subframe1,
  allocated1_4mhz_subframe2,
  allocated1_4mhz_subframe3,
  allocated1_4mhz_subframe4,
  allocated1_4mhz_subframe5,
  allocated1_4mhz_subframe6,
  allocated1_4mhz_subframe7,
  allocated1_4mhz_subframe8,
  allocated1_4mhz_subframe9
};

//3Mhz
static const U8 allocated3mhz_subframe0[] = { 13, 0xFF };
static const U8 allocated3mhz_subframe1[] = { 11, 0xFF };
static const U8 allocated3mhz_subframe2[] = { 13, 0xFF };
static const U8 allocated3mhz_subframe3[] = { 5,  0xFF };
static const U8 allocated3mhz_subframe4[] = { 9,  0xFF };
static const U8 allocated3mhz_subframe5[] = { 14, 0xFF };
static const U8 allocated3mhz_subframe6[] = { 6,  0xFF };
static const U8 allocated3mhz_subframe7[] = { 13, 0xFF };
static const U8 allocated3mhz_subframe8[] = { 0,  0xFF };
static const U8 allocated3mhz_subframe9[] = { 1,  0xFF };

static const U8 *allocated3MhzMapIndex[10] =
{
  allocated3mhz_subframe0,
  allocated3mhz_subframe1,
  allocated3mhz_subframe2,
  allocated3mhz_subframe3,
  allocated3mhz_subframe4,
  allocated3mhz_subframe5,
  allocated3mhz_subframe6,
  allocated3mhz_subframe7,
  allocated3mhz_subframe8,
  allocated3mhz_subframe9
};

//5Mhz
static const U8 allocated5mhz_subframe0[] = { 8,  0xFF };
static const U8 allocated5mhz_subframe1[] = { 17, 0xFF };
static const U8 allocated5mhz_subframe2[] = { 21, 0xFF };
static const U8 allocated5mhz_subframe3[] = { 8,  0xFF };
static const U8 allocated5mhz_subframe4[] = { 22, 0xFF };
static const U8 allocated5mhz_subframe5[] = { 2,  0xFF };
static const U8 allocated5mhz_subframe6[] = { 9,  0xFF };
static const U8 allocated5mhz_subframe7[] = { 14, 0xFF };
static const U8 allocated5mhz_subframe8[] = { 0,  0xFF };
static const U8 allocated5mhz_subframe9[] = { 13, 0xFF };

static const U8 *allocated5MhzMapIndex[10] =
{
  allocated5mhz_subframe0,
  allocated5mhz_subframe1,
  allocated5mhz_subframe2,
  allocated5mhz_subframe3,
  allocated5mhz_subframe4,
  allocated5mhz_subframe5,
  allocated5mhz_subframe6,
  allocated5mhz_subframe7,
  allocated5mhz_subframe8,
  allocated5mhz_subframe9
};

//10Mhz
static const U8 allocated10mhz_subframe0[] = { 16, 0xFF };
static const U8 allocated10mhz_subframe1[] = { 36, 0xFF };
static const U8 allocated10mhz_subframe2[] = { 19, 0xFF };
static const U8 allocated10mhz_subframe3[] = { 26, 0xFF };
static const U8 allocated10mhz_subframe4[] = { 42, 0xFF };
static const U8 allocated10mhz_subframe5[] = { 30, 0xFF };
static const U8 allocated10mhz_subframe6[] = { 17, 0xFF };
static const U8 allocated10mhz_subframe7[] = { 48, 0xFF };
static const U8 allocated10mhz_subframe8[] = { 9,  0xFF };
static const U8 allocated10mhz_subframe9[] = { 0,  0xFF };

static const U8 *allocated10MhzMapIndex[10] =
{
  allocated10mhz_subframe0,
  allocated10mhz_subframe1,
  allocated10mhz_subframe2,
  allocated10mhz_subframe3,
  allocated10mhz_subframe4,
  allocated10mhz_subframe5,
  allocated10mhz_subframe6,
  allocated10mhz_subframe7,
  allocated10mhz_subframe8,
  allocated10mhz_subframe9
};

//20Mhz
static const U8 allocated20mhz_subframe0[] = { 63, 0xFF };
static const U8 allocated20mhz_subframe1[] = { 34, 0xFF };
static const U8 allocated20mhz_subframe2[] = { 44, 0xFF };
static const U8 allocated20mhz_subframe3[] = { 7,  0xFF };
static const U8 allocated20mhz_subframe4[] = { 94, 0xFF };
static const U8 allocated20mhz_subframe5[] = { 2,  0xFF };
static const U8 allocated20mhz_subframe6[] = { 97, 0xFF };
static const U8 allocated20mhz_subframe7[] = { 19, 0xFF };
static const U8 allocated20mhz_subframe8[] = { 56, 0xFF };
static const U8 allocated20mhz_subframe9[] = { 32, 0xFF };

static const U8 *allocated20MhzMapIndex[10] =
{
  allocated20mhz_subframe0,
  allocated20mhz_subframe1,
  allocated20mhz_subframe2,
  allocated20mhz_subframe3,
  allocated20mhz_subframe4,
  allocated20mhz_subframe5,
  allocated20mhz_subframe6,
  allocated20mhz_subframe7,
  allocated20mhz_subframe8,
  allocated20mhz_subframe9
};

const U8 **E_TM_2MapIdx[6] =
{
    allocated1_4MhzMapIndex,
    allocated3MhzMapIndex,
    allocated5MhzMapIndex,
    allocated10MhzMapIndex,
    NULL,
    allocated20MhzMapIndex
};

//-----------------------------------------------------          E-TM 6.3.1        -----------------------------------------------------

const U8 **E_TM_3_1MapIdx[6] =
{
    noboosted1_4MhzMapIndex,
    noboosted3MhzMapIndex,
    noboosted5MhzMapIndex,
    noboosted10MhzMapIndex,
    NULL,
    noboosted20MhzMapIndex
};


//-----------------------------------------------------          E-TM 6.3.2        -----------------------------------------------------

//1_4Mhz
static const U8 qam16_1_4mhz_subframe0[] = { 1, 3, 4, 5, 0xFF };
static const U8 qam16_1_4mhz_subframe1[] = { 1, 2, 3, 5, 0xFF };
static const U8 qam16_1_4mhz_subframe2[] = { 0, 1, 3, 4, 0xFF };
static const U8 qam16_1_4mhz_subframe3[] = { 1, 2, 3, 5, 0xFF };
static const U8 qam16_1_4mhz_subframe4[] = { 0, 2, 3, 5, 0xFF };
static const U8 qam16_1_4mhz_subframe5[] = { 0, 1, 2, 4, 0xFF };
static const U8 qam16_1_4mhz_subframe6[] = { 0, 1, 2, 5, 0xFF };
static const U8 qam16_1_4mhz_subframe7[] = { 0, 2, 4, 5, 0xFF };
static const U8 qam16_1_4mhz_subframe8[] = { 1, 2, 3, 5, 0xFF };
static const U8 qam16_1_4mhz_subframe9[] = { 0, 1, 3, 5, 0xFF };

static const U8 *qam16_1_4MhzMapIndex[10] =
{
  qam16_1_4mhz_subframe0,
  qam16_1_4mhz_subframe1,
  qam16_1_4mhz_subframe2,
  qam16_1_4mhz_subframe3,
  qam16_1_4mhz_subframe4,
  qam16_1_4mhz_subframe5,
  qam16_1_4mhz_subframe6,
  qam16_1_4mhz_subframe7,
  qam16_1_4mhz_subframe8,
  qam16_1_4mhz_subframe9
};

//3Mhz
static const U8 qam16_3mhz_subframe0[] = { 0, 1, 2, 3,  11, 12, 13, 0xFF };
static const U8 qam16_3mhz_subframe1[] = { 0, 3, 4, 10, 11, 12, 13, 0xFF };
static const U8 qam16_3mhz_subframe2[] = { 0, 2, 3, 5,  6,  11, 13, 0xFF };
static const U8 qam16_3mhz_subframe3[] = { 0, 1, 4, 5,  7,  10, 12, 0xFF };
static const U8 qam16_3mhz_subframe4[] = { 0, 2, 3, 4,  9,  10, 13, 0xFF };
static const U8 qam16_3mhz_subframe5[] = { 0, 1, 2, 3,  11, 12, 14, 0xFF };
static const U8 qam16_3mhz_subframe6[] = { 4, 5, 6, 8,  11, 13, 14, 0xFF };
static const U8 qam16_3mhz_subframe7[] = { 2, 5, 6, 9,  12, 13, 14, 0xFF };
static const U8 qam16_3mhz_subframe8[] = { 0, 3, 4, 7,  8,  9,  11, 0xFF };
static const U8 qam16_3mhz_subframe9[] = { 1, 2, 3, 4,  5,  11, 12, 0xFF };

static const U8 *qam16_3MhzMapIndex[10] =
{
  qam16_3mhz_subframe0,
  qam16_3mhz_subframe1,
  qam16_3mhz_subframe2,
  qam16_3mhz_subframe3,
  qam16_3mhz_subframe4,
  qam16_3mhz_subframe5,
  qam16_3mhz_subframe6,
  qam16_3mhz_subframe7,
  qam16_3mhz_subframe8,
  qam16_3mhz_subframe9
};

//5Mhz
static const U8 qam16_5mhz_subframe0[] = 
{ 0, 1, 3, 4, 6,  7,  8,  16, 17, 18, 19, 20, 21, 23, 24, 0xFF };
static const U8 qam16_5mhz_subframe1[] = 
{ 0, 1, 2, 3, 4,  5,  6,  9,  10, 12, 13, 17, 18, 20, 24, 0xFF };
static const U8 qam16_5mhz_subframe2[] = 
{ 0, 1, 2, 3, 7,  8,  9,  10, 12, 13, 14, 19, 20, 23, 24, 0xFF };
static const U8 qam16_5mhz_subframe3[] = 
{ 0, 5, 6, 8, 10, 11, 12, 13, 15, 17, 18, 20, 21, 22, 24, 0xFF };
static const U8 qam16_5mhz_subframe4[] = 
{ 0, 1, 2, 4, 6,  7,  12, 13, 14, 15, 16, 17, 22, 23, 24, 0xFF };
static const U8 qam16_5mhz_subframe5[] = 
{ 0, 1, 2, 3, 4,  6,  7,  8,  16, 17, 18, 21, 22, 23, 24, 0xFF };
static const U8 qam16_5mhz_subframe6[] = 
{ 1, 3, 4, 5, 7,  9, 10,  11, 12, 13, 14, 15, 21, 22, 24, 0xFF };
static const U8 qam16_5mhz_subframe7[] = 
{ 0, 1, 2, 3, 4,  7,  8,  10, 13, 14, 18, 19, 20, 21, 24, 0xFF };
static const U8 qam16_5mhz_subframe8[] = 
{ 1, 4, 8, 9, 10, 11, 12, 13, 15, 16, 18, 20, 22, 23, 24, 0xFF };
static const U8 qam16_5mhz_subframe9[] = 
{ 1, 2, 3, 4, 5,  6,  9,  10, 11, 12, 13, 16, 17, 21, 23, 0xFF };

static const U8 *qam16_5MhzMapIndex[10] =
{
  qam16_5mhz_subframe0,
  qam16_5mhz_subframe1,
  qam16_5mhz_subframe2,
  qam16_5mhz_subframe3,
  qam16_5mhz_subframe4,
  qam16_5mhz_subframe5,
  qam16_5mhz_subframe6,
  qam16_5mhz_subframe7,
  qam16_5mhz_subframe8,
  qam16_5mhz_subframe9
};

//10Mhz
static const U8 qam16_10mhz_subframe0[] =
{ 
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 15, 16, 17, 20, 21, 28, 30, 31 ,32, 33, 35, 36, 39, 40, 42, 
    44, 46, 47, 48, 0xFF 
};
static const U8 qam16_10mhz_subframe1[] =
{ 
    0, 1, 2, 4, 5, 6, 7, 9, 10, 11, 13, 15, 18, 20, 21, 22, 24, 25, 27, 28, 29, 34, 35, 36, 37, 40, 
    43, 44, 46, 49, 0xFF 
};
static const U8 qam16_10mhz_subframe2[] = 
{ 
    0, 1, 3, 4, 5, 6, 7, 11, 12, 14, 17, 18, 19, 20, 21, 22, 24, 25, 26, 27, 28, 29, 31, 32, 34, 
    38, 41, 42, 45, 49, 0xFF 
};
static const U8 qam16_10mhz_subframe3[] =
{ 
    0, 1, 2, 3, 5, 6, 8, 12, 14, 15, 16, 17, 18, 22, 23, 26, 28, 29, 30, 32, 34, 35, 38, 39, 40, 
    41, 42, 45, 46, 47, 0xFF 
};
static const U8 qam16_10mhz_subframe4[] = 
{ 
    0, 3, 6, 7, 8, 9, 10, 12, 13, 15, 16, 17, 18, 21, 23, 25, 28, 31, 33, 37, 38, 39, 41, 42, 44, 
    45, 46, 47, 48, 49, 0xFF 
};
static const U8 qam16_10mhz_subframe5[] =
{ 
    0, 2, 3, 4, 5, 7, 9, 10, 11, 12, 13, 14, 15, 19, 20, 28, 29, 30, 31, 34, 36, 37, 38, 39, 40, 
    42, 44, 45, 48, 49, 0xFF 
};
static const U8 qam16_10mhz_subframe6[] =
{ 
    0, 1, 3, 4, 5, 6, 8, 9, 10, 13, 14, 16, 17, 18, 19, 20, 21, 23, 24, 29, 30, 31, 32, 35, 37, 38, 
    39, 40, 47, 48, 0xFF 
};
static const U8 qam16_10mhz_subframe7[] =
{  
    0, 1, 2, 3, 4, 5, 6, 7, 9, 10, 12, 14, 16, 17, 18, 19, 22, 24, 25, 26, 27, 28, 30, 31, 32, 34, 
    37, 42, 45, 48, 0xFF 
};
static const U8 qam16_10mhz_subframe8[] =
{ 
    2, 5, 7, 8, 9, 10, 11, 14, 15, 16, 17, 21, 22, 27, 28, 29, 30, 31, 32, 34, 35, 37, 38, 41, 42, 
    43, 44, 46, 48, 49, 0xFF 
};
static const U8 qam16_10mhz_subframe9[] =
{ 
    1, 3, 6, 9, 11, 13, 15, 16, 17, 18, 21, 24, 25, 26, 27, 28, 29, 30, 32, 34, 37, 38, 39, 40, 41, 
    42, 45, 47, 48, 49, 0xFF 
};

static const U8 *qam16_10MhzMapIndex[10] =
{
  qam16_10mhz_subframe0,
  qam16_10mhz_subframe1,
  qam16_10mhz_subframe2,
  qam16_10mhz_subframe3,
  qam16_10mhz_subframe4,
  qam16_10mhz_subframe5,
  qam16_10mhz_subframe6,
  qam16_10mhz_subframe7,
  qam16_10mhz_subframe8,
  qam16_10mhz_subframe9
};

//20Mhz
static const U8 qam16_20mhz_subframe0[] =
{ 
    0, 1, 4, 6, 7, 8, 9, 10, 11, 13, 14, 15, 16, 20, 21, 22, 23, 25, 26, 28, 29, 30, 31, 32, 33, 
    34, 36, 39, 41, 42, 44, 45, 54, 56, 57, 58, 60, 61, 63, 66, 67, 68, 72, 75, 76, 77, 79, 81, 82, 
    84, 85, 87, 88, 91, 92, 94, 95, 97, 98, 99, 0xFF 
};
static const U8 qam16_20mhz_subframe1[] =
{ 
    1, 3, 5, 6, 7, 9, 10, 13, 17, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 30, 32, 33, 34, 35, 36, 
    39, 41, 47, 48, 49, 50, 51, 53, 54, 55, 57, 58, 59, 60, 61, 64, 65, 67, 68, 75, 76, 77, 79, 80, 
    81, 83, 84, 86, 87, 89, 90, 91, 93, 95, 99, 0xFF 
};
static const U8 qam16_20mhz_subframe2[] =
{ 
    0, 1, 2, 3, 6, 8, 10, 11, 15, 16, 17, 19, 21, 22, 23, 25, 26, 28, 29, 30, 32, 33, 35, 38, 39, 
    40, 41, 42, 43, 44, 46, 49, 51, 52, 53, 54, 57, 59, 60, 62, 63, 64, 65, 67, 69, 71, 72, 73, 76, 
    79, 81, 84, 86, 88, 89, 90, 92, 93, 94, 99, 0xFF 
};
static const U8 qam16_20mhz_subframe3[] =
{ 
    5, 6, 7, 9 ,10, 12, 14, 15, 16, 17, 21, 22, 23, 24, 27, 28, 29, 30, 31, 33, 34, 35, 36, 37, 39, 
    41, 44, 45, 47, 49, 50, 53, 54, 55, 56, 57, 59, 64, 65, 66, 68, 70, 72, 75, 76, 77, 80, 81, 84, 
    85, 86, 87, 90, 91, 92, 94, 95, 97, 98, 99, 0xFF 
};
static const U8 qam16_20mhz_subframe4[] =
{
    0, 2, 3, 4, 5, 6, 7, 11, 12, 14, 15, 17, 19, 21, 22, 24, 26, 32, 36, 37, 40, 42, 43, 44, 47, 
    48, 49, 50, 51, 54, 56, 57, 60, 62, 63, 65, 66, 67, 70, 71, 73, 76, 77, 78, 79, 81, 82, 83, 84, 
    85, 86, 87, 89, 91, 94, 95, 96, 97, 98, 99, 0xFF 
};
static const U8 qam16_20mhz_subframe5[] =
{ 
    1, 2, 5, 6, 8, 9, 11, 12, 13, 15, 21, 22, 25, 26, 27, 28, 29, 30, 31, 32, 34, 35, 38, 39, 40, 
    41, 43, 44, 45, 46, 53, 57, 58, 59, 61, 62, 63, 64, 65, 66, 68, 69, 71, 72, 73, 75, 77, 78, 80, 
    82, 83, 84, 85, 86, 93, 94, 95, 96, 97, 99, 0xFF 
};
static const U8 qam16_20mhz_subframe6[] =
{ 
    0, 1, 2, 3, 5, 8, 9, 10, 12, 13, 14, 16, 22, 25, 26, 27, 28, 29, 31, 32, 33, 34, 36, 37, 38, 
    39, 43, 44, 45, 46, 48, 49, 52, 53, 55, 59, 61, 62, 63, 64, 70, 71, 72, 73, 74, 75, 77, 78, 80, 
    81, 82, 84, 86, 89, 90, 91, 93, 97, 98, 99, 0xFF 
};
static const U8 qam16_20mhz_subframe7[] =
{ 
    0, 1, 3, 4, 5, 7, 8, 10, 11, 15, 18, 19, 20, 21, 26, 27, 29, 30, 31, 33, 35, 37, 38, 39, 40, 
    41, 43, 44, 45, 46, 47, 48, 49, 50, 52, 53, 55, 56, 58, 60, 62, 64, 65, 66, 67, 69, 70, 71, 72, 
    73, 74, 81, 83, 84, 86, 92, 93, 94, 96, 98, 0xFF 
};
static const U8 qam16_20mhz_subframe8[] =
{ 
    2, 3, 4, 5, 7, 9, 11, 13, 15, 16, 17, 21, 23, 24, 25, 27, 28, 29, 31, 33, 35, 36, 40, 42, 43, 
    44, 45, 46, 48, 49, 51, 52, 53, 54, 55, 56, 57, 59, 61, 63, 64, 65, 68, 71, 76, 77, 78, 81, 82, 
    83, 84, 85, 86, 87, 90, 91, 93, 94, 98, 99, 0xFF 
};
static const U8 qam16_20mhz_subframe9[] =
{ 
    0, 3, 4, 6, 7, 8, 10, 11, 13, 16, 18, 21, 22, 23, 25, 26, 28, 29, 32, 35, 36, 37, 38, 43, 44, 
    46, 47, 48, 49, 53, 54, 57, 58, 59, 60, 61, 64, 66, 67, 68, 69, 70, 72, 76, 77, 78, 80, 81, 82, 
    83, 84, 86, 87, 88, 89, 91, 92, 94, 95, 96, 0xFF 
};

static const U8 *qam16_20MhzMapIndex[10] =
{
  qam16_20mhz_subframe0,
  qam16_20mhz_subframe1,
  qam16_20mhz_subframe2,
  qam16_20mhz_subframe3,
  qam16_20mhz_subframe4,
  qam16_20mhz_subframe5,
  qam16_20mhz_subframe6,
  qam16_20mhz_subframe7,
  qam16_20mhz_subframe8,
  qam16_20mhz_subframe9
};


const U8 **E_TM_3_2MapIdx[6] =
{
    qam16_1_4MhzMapIndex,
    qam16_3MhzMapIndex,
    qam16_5MhzMapIndex,
    qam16_10MhzMapIndex,
    NULL,
    qam16_20MhzMapIndex
};

//-----------------------------------------------------          E-TM 6.3.3        -----------------------------------------------------

//1_4Mhz
static const U8 qpsk_1_4mhz_subframe0[] = { 2, 3, 5, 0xFF };
static const U8 qpsk_1_4mhz_subframe1[] = { 1, 2, 3, 0xFF };
static const U8 qpsk_1_4mhz_subframe2[] = { 0, 1, 3, 0xFF };
static const U8 qpsk_1_4mhz_subframe3[] = { 1, 2, 3, 0xFF };
static const U8 qpsk_1_4mhz_subframe4[] = { 2, 3, 5, 0xFF };
static const U8 qpsk_1_4mhz_subframe5[] = { 1, 2, 5, 0xFF };
static const U8 qpsk_1_4mhz_subframe6[] = { 0, 2, 5, 0xFF };
static const U8 qpsk_1_4mhz_subframe7[] = { 0, 2, 5, 0xFF };
static const U8 qpsk_1_4mhz_subframe8[] = { 1, 2, 5, 0xFF };
static const U8 qpsk_1_4mhz_subframe9[] = { 1, 3, 5, 0xFF };

static const U8 *qpsk_1_4MhzMapIndex[10] =
{
  qpsk_1_4mhz_subframe0,
  qpsk_1_4mhz_subframe1,
  qpsk_1_4mhz_subframe2,
  qpsk_1_4mhz_subframe3,
  qpsk_1_4mhz_subframe4,
  qpsk_1_4mhz_subframe5,
  qpsk_1_4mhz_subframe6,
  qpsk_1_4mhz_subframe7,
  qpsk_1_4mhz_subframe8,
  qpsk_1_4mhz_subframe9
};

//3Mhz
static const U8 qpsk_3mhz_subframe0[] = { 0, 1, 2, 3,  11, 12, 13, 0xFF };
static const U8 qpsk_3mhz_subframe1[] = { 0, 3, 4, 10, 11, 12, 13, 0xFF };
static const U8 qpsk_3mhz_subframe2[] = { 0, 2, 3, 5,  6,  11, 13, 0xFF };
static const U8 qpsk_3mhz_subframe3[] = { 0, 1, 4, 5,  7,  10, 12, 0xFF };
static const U8 qpsk_3mhz_subframe4[] = { 0, 2, 3, 4,  9,  10, 13, 0xFF };
static const U8 qpsk_3mhz_subframe5[] = { 0, 1, 2, 3, 11,  12, 14, 0xFF };
static const U8 qpsk_3mhz_subframe6[] = { 4, 5, 6, 8, 11,  13, 14, 0xFF };
static const U8 qpsk_3mhz_subframe7[] = { 2, 5, 6, 9, 12,  13, 14, 0xFF };
static const U8 qpsk_3mhz_subframe8[] = { 0, 3, 4, 7,  8,   9, 11, 0xFF };
static const U8 qpsk_3mhz_subframe9[] = { 1, 2, 3, 4,  5,  11, 12, 0xFF };

static const U8 *qpsk_3MhzMapIndex[10] =
{
  qpsk_3mhz_subframe0,
  qpsk_3mhz_subframe1,
  qpsk_3mhz_subframe2,
  qpsk_3mhz_subframe3,
  qpsk_3mhz_subframe4,
  qpsk_3mhz_subframe5,
  qpsk_3mhz_subframe6,
  qpsk_3mhz_subframe7,
  qpsk_3mhz_subframe8,
  qpsk_3mhz_subframe9
};

//5Mhz
static const U8 qpsk_5mhz_subframe0[] = { 0, 1, 3, 6, 7,  8,  16, 17, 18, 20, 21, 23, 24, 0xFF };
static const U8 qpsk_5mhz_subframe1[] = { 0, 1, 2, 4, 5,  6,  9,  10, 12, 17, 18, 20, 24, 0xFF };
static const U8 qpsk_5mhz_subframe2[] = { 0, 1, 2, 3, 9,  10, 12, 13, 14, 19, 20, 23, 24, 0xFF };
static const U8 qpsk_5mhz_subframe3[] = { 0, 5, 6, 8, 10, 12, 13, 15, 17, 18, 20, 21, 24, 0xFF };
static const U8 qpsk_5mhz_subframe4[] = { 0, 2, 4, 6, 7,  12, 13, 15, 16, 17, 22, 23, 24, 0xFF };
static const U8 qpsk_5mhz_subframe5[] = { 0, 1, 2, 3, 4,  6,  7,  8,  16, 18, 21, 22, 24, 0xFF };
static const U8 qpsk_5mhz_subframe6[] = { 1, 3, 4, 5, 7,  9,  10, 11, 12, 15, 21, 22, 24, 0xFF };
static const U8 qpsk_5mhz_subframe7[] = { 0, 1, 2, 3, 4,  7,  10, 14, 18, 19, 20, 21, 24, 0xFF };
static const U8 qpsk_5mhz_subframe8[] = { 1, 4, 8, 9, 10, 11, 12, 13, 15, 16, 18, 20, 23, 0xFF };
static const U8 qpsk_5mhz_subframe9[] = { 1, 2, 3, 4, 5,  6,  9,  10, 11, 13, 16, 17, 23, 0xFF };

static const U8 *qpsk_5MhzMapIndex[10] =
{
  qpsk_5mhz_subframe0,
  qpsk_5mhz_subframe1,
  qpsk_5mhz_subframe2,
  qpsk_5mhz_subframe3,
  qpsk_5mhz_subframe4,
  qpsk_5mhz_subframe5,
  qpsk_5mhz_subframe6,
  qpsk_5mhz_subframe7,
  qpsk_5mhz_subframe8,
  qpsk_5mhz_subframe9
};

//10Mhz
static const U8 qpsk_10mhz_subframe0[] =
{ 
    1, 2, 3, 5, 6, 7, 8, 9, 10, 11, 15, 16, 20, 28, 31, 32, 33, 35, 36, 39, 40, 42, 46, 47, 48, 
    0xFF 
};
static const U8 qpsk_10mhz_subframe1[] =
{ 
    1, 2, 4, 5, 6, 7, 9, 11, 15, 18, 20, 21, 22, 24, 25, 27, 29, 34, 35, 36, 37, 40, 44, 46, 
    49, 0xFF 
};
static const U8 qpsk_10mhz_subframe2[] =
{ 
    0, 3, 5, 6, 11, 12, 14, 17, 18, 19, 20, 21, 22, 24, 25, 26, 27, 28, 29, 31, 34, 38, 41, 42, 49, 
    0xFF 
};
static const U8 qpsk_10mhz_subframe3[] =
{
    0, 1, 2, 3, 5, 6, 8, 14, 16, 18, 22, 23, 26, 28, 30, 32, 34, 38, 39, 40, 41, 42, 45, 46, 
    47, 0xFF 
};
static const U8 qpsk_10mhz_subframe4[] =
{ 
    0, 3, 6, 7, 8, 9, 10, 12, 13, 16, 17, 18, 21, 23, 25, 31, 33, 37, 41, 42, 45, 46, 47, 48, 49, 
    0xFF 
};
static const U8 qpsk_10mhz_subframe5[] =
{ 
    0, 2, 3, 4, 5, 7, 9, 10, 11, 12, 13, 15, 19, 20, 28, 29, 30, 31, 34, 36, 37, 42, 44, 48, 
    49, 0xFF 
};
static const U8 qpsk_10mhz_subframe6[] = 
{ 
    0, 1, 4, 5, 6, 8, 9, 10, 13, 16, 17, 18, 19, 20, 21, 24, 29, 30, 31, 32, 35, 37, 38, 39, 
    47, 0xFF 
};
static const U8 qpsk_10mhz_subframe7[] =
{ 
    0, 2, 3, 4, 5, 6, 7, 9, 10, 12, 16, 17, 18, 19, 22, 24, 25, 26, 30, 31, 34, 37, 42, 45, 48, 
    0xFF
};
static const U8 qpsk_10mhz_subframe8[] =
{ 
    5, 7, 8, 9, 14, 15, 16, 21, 22, 27, 28, 30, 31, 32, 34, 35, 37, 38, 41, 42, 43, 44, 46, 48, 49, 
    0xFF 
};
static const U8 qpsk_10mhz_subframe9[] =
{
    3, 9, 11, 13, 16, 17, 18, 21, 24, 27, 28, 29, 30, 32, 34, 37, 38, 39, 40, 41, 42, 45, 47, 48, 
    49, 0xFF 
};

static const U8 *qpsk_10MhzMapIndex[10] =
{
  qpsk_10mhz_subframe0,
  qpsk_10mhz_subframe1,
  qpsk_10mhz_subframe2,
  qpsk_10mhz_subframe3,
  qpsk_10mhz_subframe4,
  qpsk_10mhz_subframe5,
  qpsk_10mhz_subframe6,
  qpsk_10mhz_subframe7,
  qpsk_10mhz_subframe8,
  qpsk_10mhz_subframe9
};

//20Mhz
static const U8 qpsk_20mhz_subframe0[] =
{ 
    0, 1, 4, 6, 10, 13, 14, 15, 16, 20, 22, 23, 25, 26, 28, 29, 30, 31, 32, 33, 36, 39, 41, 42, 44, 
    45, 54, 56, 57, 60, 63, 66, 67, 68, 72, 76, 77, 79, 82, 84, 85, 87, 88, 91, 92, 94, 95, 97, 98,
    99, 0xFF 
};
static const U8 qpsk_20mhz_subframe1[] =
{
    1, 3, 7, 9, 10, 13, 19, 20, 21, 22, 23, 24, 25, 26, 27, 30, 33, 34, 35, 36, 47, 48, 49, 50, 51, 
    53, 54, 55, 57, 59, 60, 61, 64, 65, 67, 68, 75, 76, 77, 80, 81, 83, 84, 86, 87, 89, 90, 93, 95,
    99, 0xFF 
};
static const U8 qpsk_20mhz_subframe2[] =
{
    1, 2, 3, 6, 8, 10, 11, 15, 16, 17, 19, 21, 25, 26, 28, 29, 30, 32, 33, 35, 38, 39, 40, 41, 42, 
    43, 44, 46, 49, 51, 52, 54, 60, 62, 63, 64, 65, 69, 72, 76, 79, 81, 84, 86, 88, 89, 90, 93, 94,
    99, 0xFF 
};
static const U8 qpsk_20mhz_subframe3[] =
{
    5, 7, 9, 10, 12, 14, 15, 16, 21, 22, 23, 24, 27, 28, 29, 30, 33, 34, 35, 36, 37, 39, 41, 44,
    45, 47, 49, 54, 55, 56, 57, 64, 66, 68, 70, 72, 76, 77, 80, 81, 85, 86, 87, 90, 91, 92, 94, 95,
    98, 99, 0xFF 
};
static const U8 qpsk_20mhz_subframe4[] =
{
    2, 3, 4, 5, 6, 7, 14, 15, 17, 19, 21, 22, 24, 26, 37, 40, 42, 43, 44, 47, 49, 51, 54, 56, 57,
    60, 62, 63, 65, 66, 67, 70, 71, 73, 76, 77, 78, 81, 82, 83, 84, 85, 86, 87, 89, 94, 95, 96, 97,
    99, 0xFF 
};
static const U8 qpsk_20mhz_subframe5[] =
{
    1, 2, 5, 6, 8, 9, 12, 13, 21, 22, 25, 26, 27, 28, 29, 31, 32, 34, 35, 39, 40, 43, 45, 46, 53,
    57, 59, 61, 62, 63, 64, 66, 68, 69, 71, 73, 75, 77, 78, 82, 83, 84, 85, 86, 93, 94, 95, 96, 97,
    99, 0xFF 
};
static const U8 qpsk_20mhz_subframe6[] =
{
    0, 1, 2, 3, 5, 8, 9, 10, 12, 13, 22, 25, 26, 27, 29, 31, 32, 33, 36, 37, 38, 39, 43, 45, 48, 
    49, 52, 53, 55, 59, 62, 63, 64, 71, 72, 73, 74, 75, 77, 78, 81, 82, 84, 86, 89, 91, 93, 97, 98,
    99, 0xFF 
};
static const U8 qpsk_20mhz_subframe7[] =
{
    0, 1, 3, 4, 5, 7, 10, 11, 15, 18, 19, 20, 21, 26, 27, 29, 30, 31, 33, 35, 39, 40, 41, 43, 44,
    46, 47, 49, 50, 53, 55, 56, 62, 64, 65, 66, 67, 69, 70, 71, 72, 74, 83, 84, 86, 92, 93, 94, 96,
    98, 0xFF 
};
static const U8 qpsk_20mhz_subframe8[] =
{
    2, 3, 4, 7, 9, 11, 13, 15, 16, 24, 25, 27, 29, 31, 33, 35, 36, 40, 43, 44, 45, 46, 49, 51, 52,
    53, 54, 55, 56, 57, 59, 63, 64, 65, 68, 71, 77, 78, 81, 82, 83, 84, 85, 86, 90, 91, 93, 94, 98,
    99, 0xFF 
};
static const U8 qpsk_20mhz_subframe9[] =
{
    0, 4, 6, 7, 8, 10, 11, 13, 16, 18, 21, 22, 23, 26, 29, 32, 35, 36, 37, 43, 44, 46, 47, 48, 49,
    53, 54, 57, 59, 60, 61, 64, 66, 67, 68, 69, 70, 72, 76, 78, 80, 81, 82, 84, 87, 89, 91, 92, 95,
    96, 0xFF 
};

static const U8 *qpsk_20MhzMapIndex[10] =
{
  qpsk_20mhz_subframe0,
  qpsk_20mhz_subframe1,
  qpsk_20mhz_subframe2,
  qpsk_20mhz_subframe3,
  qpsk_20mhz_subframe4,
  qpsk_20mhz_subframe5,
  qpsk_20mhz_subframe6,
  qpsk_20mhz_subframe7,
  qpsk_20mhz_subframe8,
  qpsk_20mhz_subframe9
};

const U8 **E_TM_3_3MapIdx[6] =
{
    qpsk_1_4MhzMapIndex,
    qpsk_3MhzMapIndex,
    qpsk_5MhzMapIndex,
    qpsk_10MhzMapIndex,
    NULL,
    qpsk_20MhzMapIndex
};

