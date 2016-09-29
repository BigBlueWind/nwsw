#ifndef _CommMaco_CellSearch_h
#define _CommMaco_CellSearch_h
#ifndef VS_ENV
#include "typedef.h"
#include "tcb.h"
#endif

#define BCH_COHERENT_SHIFT_BIT	(8)
//#define BCH_CHEST_SHIFT_BIT		1
#define BCH_CHEST_SHIFT_BIT		(4) //jerry
#define BCH_CHANCOMP_SHIFT_BIT  (14)
#define RAKE_FINGER_SHIFT_BIT	(1)
#define DECODE_SHIFT_BIT		(8)
#define DESCARM_SHIFT_BIT       (1)

#define MAX_16_BIT_SIGN			(32767) 
#define MIN_16_BIT_SIGN			(-32768)
#define MAX_16_BIT_UNSIGN		(65536)
#define MAX_32_BIT_SIGN			(2147483647)
#define MIN_32_BIT_SIGN			(-2147483648)
#define MAX_8_BIT_SIGN			(127)
#define MIN_8_BIT_SIGN			(-128)





#define FRAME_CHIPS_NUM   			(38400)
#define TS_CHIPS_NUM    			(2560)
#define INERT_DATA_LEN                (256+64+6)  /* TS data len 2560 add search window len 
64, 6: due to interpolation filter which filled with 0*/
#define FRAME_SLOT_NUM     			(15)
#define SLOT_SPREAD_BIT_NUM 		(9)
#define SPREAD_CODE_LEN    			(256)
#define SCH_CHIPS_SLOT_NUM  		(256)

#define BCH_TB_CRC_LEN                     (262)
#define BCH_TRBK_LEN 				(246)
#define BCH_CRC_LEN                         (16)
#define BCCH_TTI10MS_LEN                 (270)
#define MAXIMUM_FINGER_NUM           (4)


/* interpolation */
#define INTERPOLATION_2X_FILLED_NUM   (6)
#define INTERPOLATION_8X_FILLED_NUM   (13)

/* ======== every process DAtA changed to 256 chips*/
#define PRO_CHIPS_NUM_EACH            (256)
#define PATH_SEARCh_OFFSET_CHIPS   (64)
#define DMA_TRANS_CHIPS_NUM           (320) /* 256 +64*/

#define FRE_RESIDUAL_ERR_1FRAME_SYM   (150) 
#define FRE_RESIDUAL_ERR_4FRAME_SYM   (600) // symbols num over 4 frames
#define FRE_RESIDUAL_ERR_COH_SYMB_NUM  (4)
#define FRE_RESIDUAL_ERR_SYMB_NUM  (148)
#define FRE_RESIDUAL_ANGLE_NUM        (148) //148/4*4
#define FRE_RESIDUAL_1FRAME_CALC_NUM     (36)  /*148/4 -1*/
#define FRE_RESIDUAL_SLOT_NUM           (60)
#define FRE_RESIDUAL_FRAME_NUM          (4)

/* FOC */
#define FOC_CHIPS_LEN                     (256) /* Set temp for coding,need be decide  */
#define FOC_CHIPS_SEG_NUM            (256/FOC_CHIPS_LEN) /*256/FOC_CHIPS_LEN*/
#define FOC_CHIPS_RESULT_NUM       (1024/FOC_CHIPS_LEN) /*1024/FOC_CHIPS_LEN*/
#define PI             (3.1416)
#define FRE_STEP  (100)  //Q12 of 0.025  100 = 0.025*4096

#define FRE_COM_ANGLE_OFFSET   (15)

#define PI2         (25735)  // Q12 of 3.14125 * 2  
#define CHANNEL_COM_SLOT_NUM   (3)

#define NO_CRC_CHECKED                      (0)
#define FIRST_CRC_CHECK_FAILED          (1)
#define FIRST_CRC_CHECK_HAS_DONE    (2)

/* calc frequency err value */
#define FIRST_SYMBOL_OF_TS    (0)
#define LAST_SYMBOL_OF_TS     (1)
#define OTHER_SYMBOL_OF_TS  (2)


/*PATH SEARCH*/
#define  PS_DATA_LEN                        (TS_CHIPS_NUM*8) //8: 2*2*2 
#define PATH_SEARCH_SLOT_NUM          (30) // 2frames
#define PATH_SEARCH_WIN_LEN             (64)
#define PATH_SEARCH_DEMOD_LEN         ( 5248) //(5120+PATH_SEARCH_WIN_LEN*2)  //2X
#define FINGER_STATE_ACTIVE                (1)
#define FINGER_STATE_NACTIVE              (0)
#define FINGER_STATE_HOLD                  (2)
#define PM_ACC_SLOT_NUM                  (15)

/* error code */
#define ERR_CODE_SUCCESS     (0)
#define ERR_CODE_FILE_OPEN_FAIL      (30000)
#define ERR_CODE_MSG_ADDR_ERR      (30001)
#define ERR_CODE_TASK_ERR                (30002)

#define ERR_CODE_DECODE_SUCC           (30100)
#define ERR_CODE_DECODE_ERR           (30101)
#define ERR_CODE_NO_DECODE_PRO    (30102)

/* viterbi decode */
#define TRACE_BACK_BUFFER_SIZE      ((256+1)*270*sizeof(U8))

/*TCB LIST CONTROL*/
#define  UMTSNMM_USE_TCB_LIST 1


#ifdef __CC_ARM
#define Umtsnmm_dbg_printf(x) phy_printf x
#define Umtsnmm_dbg_printf_off(x)
#else
#define Umtsnmm_dbg_printf(x) printf x
#define Umtsnmm_dbg_printf_off(x)
#endif

#ifdef VS_ENV

#ifndef S8
typedef signed char              S8;
#endif

#ifndef S16
typedef signed short int         S16; 
#endif

#ifndef S32
typedef signed long int          S32;
#endif

#ifndef U8
typedef unsigned char             U8;
#endif

#ifndef U16
typedef unsigned short int        U16;
#endif

#ifndef U32
typedef unsigned long int         U32;
#endif

#endif


#endif
