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


/**********************************************************************

     Name:     LTE-X2AP User
  
     Type:     C Source file
  
     Desc:     Build X2AP related messages

     Ret :     ROK - success
               RFAILED - failure

     File:     ve_x2_bldmsg.c

     Sid:      ve_x2_bldmsg.c@@/main/eNB_1.3_With_LatestGpr3/12 - Fri Sep 16 17:30:07 2011

     Prg:      

**********************************************************************/
#ifdef LTE_HO_SUPPORT

#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"           /* Common timer defines */
#include "cm_tkns.h"       /* Common tokens defines */
#include "cm_mblk.h"       /* Common memory allocation library defines */
#include "cm_llist.h"      /* Common link list  defines  */
#include "cm_hash.h"       /* Common hashlist  defines */
#include "cm_lte.h"        /* Common LTE defines */
#include "cm_inet.h"       /* Common internet defines */
#include "cm_tpt.h"        /* Common Transport defines */

#include "nhu_asn.h"       /* NHU ASN related defines */
#include "nhu.h"           /* NHU interface defines */
#include "pju.h"           /* PJU interface defines */
#include "cpj.h"           /* PDCP conrol interface defines */
#include "rgr.h"           /* MAC RRM control Interface */
#include "ctf.h"           /* PHY RRM control Interface */
#include "szt.h"           /* S1AP RRM control Interface */
#include "czt.h"           /* X2AP RRM control Interface */
#include "czt_asn.h"       /* X2AP ASN related defines  */
#ifdef EU_DAT_APP
#include "egt.h"           /* S1AP RRM control Interface */
#endif 
#include "lrg.h"           /* MAC RRM control Interface */
#include "cz.h"            /* X2AP header file     */


/* RRM related includes */
#include "ve.h"            /* enodeb application defines  */
#include "lve.h"           /* enodeb layer interface defines */
#ifdef LTE_ENB_PERF
#include "ve_perf.h"       /* enodeb performance header file */
#endif

/* header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */

#include "cm5.x"           /* Common timer library */
#include "cm_tkns.x"       /* Common tokens */
#include "cm_mblk.x"       /* Common memory allocation */
#include "cm_llist.x"      /* Common link list */
#include "cm_hash.x"       /* Common hashlist */
#include "cm_lte.x"        /* Common LTE includes */
#include "cm_lib.x"        /* Common library structures */
#include "cm_inet.x"       /* Common Inet structures */
#include "cm_tpt.x"        /* Common Transport Structures */

#include "nhu_asn.x"       /* NHU ASN structures   */
#include "nhu.x"           /* NHU Interface includes */
#include "cpj.x"           /* CPJ Interface includes */
#include "pju.x"           /* PJU Interface includes */
#include "rgr.x"           /* MAC RRM control Interface */
#include "ctf.x"           /* PHY RRM control Interface */
#include "szt.x"           /* S1AP RRM control Interface */
#include "czt.x"           /* X2AP RRM control Interface */
#ifdef EU_DAT_APP
#include "egt.x"           /* S1AP RRM control Interface */
#endif
#include "lrg.x"           /* MAC RRM control Interface */

/* RRM related includes */
#include "lve.x"           /* enodeb layer interface structures */
#include "ve_eut.x"        /* interface between data app and relay */
#include "ve.x"            /* enodeb application global structures */
#ifndef MSPD
#include "ve_dflt.h"       /* enodeb default header file */
#else
#include "ve_dflt_ms.x"       /* enodeb default header file */
#endif


/* X2_CFG_UPD_CHANGES */
#ifdef LTE_HO_SUPPORT
PRIVATE S16 veCfgUpdAddSrvdCell ARGS((
   U8                        numSrvdCellAdded,
   U16                       srvdCellAdd[],
   CztEvnt                   *x2ConfigUpdPdu,
   U8                        ieCnt
));

PRIVATE S16 veCfgUpdModSrvdCell ARGS((
   U8                        numSrvdCellModified,
   VeSrvdCellMod             srvdCellMod[],
   CztEvnt                   *x2ConfigUpdPdu,
   U8                        ieCnt
));

PRIVATE S16 veCfgUpdDelSrvdCell ARGS((
   U8                        numSrvdCellDeleted,
   VeCellEcgi                srvdCellDel[],
   CztEvnt                   *x2ConfigUpdPdu,
   U8                        ieCnt
));

PRIVATE S16 veCfgUpdAddGUGrpIDLst ARGS((
   U8                        numGrpIDAdded,
   VeGuId                    guIdAddLst[],
   CztEvnt                   *x2ConfigUpdPdu,
   U8                        ieCnt
));

PRIVATE S16 veCfgUpdDelGUGrpIDLst ARGS((
   U8                        numGrpIDDeleted,
   VeGuId                    guIdDelLst[],
   CztEvnt                   *x2ConfigUpdPdu,
   U8                        ieCnt
));
#endif

/*
*
*       Fun:   veUtlCztFillX2SetupReq
*
*       Desc:  Call handler to fill X2 setup request message.
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_x2_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veUtlCztFillX2SetupReq
(
CztEvnt      **pdu
)
#else
PUBLIC S16 veUtlCztFillX2SetupReq(pdu)
CztEvnt      **pdu;
#endif
{  
   CztEvnt    *x2SetupRqstPdu = NULLP;
   U16        numComp = 0;
   S16        ret = 0;
   U8         choice = 0;
   U16        len = 0;
   S16        idx = 0;
   CztProtIE_Field_X2SetupRqst_IEs *ie  = NULLP;
   CztSrvdCellsMember              *ie1 = NULLP;

   TRC2(veUtlCztFillX2SetupReq);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veUtlCztFillX2SetupReq: \
            Filling the X2 Setup Request \n"));
#endif

   /* Allocate and Initialize memory control point */
   ret = cmAllocEvnt(sizeof(CztEvnt), VE_CZ_MEM_SDU_SIZE, &veCb.mem, (Ptr *)&x2SetupRqstPdu);
   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillX2SetupReq:\
                                                       cmAllocEvnt failed \n"));
#endif
      RETVALUE(RFAILED);
   } /* end of if part */

   /* Initialize the PDU */
   cmMemset((U8 *)&(x2SetupRqstPdu->pdu), 0, sizeof(CztX2AP_PDU));

   /* Fill the contents of initiatingMsg */
   veFillTknU8(&(x2SetupRqstPdu->pdu.choice), X2AP_PDU_INITIATINGMSG);
   veFillTknU8(&(x2SetupRqstPdu->pdu.val.initiatingMsg.pres),PRSNT_NODEF);
   veFillTknU32(&(x2SetupRqstPdu->pdu.val.initiatingMsg.procedureCode), Cztid_x2Setup);
   veFillTknU32(&(x2SetupRqstPdu->pdu.val.initiatingMsg.criticality), CztCriticalityrejectEnum);
   veFillTknU8(&(x2SetupRqstPdu->pdu.val.initiatingMsg.value.u.cztX2SetupRqst.pres), PRSNT_NODEF);

   /* Filling 2 mandatory IEs (Global eNodebId and Served Cells) into the pdu */
   numComp = 2;
   veFillTknU16(&(x2SetupRqstPdu->pdu.val.initiatingMsg.value.u.cztX2SetupRqst.protocolIEs.noComp), numComp);

   /* Get memory for numComp */
   if ((cmGetMem(x2SetupRqstPdu, (numComp * sizeof(CztProtIE_Field_X2SetupRqst_IEs)),
               (Ptr*)&x2SetupRqstPdu->pdu.val.initiatingMsg.value.u.cztX2SetupRqst.protocolIEs.member)) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillX2SetupReq: cmGetMem failed \n"));
#endif
      /* Free the memory of Event Structure */
      VE_FREE_EVNT(x2SetupRqstPdu);
      RETVALUE(RFAILED);
   } /* end of if statement */

   /* IE1 - Filling ENB Global Id  in member[0]*/
   ie = &x2SetupRqstPdu->pdu.val.initiatingMsg.value.u.cztX2SetupRqst.protocolIEs.member[0];
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Cztid_GlobalENB_ID);
   veFillTknU32(&(ie->criticality), CztCriticalityrejectEnum);
   veFillTknU8(&(ie->value.u.cztGlobalENB_ID.pres), PRSNT_NODEF);
   /* Fill the PLMN ID of source Cell Id */
   len = 3;
   /* Using the first configured cell and the first PLMN ID in the list */
   veCzFillTknStrOSXL(&(ie->value.u.cztGlobalENB_ID.pLMN_Identity), 
                   len,(Data *)veCb.cellCb[0]->plmnId[0], &x2SetupRqstPdu);


   /* Check the type of enodeb whether it is MACRO or HOME */
   choice = veCb.eNBType;
   /* Fill the cell with appropriate enb ID */
   switch (choice)
   {
      case ENB_ID_MACRO_ENB_ID:
      {
         veFillTknU8(&(ie->value.u.cztGlobalENB_ID.eNB_ID.choice), choice);
         len = 20;
         veFillTknBStr32(&(ie->value.u.cztGlobalENB_ID.eNB_ID.val.macro_eNB_ID),len,\
             veCb.cellCb[0]->cellId);
         break;
      }   
      case ENB_ID_HOME_ENB_ID:
      {
         veFillTknU8(&(ie->value.u.cztGlobalENB_ID.eNB_ID.choice), choice);

         len = 28;
         veFillTknBStr32(&(ie->value.u.cztGlobalENB_ID.eNB_ID.val.home_eNB_ID), len, 
         veCb.cellCb[0]->cellId);

         /*
            czFillTknStr4(&(ie->value.u.cztGlobalENB_ID.eNB_ID.val.home_eNB_ID),4,ENB_ID_HOME_ENB_ID);
         */

         break;
      }
      default:
      {
         veFillTknU8(&(ie->value.u.cztGlobalENB_ID.eNB_ID.choice), ENB_ID_HOME_ENB_ID);
         len = 20;
         veFillTknBStr32(&(ie->value.u.cztGlobalENB_ID.eNB_ID.val.macro_eNB_ID),len,ENB_ID_MACRO_ENB_ID);
         break;
      }
   } /* end of switch statement */

   ie->value.u.cztGlobalENB_ID.iE_Extns.noComp.pres = NOTPRSNT;

   /* IE2 - Filling Served Cells */
   ie = &x2SetupRqstPdu->pdu.val.initiatingMsg.value.u.cztX2SetupRqst.protocolIEs.member[1];

   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Cztid_SrvdCells);
   veFillTknU32(&(ie->criticality),CztCriticalityrejectEnum);
   numComp = 1;
   veFillTknU16(&(ie->value.u.cztSrvdCells.noComp), numComp);

   if ((cmGetMem(x2SetupRqstPdu,(numComp * sizeof(CztSrvdCellsMember)),
               (Ptr *)&ie->value.u.cztSrvdCells.member)) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillX2SetupReq: cmGetMem failed\n"));      
#endif
      VE_FREE_EVNT(x2SetupRqstPdu);
      RETVALUE(RFAILED);
   }
   ie1 = &ie->value.u.cztSrvdCells.member[0];

   veFillTknU8(&(ie1->pres), PRSNT_NODEF);
   veFillTknU8(&(ie1->servedCellInfo.pres), PRSNT_NODEF);
   veFillTknU32(&(ie1->servedCellInfo.pCI), veCb.cellCb[0]->physCellId);
   veFillTknU8(&(ie1->servedCellInfo.cellId.pres), PRSNT_NODEF);
   /* Fill the PLMN ID in target Cell Id */
   len = 3;
   /* Using the first configured cell and the first PLMN ID in the list */
   veCzFillTknStrOSXL(&(ie1->servedCellInfo.cellId.pLMN_Identity), 
                   len, (Data *)veCb.cellCb[0]->plmnId[0], &x2SetupRqstPdu);

   if(ENB_ID_MACRO_ENB_ID == veCb.eNBType)
   {
      len = 20;
      veFillTknBStr32(&(ie1->servedCellInfo.cellId.eUTRANcellIdentifier), len,\
                      veCb.cellCb[0]->cellId);
   }
   else
   {
      len = 28;
      veFillTknBStr32(&(ie1->servedCellInfo.cellId.eUTRANcellIdentifier), len,\
                      veCb.cellCb[0]->cellId);
   }

   len = 2;

   veFillTknStr4(&(ie1->servedCellInfo.tAC), len, veCb.cellCb[0]->tac);

   /*  Filling broadcastPLMNs */
   numComp = veCb.cellCb[0]->numBrdCstPlmn;
   /* Allocate memory for broadcast PLMNs */
   if ((cmGetMem(x2SetupRqstPdu, numComp*sizeof(CztPLMN_Identity),
               (Ptr*)&ie1->servedCellInfo.broadcastPLMNs.member)) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillX2SetupReq: cmGetMem failed\n"));      
#endif
      VE_FREE_EVNT(x2SetupRqstPdu);
      RETVALUE(RFAILED);
   }
   veFillTknU16(&(ie1->servedCellInfo.broadcastPLMNs.noComp),numComp);

   for(idx = 0; idx < numComp; numComp++)
   {
      /* Fill the PLMN ID of source Cell Id */
      len = 3;
      /* Using the first configured cell and the first PLMN ID in the list */
      veCzFillTknStrOSXL(&(ie1->servedCellInfo.broadcastPLMNs.member[0]), len,
            (Data *)veCb.cellCb[0]->plmnId[idx], &x2SetupRqstPdu);
   }

   choice = veCb.cellCb[0]->duplexMode;

   switch (choice)
   {
      case VE_MODE_FDD:
      {
         veFillTknU8(&(ie1->servedCellInfo.eUTRA_Mode_Info.choice), 0);
         veFillTknU8(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.fDD.pres),PRSNT_NODEF);
         veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.fDD.uL_EARFCN),
                       veCb.cellCb[0]->u.fddInfo.ulEarfcn);
         veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.fDD.dL_EARFCN),
                             veCb.cellCb[0]->u.fddInfo.dlEarfcn);
    veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.fDD.uL_Transmission_Bandwidth),
                             veCb.cellCb[0]->u.fddInfo.ulTxBndWdth);
         veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.fDD.dL_Transmission_Bandwidth),
                             veCb.cellCb[0]->u.fddInfo.dlTxBndWdth);

         break;
      }
      case VE_MODE_TDD:
      {
         veFillTknU8(&(ie1->servedCellInfo.eUTRA_Mode_Info.choice), 1);
         veFillTknU8(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.tDD.pres),PRSNT_NODEF);
         veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.tDD.eARFCN),
                                           veCb.cellCb[0]->u.tddInfo.earfcn);
         veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.tDD.transmission_Bandwidth),
                                            veCb.cellCb[0]->u.tddInfo.txBndWdth);
         veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.tDD.subframeAssignment),
                                            veCb.cellCb[0]->u.tddInfo.sfAssgnmnt);
         /* This value has been set as NOTPRSNT as there is no support for special subframe in MAC-TDD */
         veFillTknU8(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.tDD.specialSubframe_Info.pres), NOTPRSNT);
         veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.tDD.specialSubframe_Info.specialSubframePatterns), 
                                            veCb.cellCb[0]->u.tddInfo.splSfInfo.splSfPatrn);
         veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.tDD.specialSubframe_Info.cyclicPrefixDL), 
                                            veCb.cellCb[0]->u.tddInfo.splSfInfo.dlCycPrefix);
         veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.tDD.specialSubframe_Info.cyclicPrefixUL), 
                                            veCb.cellCb[0]->u.tddInfo.splSfInfo.ulCycPrefix);
         ie1->servedCellInfo.eUTRA_Mode_Info.val.tDD.specialSubframe_Info.iE_Extns.noComp.pres = NOTPRSNT;
         break;
      }
      default:
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillX2SetupReq: Invalid Mode.\n"));
#endif
         break;
      }
   } /* end of switch statement */

   ie1->servedCellInfo.iE_Extns.noComp.pres = NOTPRSNT;
   /* neighbor info and GU grp info are optional */

   *pdu = x2SetupRqstPdu;

   RETVALUE(ROK);
   
} /* End  of veUtlCztFillX2SetupReq */


/*
*
*       Fun:   veUtlCztFillX2SetupRsp
*
*       Desc:  Call handler to fill X2 Setup Response message
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_x2_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veUtlCztFillX2SetupRsp
(
CztEvnt      **pdu
)
#else
PUBLIC S16 veUtlCztFillX2SetupRsp(pdu)
CztEvnt      **pdu;
#endif
{
   CztEvnt *x2SetupRespPdu = NULLP;
   U16     numComp = 0;
   S16     ret = RFAILED;
   U8      choice = 0;
   U16     len = 0;
   S16     idx = 0;
   CztProtIE_Field_X2SetupResp_IEs *ie  = NULLP;
   CztSrvdCellsMember              *ie1 = NULLP;

   TRC2(veUtlCztFillX2SetupRsp);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veUtlCztFillX2SetupRsp: \
            Filling the X2 Setup Response \n"));
#endif

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(CztEvnt), VE_CZ_MEM_SDU_SIZE, &veCb.mem, (Ptr *)&x2SetupRespPdu);

   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillX2SetupRsp: cmAllocEvnt failed\n"));
#endif
      RETVALUE(RFAILED);
   }

   cmMemset((U8 *)&(x2SetupRespPdu->pdu), 0, sizeof(CztX2AP_PDU));

   veFillTknU8(&(x2SetupRespPdu->pdu.choice), X2AP_PDU_SUCCESSFULOUTCOME);
   veFillTknU8(&(x2SetupRespPdu->pdu.val.successfulOutcome.pres),PRSNT_NODEF);
   veFillTknU32(&(x2SetupRespPdu->pdu.val.successfulOutcome.procedureCode),Cztid_x2Setup);
   veFillTknU32(&(x2SetupRespPdu->pdu.val.successfulOutcome.criticality),CztCriticalityignoreEnum);
   veFillTknU8(&(x2SetupRespPdu->pdu.val.successfulOutcome.value.u.cztX2SetupResp.pres),PRSNT_NODEF);

   /* Filling 2 mandatory headers */

   numComp = 2;
   veFillTknU16(&(x2SetupRespPdu->pdu.val.successfulOutcome.value.u.cztX2SetupResp.protocolIEs.noComp),numComp);

   /* Allocte memory for 2 IEs */
   if ((cmGetMem(x2SetupRespPdu, (numComp * sizeof(CztProtIE_Field_X2SetupResp_IEs)),
          (Ptr*)&x2SetupRespPdu->pdu.val.successfulOutcome.value.u.cztX2SetupResp.protocolIEs.member)) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillX2SetupRsp: cmGetMem failed\n"));
#endif
      VE_FREE_EVNT(x2SetupRespPdu);
      RETVALUE(RFAILED);
   } /* end of if statement */

   ie = &x2SetupRespPdu->pdu.val.successfulOutcome.value.u.cztX2SetupResp.protocolIEs.member[0];

   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Cztid_GlobalENB_ID);
   veFillTknU32(&(ie->criticality), CztCriticalityrejectEnum);
   veFillTknU8(&(ie->value.u.cztGlobalENB_ID.pres), PRSNT_NODEF);

   /* Fill the PLMN ID of source Cell Id */
   len = 3;
   /* Using the first configured cell and the first PLMN ID in the list */
   veCzFillTknStrOSXL(&(ie->value.u.cztGlobalENB_ID.pLMN_Identity), 
                   len,(Data *)veCb.cellCb[0]->plmnId[0], &x2SetupRespPdu);


   choice = veCb.eNBType;

   switch (choice)
   {
      case ENB_ID_MACRO_ENB_ID:
         veFillTknU8(&(ie->value.u.cztGlobalENB_ID.eNB_ID.choice), choice);
         len = 20;/* Interms of bits.*/
         veFillTknBStr32(&(ie->value.u.cztGlobalENB_ID.eNB_ID.val.macro_eNB_ID),len,\
                     veCb.cellCb[0]->cellId);
         break;
      case ENB_ID_HOME_ENB_ID:
         veFillTknU8(&(ie->value.u.cztGlobalENB_ID.eNB_ID.choice), choice);

         len = 28;
         veFillTknBStr32(&(ie->value.u.cztGlobalENB_ID.eNB_ID.val.home_eNB_ID), len, 
         veCb.cellCb[0]->cellId);

         /*
            czFillTknStr4(&(ie->value.u.cztGlobalENB_ID.eNB_ID.val.home_eNB_ID),4,ENB_ID_HOME_ENB_ID);
         */

         break;
      default:
         veFillTknU8(&(ie->value.u.cztGlobalENB_ID.eNB_ID.choice), ENB_ID_HOME_ENB_ID);
         len = 20;
         veFillTknBStr32(&(ie->value.u.cztGlobalENB_ID.eNB_ID.val.macro_eNB_ID),len,ENB_ID_MACRO_ENB_ID);
         break;
   }

   ie->value.u.cztGlobalENB_ID.iE_Extns.noComp.pres = NOTPRSNT;


   ie = &x2SetupRespPdu->pdu.val.successfulOutcome.value.u.cztX2SetupResp.protocolIEs.member[1];
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Cztid_SrvdCells);
   veFillTknU32(&(ie->criticality),CztCriticalityignoreEnum);

   numComp = 1;
   veFillTknU16(&(ie->value.u.cztSrvdCells.noComp), numComp);
   if ((cmGetMem(x2SetupRespPdu,(numComp * sizeof(CztSrvdCellsMember)),
                 (Ptr *)&ie->value.u.cztSrvdCells.member)) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillX2SetupRsp: cmGetMem failed\n"));
#endif
      VE_FREE_EVNT(x2SetupRespPdu);
      RETVALUE(RFAILED);
   }
   ie1 = &ie->value.u.cztSrvdCells.member[0];

   veFillTknU8(&(ie1->pres), PRSNT_NODEF);
   veFillTknU8(&(ie1->servedCellInfo.pres), PRSNT_NODEF);
   veFillTknU32(&(ie1->servedCellInfo.pCI), veCb.cellCb[0]->physCellId);
   veFillTknU8(&(ie1->servedCellInfo.cellId.pres), PRSNT_NODEF);
   /* Fill the PLMN ID in target Cell Id */
   len = 3;
   /* Using the first configured cell and the first PLMN ID in the list */
   veCzFillTknStrOSXL(&(ie1->servedCellInfo.cellId.pLMN_Identity), 
                   len, (Data *)veCb.cellCb[0]->plmnId[0], &x2SetupRespPdu);

   if(ENB_ID_MACRO_ENB_ID == veCb.eNBType)
   {
      len = 20;
      veFillTknBStr32(&(ie1->servedCellInfo.cellId.eUTRANcellIdentifier), len,\
                      veCb.cellCb[0]->cellId);
   }
   else
   {
      len = 28;
      veFillTknBStr32(&(ie1->servedCellInfo.cellId.eUTRANcellIdentifier), len,\
                      veCb.cellCb[0]->cellId);
   }

   len = 2;

   veFillTknStr4(&(ie1->servedCellInfo.tAC), len, veCb.cellCb[0]->tac);

      /* IE3 - Filling broadcastPLMNs */
      numComp = 1;
      /* Allocate memory for broadcast PLMNs */
   if ((cmGetMem(x2SetupRespPdu, numComp*sizeof(CztPLMN_Identity),
               (Ptr*)&ie1->servedCellInfo.broadcastPLMNs.member)) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillX2SetupRsp: cmGetMem failed \n"));
#endif
      VE_FREE_EVNT(x2SetupRespPdu);
      RETVALUE(RFAILED);
   }
   veFillTknU16(&(ie1->servedCellInfo.broadcastPLMNs.noComp),numComp);

   for(idx = 0; idx < numComp; numComp++)
   {
      /* Fill the PLMN ID of source Cell Id */
      len = 3;
      /* Using the first configured cell and the first PLMN ID in the list */
      veCzFillTknStrOSXL(&(ie1->servedCellInfo.broadcastPLMNs.member[0]), len,
            (Data *)veCb.cellCb[0]->plmnId[idx], &x2SetupRespPdu);
   }

   choice = veCb.cellCb[0]->duplexMode;

   switch (choice)
   {
      case VE_MODE_FDD:
      {
         veFillTknU8(&(ie1->servedCellInfo.eUTRA_Mode_Info.choice), 0);
         veFillTknU8(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.fDD.pres),PRSNT_NODEF);
         veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.fDD.uL_EARFCN),
                veCb.cellCb[0]->u.fddInfo.ulEarfcn);
         veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.fDD.dL_EARFCN),
                veCb.cellCb[0]->u.fddInfo.dlEarfcn);
         veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.fDD.uL_Transmission_Bandwidth),
                veCb.cellCb[0]->u.fddInfo.ulTxBndWdth);
         veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.fDD.dL_Transmission_Bandwidth),
                veCb.cellCb[0]->u.fddInfo.dlTxBndWdth);
         break;
      }
      case VE_MODE_TDD:
      {
         veFillTknU8(&(ie1->servedCellInfo.eUTRA_Mode_Info.choice), 1);
         veFillTknU8(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.tDD.pres),PRSNT_NODEF);
         veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.tDD.eARFCN),
                                           veCb.cellCb[0]->u.tddInfo.earfcn);
         veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.tDD.transmission_Bandwidth),
                                            veCb.cellCb[0]->u.tddInfo.txBndWdth);
         veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.tDD.subframeAssignment),
                                            veCb.cellCb[0]->u.tddInfo.sfAssgnmnt);
         /* This value has been set as NOTPRSNT as there is no support for special subframe in MAC-TDD */
         veFillTknU8(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.tDD.specialSubframe_Info.pres), NOTPRSNT);
         veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.tDD.specialSubframe_Info.specialSubframePatterns), 
                                            veCb.cellCb[0]->u.tddInfo.splSfInfo.splSfPatrn);
         veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.tDD.specialSubframe_Info.cyclicPrefixDL), 
                                            veCb.cellCb[0]->u.tddInfo.splSfInfo.dlCycPrefix);
         veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.tDD.specialSubframe_Info.cyclicPrefixUL), 
                                            veCb.cellCb[0]->u.tddInfo.splSfInfo.ulCycPrefix);
         ie1->servedCellInfo.eUTRA_Mode_Info.val.tDD.specialSubframe_Info.iE_Extns.noComp.pres = NOTPRSNT;
         break;
      }
      default:
         break;
   } /* end of switch statement */

   ie1->servedCellInfo.iE_Extns.noComp.pres = NOTPRSNT;

   /* neighbor info and GU grp info not mandatory*/

   *pdu = x2SetupRespPdu;

   RETVALUE(ROK);
} /* End of veUtlCztFillX2SetupRsp */

