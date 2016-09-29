/** @file LteMac2PhyInterface.h
 *
 * @brief
 * @author Mindspeed Technologies
 * @version $Revision: 1.43 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef _LTE_MAC_2_PHY_INTERFACE_H
#define _LTE_MAC_2_PHY_INTERFACE_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "bsmac.h"
#include "resultcodes.h"
#include "basetypes.h"
#ifndef AC_MSPD_TDD
#include "LtePhyL2Api.h"
#else
#include "LtePhyL2Api_TDD.h"
#endif
#include "LteBsMacPdcchMgr.h"

extern RESULTCODE (*DciDLPrepareFunctionPtr[MAX_DCI_FORMAT])(U8 *pSdu, DCI_DATA_INFO* pDciInfo,
                                                             U32 *index);
/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

#define MAXLISTSAVEDPOINTER             16

/****************************************************************************
 * Exported Types
 ****************************************************************************/
typedef struct ListSavedPointer
{
    PTR     pMac2Phy_Queue_El;
    PTR     pMessage;
} LISTSAVEDPOINTER, *PLISTSAVEDPOINTER;

typedef struct ListPointer
{
    U32                 cnt;
    LISTSAVEDPOINTER    listSavedPointer[MAXLISTSAVEDPOINTER];
} LISTPOINTER, *PLISTPOINTER;

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

extern OSAL_QUEUE dlControlTxSduQueue;

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

RESULTCODE LtePhyInterfaceInit(Mac2PhyInterface *pMac2Phy,
                               PhyTxCb txCb,
                               PhyRxCb rxCb,
                               PTR toPhy,
                               PTR fromPhy);
RESULTCODE LtePhyInterfaceClean(Mac2PhyInterface *pMac2Phy);

RESULTCODE LtePhyIfUpdate20MhzBw(PINITPARM pParm);

/*RESULTCODE LtePhyFillTxVector(BS_CONTEXT *pBsCtx,
                              DCI_CCE_CONTAINER *pDciCceInfo,
                              PDLSUBFRDESC pSubFrame);*/

RESULTCODE LtePhyPreparDataChSdu(DCI_DATA_INFO *pDci, U8 chId);

RESULTCODE LtePhyParamInit(PINITPARM pParm,
                           PDLSUBFRDESC pTxVector,
                           PULSUBFRDESC pRxVector,
                           U32 config);

RESULTCODE LteMacSendToPhyMessage(U32 MessageType, PTR param, U32 flag);
RESULTCODE LteMacSendToPhyNmmMessage(U32 MessageType, PTR param, U32 flag);
RESULTCODE LteMacSendToPhyNmmMessageTest(U32 MessageType, PTR param, U32 flag);

RESULTCODE LteMacProcessFromPhyMessage(PGENMSGDESC pMsgHdr,
                                       PDLSUBFRDESC pDlSubFrame,
                                       PULSUBFRDESC pUlSubFrame,
                                       PTR pUe);

RESULTCODE LteMacProcessFromPhyMessageRt(PGENMSGDESC pMsgHdr,
                                       PDLSUBFRDESC pDlSubFrame,
                                       PULSUBFRDESC pUlSubFrame,
                                       PTR pUe);

RESULTCODE LtePhyPreparControlChSdu(DCI_DATA_INFO *pDci, U8 chanId);
RESULTCODE LtePhyPrepareTestSdu(U8* payload, U32 numBits, U8 chId, U8 channelType, U8 cwId);
RESULTCODE LtePhyPrepareTestSduRt(U8* payload, U32 numBits, U8 chId, U8 channelType, U8 cwId, U32 flag);


RESULTCODE SduSend(U8 ApiId, U8* Data, U32 Len);

RESULTCODE DciPrepareFormat0(U8 *pSdu, DCI_DATA_INFO *pDciInfo, U32 *index);

U8* LtePhy2MacGetVirt(U8* pAddr);

RESULTCODE LtePhyIfUpdate10MhzBw(PINITPARM pParm);

RESULTCODE LteMac2PhyAddToTail(U8 MessageType, U32 MessageLen, U8 *MessagePtr);
PTR LteMac2PhyGetDataOffset(PTR pMsg);

#endif /* _LTE_MAC_2_PHY_INTERFACE_H */

