/** @file MS_externs.h
 *
 * @brief External definitions for Mobile Station
 * @author Mindspeed Technologies
 * @version $Revision: 1.16 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef _MS_EXTERNS_H__
#define _MS_EXTERNS_H__

#include "basetypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

EXPORT void WimaxMobileStationInit(void);
EXPORT void WimaxMobileStationDone(void);
EXPORT void LteMobileStationInit(int argc, char *argv[]);
EXPORT void LteMobileStationDone(void);

#ifdef __cplusplus
}
#endif

#endif/*_MS_EXTERNS_H__*/