/*
*
*       Fun:   veUtlCztFillX2SetupFail
*
*       Desc:  Call handler to fill X2 Setup Failure message
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_x2_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veUtlCztFillX2SetupFail
(
CztEvnt      **pdu,
U16          cause,
U8           causeType,
Bool         sndTimeToWait,
U32          timeToWait
)
#else
PUBLIC S16 veUtlCztFillX2SetupFail(pdu, cause, causeType, sndTimeToWait, timeToWait)
CztEvnt      **pdu;
U16          cause;
U8           causeType;
Bool         sndTimeToWait;
U32          timeToWait;
#endif
{
   CztEvnt *x2SetupFailPdu = NULLP;
   U16     numComp = 0;
   S16     ret = RFAILED;
   CztProtIE_Field_X2SetupFail_IEs *ie = NULLP;

   TRC2(veUtlCztFillX2SetupFail);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veUtlCztFillX2SetupFail: \
            Filling the X2 Setup Fail\n"));
#endif

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(CztEvnt), VE_CZ_MEM_SDU_SIZE, &veCb.mem, (Ptr *)&x2SetupFailPdu);
   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillX2SetupFail: cmAllocEvnt failed\n"));
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */

   cmMemset((U8 *)&(x2SetupFailPdu->pdu), 0, sizeof(CztX2AP_PDU));

   veFillTknU8(&(x2SetupFailPdu->pdu.choice), X2AP_PDU_UNSUCCESSFULOUTCOME);
   veFillTknU8(&(x2SetupFailPdu->pdu.val.unsuccessfulOutcome.pres),PRSNT_NODEF);
   veFillTknU32(&(x2SetupFailPdu->pdu.val.unsuccessfulOutcome.procedureCode),Cztid_x2Setup);
   veFillTknU32(&(x2SetupFailPdu->pdu.val.unsuccessfulOutcome.criticality),CztCriticalityignoreEnum);
   veFillTknU8(&(x2SetupFailPdu->pdu.val.unsuccessfulOutcome.value.u.cztX2SetupFail.pres),PRSNT_NODEF);

   if ( sndTimeToWait == TRUE )
   {
    numComp = 2;
   }
   else
   {
    numComp = 1;
   }

   veFillTknU16(&(x2SetupFailPdu->pdu.val.unsuccessfulOutcome.value.u.cztX2SetupFail.protocolIEs.noComp),numComp);

   if ((cmGetMem(x2SetupFailPdu, numComp*sizeof(CztProtIE_Field_X2SetupFail_IEs),
                 (Ptr*)&x2SetupFailPdu->pdu.val.unsuccessfulOutcome.value.u.cztX2SetupFail.protocolIEs.member)) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillX2SetupFail: cmGetMem failed\n"));
#endif
      VE_FREE_EVNT(x2SetupFailPdu);
      RETVALUE(RFAILED);
   }

   /* IE1 - Filling cause */
   ie =
      &x2SetupFailPdu->pdu.val.unsuccessfulOutcome.value.u.cztX2SetupFail.protocolIEs.member[0];

   veFillTknU8(&(ie->pres),PRSNT_NODEF);
   veFillTknU32(&(ie->id),Cztid_Cause);
   veFillTknU32(&(ie->criticality),CztCriticalityignoreEnum);

   /* Filling the cause type and value */
   veFillTknU8(&(ie->value.u.cztCause.choice),causeType);
   veFillTknU32(&(ie->value.u.cztCause.val.radioNw),cause);

   if ( sndTimeToWait == FALSE )
   {
      *pdu = x2SetupFailPdu;
      RETVALUE(ROK);
   }
   /* IE2 - Filling, Time To Wait */
   ie =
      &x2SetupFailPdu->pdu.val.unsuccessfulOutcome.value.u.cztX2SetupFail.protocolIEs.member[1];
   veFillTknU8(&(ie->pres),PRSNT_NODEF);
   veFillTknU32(&(ie->id),Cztid_TimeToWait);
   veFillTknU32(&(ie->criticality),CztCriticalityignoreEnum);
   
   /* Filling the cause type and value */
   veFillTknU32(&(ie->value.u.cztTimeToWait), timeToWait);

   *pdu = x2SetupFailPdu;

   RETVALUE(ROK);
} /* End of veUtlCztFillX2SetupFail */


/*
*
*       Fun:   veUtlCztFillUeCxtRls
*
*       Desc:  Call handler to fill UE context release
*              message.
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_x2_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veUtlCztFillUeCxtRls
(
CztEvnt        **pdu,
U32            oldUEX2APId,
U32            newUEX2APId
)
#else
PUBLIC S16 veUtlCztFillUeCxtRls(pdu, oldUEX2APId, newUEX2APId)
CztEvnt        **pdu;
U32            oldUEX2APId;
U32            newUEX2APId;
#endif
{
   CztEvnt    *ueCntxtRlsPdu = NULLP;
   S16        numComp = 0;
   S16        ret = 0;
   CztProtIE_Field_UECntxtRls_IEs *ie = NULLP;

   TRC2(veUtlCztFillUeCxtRls);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veUtlCztFillUeCxtRls: \
            Filling the UE Context Release\n"));
#endif

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(CztEvnt), VE_CZ_MEM_SDU_SIZE, &veCb.mem, (Ptr *)&ueCntxtRlsPdu);
   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillUeCxtRls: cmAllocEvnt failed \n"));
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */

   cmMemset((U8 *)&(ueCntxtRlsPdu->pdu), 0, sizeof(CztX2AP_PDU));

   veFillTknU8(&(ueCntxtRlsPdu->pdu.choice), X2AP_PDU_INITIATINGMSG);
   veFillTknU8(&(ueCntxtRlsPdu->pdu.val.initiatingMsg.pres), PRSNT_NODEF);
   veFillTknU32(&(ueCntxtRlsPdu->pdu.val.initiatingMsg.procedureCode), Cztid_uECntxtRls);
   veFillTknU32(&(ueCntxtRlsPdu->pdu.val.initiatingMsg.criticality), 
         CztCriticalityrejectEnum);

   /* Fill the requested IEs */
   numComp = 2;
   veFillTknU8(&(ueCntxtRlsPdu->pdu.val.initiatingMsg.value.u.cztUECntxtRls.pres), 
                      PRSNT_NODEF);
   veFillTknU16(&(ueCntxtRlsPdu->pdu.val.initiatingMsg.value.u.cztUECntxtRls.
                     protocolIEs.noComp),numComp);

   if ((cmGetMem(ueCntxtRlsPdu, (numComp * sizeof(CztProtIE_Field_UECntxtRls_IEs)), 
    (Ptr *)&ueCntxtRlsPdu->pdu.val.initiatingMsg.value.u.cztUECntxtRls.protocolIEs.member)) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillUeCxtRls: cmGetMem failed \n"));
#endif
      VE_FREE_EVNT(ueCntxtRlsPdu);
      RETVALUE(RFAILED);
   }

   /* Fill the Old EnB UE X2AP ID */
   ie = &ueCntxtRlsPdu->pdu.val.initiatingMsg.value.u.cztUECntxtRls.protocolIEs.member[0];

   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Cztid_Old_eNB_UE_X2AP_ID);
   veFillTknU32(&(ie->criticality), CztCriticalityrejectEnum);
   veFillTknU32(&(ie->value.u.cztUE_X2AP_ID_Old), oldUEX2APId);


   /* Fill the New EnB UE X2AP ID */
   ie = &ueCntxtRlsPdu->pdu.val.initiatingMsg.value.u.cztUECntxtRls.protocolIEs.member[1];

   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Cztid_New_eNB_UE_X2AP_ID);
   veFillTknU32(&(ie->criticality), CztCriticalityrejectEnum);
   veFillTknU32(&(ie->value.u.cztUE_X2AP_ID_New), newUEX2APId);

   *pdu = ueCntxtRlsPdu; 

   RETVALUE(ROK);

} /* End of  veUtlCztFillUeCxtRls */

