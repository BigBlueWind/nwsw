/********************************************************************16**

                         (c) COPYRIGHT 2009 by 
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
                    9450, Carroll Park Drive
                    San Diego, CA-92121, USA

                    Tel: +1 (858) 882 8800
                    Fax: +1 (858) 777 3388

                    Email: support@trillium.com
                    Web: http://www.ccpu.com

*********************************************************************17*/

/********************************************************************20**
  
     Name:     e-GTP layer
  
     Type:     C include file
  
     Desc:     Interface for tunnel management module.
  
     File:     eg_tunn.x
  
     Sid:      eg_tunn.x@@/main/3 - Fri Feb  4 18:18:13 2011
     
     Prg:      mm
  
*********************************************************************21*/

#ifndef __EG_TUNN_X__
#define __EG_TUNN_X__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**@file eg_tunn.x
    @brief Tunnel Management Module for EGTP
*/

/******************************************************************************
 *  Tunnel Manamgement local structures                                       *
 ******************************************************************************/
/**
  @ingroup TUNNMNGMT
    @struct _egTunnEvent
    @brief Enumerates Tunnel events
 */
typedef enum _egTunnEvent
{
   EG_TUN_EVENT_INCSESSREQ,  /**< A Create Session request is received */
   EG_TUN_EVENT_OUTSESSREQ,  /**< A Create Session Request is being sent */
   EG_TUN_EVENT_INCSESSRSP,  /**< A Create Session Response is received */
   EG_TUN_EVENT_OUTSESSRSP,  /**< A Create Session Response is being sent */
   EG_TUN_EVENT_ERR_IND,     /**< An Error Inidication is received from 
                                other module */
   EG_TUN_EVENT_INC_MSG,     /**< A GTP-C message is received on the tunnel */
   EG_TUN_EVENT_OUT_MSG,     /**< A GTP-C message is being sent on the tunnel */
   EG_TUN_EVENT_MAX
}EgTunnEvent;


#ifdef EGTP_C
/******************************************************************************
 *                       Prototypes of private functions                      *
 ******************************************************************************/

/************************************************
 *               TEID generators                *
*************************************************/
/**
  @ingroup TUNNMNGMT
 @brief The function egInitFreeTeidList initializes an array of teids. The array
        of free teids is initialized.
 @param[in] tunnIdx The tunnel Cb whose teid array has to be initialize
 @return 
   - ROK      - If the procedure was successful.
   - ROUTRES  - If the procedure failed due to resource failure.
   - RFAILED  - If the procedure failed due to internal reasons.
 */
/*eg004.201 Modified for eGTP-C PSF Upgarde*/
EXTERN S16 egInitFreeTeidList ARGS ((
                                U32 tunnIdx
                                ));

/**
  @ingroup TUNNMNGMT
 @brief The function egInitTeidArr intializes the global array tunnel Cbs. It
        also initializes the array of teid Cbs in the first tunnel Cb.
 @return -
        - ROK -     If the procedure was successful.
        - ROUTRES - If the procedure failed due to memory failure.
        - RFAILED - If the procedure failed due to internal reasons.
 */
PRIVATE S16 egInitTeidArr ARGS ((Void));

/**
  @ingroup TUNNMNGMT
 @brief The function egDeInitTeidArr un-intializes the global array tunnel Cbs.
        It also un-initializes the array of teid Cbs in the array of tunnel Cb.
 @return -
        - ROK -     If the procedure was successful.
        - ROUTRES - If the procedure failed due to memory failure.
        - RFAILED - If the procedure failed due to internal reasons.
 */
PRIVATE S16 egDeInitTeidArr ARGS ((Void));

/**
  @ingroup TUNNMNGMT
 @brief The function egObtainFreeTeid returns a free TEID.
 @param[out] teid The teid holds the TEID that this function returns.
 @return -
        - ROK -     If the procedure was successful.
        - ROUTRES - If the procedure failed due to memory failure.
        - RFAILED - If the procedure failed due to internal reasons.
 */
PRIVATE S16 egObtainFreeTeid ARGS ((
                             U32 *teid
                             ));

/*eg004.201 Modified for eGTP-C PSF Upgarde*/
/****************************************************************
 *               Echo mechanism handlers                        *
*****************************************************************/

/**
  @ingroup TUNNMNGMT
 @brief The function egHandleOutEchoRsp handles the outgoing Echo Response.
 @param[in] usap The usap Cb to which the message belongs to.
 @param[in] event The event representing the outgoing Echo response.
 @return -
        - ROK -     If the procedure was successful.
        - ROUTRES - If the procedure failed due to memory failure.
        - RFAILED - If the procedure failed due to internal reasons.
 */
