//-------------------------------------------------------------------------------------------
/** @file tcb.h
 *
 * @brief TCB structures and constants
 * @author Mindspeed Technologies
 * @version $Revision: 1.47 $
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

#ifdef __CC_ARM     // ARM compiler
#include "config.h"
#endif

#ifndef _TCB_H_
#define _TCB_H_

#define MAX_IOBUF_DESC      16
#define REX_DEP_SIZE        18

/*************************************************************************************************
*           TCB::STATUS configuration and status bits                                            *
**************************************************************************************************/
#define TCB_CFG_STAT_ERROR_MASK     (0xFF)      // The error mask, used to get error code of operation(task processing)
#define TCB_CFG_STAT_GET_ERROR(tcb)     ((tcb)->Status & TCB_CFG_STAT_ERROR_MASK)
#define TCB_CFG_STAT_DIS_INT        (1<<16)     // To inform CEVA to prevent generating of interrupts when TCB is done
#define TCB_CFG_STAT_MARK_DONE      (1<<17)     // CEVA marks this TCB(mainly used for SuperTCB) like done if TCB_CFG_STAT_DIS_INT was used


typedef struct tIOBufDesc
{
    void *IOBufPtr;
    unsigned int IOBufControl;
} TIOBufDesc, *PTIOBufDesc;

#define TCB_NEXT(tcb)        (tcb)->NextTcb
#define TCB_CONTEXT(tcb)     (tcb)->ContextPtr
#define TCB_CONTEXT_LEN(tcb) (tcb)->ContextLen
#define TCB_CONTROL(tcb)     (tcb)->IOControlPtr
#define TCB_CONTROL_LEN(tcb) (tcb)->IOControlLen
#define TCB_INPUT(tcb)       (tcb)->InputDataPtr
#define TCB_INPUT_LEN(tcb)   (tcb)->InputDataLen
#define TCB_OUTPUT(tcb)      (tcb)->OutputDataPtr
#define TCB_OUTPUT_LEN(tcb)  (tcb)->OutputDataLen
#define TCB_TASK_ID(tcb)     (tcb)->TaskID
#define TCB_RES_ID(tcb)      (tcb)->ResourceID
#define TCB_STATUS(tcb)      (tcb)->Status
#define TCB_CBDONE(tcb)      (tcb)->cbDone
#define TCB_GROUP_ID(tcb)    (tcb)->GroupID
#define TCB_RES_IDX(tcb)     (tcb)->ResourceIndex
#define TCB_EXEC_TICKS(tcb)  (tcb)->ExecTicks
#define TCB_SUP_TCB(tcb)     (tcb)->NextSupTcb
#define TCB_HW(tcb)          (tcb)

typedef struct _TCB_SUB_TASK_INFO_
{
    unsigned int        TaskID;
    unsigned int        TaskMips;

}TCB_SUB_TASK_INFO;

#if defined (CEVA_INTRINSICS) && !defined (EVM) || defined (_WIN32)
typedef struct tTCB
{
    //*****************<  HW BLOCK BEGIN  >*************************
    // CACHELINE-1
    struct tTCB *NextTcb;
    unsigned int TaskID;
    unsigned int ResourceID;
    volatile unsigned int Status;
    void (* cbDone)(struct tTCB *Tcb);
    void *ContextPtr;
    unsigned int ContextLen;
    void *IOControlPtr;

    // CACHELINE-2
    unsigned int IOControlLen;
    void *InputDataPtr;
    unsigned int InputDataLen;
    void *OutputDataPtr;
    unsigned int OutputDataLen;
    unsigned short ResourceIndex; //RIX is used to index which resource (ceva:0-9, MAP:0-9) are allocated to this tcb
    unsigned short GroupID; //GID super Tcb group ID , FFFF is reserved,
    void *DependencyList;

    // CACHELINE-3
    unsigned int ExecTicks;
    struct tTCB *NextSupTcb; //THIS IS THE LAST FIELD OF TCB//pointer to linked list of tasks belonging to same superTCB group
    //*****************<  HW BLOCK END  >*************************

    //new.. below are needed for scheduler
    unsigned int TaskPri; //task priority
    unsigned int TaskMIPS; //task mips used in calculating a PU load factor.
    struct tTCB *PrevTcb; //backward link for the task list
    void *SeedList;
    unsigned short isym; // symbol num to be used by new IFFT driver
    unsigned short SubTaskTimer; //to be set by PHY and used by new FFT driver
    unsigned int TaskControl; //bit field (TBD) that defines special handling for a tcb

} TCB, *PTCB;