/*
*
*       Fun:   veUtlCztFillHovrRqst 
*
*       Desc:  Call handler to fill Handover Request message
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_x2_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veUtlCztFillHovrRqst
(
CztEvnt          **pdu,
VeNghCellCb      *nghCellCb,
VeX2ConCb        *x2ConCb,
NhuEncCfmSdus    *nhuEncCfmSdus
)
#else
PUBLIC S16 veUtlCztFillHovrRqst(pdu, nghCellCb, x2ConCb, nhuEncCfmSdus)
CztEvnt          **pdu;
VeNghCellCb      *nghCellCb;
VeX2ConCb        *x2ConCb;
NhuEncCfmSdus    *nhuEncCfmSdus;
#endif
{
   CztEvnt    *hovrRqstPdu = NULLP;
   S16        numComp = 0;
   S16        idx = 0;
   S16        compCnt = 0;
   S16        ret = 0;
   U16        len = 0;
   U8         eNBKeyVal[256]; 
   Data val = 120;

   CztProtIE_Field_HovrRqst_IEs                   *ie = NULLP;
   CztProtIE_Single_Cont_E_RABs_ToBeSetup_ItemIEs *ie1 = NULLP;
   CztLastVisitedCell_Item                        *ie2 = NULLP;

   TRC2(veUtlCztFillHovrRqst);

   if(NULLP == nghCellCb)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillHovrRqst: nghCellCb is NULL.\n"));
#endif
      RETVALUE(RFAILED);
   }

   if(NULLP == x2ConCb)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillHovrRqst: x2ConCb is NULL.\n"));
#endif
      RETVALUE(RFAILED);
   }

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veUtlCztFillHovrRqst: \
            Filling the UE Handover Request\n"));
#endif

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(CztEvnt), VE_CZ_MEM_SDU_SIZE, &veCb.mem, 
                                                           (Ptr *)&hovrRqstPdu);
   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillHovrRqst: cmAllocEvnt failed\n"));
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */

   /* Fill the Interface Elementary Procedure.*/
   cmMemset((U8 *)&(hovrRqstPdu->pdu), 0, sizeof(CztX2AP_PDU));
   veFillTknU8(&(hovrRqstPdu->pdu.choice), X2AP_PDU_INITIATINGMSG);
   veFillTknU8(&(hovrRqstPdu->pdu.val.initiatingMsg.pres), PRSNT_NODEF);
   veFillTknU32(&(hovrRqstPdu->pdu.val.initiatingMsg.procedureCode), 
                                                            Cztid_handoverPrep);
   veFillTknU32(&(hovrRqstPdu->pdu.val.initiatingMsg.criticality), 
                                                      CztCriticalityrejectEnum);

   numComp = 6;/* Indicates no of mandatory fields are present.*/
   veFillTknU8(&(hovrRqstPdu->pdu.val.initiatingMsg.value.u.
                                                cztHovrRqst.pres), PRSNT_NODEF);
   veFillTknU16(&(hovrRqstPdu->pdu.val.initiatingMsg.value.u.cztHovrRqst.
                                                protocolIEs.noComp),numComp);

   if (ROK != (cmGetMem(hovrRqstPdu, 
       (numComp * sizeof(CztProtIE_Field_HovrRqst_IEs)), 
       (Ptr *)&hovrRqstPdu->pdu.val.initiatingMsg.value.u.cztHovrRqst.protocolIEs.member)))
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillHovrRqst: cmGetMem failed\n"));
#endif
      VE_FREE_EVNT(hovrRqstPdu);
      RETVALUE(RFAILED);
   } /* end of if statement */

   /* Fill the OLD ENB UE X2AP ID */
   ie = &hovrRqstPdu->pdu.val.initiatingMsg.value.u.cztHovrRqst.protocolIEs.
                                                              member[compCnt++];
   /* This is done intentionally to check at stack if this ID is not present */
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Cztid_Old_eNB_UE_X2AP_ID);
   veFillTknU32(&(ie->criticality), CztCriticalityrejectEnum);
   veFillTknU32(&(ie->value.u.cztUE_X2AP_ID), x2ConCb->oldEnbUeX2apId);

   /* Fill the Cause IE */
   ie = &hovrRqstPdu->pdu.val.initiatingMsg.value.u.cztHovrRqst.protocolIEs.
                                                              member[compCnt++];
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Cztid_Cause);
   veFillTknU32(&(ie->criticality), CztCriticalityignoreEnum);
   veFillTknU8(&(ie->value.u.cztCause.choice),x2ConCb->cause.choice.val);

   /*choice = x2ConCb->cause.choice;*/
  if(x2ConCb->cause.choice.pres)
  {
    switch (x2ConCb->cause.choice.val)
    {
      case CAUSE_RADIONW:
      {
         veFillTknU32(&(ie->value.u.cztCause.val.radioNw), 
                                                    x2ConCb->cause.val.radioNw.val);
         break;
      }
      case CAUSE_TRANSPORT:
      {
         veFillTknU32(&(ie->value.u.cztCause.val.transport), 
                                                  x2ConCb->cause.val.transport.val);
         break;
      }
      case CAUSE_PROTOCOL:
      {
         veFillTknU32(&(ie->value.u.cztCause.val.protocol), 
                                                   x2ConCb->cause.val.protocol.val);
         break;
      }
      case CAUSE_MISC:
      {
         veFillTknU32(&(ie->value.u.cztCause.val.misc), 
                                                       x2ConCb->cause.val.misc.val);
         break;
      }
      default:
      {
#ifdef DEBUGP
          VE_DBG_ERROR((VE_PRNT_BUF,"filling default HO CAUSE value\n"));
#endif
          veFillTknU8(&(ie->value.u.cztCause.choice), CAUSE_RADIONW);
          veFillTknU32(&(ie->value.u.cztCause.val.radioNw), 
                       CztCauseRadioNwhandover_desirable_for_radio_reasonsEnum);
          break;
      }/* end of default */
    } /* end of switch statement */
  } /* end of cause if statement */

   /* Fill the target Cell Id (i.e) E-UTRAN Cell Global Identifier */
   ie = &hovrRqstPdu->pdu.val.initiatingMsg.value.u.cztHovrRqst.protocolIEs.
                                                              member[compCnt++];
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Cztid_TgetCell_ID);
   veFillTknU32(&(ie->criticality), CztCriticalityrejectEnum);
   veFillTknU8(&(ie->value.u.cztECGI.pres), PRSNT_NODEF);

   /* Fill the PLMN ID of target Cell Id */
   len = 3; /* Length of the PLMN Identity */
   veCzFillTknStrOSXL(&(ie->value.u.cztECGI.pLMN_Identity), 
                                 len,(Data *) &nghCellCb->plmnId, &hovrRqstPdu);
   /* Fill the E-UTRAN Cell Identifier of target cell id*/
   len = 28; /* Length of the E-UTRAN cell identifier in term of Bits. */
   veFillTknBStr32(&(ie->value.u.cztECGI.eUTRANcellIdentifier),len,
                                          nghCellCb->veCellPrp.cellId);
   /*
    * Start of Comment
    * Fill the GUMMEI which indicates the globally unique MME identity 
    * where UE is registered.
    * It Contains Globally Unique group id and MME Code.
    * End of Comment
    */
   ie = &hovrRqstPdu->pdu.val.initiatingMsg.value.u.cztHovrRqst.
                                                  protocolIEs.member[compCnt++];
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Cztid_GUMMEI_ID);
   veFillTknU32(&(ie->criticality), CztCriticalityrejectEnum);
   veFillTknU8(&(ie->value.u.cztGUMMEI.pres),PRSNT_NODEF);
   /* Fill the GU Group Id of of GUMMEI */
   veFillTknU8(&(ie->value.u.cztGUMMEI.gU_Group_ID.pres),PRSNT_NODEF);
   /* Fill the PLMN Idendity of GU Group Id */
   len = 3; /* Length of the PLMN Identity */
   veCzFillTknStrOSXL(&(ie->value.u.cztGUMMEI.gU_Group_ID.pLMN_Identity), 
                                     len,(Data *)veCb.cellCb[0]->plmnId, &hovrRqstPdu);
   /* Fill the MME Group ID of GU Group Id */
   len = 2;
   veFillTknStr4(&(ie->value.u.cztGUMMEI.gU_Group_ID.mME_Group_ID), len, 1);
   ie->value.u.cztGUMMEI.gU_Group_ID.iE_Extns.noComp.pres = NOTPRSNT;
   /* TEST_FIX */
   /*veFillTknBStr32(&(ie->value.u.cztGUMMEI.gU_Group_ID.mME_Group_ID), len, 1);*/
   /* Fill the MME Code of GUMMEI */
   len = 1;
   veFillTknStr4(&(ie->value.u.cztGUMMEI.mMME_Code), len, 1);
   /*veFillTknBStr32(&(ie->value.u.cztGUMMEI.mMME_Code), len, 1); */
   /* 
    * Start of Comments
    * Source eNB Fill the UE Context Information to prepare Handover at 
    * the target eNB side. UE context contains Security capabilities,
    * Aggregate Max bit rate,E-RAB setup list and corresponding QOS parameters,
    * Tunnel details.
    * End of Comments
    */
   if(x2ConCb != NULLP && x2ConCb->ueCb != NULLP && nghCellCb != NULLP)
   {
      ie = &hovrRqstPdu->pdu.val.initiatingMsg.value.u.cztHovrRqst.protocolIEs.
                                                              member[compCnt++];
      veFillTknU8(&(ie->pres), PRSNT_NODEF);
      veFillTknU32(&(ie->id), Cztid_UE_CntxtInform);
      veFillTknU32(&(ie->criticality), CztCriticalityrejectEnum);

      veFillTknU8(&(ie->value.u.cztUE_CntxtInform.pres),PRSNT_NODEF);
      /* Fill the rest of the Values in UE Context Information */
      veFillTknU32(&(ie->value.u.cztUE_CntxtInform.mME_UE_S1AP_ID),
                                        x2ConCb->ueCb->s1ConCb->mme_ue_s1ap_id);
      /* Fill the security capabilities of UE */
      veFillTknU8(&(ie->value.u.cztUE_CntxtInform.uESecurCapabilities.pres), 
                                                             PRSNT_NODEF); 
      /* Fill the Encryption Algorithm suported by UE */ 
      veFillTknBStr32(&(ie->value.u.cztUE_CntxtInform.uESecurCapabilities.
        encryptionAlgorithms), VE_CIPH_KEY_LEN,*(x2ConCb->ueCb->secCfg.cpCiphKey));
      /* Fill the Integrity Algorithms which is supported by UE */
      veFillTknBStr32(&(ie->value.u.cztUE_CntxtInform.uESecurCapabilities.
        integrityProtectionAlgorithms),VE_INTG_KEY_LEN,*(x2ConCb->ueCb->secCfg.intgKey));
      ie->value.u.cztUE_CntxtInform.uESecurCapabilities.iE_Extns.
                                                         noComp.pres = NOTPRSNT;

      /* Fill the AS Security Information */
      cmMemset(eNBKeyVal,0,sizeof(eNBKeyVal));
      for (idx = 0; idx < 256; idx++)
          eNBKeyVal[idx] = 0xFF;
      len = 256;/* Length of the eNB Key */
      veFillTknU8(&(ie->value.u.cztUE_CntxtInform.aS_SecurInform.pres),
                                                              PRSNT_NODEF);
      /* Fill the Key eNodeB Star value. */
      veCzFillTknStrOSXL(&(ie->value.u.cztUE_CntxtInform.
                 aS_SecurInform.key_eNodeB_star), len, (Data *)eNBKeyVal, &hovrRqstPdu);
      /* Fill Next Hopchaining Count. */
      veFillTknU32(&(ie->value.u.cztUE_CntxtInform.aS_SecurInform.
                                                       nextHopChainingCount),5);

      ie->value.u.cztUE_CntxtInform.aS_SecurInform.iE_Extns.noComp.
                                                                pres = NOTPRSNT;

      /*Fill the UE Aggregate Max Bit Rate */
      veFillTknU8(&(ie->value.u.cztUE_CntxtInform.
                                      uEaggregateMaxBitRate.pres), PRSNT_NODEF);
      len = 4;
      veCzFillTknStrOSXL(&(ie->value.u.cztUE_CntxtInform.uEaggregateMaxBitRate.
        uEaggregateMaxBitRateDlnk), len, (Data *)&x2ConCb->ueCb->rbInfo.aggBr.dl, 
                                                                  &hovrRqstPdu);

      veCzFillTknStrOSXL(&(ie->value.u.cztUE_CntxtInform.uEaggregateMaxBitRate.
                uEaggregateMaxBitRateUlnk), len, (Data *)&x2ConCb->ueCb->rbInfo.aggBr.ul,
                                                                  &hovrRqstPdu);
      ie->value.u.cztUE_CntxtInform.uEaggregateMaxBitRate.iE_Extns.noComp.
                                                                pres = NOTPRSNT;

      /* RAB Setup list */
      /*numComp = 1;*/
      veFillTknU16(&(ie->value.u.cztUE_CntxtInform.e_RABs_ToBeSetup_Lst.
                                    noComp), x2ConCb->ueCb->rbInfo.numOfRbCfgd);
      if ((cmGetMem(hovrRqstPdu, 
         (x2ConCb->ueCb->rbInfo.numOfRbCfgd * sizeof(CztProtIE_Single_Cont_E_RABs_ToBeSetup_ItemIEs)), 
         (Ptr *)&ie->value.u.cztUE_CntxtInform.e_RABs_ToBeSetup_Lst.member)) !=ROK)
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillHovrRqst: cmGetMem failed \n"));
#endif
         VE_FREE_EVNT(hovrRqstPdu);
         RETVALUE(RFAILED);
      }
      for(idx = 0; idx < x2ConCb->ueCb->rbInfo.numOfRbCfgd; idx++)
      {
         ie1 = &ie->value.u.cztUE_CntxtInform.e_RABs_ToBeSetup_Lst.member[idx];

         veFillTknU8(&(ie1->pres), PRSNT_NODEF);
         veFillTknU32(&(ie1->id), Cztid_E_RABs_ToBeSetup_Item);
         veFillTknU32(&(ie1->criticality), CztCriticalityignoreEnum);

         veFillTknU8(&(ie1->value.u.cztE_RABs_ToBeSetup_Item.pres),
                                                                   PRSNT_NODEF);
         /* Fill the E-RAB ID */
         veFillTknU32(&(ie1->value.u.cztE_RABs_ToBeSetup_Item.e_RAB_ID),
                                         x2ConCb->ueCb->rbInfo.rab[idx].eRABId);
         /* Fill the E-RAB QOS Parameters.*/
         veFillTknU8(&(ie1->value.u.cztE_RABs_ToBeSetup_Item.
                                       e_RAB_Lvl_QoS_Params.pres), PRSNT_NODEF);
         /* Fill the QCI value. */
         veFillTknU32(&(ie1->value.u.cztE_RABs_ToBeSetup_Item.
                 e_RAB_Lvl_QoS_Params.qCI), x2ConCb->ueCb->rbInfo.rab[idx].qci);
         /* 
          * Start of Comments
          * Fill the ARP values are priority level,pre-emption capability and 
          * pre-emption vulnearability.
          * In Current release, No ARP Details.So values are filled as constant.
          */
         veFillTknU8(&(ie1->value.u.cztE_RABs_ToBeSetup_Item.
          e_RAB_Lvl_QoS_Params. allocationAndRetentionPriority.pres),PRSNT_NODEF);
         veFillTknU32(&(ie1->value.u.cztE_RABs_ToBeSetup_Item.
                        e_RAB_Lvl_QoS_Params. allocationAndRetentionPriority.
                                                              priorityLvl), 15);
         veFillTknU32(&(ie1->value.u.cztE_RABs_ToBeSetup_Item.
                        e_RAB_Lvl_QoS_Params. allocationAndRetentionPriority.
                                                        pre_emptionCapblty), 1);
         veFillTknU32(&(ie1->value.u.cztE_RABs_ToBeSetup_Item.
                        e_RAB_Lvl_QoS_Params. allocationAndRetentionPriority.
                                                  pre_emptionVulnerability), 1);

         ie1->value.u.cztE_RABs_ToBeSetup_Item.e_RAB_Lvl_QoS_Params.
                allocationAndRetentionPriority. iE_Extns.noComp.pres = NOTPRSNT;
         ie1->value.u.cztE_RABs_ToBeSetup_Item.e_RAB_Lvl_QoS_Params.
                                                iE_Extns.noComp.pres = NOTPRSNT;

         /* Future, add logic in determining whether DL forwarding needs to be 
            requested or not */
         /* Fill the DL Forwarding IE */
         veFillTknU32(&(ie1->value.u.cztE_RABs_ToBeSetup_Item.dL_Fwding), 
               CztDL_FwdingdL_forwardingProposedEnum);
         /* set the dataFwd flag to TRUE */
         x2ConCb->ueCb->rbInfo.rab[idx].isDataFwdAvl = TRUE;

         /* Filling of UL GTP Tunnel Endpoint i.e S-GW Tunnel Endpoint Ids
          * for the delivery of the UL Packets from Target eNB to S-GW
          */

         /* Fill the ULink GTP Tunnel */
         veFillTknU8(&(ie1->value.u.cztE_RABs_ToBeSetup_Item.
                                        uL_GTPtunnelEndpoint.pres),PRSNT_NODEF);
         if (CM_IPV4ADDR_TYPE == x2ConCb->ueCb->rbInfo.rab[idx].sgwAddr.type )
         {   
            /* Fill the Transport layer address */
            len = 32;
            veCzFillTknStrBSXL(&(ie1->value.u.cztE_RABs_ToBeSetup_Item.
                     uL_GTPtunnelEndpoint.transportLyrAddr),len, 
                  (Data *)&x2ConCb->ueCb->rbInfo.rab[idx].sgwAddr.u.ipv4TptAddr.address, (S1apPdu**)&hovrRqstPdu);
         } 
         else if( CM_IPV6ADDR_TYPE == x2ConCb->ueCb->rbInfo.rab[idx].sgwAddr.type )
         {
            /* Fill the Transport layer address */
            len = 128;
            veCzFillTknStrBSXL(&(ie1->value.u.cztE_RABs_ToBeSetup_Item.
                     uL_GTPtunnelEndpoint.transportLyrAddr),len, 
                  (Data *)&x2ConCb->ueCb->rbInfo.rab[idx].sgwAddr.u.ipv6TptAddr.ipv6NetAddr, (S1apPdu**)&hovrRqstPdu);
         }    

         /* Fill the GTP Tunnel End Point*/ 

         /* Allocate memory for the octect string */ 
         len = x2ConCb->ueCb->rbInfo.rab[idx].remEgtpTunId.len;
         VE_ALLOC(&(ie1->value.u.cztE_RABs_ToBeSetup_Item.uL_GTPtunnelEndpoint.gTP_TEID.val), len);
         ie1->value.u.cztE_RABs_ToBeSetup_Item.uL_GTPtunnelEndpoint.gTP_TEID.pres = TRUE;
         /* Copy Byte by Byte */
         cmMemcpy(ie1->value.u.cztE_RABs_ToBeSetup_Item.uL_GTPtunnelEndpoint.gTP_TEID.val,
               x2ConCb->ueCb->rbInfo.rab[idx].remEgtpTunId.val,len);
         ie1->value.u.cztE_RABs_ToBeSetup_Item.uL_GTPtunnelEndpoint.gTP_TEID.len = len;
         
         ie1->value.u.cztE_RABs_ToBeSetup_Item.uL_GTPtunnelEndpoint.
                                                iE_Extns.noComp.pres = NOTPRSNT;
         ie1->value.u.cztE_RABs_ToBeSetup_Item.iE_Extns.noComp.pres = NOTPRSNT;

      }/* End of for(idx = 0; idx < ue_Cntxt_Info->e_RABs_ToBeSetup_Lst.noComp; idx++)*/

      /* Fill the RRC Context */
      len = 2;
      veCzFillTknStrOSXL(&(ie->value.u.cztUE_CntxtInform.rRC_Cntxt),
                                      len, (Data *)x2ConCb->ueCb->trnspContainer->sdu.mBuf, &hovrRqstPdu);
      ie->value.u.cztUE_CntxtInform.iE_Extns.noComp.pres = NOTPRSNT;
   }/* End of if(ue_Cb = NULLP && nghCellCb != NULLP && x2ConCb != NULLP)*/
   else
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillHovrRqst: UE Context Information Is Null\n"));
#endif
      VE_FREE_EVNT(hovrRqstPdu);
      RETVALUE(RFAILED);
   } /* end of else part */
/* SELVA UeHistory informations are hardcoded to proceed further here */
#ifdef NON_DUMMY_UE_HISTORY
   /* Fill the UE History Information */
   ie = &hovrRqstPdu->pdu.val.initiatingMsg.value.u.cztHovrRqst.protocolIEs.
                                                                member[compCnt];
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Cztid_UE_HistoryInform);
   veFillTknU32(&(ie->criticality), CztCriticalityignoreEnum);
   veFillTknU16(&(ie->value.u.cztUE_HistoryInform.noComp),
                                               x2ConCb->ueCb->ueHistory.noComp.val);
   if ((cmGetMem(hovrRqstPdu,
      (x2ConCb->ueCb->ueHistory.noComp.val * sizeof(CztLastVisitedCell_Item)), 
                         (Ptr *)&ie->value.u.cztUE_HistoryInform.member)) !=ROK)
   {
      #ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillHovrRqst: cmGetMem failed\n"));
      #endif
         VE_FREE_EVNT(hovrRqstPdu);
         RETVALUE(RFAILED);
   }
   for(idx = 0; idx < (x2ConCb->ueCb->ueHistory.noComp.val); idx++)
   {
      ie2 = &ie->value.u.cztUE_HistoryInform.member[idx];
         /*choice = LASTVISITEDCELL_ITEM_UTRAN_CELL;*/
      veFillTknU8(&(ie2->choice), 
                                  (x2ConCb->ueCb->ueHistory.member + idx)->choice.val);

      switch ((x2ConCb->ueCb->ueHistory.member + idx)->choice.val)
      {
          case LASTVISITEDCELL_ITEM_E_UTRAN_CELL:
          { 
               veFillTknU8(&(ie2->val.e_UTRAN_Cell.pres),
                   (x2ConCb->ueCb->ueHistory.member + idx)->val.e_UTRAN_Cell.pres.val);
               veFillTknU8(&(ie2->val.e_UTRAN_Cell.global_Cell_ID.pres), 
                                                                   PRSNT_NODEF);
               /* Fill the PLMN ID of global cell id */
               veCzFillTknStrOSXL(&(ie2->val.e_UTRAN_Cell.global_Cell_ID.
                  pLMN_Identity), (x2ConCb->ueCb->ueHistory.member + idx)->val.
                                   e_UTRAN_Cell.global_Cell_ID.pLMNidentity.len,
                  (Data *)&((x2ConCb->ueCb->ueHistory.member + idx)->val.e_UTRAN_Cell.
                                     global_Cell_ID.pLMNidentity.val), &hovrRqstPdu);
               /* Fill the E-UTRAN Cell Identifier of global cell id*/
               veFillTknBStr32(&(ie2->val.e_UTRAN_Cell.global_Cell_ID.
                  eUTRANcellIdentifier), (x2ConCb->ueCb->ueHistory.member + idx)->val.
                  e_UTRAN_Cell.global_Cell_ID.cell_ID.len, *((x2ConCb->ueCb->
                  ueHistory.member + idx)->val.e_UTRAN_Cell.global_Cell_ID.cell_ID.val));
               veFillTknU8(&(ie2->val.e_UTRAN_Cell.cellTyp.pres),
                  (x2ConCb->ueCb->ueHistory.member + idx)->val.e_UTRAN_Cell.cellTyp.pres.val);
               
               veFillTknU32(&(ie2->val.e_UTRAN_Cell.cellTyp.cell_Size),
                     (x2ConCb->ueCb->ueHistory.member + idx)->val.e_UTRAN_Cell.
                                                             cellTyp.cell_Size.val);
               veFillTknU32(&(ie2->val.e_UTRAN_Cell.time_UE_StayedInCell),
                     (x2ConCb->ueCb->ueHistory.member + idx)->val.e_UTRAN_Cell.
                                                          time_UE_StayedInCell.val);
               break;
            }
            case LASTVISITEDCELL_ITEM_GERAN_CELL:
            {
               /* Currently not handled */
               break;
            }
            case LASTVISITEDCELL_ITEM_UTRAN_CELL:
            {
               /* Currently not handled */
               break;
            }
            default:
               break;
        } /* end of switch statement */

   }/* end of For loop */
#else
   /* Fill the UE History Information */
   ie = &hovrRqstPdu->pdu.val.initiatingMsg.value.u.cztHovrRqst.protocolIEs.
                                                                member[compCnt];
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Cztid_UE_HistoryInform);
   veFillTknU32(&(ie->criticality), CztCriticalityignoreEnum);
   veFillTknU16(&(ie->value.u.cztUE_HistoryInform.noComp),
                                               1);
   if ((cmGetMem(hovrRqstPdu,
      (1 * sizeof(CztLastVisitedCell_Item)), 
                         (Ptr *)&ie->value.u.cztUE_HistoryInform.member)) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillHovrRqst: cmGetMem failed\n"));
#endif
      VE_FREE_EVNT(hovrRqstPdu);
      RETVALUE(RFAILED);
   }
   for(idx = 0; idx < 1; idx++)
   {
      ie2 = &ie->value.u.cztUE_HistoryInform.member[idx];
         /*choice = LASTVISITEDCELL_ITEM_UTRAN_CELL;*/
      veFillTknU8(&(ie2->choice), 
                                  0);

      switch (0)
      {
          case LASTVISITEDCELL_ITEM_E_UTRAN_CELL:
          { 
               veFillTknU8(&(ie2->val.e_UTRAN_Cell.pres),
                   1);
               veFillTknU8(&(ie2->val.e_UTRAN_Cell.global_Cell_ID.pres), 
                                                                   PRSNT_NODEF);
               /* Fill the PLMN ID of global cell id */
               veCzFillTknStrOSXL(&(ie2->val.e_UTRAN_Cell.global_Cell_ID.
                  pLMN_Identity), 3,
                  &val, &hovrRqstPdu);
               /* Fill the E-UTRAN Cell Identifier of global cell id*/
               veFillTknBStr32(&(ie2->val.e_UTRAN_Cell.global_Cell_ID.
                  eUTRANcellIdentifier), 28, 10);
               veFillTknU8(&(ie2->val.e_UTRAN_Cell.cellTyp.pres),1);
               veFillTknU32(&(ie2->val.e_UTRAN_Cell.cellTyp.cell_Size),
                     2);
               veFillTknU32(&(ie2->val.e_UTRAN_Cell.time_UE_StayedInCell),
                        100);
               break;
            }
            case LASTVISITEDCELL_ITEM_GERAN_CELL:
            {
               /* Currently not handled */
               break;
            }
            case LASTVISITEDCELL_ITEM_UTRAN_CELL:
            {
               /* Currently not handled */
               break;
            }
            default:
               break;
        } /* end of switch statement */

   }/* end of For loop */
#endif

   *pdu = hovrRqstPdu;

   RETVALUE(ROK);
}/* veUtlCztFillHovrRqst */

