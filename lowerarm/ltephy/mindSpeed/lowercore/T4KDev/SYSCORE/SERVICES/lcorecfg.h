//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL lcorecfg.h
 *
 * @brief The file contains lower core configuration structure definition
*       This structure is used by various l-arm modules (for example I-CPU)
 * @author Mindspeed Technologies
 * @version $Revision: 1.16 $
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

#ifndef _LCORE_CFG_H_
#define _LCORE_CFG_H_

#include "4gmx.h"

// ====================================================
// The bits specific for <Options> of tagLCoreCfg type
// ====================================================
#define LCFG_OPT_L2_ENABLED         (1 << 0)
#define LCORECFG_DEFAULT_PROFILE	0
#define LCORECFG_T3000_PROFILE		1

typedef struct tagLCoreCfg
{

      VUINT32   MagicNum;            // The magic number to be sure that L-ARM supports core configuration parameters
      VUINT32   LowerArmVersion;     // The version of L-ARM code

      VUINT32   ICPUBase;            // The base address of I-CPU area

      VUINT32   HostIntID;           // This value is equal to I-CPU ID
				                     //   0 - set by Linux, the regular I-CPU interface
				                     //   1 - by defaut HBI interface 
				                     //   2 - extended I-CPU interface 
                                     
      VUINT32   RunStatus;           // It contains various information:
                                     //    bit0 - ARMx2:core0 - is OK
                                     //    bit1 - ARMx2:core1 - is OK
                                     //    bit2 - I-CPU is configured (in case of extended I-CPU)
                                     //    bit31- From ResetHandler, The code is started on core#0
                                     //    the rest are reserved

      VUINT32   DiagCtrl;            //    The diagnostics bit mask, it's used to control the diagnostics from the Linux

      VUINT32   DiagRes;             //  The diagnostics results

      VUINT32   UARTParams[3];       // Initial values for each of 3 UARTs:
                                     // bits 0-1 - size, bit 2 - stop, bit 3 - parity, bit 4 - parity select, bits 5-21 - speed, bits 22-23 - UART_ID
                                     // UARTParams[0] corresponds to UART-0, UARTParams[1] - UART-1, UARTParams[2] - UART2

      VUINT32   EnableDevice;        // enables or disables device: (see structure ENABLEDEVICE below)

      VUINT32   Options;             // The system options:
                                     //  bit#0 - L2 cache enable/disable

      VUINT32   CEVAFreq;            // CEVA's frequency. This parameter is 300 or 500

      VUINT32   FPFreq;              // FP's frequency. This parameter must be 300, 450, 500 or 600

      VUINT8	MacAddr[8];			 // Mac Address, used first 6 bytes.

      VUINT32   IPAddr;              // IP Address. If 0 - use DHCP

      VUINT16   Profile;	

      VUINT32   ConICPUBase;

      VUINT32   Res;

}LCoreCfg;

typedef struct
{
	unsigned int gemac1     :1;  // bit 0 - enable/disable GEMAC1: (1- enabled, 0 - disabled)
	unsigned int cmgrUART   :2;  // bits 1-2 - determines the number of UART for ConsoleManager(CM): 0 - UART-0, 1 - UART-1, 2 - UART-2, 3 - CM - disabled
	unsigned int cmgrinited :1;  // bit 3 - indicates whether ConsoleManager has been inited by Linux: 0 - not inited, 1 - inited
	unsigned int cmgrCICPU  :1;  //
	unsigned int reserved   :27;  // reserved

} ENABLEDEVICE, *PENABLEDEVICE;

extern const LCoreCfg  CoreCfg;

#define IS_ICPU()       (CoreCfg.HostIntID == 0)
#define IS_HBI_MODE()   (CoreCfg.HostIntID == 1)
#define IS_EXT_ICPU()   (CoreCfg.HostIntID == 2)
#define IS_STAND_ALONE()    (IS_HBI_MODE())

#endif /*_LCORE_CFG_H_*/

#ifdef __cplusplus
}
#endif

