/** @file simu_util.c
 *
 * @simulator utility implementation
 * @author Mindspeed Technologies
 * @version $Revision: 1.4 $
 *
 * COPYRIGHT(c) 2008 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#include "osal.h"
#include "simu_util.h"

enum TX_SIMU_STATUS TxSimuStatus;
enum RX_SIMU_STATUS RxSimuStatus;
int SimuChoice;
OSAL_DELAYM SimuStartTime;  //in ms

void PrivatePrintSimuStatus(int simuStatus, char * simuChoiceString)
{
	switch(simuStatus)
	{
		case (ONGOING):

			printf("@%d ms: %s : simulation ongoing!\n", GetSimuCurrentTime(), simuChoiceString);
			break;

		case (DONE):
			printf("@%d ms: %s : simulation done!\n", GetSimuCurrentTime(), simuChoiceString);
			break;

	}

}

void PrintSimuStatus(void)
{
	char* simuChoiceString;
	switch(SimuChoice)
	{
		case (SIMU_TX):
			simuChoiceString = "TX";
			PrivatePrintSimuStatus(TxSimuStatus, simuChoiceString);
			break;

		case (SIMU_RX):
			simuChoiceString = "RX";
			PrivatePrintSimuStatus(RxSimuStatus, simuChoiceString);
			break;

		default:
			simuChoiceString = "TX";
			PrivatePrintSimuStatus(TxSimuStatus, simuChoiceString);
			simuChoiceString = "RX";
			PrivatePrintSimuStatus(RxSimuStatus, simuChoiceString);
			break;
	}

}

void InitSimuStartTime()
{
    OsalGetTimeMs(&SimuStartTime);
}


OSAL_DELAYM GetSimuCurrentTime()
{
    OSAL_DELAYM CurrTime;
    OsalGetTimeMs(&CurrTime);
    return CurrTime - SimuStartTime;
}

