//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL Appids.h
 *
 * @brief
 * @author Mindspeed Technologies
 * @version $Revision: 1.46 $
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

#ifndef _APP_IDs_H_
#define _APP_IDs_H_

#ifndef _WIN32
#include "config.h"
#endif

typedef enum APPOBJID
{
    APP_DRIVER = 0x01,    // The 4GMX system modules have zero based ID */
    APP_MODULE = 0x02,    // The modules like a diagnostics, supervisor and so on

} AppObjIDs;

typedef enum DrvModID
{
    INTC_DRV        = 0x01, // Interrupt Controler driver
    UART_DRV        = 0x02, // UART driver
    LOCAL_TIMER_DRV = 0x03, // Timer
    DSP_DRV         = 0x04, // DSP driver
    FP_DRV          = 0x05, // FP driver
    FEC_DRV         = 0x06, // FEC driver
    CPRI_DRV        = 0x07, // CPRI driver
    SPAcc_DRV       = 0x09, // SPAcc driver
    MDMA_DRV        = 0x0A, // MDMA driver
    I2C_DRV         = 0x0B, // I2C driver
    SPI_DRV         = 0x0C, // SPI driver
    PCIE_DRV        = 0x0D, // PCIe driver
    AD9361_DRV      = 0x0E, // AD9361RADIO driver
    EVM_FPGA_DRV    = 0x0F, // EVM FPGA driver (radio module and PCIe EP)
    DEJITTER_DRV    = 0x10, // DeJitter driver
    SPACC_DRV       = 0x11, // SpaCC driver
    SRIO_DRV        = 0x12, // SRIO driver
    SERDES_DRV      = 0x13, // SERDES initialization: 0, 1, 2
    CLKRST_DRV      = 0x14,

    ICMGR           = 0x20, // Inter-cpu manager
    SUPERVISOR      = 0x21, // Supervisor manager
    CONSOLE_MGR     = 0x22, // Console manager
    COMMAND_LINE    = 0x23, // Command Line Processor
    DSPPRINTF       = 0x24, // This module outputs DSP diagnostic messages to UART2
    APILOGGER       = 0x25, // PHY API logger
    DEV_INIT        = 0x26, // T4K/EVM initialization
    LOG_MGR         = 0x27, // The logs manager (to create log, open log, control log, etc...)
    IQPHONE         = 0x28, // IQ samples player/recorder
    APIPLAYER       = 0x29, // API player
    DATAINT         = 0x2A, // 'HAL' for CPRI/sRIO/PCIe
    GENLOG          = 0x2B, // General application log
    SC_LIBRARY      = 0x2C, // SYSCORE general API
    REX             = 0x2D, // REX - Rapid eXecutive (syscore TCB scheduler)
    REX_DIAG        = 0x2E, // REX diagnostics 
    ICPU2           = 0x2F, // I-CPU externded interface (no locks)
    TRACER          = 0x30, // Tracer service 
    REX_SERV        = 0x32, // REX-SERVER part

    RES_MGMT        = 0x33, // 0x33-0x3F reserved for management modules

    DIAGNOSTICS     = 0x40, // The general diagnostics functional
    ARMDIAG         = 0x41, // The ARM diagnostics of the FPGA H/W components
    DSPDIAG         = 0x42, // The Dsp diagnostic module
    FECDIAG         = 0x43, // The FEC diagnostic module
    FPDIAG          = 0x44, // The FP diagnostic module
    CPRIDIAG        = 0x45, // The CPRI diagnostic module
    MDMADIAG        = 0x46, // MDMA diagnostics
    UNIDIAG         = 0x47, // 0x47-0x5F reserved for diagnostic modules

    WIMAXAPP        = 0x60, // WiMAX application ID
    LTEAPP          = 0x61, // LTE application ID
    LTEVOLUTION     = 0x62, // LTEvolution app
    SCH_TASK_DRV    = 0x63, // Scheduler task driver
    CPUCTRL         = 0x64,
    GEM_DRV         = 0x65, // GEM driver
    SCH_DIAG        = 0x66, // scheduler diagnostics
    PHYPROF         = 0x67, // The PHY profiler
    CORE_CFG        = 0x68,
    LTEMACAPP       = 0x69, // LTE application ID
    LOGERSTORAGE    = 0x70, // Api Loger Storage
    NETWORK_LAYER   = 0x71,	// Network subsystem
    WCDMAL1APP      = 0x72,
    WCDMAL2APP      = 0x73,
    GLOG_STOR       = 0x74, // the global storage 
    SWDIAG_APP      = 0x75, // Turner Software Diagnostics module
    PTMDIAG         = 0x76, //coresight diagnostics module
    MLOG_SVSR       = 0x77, //Mlogger
    LTENMMAPP       = 0x78 // NMM
} DrvModIDs;

