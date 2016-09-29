//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL pciediag.h
 *
 * @brief The PCIe diagnostics module
 * @author Mindspeed Technologies
 * @version $Revision: 1.1 $
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

#ifndef PCIEDIAG_H_
#define PCIEDIAG_H_
	
#include "4gmx.h"
	
MXRC PCIEDiagInit(UINT32* pReportData, UINT32* pReportSize);

#endif /*PCIEDIAG_H_*/
	
#ifdef __cplusplus
}
#endif
