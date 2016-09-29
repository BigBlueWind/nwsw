//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL pciedrv.h
 *
 * @brief PCIe (PCI express) HAL functions
 * @author Mindspeed Technologies
 * @version $Revision: 1.12 $
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

#ifndef __PCIEDRV_H__
#define __PCIEDRV_H__

#include "syscore.h"
#include "appids.h"
#include "hal.h"
#include "srdsdrv.h"

#define PCIE_RC_OK                      MX_OK
#define PCIE_RC_UNSUPPORTED_CFG         DEF_USRRC(APP_DRIVER, PCIE_DRV, 1)
#define PCIE_RC_PARAM_ERROR             DEF_USRRC(APP_DRIVER, PCIE_DRV, 2)
#define PCIE_RC_EMPTY                   DEF_USRRC(APP_DRIVER, PCIE_DRV, 3)
#define PCIE_RC_IS_DISABLED             DEF_USRRC(APP_DRIVER, PCIE_DRV, 4)
#define PCIE_RC_BUF_ALIGNMENT           DEF_USRRC(APP_DRIVER, PCIE_DRV, 5)
#define PCIE_RC_UNSUPP_API              DEF_USRRC(APP_DRIVER, PCIE_DRV, 6)
#define PCIE_RC_UNSUPP_IRQ              DEF_USRRC(APP_DRIVER, PCIE_DRV, 7)
#define PCIE_RC_LINK_IS_NOT_SET         DEF_USRRC(APP_DRIVER, PCIE_DRV, 8)
#define PCIE_RC_IRQ_IS_DISABLED         DEF_USRRC(APP_DRIVER, PCIE_DRV, 9)
#define PCIE_RC_CONNECT_TIMEOUT         DEF_USRRC(APP_DRIVER, PCIE_DRV, 10)
#define PCIE_RC_BASEMEM_NOT_ALIGNED     DEF_USRRC(APP_DRIVER, PCIE_DRV, 11)
#define PCIE_RC_DEV_NOT_INITIALIZED     DEF_USRRC(APP_DRIVER, PCIE_DRV, 12)
#define PCIE_RC_SERDES_ID_ERROR         DEF_USRRC(APP_DRIVER, PCIE_DRV, 14)
#define PCIE_RC_EXTGPIO_WR_ERROR        DEF_USRRC(APP_DRIVER, PCIE_DRV, 15)
#define PCIE_RC_UNKNOWN_EP_ENABLE       DEF_USRRC(APP_DRIVER, PCIE_DRV, 16)
#define PCIE_EP_IRQ_GEN_ERROR           DEF_USRRC(APP_DRIVER, PCIE_DRV, 17)
#define PCIE_MSI_CAP_ERROR              DEF_USRRC(APP_DRIVER, PCIE_DRV, 18)
/***************************************************************************
*                  PCIe configuration parameters                           *
****************************************************************************/
#define PCIE_DRV_OPT_RC                 (1 << 0)
#define PCIE_DRV_OPT_EP                 (0 << 0)
#define PCIE_DRV_OPT_SRDS_0             (0 << 1)    // Serdes-0 is used (x4 lanes)
#define PCIE_DRV_OPT_SRDS_2             (1 << 1)    // Serdes-2 is used (x4 lanes)
#define PCIE_DRV_OPT_LANE_1             (0 << 2)    // 1 lanes is used
#define PCIE_DRV_OPT_LANE_2             (1 << 2)    // 2 lanes is used
#define PCIE_DRV_OPT_LANE_4             (2 << 2)    // 4 lanes is used
#define PCIE_DRV_OPT_BURST_64           (0 << 4)    // 8 beats (64 bytes)
#define PCIE_DRV_OPT_BURST_128          (1 << 4)    // 16 beats (128 bytes)

#define PCIE_RC_MEM                     (0xFA800000)    // PCIE_BASEADDR + 0x000000
#define PCIE_RC_IO                      (0xFA900000)    // PCIE_BASEADDR + 0x100000
#define PCIE_RC_CFG                     (0xFAA00000)    // PCIE_BASEADDR + 0x200000

#define PCIE_DRV_CONNECT_TIMEOUT        (150000 * (3 * 1000))  // 3 seconds
#define PCIE_DRV_IRQ_ACK_TIMEOUT        (150000 * (2 * 1000))  // 2 seconds

