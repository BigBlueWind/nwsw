// -------------------------------------------------------------------------------------------
/** @file bs_listening.c
 *
 * @brief LTE FDD listening
 * @author Mindspeed Technologies
 * @version $Revision: 1.1.4.1.2.1 $
 *
 * COPYRIGHT&copy; 2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
// -------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#include "typedef.h"
#include "ltephydef.h"
#include "prototypes.h"
#include "ext_phy.h"
#include "lte_bs_phy_nmm_fdd.h"
#include "lte_bs_phy_nmm_radio_itf.h"


#ifdef CEVA_INTRINSICS
#include <basic_op.h>
#else
#include "basic_op.h"
#endif


EARFNC_item     EARFNC_TABLE[MAX_EUTRA_BAND + 1] = {
    /* band 0 */ {0, 0, 0, 0}
    ,
    /* band 1 */ {2110, 0, 0, 599}
    ,
    /* band 2 */ {1930, 600, 600, 1199}
    ,
    /* band 3 */ {1805, 1200, 1200, 1949}
    ,
    /* band 4 */ {2110, 1950, 1950, 2399}
    ,
    /* band 5 */ {869, 2400, 2400, 2649}
    ,
    /* band 6 */ {875, 2650, 2650, 2749}
    ,
    /* band 7 */ {2620, 2750, 2750, 3449}
    ,
    /* band 8 */ {925, 3450, 3450, 3799}
    ,
    /* band 9 */ {1844, 3800, 3800, 4149}
    ,
    /* band 10 */ {2110, 4150, 4150, 4749}
    ,
    /* band 11 */ {1475, 4750, 4750, 4949}
    ,
    /* band 12 */ {728, 5000, 5000, 5179}
    ,
    /* band 13 */ {746, 5180, 5180, 5279}
    ,
    /* band 14 */ {758, 5280, 5280, 5379}
    ,
    /* band 15 */ {0, 0, 0, 0}
    ,
    /* band 16 */ {0, 0, 0, 0}
    ,
    /* band 17 */ {734, 5730, 5730, 5849}
    ,
    /* band 18 */ {860, 5850, 5850, 5999}
    ,
    /* band 19 */ {875, 6000, 6000, 6149}
    ,
    /* band 20 */ {791, 6150, 6150, 6449}
    ,
    /* band 21 */ {1495, 6450, 6450, 6599}
    ,
    /* band 22 */ {0, 0, 0, 0}
    ,
    /* band 23 */ {0, 0, 0, 0}
    ,
    /* band 24 */ {0, 0, 0, 0}
    ,
    /* band 25 */ {0, 0, 0, 0}
    ,
    /* band 26 */ {0, 0, 0, 0}
    ,
    /* band 27 */ {0, 0, 0, 0}
    ,
    /* band 28 */ {0, 0, 0, 0}
    ,
    /* band 29 */ {0, 0, 0, 0}
    ,
    /* band 30 */ {0, 0, 0, 0}
    ,
    /* band 31 */ {0, 0, 0, 0}
    ,
    /* band 32 */ {0, 0, 0, 0}
    ,
    /* band 33 */ {1900, 36000, 36000, 36199}
    ,
    /* band 34 */ {2010, 36200, 36200, 36349}
    ,
    /* band 35 */ {1850, 36350, 36350, 36949}
    ,
    /* band 36 */ {1930, 36950, 36950, 37549}
    ,
    /* band 37 */ {1910, 37550, 37550, 37749}
    ,
    /* band 38 */ {2570, 37750, 37750, 38249}
    ,
    /* band 39 */ {1880, 38250, 38250, 38649}
    ,
    /* band 40 */ {2300, 38650, 38650, 39649}
    ,
    /* band 41 */ {2496, 39650, 39650, 41589}
    ,
};


void
earfcn_to_khz(U32 earfcn, U32 * freq, U8 * band)
{
    int             i;

    for (i = 1; i < MAX_EUTRA_BAND + 1; i++)
        if (earfcn >= EARFNC_TABLE[i].range_min && earfcn <= EARFNC_TABLE[i].range_max)
            break;
    if (band)
        *band = (U8) i;
    *freq = (1000 * EARFNC_TABLE[i].FDLlow + 100 * (earfcn - EARFNC_TABLE[i].Noffs_DL));
}


/*
 * dummy
 */
void
lte_bs_phy_nmm_radio_itf_query_available_band(U8 * band, U8 * nb_band)
{
    int             i = 0;
#define MAX_SUPPORTED_BAND 6

    *nb_band = MAX_SUPPORTED_BAND;
    band[i++] = 1;
    band[i++] = 2;
    band[i++] = 3;
    band[i++] = 5;
    band[i++] = 7;
    band[i++] = 8;
}


void
lte_bs_phy_nmm_radio_register_radio(void)
{
}

