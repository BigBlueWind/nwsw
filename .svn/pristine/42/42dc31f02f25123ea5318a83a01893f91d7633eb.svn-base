/******************************************************************************** 
**Copyright (c) 2011, Mindspeed
**			All Rights Reserved.
**
**  File: celllisten main test fun
**  Created By:  Qingyu Gao	
**  Created On:  07/28/2011
**
**  ----------------------------------------------------------------------------
********************************************************************************/

#ifndef _FRE_AND_CHANNEL_COMPENSATE_
#define _FRE_AND_CHANNEL_COMPENSATE_

/*------commom head file-------*/
#ifndef VS_ENV
#include "..//prototypes.h"
#endif

#include "UMTSNwlCommMacro.h"

#ifdef VS_ENV
void ChannelCompensation(S16* const psChannel,	S16 *psDataIn,S16 *PsDataOut);
void FreErrCompensation(U32 SlotCnt,  S16 *psDataIn,S16 *pDataOut,U32 dNum,S16 angle,  S16 *pAngleTable);
void CalculFreErrEstRun(S16 *pReFre_4Frame, S16 *pResiFreErrValue,S32 *IQ_sum);

void CalcInitFreErr(S8 Pscr[],S16 FreErrVale[],S16 InputAnt0[],U32 ChipNum,U8 SampleRate,U8 SymbolFlag);

#else
void ChannelCompensation(S16* const psChannel,	S16 *psDataIn,S16 *PsDataOut)CEVAEXTERN;
void FreErrCompensation(U32 SlotCnt,  S16 *psDataIn,S16 *pDataOut,U32 dNum,S16 angle,  S16 *pAngleTable)CEVAEXTERN;
void CalculFreErrEstRun(S16 *pReFre_4Frame, S16 *pResiFreErrValue,S32 *IQ_sum)CEVAEXTERN;

void CalcInitFreErr(S8 Pscr[],S16 FreErrVale[],S16 InputAnt0[],U32 ChipNum,U8 SampleRate,U8 SymbolFlag)CEVAEXTERN;
void CalculFreErrEst_1Frame(S16 *sIn, S16 *sOut, U32 dNoncoLen,U32 dconLen)CEVAEXTERN;
void CalcFreErrAngle(S16 *pReFreErr,S32 *IQ_sum)CEVAEXTERN;
#endif

#endif