#define PCIE_LINK_NOT_DETECTED          0
#define PCIE_LINK_TRAINING              1
#define PCIE_LINK_DL_INIT               2
#define PCIE_LINK_DL_COMPLETE           3

#define PCI_VENDOR_ID_FPGA              0x10EE  // Vendor ID advertised by the FPGA on T4K EVM
#define PCI_DEVICE_ID_FPGA              0x0007  // Device ID advertised by the FPGA on T4K EVM
#define PCI_VENDOR_ID_OXPCIE952         0x1415  // Vendor ID advertised by the 2 UART PCIe plug-in Endpoint
#define PCI_DEVICE_ID_OXPCIE952         0xC158  // Device ID advertised by the 2 UART PCIe plug-in Endpoint

#define PCI_VENDOR_ID_MINDSPEED         0x1234  //0x18DC
#define PCI_SUBVENDOR_ID_MINDSPEED      0x4321  //0x18DD
#define PCI_DEVICE_ID_MINDSPEED         0xBEEF

#define PCI_CLASS_PCIE_REV1_0           0
#define PCI_CLASS_MULTIMEDIA            0x4
#define    PCI_SUBCLASS_OTHER           0x80
#define PCI_CLASS_SIMPLE_COM_CTRL       0x7     // OXPCIe952 - Simple Communication Controller
#define    PCI_SUBCLASS_SERIAL          0x0     // OXPCIe952 
#define       PCI_PROG_IF_16550         0x2     // OXPCIe952
#define    PCI_SUBCLASS_PARALLEL        0x0

// PCIe header type (used to further parse fields based on variation of the PCIe header based on device
// type (example, bridges are parsed different from normal end stations)
#define PCI_HDR_NON_BRIDGE              0
#define PCI_HDR_PCI_TO_PCI_BRIDGE       1
#define PCI_HDR_CARDBUS_BRIDGE          2
#define PCI_HDR_TYPE_MASK               0x7f // lower 7 bits

#define PCI_HDR_SINGLE_FUNCTION_DEVICE  (0<<7)
#define PCI_HDR_MULTI_FUNCTION_DEVICE   (1<<7)
#define PCI_HDR_FUNCTION_MASK           0x80

/** @brief Maximum number of devices to enumerate into a table (sets table size) */
#define MAX_PCIE_DEVICES                    4       // Actually 256-devices is possible

/** @brief Macro to check if the controller is in RC or EP configuration */
#define PCIeDrvIsRC()                   (REG32(RAD_CFG_PCIE_CFG) & (1 << 1))

/** @brief Macro to read a given PCIe configuration register based on register number 
 * where each register represents 32 bits
 */
#define PCIEREG(x)                              ((VUINT32 *)PCIE_RC_CFG + x)

// PCI 3.0 registers Doubleword number (Index from base)
#define PCI_FUNC_VIDDID                     0x0 // Vendor ID and Device ID 
#define PCI_FUNC_CMDSTAT                    0x1 // Command and Status
#define PCI_FUNC_REVIDCLASS                 0x2 // Revision ID and Class Code
#define PCI_FUNC_BIST                       0x3 // BIST, Header Type, Latency Timer and Cache Line Size
#define PCI_FUNC_BAR0                       0x4 // Base Address 0
#define PCI_FUNC_BAR1                       0x5 // Base Address 1
#define PCI_FUNC_BAR2                       0x6 // Base Address 2
#define PCI_FUNC_BAR3                       0x7 // Base Address 3
#define PCI_FUNC_BAR4                       0x8 // Base Address 4
#define PCI_FUNC_BAR5                       0x9 // Base Address 5
#define PCI_FUNC_SUBVIDDID                  0xB // Subsystem Vendor ID and Subsystem ID
#define PCI_FUNC_EXPROMBASE                 0xC // Expansion ROM Base Address
#define PCI_FUNC_CAPPTR                     0xD // Capabilities Pointer
#define PCI_FUNC_INTLINEPIN                 0xF // Interrupt Line and Interrupt Pin