PRIVATE S16 egHandleOutEchoRsp ARGS ((
                               EgUSapCb *usap,
                               EgEvnt *event
                               ));

/**
  @ingroup TUNNMNGMT
 @brief The function egHandleOutEchoReq handles the outgoing Echo Request.
 @param[in] usap The usap Cb to which the message belongs to.
 @param[in] event The event representing the outgoing Echo request.
 @return -
        - ROK -     If the procedure was successful.
        - ROUTRES - If the procedure failed due to memory failure.
        - RFAILED - If the procedure failed due to internal reasons.
*/
PRIVATE S16 egHandleOutEchoReq ARGS ((
                                EgUSapCb *usap,
                                EgEvnt *event
                                ));

/**
  @ingroup TUNNMNGMT
 @brief The function egHandleIncEchoRsp handles the incoming Echo Response.
 @param[in] usap The usap Cb to which the message belongs to.
 @param[in] event The event representing the incoming Echo response
 @return -
        - ROK     - If the procedure was successful.
        - ROUTRES - If the procedure failed due to memory failure.
        - RFAILED - If the procedure failed due to internal reasons.
*/
PRIVATE S16 egHandleIncEchoRsp ARGS ((
                                EgUSapCb *usap,
                                EgEvnt *event
                                ));

/**
  @ingroup TUNNMNGMT
 @brief The function egHandleIncEchoReq handles the incoming Echo Request.
 @param[in] usap The usap Cb to which the message belongs to.
 @param[in] event The event representing the incoming Echo request.
 @return -
        - ROK -     If the procedure was successful.
        - ROUTRES - If the procedure failed due to memory failure.
        - RFAILED - If the procedure failed due to internal reasons.
*/
PRIVATE S16 egHandleIncEchoReq ARGS ((
                                EgUSapCb *usap,
                                EgEvnt *event
                                ));

/*****************************************************************
 *               Tunnel creation and deletion                    *
******************************************************************/

/**
  @ingroup TUNNMNGMT
  @brief The function egTunCreateSend creates a tunnel and sends the GTP-C
          request on that tunnel.
  @param[in] usap The usap is the pointer to the SAP control block on which the
                tunnel is created. It represents the GTP-C interface.
  @param[in] event The event is the event representing the request message
                which is creating the tunnel.
  @return -
        - ROK -     If the procedure was successful.
        - ROUTRES - If the procedure failed due to memory failure.
        - RFAILED - If the procedure failed due to internal reasons.
*/
PRIVATE S16 egTunCreateSend ARGS ((
                             EgUSapCb *usap,
                             EgEvnt *event
                             ));

/**
  @ingroup TUNNMNGMT
  @brief The function egTunCreateReceive creates a tunnel and for a GTP-C
          Create Session Request that is received.
  @param[in] usap The usap is the pointer to the SAP control block on which the
                tunnel is created. It represents the GTP-C interface.
  @param[in] event The event is the event representing the request message
                which is creating the tunnel.
  @return -
        - ROK -     If the procedure was successful.
        - ROUTRES - If the procedure failed due to memory failure.
        - RFAILED - If the procedure failed due to internal reasons.
*/
PRIVATE S16 egTunCreateReceive ARGS ((
                                EgUSapCb *usap,
                                EgEvnt *event
                                ));

/*eg004.201 Modified for eGTP-C PSF Upgarde*/
/*********************************************************************
 *                       Utilities                                   *
**********************************************************************/
/**
  @ingroup TUNNMNGMT
  @brief The function egSetDefPortAddr is an utility to set the remote
         port to the default port.
  @param[in] event The pointer to the event whose remote address is to be 
         overwritten.
*/
PRIVATE Void egSetDefPortAddr ARGS ((
                                EgEvnt *event
                                ));

/*eg004.201 Modified for eGTP-C PSF Upgarde*/
/**
  @ingroup TUNNMNGMT
 @brief The function egSaveMsgRemDetails saves the source details of an
        incoming request/notification/command message.
 @param[in] usap The usap is the pointer to the usap control block representing
              the interface on which message is sent. 
 @param[in] incReqCp The incReqCp is the control pointer of the hash list
                where the details are stored against sequence number.
 @param[in] incReqNode The incReqNode is node in which the details are 
                stored against sequence number.
 @param[in] event The pointer to the event.
 @return 
   - ROK      - If the procedure was successful.
   - ROUTRES  - If the procedure failed due to resource failure.
   - RFAILED  - If the procedure failed due to internal reasons.
*/