/*
*
*       Fun:   veUtlCztFillHovrRqstAck 
*
*       Desc:  Call handler to fill Handover Request Acknowledge message
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_x2_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veUtlCztFillHovrRqstAck
(
CztEvnt        **pdu,
VeX2ConCb      *x2ConCb,
NhuEncCfmSdus  *nhuEncCfmSdus,
U8             nmbBearer,
VeUeRAB        *ueRab
)
#else
PUBLIC S16 veUtlCztFillHovrRqstAck(pdu, x2ConCb, nhuEncCfmSdus, nmbBearer, ueRab)
CztEvnt        **pdu;
VeX2ConCb      *x2ConCb;
NhuEncCfmSdus  *nhuEncCfmSdus;
U8             nmbBearer;
VeUeRAB        *ueRab;
#endif
{
 
   CztEvnt *hovrRqstAckgPdu = NULLP;
   S16        numComp = 0;
   S16        idx;
   S16        ret = 0;
   U16        compcnt = 0;
   CztProtIE_Field_HovrRqstAckg_IEs *ie = NULLP;
   CztProtIE_Single_Cont_E_RABs_Admtd_ItemIEs *ie1 = NULLP;
   MsgLen cnt = 0;
   MsgLen len = 0;
   U8  offSet;


   TRC2(veUtlCztFillHovrRqstAck);

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(CztEvnt), VE_CZ_MEM_SDU_SIZE, &veCb.mem, (Ptr *)&hovrRqstAckgPdu);
   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillHovrRqstAck: cmAllocEvnt failed\n"));
#endif
      RETVALUE(RFAILED);
   }

   cmMemset((U8 *)&(hovrRqstAckgPdu->pdu), 0, sizeof(CztX2AP_PDU));

   veFillTknU8(&(hovrRqstAckgPdu->pdu.choice), X2AP_PDU_SUCCESSFULOUTCOME);
   veFillTknU8(&(hovrRqstAckgPdu->pdu.val.successfulOutcome.pres), PRSNT_NODEF);
   veFillTknU32(&(hovrRqstAckgPdu->pdu.val.successfulOutcome.procedureCode), Cztid_handoverPrep);
   veFillTknU32(&(hovrRqstAckgPdu->pdu.val.successfulOutcome.criticality), CztCriticalityrejectEnum);

   {
      numComp = 4;
      veFillTknU8(&(hovrRqstAckgPdu->pdu.val.successfulOutcome.value.u.cztHovrRqstAckg.pres), PRSNT_NODEF);
      veFillTknU16(&(hovrRqstAckgPdu->pdu.val.successfulOutcome.value.u.cztHovrRqstAckg.protocolIEs.noComp), numComp);

      if ((cmGetMem(hovrRqstAckgPdu, (numComp * sizeof(CztProtIE_Field_HovrRqstAckg_IEs)), 
             (Ptr *)&hovrRqstAckgPdu->pdu.val.successfulOutcome.value.u.cztHovrRqstAckg.protocolIEs.member)) !=ROK)
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillHovrRqstAck: cmGetMem failed\n"));
#endif
         VE_FREE_EVNT(hovrRqstAckgPdu);
         RETVALUE(RFAILED);
      }

      {
         /* Fill the Old ENB UE X2AP ID */
         ie = &hovrRqstAckgPdu->pdu.val.successfulOutcome.value.u.cztHovrRqstAckg.protocolIEs.member[compcnt++];

         veFillTknU8(&(ie->pres), PRSNT_NODEF);
         veFillTknU32(&(ie->id), Cztid_Old_eNB_UE_X2AP_ID);
         veFillTknU32(&(ie->criticality), CztCriticalityignoreEnum);
         veFillTknU32(&(ie->value.u.cztUE_X2AP_ID_Old), x2ConCb->oldEnbUeX2apId);

         /* Fill the New ENB UE X2AP ID */
         ie = &hovrRqstAckgPdu->pdu.val.successfulOutcome.value.u.cztHovrRqstAckg.protocolIEs.member[compcnt++];

         veFillTknU8(&(ie->pres), PRSNT_NODEF);
         veFillTknU32(&(ie->id), Cztid_New_eNB_UE_X2AP_ID);
         veFillTknU32(&(ie->criticality), CztCriticalityignoreEnum);
         veFillTknU32(&(ie->value.u.cztUE_X2AP_ID_New), x2ConCb->newEnbUeX2apId);

         /* Fill the E-RABs Admitted list */
         ie = &hovrRqstAckgPdu->pdu.val.successfulOutcome.value.u.cztHovrRqstAckg.protocolIEs.member[compcnt++];
         veFillTknU8(&(ie->pres), PRSNT_NODEF);
         veFillTknU32(&(ie->id), Cztid_E_RABs_Admtd_Lst);
         veFillTknU32(&(ie->criticality), CztCriticalityignoreEnum);

         /*numComp = 1;*/
         veFillTknU16(&(ie->value.u.cztE_RABs_Admtd_Lst.noComp),nmbBearer);

         if ((cmGetMem(hovrRqstAckgPdu, 
                     (nmbBearer * sizeof(CztProtIE_Single_Cont_E_RABs_Admtd_ItemIEs)), 
                     (Ptr *)&ie->value.u.cztE_RABs_Admtd_Lst.member)) !=ROK)
         {
#ifdef DEBUGP
            VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillHovrRqstAck: cmGetMem failed\n"));
#endif
            VE_FREE_EVNT(hovrRqstAckgPdu);
            RETVALUE(RFAILED);
         }
         for(idx = 0; idx < nmbBearer; idx++)
         {
            ie1 = &ie->value.u.cztE_RABs_Admtd_Lst.member[idx];
            veFillTknU8(&(ie1->pres),PRSNT_NODEF);
            veFillTknU32(&(ie1->id), Cztid_E_RABs_Admtd_Item);
            veFillTknU32(&(ie1->criticality),CztCriticalityignoreEnum);
            veFillTknU8(&(ie1->value.u.cztE_RABs_Admtd_Item.pres),
                                                                   PRSNT_NODEF);

            veFillTknU32(&(ie1->value.u.cztE_RABs_Admtd_Item.e_RAB_ID),
                                                   (ueRab + idx)->eRABId);
            if ((VE_UL == (ueRab + idx)->rbDir) || (VE_DL_UL == (ueRab + idx)->rbDir)) 
            {
               veFillTknU8(&(ie1->value.u.cztE_RABs_Admtd_Item.
                        uL_GTP_TunnelEndpoint.pres), PRSNT_NODEF);
               if (CM_TPTADDR_IPV4 == veCb.datAppAddr.type)
               {
                  /* length in bits */
                  len = 32;
                  ie1->value.u.cztE_RABs_Admtd_Item.uL_GTP_TunnelEndpoint.transportLyrAddr.len = 
                     len;
                  veCzFillTknStrBSXL(&(ie1->value.u.cztE_RABs_Admtd_Item.
                           uL_GTP_TunnelEndpoint.transportLyrAddr),len,
                        (Data *)&veCb.datAppAddr.u.ipv4TptAddr.address, 
                        (S1apPdu**)&hovrRqstAckgPdu); 

               }
               else if(CM_TPTADDR_IPV6 == veCb.datAppAddr.type)
               {
                  /* length in bits */
                  len = 128;
                  ie1->value.u.cztE_RABs_Admtd_Item.uL_GTP_TunnelEndpoint.transportLyrAddr.len = 
                     len;
                  veCzFillTknStrBSXL(&(ie1->value.u.cztE_RABs_Admtd_Item.
                           uL_GTP_TunnelEndpoint.transportLyrAddr),len,
                        (Data *)&veCb.datAppAddr.u.ipv6TptAddr.ipv6NetAddr, 
                        (S1apPdu**)&hovrRqstAckgPdu); 
               }
               len = 4;
               ie1->value.u.cztE_RABs_Admtd_Item.uL_GTP_TunnelEndpoint.gTP_TEID.pres = PRSNT_NODEF;
               ie1->value.u.cztE_RABs_Admtd_Item.uL_GTP_TunnelEndpoint.gTP_TEID.len = len;
               VE_GET_MEM(hovrRqstAckgPdu, len,
                     &(ie1->value.u.cztE_RABs_Admtd_Item.
                        uL_GTP_TunnelEndpoint.gTP_TEID.val));

               for (cnt = 0; cnt < len; cnt++)
               {
                  /* compute the offset to right shift the val in U32
                   * shift by 24, 16 & 8 bytes */
                  offSet = (U8)(len - (cnt + 1)) * 8;
                  ie1->value.u.cztE_RABs_Admtd_Item.uL_GTP_TunnelEndpoint.gTP_TEID.val[cnt] =
                     (U8)((ueRab + idx)->datFwdUlLocEgtpTunId >> offSet);
               }

            }
            
            if ((VE_DL == (ueRab + idx)->rbDir) || (VE_DL_UL == (ueRab + idx)->rbDir)) 
            {
               veFillTknU8(&(ie1->value.u.cztE_RABs_Admtd_Item.
                        dL_GTP_TunnelEndpoint.pres),PRSNT_NODEF);

               if (CM_TPTADDR_IPV4 == veCb.datAppAddr.type)
               {
                  /* length in bits */
                  len = 32;
                  ie1->value.u.cztE_RABs_Admtd_Item.dL_GTP_TunnelEndpoint.transportLyrAddr.len = 
                     len;
                  veCzFillTknStrBSXL(&(ie1->value.u.cztE_RABs_Admtd_Item.
                           dL_GTP_TunnelEndpoint.transportLyrAddr),len,
                        (Data *)&veCb.datAppAddr.u.ipv4TptAddr.address, 
                        (S1apPdu**)&hovrRqstAckgPdu); 
               }
               else 
               {
                  /* length in bits */
                  len = 128;
                  ie1->value.u.cztE_RABs_Admtd_Item.dL_GTP_TunnelEndpoint.transportLyrAddr.len = 
                     len;
                  veCzFillTknStrBSXL(&(ie1->value.u.cztE_RABs_Admtd_Item.
                           dL_GTP_TunnelEndpoint.transportLyrAddr),len,
                        (Data *)&veCb.datAppAddr.u.ipv6TptAddr.ipv6NetAddr, 
                        (S1apPdu**)&hovrRqstAckgPdu); 
               }
               len = 4;
               ie1->value.u.cztE_RABs_Admtd_Item.dL_GTP_TunnelEndpoint.gTP_TEID.pres = PRSNT_NODEF;
               ie1->value.u.cztE_RABs_Admtd_Item.dL_GTP_TunnelEndpoint.gTP_TEID.len = len;
               VE_GET_MEM(hovrRqstAckgPdu, len,
                     &(ie1->value.u.cztE_RABs_Admtd_Item.
                        dL_GTP_TunnelEndpoint.gTP_TEID.val));

               for (cnt = 0; cnt < len; cnt++)
               {
                  /* compute the offset to right shift the val in U32
                   * shift by 24, 16 & 8 bytes */
                  offSet = (U8)(len - (cnt + 1)) * 8;
                  ie1->value.u.cztE_RABs_Admtd_Item.dL_GTP_TunnelEndpoint.gTP_TEID.val[cnt] =
                     (U8)((ueRab + idx)->datFwdDlLocEgtpTunId >> offSet);
               }

            }
            ie1->value.u.cztE_RABs_Admtd_Item.iE_Extns.noComp.pres = NOTPRSNT;
         }

         /* Fill target to Source Transparant container */
         ie = &hovrRqstAckgPdu->pdu.val.successfulOutcome.value.u.
                                    cztHovrRqstAckg.protocolIEs.member[compcnt];

         veFillTknU8(&(ie->pres), PRSNT_NODEF);
         veFillTknU32(&(ie->id), Cztid_TgeteNBtoSrc_eNBTprntCont);
         veFillTknU32(&(ie->criticality), CztCriticalityignoreEnum);
         /* Find the length of the Encoded TrgtToSrcTC */
         ret = SFndLenMsg(nhuEncCfmSdus->sdu.mBuf, &len);
         if((len == 0) || (ret != ROK))
         {
            VE_DBG_ERROR((VE_PRNT_BUF,
                     "veUtlCztFillHovrRqstAck: In correct buffer len %d",len));
            RETVALUE(RFAILED);
         }
         ie->value.u.cztTgeteNBtoSrc_eNBTprntCont.pres = 1;
         ie->value.u.cztTgeteNBtoSrc_eNBTprntCont.len = len;
         cnt = 0;
         /* Allocate memory for the flat buffer */
         VE_ALLOC(&(ie->value.u.cztTgeteNBtoSrc_eNBTprntCont.val), len);
         SCpyMsgFix(nhuEncCfmSdus->sdu.mBuf,0,len,
                    (Data *)ie->value.u.cztTgeteNBtoSrc_eNBTprntCont.val,
                     &cnt);
      }
   }
   *pdu = hovrRqstAckgPdu;

   RETVALUE(ROK);
}/* veUtlCztFillHovrRqstAck */


/*
*
*       Fun:   veUtlCztFillHovrPrepFail 
*
*       Desc:  Call handler to fill Handover Failure message
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_x2_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veUtlCztFillHovrPrepFail
(
CztEvnt    **pdu,
VeX2ConCb *x2ConCb
)
#else
PUBLIC S16 veUtlCztFillHovrPrepFail(pdu,x2ConCb)
CztEvnt    **pdu;
VeX2ConCb *x2ConCb;
#endif
{
   
   CztEvnt *hovrPrepFailPdu = NULLP;
   S16        numComp = 0;
   S16        ret = 0;
   U16        compcnt = 0;
   CztProtIE_Field_HovrPrepFail_IEs *ie = NULLP;

   TRC2(veUtlCztFillHovrPrepFail);

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(CztEvnt), VE_CZ_MEM_SDU_SIZE, &veCb.mem, (Ptr *)&hovrPrepFailPdu);
   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillHovrPrepFail: cmAllocEvnt failed\n"));
#endif
      RETVALUE(RFAILED);
   }

   cmMemset((U8 *)&(hovrPrepFailPdu->pdu), 0, sizeof(CztX2AP_PDU));

   veFillTknU8(&(hovrPrepFailPdu->pdu.choice), X2AP_PDU_UNSUCCESSFULOUTCOME);
   veFillTknU8(&(hovrPrepFailPdu->pdu.val.unsuccessfulOutcome.pres), PRSNT_NODEF);
   veFillTknU32(&(hovrPrepFailPdu->pdu.val.unsuccessfulOutcome.procedureCode), 
         Cztid_handoverPrep);
   veFillTknU32(&(hovrPrepFailPdu->pdu.val.unsuccessfulOutcome.criticality), 
         CztCriticalityrejectEnum);

   /* Fill the required IEs*/
   numComp = 2;
   veFillTknU8(
       &(hovrPrepFailPdu->pdu.val.unsuccessfulOutcome.value.u.cztHovrPrepFail.pres), 
       PRSNT_NODEF);
      
   veFillTknU16(
      &(hovrPrepFailPdu->pdu.val.unsuccessfulOutcome.value.u.cztHovrPrepFail.protocolIEs.noComp), numComp);

   if ((cmGetMem(hovrPrepFailPdu, (numComp * sizeof(CztProtIE_Field_HovrPrepFail_IEs)), 
          (Ptr *)&hovrPrepFailPdu->pdu.val.unsuccessfulOutcome.value.u.cztHovrPrepFail.protocolIEs.member)) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillHovrPrepFail: cmAllocEvnt failed \n"));
#endif
     VE_FREE_EVNT(hovrPrepFailPdu);
     RETVALUE(RFAILED);
   }

   /* Fill Old EnB UE X2AP ID */
   ie = &hovrPrepFailPdu->pdu.val.unsuccessfulOutcome.value.u.cztHovrPrepFail.protocolIEs.member[compcnt++];

   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Cztid_Old_eNB_UE_X2AP_ID);
   veFillTknU32(&(ie->criticality), CztCriticalityignoreEnum);
   veFillTknU32(&(ie->value.u.cztUE_X2AP_ID), x2ConCb->oldEnbUeX2apId);

   ie = &hovrPrepFailPdu->pdu.val.unsuccessfulOutcome.value.u.cztHovrPrepFail.protocolIEs.member[compcnt];

   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Cztid_Cause);
   veFillTknU32(&(ie->criticality), CztCriticalityignoreEnum);
   veFillTknU8(&(ie->value.u.cztCause.choice),x2ConCb->cause.choice.val);
   if(x2ConCb->cause.choice.pres)
   {
    switch (x2ConCb->cause.choice.val)
     {
        case CAUSE_RADIONW:
        {
               veFillTknU32(&(ie->value.u.cztCause.val.radioNw), x2ConCb->cause.val.radioNw.val);
               break;
        }
        case CAUSE_TRANSPORT:
        {
               veFillTknU32(&(ie->value.u.cztCause.val.transport), x2ConCb->cause.val.transport.val);
               break;
        }
        case CAUSE_PROTOCOL:
        {
               veFillTknU32(&(ie->value.u.cztCause.val.protocol), x2ConCb->cause.val.protocol.val);
               break;
        }
        case CAUSE_MISC:
        {
               veFillTknU32(&(ie->value.u.cztCause.val.misc), x2ConCb->cause.val.misc.val);
               break;
        }
        default:
        {
               veFillTknU32(&(ie->value.u.cztCause.val.radioNw), x2ConCb->cause.val.radioNw.val);
               break;
        }
     }
   }

   *pdu = hovrPrepFailPdu;

   RETVALUE(ROK);
}/* veUtlCztFillHovrPrepFail */

/*
*
*       Fun:   veUtlCztFillHovrCancel 
*
*       Desc:  Call handler to fill Handover Cancel message
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_x2_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veUtlCztFillHovrCancel
(
CztEvnt    **pdu,
VeX2ConCb  *x2ConCb,
Bool isNeweNBUeX2apIdPres
)
#else
PUBLIC S16 veUtlCztFillHovrCancel(pdu,x2ConCb,isNeweNBUeX2apIdPres)
CztEvnt    **pdu;
VeX2ConCb *x2ConCb;
Bool isNeweNBUeX2apIdPres;
#endif
{
   CztEvnt *hovrCancelPdu = NULLP;
   S16        numComp = 0;
   S16        ret = 0;
   CztProtIE_Field_HovrCancel_IEs *ie = NULLP;
   U8         compCnt = 0;

   TRC2(veUtlCztFillHovrCancel);
   
  /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(CztEvnt), VE_CZ_MEM_SDU_SIZE, &veCb.mem, (Ptr *)&hovrCancelPdu);
   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillHovrCancel: cmAllocEvnt failed\n"));
#endif
      RETVALUE(RFAILED);
   }

   cmMemset((U8 *)&(hovrCancelPdu->pdu), 0, sizeof(CztX2AP_PDU));

   veFillTknU8(&(hovrCancelPdu->pdu.choice), X2AP_PDU_INITIATINGMSG);
   veFillTknU8(&(hovrCancelPdu->pdu.val.initiatingMsg.pres), PRSNT_NODEF);
   veFillTknU32(&(hovrCancelPdu->pdu.val.initiatingMsg.procedureCode), 
         Cztid_handoverCancel);

   veFillTknU32(&(hovrCancelPdu->pdu.val.initiatingMsg.criticality), 
         CztCriticalityignoreEnum);

 
   veFillTknU8(&(hovrCancelPdu->pdu.val.initiatingMsg.value.u.cztHovrCancel.pres), 
            PRSNT_NODEF);
   numComp = (TRUE == isNeweNBUeX2apIdPres) ? 3: 2;
   veFillTknU16(
        &(hovrCancelPdu->pdu.val.initiatingMsg.value.u.cztHovrCancel.protocolIEs.noComp), 
        numComp);

   if ((cmGetMem(hovrCancelPdu, (numComp * sizeof(CztProtIE_Field_HovrCancel_IEs)), 
         (Ptr *)&hovrCancelPdu->pdu.val.initiatingMsg.value.u.cztHovrCancel.protocolIEs.member)) !=ROK)
   {
#ifdef DEBUGP
       VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillHovrCancel: cmAllocEvnt failed\n"));
#endif
       VE_FREE_EVNT(hovrCancelPdu);
       RETVALUE(RFAILED);
   }

   /* Fill the Old EnB UE X2AP Id */
   ie = &hovrCancelPdu->pdu.val.initiatingMsg.value.u.cztHovrCancel.protocolIEs.member[compCnt++];

   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Cztid_Old_eNB_UE_X2AP_ID);
   veFillTknU32(&(ie->criticality), CztCriticalityrejectEnum);
   veFillTknU32(&(ie->value.u.cztUE_X2AP_ID_Old),x2ConCb->oldEnbUeX2apId);
   if (TRUE == isNeweNBUeX2apIdPres)
   {
       /* Fill the New  EnB UE X2AP Id */
      ie = &hovrCancelPdu->pdu.val.initiatingMsg.value.u.cztHovrCancel.protocolIEs.member[compCnt++];

      veFillTknU8(&(ie->pres), PRSNT_NODEF);
      veFillTknU32(&(ie->id), Cztid_New_eNB_UE_X2AP_ID);
      veFillTknU32(&(ie->criticality), CztCriticalityignoreEnum);     
      veFillTknU32(&(ie->value.u.cztUE_X2AP_ID_New), x2ConCb->newEnbUeX2apId);
   }
   /* Fill the Cause Value */
    ie = &hovrCancelPdu->pdu.val.initiatingMsg.value.u.cztHovrCancel.protocolIEs.member[compCnt];

    veFillTknU8(&(ie->pres), PRSNT_NODEF);
    veFillTknU32(&(ie->id), Cztid_Cause);
    veFillTknU32(&(ie->criticality), CztCriticalityignoreEnum);
    if(x2ConCb->cause.choice.pres)
    {
       veFillTknU8(&(ie->value.u.cztCause.choice), x2ConCb->cause.choice.val);
       switch (x2ConCb->cause.choice.val)
       {
          case CAUSE_RADIONW:
          {
             veFillTknU32(&(ie->value.u.cztCause.val.radioNw), x2ConCb->cause.val.radioNw.val);
             break;
          }
          case CAUSE_TRANSPORT:
          {
              veFillTknU32(&(ie->value.u.cztCause.val.transport),x2ConCb->cause.val.transport.val);
              break;
          }
          case CAUSE_PROTOCOL:
          {
             veFillTknU32(&(ie->value.u.cztCause.val.protocol), x2ConCb->cause.val.protocol.val);
             break;
          }
          case CAUSE_MISC:
          {
             veFillTknU32(&(ie->value.u.cztCause.val.misc), x2ConCb->cause.val.misc.val);
             break;
          }
          default:
          {
             veFillTknU32(&(ie->value.u.cztCause.val.radioNw), x2ConCb->cause.val.radioNw.val);
             break;
          }      
       }
   }

   *pdu = hovrCancelPdu;

   RETVALUE(ROK);
}/* veUtlCztFillHovrCancel */