// Bridge control variants (Header Type 1)
#define PCI_FUNC_BUS_CONFIG                 0x6 // Primary, Secondary, Subordinate Bus Number
#define PCI_FUNC_SEC_STATUS_IO_INFO         0x7 // I/O Info, Limit and Secondary Status
#define PCI_FUNC_MEM_LIMIT_AND_BASE         0x8 // Memory Base and Memory Limit
#define PCI_FUNC_PREF_MEM_LIMIT_AND_BASE    0x9 // Prefetchable Memory Base and Memory Limit
#define PCI_FUNC_PREF_BASE_UPPER_32         0xA // Prefetchable Base (Upper 32 bits_
#define PCI_FUNC_PREF_LIMIT_UPPER_32        0xB // Prefetchable Base (Lower 32 bits)
#define PCI_FUNC_IO_LIMIT_BASE_UPPER_32     0xC // I/O Base (Upper 16 bits) and I/O Limit (Upper 16 bits)

// Bits definition for Command & Status register (PCI_FUNC_CMDSTAT)
#define PCI_COMMAND_IO                      (1<<0)  // I/O space
#define PCI_COMMAND_MEMORY                  (1<<1)  // Memory space
#define PCI_COMMAND_MASTER                  (1<<2)  // Bus mastering
#define PCI_COMMAND_PARITY_ERROR_RESP       (1<<6)  // Parity Error Response
#define PCI_COMMAND_SERR                    (1<<8)  // SERR#
#define PCI_COMMAND_INT_DISABLE             (1<<10) // Interrupt Disable
#define PCI_STATUS_INT_PENDING              (1<<19) // Interrupt Status (Pending)
#define PCI_STATUS_CAPABILITIES_LIST        (1<<20) // Capabilites list

/** @brief Macros and values of the PCI Capabilites field in the config space
 */

#define CAPABILITY_ID(x)                    (x & 0xFF)
#define CAPABILITY_NEXT(x)                  ((x>>8) & 0xFF)
#define CAPABILITY_CONTROL(x)               ((x>>16) & 0xFF)

#define CAPABILITY_ID_POWER_MGT             0x01
#define CAPABILITY_ID_MSI                   0x05
#define CAPABILITY_ID_PCI_EXPRESS           0x10
#define CAPABILITY_ID_MSI_X                 0x11

// new define for the Fix from Korean BMT by Jayden Lee; 
// 1) PHY initilize the DL buffer with small values to not damage the PA
// 2) PCIe link up failure, call serdes init again if failed
#define Korean_BMT_RF_PCIE

// Message signalled Interrupts (MSI) Capability
typedef struct tag_MSI_CAP_REGISTERS 
{   
    VUINT32 Header;             // +0x00, Control, Next pointer, Capability ID = 5
    VUINT32 LowAddr;            // +0x04, MSI Lower 32-bit Address Register
    VUINT32 HighAddr;           // +0x08, MSI High 32-bit Address Register
    VUINT32 Data;               // +0x0C, MSI Data
    VUINT32 MaskBits;           // +0x10, Mask Bits
    VUINT32 PendingBits;        // +0x14, Pending Bits            
}MSI_CAP_REGISTERS, *PMSI_CAP_REGISTERS;


/** @brief The definition of the PCI configuration registers for Header Type 0
           the function registers:

             * PCI 3.0 Configuration Registers
             * PCI Express Configuration Registers   */

