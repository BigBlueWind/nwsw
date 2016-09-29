/** @file e_tm.h
 *
 * @brief Interface file to E-TM library
 * @author Mindspeed Technologies
 * @version $Revision: 1.2 $
 *
 * COPYRIGHT(c) 2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef E_TM_H
#define E_TM_H

#include "LtePhyL2Api.h"

/*
    E_TM_INT_MODEL - enumeration of all E-TM models per 36.141 [6.x] (need to use internal into mac-project)
*/
typedef enum E_TM_INT_MODEL_ENUM
{
    E_TM_INT_UNDEFINED = 0,
    E_TM_INT_1_1,
    E_TM_INT_1_2,
    E_TM_INT_2,
    E_TM_INT_3_1,
    E_TM_INT_3_2,
    E_TM_INT_3_3,

    BE_INT_TEST_1,
    BE_INT_TEST_2,
    SWEEP_INT_TEST_1
}E_TM_INT_MODEL;

/*
    E_TM_CONFIG - configuration for E-TM testing
*/
typedef struct E_TM_CONFIG_STRUCT
{
    E_TM_INT_MODEL model;     // E-TM model
    enum ChBwOptions bw;      // Bandwidth    
    int    e_rs;              // Ers [dB]
    unsigned int ant0;        // 0 - diable Ant0, 1 - enable Ant0
    unsigned int ant1;        // 0 - diable Ant1, 1 - enable Ant1
    unsigned int num_layers;  // Number of layers
    unsigned int num_cw;      // Number of codewords
    unsigned int tdd;         // 0 - fdd, 1 - tdd (not in use now)
}E_TM_CONFIG;

/*
    init()   : init E-TM model testing
    cfg [in] : cfg - configuration for E-TM model
    return   : 0 - success, >0 - failure
*/
unsigned int etm_init(E_TM_CONFIG cfg);

/*
    get_vectors()     : get list of vectors to be sent to PHY
    phy_vectors [out] : address of pointer to all PHY vectors
    return              : 0 - success, >0 - failure
    
*/
unsigned int etm_get_vectors(MAC2PHY_QUEUE_EL **phy_vectors, unsigned int fn, unsigned int sn);

#endif // E_TM_H
