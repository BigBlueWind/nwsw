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
/***********************************************************************

    Name: Source file containing IE COdes 

    Type: C Source

    Desc: 

    File: eg_dbutl.c

    Sid:      eg_dbutl.h@@/main/1 - Fri Feb  4 18:18:09 2011

    Prg : an

************************************************************************/
#ifdef EGTP_C

/*  Interface IE Codes */

#define EG_IMSI_IETYPE                                               1         
#define EG_PROCEDURE_TID_IETYPE                                      100       
#define EG_DRX_PARAM_IETYPE                                          101       
#define EG_UE_NTWRK_CAPB_IETYPE                                      102       
#define EG_GSM_KEY_TRIP_IETYPE                                       103       
#define EG_UMTS_KEY_CIPH_QUIN_IETYPE                                 104       
#define EG_GSM_KEY_CIPH_QUIN_IETYPE                                  105       
#define EG_UMTS_KEY_QUIN_IETYPE                                      106       
#define EG_EPS_SEC_QUAD_QUIN_IETYPE                                  107       
#define EG_UMTS_KEY_QUAD_QUIN_IETYPE                                 108       
#define EG_PDN_CONN_IETYPE                                           109       
#define EG_PDU_NUMBER_IETYPE                                         110       
#define EG_PTMSI_IETYPE                                              111       
#define EG_PTMSI_SIG_IETYPE                                          112       
#define EG_HOP_CNTR_IETYPE                                           113       
#define EG_UE_TIME_ZONE_IETYPE                                       114       
#define EG_TRACE_REF_IETYPE                                          115       
#define EG_COMP_REQ_MSG_IETYPE                                       116       
#define EG_GUTI_IETYPE                                               117       
#define EG_F_CONTAINER_IETYPE                                        118       
#define EG_F_CAUSE_IETYPE                                            119       
#define EG_SEL_PLMN_ID_IETYPE                                        120       
#define EG_TARGET_ID_IETYPE                                          121       
#define EG_PKT_FLOW_ID_IETYPE                                        123       
#define EG_RAB_CONTEXT_IETYPE                                        124       
#define EG_SRC_RNC_PDCP_CTX_IETYPE                                   125       
#define EG_PORT_IETYPE                                               126       
#define EG_APN_RESTRICT_IETYPE                                       127       
#define EG_SEL_MODE_IETYPE                                           128       
#define EG_SRC_ID_IETYPE                                             129       
#define EG_CNG_RPT_ACT_IETYPE                                        131       
#define EG_CSID_IETYPE                                               132       
#define EG_CHNL_NEEDED_IETYPE                                        133       
#define EG_EMLPP_PRIOR_IETYPE                                        134       
#define EG_NODE_TYPE_IETYPE                                          135       
#define EG_FQDN_IETYPE                                               136       
#define EG_TRANS_ID_IETYPE                                           137       
#define EG_MBMS_SESS_DURATION_IETYPE                                 138       
#define EG_MBMS_SERV_AREA_IETYPE                                     139       
#define EG_MBMS_SESS_ID_IETYPE                                       140       
#define EG_MBMS_FLOW_ID_IETYPE                                       141       
#define EG_MBMS_IP_MULT_DIST_IETYPE                                  142       
#define EG_MBMS_DIST_ACK_IETYPE                                      143       
#define EG_RFSP_ID_IETYPE                                            144       
#define EG_UCI_IETYPE                                                145       
#define EG_CSG_INFO_REP_ACT_IETYPE                                   146       
#define EG_CSG_ID_IETYPE                                             147       
#define EG_CMI_IETYPE                                                148       
#define EG_SERV_IND_IETYPE                                           149       
#define EG_CAUSE_IETYPE                                              2         
#define EG_PRIVATE_EXT_IETYPE                                        255       
#define EG_RECOVERY_IETYPE                                           3         
#define EG_APN_IETYPE                                                71        
#define EG_AMBR_IETYPE                                               72        
#define EG_EBI_IETYPE                                                73        
#define EG_IPADDR_IETYPE                                             74        
#define EG_MEI_IETYPE                                                75        
#define EG_MSISDN_IETYPE                                             76        
#define EG_INDICATION_IETYPE                                         77        
#define EG_PCO_IETYPE                                                78        
#define EG_PAA_IETYPE                                                79        
#define EG_BEARER_QOS_IETYPE                                         80        
#define EG_FLOW_QOS_IETYPE                                           81        
#define EG_RAT_TYPE_IETYPE                                           82        
#define EG_SERVING_NWK_IETYPE                                        83        
#define EG_BEARER_TFT_IETYPE                                         84        
#define EG_TAD_IETYPE                                                85        
#define EG_ULI_IETYPE                                                86        
#define EG_FTEID_IETYPE                                              87        
#define EG_TMSI_IETYPE                                               88        
#define EG_GBL_CN_ID_IETYPE                                          89        
#define EG_S103_PDN_FW_INFO_IETYPE                                   90        
#define EG_S1U_DATA_FW_IETYPE                                        91        
#define EG_DELAY_IETYPE                                              92        
#define EG_BEARER_CNTX_IETYPE                                        93        
#define EG_CHARGING_ID_IETYPE                                        94        
#define EG_CHARGING_CHAR_IETYPE                                      95        
#define EG_TRACE_INFO_IETYPE                                         96        
#define EG_BEARER_FLAGS_IETYPE                                       97        
#define EG_PDN_TYPE_IETYPE                                           99        
#define EG_TMGI_IETYPE                                               150
/* eg009.201 -Added New IE types for S2A and S2B  */
#define EG_LDN_IETYPE                                                151
#define EG_APCO_IETYPE                                               163


/* Message Codes */

#endif /* End of EGTP_C */
#ifdef EGTP_U

/* GTP-U Message Types */
#define EGU_ECHO_REQ          1
#define EGU_ECHO_RSP          2
#define EGU_ERROR_IND         26
#define EGU_SUPP_EXT_HDR_NTF  31
#define EGU_END_MARKER        254

/* GTP-U IEs */
#define EGU_RECOVERY_IETYPE   14 
#define EGU_TEID_IETYPE       16
#define EGU_GSN_ADDR_IETYPE   133
#define EGU_EXT_HDR_LST_IETYPE 141
#define EGU_PVT_EXTN_IETYPE   255


#endif /* End of EGTP_U */
/**********************************************************************
         End of file:     eg_dbutl.h@@/main/1 - Fri Feb  4 18:18:09 2011

*********************************************************************31*/

/********************************************************************40**

        Notes:

*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/


/********************************************************************60**

        Revision history:

*********************************************************************61*/

/********************************************************************80**

*********************************************************************81*/

/********************************************************************90**

    ver       pat    init                  description
----------- -------- ---- -----------------------------------------------
/main/1      ---      pmacharla       Initial Release of eGTP 2.1 release
            eg009.201 asaurabh     1. Added New IE types for S2A and S2B 
*********************************************************************91*/

