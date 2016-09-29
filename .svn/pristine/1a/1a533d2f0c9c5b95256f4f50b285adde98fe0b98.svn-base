//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL i2cdiag.h
 *
 * @brief The I2C diagnostics module
 * @author Mindspeed Technologies
 * @version $Revision: 1.3 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif


#ifndef I2CDIAG_H_
#define I2CDIAG_H_

//#include "4gmx_types.h"
//#include "4gmx_const.h"
//#include "appprintf.h"
//#include "4gmx_arm.h"
#include "4gmx.h"

MXRC I2CDiagInit (UINT32* pReportData, UINT32* pReportSize);
MXRC I2CDiagSearchDev(UINT8* DevSelCode); // Searches any device on I2C bus
MXRC I2CDiagSearchDevAll(UINT8* DevSelCode, UINT32 nLen); // Searches all specified devices on I2C bus

#endif /*I2CDIAG_H_*/

#ifdef __cplusplus
}
#endif

