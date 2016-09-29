//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL apiplayer.h
 *
 * @brief The declaration of api player API
 * @author Mindspeed Technologies
 * @version $Revision: 1.23 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifndef _API_PLAYER_H_
#define _API_PLAYER_H_

#include "supervisor.h"
#include "LtePhyL2Api.h"

#define APIPLAY_PAR_BUF_START_PARAM_ID	(2)
#define APIPLAY_PAR_BUF_SIZE_PARAM_ID	(3)
#define APIPLAY_PAR_BUF_OFFS_PARAM_ID	(4)
#define APIPLAY_PAR_DATA_PARAM_ID		(5)
#define APIPLAY_PAR_START_SFN			(6)
#define APIPLAY_PAR_SFN_TO_PLAY			(7)
#define APIPLAY_PAR_PHY_INTERFACE		(8)
#define APIPLAY_PAR_SDU_LOG_CTRL		(9)
#define APIPLAY_PAR_SDU_LOG_PTR			(10)
#define APIPLAY_PAR_SDU_LOG_SIZE      		(11)
#define APIPLAY_PAR_PHY_TTI             		(12)
#define APIPLAY_PAR_PHY_DI              		(13)
#define APIPLAY_PAR_LAST_SFN     			(14)
#define APIPLAY_PAR_FINISH_SFN			(15)
#define APIPLAY_PAR_FINISH_PART			(16)
#define APIPLAY_PAR_FIRST_SFN           (17)

#define APIPLAY_INIT_MODE	(1)

#define RC_APIPLAY_OK                  MX_OK
#define RC_APIPLAY_PARAM_ERROR         DEF_USRRC(APP_MODULE, APIPLAYER, 1)
#define RC_APIPLAY_ALLOC_ERROR         DEF_USRRC(APP_MODULE, APIPLAYER, 2)
#define RC_APIPLAY_BUFFER_OUT     DEF_USRRC(APP_MODULE, APIPLAYER, 3)
#define RC_APIPLAY_LOGVER_ERROR         DEF_USRRC(APP_MODULE, APIPLAYER, 4)

#define APIPLAYER_DEF_LOG_SDU_CTRL 1 // 1 - enabled SDU logging by default, 0 - disabled by default

//typedef UINT8 apiplayer_mode_t;

typedef enum _apiplayer_mode_t
{
	APIPLAY_MODE_DEF = 0
	,APIPLAY_MODE_OWN_BUFF	
}apiplayer_mode_t;

#define API_PLAYER_OWN_BUFF_SIZE	(SVSR_API_LOGGER_BUF_SIZE) 

//#define APIPLAY_MODE_DEF		0
//#define APIPLAY_MODE_OWN_BUFF	1

/** @brief This type describes API player configuration parameters 
            used in a process of playing 

    \ingroup group_lte_services */

typedef struct _ApiPlayerParam_
{
    BOOL   InitAndStartDone;        // Init and Start messages are sent
    UINT32 StartSFN;                // First SFN to play
    UINT32 FinishSFN;               // Last SFN to play
    UINT32 FirstSFN;                // First SFN from buffer
    UINT32 LastSFN;                 // Last SFN from buffer
    UINT32 CurrentSFN;              // Current SFN to process

    UINT32 NextMsgPos;              // Next message position at the  ApiLog buffer
    UINT32 nMask;                   // Log mask would be saved here
    
    UINT32 MacInstID;
    UINT32 nPhyTTI;                 // The TTI (used for timer mode)
    UINT32 nPhyDI;                  // The PHY data (antenna) interface (if it's needed to be changed)

    PMAC2PHY_QUEUE_EL MsgHead;      // The first message in a list of messages (new PHY interface)
    PMAC2PHY_QUEUE_EL MsgTail;      // The last message in a list of messages (new PHY interface)
    MsgHandlerCtx hdlr_ctx;
    MsgHandlerCtx hdlr_ctx_ex;
	
    API_LOG_INFO * pHdr;
} ApiPlayerParam;

/** @brief This type describes API player context 

    \ingroup group_lte_services*/
            
typedef struct _APIPLAYERCTX_
{
    UINT32 bCtxInit;
    UINT32 bSendStopInd;
    HANDLE hThr;
	HANDLE hWdg;
    ApiPlayerParam param;
    UINT32 PlayStarted;
    VUINT32 TTINum;
    VUINT32 StopInd;

    UINT32 bLogSdu;

    PUINT8 LogPtr;
    UINT32 LogSize;
    UINT32 LogPos;
    UINT32 LogStatus;
    UINT32 LogSduNum;
    UINT32 LogSduStatusNum;
	UINT32 lasttick;
	UINT32 delay_ticks;

} APIPLAYERCTX;

extern APIPLAYERCTX player_ctx;

/**brief This function returns the pointer to api player context

    @return [APIPLAYERCTX *]

    \ingroup group_lte_services */

INLINE APIPLAYERCTX * ApiPlayerGetCtx(void)
{
    return &player_ctx;
}

/**brief This function returns the pointer to api player configuration parameters

    @return [ApiPlayerParam *]

    \ingroup group_lte_services */
    
INLINE ApiPlayerParam * ApiPlayerGetParam(void)
{
    return &player_ctx.param;
}

/** @brief Thi function initializes the API player and has to be calle before usage of any API player API
    @return [MXRC] an error code of operation , MX_OK - OK 

    \ingroup group_lte_services */
    
MXRC ApiPlayerInit(void);

/** @brief This function starts playing the preloaded API trace to the PHY code 

    @param DataPtr   [in] the pointer to the API trace buffer
    @param StartSFN  [in] the start index , 0 - based, the first SFN to play
    @param FinishSFN [in] the last SFN to play
    @param tti       [in] the TTI value, if 0 - TTI provided in START message will be used
    @param di        [in] the Data interface ID (if 0xFF the specified in START command will be used)
    @param bLogRxSdu [in] if 1 - the API player will log received SDUs

    @return [MXRC] an error code of operation 

    \ingroup group_lte_services*/

MXRC ApiPlayerStart(PUINT8 DataPtr, UINT32 StartSFN, UINT32 FinishSFN, UINT32 tti, UINT32 di, UINT32 bLogRxSdu);
#endif