typedef struct tagPCIEREGS
{
    // Address Map for Configuration Register Sets (Header Type 0)

    VUINT32     VDid;           // Vendor ID and Device ID Register
    VUINT32     CmdStat;        // Command and Status Register
    VUINT32     RevIDClassID;   // Revision ID and Class Code Register
    VUINT32     BistHdrLatCache;// BIST, Header Type, Latency Timer and Cache Line Size Registers
    VUINT32     Bar[6];         // Base Address Register
    VUINT32     Res_1;
    VUINT32     SubVid;         // Subsystem Vendor ID and Subsystem ID Registers
    VUINT32     ExpROMBaseAddr; // Expansion ROM Base Address Register
    VUINT32     CapPointer;     // Capabilities Pointer
    VUINT32     Res_2;
    VUINT32     IntCtrl;        // Interrupt Line and Interrupt Pin Register
    VUINT32     Res_3[16];

    // Power Management Capability Structure

    VUINT32     PwrCapReg;      // Power Management Capabilities Register
    VUINT32     PwrCtrlStatReg; // Power Management Control/Status Register
    VUINT32     Res_4[2];

    // MSI Capability Structure

    VUINT32     MsiCtrlReg;     // MSI Control Register
    VUINT32     MsiLowAddr;     // MSI Message Low Address Register
    VUINT32     MsiHighAddr;    // MSI Message High Address Register
    VUINT32     MsiDataReg;     // MSI Message Data Register
    VUINT32     MsiMaskReg;     // MSI Mask Register
    VUINT32     MsiPendInt;     // MSI Pending Interrupt Register
    VUINT32     Res_5[2];

    // MSI-X Capability Structure

    VUINT32     MsiXCtrlReg;    // MSI-X Control Register
    VUINT32     MsiXTableOffs;  // MSI-X Table Offset
    VUINT32     MsiXPBAOffs;    // MSI-X PBA Offset
    VUINT32     Res_6[1];

    // PCI Express Capability Structure
    VUINT32     PCIeCapListReg; // PCI Express Capability List Register
    VUINT32     PCIeDevCapReg;  // PCI Express Device Capabilities Register
    VUINT32     PCIeDevCtrlStat;// PCI Express Device Control and Status Registers
    VUINT32     PCIeLinkCap;    // Link Capabilities Register
    VUINT32     PCIeLinkCtrlStat;//Link Control and Status Registers
    VUINT32     Res_7[4];
    VUINT32     PCIeDevCapReg2; // PCI Express Device Capabilities 2 Register
    VUINT32     PCIeDevCtrlStat2;//PCI Express Device Control and Status Registers
    VUINT32     Res_8;
    VUINT32     PCIeLinkCtrlStat2;//PCI Express Link Control and Status 2 Registers
    VUINT32     Res_9[3];

    // Advanced Error Reporting (AER) Capability Structure

    VUINT32     AEREnhCapHdrReg;// PCI Express Enhanced Capability Header Register
    VUINT32     AERUncorrErrors;// Uncorrectable Error Status Register
    VUINT32     AERUncorrMask;  // Uncorrectable Error Mask Register
    VUINT32     AERUncorrSeverity;// Uncorrectable Error Severity Register
    VUINT32     AERCorrErrors;  // Correctable Error Status Register
    VUINT32     AERCorrMask;    // Correctable Error Mask Register
    VUINT32     AERAdvErrCap;   // Advanced Error Capabilities and Control Register
    VUINT32     AERHeaderLog[4];// Header Log Register

} PCIEREGS, *PPCIEREGS;

/** @brief The definition of the PCI configuration registers for Header Type 1 (used by PCI/PCIe bridges) */

typedef struct tagPCIEHDR1REGS
{
    // Address Map for Configuration Register Sets (Header Type 1)

    VUINT32     VDid;           /**< @brief 00: Vendor ID and Device ID Register */
    VUINT32     CmdStat;        /**< @brief 01: Command and Status Register */
    VUINT32     RevIDClassID;   /**< @brief 02: Revision ID and Class Code Register */
    VUINT32     BistHdrLatCache;/**< @brief 03: BIST, Header Type, Latency Timer and Cache Line Size Registers */
    VUINT32     Bar[2];         /**< @brief 04-05: Base Address Registers 0-1 */
    VUINT32     BusConfig;      /**< @brief 06: Primary, Secondary and Subordinate bus config */
    VUINT32     SecStatusAndIO; 
    VUINT32     MemLimitAndBase;
    VUINT32     PrefetchableMemLimitAndBase;
    VUINT32     PrefetchableUpperBase;  /**< @brief 10: Prefetchable Base  upper 32 bits */
    VUINT32     PrefetchableUpperLimit; /**< @brief 11: Prefetchable Limit upper 32 bits */
    VUINT32     IOUpperBaseAndLimit;    /**< @brief 12: I/O Limit and Base upper 16 bits */
    VUINT32     CapPointer;             /**< @brief 13: Capabilities Pointer */
    VUINT32     ExpROMBaseAddr;         /**< @brief 14: Expansion ROM Base Address */
    VUINT32     BridgeAndIntCtrl;       /**< @brief 15: Bridge control, Interrupt Line and Interrupt Pin Register */
} PCIEHDR1REGS, *PPCIEHDR1REGS;

/** @brief The ROOT COMPLEX registers */