PRIVATE S16 egSaveMsgRemDetails ARGS ((
                                EgUSapCb *usap,
                                CmHashListCp *incReqCp,
                                EgIncReq *incReqNode,
                                EgEvnt *event
                                ));

/**
 @ingroup TUNNMNGMT
 @brief The function egGetMsgRemDetails gets the source details of an
        incoming request/notification/command message.
 @param[in] usap The usap is the pointer to the usap control block representing
              the interface on which message is sent. 
 @param[in] incReqCp The incReqCp is the control pointer of the hash list
                where the details are stored against sequence number.
 @param[in] incReqNode The incReqNode is node in which the details are 
                stored against sequence number.
 @param[in] event The pointer to the event.
 @param[in] msgCat The msgCat is the category of the message being saved.
 @return 
   - ROK      - If the procedure was successful.
   - ROUTRES  - If the procedure failed due to resource failure.
   - RFAILED  - If the procedure failed due to internal reasons.
*/

PRIVATE S16 egGetMsgRemDetails ARGS((
                                EgUSapCb *usap,
                                CmHashListCp *incReqCp,
                                EgIncReq *incReqNode,
                                EgEvnt *event,
                                EgtMsgCat msgCat
                                ));

/********************************************************************
 *                       FSM handlers                               *
 ********************************************************************/
/**
 @ingroup TUNNMNGMT
 @brief The function is the FSM handler for event - in the state -.
 @param[in] teCCb The pointer to the tunnel endpoint on which an event 
              has occured.
 @param[in] event The pointer to the event.
 @return 
   - ROK      - If the procedure was successful.
   - ROUTRES  - If the procedure failed due to resource failure.
   - RFAILED  - If the procedure failed due to internal reasons.
*/
PRIVATE S16 egTunnFsmInvNetEvnt ARGS ((
                                EgTECCb *teCCb,
                                EgEvnt *event
                                ));

/**
 @ingroup TUNNMNGMT
 @brief The function is the FSM handler for event - in the state -.
 @param[in] teCCb The pointer to the tunnel endpoint on which an event 
              has occured.
 @param[in] event The pointer to the event.
 @return 
    - ROK      - If the procedure was successful.
    - ROUTRES  - If the procedure failed due to resource failure.
    - RFAILED  - If the procedure failed due to internal reasons.
*/
PRIVATE S16 egTunnFsmInvUsrEvnt ARGS ((
                                EgTECCb *teCCb,
                                EgEvnt *event
                                ));

/**
 @ingroup TUNNMNGMT
 @brief The function is the FSM handler for event - receive session request
        in the state - IDLE(new tunnel).
 @param[in] teCCb The pointer to the tunnel endpoint on which an event 
              has occured.
 @param[in] event The pointer to the event.
 @return 
      - ROK      - If the procedure was successful.
      - ROUTRES  - If the procedure failed due to resource failure.
      - RFAILED  - If the procedure failed due to internal reasons.
*/
PRIVATE S16 egTunnFsmIdlISesReq ARGS ((
                                EgTECCb *teCCb,
                                EgEvnt *event
                                ));

/**
 @ingroup TUNNMNGMT
 @brief The function is the FSM handler for event - send Session Request,
        in the state - IDLE(new tunnel).
 @param[in] teCCb The pointer to the tunnel endpoint on which an event 
              has occured.
 @param[in] event The pointer to the event.
 @return 
    - ROK      - If the procedure was successful.
    - ROUTRES  - If the procedure failed due to resource failure.
    - RFAILED  - If the procedure failed due to internal reasons.
*/
PRIVATE S16 egTunnFsmIdlOSesReq ARGS ((
                                EgTECCb *teCCb,
                                EgEvnt *event
                                ));

/**
 @ingroup TUNNMNGMT
 @brief The function is the FSM handler for event - send Session Response,
         in the state - INCSESSREQ.
 @param[in] teCCb The pointer to the tunnel endpoint on which an event 
              has occured.
 @param[in] event The pointer to the event.
 @return 
    - ROK      - If the procedure was successful.
    - ROUTRES  - If the procedure failed due to resource failure.
    - RFAILED  - If the procedure failed due to internal reasons.
*/
PRIVATE S16 egTunnFsmIncOutRsp ARGS ((
                                EgTECCb *teCCb,
                                EgEvnt *event
                                ));