#else // REX_ENABLED

typedef struct tTCB
{
    struct tTCB *           NextTcb;
    unsigned int            TaskID;
    unsigned int            ResourceID;
    volatile unsigned int   Status;
    void                    (*cbDone)(struct tTCB *Tcb);
    void *                  ContextPtr;
    unsigned int            ContextLen;
    void *                  IOControlPtr;
    unsigned int            IOControlLen;
    void *                  InputDataPtr;
    unsigned int            InputDataLen;
    void *                  OutputDataPtr;
    unsigned int            OutputDataLen;
    unsigned short          ResourceIndex;  //RIX is used to index which resource (ceva:0-9, MAP:0-9) are allocated to this tcb
    unsigned short          GroupID;        //GID super Tcb group ID , FFFF is reserved,
    void *                  DependencyList;
    unsigned int            ExecTicks;
    struct tTCB *           NextSupTcb;     //THIS IS THE LAST FIELD OF TCB//pointer to linked list of tasks belonging to same superTCB group

    unsigned short          isym;           // symbol num to be used by new IFFT driver
    unsigned short          SubTaskTimer;   // to be set by PHY and used by new FFT driver
    unsigned int            SubTaskNum;     // On input: the maximum number of elements that can be putted to the array pointed by SubTaskData
                                            // On out:  the number of elements already putted by PHY code
    TCB_SUB_TASK_INFO *     SubTaskinfo;    // The address of the storage where code may add sub-task information
    void*                   RexDescr;       // The pointer to the REX-TCB descriptor

}TCB, *PTCB;

typedef struct _TCB_CEVA_CALL_
{
    unsigned int            ProcAddr;
    unsigned int            TaskID;     // 
    unsigned int            Res1;       // ResourceID
    volatile unsigned int   Status;     // 

    unsigned int            ParamNum;
    unsigned int            Params[8];

}TCBCEVACALL, *PTCBCEVACALL;

#endif // REX_ENABLED

typedef struct tCEVAPROC
{
    void * 		 func;
    unsigned int res[3];
	unsigned int argnum;
	unsigned int arg[6];	 
} CEVAPROC, *PCEVAPROC;

// the size of TCB block used to communicate with HW (like CEVA)
#define HW_TCB_SIZE         (((UINT32)&((TCB*)NULL)->ExecTicks) + 8)

#ifdef REX_ENABLED
#define TCB_HW_SIZE         (96)
//#define TCB_HW_SIZE         sizeof (TCB)
#else
#define TCB_HW_SIZE         sizeof (TCB)
#endif

#define IN_BUF              0x40000000
#define OUT_BUF             0x80000000
#define INOUT_BUF           0xC0000000
#define BUF_SIZE_MASK       0x00FFFFFF
#define IOCONTR_MASK        0xFF000000
#define IO_MASK             0xc0000000

#define MAX_CEVA_FUNCTION_PROFILE 16

////////////////////////////////////////////
// define TCB Resource TYPE
////////////////////////////////////////////
#define RESOURCE_RSRV       0
#define RESOURCE_LOWERARM   1
#define RESOURCE_LARM       1 //shorter
#define RESOURCE_FLTP       2
#define RESOURCE_MAP        2 //new name for FLTP
#define RESOURCE_CEVA       3
#define RESOURCE_FEC        4
#define RESOURCE_HOST       5 //used for running tasks on HOST PC only
#define RESOURCE_MDMA       6

#define RESOURCE_FECUL      7
#define RESOURCE_FECDL      8

#define RESOURCE_XP_AXI     9
#define RESOURCE_SYS_AXI    10
#define RESOURCE_SPU_AXI    11
#define RESOURCE_RAD_AXI    12

