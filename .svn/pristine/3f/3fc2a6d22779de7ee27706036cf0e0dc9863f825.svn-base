/** @file MSMAC.h
 *
 * @brief Check API processing logic
 * @author Mindspeed Technologies
 * @version $Revision: 1.10 $
 *
 * COPYRIGHT(c) 2008 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef _MSMAC_H
#define _MSMAC_H

#include "LteMacUeContext.h"


#define MAX_NUMBER_OF_LOGICAL_CHANNEL      11

typedef enum
{
    UE_TEST_SCHEMA,
    UE_MAIN_SCHEMA, 
} UE_SCHEDULING_SCHEMA;
typedef struct _stDlUeContext_
{
    DlUeContext DlDesc;
    // Callbacks provided by upper layer
    MacRlcIoFunc    LteMacGetDataBlock;
    MacRlcIoFunc    LteMacPutDataBlock;
}UE_DL_CTX, *PUE_DL_CTX;

typedef struct _stUlUeContext_
{
    UlUeContext UlDesc;
     // Callbacks provided by upper layer
    MacRlcIoFunc    LteMacGetDataBlock;
    MacRlcIoFunc    LteMacPutDataBlock;

}UE_UL_CTX, *PUE_UL_CTX;

typedef struct MsDesc
{
	HANDLE pHeap;
    COMMON_DESC MacCommon;
    UE_SCHEDULING_SCHEMA schedulingSchema;
    /*indicates the C-RNTI assigned to this UE*/
    U16     ueId;
    // UE specific
   // UE_DESC* pUe;
    UE_DL_CTX DlCtx;
    UE_UL_CTX UlCtx;

} MS_DESC;
// This function is to process the received downlink Transport Block from BS
U32 GetUeId();

#endif /* _MSMAC_H */
