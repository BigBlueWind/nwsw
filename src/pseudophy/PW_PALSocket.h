/******************************************************************************
* Copyright: (C) 2010 Polaris Networks. All Rights Reserved.                  *
*    No part of this source code may be reproduced, transmitted, transcribed  *
*    or translated into any language without the written permission of        *
*    Polaris Networks or its suppliers or affiliate companies. To obtain this *
*    permission write to the attention of Legal counsel, Polaris Networks,    *
*    75 Robbins Road, MA 02421 United States.                                 *
******************************************************************************/

#include "PW_PALDefinition.h"

int SKInitializeSocketModule(int *piSockFd);
int SKReadFromSocket(char *pcRxBuffer, int iSockFd);
int SKWriteOverSocket(unsigned char* pucMsg, unsigned int uiMsgLen, int iSockFd,
                      unsigned int uiPort);
int SKCloseSocketModule(int *piSockFd);