#define RESOURCE_LARM0      1
#define RESOURCE_LARM1      13
#define RESOURCE_UARM0      14
#define RESOURCE_UARM1      15
#define RESOURCE_UARM2      16
#define RESOURCE_UARM3      17





////////////////////////////////////////////
//define TCB Status types
////////////////////////////////////////////
#define STATUS_READY        0  //tcb is not scheduleed yet
#define STATUS_QUED         3  //tcb is queued to run, should not be looked at it again
#define STATUS_COMPLETE     1  //tcb has launched and has finished execution
#define STATUS_RUNNING      2  //tcb has been launched but NOT done yet

////////////////////////////////////////////
//define TCB pri
////////////////////////////////////////////
#define TCB_DEFAULT_PRI    0x10000000   //pri goes from 0 the highest pri
#define MAX_NUM_TCB        512         //4096
#define MAX_NUM_RSRC       16

////////////////////////////////////////////////////////////////////////////////////////
// System Task IDs defined across CEVA code
////////////////////////////////////////////////////////////////////////////////////////
#define TASKID_CALL_CEVA_PROC                   0xF000

////////////////////////////////////////////////////////////////////////////////////////
// All Task IDs defined across PHY code
////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////
// 1. LTE BS Tx (DL) Tasks
////////////////////////////////////////////
#define TASKID_FEC_TX                            100
#define TASKID_TX_DLCONTROL                      101
#define TASKID_TX_SETUP_SYMB_BUFS                102
#define TASKID_TX_MULTICHAN_MODULATION           103
#define TASKID_TX_IFFT                           104
#define TASKID_TX_PAPR_RCF                       105
#define TASKID_LTE_ENODEB_DL_START_ID            TASKID_FEC_TX
#define TASKID_LTE_ENODEB_DL_END_ID              TASKID_TX_PAPR_RCF

////////////////////////////////////////////
// 2. LTE BS Rx (UL) Tasks
////////////////////////////////////////////
//Common Task for PUSCH and PUCCH
#define TASKID_RX_FFT                            130
#define TASKID_RX_RUNNING_AVG                    131
#define TASKID_RX_SNRSUBFRAME                    132

//Tasks for PUSCH
#define TASKID_RX_ULPILOT                        133
#define TASKID_RX_CHANEST_P0                     134
#define TASKID_RX_CHANEST_P1                     135
#define TASKID_RX_MULTICHAN_DEMODULATION         136
#define TASKID_RX_IDFT                           137
#define TASKID_RX_DEMAPPER                       138
#define TASKID_RX_FEC_MUX_ACK_CNTL               139
#define TASKID_RX_FEC_MUX_RI_CQI_CNTL            140
#define TASKID_FEC_RX                            141

//Tasks for PUCCH
#define TASKID_RX_ULPILOT_PUCCH                  142
#define TASKID_RX_CHANEST_PUCCH_P0               143
#define TASKID_RX_RUNNING_AVG_PUCCH              144
#define TASKID_RX_CHANEST_PUCCH                  145
#define TASKID_RX_MULTICHAN_PUCCH_DEMODULATION   146

//Tasks for SRS
#define TASKID_RX_SRS_PILOTS                     147
#define TASKID_RX_SRS                            148
#define TASKID_RX_SRS_SNR                        149

//Tasks for PRACH
#define TASKID_RX_PRACH                          150




#define TASKID_LTE_ENODEB_UL_RX_START_ID         TASKID_RX_FFT
#define TASKID_LTE_ENODEB_UL_RX_END_ID           TASKID_RX_PRACH

////////////////////////////////////////////
// 3. Others Useful Tasks
////////////////////////////////////////////
#define TASKID_COPY_DATA                         200
#define TASKID_COPYDATA                          201
#define TASKID_CLEARBUFF                         202
#define TASKID_FILEPRINT_TCB                     203
#define TASKID_MALLOC                            204
#define TASKID_MFREE                             205
#define TASKID_CPY_FROM_EXT_MEM                  206
#define TASKID_CPY_TO_EXT_MEM                    207
#define TASKID_UTIL_TASKS_START_ID               TASKID_COPY_DATA
#define TASKID_UTIL_TASKS_END_ID                 TASKID_CPY_TO_EXT_MEM

