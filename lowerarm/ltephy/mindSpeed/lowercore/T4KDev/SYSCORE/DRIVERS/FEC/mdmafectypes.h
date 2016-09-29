//-------------------------------------------------------------------------------------------
/** @file mdmatypes.h
 *
 * @brief MDMA (Memory DMA) types definition
 * @author Mindspeed Technologies
 * @version $Revision: 1.8 $
 *
 * COPYRIGHT(c) 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef __MDMA_TYPE_H__
#define __MDMA_TYPE_H__


/*************************************************************************
*                     Configuration parameters                           *
*************************************************************************/
#define MDMA_BDESC_NUM          4      // total number of Buffer Descriptors in Frame Descriptor
#define MAX_DMA_BDESC           16
#define MAX_CEVA_CB_DONE_NUM    16

#define MAX_DL_IO_BDESC         2
#define MAX_UL_IO_BDESC         16

#define MDMA_CEVA_CB_DONE_PROC  0xC0000000
#define FEC_CEVA_CB_DONE_PROC   0xC0000000

#define SYS_MDMA_HW_SEMA        10
#define SPU_MDMA_HW_SEMA        11
#define RAD_MDMA_HW_SEMA        12
#define FEC_DL_HW_SEMA          13
#define FEC_UL_HW_SEMA          14

#define BCTRL_LEN_MASK          0x0000FFFF
#define BCTRL_BLAST_MASK        0x00010000

#define DMAFCTRL_IRQEN          0x00000001
#define DMAFCTRL_INBOFF         0x0000FFF0

// inbound/outbound combined descriptor mode
#define DMA_FCOM_MODE           0x00000004

#define DMAIRQ_FREADY           (1 << 0)
#define DMAIRQ_FLAST            (1 << 1)
#define DMAIRQ_FDONE            (1 << 2)
#define DMAIRQ_FCTRL            (1 << 6)

#define FDESC_INTENA            0x1

#define MDMA_GET_HW(desc) ((MDMA_HW *)&(((MDMA_FDESC*)(desc))->HwContext))

typedef enum
{
    MDMA_HW_SYS = 0,
    MDMA_HW_SPU,
    MDMA_HW_RAD,

    MDMA_HW_MAX
} MDMA_HW_ID;

typedef enum
{
    MDMA_OP_COPY        = 0x0000,
    MDMA_OP_HSDSCH      = 0x1000,
    MDMA_OP_CRC         = 0x2000,
    MDMA_OP_RAND        = 0x3000,
    MDMA_OP_3GDLSPCG    = 0x4000,
    MDMA_OP_3GULSPCG    = 0x5000,
    MDMA_OP_4SCR        = 0xe000,
    MDMA_OP_4DSC        = 0x7000,
    MDMA_OP_RAND_BE     = 0x8000 | MDMA_OP_RAND,
    MDMA_OP_4SCR_BE     = 0x8000 | MDMA_OP_4SCR

} MDMA_OP_TYPE;

#ifndef _DMABDESC_
#define _DMABDESC_
typedef struct tDMABDESC
{
    VUINT32     BPtr;
    VUINT32     BCtrl;
} DMABDESC, *PDMABDESC;
#endif // _DMABDESC_

#ifndef _DMAFDESC_
#define _DMAFDESC_
typedef struct tDMAFDESC
{
    struct  tDMAFDESC   *NextFDesc;
    VUINT32             FControl;
    VUINT32             FStatus0;
    VUINT32             FStatus1;
    DMABDESC            OutBDesc;
    DMABDESC            InBDesc;
} DMAFDESC, *PDMAFDESC;
#endif // _DMAFDESC_

typedef struct tDMA_CHAN_CTRL
{
    VUINT32     Control;
    VUINT32     HeadFDesc;
    VUINT32     BurstLength;
    VUINT32     FrameLength;
    VUINT32     DmaIrqEnable;
    VUINT32     IrqStatusClear;
    VUINT32     Reserved[2];
    VUINT32     SoftReset;
    VUINT32		DiagRegs[23];
} DMA_CHAN_CTRL, *PDMA_CHAN_CTRL;