typedef struct tagPCIERCREGS
{
    VUINT32     Res1[4];
    VUINT32     BAR[6];             // RC BAR
    VUINT32     Res2[0x31-0x0A];
    VUINT32     DevCapReg;          // PCI Express Device Capabilities Register
    VUINT32     DevCtrlStatReg;     // PCI Express Device Control and Status Registers
    VUINT32     LinkCapReg;         // Link Capabilities Register
    VUINT32     LinkCtrlStatReg;    // Link Control and Status
    VUINT32     Res3[4];
    VUINT32     DevCapReg2;         // PCI Express Device Capabilities 2 Register
    VUINT32     DevCtrlStatReg2;    // PCI Express Device Control and Status 2 Registers
    VUINT32     Res4;
    VUINT32     LinkCtrlStatReg2;   // PCI Express Link Control and Status 2 Registers
    VUINT32     Res5[3];            // [0x3D - 0x3F]
    VUINT32     EnhCapHdrReg;       // PCI Express Enhanced Capability Header Register
    VUINT32     UncorrErrStatReg;   // Uncorrectable Error Status Register
    VUINT32     UncorrErrMaskReg;   // Uncorrectable Error Mask Register
    VUINT32     UncorrSeverityReg;  // Uncorrectable Error Severity Register
    VUINT32     CorrErrStatReg;     // Correctable Error Status Register
    VUINT32     CorrErrMaskReg;     // Correctable Error Mask Register
    VUINT32     AdvErrCapReg;       // Advanced Error Capabilities and Control Register
    VUINT32     HdrLog[4];          // Header Log Register
    VUINT32     RootErrCmdReg;      // Root Error Command Register
    VUINT32     RootErrStatReg;     // Root Error Status Register
    VUINT32     ErrSrcId;           // Error Source Identification Register
} PCIERCREGS, *PPCIERCREGS;

/** @brief The definition of the LOCAL registers */

typedef struct tagPCIELOCREGS
{
    VUINT32     PhyLayer0;      // Physical Layer Configuration Register 0
    VUINT32     PhyLayer1;      // Physical Layer Configuration Register 1
    VUINT32     DataLinkTimerCfg; // Data Link Layer Timer Configuration Register
    VUINT32     RxCreditLim0;   // Receive Credit Limit Register 0
    VUINT32     RxCreditLim1;   // Receive Credit Limit Register 1
    VUINT32     TxCreditLim0;   // Transmit Credit Limit Register 0
    VUINT32     TxCreditLim1;   // Transmit Credit Limit Register 1
    VUINT32     UpdateInterval0;// Credit Update Interval Configuration Register 0
    VUINT32     UpdateInterval1;// Credit Update Interval Configuration Register 1
    VUINT32     LOSTimeoutLim;  // L0S Timeout Limit Register
    VUINT32     TxTLPCount;     // Transmit TLP Count Register
    VUINT32     TxTLPPayload;   // Transmit TLP Payload Dword Count Register
    VUINT32     RxTLPCount;     // Receive TLP Count Register
    VUINT32     RxTLPPayload;   // Receive TLP Payload Dword Count Register
    VUINT32     CompTimeoutLim; // Completion Timeout Limit Register
    VUINT32     VendorID;       // Vendor ID Register
    VUINT32     L1State;        // L1 State Re-entry Delay Register
    VUINT32     MemBase;        // Address base for master memory address translation
    VUINT32     IOBase;         // Address base for master I/O address translation
    VUINT32     CfgBase;        // Address base for master configuration address translation
    VUINT32     Res_1[0x6E];   // [0x14 - 0x81]

    VUINT32     DbgMuxCtrl;     // Debug Mux Control Register
    VUINT32     ErrStatus;      // Local Error Status Register
    VUINT32     IntMask;        // Local Interrupt Mask Register
    VUINT32     LCRCErrCount;   // LCRC Error Count Register
    VUINT32     Res_2[0xA];    // [0x86 - 0x8F]
    VUINT32     PhyBarCfg[2];   // PF BAR Configuration Register
    VUINT32     Res_3[0x2E];   // [0x92-0xBF]
    VUINT32     RootBarCfg;     // RC BAR Configuration Register
    VUINT32     Res_4[0xF];    // [0x0C1 – 0x0CF]
    VUINT32     PhyCfg;         // Physical Function Configuration Register
    VUINT32     Res_5[0x32F];  // [0x0D1 – 0x3FF]
} PCIELOCREGS, *PPCIELOCREGS;

typedef enum
{
    PCIE_INT_NONE = 0,
    PCIE_INT_LEGACY,
    PCIE_INT_MSI
} PCIE_INT_MODE;