/*
*
*       Fun:   veUtlCztFillSnStatusTrfr 
*
*       Desc:  Call handler to fill SN Status Transfer message
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_x2_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veUtlCztFillSnStatusTrfr
(
CztEvnt             **pdu,
VeX2ConCb           *x2ConCb,
U8                  nmbBearer,
NhuPdcpSduStaInfo   *nhuPdcpSduStaInfo
)
#else
PUBLIC S16 veUtlCztFillSnStatusTrfr(pdu, x2ConCb, nmbBearer, nhuPdcpSduStaInfo)
CztEvnt             **pdu;
VeX2ConCb           *x2ConCb;
U8                  nmbBearer;
NhuPdcpSduStaInfo   *nhuPdcpSduStaInfo;
#endif
{
   CztEvnt *snStatusTfrPdu = NULLP;
   S16        numComp = 0;
   S16        ret = 0;
   U8         compCnt = 0;   
   CztProtIE_Field_SNStatusTfr_IEs *ie = NULLP;
   CztProtIE_Field_E_RABs_SubjToStatusTfr_ItemIEs *ie1 = NULLP;
   U8 rbCnt = 0;
   U8 idx = 0;
   U8 idx1 = 0;
   VeUeRAB *rbInfo = NULLP;
   U32        pdcpHfn = 0;
   U16        pdcpSn = 0;
   U8 snStsRbCnt = 0;
   Bool sndSNSts = FALSE;

   TRC2(veUtlCztFillSnStatusTrfr);

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(CztEvnt), VE_CZ_MEM_SDU_SIZE, &veCb.mem, (Ptr *)&snStatusTfrPdu);
   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillSnStatusTrfr: cmAllocEvnt failed\n"));
#endif
      RETVALUE(RFAILED);
   }

   cmMemset((U8 *)&(snStatusTfrPdu->pdu), 0, sizeof(CztX2AP_PDU));

   veFillTknU8(&(snStatusTfrPdu->pdu.choice), X2AP_PDU_INITIATINGMSG);
   veFillTknU8(&(snStatusTfrPdu->pdu.val.initiatingMsg.pres), PRSNT_NODEF);
   veFillTknU32(&(snStatusTfrPdu->pdu.val.initiatingMsg.procedureCode), 
         Cztid_snStatusTfr);
   veFillTknU32(&(snStatusTfrPdu->pdu.val.initiatingMsg.criticality), 
         CztCriticalityignoreEnum);

   /* Fill the required IEs */
   
   numComp = 3;
   veFillTknU8(&(snStatusTfrPdu->pdu.val.initiatingMsg.value.u.cztSNStatusTfr.pres), 
       PRSNT_NODEF);
   veFillTknU16(
       &(snStatusTfrPdu->pdu.val.initiatingMsg.value.u.cztSNStatusTfr.protocolIEs.noComp),
      numComp);

   if ((cmGetMem(snStatusTfrPdu, (numComp * sizeof(CztProtIE_Field_SNStatusTfr_IEs)), 
             (Ptr *)&snStatusTfrPdu->pdu.val.initiatingMsg.value.u.cztSNStatusTfr.protocolIEs.member)) !=ROK)
   {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillSnStatusTrfr: cmAllocEvnt failed\n"));
#endif
         VE_FREE_EVNT(snStatusTfrPdu);
         RETVALUE(RFAILED);
   }

   /* Fill the Old EnB UE X2AP ID */
   ie = &snStatusTfrPdu->pdu.val.initiatingMsg.value.u.cztSNStatusTfr.protocolIEs.member[compCnt++];

   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Cztid_Old_eNB_UE_X2AP_ID);
   veFillTknU32(&(ie->criticality), CztCriticalityrejectEnum);
   veFillTknU32(&(ie->value.u.cztUE_X2AP_ID_Old), x2ConCb->oldEnbUeX2apId);

   /* Fill the New EnB UE X2AP ID */
   ie = &snStatusTfrPdu->pdu.val.initiatingMsg.value.u.cztSNStatusTfr.protocolIEs.member[compCnt++];

   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Cztid_New_eNB_UE_X2AP_ID);
   veFillTknU32(&(ie->criticality), CztCriticalityrejectEnum);
   veFillTknU32(&(ie->value.u.cztUE_X2AP_ID_New), x2ConCb->newEnbUeX2apId);

   /* Fill the E-RABs list */
   ie = &snStatusTfrPdu->pdu.val.initiatingMsg.value.u.cztSNStatusTfr.protocolIEs.member[compCnt];

   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Cztid_E_RABs_SubjToStatusTfr_Lst);
   veFillTknU32(&(ie->criticality), CztCriticalityignoreEnum);

   rbCnt = x2ConCb->ueCb->rbInfo.numOfRbCfgd;
   rbInfo = x2ConCb->ueCb->rbInfo.rab;
   for(idx1 = 0; idx1 < nmbBearer; idx1++)
   {
      for(idx1 = 0; idx1 < rbCnt; idx1++)
      {
         if(nhuPdcpSduStaInfo[idx].drbId == rbInfo[idx1].rbId)
         {
            if(NHU_RLC_MODE_AM == rbInfo[idx1].rbMode)
            {
               snStsRbCnt++;
               break;
            }
         }
         else
         {
#ifdef DEBUGP
            VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillSnStatusTrfr: SduStsCfm Reveived for invalid RBID: %d.\n", 
             nhuPdcpSduStaInfo[idx].drbId));
#endif
            RETVALUE(RFAILED);
         }
      }/* End of for() */
   }
   numComp = snStsRbCnt;
   veFillTknU16(
               &(ie->value.u.cztE_RABs_SubjToStatusTfr_Lst.noComp),
               numComp);

   if ((cmGetMem(snStatusTfrPdu, 
                     (numComp * sizeof(CztProtIE_Field_E_RABs_SubjToStatusTfr_ItemIEs)), 
                (Ptr *)&ie->value.u.cztE_RABs_SubjToStatusTfr_Lst.member)) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillSnStatusTrfr: cmAllocEvnt failed\n"));
#endif
      VE_FREE_EVNT(snStatusTfrPdu);
      RETVALUE(RFAILED);
   }

   snStsRbCnt = 0;
   for(idx1 = 0; idx1 < nmbBearer; idx1++)
   {
      for(idx1 = 0; idx1 < rbCnt; idx1++)
      {
         if((nhuPdcpSduStaInfo[idx].drbId == rbInfo[idx1].rbId) && 
            (NHU_RLC_MODE_AM == rbInfo[idx1].rbMode))
         {
            sndSNSts = TRUE;
            break;
         }
      }
      if (TRUE != sndSNSts)
      {
         continue;
      }
      else
      {
         /* reset the flag */
         sndSNSts =  FALSE;
      }

      ie1 = &ie->value.u.cztE_RABs_SubjToStatusTfr_Lst.member[snStsRbCnt++];

      veFillTknU8(&(ie1->pres), PRSNT_NODEF);
      veFillTknU32(&(ie1->id), Cztid_E_RABs_SubjToStatusTfr_Item);
      veFillTknU32(&(ie1->criticality), CztCriticalityignoreEnum);
      veFillTknU8(&(ie1->value.u.cztE_RABs_SubjToStatusTfr_Item.pres), PRSNT_NODEF);

      veFillTknU32(&(ie1->value.u.cztE_RABs_SubjToStatusTfr_Item.e_RAB_ID), \
                  rbInfo[idx].eRABId);

      /* Fill the UL Count Value */
      veFillTknU8(&(ie1->value.u.cztE_RABs_SubjToStatusTfr_Item.uL_COUNTvalue.pres),PRSNT_NODEF);
      pdcpSn = (nhuPdcpSduStaInfo[idx1].ulCountVal & (0x0fff));
      veFillTknU32(
            &(ie1->value.u.cztE_RABs_SubjToStatusTfr_Item.uL_COUNTvalue.pDCP_SN), pdcpSn);
      pdcpHfn = ((nhuPdcpSduStaInfo[idx1].ulCountVal >> 12) & (0xfffff));
      veFillTknU32(
            &(ie1->value.u.cztE_RABs_SubjToStatusTfr_Item.uL_COUNTvalue.hFN), pdcpHfn);
      ie1->value.u.cztE_RABs_SubjToStatusTfr_Item.uL_COUNTvalue.iE_Extns.noComp.pres =
         NOTPRSNT;

      /* Fill the DL Count Value */
      veFillTknU8(&(ie1->value.u.cztE_RABs_SubjToStatusTfr_Item.dL_COUNTvalue.pres),
            PRSNT_NODEF);
      pdcpSn = (nhuPdcpSduStaInfo[idx1].dlCountVal & (0x0fff));
      veFillTknU32(
            &(ie1->value.u.cztE_RABs_SubjToStatusTfr_Item.dL_COUNTvalue.pDCP_SN), pdcpSn);
      pdcpHfn = ((nhuPdcpSduStaInfo[idx1].dlCountVal >> 12) & (0xfffff)); 
      veFillTknU32(
            &(ie1->value.u.cztE_RABs_SubjToStatusTfr_Item.dL_COUNTvalue.hFN), pdcpHfn);
      /* selva MSPD FIX*/
      /* Filling UL PDCP SDU's Bit Map in case of AM Mode PDUs*/
      if(0 != nhuPdcpSduStaInfo[idx1].rcvStaOfUlPdcpSdus.numBits)
      {
         veFillTknStrBSXL(&(ie1->value.u.cztE_RABs_SubjToStatusTfr_Item.receiveStatusofULPDCPSDUs),\
               nhuPdcpSduStaInfo[idx1].rcvStaOfUlPdcpSdus.numBits,\
               nhuPdcpSduStaInfo[idx1].rcvStaOfUlPdcpSdus.ulBitMap,\
               (S1apPdu **)&snStatusTfrPdu);
      }
      ie1->value.u.cztE_RABs_SubjToStatusTfr_Item.dL_COUNTvalue.iE_Extns.noComp.pres =
         NOTPRSNT;
   }
    
   *pdu = snStatusTfrPdu;

   RETVALUE(ROK);
}/* veUtlCztFillSnStatusTrfr */


/*
*
*       Fun:   veFillErabFrmX2HoReq
*
*       Desc:  Call handler to fill RAB info in UECB from X2 HO Request message
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_x2_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillErabFrmX2HoReq
(
VeUeRABInfo *rbInfo,
CztE_RABs_ToBeSetup_Lst *erabLst,
VeNghCellCb *nghCellCb,
VeMmeCb     *mmeCb
)
#else
PUBLIC S16 veFillErabFrmX2HoReq(rbInfo, erabLst, nghCellCb, mmeCb)
VeUeRABInfo *rbInfo;
CztE_RABs_ToBeSetup_Lst *erabLst;
VeNghCellCb *nghCellCb;
VeMmeCb     *mmeCb;
#endif
{
   U8   idx = 0;
   U8   idx1 = 0;
   U16  ipAddrIdx = 0;

   CztE_RABs_ToBeSetup_Item *item = NULLP;

   TRC2(veFillErabFrmX2HoReq);

   if(rbInfo == NULLP || erabLst == NULLP || nghCellCb == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillErabFrmX2HoReq : Invalid Arguments.\n"));
#endif
      RETVALUE(RFAILED);
   }

   for(idx = 0; idx < erabLst->noComp.val; idx++)
   {
      item = &erabLst->member[idx].value.u.cztE_RABs_ToBeSetup_Item;
      if (PRSNT_NODEF == item->pres.pres)
      {
         for(idx1 = 0; idx1 < idx; idx1++)
         {
            if(rbInfo->rab[idx1].eRABId == item->e_RAB_ID.val)
            {
#ifdef DEBUGP
               VE_DBG_ERROR((VE_PRNT_BUF,"veFillErabFrmX2HoReq : \
                        Multiple ERAB Id [id = %ld].\n",item->e_RAB_ID.val));
#endif
               RETVALUE(RFAILED);
            }
         } /* end of for(idx1...( */

         rbInfo->rab[idx].eRABId = item->e_RAB_ID.val;

         rbInfo->rab[idx].rbType = CM_LTE_DRB;
         rbInfo->rab[idx].rbId = VE_GET_RB_ID_FRM_ERAB(rbInfo->rab[idx].eRABId);
         rbInfo->rab[idx].pdcpId = rbInfo->rab[idx].rbId;
         rbInfo->rab[idx].qci = item->e_RAB_Lvl_QoS_Params.qCI.val;
       
         VE_GET_U32_FRM_OSXL(rbInfo->rab[idx].mbr.dl, \
                      item->e_RAB_Lvl_QoS_Params.gbrQosInform.e_RAB_MaxBitrateDL);
       
         VE_GET_U32_FRM_OSXL(rbInfo->rab[idx].mbr.ul, \
                      item->e_RAB_Lvl_QoS_Params.gbrQosInform.e_RAB_MaxBitrateUL);
       
         VE_GET_U32_FRM_OSXL(rbInfo->rab[idx].gbr.dl, \
                      item->e_RAB_Lvl_QoS_Params.gbrQosInform.e_RAB_GuaranteedBitrateDL);
       
         VE_GET_U32_FRM_OSXL(rbInfo->rab[idx].gbr.ul, \
                      item->e_RAB_Lvl_QoS_Params.gbrQosInform.e_RAB_GuaranteedBitrateUL);

         /* TODO : Determine the rbDir and rbMode */
#ifndef VE_UM_MODE
         rbInfo->rab[idx].rbDir = VE_DL_UL;
         rbInfo->rab[idx].rbMode = NHU_RLC_MODE_AM;
#else
#ifndef  VE_UM_MODE_BI_DIR
         rbInfo->rab[idx].rbDir = VE_DL;
#else
         rbInfo->rab[idx].rbDir = VE_DL_UL;
#endif
         rbInfo->rab[idx].rbMode = NHU_RLC_MODE_UM;
#endif

         VE_ALLOC(&(rbInfo->rab[idx].remEgtpTunId.val), (item->uL_GTPtunnelEndpoint.gTP_TEID.len));
     
         /* Copy Byte by Byte */
         cmMemcpy(rbInfo->rab[idx].remEgtpTunId.val, item->uL_GTPtunnelEndpoint.gTP_TEID.val, \
                  item->uL_GTPtunnelEndpoint.gTP_TEID.len);
     
         rbInfo->rab[idx].remEgtpTunId.len = item->uL_GTPtunnelEndpoint.gTP_TEID.len;

         
         /* retrieve Serv. GW Address later will be used for tunnel creation */
         switch(item->uL_GTPtunnelEndpoint.transportLyrAddr.len)
         {
            case 32:
            {
               rbInfo->rab[idx].sgwAddr.type = CM_TPTADDR_IPV4;
               rbInfo->rab[idx].sgwAddr.u.ipv4TptAddr.port = VE_DFLT_EGTP_PORT; 
               rbInfo->rab[idx].sgwAddr.u.ipv4TptAddr.address = 
                  *(U32*)item->uL_GTPtunnelEndpoint.transportLyrAddr.val;
               break;
            }
            case 128:
            {
               rbInfo->rab[idx].sgwAddr.type = CM_TPTADDR_IPV6;
               rbInfo->rab[idx].sgwAddr.u.ipv6TptAddr.port = VE_DFLT_EGTP_PORT; 
               for (ipAddrIdx = 0; ipAddrIdx < item->uL_GTPtunnelEndpoint.transportLyrAddr.len; ipAddrIdx++)
               {
                  rbInfo->rab[idx].sgwAddr.u.ipv6TptAddr.ipv6NetAddr[ipAddrIdx] = 
                     item->uL_GTPtunnelEndpoint.transportLyrAddr.val[ipAddrIdx];
               }
               break;
            }
            default:
            {
               /* would be in case if len = 160 bits which means support for both ipv4 and
                * ipv6 */
#ifdef DEBUGP
               VE_DBG_ERROR((VE_PRNT_BUF,"veFillErabFrmX2HoReq : \
                        Invalid IPaddress len %d\n",item->uL_GTPtunnelEndpoint.transportLyrAddr.len));
#endif
               break;
             }
         } /* end of switch statement */
         /* Store local Tunnel Id */
         VE_GET_LOC_TUNNEL_ID((mmeCb)->locEgtpTunCntr);
         rbInfo->rab[idx].locEgtpTunId = (mmeCb)->locEgtpTunCntr;

         /* verfy if data forwarding is proposed for this ERAB */
         if(TRUE == item->dL_Fwding.pres)
         {
            /* setting data forwarding flag */
            if(CztDL_FwdingdL_forwardingProposedEnum == item->dL_Fwding.val)
               rbInfo->rab[idx].isDataFwdAvl = TRUE;
         }

         if(rbInfo->rab[idx].isDataFwdAvl == TRUE)
         {
            switch(rbInfo->rab[idx].rbDir)
            {
               case VE_DL:
               {
                  /* Store local DL Fwd Tunnel Id */
                  VE_GET_LOC_TUNNEL_ID((mmeCb)->locEgtpTunCntr);
                  rbInfo->rab[idx].datFwdDlLocEgtpTunId = (mmeCb)->locEgtpTunCntr;
                  /* Since there will not be any forwarding of data,
                   * from Tget and SGW, there will not be any remote tunnel 
                   * info for dat fwding, present in the HO REQ message.
                   * And EGTP needs both loc and rem address info to 
                   * create a tunnel.
                   * So we are filling a Src eNB Address */

                  VE_ALLOC(&(rbInfo->rab[idx].remDlDataFwdTeId.val), \
                        (rbInfo->rab[idx].remEgtpTunId.len));

                  /* Copy Byte by Byte */
                  cmMemcpy(rbInfo->rab[idx].remDlDataFwdTeId.val, \
                        rbInfo->rab[idx].remEgtpTunId.val, \
                        rbInfo->rab[idx].remEgtpTunId.len);

                  rbInfo->rab[idx].remDlDataFwdTeId.len = rbInfo->rab[idx].remEgtpTunId.len;

                  switch(nghCellCb->tEnbAddr.type)
                  {
                     case CM_TPTADDR_IPV4:
                        rbInfo->rab[idx].datFwdPeerAddr.type = CM_TPTADDR_IPV4;
                        rbInfo->rab[idx].datFwdPeerAddr.u.ipv4TptAddr.port = VE_DFLT_EGTP_PORT; 

                       cmMemcpy((U8 *)&(rbInfo->rab[idx].datFwdPeerAddr.u.ipv4TptAddr.address), \
                              (U8 *)&(nghCellCb->tEnbAddr.u.ipv4TptAddr.address),\
                              sizeof(rbInfo->rab[idx].datFwdPeerAddr.u.ipv4TptAddr.address));
                        break;

                     case CM_TPTADDR_IPV6:
                        rbInfo->rab[idx].datFwdPeerAddr.type = CM_TPTADDR_IPV6;
                        rbInfo->rab[idx].datFwdPeerAddr.u.ipv4TptAddr.port = VE_DFLT_EGTP_PORT; 

                        for (ipAddrIdx = 0; ipAddrIdx < 16; ipAddrIdx++)
                        {
                           rbInfo->rab[idx].datFwdPeerAddr.u.ipv6TptAddr.ipv6NetAddr[ipAddrIdx] = \
                                   rbInfo->rab[idx].sgwAddr.u.ipv6TptAddr.ipv6NetAddr[ipAddrIdx];
                        }
                        break;

                     default:
                        /* would be in case if len = 160 bits which means support for both ipv4 and
                         * ipv6 */
                        break;
                  }

                  break;
               }
               case VE_UL:
               {
                  /* Store local UL Fwd Tunnel Id */
                  VE_GET_LOC_TUNNEL_ID((mmeCb)->locEgtpTunCntr);
                  rbInfo->rab[idx].datFwdUlLocEgtpTunId = (mmeCb)->locEgtpTunCntr;
                  /* Since there will not be any forwarding of data,
                   * from Tget and SGW, there will not be any remote tunnel 
                   * info for dat fwding, present in the HO REQ message.
                   * And EGTP needs both loc and rem address info to 
                   * create a tunnel.
                   * So we are filling a Src eNB Address */

                  VE_ALLOC(&(rbInfo->rab[idx].remDlDataFwdTeId.val), \
                        (rbInfo->rab[idx].remEgtpTunId.len));

                  /* Copy Byte by Byte */
                  cmMemcpy(rbInfo->rab[idx].remDlDataFwdTeId.val, \
                        rbInfo->rab[idx].remEgtpTunId.val, \
                        rbInfo->rab[idx].remEgtpTunId.len);

                  rbInfo->rab[idx].remDlDataFwdTeId.len = rbInfo->rab[idx].remEgtpTunId.len;

                  switch(nghCellCb->tEnbAddr.type)
                  {
                     case CM_TPTADDR_IPV4:
                        rbInfo->rab[idx].datFwdPeerAddr.type = CM_TPTADDR_IPV4;
                        rbInfo->rab[idx].datFwdPeerAddr.u.ipv4TptAddr.port = VE_DFLT_EGTP_PORT; 

                       cmMemcpy((U8 *)&(rbInfo->rab[idx].datFwdPeerAddr.u.ipv4TptAddr.address), \
                              (U8 *)&(nghCellCb->tEnbAddr.u.ipv4TptAddr.address),\
                              sizeof(rbInfo->rab[idx].datFwdPeerAddr.u.ipv4TptAddr.address));
                        break;

                     case CM_TPTADDR_IPV6:
                        rbInfo->rab[idx].datFwdPeerAddr.type = CM_TPTADDR_IPV6;
                        rbInfo->rab[idx].datFwdPeerAddr.u.ipv4TptAddr.port = VE_DFLT_EGTP_PORT; 

                        for (ipAddrIdx = 0; ipAddrIdx < 16; ipAddrIdx++)
                        {
                           rbInfo->rab[idx].datFwdPeerAddr.u.ipv6TptAddr.ipv6NetAddr[ipAddrIdx] = \
                                   rbInfo->rab[idx].sgwAddr.u.ipv6TptAddr.ipv6NetAddr[ipAddrIdx];
                        }
                        break;

                     default:
                        /* would be in case if len = 160 bits which means support for both ipv4 and
                         * ipv6 */
                        break;
                  }

                  break;
               }
               case VE_DL_UL:
               {
                  /* Store local DL Fwd Tunnel Id */
                  VE_GET_LOC_TUNNEL_ID((mmeCb)->locEgtpTunCntr);
                  rbInfo->rab[idx].datFwdDlLocEgtpTunId = (mmeCb)->locEgtpTunCntr;

                  /* Store local UL Fwd Tunnel Id */
                  VE_GET_LOC_TUNNEL_ID((mmeCb)->locEgtpTunCntr);
                  rbInfo->rab[idx].datFwdUlLocEgtpTunId = (mmeCb)->locEgtpTunCntr;

                  /* Since there will not be any forwarding of data,
                   * from Tget and SGW, there will not be any remote tunnel 
                   * info for dat fwding, present in the HO REQ message.
                   * And EGTP needs both loc and rem address info to 
                   * create a tunnel.
                   * So we are filling a Src eNB Address */
                  
                  VE_ALLOC(&(rbInfo->rab[idx].remDlDataFwdTeId.val), \
                        (rbInfo->rab[idx].remEgtpTunId.len));

                  /* Copy Byte by Byte */
                  cmMemcpy(rbInfo->rab[idx].remDlDataFwdTeId.val, \
                        rbInfo->rab[idx].remEgtpTunId.val, \
                        rbInfo->rab[idx].remEgtpTunId.len);

                  rbInfo->rab[idx].remDlDataFwdTeId.len = rbInfo->rab[idx].remEgtpTunId.len;

                  VE_ALLOC(&(rbInfo->rab[idx].remUlDataFwdTeId.val), \
                        (rbInfo->rab[idx].remEgtpTunId.len));

                  /* Copy Byte by Byte */
                  cmMemcpy(rbInfo->rab[idx].remUlDataFwdTeId.val, \
                        rbInfo->rab[idx].remEgtpTunId.val, \
                        rbInfo->rab[idx].remEgtpTunId.len);

                  rbInfo->rab[idx].remUlDataFwdTeId.len = rbInfo->rab[idx].remEgtpTunId.len;

                  switch(nghCellCb->tEnbAddr.type)
                  {
                     case CM_TPTADDR_IPV4:
                        rbInfo->rab[idx].datFwdPeerAddr.type = CM_TPTADDR_IPV4;
                        rbInfo->rab[idx].datFwdPeerAddr.u.ipv4TptAddr.port = VE_DFLT_EGTP_PORT; 

                        cmMemcpy((U8 *)&(rbInfo->rab[idx].datFwdPeerAddr.u.ipv4TptAddr.address), \
                              (U8 *)&(nghCellCb->tEnbAddr.u.ipv4TptAddr.address),\
                              sizeof(rbInfo->rab[idx].datFwdPeerAddr.u.ipv4TptAddr.address));
                        break;

                     case CM_TPTADDR_IPV6:
                        rbInfo->rab[idx].datFwdPeerAddr.type = CM_TPTADDR_IPV6;
                        rbInfo->rab[idx].datFwdPeerAddr.u.ipv4TptAddr.port = VE_DFLT_EGTP_PORT; 

                        for (ipAddrIdx = 0; ipAddrIdx < 16; ipAddrIdx++)
                        {
                           rbInfo->rab[idx].datFwdPeerAddr.u.ipv6TptAddr.ipv6NetAddr[ipAddrIdx] = \
                                                                                              rbInfo->rab[idx].sgwAddr.u.ipv6TptAddr.ipv6NetAddr[ipAddrIdx];
                        }
                        break;

                     default:
                        /* would be in case if len = 160 bits which means support for both ipv4 and
                         * ipv6 */
                        break;
                  }

                  break;
                }
                default:
#ifdef DEBUGP
          VE_DBG_ERROR((VE_PRNT_BUF,"veFillErabFrmX2HoReq : \
              Invalid RAB Direction %d\n", rbInfo->rab[idx].rbDir));
#endif
                   break;
            }
         }
      }
   }
   rbInfo->numOfRbCfgd = erabLst->noComp.val;
   
   RETVALUE(ROK);
} /* end of veFillErabFrmX2HoReq */