////////////////////////////////////////////
// 3.1  run CEVA function
////////////////////////////////////////////
#define TASKID_RUN_CEVA_FUNC                     0xF000


////////////////////////////////////////////
// 3a. CEVA SUBTASK PROFILE IDs
////////////////////////////////////////////

////////////////////////////////////////////
// 1. LTE BS Tx (DL) SubTasks
////////////////////////////////////////////
#define SUBTASKID_TX_DLCONTROL_EXTRACT                              304                                         // 0x0130
#define SUBTASKID_TX_DLCONTROL_DMA_IN                               305
#define SUBTASKID_TX_DLCONTROL_DMA_OUT                              306
#define SUBTASKID_TX_DLCONTROL_SDU_ENC                              307
#define SUBTASKID_TX_DLCONTROL_MAIN_CFI_PROC                        308
#define SUBTASKID_TX_DLCONTROL_MAIN_PHICH_PROC                      309
#define SUBTASKID_TX_DLCONTROL_MAIN_PDCCH_PROC                      310
#define SUBTASKID_TX_DLCONTROL_MAIN_PILOT_PROC                      311
#define SUBTASKID_TX_DLCONTROL_START                                SUBTASKID_TX_DLCONTROL_EXTRACT
#define SUBTASKID_TX_DLCONTROL_END                                  SUBTASKID_TX_DLCONTROL_MAIN_PILOT_PROC


#define SUBTASKID_TX_SETUP_SYMB_BUFS_EXTRACT                        320                                         // 0x0140
#define SUBTASKID_TX_SETUP_SYMB_BUFS_DMA_IN                         321
#define SUBTASKID_TX_SETUP_SYMB_BUFS_DMA_OUT                        322
#define SUBTASKID_TX_SETUP_SYMB_BUFS_START                          SUBTASKID_TX_SETUP_SYMB_BUFS_EXTRACT
#define SUBTASKID_TX_SETUP_SYMB_BUFS_END                            SUBTASKID_TX_SETUP_SYMB_BUFS_DMA_OUT


#define SUBTASKID_TX_MULTICHAN_MODULATION_EXTRACT                   336                                         // 0x0150
#define SUBTASKID_TX_MULTICHAN_MODULATION_DMA_IN                    337
#define SUBTASKID_TX_MULTICHAN_MODULATION_MAPPER                    338
#define SUBTASKID_TX_MULTICHAN_MODULATION_LAYERMAPPER               339
#define SUBTASKID_TX_MULTICHAN_MODULATION_PRECODER                  340
#define SUBTASKID_TX_MULTICHAN_MODULATION_DLPILOT                   341
#define SUBTASKID_TX_MULTICHAN_MODULATION_MEMSET                    342
#define SUBTASKID_TX_MULTICHAN_MODULATION_BSTXSYNCCH                343
#define SUBTASKID_TX_MULTICHAN_MODULATION_DLRESELEMMAPPER           344
#define SUBTASKID_TX_MULTICHAN_MODULATION_DMA_OUT                   345
#define SUBTASKID_TX_MULTICHAN_MODULATION_START                     SUBTASKID_TX_MULTICHAN_MODULATION_EXTRACT
#define SUBTASKID_TX_MULTICHAN_MODULATION_END                       SUBTASKID_TX_MULTICHAN_MODULATION_DMA_OUT



////////////////////////////////////////////
// 2. LTE BS Rx (UL) SubTasks
////////////////////////////////////////////
// Common SubTasks for PUSCH and PUCCH
#define SUBTASKID_RX_RUNNING_AVG_EXTRACT                            352                                         // 0x0160
#define SUBTASKID_RX_RUNNING_AVG_DMA_IN                             353
#define SUBTASKID_RX_RUNNING_AVG_DMA_OUT                            354
#define SUBTASKID_RX_RUNNING_AVG_START                              SUBTASKID_RX_RUNNING_AVG_EXTRACT
#define SUBTASKID_RX_RUNNING_AVG_END                                SUBTASKID_RX_RUNNING_AVG_DMA_OUT


