/** @file dbghelp.c
 *
 * @brief Debug features
 * @author Mindspeed Technologies
 * @version $Revision: 1.7 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/

#include "ltephy.h"
#include "svsrlogger.h"
#include "mlog.h"
#include "LteBsPhyStats.h"

U8 *LteGetMlogInformation(U32* MsgSize)
{
    U8 * ret = 0;
    unsigned int *pMLogArray;
    unsigned int MLogLength;

    MLogLength = MLogGetFileSize();
    pMLogArray = MLogGetFileLocation();
    MxCacheClean(pMLogArray, MLogLength);

    ret = (U8 *) MLogGetFileLocation();
    *MsgSize = MLogGetFileSize();

    return ret;
}
U8 *LteGetSvsrInformation(U32* MsgSize)
{
    U8 * ret = 0;

    ret = (U8 *) SvsrLoggerGetLocation();
    *MsgSize = SvsrLoggerGetSize();

    return ret;
}

U8 *LteGetPhyStatsInformation(U32* MsgSize)
{
    U8 * ret = 0;
    unsigned int *pPhyStatsArray;
    unsigned int PhyStatsLength;

    PhyStatsLength = LteBsPhyStatsGetSize();
    pPhyStatsArray = (unsigned int*)LteBsPhyStatsGetCtx();
    if (MxIsDCached((UINT32)pPhyStatsArray))
        MxCacheClean(pPhyStatsArray, PhyStatsLength);

    ret = (U8 *) LteBsPhyStatsGetCtx();
    *MsgSize = LteBsPhyStatsGetSize();

    return ret;
}

