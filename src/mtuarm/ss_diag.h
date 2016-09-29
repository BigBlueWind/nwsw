/********************************************************************16**

                         (c) COPYRIGHT 2006 by 
                         Continuous Computing Corporation.
                         All rights reserved.

     This software is confidential and proprietary to Continuous Computing 
     Corporation (CCPU).  No part of this software may be reproduced,
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written Software License 
     Agreement between CCPU and its licensee.

     CCPU warrants that for a period, as provided by the written
     Software License Agreement between CCPU and its licensee, this
     software will perform substantially to CCPU specifications as
     published at the time of shipment, exclusive of any updates or 
     upgrades, and the media used for delivery of this software will be 
     free from defects in materials and workmanship.  CCPU also warrants 
     that has the corporate authority to enter into and perform under the   
     Software License Agreement and it is the copyright owner of the software 
     as originally delivered to its licensee.

     CCPU MAKES NO OTHER WARRANTIES, EXPRESS OR IMPLIED, INCLUDING
     WITHOUT LIMITATION WARRANTIES OF MERCHANTABILITY OR FITNESS FOR
     A PARTICULAR PURPOSE WITH REGARD TO THIS SOFTWARE, SERVICE OR ANY RELATED
     MATERIALS.

     IN NO EVENT SHALL CCPU BE LIABLE FOR ANY INDIRECT, SPECIAL,
     CONSEQUENTIAL DAMAGES, OR PUNITIVE DAMAGES IN CONNECTION WITH OR ARISING
     OUT OF THE USE OF, OR INABILITY TO USE, THIS SOFTWARE, WHETHER BASED
     ON BREACH OF CONTRACT, TORT (INCLUDING NEGLIGENCE), PRODUCT
     LIABILITY, OR OTHERWISE, AND WHETHER OR NOT IT HAS BEEN ADVISED
     OF THE POSSIBILITY OF SUCH DAMAGE.

                       Restricted Rights Legend

     This software and all related materials licensed hereby are
     classified as "restricted computer software" as defined in clause
     52.227-19 of the Federal Acquisition Regulation ("FAR") and were
     developed entirely at private expense for nongovernmental purposes,
     are commercial in nature and have been regularly used for
     nongovernmental purposes, and, to the extent not published and
     copyrighted, are trade secrets and confidential and are provided
     with all rights reserved under the copyright laws of the United
     States.  The government's rights to the software and related
     materials are limited and restricted as provided in clause
     52.227-19 of the FAR.

                    IMPORTANT LIMITATION(S) ON USE

     The use of this software is limited to the use set
     forth in the written Software License Agreement between CCPU and
     its Licensee. Among other things, the use of this software
     may be limited to a particular type of Designated Equipment, as 
     defined in such Software License Agreement.
     Before any installation, use or transfer of this software, please
     consult the written Software License Agreement or contact CCPU at
     the location set forth below in order to confirm that you are
     engaging in a permissible use of the software.

                    Continuous Computing Corporation
                    9380, Carroll Park Drive
                    San Diego, CA-92121, USA

                    Tel: +1 (858) 882 8800
                    Fax: +1 (858) 777 3388

                    Email: support@trillium.com
                    Web: http://www.ccpu.com

*********************************************************************17*/


/********************************************************************20**
 
     Name:     System Services -- Logging functions
 
     Type:     C Header file
 
     Desc:     Header file for System Services Logging.
 
     File:     
 
     Sid:      
 
     Prg:      pakumar
  
*********************************************************************21*/ 
#ifndef __SS_DIAG_H__
#define __SS_DIAG_H__
#include "pthread.h"
 
/* 
 * @details Macro definition  for ss_log  
 */
 
typedef enum
{
   SS_DIAG_NORMAL,
   SS_DIAG_RPTPEND,
   SS_DIAG_REPORTED
} SsDiagLostState;

typedef enum
{
   SS_DIAG_BUF1,                   /*!< buffer used for EGTP, eNBAPP, S1AP, X2Ap     */
   SS_DIAG_BUF2,                   /*!< buffer used for SCTP                         */
   SS_DIAG_BUF3,                   /*!< buffer used for SM                           */
   SS_DIAG_BUF4,                   /*!< buffer used for TUCL                         */
   SS_DIAG_BUF5,                   /*!< buffer used for lower arm core1              */
   SS_DIAG_BUF6                    /*!< buffer used for lower arm core2              */
} SsDiagCircBuf;

typedef enum
{
   SS_DIAG_MSG_TYPE_FIXED = 1,       /*!< message type fixed log          */
   SS_DIAG_MSG_TYPE_HEXDUMP,       /*!< message type Message Hex Dump   */
   SS_DIAG_MSG_TYPE_VAR,           /*!< message type Varible length     */
   SS_DIAG_MSG_TYPE_MARKER,         /*!< message type Sort marker to indicate start sort in post processing  */
   SS_DIAG_MSG_TYPE_NOLOGLOST        /**/
}SsDiagMsgType; 


#define SS_DIAG_INST0            0
#define SS_DIAG_INST1            1

