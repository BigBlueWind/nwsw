/******************************************************************************
* Copyright: (C) 2010 Polaris Networks. All Rights Reserved.                  *
*    No part of this source code may be reproduced, transmitted, transcribed  *
*    or translated into any language without the written permission of        *
*    Polaris Networks or its suppliers or affiliate companies. To obtain this *
*    permission write to the attention of Legal counsel, Polaris Networks,    *
*    75 Robbins Road, MA 02421 United States.                                 *
******************************************************************************/

#include "PW_PALDefinition.h"

typedef struct SSockFdToRntiMap
{
    int iSockFd[MAX_UE_COUNT];
    unsigned int uiServicePort[MAX_UE_COUNT];
    unsigned short usRnti[MAX_UE_COUNT];
    PW_BOOL bIsRntiValid[MAX_UE_COUNT];
    PW_BOOL bTxDataPresent[MAX_UE_COUNT];
    PW_BOOL bIsBroadcast;
} SSockFdToRntiMap;

int FSMInitialize();
int FSMClose();
void FSMExecute();
