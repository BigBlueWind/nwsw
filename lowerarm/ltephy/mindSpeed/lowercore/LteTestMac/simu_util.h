/** @file simu_util.h
 *
 * @simulator utility header file
 * @author Mindspeed Technologies
 * @version $Revision: 1.4 $
 *
 * COPYRIGHT(c) 2008 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef SIMU_UTIL_H
#define SIMU_UTIL_H

enum TX_SIMU_STATUS
{
	ONGOING = 0,
	DONE
};

enum RX_SIMU_STATUS
{
	SIMU_0 = 0,
	SIMU_TX,
	SIMU_RX,
	SIMU_TXRX
};

void PrintSimuStatus(void);

void InitSimuStartTime();
OSAL_DELAYM GetSimuCurrentTime();

#endif // SIMU_UTIL_H

