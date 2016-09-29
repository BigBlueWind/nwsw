//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL Lcorecfg.c
 *
 * @brief
 * @author Mindspeed Technologies
 * @version $Revision: 1.13 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "lcorecfg.h"
#include "config.h"
#include "intercpu.h"
#include "bootlog.h"
#include "network/network.h"

#define MAGIC_NUMBER   0x12345678
#define DEF_DIAG_MASK  0xFFFFFFFF

#define UART_DEF_VALUE 0

const LCoreCfg  CoreCfg = {
                             MAGIC_NUMBER,                       // MagicNum
                             (APP_H_VER << 16) | (APP_L_VER),    // LowerArmVersion
                             INTER_CPU_MEM_ADDRESS,              // ICPUBase
                             1,                                  // HbiEnabled
                             0,                                  // RunStatus
                             DEF_DIAG_MASK,                      // DiagCtrl
                             NULL,                               // diagnostic results
                             UART_DEF_VALUE,                     // Def UART parameters
                             UART_DEF_VALUE,                     // Def UART parameters
                             UART_DEF_VALUE,                     // Def UART parameters
                             0x0,                                  // enable devices (GEMAC-1 and etc.)
                             0,                                  // The system options
                             500,                                // CEVA freq.
                             600,                                // Fp freq.
                             {NET_DEF_MAC,0,0},					 // Def Mack Address
                             0,									 // Use DHCP by def
#ifndef 					 PROFILE_T3K
                             LCORECFG_DEFAULT_PROFILE,			 // Profile of clock freq
#else
                             LCORECFG_T3000_PROFILE,			 // Profile of clock freq
#endif
                             0                                   // RES
                          };