#define SUBTASKID_RX_SNRSUBFRAME_EXTRACT                            368                                         // 0x0170
#define SUBTASKID_RX_SNRSUBFRAME_DMA_IN                             369
#define SUBTASKID_RX_SNRSUBFRAME_DMA_OUT                            370
#define SUBTASKID_RX_SNRSUBFRAME_START                              SUBTASKID_RX_SNRSUBFRAME_EXTRACT
#define SUBTASKID_RX_SNRSUBFRAME_END                                SUBTASKID_RX_SNRSUBFRAME_DMA_OUT



// SubTasks for PUSCH
#define SUBTASKID_RX_ULPILOT_EXTRACT                                384                                         // 0x0180
#define SUBTASKID_RX_ULPILOT_DMA_IN                                 385
#define SUBTASKID_RX_ULPILOT_DMA_OUT                                386
#define SUBTASKID_RX_ULPILOT_CONFIG_PILOTS                          387
#define SUBTASKID_RX_ULPILOT_MAIN                                   388
#define SUBTASKID_RX_ULPILOT_DEMUX                                  389
#define SUBTASKID_RX_ULPILOT_START                                  SUBTASKID_RX_ULPILOT_EXTRACT
#define SUBTASKID_RX_ULPILOT_END                                    SUBTASKID_RX_ULPILOT_DEMUX


#define SUBTASKID_RX_CHANEST_P0_EXTRACT                             400                                         // 0x0190
#define SUBTASKID_RX_CHANEST_P0_DMA_IN                              401
#define SUBTASKID_RX_CHANEST_P0_DMA_OUT                             402
#define SUBTASKID_RX_CHANEST_P0_MAIN                                403
#define SUBTASKID_RX_CHANEST_P0_START                               SUBTASKID_RX_CHANEST_P0_EXTRACT
#define SUBTASKID_RX_CHANEST_P0_END                                 SUBTASKID_RX_CHANEST_P0_MAIN


#define SUBTASKID_RX_CHANEST_P1_EXTRACT                             416                                         // 0x0200
#define SUBTASKID_RX_CHANEST_P1_DMA_IN                              417
#define SUBTASKID_RX_CHANEST_P1_DMA_OUT                             418
#define SUBTASKID_RX_CHANEST_P1_START                               SUBTASKID_RX_CHANEST_P1_EXTRACT
#define SUBTASKID_RX_CHANEST_P1_END                                 SUBTASKID_RX_CHANEST_P1_DMA_OUT


#define SUBTASKID_RX_MULTICHAN_DEMODULATION_EXTRACT                 432                                         // 0x0210
#define SUBTASKID_RX_MULTICHAN_DEMODULATION_DMA_IN                  433
#define SUBTASKID_RX_MULTICHAN_DEMODULATION_CHANEST_P3              434
#define SUBTASKID_RX_MULTICHAN_DEMODULATION_CHANEST_P4              435
#define SUBTASKID_RX_MULTICHAN_DEMODULATION_EXP_EQ16                436
#define SUBTASKID_RX_MULTICHAN_DEMODULATION_MRCOM                   437
#define SUBTASKID_RX_MULTICHAN_DEMODULATION_FEQ                     438
#define SUBTASKID_RX_MULTICHAN_DEMODULATION_DMA_OUT                 439
#define SUBTASKID_RX_MULTICHAN_DEMODULATION_START                   SUBTASKID_RX_MULTICHAN_DEMODULATION_EXTRACT
#define SUBTASKID_RX_MULTICHAN_DEMODULATION_END                     SUBTASKID_RX_MULTICHAN_DEMODULATION_DMA_OUT


