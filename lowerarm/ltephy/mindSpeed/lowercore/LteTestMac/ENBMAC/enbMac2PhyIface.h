/** @file LteMac2PhyInterface.h
 *
 * @brief Message-based MAC-PHY interface
 * @author Mindspeed Technologies
 * @version $Revision: 1.28 $
 *
 * COPYRIGHT(c) 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef _ENB_2_PHY_IFACE_H
#define _ENB_2_PHY_IFACE_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "4gmx_types.h"
#include "bsmac.h"
#include "resultcodes.h"
#include "LtePhyL2Api.h"

/*
 * Global Variables
 */

extern HANDLE dlControlTxSduQueue;
extern RESULTCODE (*DciDLPrepareFunctionPtr[MAX_DCI_FORMAT])(U8 *pSdu, DCI_DATA_INFO* pDciInfo,
                                                             U32 *index);
typedef RESULTCODE (*pLtePhyHiDciAPISet)(PDLSUBFRDESC pDlSubFrame, PULSUBFRDESC pUlSubFrame,
                                         PRXSDUIND pRxSdu);

extern pLtePhyHiDciAPISet pHiDciApiSet;

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

RESULTCODE LtePhyIfaceInit(Mac2PhyInterface *pMac2Phy, 
                           PhyTxCb txCb, 
                           PhyRxCb rxCb, 
                           PTR toPhy,
                           PTR fromPhy);
RESULTCODE LtePhyIfaceClean(Mac2PhyInterface *pMac2Phy);

RESULTCODE LtePhyIfInitDefault(PINITPARM pParm);

RESULTCODE LtePhyIfUpdate20MhzBw(PINITPARM pParm);

RESULTCODE LtePhyFillTxVector(BS_CONTEXT *pBsCtx, 
                              DCI_CCE_CONTAINER *pDciCceInfo,
                              PDLSUBFRDESC pSubFrame);
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

MXRC LteMacRxSduHandler(LPVOID pClientCtx, LPVOID pMsg);

RESULTCODE LtePhyPreparControlChSdu(DCI_DATA_INFO *pDci, U8 chanId);
RESULTCODE LtePhyPrepareTestZeroSdu(U8* Payload, U32 numBits, U8 chId, U8 channelType, U32 cwId, U32 ones);
RESULTCODE LtePhyPrepareTestZeroSduPbch(U32 numBits, U8 chId, U8 channelType, U8* PayloadPbch);
RESULTCODE LtePhyPrepareTestPdcchSdu(DCI_DATA_INFO* pDciInfo, 
                                     U32 numBits, 
                                     U8 chId, 
                                     U8 channelType,
                                     U8* pDci);

RESULTCODE LtePhyPrepareTestSduPhich(U8 chId, U8 groupId, U8 seqIdx, U8 channelType, U8 nackAck);
RESULTCODE LtePhyHiDciSet(PDLSUBFRDESC pDlSubFrame, PULSUBFRDESC pUlSubFrame, PRXSDUIND pRxSdu);

RESULTCODE SduSend(U8 ApiId, U8* Data, U32 Len);

RESULTCODE DciPrepareFormat0(U8 *pSdu, DCI_DATA_INFO *pDciInfo, U32 *index);
RESULTCODE DciPrepareFormat1(U8 *pSdu, DCI_DATA_INFO* pDciInfo, U32 *index);
RESULTCODE DciPrepareFormat1a(U8 *pSdu, DCI_DATA_INFO* pDciInfo, U32 *index);
RESULTCODE DciPrepareFormat1a_autoRBA(U8 *pSdu, DCI_DATA_INFO* pDciInfo, U32 *index);
RESULTCODE DciPrepareFormat2(U8 *pSdu, DCI_DATA_INFO* pDciInfo, U32 *index);
RESULTCODE DciPrepareFormat2_autoRes(U8 *pSdu, DCI_DATA_INFO* pDciInfo, U32 *index);
RESULTCODE DciPrepareFormat1C(U8 *pSdu, DCI_DATA_INFO* pDciInfo, U32 *index);

RESULTCODE LtePhyIfUpdate10MhzBw(PINITPARM pParm);

void Mac2PhyEnableICPUEmul(UINT32 nFlag);

PGENMSGDESC Mac2PhyIfAllocBuffer(void);
PGENMSGDESC Mac2PhyIfAllocBufferEx(U32 size);
void Mac2PhyIfFreeBuffer(PGENMSGDESC pMsg);

// TODO: Move to diagnostics module
RESULTCODE DiagDumpPhyApi(PTR vector);

#endif /* _ENB_2_PHY_INTERFACE_H */
