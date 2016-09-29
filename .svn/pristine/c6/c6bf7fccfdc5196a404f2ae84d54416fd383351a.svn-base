/** @file enbMacIface.h
 *
 * @brief Interface to WiMAX MAC or LTE L2/L3
 * @author Mindspeed Technologies
 * @version $Revision: 1.7 $
 *
 * COPYRIGHT(c) 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef ENB_MAC_IFACE_H_
#define ENB_MAC_IFACE_H_

#include "resultcodes.h"

/*
 * MAC-PHY Callbacks
 */

// Used to transmit messages from MAC to PHY
typedef RESULTCODE(*PhyTxCb)(PTR h,PTR pMsg, U32 msgSize);

// Used to receive messages from PHY to MAC
typedef RESULTCODE(*PhyRxCb)(PTR h,PTR pMsg, U32 *pMsgSize);

/*
 * MAC Interface Functions
 */

RESULTCODE MacInit(PhyTxCb txCb, PhyRxCb rxCb, PTR toPhy, PTR fromPhy);
RESULTCODE MacSetInitConfig(PTR h);
RESULTCODE MacProcConsoleParams(U32* pVals, U32 num);

RESULTCODE MacCreatePhy(U32* pMode);
RESULTCODE MacReconfigPhy(U32* pMode);
RESULTCODE MacDone(void);
RESULTCODE MacNMMStart(U32 mode);
RESULTCODE MacNMMStop(U32 mode);
RESULTCODE MacNMMCmd(U32 mode);
RESULTCODE MacDispatch(U32 size, PTR msg);
/*UMTSNMM*/
RESULTCODE MacUmtsNMMCmd(U32 mode);
/*
 * MAC configuration parameters
 */
int MacDrainRxSduModeOn(void);
int MacExtTestMode(void);
int MacGetPhyInstanceId(void);
int MacGetNfft(void);
int MacGetNantRx(void);
int MacGetNantTx(void);

int get_mac_console_vals_num(void);
int get_mac_console_vals(U32 idx);

#endif /* ENB_MAC_IFACE_H_ */