typedef enum
{
    PCIE_INTPIN_NONE = 0,
    PCIE_INTPIN_INTA,
    PCIE_INTPIN_INTB,
    PCIE_INTPIN_INTC,
    PCIE_INTPIN_INTD,
} PCIE_INT_PIN;

/** @brief The definition of the PCI EP config*/

typedef struct tagPCI_DEV
{
    UINT16  FuncNum;            /**< @brief PCIe function number */
    UINT16  VendorID;           /**< @brief Vendor ID of the device */

    UINT16  DeviceID;           /**< @brief Device ID assigned by the manufacturer of the device */
    UINT16  Class;              /**< @brief Class code */

    UINT16  SubClass;           /**< @brief Sub Class code */
    UINT16  RevID;              /**< @brief RevisionID */

    UINT32  IntPin;             /**< @brief Interrupt Pin */

    UINT32  BarSize[6];         /**< @brief Detected size of BAR[] */
    BOOL  BarIsMem[6];          /**< @brief Detected Bar is Mem (not IO) */

    UINT32  PmCap;              /**< @brief PM capability offset in the configuration space */
    UINT32  MSICap;             /**< @brief MSI capability offset in the configuration space */
    UINT32  MSIXCap;                /**< @brief MSIX capability offset in the configuration space */
    UINT32  PCIeCap;                /**< @brief PCIe capability offset in the configuration space */

    UINT16  CurPwrState;            /**< @brief Current operating state. In ACPI-speak, this is D0-D3, D0 being fully functional (0x00),
                                    and D3 (0x03) being off.  */
    PCIE_INT_MODE IntMode;      /**< @brief Interrupt Mode (0 - Legacy Mode, 1 - MSI mode)  */
} PCI_DEV, *PPCI_DEV;

/** @brief The definition of the driver context */

typedef void (*PCIEIRQCB)(UINT32 irq, void *pData);

typedef struct tagPCIEDRVCTX
{
    UINT32  Initialized;
    UINT32  MSIEnabled;
    UINT32  MasterCpu;
    UINT32  CtrlReg;    // PCIe core address
    UINT32  Mode;       // 0 - RC and 1 - EP
    UINT32  Lanes;      // 1 lane, 2 lanes, or 4 lanes
    UINT32  BaseMem;    // Base address of shared memory
    UINT32  BaseIO;     // Base address of IO space
    UINT32  BaseCfg;    // Base address of Cfg space
    UINT32  SysBaseMem;

    UINT32  INTPin;

    PCIEIRQCB IrqCB;
    void * pIrqData;

    PCIEIRQCB MIrqCB;
    void * pMIrqData;

    PCI_DEV PciDev;
    PCIEREGS EpRegs;

    UINT32  TestData;
} PCIEDRVCTX, *PPCIEDRVCTX;


/** @brief PCIe driver configuration parameters  */

typedef struct tagPCIECFG
{
    UINT32  Mode;       // 1 - RC and 0 - EP
    UINT32  Lanes;      // 1 lane, 2 lanes, or 4 lanes
    UINT32  BaseMem;    // Base address
    UINT32  SerDesID;
    UINT32  MSICfg;     // 1 - to configure Message signalled Interrupts (MSI) Capability

    void (*IrqCB) (UINT32 nIrq, void * pData);
    void * pIrqData;

    void (*MIrqCB) (UINT32 nIrq, void * pData);
    void * pMIrqData;
} PCIECFG, *PPCIECFG;

/** @brief This function initializes and configures driver,
    it should be called first before usage of any driver API

    @param pCfg[in] - configuration parameters
    @return [MXRC] an error code */
MXRC PCIeDrvInit(PCIECFG * pCfg);

/** @brief This function just disables PCIe driver and set HW block under reset
    @return [MX_OK] */

MXRC PCIeDrvShutdown (void);

/** @brief  This function just stops device and does not free any resources
 *          like allocated memory. It's needed to save allocated resources for
 *          the futher debugging and do not touch potentially corrupted heaps

 @return [MX_OK] */

MXRC PCIeDrvGenShedding (void);

/** @brief This function checks connection status
    @return [MXRC] an error code of status */

MXRC PCieDrvGetLinkStatus(void);

/** @brief This function sets connection between RC and EP
    @param timeout [in] - timeout in ms of waiting
    @return [MXRC] an error code of status */