/**
 @ingroup TUNNMNGMT
 @brief The function is the FSM handler for event - receive Session Response
        in the state - OUTSESSREQ.
 @param[in] teCCb The pointer to the tunnel endpoint on which an event 
              has occured.
 @param[in] event The pointer to the event.
 @return 
    - ROK      - If the procedure was successful.
    - ROUTRES  - If the procedure failed due to resource failure.
    - RFAILED  - If the procedure failed due to internal reasons.
*/
PRIVATE S16 egTunnFsmOutIncRsp ARGS ((
                                EgTECCb *teCCb,
                                EgEvnt *event
                                ));
/**
 @ingroup TUNNMNGMT
 @brief The function is the FSM handler for event - error in the state 
        - OUTSESSREQ.
 @param[in] teCCb The pointer to the tunnel endpoint on which an event 
              has occured.
 @param[in] event The pointer to the event.
 @return 
    - ROK      - If the procedure was successful.
    - ROUTRES  - If the procedure failed due to resource failure.
    - RFAILED  - If the procedure failed due to internal reasons.
*/
PRIVATE S16 egTunnFsmOutErr ARGS ((
                                EgTECCb *teCCb,
                                EgEvnt *event
                                ));

/**
 @ingroup TUNNMNGMT
 @brief The function is the FSM handler for event - receive any GTP message
         in the state - CONNECTED.
 @param[in] teCCb The pointer to the tunnel endpoint on which an event 
              has occured.
 @param[in] event The pointer to the event.
 @return 
    - ROK      - If the procedure was successful.
    - ROUTRES  - If the procedure failed due to resource failure.
    - RFAILED  - If the procedure failed due to internal reasons.
*/
PRIVATE S16 egTunnFsmConIncMsg ARGS ((
                                EgTECCb *teCCb,
                                EgEvnt *event
                                ));

/**
 @ingroup TUNNMNGMT
 @brief The function is the FSM handler for event - send a GTP message 
        in the state - CONNECTED.
 @param[in] teCCb The pointer to the tunnel endpoint on which an event 
              has occured.
 @param[in] event The pointer to the event.
 @return 
    - ROK      - If the procedure was successful.
    - ROUTRES  - If the procedure failed due to resource failure.
    - RFAILED  - If the procedure failed due to internal reasons.
*/
PRIVATE S16 egTunnFsmConOutMsg ARGS ((
                                EgTECCb *teCCb,
                                EgEvnt *event
                                ));

/**
 @ingroup TUNNMNGMT
 @brief The function is the FSM handler for event - error, in the state 
         - CONNECTED.
 @param[in] teCCb The pointer to the tunnel endpoint on which an event 
              has occured.
 @param[in] event The pointer to the event.
 @return 
    - ROK      - If the procedure was successful.
    - ROUTRES  - If the procedure failed due to resource failure.
    - RFAILED  - If the procedure failed due to internal reasons.
*/
PRIVATE S16 egTunnFsmConErr ARGS ((
                                EgTECCb *teCCb,
                                EgEvnt *event
                                ));
/********************************************************************
 *               FSM handler function pointer definition            *
*********************************************************************/

typedef S16 (* EgTunnFsmDef) ARGS((
                                EgTECCb *teCCb,
                                EgEvnt *event
                                ));
/*eg004.201 Modified for eGTP-C PSF Upgarde*/
/**
  @ingroup TUNNMNGMT
  @brief egGetTunnelCb is function to get the tunnel Cb given the teid.
                 first get the array into which the teid falls into.
                 get the index ofthe teid in that array.
                 return the tunnel Cb.
  @param[in] teid The teid is the TEID of the tunnel.
  @return This function returns the pointer to the tunnel endpoint
                 control block.
*/
PUBLIC EgTECCb** egGetTunnelCb ARGS((
                                U32 teid
                                ));
PUBLIC S16 egReleaseTeid ARGS ((U32 teid));


#endif /* EGTP_C */

#ifdef __cplusplus
}
#endif

#endif /* __EG_TUNN_X__ */


  
/********************************************************************30**
  
         End of file:     eg_tunn.x@@/main/3 - Fri Feb  4 18:18:13 2011
    
*********************************************************************31*/


/********************************************************************40**
  
        Notes:
  
*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/

   
/********************************************************************60**
  
        Revision history:
  
*********************************************************************61*/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      mm                1. Created for Initial Release 1.1
/main/2      ---      rsharon           1. Initial for eGTP 1.2 Release
/main/3      ---      pmacharla       Initial Release of eGTP 2.1 release
/main/3      eg004.201 magnihotri       1. Modified for eGTP-C PSF Upgrade
*********************************************************************91*/