typedef struct tDMA_CHAN_CTRL_OPT
{
    VUINT32     DiagRegs[64];
    VUINT32     MemToIOFIFO;
    VUINT32     IOToMemFIFO;
    VUINT32     Reserved[2];
    VUINT32     AXIErrDmaIrqEnable;
    VUINT32     AXIErrIrqStatusClear;
    VUINT32     AXIOutReadAllowed;
    VUINT32	    DiagRegs2[57];
} DMA_CHAN_CTRL_OPT, *PDMA_CHAN_CTRL_OPT;

typedef struct tMDMA_CTRL_REGS
{
    DMA_CHAN_CTRL   DMAInCtrl;
    DMA_CHAN_CTRL   DMAOutCtrl;

#ifdef MDMA_AXI_BUS_ERROR_PROCESS
    DMA_CHAN_CTRL_OPT DMAOptCtrl;
#endif // #ifdef MDMA_AXI_BUS_ERROR_PROCESS

} MDMA_CTRL_REGS, *PMDMA_CTRL_REGS;

typedef struct tMDMAFDESC
{
    struct tDMAFDESC   *NextFDesc;
    VUINT32             FControl;
    VUINT32             FStatus0;
    VUINT32             FStatus1;
    DMABDESC            InOutBDesc[MAX_DMA_BDESC];

} MDMAFDESC, *PMDMAFDESC;

typedef struct tMDMAHWCTX
{
// -----------32bits WORD-1 ------------
    volatile unsigned IRQEN         :1;      // 0
    volatile unsigned W1_Unused1    :3;      // 3:1
    volatile unsigned BOffset       :8;      // 11:4
    volatile unsigned CalcType      :4;      // 15:12
    volatile unsigned UserDefined_2 :16;     // 31:16
// -----------32bits WORD-2 ------------
    volatile unsigned UserDefined_0;         // 31:0
// -----------32bits WORD-3 ------------
    volatile unsigned M2DBdesc      :8;      // 7:0
    volatile unsigned D2MBdesc      :8;      // 15:8
    volatile unsigned UserDefined_1 :15;     // 30:16
    volatile unsigned StatusComp    :1;      // 31

} MDMAHWCTX, *PMDMAHWCTX;

typedef struct tMDMA_HW
{
    VUINT32             FControl;
    VUINT32             FStatus0;
    VUINT32             FStatus1;

}MDMA_HW, *PMDMA_HW;

typedef void (*MdmaCbDoneType)(LPVOID ctx); // callback routine
typedef void (*FecCbDoneType)(LPVOID ctx); // callback routine

typedef struct tMDMA_FDESC
{
    struct tMDMA_FDESC *Next;
    MDMAHWCTX           HwContext;
    DMABDESC            IoBdesc[MDMA_BDESC_NUM];

    MdmaCbDoneType      CbDone;         // callback routine
    LPVOID              CbContext;      // callback context
    UINT32              OwnerCpuID;

} MDMA_FDESC, *PMDMA_FDESC;

/** @brief This type defines a structure to control active and non-active
           lists and to handle HW MDMA RUN operations, it's needed to
           work with MDMA from two processors ARM and CEVA,
           in this case such object has to be located in non-cacheable memory (CRAM for example)*/

typedef struct _MDMA_LIST_INF_
{
    volatile PMDMA_FDESC    pFDescActive;
    volatile PMDMA_FDESC    pFDescInactive;
    VUINT32                 HWBusy;
    VUINT32                 StartTimeTicks;
    VUINT32                 ActiveTotalTicks;
    VUINT32                 InactiveTotalTicks;

    VUINT32                 CbDonePut;
    VUINT32                 CbDoneGet;
    VUINT32                 CbDone    [MAX_CEVA_CB_DONE_NUM];
    void *                  CbDonePtr [MAX_CEVA_CB_DONE_NUM];

}MDMA_LIST_INF, *PMDMA_LIST_INF;