#define SUBTASKID_RX_DEMAPPER_EXTRACT                               448                                         // 0x0220
#define SUBTASKID_RX_DEMAPPER_DMA_IN                                449
#define SUBTASKID_RX_DEMAPPER_DMA_OUT                               450
#define SUBTASKID_RX_DEMAPPER_MAIN                                  451
#define SUBTASKID_RX_DEMAPPER_MUX_SOFTBITS                          452
#define SUBTASKID_RX_DEMAPPER_START                                 SUBTASKID_RX_DEMAPPER_EXTRACT
#define SUBTASKID_RX_DEMAPPER_END                                   SUBTASKID_RX_DEMAPPER_MUX_SOFTBITS


#define SUBTASKID_RX_FEC_MUX_ACK_CNTL_EXTRACT                       464                                         // 0x0230
#define SUBTASKID_RX_FEC_MUX_ACK_CNTL_DMA_IN                        465
#define SUBTASKID_RX_FEC_MUX_ACK_CNTL_DMA_OUT                       466
#define SUBTASKID_RX_FEC_MUX_ACK_CNTL_MAIN                          467
#define SUBTASKID_RX_FEC_MUX_ACK_CNTL_START                         SUBTASKID_RX_FEC_MUX_ACK_CNTL_EXTRACT
#define SUBTASKID_RX_FEC_MUX_ACK_CNTL_END                           SUBTASKID_RX_FEC_MUX_ACK_CNTL_MAIN


#define SUBTASKID_RX_FEC_MUX_RI_CQI_CNTL_EXTRACT                    480                                         // 0x0240
#define SUBTASKID_RX_FEC_MUX_RI_CQI_CNTL_DMA_IN                     481
#define SUBTASKID_RX_FEC_MUX_RI_CQI_CNTL_CC_CHAN_CODING             482
#define SUBTASKID_RX_FEC_MUX_RI_CQI_CNTL_DMA_OUT                    483
#define SUBTASKID_RX_FEC_MUX_RI_CQI_SFCOMB                          484
#define SUBTASKID_RX_FEC_MUX_RI_CQI_REED_MULLER_DMA_IN              485
#define SUBTASKID_RX_FEC_MUX_RI_CQI_REED_MULLER                     486
#define SUBTASKID_RX_FEC_MUX_RI_CQI_REED_MULLER_FHT_DMA_IN          487
#define SUBTASKID_RX_FEC_MUX_RI_CQI_REED_MULLER_FHT                 488
#define SUBTASKID_RX_FEC_MUX_RI_CQI_REED_RI_DEC                     489
#define SUBTASKID_RX_FEC_MUX_RI_CQI_CNTL_START                      SUBTASKID_RX_FEC_MUX_RI_CQI_CNTL_EXTRACT
#define SUBTASKID_RX_FEC_MUX_RI_CQI_CNTL_END                        SUBTASKID_RX_FEC_MUX_RI_CQI_REED_RI_DEC



// SubTasks for PUCCH
#define SUBTASKID_RX_ULPILOT_PUCCH_EXTRACT                          496                                         // 0x0250
#define SUBTASKID_RX_ULPILOT_PUCCH_DMA_IN                           497
#define SUBTASKID_RX_ULPILOT_PUCCH_DMA_OUT                          498
#define SUBTASKID_RX_ULPILOT_PUCCH_START                            SUBTASKID_RX_ULPILOT_PUCCH_EXTRACT
#define SUBTASKID_RX_ULPILOT_PUCCH_END                              SUBTASKID_RX_ULPILOT_PUCCH_DMA_OUT


#define SUBTASKID_RX_CHANEST_PUCCH_EXTRACT                          512                                         // 0x0260
#define SUBTASKID_RX_CHANEST_PUCCH_DMA_IN                           513
#define SUBTASKID_RX_CHANEST_PUCCH_DMA_OUT                          514
#define SUBTASKID_RX_CHANEST_PUCCH_START                            SUBTASKID_RX_CHANEST_PUCCH_EXTRACT
#define SUBTASKID_RX_CHANEST_PUCCH_END                              SUBTASKID_RX_CHANEST_PUCCH_DMA_OUT