/*
*
*       Fun:   veFillErabFrmX2HoAck
*
*       Desc:  Call handler to fill RAB info in UECB from X2 HO Request Acknoledge message
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_x2_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillErabFrmX2HoAck
(
VeUeCb *ueCb,
CztX2AP_PDU *x2HoAckPdu
)
#else
PUBLIC S16 veFillErabFrmX2HoAck(ueCb, x2HoAckPdu)
VeUeCb *ueCb;
CztX2AP_PDU *x2HoAckPdu;
#endif
{
   U8 erabId = 0;
   U8 rbId = 0;
   VeUeRABInfo *rbInfo;
   CztE_RABs_Admtd_Lst *eRABLst = NULLP;
   CztE_RABs_Admtd_Item *eRABItem = NULLP;
   U16  ipAddrIdx = 0;
   VeMmeCb *mmeCb = NULLP;

   TRC2(veFillErabFrmX2HoReq);

   rbInfo = &ueCb->rbInfo;
   /* fetch the ERAB Admitted list from the ACK PDU */
   veUtlGetCztIE(x2HoAckPdu, Cztid_E_RABs_Admtd_Lst, (TknU8 **) &eRABLst);
   /* Retrieve the MME CB */
   mmeCb = ueCb->s1ConCb->mmeCb;

   /* fill the FWD tunnel information from the ERAB list */
   for(erabId = 0; erabId < eRABLst->noComp.val; erabId++)
   {
      eRABItem = &eRABLst->member[erabId].value.u.cztE_RABs_Admtd_Item;
      for (rbId = 0; rbId < rbInfo->numOfRbCfgd; rbId++)
      {
         if(rbInfo->rab[rbId].eRABId == eRABItem->e_RAB_ID.val)
         {
            /* check if FWD tunel is already configured for this ERAB */
            if(rbInfo->rab[rbId].remDlDataFwdTeId.val != NULLP && 
                  rbInfo->rab[rbId].remUlDataFwdTeId.val != NULLP)
            {
#ifdef DEBUGP
               VE_DBG_ERROR((VE_PRNT_BUF,"veFillErabFrmX2HoAck : \
                        Multiple entry of same ERAB ID[id = %ld].\n",eRABItem->e_RAB_ID.val));
#endif
               RETVALUE(RFAILED);
            }

            /* Copy DL Tunnel info */
            if(eRABItem->dL_GTP_TunnelEndpoint.pres.pres == PRSNT_NODEF)
            {
               VE_ALLOC(&(rbInfo->rab[rbId].remDlDataFwdTeId.val), \
                     (eRABItem->dL_GTP_TunnelEndpoint.gTP_TEID.len));

               /* Copy Byte by Byte */
               cmMemcpy(rbInfo->rab[rbId].remDlDataFwdTeId.val, \
                     eRABItem->dL_GTP_TunnelEndpoint.gTP_TEID.val, \
                     eRABItem->dL_GTP_TunnelEndpoint.gTP_TEID.len);

               rbInfo->rab[rbId].remDlDataFwdTeId.len = 
                     eRABItem->dL_GTP_TunnelEndpoint.gTP_TEID.len;

               /* retrieve Serv. GW Address later will be used for tunnel creation */
               switch(eRABItem->dL_GTP_TunnelEndpoint.transportLyrAddr.len)
               {
                  case 32:
                  {
                     rbInfo->rab[rbId].datFwdPeerAddr.type = CM_TPTADDR_IPV4;
                     rbInfo->rab[rbId].datFwdPeerAddr.u.ipv4TptAddr.port = VE_DFLT_EGTP_PORT; 
                     rbInfo->rab[rbId].datFwdPeerAddr.u.ipv4TptAddr.address = 
                        *(U32*)eRABItem->dL_GTP_TunnelEndpoint.transportLyrAddr.val;
                  }
                  break;

                  case 128:
                  {
                     rbInfo->rab[rbId].datFwdPeerAddr.type = CM_TPTADDR_IPV6;
                     rbInfo->rab[rbId].datFwdPeerAddr.u.ipv6TptAddr.port = VE_DFLT_EGTP_PORT; 
                     for (ipAddrIdx = 0; ipAddrIdx < eRABItem->dL_GTP_TunnelEndpoint.transportLyrAddr.len; 
                           ipAddrIdx++)
                     {
                        rbInfo->rab[rbId].datFwdPeerAddr.u.ipv6TptAddr.ipv6NetAddr[ipAddrIdx] = 
                           eRABItem->dL_GTP_TunnelEndpoint.transportLyrAddr.val[ipAddrIdx];
                     }
                  }
                  break;

                  default:
                  {
                     /* would be in case if len = 160 bits which means support for both ipv4 and
                      * ipv6 */
                  }
                  break;
               }/* End of switch() */
               /* Generate and Store Local DL forwarding tunnel Ids*/
               VE_GET_LOC_TUNNEL_ID(mmeCb->locEgtpTunCntr);
               rbInfo->rab[rbId].datFwdDlLocEgtpTunId = mmeCb->locEgtpTunCntr;

            }
            /* Copy UL Tunnel info */
            if(eRABItem->uL_GTP_TunnelEndpoint.pres.pres == PRSNT_NODEF)
            {
               VE_ALLOC(&(rbInfo->rab[rbId].remUlDataFwdTeId.val), \
                     (eRABItem->uL_GTP_TunnelEndpoint.gTP_TEID.len));

               /* Copy Byte by Byte */
               cmMemcpy(rbInfo->rab[rbId].remUlDataFwdTeId.val, \
                     eRABItem->uL_GTP_TunnelEndpoint.gTP_TEID.val, \
                     eRABItem->uL_GTP_TunnelEndpoint.gTP_TEID.len);

               rbInfo->rab[rbId].remUlDataFwdTeId.len = 
                  eRABItem->dL_GTP_TunnelEndpoint.gTP_TEID.len;

               /* if DL Fwding was not proposed by S-eNB and UL Fwding was proposed by T-eNB, 
                  then also set isDataFwdAvl flag to TRUE */
               rbInfo->rab[rbId].isDataFwdAvl = TRUE;

               /* retrieve Serv. GW Address later will be used for tunnel creation */
               switch(eRABItem->uL_GTP_TunnelEndpoint.transportLyrAddr.len)
               {
                  case 32:
                  {   
                     rbInfo->rab[rbId].datFwdPeerAddr.type = CM_TPTADDR_IPV4;
                     rbInfo->rab[rbId].datFwdPeerAddr.u.ipv4TptAddr.port = VE_DFLT_EGTP_PORT; 
                     rbInfo->rab[rbId].datFwdPeerAddr.u.ipv4TptAddr.address = 
                        *(U32*) eRABItem->uL_GTP_TunnelEndpoint.transportLyrAddr.val;
                  }
                  break;

                  case 128:
                  {
                     rbInfo->rab[rbId].datFwdPeerAddr.type = CM_TPTADDR_IPV6;
                     rbInfo->rab[rbId].datFwdPeerAddr.u.ipv6TptAddr.port = VE_DFLT_EGTP_PORT; 
                     for (ipAddrIdx = 0; ipAddrIdx < eRABItem->uL_GTP_TunnelEndpoint.transportLyrAddr.len; 
                           ipAddrIdx++)
                     {
                        rbInfo->rab[rbId].datFwdPeerAddr.u.ipv6TptAddr.ipv6NetAddr[ipAddrIdx] = 
                           eRABItem->uL_GTP_TunnelEndpoint.transportLyrAddr.val[ipAddrIdx];
                     }
                  }
                  break;

                  default:
                  {
                     /* would be in case if len = 160 bits which means support for both ipv4 and
                      * ipv6 */
                  }
                  break;
               }/* End of switch() */
               /* Generate and Store Local UL forwarding tunnel Ids*/
               VE_GET_LOC_TUNNEL_ID(mmeCb->locEgtpTunCntr);
               rbInfo->rab[rbId].datFwdUlLocEgtpTunId = mmeCb->locEgtpTunCntr;
            }
         }/* End of if()*/
      }/* End of for() */
   }/* End of for() */
   
   RETVALUE(ROK);
} /* end of veFillErabFrmX2HoAck */


/*
*
*       Fun:   veUtlCztFillResetRqst 
*
*       Desc:  Call handler to fill X2-Reset Request message
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_x2_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veUtlCztFillResetRqst
(
CztEvnt          **pdu,
CztCause          *cause
)
#else
PUBLIC S16 veUtlCztFillResetRqst(pdu, cause)
CztEvnt          **pdu;
CztCause          *cause;
#endif
{
   CztEvnt    *resetRqstPdu = NULLP;
   S16        numComp = 0;
   S16        compCnt = 0;
   S16        ret = 0;

   CztProtIE_Field_ResetRqst_IEs *ie = NULLP;

   TRC2(veUtlCztFillResetRqst);

   if(NULLP == cause)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillResetRqst: cause is NULL.\n"));
#endif
      RETVALUE(RFAILED);
   }

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veUtlCztFillResetRqst: \
            Filling the X2-Reset Request\n"));
#endif

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(CztEvnt), VE_CZ_MEM_SDU_SIZE, &veCb.mem, 
                                                           (Ptr *)&resetRqstPdu);
   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillResetRqst: cmAllocEvnt failed\n"));
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */

   /* Fill the Interface Elementary Procedure.*/
   cmMemset((U8 *)&(resetRqstPdu->pdu), 0, sizeof(CztX2AP_PDU));
   veFillTknU8(&(resetRqstPdu->pdu.choice), X2AP_PDU_INITIATINGMSG);
   veFillTknU8(&(resetRqstPdu->pdu.val.initiatingMsg.pres), PRSNT_NODEF);
   veFillTknU32(&(resetRqstPdu->pdu.val.initiatingMsg.procedureCode), 
                                                            Cztid_reset);
   veFillTknU32(&(resetRqstPdu->pdu.val.initiatingMsg.criticality), 
                                                      CztCriticalityrejectEnum);

   numComp = 1;/* Indicates no of mandatory fields are present.*/
   veFillTknU8(&(resetRqstPdu->pdu.val.initiatingMsg.value.u.
                                                cztResetRqst.pres), PRSNT_NODEF);
   veFillTknU16(&(resetRqstPdu->pdu.val.initiatingMsg.value.u.cztResetRqst.
                                                protocolIEs.noComp),numComp);

   if (ROK != (cmGetMem(resetRqstPdu, 
       (numComp * sizeof(CztProtIE_Field_ResetRqst_IEs)), 
       (Ptr *)&resetRqstPdu->pdu.val.initiatingMsg.value.u.cztResetRqst.protocolIEs.member)))
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillResetRqst: cmGetMem failed\n"));
#endif
      VE_FREE_EVNT(resetRqstPdu);
      RETVALUE(RFAILED);
   } /* end of if statement */

   /* Fill the Cause IE */
   ie = &resetRqstPdu->pdu.val.initiatingMsg.value.u.cztResetRqst.protocolIEs.
                                                              member[compCnt++];
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Cztid_Cause);
   veFillTknU32(&(ie->criticality), CztCriticalityignoreEnum);
   veFillTknU8(&(ie->value.u.cztCause.choice), cause->choice.val);

  if(cause->choice.pres)
  {
    switch (cause->choice.val)
    {
      case CAUSE_RADIONW:
      {
         veFillTknU32(&(ie->value.u.cztCause.val.radioNw), 
                                                    cause->val.radioNw.val);
         break;
      }
      case CAUSE_TRANSPORT:
      {
         veFillTknU32(&(ie->value.u.cztCause.val.transport), 
                                                  cause->val.transport.val);
         break;
      }
      case CAUSE_PROTOCOL:
      {
         veFillTknU32(&(ie->value.u.cztCause.val.protocol), 
                                                   cause->val.protocol.val);
         break;
      }
      case CAUSE_MISC:
      {
         veFillTknU32(&(ie->value.u.cztCause.val.misc), 
                                                       cause->val.misc.val);
         break;
      }
      default:
      {
#ifdef DEBUGP
          VE_DBG_ERROR((VE_PRNT_BUF,"filling default X2_Reset CAUSE value\n"));
#endif
          veFillTknU8(&(ie->value.u.cztCause.choice), CAUSE_MISC);
          veFillTknU32(&(ie->value.u.cztCause.val.misc), 
                       CztCauseMiscunspecifiedEnum);
          break;
      }/* end of default */
    } /* end of switch statement */
  } /* end of cause if statement */

  *pdu = resetRqstPdu;

   RETVALUE(ROK);
}/* veUtlCztFillResetRqst */

/*
*
*       Fun:   veUtlCztFillResetRsp 
*
*       Desc:  Call handler to fill X2-Reset Response message
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_x2_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veUtlCztFillResetRsp
(
CztEvnt          **pdu
)
#else
PUBLIC S16 veUtlCztFillResetRsp(pdu)
CztEvnt          **pdu;
#endif
{
   CztEvnt    *resetRspPdu = NULLP;
   S16        numComp = 0;
   S16        ret = 0;

   TRC2(veUtlCztFillResetRsp);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veUtlCztFillResetRsp: \
            Filling the X2-Reset Request\n"));
#endif

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(CztEvnt), VE_CZ_MEM_SDU_SIZE, &veCb.mem, 
                                                           (Ptr *)&resetRspPdu);
   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillResetRsp: cmAllocEvnt failed\n"));
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */

   /* Fill the Interface Elementary Procedure.*/
   cmMemset((U8 *)&(resetRspPdu->pdu), 0, sizeof(CztX2AP_PDU));

   veFillTknU8(&(resetRspPdu->pdu.choice), X2AP_PDU_SUCCESSFULOUTCOME);
   veFillTknU8(&(resetRspPdu->pdu.val.successfulOutcome.pres), PRSNT_NODEF);
   veFillTknU32(&(resetRspPdu->pdu.val.successfulOutcome.procedureCode), 
                                                            Cztid_reset);
   veFillTknU32(&(resetRspPdu->pdu.val.successfulOutcome.criticality), 
                                                      CztCriticalityrejectEnum);
   veFillTknU8(&(resetRspPdu->pdu.val.successfulOutcome.value.u.cztResetResp.pres),PRSNT_NODEF);

   numComp = 0;/* Indicates no of mandatory fields are present.*/

   veFillTknU16(&(resetRspPdu->pdu.val.successfulOutcome.value.u.cztResetResp.
                                                protocolIEs.noComp),numComp);
   /* TODO: Can add optional parameter - Criticality Diagnostics */
  *pdu = resetRspPdu;

   RETVALUE(ROK);
}/* veUtlCztFillResetRsp */




/* X2_CFG_UPD_CHANGES*/
/*
 *       Fun:     veCztFillCfgUpdReq
 *
 *       Desc:    This function is for filling the config update
 *
 *       Ret:     ROK   - OK / RFAILED - Failure
 *
 *       Notes :  None.
 */
PUBLIC S16 veCztFillCfgUpdReq
(
   VeCfgUpdInfo              *x2CfgUpd,
  CztEvnt                    **pdu
)
{
   CztEvnt                   *x2ConfigUpdPdu = NULLP;
   U16                       numComp = 0;
   S16                        ret;
   U8                        ieCnt = 0;

   TRC2(veCztFillCfgUpdReq);
#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf,
       "veCztFillCfgUpdReq Building of  the eNodeB config update Request \n"));
#endif

   /* Allocate and Initialize memory control point */
   ret = cmAllocEvnt(sizeof(CztEvnt), VE_CZ_MEM_SDU_SIZE, &veCb.mem,
                                            (Ptr *)&x2ConfigUpdPdu);
   if(ret != ROK)
   {
#ifdef DEBUGP
     VE_DBG_ERROR((VE_PRNT_BUF,"veCztFillCfgUpdReq: cmAllocEvnt failed \n"));
#endif
     RETVALUE(RFAILED);
   } /* end of if part */

   /* Initialize the PDU */
   cmMemset((U8 *)&(x2ConfigUpdPdu->pdu), 0, sizeof(CztX2AP_PDU));

   /* Fill the contents of initiatingMsg */
   veFillTknU8(&(x2ConfigUpdPdu->pdu.choice), X2AP_PDU_INITIATINGMSG);
   veFillTknU8(&(x2ConfigUpdPdu->pdu.val.initiatingMsg.pres),PRSNT_NODEF);
   veFillTknU32(&(x2ConfigUpdPdu->pdu.val.initiatingMsg.procedureCode),
                             Cztid_eNBConfigUpd);
   veFillTknU32(&(x2ConfigUpdPdu->pdu.val.initiatingMsg.criticality),
                             CztCriticalityrejectEnum);
   veFillTknU8(&(x2ConfigUpdPdu->pdu.val.initiatingMsg.value.u.
                             cztENBConfigUpd.pres), PRSNT_NODEF);

   /* Check what all optional IEs required to be filled. */
   if(x2CfgUpd->numSrvdCellAdded != 0)
   {
     numComp++;
   }
   if(x2CfgUpd->numSrvdCellModified != 0)
   {
     numComp++;
   }
   if(x2CfgUpd->numSrvdCellDeleted != 0)
   {
     numComp++;
   }
   if(x2CfgUpd->numGrpIdAdded != 0)
   {
     numComp++;
   }
   if(x2CfgUpd->numGrpIdDeleted != 0)
   {
     numComp++;
   }

   /* Filling IEs */
   veFillTknU16(&(x2ConfigUpdPdu->pdu.val.initiatingMsg.value.u.
                             cztENBConfigUpd.protocolIEs.noComp), numComp);

   /* Get memory for numComp */
   if ((cmGetMem(x2ConfigUpdPdu,
                          (numComp * sizeof(CztProtIE_Field_ENBConfigUpd_IEs)),
                          (Ptr*)&x2ConfigUpdPdu->pdu.val.initiatingMsg.value.u.
                             cztENBConfigUpd.protocolIEs.member)) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veCztFillCfgUpdReq: cmGetMem failed \n"));
#endif
      /* Free the memory of Event Structure */
      VE_FREE_EVNT(x2ConfigUpdPdu);
      RETVALUE(RFAILED);
   } /* end of if statement */


   /* Fill the IE1 : Served Cells to Add IE if served cells are to added*/
   /* IE1 consists of Served Cell Info and Neighbour Info. */
   if(x2CfgUpd->numSrvdCellAdded != 0)
   {
      ret = veCfgUpdAddSrvdCell(x2CfgUpd->numSrvdCellAdded , x2CfgUpd->srvdCellAdd,
                             x2ConfigUpdPdu ,ieCnt);
      if( ret != ROK)
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,
                      "veCztFillCfgUpdReq: CfgUpdAddSrvdCell failed \n"));
#endif
         VE_FREE_EVNT(x2ConfigUpdPdu);
         RETVALUE(ret);
      }
      ieCnt++;
    }
   /* Fill the IE2 : Served Cells to Modify if served cells are to be modified*/
   /* IE2 consists of old ecgi, Served Cell Info and Neighbour Info. */
   if(x2CfgUpd->numSrvdCellModified != 0)
   {
       ret = veCfgUpdModSrvdCell(x2CfgUpd->numSrvdCellModified, x2CfgUpd->srvdCellMod,
                              x2ConfigUpdPdu, ieCnt );
      if( ret != ROK)
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,
                      "veCztFillCfgUpdReq: CfgUpdModSrvdCell failed \n"));
#endif
         VE_FREE_EVNT(x2ConfigUpdPdu);
         RETVALUE(ret);
      }
      ieCnt++;
    }
   /* Fill the IE3 : Served Cells to Delete if served cells are to be deleted*/
   /* IE3 consists of old ecgi */
   if(x2CfgUpd->numSrvdCellDeleted != 0)
   {
 
     ret = veCfgUpdDelSrvdCell(x2CfgUpd->numSrvdCellDeleted, x2CfgUpd->oldEcgiToDel,
                              x2ConfigUpdPdu, ieCnt );
     if( ret != ROK)
     {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,
                      "veCztFillCfgUpdReq: CfgUpdDelSrvdCell failed \n"));
#endif
         VE_FREE_EVNT(x2ConfigUpdPdu);
         RETVALUE(ret);
     }
     ieCnt++;
   }
   /* Fill the IE4 : GU Group Id To Add List if GU group id has to be added*/
   /* IE4 consists of GU Group ID IE. which in turns consists of:
      a. PLMN Id
      b. MME group id */
   if(x2CfgUpd->numGrpIdAdded != 0)
   {
     ret = veCfgUpdAddGUGrpIDLst(x2CfgUpd->numGrpIdAdded, x2CfgUpd->guIdAddLst,
                             x2ConfigUpdPdu, ieCnt);
     if( ret != ROK)
     {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,
                      "veCztFillCfgUpdReq: CfgUpdAddGuGrp failed \n"));
#endif
         VE_FREE_EVNT(x2ConfigUpdPdu);
         RETVALUE(ret);
     }
     ieCnt++;
   }
   /* Fill the IE5 : GU Group Id To Delete List if GU group id has to be deleted*/
   if(x2CfgUpd->numGrpIdDeleted != 0)
   {
     /* TODO: */
   }
   *pdu = x2ConfigUpdPdu;
   RETVALUE(ROK);
}/* end of veCztFillCfgUpdReq */




/*
 *       Fun:     VECfgUpdAddSrvdCell
 *
 *       Desc:    This function is for filling the Served Cells To Add IE
 *
 *       Ret:     ROK   - OK / RFAILED - Failure
 *
 *       Notes :  None.
 */
