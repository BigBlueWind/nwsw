//-------------------------------------------------------------------------------------------
/** @file fptypes.h
 *
 * @brief MAP (Mindspeed Advanced Processor) types/macro definition
 * @author Mindspeed Technologies
 * @version $Revision: 1.1 $
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

#ifndef _FP_TYPES_H_
#define _FP_TYPES_H_

#define FP_GRP0                             0
#define FP_GRP1                             1

#define MAX_IDFT_TRANSACTIONS_PER_SYMBOL    4
#define MAX_IDFT_SIZE_SMALLER_TRANSACTION   216
#define NUM_IDFT_INPUT_DMA_DESC             1    // 1 Input
#define NUM_IDFT_OUTPUT_DMA_DESC            (MAX_IDFT_TRANSACTIONS_PER_SYMBOL)      // 4 Output (3 Dummy and 1 Real)
#define NUM_IDFT_CONFIG_TABLE_DMA_DESC      (MAX_IDFT_TRANSACTIONS_PER_SYMBOL * 2)  // (1 Small Config, 1 Mult Tables) x 4
#define NUM_IDFT_OUTPUT_DESC_START          (NUM_IDFT_CONFIG_TABLE_DMA_DESC + 1)    // 1 Input + all config /mult tables
#define MAX_NUM_IDFT_DMA_DESC               (NUM_IDFT_INPUT_DMA_DESC + NUM_IDFT_OUTPUT_DMA_DESC + NUM_IDFT_CONFIG_TABLE_DMA_DESC) // 13 descriptors in total

typedef struct tMAP_DMA_EXP_REGS
{
    VUINT32  Enable;
    VUINT32  Priority;
    VUINT32  Status;
    VUINT32  RRobinCtrl;
    VUINT32  FHeadIn[5];
    VUINT32  FHeadSharedIn;
    VUINT32  Reserved[2];
    VUINT32  FHeadOut[5];
    VUINT32  FHeadSharedOut;
} MAP_DMA_EXP_REGS, *PMAP_DMA_EXP_REGS;

#endif // _FP_TYPES_H_
#ifdef __cplusplus
}
#endif