#define SUBTASKID_RX_MULTICHAN_PUCCH_DEMODULATION_EXTRACT           528                                         // 0x0270
#define SUBTASKID_RX_MULTICHAN_PUCCH_DEMODULATION_DMA_IN            529
#define SUBTASKID_RX_MULTICHAN_PUCCH_DEMODULATION_DMA_OUT           530
#define SUBTASKID_RX_MULTICHAN_PUCCH_DEMODULATION_START             SUBTASKID_RX_MULTICHAN_PUCCH_DEMODULATION_EXTRACT
#define SUBTASKID_RX_MULTICHAN_PUCCH_DEMODULATION_END               SUBTASKID_RX_MULTICHAN_PUCCH_DEMODULATION_DMA_OUT



// SubTasks for SRS
#define SUBTASKID_RX_SRS_PILOTS_EXTRACT                             544                                         // 0x0280
#define SUBTASKID_RX_SRS_PILOTS_DMA_IN                              545
#define SUBTASKID_RX_SRS_PILOTS_DMA_OUT                             546
#define SUBTASKID_RX_SRS_PILOTS_START                               SUBTASKID_RX_SRS_PILOTS_EXTRACT
#define SUBTASKID_RX_SRS_PILOTS_END                                 SUBTASKID_RX_SRS_PILOTS_DMA_OUT


#define SUBTASKID_RX_SRS_EXTRACT                                    560                                         // 0x0290
#define SUBTASKID_RX_SRS_DMA_IN                                     561
#define SUBTASKID_RX_SRS_DMA_OUT                                    562
#define SUBTASKID_RX_SRS_START                                      SUBTASKID_RX_SRS_EXTRACT
#define SUBTASKID_RX_SRS_END                                        SUBTASKID_RX_SRS_DMA_OUT


#define SUBTASKID_RX_SRS_SNR_EXTRACT                                576                                         // 0x0300
#define SUBTASKID_RX_SRS_SNR_DMA_IN                                 577
#define SUBTASKID_RX_SRS_SNR_DMA_OUT                                578
#define SUBTASKID_RX_SRS_SNR_START                                  SUBTASKID_RX_SRS_SNR_EXTRACT
#define SUBTASKID_RX_SRS_SNR_END                                    SUBTASKID_RX_SRS_SNR_DMA_OUT



// SubTasks for PRACH
#define SUBTASKID_RX_PRACH_EXTRACT                                  592                                         // 0x0310
#define SUBTASKID_RX_PRACH_DMA_IN                                   593
#define SUBTASKID_RX_PRACH_DMA_OUT                                  594
#define SUBTASKID_RX_PRACH_START                                    SUBTASKID_RX_PRACH_EXTRACT
#define SUBTASKID_RX_PRACH_END                                      SUBTASKID_RX_PRACH_DMA_OUT


////////////////////////////////////////////
// 1. LTE BS Tx (DL) Ceva Tasks time limit
////////////////////////////////////////////
#define TIME_TX_DLCONTROL                      500
#define TIME_TX_SETUP_SYMB_BUFS                300
#define TIME_TX_MULTICHAN_MODULATION           450

////////////////////////////////////////////
// 2. LTE BS Rx (UL) Ceva Tasks time limit
////////////////////////////////////////////
#define TIME_RX_RUNNING_AVG                    150
#define TIME_RX_SNRSUBFRAME                    100
#define TIME_RX_ULPILOT                        250
#define TIME_RX_CHANEST_P0                     100
#define TIME_RX_CHANEST_P1                     200
#define TIME_RX_MULTICHAN_DEMODULATION         350
#define TIME_RX_DEMAPPER                       200
#define TIME_RX_FEC_MUX_ACK_CNTL               80
#define TIME_RX_FEC_MUX_RI_CQI_CNTL            1000
#define TIME_RX_ULPILOT_PUCCH                  300
#define TIME_RX_CHANEST_PUCCH_P0               300
#define TIME_RX_RUNNING_AVG_PUCCH              300
#define TIME_RX_CHANEST_PUCCH                  500
#define TIME_RX_MULTICHAN_PUCCH_DEMODULATION   200
#define TIME_RX_SRS_PILOTS                     300
#define TIME_RX_SRS                            800
#define TIME_RX_SRS_SNR                        200
#define TIME_RX_PRACH                          2500

#endif

#ifdef __cplusplus
}
#endif

