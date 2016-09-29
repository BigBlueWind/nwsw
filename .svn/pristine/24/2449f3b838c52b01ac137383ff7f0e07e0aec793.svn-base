//-------------------------------------------------------------------------------------------
/** @file dspprintf.h
 *
 * @brief The file contains macros, structure which need to CEVA debug ability
 * @author Mindspeed Technologies
 * @version $Revision: 1.5 $
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

#ifndef _DSP_PRINTF_DRV_H_
#define _DSP_PRINTF_DRV_H_

/// The number of UART port which receives data from "printf" function of CEVA.
#define DSP_UART_PORT        1

/// The length of the cyclic buffer
#define DSP_PRINTF_BUF_LEN  1024

#define SOFT_ICPU_DSP_PRINTF    (-1)

/// The pointer to cyclic buffer
#define CRAM_ARM_PRINTBUF_ADDR *((unsigned int *) 0xFC000030)

/// The descriptor of cyclic buffer
typedef struct tDSPPRINTFDESC {
    unsigned char *pStart;      ///< the pointer to start of the cyclic buffer
    UINT32 size;                ///< the length of hte cyclic buffer
    volatile UINT32 put;        ///< the counter of putting data
    volatile UINT32 get;        ///< the counter of getting data
    HANDLE h;                   ///< the handler of the cyclic buffer

} DSPPRINTFDESC, *PDSPPRINTFDESC;

/** @brief This function initializes cyclic buffer descriptor and sets interrupt handler of CEVAs.

  * @return [MXRC] an error code
  \ingroup group_lte_services
  */
MXRC DspPrintfInit(void);

/** @brief This handler is called to read data from the cyclic buffer and transfer to the UART port.
  * @param pAckReg [out] - the pointer to the IRQ ack register
  * @param nIrqID       [in]  - the interrupt ID
  * @param pParam       [in]  - the pointer to the driver context
 \ingroup group_lte_services
 */
void DspPrintfIrqHandler(volatile UINT32 * pAckReg, UINT32 nIrqID, LPVOID pParam);

/** @brief This function gets data of the cyclic buffer and sends to UART port.

 \ingroup group_lte_services
 */
void DspPrintfThread (void);

#endif /*_DSP_PRINTF_DRV_H_*/

#ifdef __cplusplus
}
#endif

