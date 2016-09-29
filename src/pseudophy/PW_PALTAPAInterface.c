/******************************************************************************
* Copyright: (C) 2010 Polaris Networks. All Rights Reserved.                  *
*    No part of this source code may be reproduced, transmitted, transcribed  *
*    or translated into any language without the written permission of        *
*    Polaris Networks or its suppliers or affiliate companies. To obtain this *
*    permission write to the attention of Legal counsel, Polaris Networks,    *
*    75 Robbins Road, MA 02421 United States.                                 *
******************************************************************************/

#include "PW_PALInterface.h"

static void _PostTaskFromPAL(void* pvTfuStruct, unsigned int uiParamType);

PUBLIC S16 TFActvInit(
    Ent ent,
    Inst inst,
    Region region,
    Reason reason)
{
    PW_PALInitLibrary(_PostTaskFromPAL);
}

PUBLIC S16 TFActvTsk(
    Pst *pst,
    Buffer *mBuf)
{
    switch(pst->event)
    {
        case EVTTFUCNTRLREQ:
        /*Call Unpack routine for TfUiTfuCntrlReq*/
        /*???tfUnpkTfuCntrlReq(TfUiTfuCntrlReq, pst, mBuf);???*/
        {
            STfuCntrlReqInfo* psTfuCntrlReqInfo = {0};
            /*Fill up PAL structure*/
            PW_PALProcessTfuCntrlReq(psTfuCntrlReqInfo);
        }
            break;
        case EVTTFUDATREQ:
        {
        }
            break;
        case EVTTFURECPREQ:
        {
        }
            break;
        default:
            break;
    }
}

static void _PostTaskFromPAL(void* pvTfuStruct, unsigned int uiParamType)
{
    Pst *pst;
    Buffer *mBuf;

    switch(uiParamType)
    {
    case TFUTTIINDINFO:
        {
            STfuTtiIndInfo* psTfuTtiIndInfo = (STfuTtiIndInfo*)pvTfuStruct;
            /*Call pack routine for TfUiTfuTtiInd*/
            /*Call twice for RG and SCH*/
            /*SPstTsk(pst, mBuf);*/
        }
        break;
    default:
        break;
    }
    return;
}