/******************************************************************************
*                FEC module types definition                                  *
*******************************************************************************/
typedef struct tDLHWCTX
{
//-------------- W1 -----------------
    volatile unsigned   IRQEN       :1;     // 0
    unsigned            W1_Unused1  :3;     // 3:1
    unsigned            BOffset     :8;     // 11:4
    unsigned            W1_Unused2  :3;     // 14:12
    unsigned            E_Param     :17;    // 31:15
//-------------- W2 ------------------
    unsigned            CTC_CCn     :1;     // 0
    unsigned            CRC_Sel     :3;     // 3:1
    unsigned            LTERateMaching_K0:14; // 17:4
    unsigned            BlockSize   :13;    // 30:18
    unsigned            Reserved    :1;     // 31
//-------------- W3 ------------------
    unsigned            spid        :2;     // 1:0
    unsigned            Modulation  :2;     // 3:2
    unsigned            Ncb_14_13   :2;     // 5:4
    unsigned            Bypass_Encoder:1;   // 6
    unsigned            Bypass_Interleaver:1; // 7
    unsigned            Rate_Index  :3;     // 10:8
    unsigned            Harq_spid   :2;     // 12:11, LTE: Scramble_mask
    unsigned            Harq_rep_end:1;     // 13,    LTE: Scramble_en
    unsigned            Harq_req    :1;     // 14,    LTE: BypassRateMatching
    unsigned            Harq_en     :1;     // 15
    unsigned            Reserved_1  :1;     // 16
    unsigned            Multi_FEC_blk:1;    // 17
    unsigned            Filler      :13;    // 30:18
    volatile unsigned   StatusComp  :1;     // 31

} DLHWCTX, *PDLHWCTX;

typedef struct tULHWCTX
{
//-------------- W1 -------------------
    unsigned            IRQEN       :1;     // 0
    unsigned            W1_Unused1  :3;     // 3:1
    unsigned            BOffset     :8;     // 11:4
    unsigned            W1_Unused2  :3;     // 14:12
    unsigned            E_Param     :17;    // 31:15
//-------------- W2 -------------------
    unsigned            CTC_CCn     :1;     // 0
    unsigned            CRC_Sel     :3;     // 3:1
    unsigned            LTERateMaching_K0:14; // 17:4
    unsigned            BlockSize   :13;    // 30:18
    unsigned            Reserved    :1;     // 31
//-------------- W3 -------------------
    unsigned            Modulation  :2;     // 1:0
    unsigned            HalfInteration:6;   // 7:2
    unsigned            Rate_Index  :3;     // 10:8
    unsigned            Enable_Combine:1;   // 11
    unsigned            Bypass_Decode_Enable:2;     // 13:12
    unsigned            Bypass_Rate_Match:1;        // 14
    unsigned            Dynamic_Stop_Threshold:2;   // 16:15
    unsigned            Dynamic_Stop_Iteration:1;   // 17
    unsigned            Filler      :13;            // 30:18
    unsigned            StatusComp  :1;             // 31

} ULHWCTX, *PULHWCTX;

// FEC config registers
typedef struct tFEC_CONFIG_REGS
{
// config0
    unsigned wimax_lte:1;               // [0]
    unsigned reserved_0:15;             // [15:1]
    unsigned loopback_en_0:1;           // [16]
    unsigned loopback_en_1:1;           // [17]
    unsigned bp_channel_interleaver:1;  // [18] LTE UL
    unsigned bp_rate_match:1;           // [19] LTE UL
    unsigned bp_block_interleaver:1;    // [20] WiMAX UL
    unsigned bp_subblock_interleaver:1; // [21] LTE UL CC
    unsigned bp_harq_soft_combine:1;    // [22] UL
    unsigned bp_decoder:1;              // [23]
    unsigned ci_symbol_number:1;        // [24] LTE UL
    unsigned convergence_length:3;      // [27:25] LTE UL CC
    unsigned bp_k0:1;                   // [28]
    unsigned harq_zerolen:1;            // [29]
    unsigned reserved_1:2;              // [31:30]
// config1
    unsigned scrambler_mask_sel:1;      // [0]
    unsigned reserved_2:3;              // [3:1]
    unsigned scrambler_mask_b:12;       // [15:4]
    unsigned reserved_3:4;              // [19:16]
    unsigned scrambler_mask_a:12;       // [31:20]
// config2
    unsigned dyn_stop_thresh_0:8;       // [7:0]
    unsigned dyn_stop_thresh_1:8;       // [15:8]
    unsigned dyn_stop_thresh_2:8;       // [23:16]
    unsigned dyn_stop_thresh_3:8;       // [31:24]
// config3
    unsigned config3:32;                // [31:0]
// status0
    unsigned tc1000_monitor:8;          // [7:0]
    unsigned reserved_4:8;              // [15:8]
    unsigned tc1000_idle:1;             // [16]
    unsigned tc1000_error:2;            // [18:17]
    unsigned tc1000_warning:3;          // [21:19]
    unsigned tc1000_status:3;           // [25:22]
    unsigned reserved_5:7;              // [31:26]
// status1
    unsigned tc1700_monitor:8;          // [7:0]
    unsigned reserved_6:8;              // [15:8]
    unsigned tc1700_idle:1;             // [16]
    unsigned tc1700_crc_error:1;        // [17]
    unsigned reserved_7:1;              // [18]
    unsigned tc1700_ber:13;             // [31:19]
// Inbound burst length
    unsigned inbound_dma_burst_len;
// Outbound burst length
    unsigned outbound_dma_burst_len;

} FEC_CONFIG_REGS, *PFEC_CONFIG_REGS;