typedef enum tagDataObjID
{
    TID_RESERVED        =   0,
    TID_I_CPU_TX        =   1,      // Trace ID, I-CPU1 TX partition
    TID_I_CPU_RX        =   2,
    TID_I_CORE_0        =   3,
    TID_I_CORE_1        =   4,
    TID_I_CORE_2        =   5,
    TID_I_CORE_3        =   6,

}DataObjID;

typedef enum tagInstID
{
    IID_RESERVED        =   0,
    IID_SUPERVISOR      =   1,

    IID_DSP_DIAG_0      =   2,
    IID_DSP_DIAG_1      =   3,
    IID_DSP_DIAG_2      =   4,
    IID_DSP_DIAG_3      =   5,
    IID_DSP_DIAG_4      =   6,
    IID_DSP_DIAG_5      =   7,
    IID_DSP_DIAG_6      =   8,
    IID_DSP_DIAG_7      =   9,
    IID_DSP_DIAG_8      =   10,
    IID_DSP_DIAG_9      =   11,

    IID_FP_DIAG_0       =   12,
    IID_FP_DIAG_1       =   13,
    IID_FP_DIAG_2       =   14,
    IID_FP_DIAG_3       =   15,
    IID_FP_DIAG_4       =   16,
    IID_FP_DIAG_5       =   17,
    IID_FP_DIAG_6       =   18,
    IID_FP_DIAG_7       =   19,
    IID_FP_DIAG_8       =   20,
    IID_FP_DIAG_9       =   21,

    IID_FEC_DIAG        =   22,

    IID_SW_DIAG         =   23,

    IID_WIMAX           =   24,
    IID_LTE             =   25,

    IID_CPRI_DIAG       =   26,
    IID_MDMA_DIAG       =   27,

    IID_LTE_MAC         =   28,
    IID_SVSR_MEM_CTRL   =   29,

    IID_SCH_CONTROL	    =   30,
    IID_CPRI_MGR        =   31,
    IID_I2C             =   32,
    IID_API_LOGGER      =   33,
    IID_IQ_PHONE        =   34, // IQ samples player/recorder
    IID_API_PLAYER      =   35,
    IID_GEN_LOGGER      =   36, // General application logger
    IID_SPACC_DIAG      =   37,
    IID_LTE_EX          =   38,
    IID_WCDMA_L1        =   39,
    IID_WCDMA_L2        =   40,
    IID_WCDMA_L2_L1_IF  =   41,
    IID_INFORMER        =   42, // the service to return information about system objects (MLog, Svsrlog, DL/UL IQ buffers ... )
    IID_DI              =   43,
    IID_UNIDIAG         =   44,
    IID_LTE_MAC_EX      =   45,
    IID_SWDIAG          =   46, // Turner Software Diagnostics module
    IID_PTMDIAG         =   47,
    IID_NETWORK         =   48,
    IID_LTE_NMM         =   49,
    IDD_UMTS_NWL          = 50,

    // Please add new one syscore/PHY IID here



    // from here (50) and up to 100, the IIDs are reserved for
    // the external application designed to use this library
    IID_EXT_APP			= 51,
    SVSR_INST_NUM 		= 100
}InstID;

#endif /*_APP_IDs_H_*/

#ifdef __cplusplus
}
#endif