MXRC PCIeDrvConnect(UINT32 timeout);
#ifdef Korean_BMT_RF_PCIE
MXRC PCIeSrdsDrvConnect(UINT32 timeout, UINT32 SerDesID);
#endif

/** @brief This function select special bit in EVM FPGA
           device to activate master access
    @return [MXRC] an error code */

MXRC PCIeDrvSelectMaster (void);

/** @brief This function sets the Irq or MIrq callbacks

    @param IsMIRQ  [in] - 1 if this is for MIRQ or 0 if this is for IRQ
    @param irq_cb    [in] - The callback
    @param pData    [in] - data associated with callback
*/

void PCIeDrvSetIrqCb(BOOL IsMIRQ, PCIEIRQCB irq_cb, void *pData);

/** @brief This function is designed to be used with EndPoint PCIe configuration
           to generate IRQ-A, B, C, D

    @param pCtx     [in] - the driver context
    @param IrqNum   [in] - IRQ number
    @param bWaitAck [in] - the wait the ack

    @return [UINT32] error code */

UINT32 PCIeDrvGenIrq(LPVOID pCtx, UINT32 irqNum, BOOL bWaitAck);

/** @brief This function is designed to be used with EndPoint PCIe configuration
           to clear IRQ-A, B, C, D

    @param pCtx     [in] - the driver context
    @param IrqNum   [in] - IRQ number
    @param bWaitAck [in] - the wait the ack

    @return [UINT32] error code */
    
UINT32 PCIeDrvClearIrq(LPVOID pCtx, UINT32 irqNum, BOOL bWaitAck);

/** @brief This function selects PCI FunctionNumber,
    @Note: Please use 0x100 to select local management

    @param nFNum [in] - the Function number

    @return [MXRC] an error code */

MXRC PCIeDrvSelectFNum (UINT32 nFNum);

/** @brief This function reads PCIe registers selected by PCIeDrvSelectFNum,

    @param nIndex  [in]  - register index in the HW registers list
    @param pRegOut [out] - the pointer to registers structure of:
                                PCIELOCREGS, PCIERCREGS or PCIEREGS

    @param nRegNum [in] - the number of required registers

    @return [MXRC] an error code */

MXRC PCIeDrvReadCtrlReg (UINT32 nIndex, PUINT32 pRegOut, UINT32 nRegNum);

/** @brief This function writes PCIe registers selected by PCIeDrvSelectFNum,

    @param nIndex  [in]  - register index in the HW registers list
    @param pReg [in] - the pointer to registers structure of:
                                PCIELOCREGS, PCIERCREGS or PCIEREGS

    @param nRegNum [in] - the number of required registers

    @return [MXRC] an error code */

MXRC PCIeDrvWriteCtrlReg (UINT32 nIndex, PUINT32 pReg, UINT32 nRegNum);

UINT32 PCIeDrvReadMemory (UINT32 addr, PUINT32 pOut, UINT32 nWords);
UINT32 PCIeDrvWriteMemory (UINT32 addr, PUINT32 pIn, UINT32 nWords);
MXRC PCIeDrvDebug (UINT32 Type);
void PCIeDrvPrintUARTReg(void);

/** @brief This function just reads all configuration register (FNum=0)
    @param pRegs [out] - the pointer to the local storage
    @return [MXRC] an error code */

MXRC PCIeDrvReadRCConfRegs(PPCIERCREGS pRegs);

/** @brief This function just reads all local management register (FNum=0x100)
    @param pRegs [out] - the pointer to the local storage
    @return [MXRC] an error code */

MXRC PCIeDrvReadLocMgtRegs(PPCIELOCREGS pRegs);

/** @brief This function checks/enumerates the EP on the bus
    @return [UINT32] number of devices found */

UINT32 PCIeDrvScan(void);

/** @brief This function enables/disables PCI bus mastering on an Endpoint
    @param Enable [in]  - PCI bus mastering on/off
    @return [] none */

void PCIeDrvSetMaster(BOOL Enable);

/** @brief This function initialize device before it's used by a driver
    @param pDev [in]  - Pointer to the PCI_DEV structure
    @return [MXRC] error code */

MXRC PCIeDrvEnableDevice(PPCI_DEV pDev);

void PcieMmuCfg(UINT32 nCpu, UINT32 nParam);

#endif /*__PCIEDRV_H__*/

#ifdef __cplusplus
}
#endif

