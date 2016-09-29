//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL Globalstoragearrays.h
 *
 * @brief ARM L2 PL310 control (based on ARM code)
 * @author Mindspeed Technologies
 * @version $Revision: 1.20 $
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

#include "logmgr.h"


#define GS_RC_UNKNOWN_API           DEF_USRRC(APP_MODULE, GLOG_STOR, 1)
#define GS_RC_PARAM_ERROR           DEF_USRRC(APP_MODULE, GLOG_STOR, 2)
#define GS_RC_ALLOC_ERROR           DEF_USRRC(APP_MODULE, GLOG_STOR, 3)

#define DL_IQ_SAMPLES_BUF_SIZE  (15360 *2 * 1000)   /*Samples for 10Mhz * IQ * ms   */
#define UL_IQ_SAMPLES_BUF_SIZE  (15360 *2 * 1000)   /*Samples for 10Mhz * IQ * ms   */
#define DL_SDU_BUF_SIZE         (6000 * 1000)       /*MAX SDU Size for 10Mhz * ms   */
#define UL_SDU_BUF_SIZE         (6000 * 1000)       /*MAX SDU Size for 10Mhz * ms   */
#define VIDEO_FILE_BUF_SIZE     (10*1024*1024)      /*MBytes                        */
#define PACK_HEAP_INFO_BUF      (1 * 1024 * 1024)

#define GS_PARAM_DL_INFO        1
#define GS_PARAM_UL_INFO        2
#define GS_PARAM_DL_SDU_INFO    3
#define GS_PARAM_UL_SDU_INFO    4
#define GS_PARAM_MLOG_INFO      5
#define GS_PARAM_API_TRACE_INFO 6

#define GS_INFO_OBJ_BASE        2
#define GS_INFO_OBJ_SIZE        3


typedef struct t_Global_Storage_Array_Ctx
{
    PUINT8 pDlUlIqBuf;
    PUINT8 pDlIqCramBuf[MAX_NUM_ANT_SUPPORTED][2];      // For 2 Contexts
    PUINT8 pUlIqCramBuf[MAX_NUM_ANT_SUPPORTED][2];      // For 2 Contexts

    UINT32 nTxAnt, nRxAnt;
    UINT32 bufSizePerAnt;

} GLOBALSTORAGEARRAYSCTX, *PGLOBALSTORAGEARRAYSCTX;

#ifndef GLOBAL_STORAGE_ARRAY_CFILE

extern UINT8 gStorageArrayNotInit;
extern PSINT16 gDlIqBuf;
extern PSINT16 gUlIqBuf;
extern PSINT8 gDlSduBuf;
extern PSINT8 gUlSduBuf;
extern LOGHANDLE hLogDlIQ, hLogUlIQ;

extern PUINT8 gSvsrLoggerBuf;
extern PUINT8 gMlogBuf;
extern UINT8 * MLogDevPtr;
extern PUINT8 videoFileData;
extern PUINT8 PackHeapPtr; // The pointer to the buffer used to save current heap state and to analyze it on the host side

extern unsigned int iq_samples_size_dl;
extern unsigned int iq_samples_size_ul;

#else

UINT8 gStorageArrayNotInit = 1;
PSINT16 gDlIqBuf;
PSINT16 gUlIqBuf;
PSINT8 gDlSduBuf;
PSINT8 gUlSduBuf;
LOGHANDLE hLogDlIQ, hLogUlIQ;

PUINT8  gSvsrLoggerBuf;
PUINT8  gMlogBuf;
UINT8 * MLogDevPtr;

unsigned int iq_samples_size_dl = DL_IQ_SAMPLES_BUF_SIZE;
unsigned int iq_samples_size_ul = UL_IQ_SAMPLES_BUF_SIZE;

PUINT8 PackHeapPtr = NULL;  // The pointer to the buffer used to save current heap state and to analyze it on the host side

#endif

MXRC InitializeStorageArrays(void);
void DestroyStorageArrays(void);
UINT32 GetStorageCapacity(UINT32 fftSize, UINT32 numAntennas);

MXRC InitializeCramIqBufs(UINT32 nTxAnt, UINT32 nRxAnt, UINT32 bufSizePerAnt);
MXRC DestroyCramIqBufs(void);
PUINT8 QueryCramIqBuf(UINT32 direction, UINT32 context, UINT32 antId);


#ifdef __cplusplus
}
#endif