#define SS_DIAG_LVL0             0x1
#define SS_DIAG_LVL1             0x2
#define SS_DIAG_LVL2             0x4
#define SS_DIAG_LVL3             0x8
#define SS_DIAG_LVL4             0x10
#define SS_DIAG_MSG              0x20  /*!< Macro used for Message Hex Dump  level     */
#define SS_DIAG_VAR              0x40   /*!< Macro used for Varible length level  */

#define SS_DIAG_BUF_SIZE_SYSTSK1     4095
#define SS_DIAG_BUF_SIZE_SYSTSK2     4095
#define SS_DIAG_BUF_SIZE_SYSTSK3     4095
#define SS_DIAG_BUF_SIZE_SYSTSK4     4095
#define SS_DIAG_BUF_SIZE_SYSTSK5     4095
#define SS_DIAG_BUF_SIZE_SYSTSK6     4095

#define SS_DIAG_SPL_TOKN         0xfffffff1
#define SS_DIAG_INV_ARG          0xffffffff
#define SS_DIAG_SORT_MARKER      0xfffffffe
#define SS_DIAG_MAX_CIRC_BUF     6
#define SS_DIAG_NUM_CIRBUF_UARM  4
#define SS_DIAG_MIN_BUF_DIFF     4
#define SS_DIAG_SLEEP            10
#define SS_DIAG_MAX_NUM_VAR_BUF  10
#define SS_DIAG_VAR_BUF_LEN      255
/* 
 * @details Macro definition for unpack a U16 into a static array  
 */
#define SS_UNPKU16(_tmpU16) \
{                           \
   _tmpU16 = 0;             \
   _tmpU16 = (U16)PutHiByte(_tmpU16, (U8)pkArray[bytCnt++]); \
   _tmpU16 = (U16)PutLoByte(_tmpU16, (U8)pkArray[bytCnt++]); \
}


/* 
 * @details Get Time stamp for Lower ARM and Upper ARM
 */
#define SS_DIAG_GET_TIME_STAMP(_timeRef)\
{\
   _timeRef=numTti;\
}

#define SS_DIAG_FILE_OPEN(_string, _mode)\
   fopen(_string, _mode);

#define SS_DIAG_FILE_WRITE(src, size, count, dst)\
   fwrite(src, size, count, dst);
 
 
/* 
 * @brief Logging buffer structure to store the runtime values. At present,
 * fixed length logs supports only 4 run time arguments and one standalone
 * argument
 */
typedef struct _ssDiagBuf
{
   U32       timRef;        /*!< Time tick reference                          */
   U32       tknId;         /*!< Token identifier of the log                  */
   U32       entInstId;     /*!< Layer Entity and instance Id                 */
   U32       level;         /*!< Level of log                                 */
   U32       type;          /*!< Type of log, Message or Event log            */
   U32       splArg;        /*!< Additional standalone argument for sorting   */
   U32       arg1;          /*!< User Argument 1                              */
   U32       arg2;          /*!< User Argument 2                              */
   U32       arg3;          /*!< User Argument 3                              */
   U32       arg4;          /*!< User Argument 4                              */
   U32       isValid;       /*!< variable to check buffer is completely 
                                written or not                                */
}SsDiagBuf;

/** 
 * @brief Structure for circular log buffer. This buffer is used for storing
 * run time values of the logging macro
 */
typedef struct _ssDiagCirBuf
{
   U32              front;       /*!< Start pointer of the circular buffer */
   U32              rear;        /*!< End pointer of the circular buffer   */
   U32              noLogLost;   /*!< Number of lost log                   */
   U32              logLostState;/*!< State of lost log                    */
   U32              nLen;        /*!< length of circular buffer            */
   SsDiagBuf        *logBuf;     /*!< Log buffer used to store values      */
}SsDiagCirBuf;

/** 
 * @brief Structure for circular Variable buffer. This buffer is used for storing
 * run time values of the Variable string logging macro
 */       
typedef struct _ssDiagVarBuf
{
   U8         varStr[SS_DIAG_VAR_BUF_LEN];  /*!< variable to store the Variable length log       */
   Bool       isValid;      /*!< variable to check buffer is free or not         */
}SsDiagVarBuf;



/*
 * @brief Function forward declarations */
EXTERN SsDiagCirBuf ssDiagCirLogBuf[SS_DIAG_MAX_CIRC_BUF];
EXTERN SsDiagCirBuf ssDiagCirVarBuf[SS_DIAG_NUM_CIRBUF_UARM];

EXTERN Void ssDiagFix ARGS(( U32 tknId, U32 splArgEnum, U32 entId, U32 instId, U32 logLvl, U32 logType, 
         U32 splArg, U32 arg1, U32 arg2, U32 arg3, U32 arg4, S8 *string));

EXTERN S16 ssDiagInitFixCircBuf(U32 ssSysTstId, U16 maxBufLen);
EXTERN S16 ssDiagInitVarCircBuf(U32 ssSysTstId, U16 maxBufLen);

EXTERN Void ssDiagVarible ARGS((U32 entId, U32 instId, U32 logLvl, U32 logType,
         U32 tknId, U32 splArg, S8 string[]));
EXTERN S16 ssDiagUnPackLogs ARGS((Buffer *mBuf));
#endif