typedef struct tFEC_CTRL_REGS
{
    DMA_CHAN_CTRL   DMA0InCtrl; // DMA0 - DL
    DMA_CHAN_CTRL   DMA0OutCtrl;
    DMA_CHAN_CTRL   DMA1InCtrl; // DMA1 - UL
    DMA_CHAN_CTRL   DMA1OutCtrl;
    UINT32          Reserved[128];
    FEC_CONFIG_REGS FecConfigRegs;

} FEC_CTRL_REGS, *PFEC_CTRL_REGS;

typedef struct tFEC_DL_FDESC
{
	volatile struct tFEC_DL_FDESC * Next;
	volatile DLHWCTX                HwContext;
	volatile DMABDESC               IoBdesc[MAX_DL_IO_BDESC];
    void                            (*CbDone)(LPVOID ctx); // callback routine
    LPVOID                          CbContext;      // callback context
    UINT32                          OwnerCpuID;

} FEC_DL_FDESC, *PFEC_DL_FDESC;

typedef struct tFEC_UL_FDESC
{
	volatile struct tFEC_UL_FDESC * Next;
    volatile ULHWCTX                HwContext;
    volatile DMABDESC               IoBdesc[MAX_UL_IO_BDESC];
    void                            (*CbDone)(LPVOID ctx); // callback routine
    LPVOID                          CbContext;      // callback context
    UINT32                          OwnerCpuID;

} FEC_UL_FDESC, *PFEC_UL_FDESC;

/** @brief This type defines a structure to control active and non-active
           lists and to handle HW FEC-DL RUN operations, it's needed to
           work with FEC-DL from two processors ARM and CEVA,
           in this case such object has to be located in non-cacheable memory (CRAM for example)*/

typedef struct _FECDL_LIST_INF_
{
    volatile PFEC_DL_FDESC  pFDescActive;
    volatile PFEC_DL_FDESC  pFDescInactive;
    volatile BOOL           bHwBusy;
    VUINT32                 StartTimeTicks;
    VUINT32                 ActiveTotalTicks;
    VUINT32                 InactiveTotalTicks;

    VUINT32                 CbDonePut;
    VUINT32                 CbDoneGet;
    VUINT32                 CbDone    [MAX_CEVA_CB_DONE_NUM];
    void *                  CbDonePtr [MAX_CEVA_CB_DONE_NUM];

}FECDL_LIST_INF, *PFECDL_LIST_INF;

/** @brief This type defines a structure to control active and non-active
           lists and to handle HW FEC-UL RUN operations, it's needed to
           work with FEC-UL from two processors ARM and CEVA,
           in this case such object has to be located in non-cacheable memory (CRAM for example)*/

typedef struct _FECUL_LIST_INF_
{
    volatile PFEC_UL_FDESC  pFDescActive;
    volatile PFEC_UL_FDESC  pFDescInactive;
    volatile BOOL           bHwBusy;
    VUINT32                 StartTimeTicks;
    VUINT32                 ActiveTotalTicks;
    VUINT32                 InactiveTotalTicks;

    VUINT32                 CbDonePut;
    VUINT32                 CbDoneGet;
    VUINT32                 CbDone    [MAX_CEVA_CB_DONE_NUM];
    void *                  CbDonePtr [MAX_CEVA_CB_DONE_NUM];

}FECUL_LIST_INF, *PFECUL_LIST_INF;

#endif // __MDMA_TYPE_H__

#ifdef __cplusplus
}
#endif