PRIVATE S16 veCfgUpdAddSrvdCell
(
   U8                        numSrvdCellAdded,
   U16                       srvdCellAdd[],
   CztEvnt                   *x2ConfigUpdPdu,
   U8                        ieCnt
)
{
   U16                       idx=0, idx1=0, len =0;
   U16                       brdcstComp = 0,  nbrComp;
   U16                       crntCellId;
   VeCellCb                  *crntCellCb = NULLP;
   CztProtIE_Field_ENBConfigUpd_IEs *ie  = NULLP;
   CztSrvdCellsMember        *ie1 = NULLP;
   CmLList                   *crtNode = NULLP;
   VeNghCellCb               *nghCellCb = NULLP;
   U8                        choice = 0;

   TRC2(veCfgUpdAddSrvdCell);
 
    /* Getting Served Cells To Add IE. */
   ie = &x2ConfigUpdPdu->pdu.val.initiatingMsg.value.u.cztENBConfigUpd.
                             protocolIEs.member[ieCnt];

   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Cztid_SrvdCellsToAdd);
   veFillTknU32(&(ie->criticality),CztCriticalityrejectEnum);

   /* The Served Cells To Add IE contains 2 IEs:
      a) Served Cell Information IE - mandatory ,
      b) Neighbour Information IE - optional */

   /* TODO :  Checking for optional IE. As of now we are always filling neighbour info */

   veFillTknU16(&(ie->value.u.cztSrvdCells.noComp), numSrvdCellAdded);

    /*Allocate memory for components. */
   if ((cmGetMem(x2ConfigUpdPdu,(numSrvdCellAdded * sizeof(CztSrvdCellsMember)),
                              (Ptr *)&ie->value.u.cztSrvdCells.member)) != ROK)
   {
       VE_DBG_ERROR((VE_PRNT_BUF,"veCfgUpdAddSrvdCell: cmGetMem failed\n"));
       RETVALUE(RFAILED);
   } 

   for(idx=0; idx < numSrvdCellAdded; idx++)
   {
     crntCellId = srvdCellAdd[idx];
   /*  VE_GET_CELLCB(crntCellCb,crntCellId);*/ 

    /* TODO */
    crntCellCb = veCb.cellCb[0];

     ie1 = &ie->value.u.cztSrvdCells.member[idx];

     veFillTknU8(&(ie1->pres), PRSNT_NODEF);
     /* Filling subIE1 : Served Cell Info IE */
     veFillTknU8(&(ie1->servedCellInfo.pres), PRSNT_NODEF);

     /* 1. Filling PCI */
      veFillTknU32(&(ie1->servedCellInfo.pCI), crntCellCb->physCellId);

     /* 2. Fill cell id */
     veFillTknU8(&(ie1->servedCellInfo.cellId.pres), PRSNT_NODEF);

     /* 2.1 Fill the PLMN ID within cell id. */
    len = 3;
    veCzFillTknStrOSXL(&(ie1->servedCellInfo.cellId.pLMN_Identity),
                   len, (Data *)crntCellCb->plmnId[0], &x2ConfigUpdPdu);

     /* 2.2 Fill eUTRAN cell identifier with cell id */
      if(ENB_ID_MACRO_ENB_ID == veCb.eNBType)
     {
       len = 20;
       veFillTknBStr32(&(ie1->servedCellInfo.cellId.eUTRANcellIdentifier), len,
                             crntCellId);
     }
     else
     {
       len = 28;
       veFillTknBStr32(&(ie1->servedCellInfo.cellId.eUTRANcellIdentifier), len,
                             crntCellId);
     }

     /* 3 Fill TAC. */
      len = 2;
     veFillTknStr4(&(ie1->servedCellInfo.tAC), len, crntCellCb->tac);

     /* 4 Fill broadcast PLMNS. */
     brdcstComp = crntCellCb->numBrdCstPlmn;
     veFillTknU16(&(ie1->servedCellInfo.broadcastPLMNs.noComp),brdcstComp); 

     /* Allocate memory for broadcast PLMNs */
     if ((cmGetMem(x2ConfigUpdPdu, brdcstComp*sizeof(CztPLMN_Identity),
               (Ptr*)&ie1->servedCellInfo.broadcastPLMNs.member)) !=ROK)
     {
#ifdef DEBUGP
       VE_DBG_ERROR((VE_PRNT_BUF,"veCfgUpdAddSrvdCell: cmGetMem failed\n"));
#endif
       VE_FREE_EVNT(x2ConfigUpdPdu);
       RETVALUE(RFAILED);
     }

     for(idx1 = 0; idx1 < brdcstComp; brdcstComp++)
     {
       len = 3;
       /* Fill the PLMN ID of source Cell Id */
       veCzFillTknStrOSXL(&(ie1->servedCellInfo.broadcastPLMNs.member[idx1]), len,
                      (Data *)crntCellCb->plmnId[idx1], &x2ConfigUpdPdu);
     }


     /* 5 Fill the eUTRA mode info. */
     choice = crntCellCb->duplexMode;

     switch (choice)
     {
       case EUTRA_MODE_INFO_FDD:
       {
         veFillTknU8(&(ie1->servedCellInfo.eUTRA_Mode_Info.choice), choice);
         veFillTknU8(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.fDD.pres),PRSNT_NODEF);
         veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.fDD.uL_EARFCN),
                       crntCellCb->u.fddInfo.ulEarfcn);
         veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.fDD.dL_EARFCN),
                             crntCellCb->u.fddInfo.dlEarfcn);
         veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.fDD.dL_Transmission_Bandwidth),
                             crntCellCb->u.fddInfo.dlTxBndWdth);
         break;
       }
       case EUTRA_MODE_INFO_TDD:
       {
         veFillTknU8(&(ie1->servedCellInfo.eUTRA_Mode_Info.choice), 1);
         veFillTknU8(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.tDD.pres),PRSNT_NODEF);
         veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.tDD.eARFCN),
                                           veCb.cellCb[0]->u.tddInfo.earfcn);
         veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.tDD.transmission_Bandwidth),
                                            veCb.cellCb[0]->u.tddInfo.txBndWdth);
         veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.tDD.subframeAssignment),
                                            veCb.cellCb[0]->u.tddInfo.sfAssgnmnt);
         /* This value has been set as NOTPRSNT as there is no support for special subframe in MAC-TDD */
         veFillTknU8(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.tDD.specialSubframe_Info.pres), NOTPRSNT);
         veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.tDD.specialSubframe_Info.specialSubframePatterns),
                                            veCb.cellCb[0]->u.tddInfo.splSfInfo.splSfPatrn);
         veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.tDD.specialSubframe_Info.cyclicPrefixDL),
                                            veCb.cellCb[0]->u.tddInfo.splSfInfo.dlCycPrefix);
         veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.tDD.specialSubframe_Info.cyclicPrefixUL),
                                            veCb.cellCb[0]->u.tddInfo.splSfInfo.ulCycPrefix);
         ie1->servedCellInfo.eUTRA_Mode_Info.val.tDD.specialSubframe_Info.iE_Extns.noComp.pres = NOTPRSNT;
   
        break;
       }
       default:
       {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veCfgUpdAddSrvdCell: Invalid Mode.\n"));
#endif
         break;
       }
     } /* end of switch statement */

     /* Fill Extn IE */
     ie1->servedCellInfo.iE_Extns.noComp.pres = NOTPRSNT;
 
     /*  Fill subIE2 : Neighbour info. */
     crtNode = veCb.cellCb[0]->nghCellLst.first; 
     while(NULLP != crtNode)
     {
       idx1 = 0;  
       crtNode = crntCellCb->nghCellLst.first;
       nghCellCb = (VeNghCellCb *)crtNode->node;
       if(nghCellCb != NULL)
          break;
          
       nbrComp = nghCellCb->numNghInfo;
       veFillTknU16(&(ie1->neighbour_Info.noComp),nbrComp);
       if(nbrComp == 0)
          break;
      
       /* Allocate memory for Neighbour info */
       if ((cmGetMem(x2ConfigUpdPdu, nbrComp*sizeof(CztNeighbour_InformMember),
               (Ptr*)&ie1->neighbour_Info.member)) !=ROK)
       {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veCfgUpdAddSrvdCell: cmGetMem failed\n"));
#endif
         VE_FREE_EVNT(x2ConfigUpdPdu);
         RETVALUE(RFAILED);
       }

       veFillTknU8(&(ie1->neighbour_Info.member[idx1].pres), PRSNT_NODEF);
        /*Fill ECGI */
       veFillTknU8(&(ie1->neighbour_Info.member[idx1].eCGI.pres), PRSNT_NODEF);

       /* 2.1 Fill the PLMN ID within cell id. */
        len = 3;
       veCzFillTknStrOSXL(&(ie1->neighbour_Info.member[idx1].eCGI.pLMN_Identity), len,
                      (Data *)nghCellCb->nghInfo[idx1]->ecgi.plmnId, &x2ConfigUpdPdu);

       /* 2.2 Fill eUTRAN cell identifier with cell id */
                                                                                                       
       if(ENB_ID_MACRO_ENB_ID == veCb.eNBType)
       {
         veFillTknBStr32(&(ie1->neighbour_Info.member[idx1].eCGI.eUTRANcellIdentifier), 20,
                                    nghCellCb->nghInfo[idx]->ecgi.eutranCellId);
       }
       else
       {
         veFillTknBStr32(&(ie1->neighbour_Info.member[idx1].eCGI.eUTRANcellIdentifier), 28,
                               nghCellCb->nghInfo[idx]->ecgi.eutranCellId);
       }

        /*Fill PCI */
        veFillTknU32(&(ie1->neighbour_Info.member[idx1].pCI), nghCellCb->nghInfo[idx]->physCellId );

       /* Fill EARFCN */
        veFillTknU32(&(ie1->neighbour_Info.member[idx1].eARFCN), nghCellCb->nghInfo[idx]->earfcn);
     
        crtNode = crtNode->next;
     }  /*end of for */
   } /* end for */
   RETVALUE(ROK);
} /* end of veCfgUpdAddSrvdCell */



/*
 *       Fun:     veCfgUpdModSrvdCell
 *
 *       Desc:    This function is for filling modifying served cell IE
 *
 *
 *       Ret:     ROK   - OK / RFAILED - Failure
 *
 *       Notes :  None.
 */
PRIVATE S16 veCfgUpdModSrvdCell
(
   U8                        numSrvdCellModified,
   VeSrvdCellMod             srvdCellMod[],
   CztEvnt                   *x2ConfigUpdPdu,
   U8                        ieCnt
)
{
   U16                       idx=0, idx1=0, len =0;
   U16                       brdcstComp = 0, nbrComp;
   U16                       crntCellId;
   VeCellCb                  *crntCellCb = NULLP;
   CztProtIE_Field_ENBConfigUpd_IEs *ie  = NULLP;
   CztSrvdCellsToMdfy_Item   *ie1 = NULLP;
   CmLList                   *crtNode = NULLP;
   VeNghCellCb               *nghCellCb = NULLP;
   U8                        choice = 0, plmn;

   TRC2(veCfgUpdModSrvdCell);
 
    /* Getting Served Cells To Add IE. */
   ie = &x2ConfigUpdPdu->pdu.val.initiatingMsg.value.u.cztENBConfigUpd.
                             protocolIEs.member[ieCnt];

   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Cztid_SrvdCellsToMdfy);
   veFillTknU32(&(ie->criticality),CztCriticalityrejectEnum);

   /* The Served Cells To Add IE contains 2 IEs:
      a) Served Cell Information IE - mandatory ,
      b) Neighbour Information IE - optional */

   /* TODO :  Checking for optional IE. As of now we are always filling neighbour info */
   veFillTknU16(&(ie->value.u.cztSrvdCells.noComp), numSrvdCellModified);
    /*Allocate memory for components. */
   if ((cmGetMem(x2ConfigUpdPdu,(numSrvdCellModified * sizeof(CztSrvdCellsToMdfy_Item)),
                              (Ptr *)&ie->value.u.cztSrvdCellsToMdfy.member)) != ROK)
   {
       VE_DBG_ERROR((VE_PRNT_BUF,"veCfgUpdModSrvdCell: cmGetMem failed\n"));
       RETVALUE(RFAILED);
   }

   for(idx=0; idx < numSrvdCellModified; idx++)
   {
     crntCellId = srvdCellMod[idx].newCellId;
   /*  VE_GET_CELLCB(crntCellCb,crntCellId);*/
    crntCellCb = veCb.cellCb[0];

     ie1 = &ie->value.u.cztSrvdCellsToMdfy.member[idx];

     veFillTknU8(&(ie1->pres), PRSNT_NODEF);
     /* Filling Old ECGI */
     /* Fill cell id */
     veFillTknU8(&(ie1->old_ecgi.pres), PRSNT_NODEF);
     /* Fill the PLMN ID within cell id. */
     len = 3;
     plmn = srvdCellMod[idx].oldCellEcgi.plmnId; 
     veCzFillTknStrOSXL(&(ie1->old_ecgi.pLMN_Identity),
                   len, (Data *)&plmn, &x2ConfigUpdPdu);

     /* Fill eUTRAN cell identifier with cell id */
     if(ENB_ID_MACRO_ENB_ID == veCb.eNBType)
     {
       len = 20;
       veFillTknBStr32(&(ie1->old_ecgi.eUTRANcellIdentifier), len,
                             srvdCellMod[idx].oldCellEcgi.eutranCellId);
     }
     else
     {
       len = 28;
       veFillTknBStr32(&(ie1->old_ecgi.eUTRANcellIdentifier), len,
                             srvdCellMod[idx].oldCellEcgi.eutranCellId);
     }

     ie1->old_ecgi.iE_Extns.noComp.pres = NOTPRSNT;

     /* Filling subIE1 : Served Cell Info IE */
     veFillTknU8(&(ie1->servedCellInfo.pres), PRSNT_NODEF);

     /* 1. Filling PCI */
     veFillTknU32(&(ie1->servedCellInfo.pCI), crntCellCb->physCellId);

     /* 2. Fill cell id */
     veFillTknU8(&(ie1->servedCellInfo.cellId.pres), PRSNT_NODEF);

     /* 2.1 Fill the PLMN ID within cell id. */
    len = 3;
    veCzFillTknStrOSXL(&(ie1->servedCellInfo.cellId.pLMN_Identity),
                   len, (Data *)crntCellCb->plmnId[0], &x2ConfigUpdPdu);

     /* 2.2 Fill eUTRAN cell identifier with cell id */
                                                                                                                              
     if(ENB_ID_MACRO_ENB_ID == veCb.eNBType)
     {
       len = 20;
       veFillTknBStr32(&(ie1->servedCellInfo.cellId.eUTRANcellIdentifier), len,
                             crntCellId);
     }
     else
     {
       len = 28;
       veFillTknBStr32(&(ie1->servedCellInfo.cellId.eUTRANcellIdentifier), len,
                             crntCellId);
     }
    
     /* 3 Fill TAC. */
     len = 2;
     veFillTknStr4(&(ie1->servedCellInfo.tAC), len, crntCellCb->tac);

     /* 4 Fill broadcast PLMNS. */
     brdcstComp = crntCellCb->numBrdCstPlmn;
     veFillTknU16(&(ie1->servedCellInfo.broadcastPLMNs.noComp),brdcstComp);

     /* Allocate memory for broadcast PLMNs */
     if ((cmGetMem(x2ConfigUpdPdu, brdcstComp*sizeof(CztPLMN_Identity),
               (Ptr*)&ie1->servedCellInfo.broadcastPLMNs.member)) !=ROK)
     {
#ifdef DEBUGP
       VE_DBG_ERROR((VE_PRNT_BUF,"veCfgUpdModSrvdCell: cmGetMem failed\n"));
#endif
       VE_FREE_EVNT(x2ConfigUpdPdu);
       RETVALUE(RFAILED);
     }

     for(idx1 = 0; idx1 < brdcstComp; brdcstComp++)
     {
       len = 3;
       /* Fill the PLMN ID of source Cell Id */
       veCzFillTknStrOSXL(&(ie1->servedCellInfo.broadcastPLMNs.member[idx1]), len,
                      (Data *)crntCellCb->plmnId[idx1], &x2ConfigUpdPdu);
     }

     /* 5 Fill the eUTRA mode info. */
     choice = crntCellCb->duplexMode;

     switch (choice)
     {
       case EUTRA_MODE_INFO_FDD:
       {
         veFillTknU8(&(ie1->servedCellInfo.eUTRA_Mode_Info.choice), choice);
         veFillTknU8(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.fDD.pres),PRSNT_NODEF);
         veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.fDD.uL_EARFCN),
                       crntCellCb->u.fddInfo.ulEarfcn);
         veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.fDD.dL_EARFCN),
                             crntCellCb->u.fddInfo.dlEarfcn);
         veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.fDD.dL_Transmission_Bandwidth),
                             crntCellCb->u.fddInfo.dlTxBndWdth);
         break;
       }
       case EUTRA_MODE_INFO_TDD:
       {
         veFillTknU8(&(ie1->servedCellInfo.eUTRA_Mode_Info.choice), 1);
         veFillTknU8(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.tDD.pres),PRSNT_NODEF);
         veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.tDD.eARFCN),
                                           veCb.cellCb[0]->u.tddInfo.earfcn);
         veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.tDD.transmission_Bandwidth),
                                            veCb.cellCb[0]->u.tddInfo.txBndWdth);
         veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.tDD.subframeAssignment),
                                            veCb.cellCb[0]->u.tddInfo.sfAssgnmnt);
         /* This value has been set as NOTPRSNT as there is no support for special subframe in MAC-TDD */
         veFillTknU8(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.tDD.specialSubframe_Info.pres), NOTPRSNT);
         veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.tDD.specialSubframe_Info.specialSubframePatterns),
                                            veCb.cellCb[0]->u.tddInfo.splSfInfo.splSfPatrn);
         veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.tDD.specialSubframe_Info.cyclicPrefixDL),
                                            veCb.cellCb[0]->u.tddInfo.splSfInfo.dlCycPrefix);
         veFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.tDD.specialSubframe_Info.cyclicPrefixUL),
                                            veCb.cellCb[0]->u.tddInfo.splSfInfo.ulCycPrefix);
         ie1->servedCellInfo.eUTRA_Mode_Info.val.tDD.specialSubframe_Info.iE_Extns.noComp.pres = NOTPRSNT;

        break;
       }
       default:
       {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veCfgUpdModSrvdCell: Invalid Mode.\n"));
#endif
         break;
       }
     } /* end of switch statement */

     /* Fill Extn IE */
     ie1->servedCellInfo.iE_Extns.noComp.pres = NOTPRSNT;

     /*  Fill subIE2 : Neighbour info. */
     crtNode = veCb.cellCb[0]->nghCellLst.first; 
     while(NULLP != crtNode)
     {
       idx1 = 0;  
       crtNode = crntCellCb->nghCellLst.first;
       nghCellCb = (VeNghCellCb *)crtNode->node;
       if(nghCellCb == NULL)
          break;

       nbrComp = nghCellCb->numNghInfo;
       veFillTknU16(&(ie1->neighbour_Info.noComp),nbrComp);
       if(nbrComp == 0) /*ISSUE*/
         break; 
       /* Allocate memory for Neighbour info */
       if ((cmGetMem(x2ConfigUpdPdu, nbrComp*sizeof(CztNeighbour_InformMember),
               (Ptr*)&ie1->neighbour_Info.member)) !=ROK)
       {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veCfgUpdModSrvdCell: cmGetMem failed\n"));
#endif
         VE_FREE_EVNT(x2ConfigUpdPdu);
         RETVALUE(RFAILED);
       }

       veFillTknU8(&(ie1->neighbour_Info.member[idx1].pres), PRSNT_NODEF);
        /*Fill ECGI */
      veFillTknU8(&(ie1->neighbour_Info.member[idx1].eCGI.pres), PRSNT_NODEF);

       /* 2.1 Fill the PLMN ID within cell id. */
       len = 3;
       veCzFillTknStrOSXL(&(ie1->neighbour_Info.member[idx1].eCGI.pLMN_Identity), len,
                      (Data *)nghCellCb->nghInfo[idx1]->ecgi.plmnId, &x2ConfigUpdPdu);

       /* 2.2 Fill eUTRAN cell identifier with cell id */
       if(ENB_ID_MACRO_ENB_ID == veCb.eNBType)
       {
         veFillTknBStr32(&(ie1->neighbour_Info.member[idx1].eCGI.eUTRANcellIdentifier), 20,
                                    nghCellCb->nghInfo[idx]->ecgi.eutranCellId);
       }
       else
       {
         veFillTknBStr32(&(ie1->neighbour_Info.member[idx1].eCGI.eUTRANcellIdentifier), 28,
                               nghCellCb->nghInfo[idx]->ecgi.eutranCellId);
       }

        /*Fill PCI */
       veFillTknU32(&(ie1->neighbour_Info.member[idx1].pCI), nghCellCb->nghInfo[idx]->physCellId );

       /* Fill EARFCN */
       veFillTknU32(&(ie1->neighbour_Info.member[idx1].eARFCN), nghCellCb->nghInfo[idx]->earfcn);

        crtNode = crtNode->next;
     }  /*end of for */
   } /* end for */
   RETVALUE(ROK);
} /* end of veCfgUpdModSrvdCell */


                                                                                                             
/*
 *       Fun:     veCfgUpdDelSrvdCell
 *
 *       Desc:    This function is for filling Deleted served cell IE
 *
 *
 *       Ret:     ROK   - OK / RFAILED - Failure
 *
 *       Notes :  None.
 */
