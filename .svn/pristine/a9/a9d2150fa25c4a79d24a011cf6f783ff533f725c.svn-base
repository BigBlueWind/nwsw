/** @file LteMacUeContext.c
 *
 * @brief Implemenataion of Ue MAC Context
 * @author Mindspeed Technologies
 * @version $Revision: 1.15 $
 *
 * COPYRIGHT(c) 2008-2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "osal.h"
#include "basetypes.h"
#include "resultcodes.h"
#include "LteMacUeContext.h"
//#include "queue.h"

/****************************************************************************
 Private Definitions
 ****************************************************************************/

/****************************************************************************
 Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/*****************************************************************************
 * Private Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/* Array of UE registred on this eNode. */
DlUeContext * ueDlContextArr[MAX_UE_COUNT];
UlUeContext * ueUlContextArr[MAX_UE_COUNT];

/* TTI tick counter (1ms) */
U32 globalTTITickCount = 0;

OSAL_MUTEX ueQueueLoadListLock;
UE_LOAD_LIST ueLoadList;


/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/

static OSAL_MEMARRAY ueDlContextPart;
static OSAL_MEMARRAY dlTxNodePart;

static OSAL_MEMARRAY ueUeLoadListPart;
extern OSAL_HEAP OsalGlobalHeap;


RESULTCODE UeContextClean(void)
{
    // TODO: clean

    return SUCCESS;
}

RESULTCODE UeContextInit(void)
{
    OSAL_PADDRESS pMemory;

    if (OsalAllocHeap(&OsalGlobalHeap, &pMemory,
                          (OSAL_SIZE)(sizeof(DlUeContext) * MAX_UE_COUNT))
                   != SUCCESS)
    {
        return FAILURE;
    }

    if (OsalCreateMemArray(&ueDlContextPart,
                           pMemory,
                          (OSAL_SIZE)(sizeof(DlUeContext) * MAX_UE_COUNT),
                          (OSAL_SIZE) sizeof(DlUeContext))
               != SUCCESS)
    {
        return FAILURE;
    }

    if (OsalAllocHeap(&OsalGlobalHeap, &pMemory,
                          (OSAL_SIZE)(sizeof(UlUeContext) * MAX_UE_COUNT))
                   != SUCCESS)
    {
        return FAILURE;
    }

    if (OsalCreateMemArray(&dlTxNodePart,
                           pMemory,
                          (OSAL_SIZE)(sizeof(UlUeContext) * MAX_UE_COUNT),
                          (OSAL_SIZE) sizeof(UlUeContext))
               != SUCCESS)
    {
        return FAILURE;
    }

    // load list init
    LIST_INIT(&ueLoadList);

    // allocate mamory for Load List requests
    if (OsalAllocHeap(&OsalGlobalHeap, &pMemory,
                          (OSAL_SIZE)(sizeof(UE_INDEX_LIST_NODE) * MAX_UE_COUNT))
                   != SUCCESS)
    {
        return FAILURE;
    }

    if (OsalCreateMemArray(&ueUeLoadListPart,
                           pMemory,
                          (OSAL_SIZE)(sizeof(UE_INDEX_LIST_NODE)* MAX_UE_COUNT),
                          (OSAL_SIZE) sizeof(UE_INDEX_LIST_NODE))
               != SUCCESS)
    {
        return FAILURE;
    }

    return SUCCESS;
}

DlUeContext* UeContextDlAlloc(void)
{
    OSAL_PADDRESS pBlock;

    if (OsalAllocMemArray(&ueDlContextPart, &pBlock) == SUCCESS)
        return (DlUeContext*)pBlock;
    else
        return NULL;
}

/*
RESULTCODE UeContextDlFree(DlUeContext* pDlUeContext)
{
    return OsalFreeMemArray(&ueDlContextPart,(OSAL_PADDRESS) pDlUeContext);
}

UlUeContext* UeContextUlAlloc(void)
{
    OSAL_PADDRESS pBlock;

    if (OsalAllocMemArray(&dlTxNodePart, &pBlock) == SUCCESS)
        return (UlUeContext*)pBlock;
    else
        return NULL;
}

RESULTCODE UeContextUlFree(UlUeContext* pUlUeContext)
{
    return OsalFreeMemArray(&dlTxNodePart,(OSAL_PADDRESS) pUlUeContext);
}

UE_INDEX_LIST_NODE* UeLoadListAlloc(void)
{
    OSAL_PADDRESS pBlock;

    if (OsalAllocMemArray(&ueUeLoadListPart, &pBlock) == SUCCESS)
        return (UE_INDEX_LIST_NODE*)pBlock;
    else
        return NULL;
}

RESULTCODE UeLoadListFree(UE_INDEX_LIST_NODE* pListBlock)
{
    return OsalFreeMemArray(&ueUeLoadListPart,(OSAL_PADDRESS) pListBlock);
}*/

