/** @file LteTestTables.h
 *
 * @brief Header file to tables
 * @author Mindspeed Technologies
 * @version $Revision: 1.2 $
 *
 * COPYRIGHT(c) 2008-2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef _LTE_TEST_TABLES_H
#define _LTE_TEST_TABLES_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
//#include "resultcodes.h"
//#include "basetypes.h"
#include "ltemaccore.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/* RCT */
extern const U8 **E_TM_1_1MapIndex[6];
extern const U8 **E_TM_1_2MapIndex[6];
extern const U8 **E_TM_1_2MapIndexDeBoosted[6];
extern const U8 **E_TM_2MapIndex[6];
extern const U8 **E_TM_3_1MapIndex[6];
extern const U8 **E_TM_3_2MapIndex[6];
extern const U8 **E_TM_3_3MapIndex[6];
extern const U8 **RctULMapIndex[6];
extern const U8 **RctDLMapIndex[6];
extern const U8 **FdxULTestMapIndex[6];

/* FDX */
extern const U8 **FdxMapIndex[6];
extern const U8 **FdxULMapIndex[6];
extern const U8 **testMapIndex[6];
extern const U8 **HalfMapIndex[6];
extern const U8 **upperHalfMapIndex[6];

extern U8 TBSArr[MAX_MCS_INDEX];
extern U32 TBSizeArr[MAX_IBS_INDEX][MAX_RESOURCE_BLOCK];
extern U32 TBSizeArrSpaMux[28*4][2];
extern U8 testPDCCH[128];

/* TM500 */
extern const U8 Tm500DlRb[];
extern const U8 *Tm500DlMap[10];
extern const U8 Tm500UlRb[];
extern const U8 *Tm500UlMap[10];
extern S16 CA_srsSubframeConfigTable[];
extern U16 UESpecificSRSPeriodicityTable[][4];

extern U8 TBSArr[MAX_MCS_INDEX];
extern U32 TBSizeArr[MAX_IBS_INDEX][MAX_RESOURCE_BLOCK];
extern U8 testPDCCH[128];

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

#endif /* _LTE_TEST_TABLES_H */

