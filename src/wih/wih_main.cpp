/**
 * @file    wih_main.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file will handle all the messages coming
 *          from different modules
 *
 * Author     : Tara Shankar Maji
 * Date       : 13-09-2010
 * Description: This file contains functions to process different messages
 *              received from other modules
 * Change History: 13-09-10 Initial version.
 */

#include "wih.h"
#include "wih_oamInterface.h"

/**
* @details DESCRIPTION
* This is the entry point for all messages coming into WIH module
*/

void wihMain( FR Msg *msgIn )
{
	// Process the received IPC message
	COamInterface::getInstance().wihProcessRecvIpcMsg(msgIn);

	// Free received message
	freeMsgIn( msgIn);

	printAndEmptyErrorChain(&(getModuleContext()->errChain));
}
