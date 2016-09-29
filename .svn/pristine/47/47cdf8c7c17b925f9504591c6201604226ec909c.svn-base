//-------------------------------------------------------------------------------------------
/** @file cevaservapi.c
 *
 * @brief CEVA services, API used to control the same as in ARM primitives
 * @author Mindspeed Technologies
 * @version $Revision: 1.1 $
 *
 * COPYRIGHT(c) 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#include "cevaservapi.h"


void* CSAPoolAlloc(PMXPOOL pPool)
{
    UINT32 nIndex;
    _ASSERT_PTR(pPool);

    if (pPool->FreeGet == pPool->FreePut)
        return NULL;

    nIndex = pPool->FreePtr[pPool->FreeGet++];

    if (pPool->FreeGet >= (pPool->BlockNum+1))
        pPool->FreeGet = 0;

    return pPool->StoragePtr + (pPool->BlockSize * nIndex);
}

UINT32 CSAPoolFree(PMXPOOL pPool, void * pBlock)
{
    UINT32 index;

    _ASSERT_PTR(pPool);
    _ASSERT_PTR(pBlock);

    _ASSERT(((UINT32)pBlock >= (UINT32)pPool->StoragePtr));
    _ASSERT(((UINT32)pBlock < (UINT32)pPool->StoragePtr+pPool->BlockSize*pPool->BlockNum));

    index = ((UINT32)pBlock - (UINT32)pPool->StoragePtr) / pPool->BlockSize;

    pPool->FreePtr [pPool->FreePut ++] = index;

    if (pPool->FreePut >= (pPool->BlockNum+1))
        pPool->FreePut = 0;

    return 1;
}
