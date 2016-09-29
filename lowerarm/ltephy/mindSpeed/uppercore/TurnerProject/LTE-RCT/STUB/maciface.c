/** @file maciface.c
 *
 * @brief Interface for LTE eNodeB
 * @author Mindspeed Technologies
 * @version $Revision: 1.64 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */
#include "maciface.h"

/*
 * Local variables
 */

/** Sends PHY_INIT.req message to the PHY instance with the given mode of operation.
 *  Implements 'phystart' console command.
 *
 * @param mode Defines API compatibility support:
 *             0 - Old API mode
 *             2 - TODO:
 * @return Standard result code
 */
RESULTCODE MacCreatePhy(U32 mode)
{
    return MAC_SUCCESS;
}

/** Processes MAC configuration parameters specified in 'createmac' console command
 *
 * @param configId Configuration number as defined in CCP-E-81944
 * @param opt Bit mask specifies configuration options:
 *            bit 0: 1 = Enable extended RXSDU.ind format (84xxx-SWG-009-A)
 * @return Standard result code
 */
RESULTCODE MacProcConsoleParams(U32 configId, U32 opt)
{
    return MAC_SUCCESS;
}

/** Handles configuration part of SVSR_CREATEMAC command
 *  Fills internal INITPARM structure
 *
 * @param h Pointer to the MSPD API header (MSA-E-81479, defined as struct _API_HEADER_ or
 *          struct MsgHeader)
 * @return Standard result code
 */
RESULTCODE MacSetInitConfig(PTR h)
{
    return MAC_SUCCESS;
}

/** Top-level MAC initialization function which should be called from application module
 *
 * @param txCb Pointer to the callback function which will send API messages to PHY
 * @param rxCb Pointer to the callback function which will receive API messages from PHY
 * @param toPhy Application-specific handle associated with the txCb callback
 * @param fromPhy Application-specific handle associated with the rxCb callback
 * @return Standard result code
 */
RESULTCODE MacInit(PhyTxCb txCb, PhyRxCb rxCb, PTR toPhy, PTR fromPhy)
{
    return MAC_SUCCESS;
}

/** Stop MAC operation and destroy MAC instance

 * @return Standard result code
 */
RESULTCODE MacDone(void)
{
    RESULTCODE ret = MAC_SUCCESS;

    return ret;
}

/** Dispatch MAC messages
 *  This dispatcher should be called from Supervisor level when Destination ID set to 'MAC'
 *
 * @param size Message size in bytes
 * @param msg Pointer to the message header (MSA-E-81479)
 * @return Standard result code
 */
RESULTCODE MacDispatch(U32 size, PTR msg)
{
    U32 ret = MAC_SUCCESS;

    return ret;
}

RESULTCODE MacRxEthPacket(PTR pPacket, U32 Len)
{
    return MAC_SUCCESS;
}