PRIVATE S16 veCfgUpdDelSrvdCell
(
   U8                        numSrvdCellDeleted,
   VeCellEcgi                srvdCellDel[],
   CztEvnt                   *x2ConfigUpdPdu,
   U8                        ieCnt
)
{
   U16                       idx=0, len = 0;
   U16                       brdcstComp = 0;
   CztProtIE_Field_ENBConfigUpd_IEs *ie  = NULLP;
   CztECGI                  *ie1 = NULLP;

   U8 plmn;

   TRC2(veCfgUpdDelSrvdCell);

   ie = &x2ConfigUpdPdu->pdu.val.initiatingMsg.value.u.cztENBConfigUpd.
                             protocolIEs.member[ieCnt];

   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Cztid_SrvdCellsToDelete); 
   veFillTknU32(&(ie->criticality),CztCriticalityrejectEnum);

   veFillTknU16(&(ie->value.u.cztOld_ECGIs.noComp), numSrvdCellDeleted);

   /* Allocate memory for components. */
   if ((cmGetMem(x2ConfigUpdPdu,(numSrvdCellDeleted * sizeof(CztECGI)),
               (Ptr *)&ie->value.u.cztOld_ECGIs.member)) !=ROK)

   {
#ifdef DEBUGP
       VE_DBG_ERROR((VE_PRNT_BUF,"veCfgUpdDelSrvdCell: cmGetMem failed\n"));
#endif
       VE_FREE_EVNT(x2ConfigUpdPdu);
       RETVALUE(RFAILED);
   }
  
   for(idx=0; idx < numSrvdCellDeleted; idx++)
   {
     ie1 = &ie->value.u.cztOld_ECGIs.member[idx];

     /* Filling Old ECGI */
     /* Fill cell id */
     veFillTknU8(&(ie1->pres), PRSNT_NODEF);

     /* Fill the PLMN ID within cell id. */
     len = 3;
     plmn = srvdCellDel[idx].plmnId; 
     veCzFillTknStrOSXL(&(ie1->pLMN_Identity),
                   len, (Data *)&plmn, &x2ConfigUpdPdu);

     /* Fill eUTRAN cell identifier with cell id */
     if(ENB_ID_MACRO_ENB_ID == veCb.eNBType)
     {
       len = 20;
       veFillTknBStr32(&(ie1->eUTRANcellIdentifier), len,
                             srvdCellDel[idx].eutranCellId);
     }
     else
     {
       len = 28;
       veFillTknBStr32(&(ie1->eUTRANcellIdentifier), len,
                             srvdCellDel[idx].eutranCellId);
     }
  }/* end of for loop */
  RETVALUE(ROK);
} /* end of veCfgUpdDelSrvdCell */




/*
 *       Fun:     veCfgUpdAddGUGrpIDLst
 *
 *       Desc:    This function is for filling GU Group ID Add
 *
 *
 *       Ret:     ROK   - OK / RFAILED - Failure
 *
 *       Notes :  None.
 */
PRIVATE S16 veCfgUpdAddGUGrpIDLst
(
   U8                        numGrpIDAdded,
   VeGuId                    guIdAddLst[],
   CztEvnt                   *x2ConfigUpdPdu,
   U8                        ieCnt
)
{
   U8                        numComp;
   U16                       idx=0, len =0;
   U32                       plmn;

   TRC2(veCfgUpdAddGUGrpIDLst);

   CztProtIE_Field_ENBConfigUpd_IEs *ie  = NULLP;
   CztGU_Group_ID            *ie1 = NULLP;

   TRC2(veCfgUpdAddGUGrpIDLst);

   ie = &x2ConfigUpdPdu->pdu.val.initiatingMsg.value.u.cztENBConfigUpd.
                             protocolIEs.member[ieCnt];


   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Cztid_GUGroupIDToAddLst);
   veFillTknU32(&(ie->criticality),CztCriticalityrejectEnum);

   veFillTknU16(&(ie->value.u.cztGUGroupIDLst.noComp), numGrpIDAdded);
   /* Allocate memory for components. */
   if ((cmGetMem(x2ConfigUpdPdu,(numGrpIDAdded * sizeof(CztGU_Group_ID)),
               (Ptr *)&ie->value.u.cztGUGroupIDLst.member)) != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veCfgUpdAddGUGrpIDLst: cmGetMem failed\n"));
#endif
      VE_FREE_EVNT(x2ConfigUpdPdu);
      RETVALUE(RFAILED);
   }

   for(idx=0; idx < numGrpIDAdded; idx++)
   {
     ie1 = &ie->value.u.cztGUGroupIDLst.member[idx];

     veFillTknU8(&(ie1->pres), PRSNT_NODEF);
     /* Fill the PLMN ID within GU Group Id  */
     plmn = guIdAddLst[idx].plmnId;
     len =3;
     veCzFillTknStrOSXL(&(ie1->pLMN_Identity),
                   len, (Data *)&plmn, &x2ConfigUpdPdu);
    
     /* Fill the MME id */
       len = 2;
      veFillTknStr4(&(ie1->mME_Group_ID), len, guIdAddLst[idx].mmeGrpId); 
  }

  RETVALUE(ROK);
}/* end of veCfgUpdAddGUGrpIDLst */


/*
 *       Fun:     veCfgUpdDelGUGrpIDLst
 *
 *       Desc:    This function is for filling Deleted GU Group Id IE
 *
 *
 *       Ret:     ROK   - OK / RFAILED - Failure
 *
 *       Notes :  None.
 */
PRIVATE S16 veCfgUpdDelGUGrpIDLst
(
   U8                        numGrpIDDeleted,
   VeGuId                    guIdDelLst[],
   CztEvnt                   *x2ConfigUpdPdu,
   U8                        ieCnt
)
{
   U8                        numComp;
   U16                       idx=0, len = 0;
   CztProtIE_Field_ENBConfigUpd_IEs *ie  = NULLP;
   CztGU_Group_ID            *ie1 = NULLP;
   U32 plmn = 0;

   TRC2(veCfgUpdDelGUGrpIDLst);

   ie = &x2ConfigUpdPdu->pdu.val.initiatingMsg.value.u.cztENBConfigUpd.
                             protocolIEs.member[ieCnt];

   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Cztid_GUGroupIDToDeleteLst);
   veFillTknU32(&(ie->criticality),CztCriticalityrejectEnum);
   
   veFillTknU16(&(ie->value.u.cztGUGroupIDLst.noComp), numGrpIDDeleted); /* ISSUE */

   /* Allocate memory for components. */
   if ((cmGetMem(x2ConfigUpdPdu,(numGrpIDDeleted * sizeof(Cztid_GUGroupIDToDeleteLst)),
              (Ptr *)&ie->value.u.cztGUGroupIDLst.member)) !=ROK)
   {
#ifdef DEBUGP
     VE_DBG_ERROR((VE_PRNT_BUF,"veConfigUpd: cmGetMem failed\n"));
#endif
     VE_FREE_EVNT(x2ConfigUpdPdu);
          RETVALUE(RFAILED);
   }

   for(idx=0; idx < numGrpIDDeleted; idx++)
   {
     ie1 = &ie->value.u.cztGUGroupIDLst2.member[idx];

     veFillTknU8(&(ie1->pres), PRSNT_NODEF);
     /* Fill the PLMN ID within GU Group Id  */
     len = 3;
     plmn = guIdDelLst[idx].plmnId;
     veCzFillTknStrOSXL(&(ie1->pLMN_Identity),
                   len, (Data *)&plmn, &x2ConfigUpdPdu);
     printf("\n^^^^^^^^^^^ %d\n", plmn);
     /* Fill the MME id */
     veFillTknStr4(&(ie1->mME_Group_ID), 2, guIdDelLst[idx].mmeGrpId);
  }
  RETVALUE(ROK);
}/* end of veCfgUpdDelGUGrpIDLst */



/*
 *       Fun:     veFillConfigUpdateFail
 *
 *       Desc:    This function is for filling config update failure message
 *
 *
 *       Ret:     ROK   - OK / RFAILED - Failure
 *
 *       Notes :  None.
 */
PUBLIC S16 veFillConfigUpdateFail
(
   CztEvnt                  **pdu,
   VeIeInfo                  *idLst,
   U16                       cause,
   U8                        causeType,
   U32                       timeToWait
)
{
   CztEvnt                   *x2ConfigUpdFailPdu = NULLP;
   U16                       numComp = 0;
   U8                        ret;
   CztProtIE_Field_ENBConfigUpdFail_IEs *ie = NULLP;

   TRC2(veFillConfigUpdateFail);

   /* Allocate and Initialize memory control point */
   ret = cmAllocEvnt(sizeof(CztEvnt), VE_CZ_MEM_SDU_SIZE, &veCb.mem,
                                            (Ptr *)&x2ConfigUpdFailPdu);
   if(ret != ROK)
   {
#ifdef DEBUGP
     VE_DBG_ERROR((VE_PRNT_BUF,"veFillConfigUpdateFail: cmAllocEvnt failed \n"));
#endif
     RETVALUE(RFAILED);
   } /* end of if part */

   /* Initialize the PDU */
   cmMemset((U8 *)&(x2ConfigUpdFailPdu->pdu), 0, sizeof(CztX2AP_PDU));

   /* Fill the contents of initiatingMsg */
   veFillTknU8(&(x2ConfigUpdFailPdu->pdu.choice), X2AP_PDU_UNSUCCESSFULOUTCOME);
   veFillTknU8(&(x2ConfigUpdFailPdu->pdu.val.unsuccessfulOutcome.pres),PRSNT_NODEF);
   veFillTknU32(&(x2ConfigUpdFailPdu->pdu.val.unsuccessfulOutcome.procedureCode),
                            Cztid_eNBConfigUpd);
   veFillTknU32(&(x2ConfigUpdFailPdu->pdu.val.unsuccessfulOutcome.criticality),
                             CztCriticalityrejectEnum);
   veFillTknU8(&(x2ConfigUpdFailPdu->pdu.val.unsuccessfulOutcome.value.u.
                            cztENBConfigUpdFail.pres), PRSNT_NODEF);

   /* Check for optional IE. */
   /* Filling 1 mandatory IE and 2  optional IEs if present */
   numComp = 1;
   if(timeToWait > 0)
   {
     numComp++;
   }
   if(idLst)
   {
     numComp++;
   }

   veFillTknU16(&(x2ConfigUpdFailPdu->pdu.val.unsuccessfulOutcome.value.u.
                             cztENBConfigUpdFail.protocolIEs.noComp), numComp);
   /* Get memory for numComp */
   if ((cmGetMem(x2ConfigUpdFailPdu,
                          (numComp * sizeof(CztProtIE_Field_ENBConfigUpdFail_IEs)),
                          (Ptr*)&x2ConfigUpdFailPdu->pdu.val.unsuccessfulOutcome.value.u.
                             cztENBConfigUpdFail.protocolIEs.member)) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillConfigUpdateFail: cmGetMem failed \n"));
#endif
      /* Free the memory of Event Structure */
      VE_FREE_EVNT(x2ConfigUpdFailPdu);
      RETVALUE(RFAILED);
   } /* end of if statement */


   /* Filling IE1 Cause. */
   /* IE1 - Filling cause */
   ie = &x2ConfigUpdFailPdu->pdu.val.unsuccessfulOutcome.value.u.
                       cztENBConfigUpdFail.protocolIEs.member[0];


   veFillTknU8(&(ie->pres),PRSNT_NODEF);
   veFillTknU32(&(ie->id),Cztid_Cause);
   veFillTknU32(&(ie->criticality),CztCriticalityignoreEnum);

   /* Filling the cause type and value */
   veFillTknU8(&(ie->value.u.cztCause.choice),causeType);
   veFillTknU32(&(ie->value.u.cztCause.val.radioNw),cause);

   if(timeToWait > 0) 
   {
     /* Filling IE2 Time To Wait */
     ie = &x2ConfigUpdFailPdu->pdu.val.unsuccessfulOutcome.value.u.
                       cztENBConfigUpdFail.protocolIEs.member[1];

     veFillTknU8(&(ie->pres),PRSNT_NODEF);
     veFillTknU32(&(ie->id),Cztid_TimeToWait);  /* ISSUE */
     veFillTknU32(&(ie->criticality),CztCriticalityignoreEnum);
     veFillTknU32(&(ie->value.u.cztTimeToWait), timeToWait); /*ISSUE */
   }
 
   if(idLst)
   { 
     /* Filling IE3 Criticality Diagnostic IE */
     ie = &x2ConfigUpdFailPdu->pdu.val.unsuccessfulOutcome.value.u.
                       cztENBConfigUpdFail.protocolIEs.member[2];

     veFillTknU8(&(ie->pres),PRSNT_NODEF);
     veFillTknU32(&(ie->id),Cztid_Cause);
     veFillTknU32(&(ie->criticality),CztCriticalityignoreEnum);

     /* TODO */
   }
   *pdu = x2ConfigUpdFailPdu;
   RETVALUE(ROK);
} /* end of veFillConfigUpdateFail */



/*
 *       Fun:     veFillConfigUpdateAck
 *
 *       Desc:    This function is for filling config update ack message
 *
 *
 *       Ret:     ROK   - OK / RFAILED - Failure
 *
 *       Notes :  None.
 */
PUBLIC S16 veFillConfigUpdateAck
(
   CztEvnt                   **pdu,
   VeIeInfo                  *idLst
)
{
   CztEvnt                   *x2ConfigUpdAckPdu = NULLP;
   U16                       numComp = 0;
   U8                        ret;

   CztProtIE_Field_ENBConfigUpdAckg_IEs *ie  = NULLP;
   TRC2(veFillConfigUpdateAck);

   /* Allocate and Initialize memory control point */
   ret = cmAllocEvnt(sizeof(CztEvnt), VE_CZ_MEM_SDU_SIZE, &veCb.mem,
                                            (Ptr *)&x2ConfigUpdAckPdu);
   if(ret != ROK)
   {
#ifdef DEBUGP
     VE_DBG_ERROR((VE_PRNT_BUF,"veFillConfigUpdateAck: cmAllocEvnt failed \n"));
#endif
     RETVALUE(RFAILED);
   } /* end of if part */

   /* Initialize the PDU */
   cmMemset((U8 *)&(x2ConfigUpdAckPdu->pdu), 0, sizeof(CztX2AP_PDU));

   /* Fill the contents of initiatingMsg */
   veFillTknU8(&(x2ConfigUpdAckPdu->pdu.choice), X2AP_PDU_SUCCESSFULOUTCOME);
   veFillTknU8(&(x2ConfigUpdAckPdu->pdu.val.successfulOutcome.pres),PRSNT_NODEF);
   veFillTknU32(&(x2ConfigUpdAckPdu->pdu.val.successfulOutcome.procedureCode),
                             Cztid_eNBConfigUpd);
   veFillTknU32(&(x2ConfigUpdAckPdu->pdu.val.successfulOutcome.criticality),
                             CztCriticalityrejectEnum);
  veFillTknU8(&(x2ConfigUpdAckPdu->pdu.val.successfulOutcome.value.u.
                             cztENBConfigUpdAckg.pres), PRSNT_NODEF);

   /* Filling 1  optional IE if present */
   numComp = 0;
   if(idLst)
   {
     numComp++;
   }

   veFillTknU16(&(x2ConfigUpdAckPdu->pdu.val.successfulOutcome.value.u.
                             cztENBConfigUpdAckg.protocolIEs.noComp), numComp);

   if(numComp > 0)
   {
     /* Get memory for numComp */
     if ((cmGetMem(x2ConfigUpdAckPdu,
                          (numComp * sizeof(CztProtIE_Field_ENBConfigUpdAckg_IEs)),
                          (Ptr*)&x2ConfigUpdAckPdu->pdu.val.successfulOutcome.value.u.
                             cztENBConfigUpdAckg.protocolIEs.member)) !=ROK)
     {
#ifdef DEBUGP
       VE_DBG_ERROR((VE_PRNT_BUF,"veFillConfigUpdateAck: cmGetMem failed \n"));
#endif
       /* Free the memory of Event Structure */
       VE_FREE_EVNT(x2ConfigUpdAckPdu);
       RETVALUE(RFAILED);
     } /* end of if statement */


     /* Filling Criticality Diagnostic IE. */
     ie = &x2ConfigUpdAckPdu->pdu.val.successfulOutcome.value.u.cztENBConfigUpdAckg.
                             protocolIEs.member[0];

     veFillTknU8(&(ie->pres), PRSNT_NODEF);
     veFillTknU32(&(ie->id), Cztid_CriticalityDiag);
     veFillTknU32(&(ie->criticality),CztCriticalityignoreEnum);

     /* TODO */
   } /* end if */
   *pdu = x2ConfigUpdAckPdu;
   RETVALUE(ROK);
} /* end of veFillConfigUpdateAck */


/*
 *       Fun:     veFillErrInd
 *
 *       Desc:    This function is for filling error indication message
 *
 *
 *       Ret:     ROK   - OK / RFAILED - Failure
 *
 *       Notes :  None.
 */
PUBLIC S16 veFillErrInd
(
  CztEvnt                    **pdu,
  U32                        oldeNBUeX2apId,
  U32                        neweNBUeX2apId,
  Bool                       causePres,
  U8                         causeType,
  U8                         causeVal,
  VeIeInfo                   *idLst
)
{
   CztEvnt                   *x2ErrIndPdu = NULLP;
   U16                       numComp = 0 , ret = 0;
   CztProtIE_Field_ErrInd_IEs *ie  = NULLP;

   TRC2(veFillErrInd);

   /* Allocate and Initialize memory control point */
   ret = cmAllocEvnt(sizeof(CztEvnt), VE_CZ_MEM_SDU_SIZE, &veCb.mem,
                                            (Ptr *)&x2ErrIndPdu);
   if(ret != ROK)
   {
#ifdef DEBUGP
     VE_DBG_ERROR((VE_PRNT_BUF,"veFillErrInd: cmAllocEvnt failed \n"));
#endif
     RETVALUE(RFAILED);
   } /* end of if part */

   /* Initialize the PDU */
   cmMemset((U8 *)&(x2ErrIndPdu->pdu), 0, sizeof(CztX2AP_PDU));

   /* Fill the contents of initiatingMsg */
   veFillTknU8(&(x2ErrIndPdu->pdu.choice), X2AP_PDU_INITIATINGMSG);
   veFillTknU8(&(x2ErrIndPdu->pdu.val.initiatingMsg.pres),PRSNT_NODEF);
   veFillTknU32(&(x2ErrIndPdu->pdu.val.initiatingMsg.procedureCode),
                             Cztid_errorInd);
   veFillTknU32(&(x2ErrIndPdu->pdu.val.initiatingMsg.criticality),
                             CztCriticalityrejectEnum);
  veFillTknU8(&(x2ErrIndPdu->pdu.val.initiatingMsg.value.u.
                             cztErrInd.pres), PRSNT_NODEF);

  /* Check for optional IEs */
  numComp = 0;
  if(oldeNBUeX2apId != 0)
  {
     numComp++;
  }
  if(neweNBUeX2apId != 0)
  {
    numComp++;
  }
  if(causePres == TRUE)
  {
     numComp++;
  }
  if(idLst)
  {
    numComp++;
  }

  veFillTknU16(&(x2ErrIndPdu->pdu.val.initiatingMsg.value.u.
                             cztErrInd.protocolIEs.noComp), numComp);

  if(numComp == 0)
  {
     *pdu =x2ErrIndPdu;
     RETVALUE(ROK);
  }

   /* Get memory for numComp */
   if ((cmGetMem(x2ErrIndPdu,
                          (numComp * sizeof(CztProtIE_Field_ErrInd_IEs)),
                          (Ptr*)&x2ErrIndPdu->pdu.val.initiatingMsg.value.u.
                             cztErrInd.protocolIEs.member)) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillErrInd: cmGetMem failed \n"));
#endif
      /* Free the memory of Event Structure */
      VE_FREE_EVNT(x2ErrIndPdu);
      RETVALUE(RFAILED);
   } /* end of if statement */


   /* Fill the Old EnB UE X2AP Id */
   if(oldeNBUeX2apId != 0)
   {
      ie = &x2ErrIndPdu->pdu.val.initiatingMsg.value.u.cztErrInd.
                             protocolIEs.member[0];

     veFillTknU8(&(ie->pres), PRSNT_NODEF);
     veFillTknU32(&(ie->id), Cztid_Old_eNB_UE_X2AP_ID);
     veFillTknU32(&(ie->criticality),CztCriticalityignoreEnum);
     veFillTknU32(&(ie->value.u.cztUE_X2AP_ID_Old),
                             oldeNBUeX2apId);
   }
   /* Fill the Old EnB UE X2AP Id */
   if(neweNBUeX2apId != 0)
   {
      ie = &x2ErrIndPdu->pdu.val.initiatingMsg.value.u.cztErrInd.
                             protocolIEs.member[1];

     veFillTknU8(&(ie->pres), PRSNT_NODEF);
     veFillTknU32(&(ie->id), Cztid_New_eNB_UE_X2AP_ID);
     veFillTknU32(&(ie->criticality),CztCriticalityignoreEnum);
     veFillTknU32(&(ie->value.u.cztUE_X2AP_ID_New),
                             neweNBUeX2apId);
   }
   /* IE3 : Cause */
   if(causePres)
   {
     ie = &x2ErrIndPdu->pdu.val.initiatingMsg.value.u.cztErrInd.
                             protocolIEs.member[2];

     veFillTknU8(&(ie->pres), PRSNT_NODEF);
     veFillTknU32(&(ie->id), Cztid_Cause);
     veFillTknU32(&(ie->criticality),CztCriticalityignoreEnum);
     /* Filling the cause type and value. */
     veFillTknU8(&(ie->value.u.cztCause.choice), causeType);
     veFillTknU32(&(ie->value.u.cztCause.val.radioNw),
                             causeVal);
    }
    if(idLst)
     {
        ie = &x2ErrIndPdu->pdu.val.initiatingMsg.value.u.cztErrInd.
                             protocolIEs.member[3];

       veFillTknU8(&(ie->pres), PRSNT_NODEF);
       veFillTknU32(&(ie->id), Cztid_Cause);
       veFillTknU32(&(ie->criticality),CztCriticalityignoreEnum);
       /* TODO */
     }
     
     *pdu =x2ErrIndPdu;
     RETVALUE(ROK);
}
/* X2_CFG_UPD_CHANGES END */


#endif /* End of LTE_HO_SUPPORT */
/********************************************************************30**

         End of file:     ve_x2_bldmsg.c@@/main/eNB_1.3_With_LatestGpr3/12 - Fri Sep 16 17:30:07 2011

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
$SID$     ----       mnawas     1. eNodeb 3.1 Release
*********************************************************************91*/
