/**
 * @file    ueh_utils.c
 * @brief This file contains the UEH utility functions
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Amar Ramteke, 16Sep2010 :
*     a) Created initial version from sample application
 */

/* Header include files (.h) */

#include "ccpu_cmn.h"

/* RRM related includes */
#include "lueh.h"
#include "lueh.x" 
#include "ueh_eut_x.h" 
#include "ueh.h" 
#include "bs_tables.h"

#ifdef __cplusplus
EXTERN "C" {
#endif

#ifdef __cplusplus
}
#endif

EXTERN U16 szElmSize[][SZT_MAX_PROC_ID];

PRIVATE U8  SzMsgId[SZT_MAX_PROC_ID][SZT_MAX_MSG_TYPE] =
{
   /* Handover preparation : Procedure Code : 0  */
   {  
      SZT_MSG_ID_HO_REQD,      /* Initiating Message */
      SZT_MSG_ID_HO_CMD,       /* Successful Outcome */
      SZT_MSG_ID_HO_PREP_FAIL, /* UnSuccessful Outcome */
   },
   /* Handover resource allocation : Procedure Code : 1 */
   {  
      SZT_MSG_ID_HO_REQ,     /* Initiating Message */
      SZT_MSG_ID_HO_REQ_ACK, /* Successful Outcome */
      SZT_MSG_ID_HO_FAIL,    /* UnSuccessful Outcome */
   },
   /* Handover notification : Procedure Code : 2 */
   {  
      SZT_MSG_ID_HO_NTFY, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,  /* Successful Outcome */
      SZT_MSG_ID_NO_MSG,  /* UnSuccessful Outcome */
   },
   /* Path switch request : Procedure Code : 3 */
   {  
      SZT_MSG_ID_PATH_SWTCH_REQ,      /* Initiating Message */
      SZT_MSG_ID_PATH_SWTCH_REQ_ACK,  /* Successful Outcome */
      SZT_MSG_ID_PATH_SWTCH_REQ_FAIL, /* UnSuccessful Outcome */
   },
   /* Handover cancel : Procedure Code : 4 */
   {  
      SZT_MSG_ID_HO_CANCEL,     /* Initiating Message */
      SZT_MSG_ID_HO_CANCEL_ACK, /* Successful Outcome */
      SZT_MSG_ID_NO_MSG,        /* UnSuccessful Outcome */
   },
   /* E-RAB Assignment : Procedure Code : 5  */
   {  
      SZT_MSG_ID_E_RAB_BRR_SETUP_REQ, /* Initiating Message */
      SZT_MSG_ID_E_RAB_BRR_SETUP_RSP, /* Successful Outcome */
      SZT_MSG_ID_NO_MSG,            /* UnSuccessful Outcome */
   },
   /* E-RAB modification : Procedure Code : 6  */
   {  
      SZT_MSG_ID_E_RAB_BRR_MOD_REQ, /* Initiating Message */
      SZT_MSG_ID_E_RAB_BRR_MOD_RSP, /* Successful Outcome */
      SZT_MSG_ID_NO_MSG,          /* UnSuccessful Outcome */
   },
   /* E-RAB release : Procedure Code : 7  */
   {
      SZT_MSG_ID_E_RAB_BRR_REL_CMD,  /* Initiating Message */
      SZT_MSG_ID_E_RAB_BRR_REL_CMPLT,/* Successful Outcome */
      SZT_MSG_ID_NO_MSG,           /* UnSuccessful Outcome */
   },
   /* E-RAB release request : Procedure Code : 8  */
   {
      SZT_MSG_ID_E_RAB_BRR_REL_REQ, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,          /* Successful Outcome */
      SZT_MSG_ID_NO_MSG           /* UnSuccessful Outcome */
   },
   /* Initial Context setup : Procedure Code : 9  */
   {
      SZT_MSG_ID_CXT_SETUP_REQ,      /* Initiating Message */
      SZT_MSG_ID_CXT_SETUP_RSP,      /* Successful Outcome */
      SZT_MSG_ID_INIT_CXT_SETUP_FAIL,/* UnSuccessful Outcome */
   },
   /* Paging : Procedure Code : 10  */
   {
      SZT_MSG_ID_PAGING, /* Initiating Message */
      SZT_MSG_ID_NO_MSG, /* Successful Outcome */
      SZT_MSG_ID_NO_MSG  /* UnSuccessful Outcome */
   },
   /* DL NAS transport  : Procedure Code : 11 */
   {
      SZT_MSG_ID_DL_NAS_TPT, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,     /* Successful Outcome */
      SZT_MSG_ID_NO_MSG      /* UnSuccessful Outcome */
   },
   /* Initial UE message : Procedure Code : 12  */
   {
      SZT_MSG_ID_INIT_UE_MSG, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,      /* Successful Outcome */
      SZT_MSG_ID_NO_MSG       /* UnSuccessful Outcome */
   },
   /* UL NAS transport  : Procedure Code : 13 */
   {
      SZT_MSG_ID_UL_NAS_TPT, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,     /* Successful Outcome */
      SZT_MSG_ID_NO_MSG      /* UnSuccessful Outcome */
   },
   /* Reset : Procedure Code : 14  */
   {
      SZT_MSG_ID_RESET,      /* Initiating Message */
      SZT_MSG_ID_RESET_ACK,  /* Successful Outcome */
      SZT_MSG_ID_NO_MSG,     /* UnSuccessful Outcome */
   },
   /* Error indication : Procedure Code : 15 */
   {
      SZT_MSG_ID_ERR_IND, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,  /* Successful Outcome */
      SZT_MSG_ID_NO_MSG   /* UnSuccessful Outcome */
   },
   /* NAS non delivery indication : Procedure Code : 16 */
   {
      SZT_MSG_ID_NAS_NON_DEL_IND, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,          /* Successful Outcome */
      SZT_MSG_ID_NO_MSG           /* UnSuccessful Outcome */
   },
   /* Setup : Procedure Code : 17  */
   {
      SZT_MSG_ID_SETUP_REQ,  /* Initiating Message */
      SZT_MSG_ID_SETUP_RSP,  /* Successful Outcome */
      SZT_MSG_ID_SETUP_FAIL  /* UnSuccessful Outcome */
   },
   /* UE Context release request : Procedure Code : 18 */
   {
      SZT_MSG_ID_UE_CXT_REL_REQ, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,         /* Successful Outcome */
      SZT_MSG_ID_NO_MSG          /* UnSuccessful Outcome */
   },
   /* DL CDMA2000 Tunnelling : Procedure Code : 19  */
   {  
      SZT_MSG_ID_DNLNK_CDMA_TUNNL, /* Initiating Message */
      SZT_MSG_ID_NO_MSG, /* Successful Outcome */
      SZT_MSG_ID_NO_MSG, /* UnSuccessful Outcome */
   },
   /* UL CDMA2000 Tunnelling : Procedure Code : 20  */
   {  
      SZT_MSG_ID_UPLNK_CDMA_TUNNL, /* Initiating Message */
      SZT_MSG_ID_NO_MSG, /* Successful Outcome */
      SZT_MSG_ID_NO_MSG, /* UnSuccessful Outcome */
   },
   /* UE context modification : Procedure Code : 21  */
   {
      SZT_MSG_ID_UE_CXT_MOD_REQ, /* Initiating Message */
      SZT_MSG_ID_UE_CXT_MOD_RSP, /* Successful Outcome */
      SZT_MSG_ID_UE_CXT_MOD_FAIL /* UnSuccessful Outcome */
   },
   /* UE capability Info Indication : Procedure Code : 22  */
   {  
      SZT_MSG_ID_UE_CAP_INFO, /* Initiating Message */
      SZT_MSG_ID_NO_MSG, /* Successful Outcome */
      SZT_MSG_ID_NO_MSG, /* UnSuccessful Outcome */
   },
   /* UE context release : Procedure Code : 23  */
   {
      SZT_MSG_ID_UE_CXT_REL_CMD,  /* Initiating Message */
      SZT_MSG_ID_UE_CXT_REL_CMPL, /* Successful Outcome */
      SZT_MSG_ID_NO_MSG           /* UnSuccessful Outcome */
   },
   /* ENB status transfer : Procedure Code : 24  */
   {  
      SZT_MSG_ID_ENB_STA_TRFR, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,       /* Successful Outcome */
      SZT_MSG_ID_NO_MSG,       /* UnSuccessful Outcome */
   },
   /* MME status transfer : Procedure Code : 25  */
   {  
      SZT_MSG_ID_MME_STA_TRFR, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,       /* Successful Outcome */
      SZT_MSG_ID_NO_MSG,       /* UnSuccessful Outcome */
   },
   /* Deactivate trace : Procedure Code : 26  */
   {  
      SZT_MSG_ID_DEACTV_TRACE, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,       /* Successful Outcome */
      SZT_MSG_ID_NO_MSG,       /* UnSuccessful Outcome */
   },
   /* Trace start : Procedure Code : 27  */
   {  
      SZT_MSG_ID_TRACE_START, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,      /* Successful Outcome */
      SZT_MSG_ID_NO_MSG,      /* UnSuccessful Outcome */
   },
   /* Trace failure indication : Procedure Code : 28  */
   {  
      SZT_MSG_ID_TRACE_FAIL_IND, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,         /* Successful Outcome */
      SZT_MSG_ID_NO_MSG,         /* UnSuccessful Outcome */
   },
   /* ENB configuration update : Procedure Code : 29  */
   {  
      SZT_MSG_ID_ENB_CFG_UPD,      /* Initiating Message */
      SZT_MSG_ID_ENB_CFG_UPD_ACK,  /* Successful Outcome */
      SZT_MSG_ID_ENB_CFG_UPD_FAIL, /* UnSuccessful Outcome */
   },
   /* MME configuration update : Procedure Code : 30  */
   {  
      SZT_MSG_ID_MME_CFG_UPD,      /* Initiating Message */
      SZT_MSG_ID_MME_CFG_UPD_ACK,  /* Successful Outcome */
      SZT_MSG_ID_MME_CFG_UPD_FAIL, /* UnSuccessful Outcome */
   },
   /* Location reporting control : Procedure Code : 31  */
   {  
      SZT_MSG_ID_LOC_REPORT_CNTRL, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,           /* Successful Outcome */
      SZT_MSG_ID_NO_MSG,           /* UnSuccessful Outcome */
   },
   /* Location report failure indication : Procedure Code : 32  */
   {  
      SZT_MSG_ID_LOC_REPORT_FAIL_IND, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,              /* Successful Outcome */
      SZT_MSG_ID_NO_MSG,              /* UnSuccessful Outcome */
   },
   /* Location report : Procedure Code : 33  */
   {  
      SZT_MSG_ID_LOC_REPORT, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,     /* Successful Outcome */
      SZT_MSG_ID_NO_MSG,     /* UnSuccessful Outcome */
   },
   /* Overload Start : Procedure Code : 34  */
   {  
      SZT_MSG_ID_OVLD_START, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,     /* Successful Outcome */
      SZT_MSG_ID_NO_MSG,     /* UnSuccessful Outcome */
   },
   /* Overload Stop : Procedure Code : 35  */
   {  
      SZT_MSG_ID_OVLD_STOP, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,     /* Successful Outcome */
      SZT_MSG_ID_NO_MSG,     /* UnSuccessful Outcome */
   },
   /* Write Replace Warning : Procedure Code : 36  */
   {  
      SZT_MSG_ID_WARNING_REQ, /* Initiating Message */
      SZT_MSG_ID_WARNING_RSP,     /* Successful Outcome */
      SZT_MSG_ID_NO_MSG,     /* UnSuccessful Outcome */
   },
   /* ENB Direct Information Transfer : Procedure Code : 37  */
   {  
      SZT_MSG_ID_ENB_DIR_TRANS, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,     /* Successful Outcome */
      SZT_MSG_ID_NO_MSG,     /* UnSuccessful Outcome */
   },
   /* MME Direct Information Transfer : Procedure Code : 38  */
   {  
      SZT_MSG_ID_MME_DIR_TRANS, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,     /* Successful Outcome */
      SZT_MSG_ID_NO_MSG,     /* UnSuccessful Outcome */
   }
};

/*
 *
 *      Fun:   uehCpyCmTptAddr
 *
 *      Desc:  Copy Addr.
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *      File:  ueh_utils.c
 *
 */
PUBLIC S16 uehCpyCmTptAddr(
    CmTptAddr *dst,         /* pointer to the destination CmTptAddr. */
    CmTptAddr *src          /* pointer to the source CmTptAddr. */
  )
{
   logFunc("%s", "uehCpyCmTptAddr");

   if((src == NULLP) || (dst == NULLP))
   {
      UEH_ERROR_NULL_POINTER_IDENTIFIED();
      RETVALUE(RFAILED);
   }

   dst->type = src->type;
   if(src->type == CM_INET_IPV4ADDR_TYPE)
   {
      dst->u.ipv4TptAddr.port = src->u.ipv4TptAddr.port;
      dst->u.ipv4TptAddr.address = src->u.ipv4TptAddr.address;
   }
   else if(src->type == CM_INET_IPV6ADDR_TYPE)
   {
      dst->u.ipv6TptAddr.port = src->u.ipv6TptAddr.port;
      cmMemcpy((U8*)dst->u.ipv6TptAddr.ipv6NetAddr, (U8*)src->u.ipv6TptAddr.ipv6NetAddr,
            sizeof(CmInetIpAddr6));
   }

   RETVALUE( ROK);
}
#if 0
/*
 *
 *      Fun:   uehSetState
 *
 *      Desc:  changes the state
 *
 *      Ret:   void
 *
 *      Notes: None.
 *
 *      File:  ueh_utils.c
 *
 */
inline void uehSetState( UehUeCb *ueCb, UeState newState )
{
  logState("%d->%d|crnti=%d", ueCb->ueState, newState, ueCb->crnti);
  ueCb->ueState = newState;
}
#endif
/*
 *
 *      Fun:   uehSztUtilsGetMsgIdx
 *
 *      Desc:  Retrieve message index
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *      File:  ueh_utils.c
 *
 */
PUBLIC S16 uehSztUtilsGetMsgIdx  (
    U8 *msgIdx,
    S1apPdu *pdu
  )
{
   U8 msgType;
   U8 procCode;

   logFunc("%s", "uehSztUtilsGetMsgIdx");

   if(pdu->pdu.choice.pres == TRUE)
   {
      msgType  = pdu->pdu.choice.val;
   }
   else
   {
      *msgIdx = SZT_MSG_ID_NO_DATA;
      RETVALUE(ROK);
   }

   switch(msgType)
   {
      case S1AP_PDU_INITIATINGMSG:
         if(pdu->pdu.val.initiatingMsg.pres.pres == TRUE)
         {
            procCode = pdu->pdu.val.initiatingMsg.procedureCode.val;
            *msgIdx = SzMsgId[procCode][msgType];
         }
         else 
         { 
            *msgIdx = SZT_MSG_ID_NO_MSG;
         }
         break;
      case S1AP_PDU_SUCCESSFULOUTCOME:
         if(pdu->pdu.val.successfulOutcome.pres.pres == TRUE)
         {
            procCode = pdu->pdu.val.successfulOutcome.procedureCode.val;
            *msgIdx = SzMsgId[procCode][msgType];
         }
         else
         {
            *msgIdx = SZT_MSG_ID_NO_MSG;
         }
         break;
      case S1AP_PDU_UNSUCCESSFULOUTCOME:
         if(pdu->pdu.val.unsuccessfulOutcome.pres.pres == TRUE)
         {
            procCode = pdu->pdu.val.unsuccessfulOutcome.procedureCode.val;
            *msgIdx = SzMsgId[procCode][msgType];
         }
         else
         {
            *msgIdx = SZT_MSG_ID_NO_MSG;
         }
         break;
   }
   RETVALUE(ROK);
}


/*                                     
*
*       Fun:   uehSzFillTknU8
*
*       Desc:  Fills  the TknU8 structure with the specified value
*
*       Ret:   ROK       - ok
*
*       Notes: None
*
*       File:  ueh_utils.c
*
*/
PUBLIC S16  uehSzFillTknU8(
    TknU8  *encParam,
    U8     val
  )
{
   logFunc("%s", "uehSzFillTknU8");

   encParam->pres = PRSNT_NODEF;
   encParam->val  = val;

   RETVALUE(ROK);
}

/*                                     
*
*       Fun:   uehSzFillTknU16
*
*       Desc:  Fill a TknU16 structure
*
*       Ret:   ROK       - ok
*
*       Notes: None
*
*       File:  sz_msgfn.c
*
*/
PUBLIC S16  uehSzFillTknU16(
    TknU16  *encParam,
    U16     val
  )
{
   logFunc("%s", "uehSzFillTknU16");

   encParam->pres = PRSNT_NODEF;
   encParam->val  = val;

   RETVALUE(ROK);
}

/*                                     
*
*       Fun:   uehSzFillTknU32
*
*       Desc:  Fills the TknU32 structure
*
*       Ret:   RA_ROK       - ok
*
*       Notes: None
*
*       File:  ueh_utils.c
*
*/

PUBLIC S16  uehSzFillTknU32(
    TknU32  *encParam,
    U32     val
  )
{
   logFunc("%s", "uehSzFillTknU32");

   encParam->pres = PRSNT_NODEF;
   encParam->val  = val;

   RETVALUE(ROK);
}

/*
 *
 *      Fun:   uehSzFillTknStrOSXL
 *
 *      Desc:  Fill TknStrOSXL
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *      File:  ueh_utils.c
 *
 */
PUBLIC S16 uehSzFillTknStrOSXL(
    TknStrOSXL *ptr,     /* Pointer to the structure */
    U16         len,     /* length to be filled */
    Data       *val,     /* byte to be repetitively filled in */
    S1apPdu    **pdu
  )
{
   U16     idx;     /* counter */

   logFunc("%s", "uehSzFillTknStrOSXL");

   /* make the element as present */
   ptr->pres = TRUE;
   ptr->len =  len;

   if (cmGetMem(pdu[0], len * sizeof(U8), (Ptr *)&ptr->val) != ROK)
   {
      UEH_ERROR_MEM_ALLOC_FAILED(len, "s1AP PDU");
      RETVALUE(RFAILED);
   }

   for(idx = 0; idx < len; idx++)
   {
      ptr->val[idx] = val[idx];
   }

   RETVALUE(ROK);
}

PUBLIC S16 uehSzFillTknStrOSXL1(
    TknStrOSXL *ptr,     /* Pointer to the structure */
    U16         len,     /* length to be filled */
    U32         val,     /* byte to be repetitively filled in */
    S1apPdu    **pdu
  )
{
   U16     idx;     /* counter */
   U8      tmpVal;

   logFunc("%s", "uehSzFillTknStrOSXL1");

   /* make the element as present */
   ptr->pres = TRUE;
   ptr->len =  len;

   if (cmGetMem(pdu[0], len * sizeof(U8), (Ptr *)&ptr->val) != ROK)
   {
     UEH_ERROR_MEM_ALLOC_FAILED(len, "s1AP PDU");
     RETVALUE(RFAILED);
   }

   for(idx = 0; idx < len; idx++)
   {
      tmpVal = (U8)(val >> (8 * idx));
      ptr->val[len - (idx + 1)] = tmpVal;

   }

   RETVALUE(ROK);
}

/*
*
*       Fun:   uehSzFillTknBitStr4
*
*       Desc:  Call handler to fill Bit string of specified length
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  szac_sztutl.c
*
*/
PUBLIC S16 uehSzFillTknBitStr4(
    TknStr4 *ptr,     /* Pointer to the structure */
    U8      len,      /* length to be filled */
    U8      *val       /* byte to be repetitively filled in */
  )
{
   U8      idx;     /* counter */
   U8      lenInBytes = 0;

   logFunc("%s", "uehSzFillTknBitStr4");

   /* make the element as present */
   ptr->pres = TRUE;

   if (len > 32)
   {
      RETVALUE(RFAILED);
   }
   else
   {
      ptr->len = (U8) len;
   }

   if((len % 8) == 0 )
   {
     lenInBytes = (len/8);
   }
   else
   {
     lenInBytes = (len/8) + 1;
   }

   for(idx = 0; idx < lenInBytes; idx++)
   {
      ptr->val[idx] = val[idx];
   }

   RETVALUE(ROK);
}

/*
*
*       Fun:   uehSzFillTknStr4
*
*       Desc:  Call handler to fill Octet string of length less than 4
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  szac_sztutl.c
*
*/
PUBLIC S16 uehSzFillTknStr4(
    TknStr4 *ptr,     /* Pointer to the structure */
    U8      len,      /* length to be filled */
    U8      val       /* byte to be repetitively filled in */
  )
{
   U8      idx;     /* counter */

   logFunc("%s", "uehSzFillTknStr4");

   /* make the element as present */
   ptr->pres = TRUE;

   if (len > 4)
   {
      RETVALUE(RFAILED);
   }
   else
   {
      ptr->len = (U8) len;
   }

   for(idx = 0; idx < len; idx++)
   {
      ptr->val[idx] = val;
   }

   RETVALUE(ROK);
}

/*
*
*       Fun:   uehSzFill2TknStr4
*
*       Desc:  Call handler to fill Octet string of length less than 4
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  szac_sztutl.c
*
*/
PUBLIC S16 uehSzFill2TknStr4(
    TknStr4 *ptr,     /* Pointer to the structure */
    U8      len,      /* length to be filled */
    U8      *val       /* byte to be repetitively filled in */
  )
{
   U8      idx;     /* counter */

   logFunc("%s", "uehSzFill2TknStr4");

   /* make the element as present */
   ptr->pres = TRUE;

   if (len > 4)
   {
      RETVALUE(RFAILED);
   }
   else
   {
      ptr->len = (U8) len;
   }

   for(idx = 0; idx < len; idx++)
   {
      ptr->val[idx] = val[idx];
   }

   RETVALUE(ROK);
}


#ifndef LTE_ENB_PERF
/*
 *
 *      Fun:   uehSzGetIE
 *
 *      Desc:  Retrieve message index
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *      File:  ueh_utils.c
 *
 */
PUBLIC S16 uehSzGetIE(
    SztS1AP_PDU *pdu,
    U32     id,
    TknU8   **ie
  )
{
   TknU8   *currStr;    /* Start definition */
   TknU8   *startStr;   /* Start event structure */
   U8      choiceType;
   U16     numComp;
   U16     recIdx;
   U32     recId;
   U32     procCode;
   U16     size;

   logFunc("%s", "uehSzGetIE");

   /* Initialize the return pointer Tkn to NOTPRSNT */
   *ie = NULLP;

   numComp = 0;
   recIdx = 0; 

   if(pdu->choice.pres != PRSNT_NODEF)
   {
      RETVALUE(ROK);
   }

   choiceType = pdu->choice.val;

   /* Make start string point to the message in the PDU   */
   switch(choiceType)
   {
      case S1AP_PDU_INITIATINGMSG:
         if (pdu->val.initiatingMsg.pres.pres != PRSNT_NODEF)
            RETVALUE(ROK);

         startStr  = (TknU8 *)(&(pdu->val.initiatingMsg.value.u));
         procCode  = pdu->val.initiatingMsg.procedureCode.val;
         break;

      case S1AP_PDU_SUCCESSFULOUTCOME:
         if (pdu->val.successfulOutcome.pres.pres != PRSNT_NODEF)
            RETVALUE(ROK);

         startStr  = (TknU8 *)(&(pdu->val.successfulOutcome.value.u));
         procCode  = pdu->val.successfulOutcome.procedureCode.val;
         break;

      default: 
         if ((pdu->val.unsuccessfulOutcome.pres.pres != PRSNT_NODEF) ||
               (choiceType != S1AP_PDU_UNSUCCESSFULOUTCOME))
            RETVALUE(ROK);

         startStr  = (TknU8 *)(&(pdu->val.unsuccessfulOutcome.value.u));
         procCode  = pdu->val.unsuccessfulOutcome.procedureCode.val;
         break;
   }

   if (((TknU8 *)startStr)->pres == PRSNT_NODEF)
   {
      /* Increment it by TknU8 for TknPres */
      currStr  = (TknU8 *) ((PTR)startStr + sizeof(TknU8));
   }
   else
      RETVALUE(ROK);

   if (((TknU16 *)(currStr))->pres == PRSNT_NODEF)
   {
      /* get the Number of IEs received : noComp */
      numComp = ((TknU16 *)(currStr))->val;
   }

   /*  Increment currStr by TknU16 for skipping noComp */
   currStr = (TknU8 *)((PTR)currStr + sizeof(TknU16));

   currStr = *(TknU8**)currStr;

   while (recIdx < numComp) 
   {
      startStr = currStr;

      if (((TknPres *)currStr)->pres == PRSNT_NODEF)
      {   
         /* Increment it by TknU8 for TknPres */
         currStr  = (TknU8 *) ((PTR)currStr + sizeof(TknPres));

         /* Get the received Id  */
         recId  = ((SztProtIE_ID *)currStr)->val; 

         /* Compare the two Ids */
         if (recId == id)
         {
            startStr = (TknU8 *) ((PTR)currStr + sizeof(SztProtIE_ID) + sizeof(SztCriticality));

            *ie = startStr;
            break;
         }
      }

      size = szElmSize[choiceType][procCode];
      currStr  = (TknU8 *) ((PTR)startStr + size);
      recIdx++;
   }

   RETVALUE(ROK);
} /* end of uehSzGetIE */
#endif
/*
 *
 *      Fun:   uehInitUeSrbInfo
 *
 *      Desc:  Populate UehUeRABInfo Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_utils.c 
 *
 */
PUBLIC void uehInitUeSrbInfo(
    UehUeRABInfo *srbCfg,
    U8            rabId
  )
{
   U8 idx;
   
   logFunc("%s", "uehInitUeSrbInfo");

   idx = srbCfg->numOfSrbCfgd++;
   srbCfg->srb[idx].rbType = CM_LTE_SRB;
   srbCfg->srb[idx].rbId = rabId;
   srbCfg->srb[idx].pdcpId = rabId;
   srbCfg->srb[idx].rgrCfgMask = UEH_RB_RGRMSK_ZERO;
   return;

} /* uehInitUeSrbInfo */

#if 0
/* Akash: SAVE Msg prototype - start */
/*       Fun:   UehInitSaveQ
 *
 *      Desc:  Initialize the SAVEQ in ue-cb
 *      
 *      OK / RFAILED
 *
 *      Notes: None.
 *
 *      File:  ueh_utils.c
 */
PUBLIC S8 UehInitSaveQ (
    UehUeCb *uecb
    )
{
  UINT32 i=0;
  uecb->uecmSaveQNextFreeIdx = 0;

  for(i=0;i<SAVEQLEN;i++) 
  {
    uecb->uecmSaveQ[i].event = -1;
  }
  RETVALUE(ROK);
}

/*       Fun:   UehSaveInput
 *
 *      Desc:  Add an event to the SAVEQ in ue-cb
 *      
 *      OK / RFAILED
 *
 *      Notes: None.
 *
 *      File:  ueh_utils.c
 */
PUBLIC S8 UehSaveInput(
    SINT32 event,
    void *cb,
    void *sdu
    /* TODO: BOOL  head;*/
  )
{
  char details[UEH_EVNT_DETAIL_MAX_LEN];
  /* TODO: param 'head' : if true, add to HEAD, else add to TAIL of the saveQ, default add to tail */

  UehUeCb  *ueCb = (UehUeCb *)cb;
  snprintf(details, UEH_EVNT_DETAIL_MAX_LEN, "Input %d saved in state %d", event, ueCb->ueState);
  //FUNC(ueCb->crnti,UehSaveInput,UECM,UECM,details);

  /* MAX Q leng exceeded */
  if(ueCb->uecmSaveQNextFreeIdx>=SAVEQLEN)
  {
    RETVALUE(RFAILED);
  }

  /* ueCb->uecmSaveQNextFreeIdx should be minimum -1 when saveQ is empty,
   * it should never be less than -1
   */
  if(ueCb->uecmSaveQNextFreeIdx < -1)
  {
    UEH_ERROR_ELMNT_OUT_OF_RANGE_VERIFY_VALID_RANGE( "uecmSaveQNextFreeIdx" , ueCb->uecmSaveQNextFreeIdx, "greater or equal to -1");
    RETVALUE(RFAILED);
  }

  /* The free Index must have had NULL event (-1) - this is to find coding bugs */
  if(ueCb->uecmSaveQ[(U8)(ueCb->uecmSaveQNextFreeIdx)].event != -1)
  {
    RETVALUE(RFAILED);
  }

  ueCb->uecmSaveQ[(U8)(ueCb->uecmSaveQNextFreeIdx)].event = event;
  ueCb->uecmSaveQ[(U8)(ueCb->uecmSaveQNextFreeIdx)].sdu = sdu;
  ueCb->uecmSaveQ[(U8)(ueCb->uecmSaveQNextFreeIdx)].cb = ueCb;
  ueCb->uecmSaveQNextFreeIdx++;

  RETVALUE(ROK);
}

/*       Fun:   UehPopSaveQHead
 *
 *      Desc:  Gets the head node (event) from the SAVEQ in ue-cb
 *      
 *      Ret:   UecmSaveQNode
 *
 *      Notes: None.
 *
 *      File:  ueh_utils.c
 */
PUBLIC UecmSaveQNode UehPopSaveQHead(
    UehUeCb  *ueCb
    )
{
  UecmSaveQNode node;
  UINT32 i=0;
  char details[UEH_EVNT_DETAIL_MAX_LEN];

  node.event = -1;

  /* if the returned node has event = -1, then it means failure to pop from saveQ
   * Q is empty
   */
  if(ueCb->uecmSaveQNextFreeIdx < 0)
  {
    RETVALUE(node); //
  }

  node = ueCb->uecmSaveQ[0];
  ueCb->uecmSaveQNextFreeIdx--;
  /* shift nodes left to replace the head that is just popped out */
  while(i<ueCb->uecmSaveQNextFreeIdx)
  {
    ueCb->uecmSaveQ[i] = ueCb->uecmSaveQ[i+1];
  }

  snprintf(details, UEH_EVNT_DETAIL_MAX_LEN, "Popped event %d, SaveQ depth=%d", node.event,ueCb->uecmSaveQNextFreeIdx);
  //FUNC(ueCb->crnti,UehPopSaveQHead,UECM,UECM,details);
  
  RETVALUE(node);
}

PUBLIC S8 UehFlushTrace()
{
  if(strlen(uehCb.callTrcBuf) > 0)
  {
    logInfo("CALLTRC: Writing to callTrcFile %d bytes", strlen(uehCb.callTrcBuf));
    if(fprintf(callTrcFile,uehCb.callTrcBuf) < 0)
    {
      UEH_ERROR_WRITE_TO_FILE_FAILED("call trace"); 
      RETVALUE(RFAILED);
    }
    fflush(callTrcFile);
    uehCb.callTrcBuf[0]=0; /* reinitializing -> setting string length to zero */
  }
  RETVALUE(ROK);
}

PUBLIC S8 UehSaveTrace
  (
    char *trc
  )
{
  int trcLen=0, bufMargin=0;
  trcLen = strlen(trc);

  bufMargin = CALLTRCBUFLEN - strlen(uehCb.callTrcBuf) - 1;

  if(trcLen < bufMargin )
  {
    /* There is room in buffer */
    strcat(uehCb.callTrcBuf, trc);
    RETVALUE(ROK);
  }
  else
  {
    /* not enough room in buffer, so first clear the buffer (write to Trc file)
     * then write to buffer */
    if(fprintf(callTrcFile,uehCb.callTrcBuf) < 0)
    {
      UEH_ERROR_WRITE_TO_FILE_FAILED("call trace"); 
      RETVALUE(RFAILED);
    }
    logInfo("CALLTRC: Writing to callTrcFile:%d bytes", strlen(uehCb.callTrcBuf) );

    uehCb.callTrcBuf[0]=0; /* reinitializing -> setting string length to zero */
    if(fprintf(callTrcFile,trc) < 0)
    {
      UEH_ERROR_WRITE_TO_FILE_FAILED("call trace"); 
      RETVALUE(RFAILED);
    }
    fflush(callTrcFile);
  }
  RETVALUE(ROK);
}

#endif

PUBLIC S8 UehStrTime (char *timestr)
{
  struct timeval tv;
  struct tm* ptm;
  char time_string[UEH_EVNT_TIME_STR_MAX_LEN - 5];
  long milliseconds;

  /* Obtain the time of day, and convert it to a tm struct.
   * Format the date and time, down to a single second
   * Compute milliseconds from microseconds
   */
  gettimeofday (&tv, NULL);
  ptm = localtime (&tv.tv_sec);
  strftime (time_string, sizeof (time_string), "%H:%M:%S", ptm);
  milliseconds = tv.tv_usec / 1000;

  /* Print the formatted time, in seconds, followed by a decimal point
   * and the milliseconds. */
  snprintf (timestr, UEH_EVNT_TIME_STR_MAX_LEN, "%s.%03ld", time_string, milliseconds);
  RETVALUE(ROK);
}

/* Akash: CallTrace - end */

/***********************************************************
 *
 *     Func : uehGetPollRetxForOamVal
 *
 *     Desc : Returns the Mapped value in RRC
 *
 *     Ret  :
 *
 *     Notes: None
 *
 *     File : ueh_utils.c
 *
 **********************************************************/
const NhuTPollReTx pollRetxForOamVal[ AMRLCTPOLLRETRANSMIT_MS500 + 1] =
  {
    NHU_TPOLL_RETX_MS5,
    NHU_TPOLL_RETX_MS10,
    NHU_TPOLL_RETX_MS15,
    NHU_TPOLL_RETX_MS20,
    NHU_TPOLL_RETX_MS25,
    NHU_TPOLL_RETX_MS30,
    NHU_TPOLL_RETX_MS35,
    NHU_TPOLL_RETX_MS40,
    NHU_TPOLL_RETX_MS45,
    NHU_TPOLL_RETX_MS50,
    NHU_TPOLL_RETX_MS55,
    NHU_TPOLL_RETX_MS60,
    NHU_TPOLL_RETX_MS65,
    NHU_TPOLL_RETX_MS70,
    NHU_TPOLL_RETX_MS75,
    NHU_TPOLL_RETX_MS80,
    NHU_TPOLL_RETX_MS85,
    NHU_TPOLL_RETX_MS90,
    NHU_TPOLL_RETX_MS95,
    NHU_TPOLL_RETX_MS100,
    NHU_TPOLL_RETX_MS105,
    NHU_TPOLL_RETX_MS110,
    NHU_TPOLL_RETX_MS115,
    NHU_TPOLL_RETX_MS120,
    NHU_TPOLL_RETX_MS125,
    NHU_TPOLL_RETX_MS130,
    NHU_TPOLL_RETX_MS135,
    NHU_TPOLL_RETX_MS140,
    NHU_TPOLL_RETX_MS145,
    NHU_TPOLL_RETX_MS150,
    NHU_TPOLL_RETX_MS155,
    NHU_TPOLL_RETX_MS160,
    NHU_TPOLL_RETX_MS165,
    NHU_TPOLL_RETX_MS170,
    NHU_TPOLL_RETX_MS175,
    NHU_TPOLL_RETX_MS180,
    NHU_TPOLL_RETX_MS185,
    NHU_TPOLL_RETX_MS190,
    NHU_TPOLL_RETX_MS195,
    NHU_TPOLL_RETX_MS200,
    NHU_TPOLL_RETX_MS205,
    NHU_TPOLL_RETX_MS210,
    NHU_TPOLL_RETX_MS215,
    NHU_TPOLL_RETX_MS220,
    NHU_TPOLL_RETX_MS225,
    NHU_TPOLL_RETX_MS230,
    NHU_TPOLL_RETX_MS235,
    NHU_TPOLL_RETX_MS240,
    NHU_TPOLL_RETX_MS245,
    NHU_TPOLL_RETX_MS250,
    NHU_TPOLL_RETX_MS300,
    NHU_TPOLL_RETX_MS350,
    NHU_TPOLL_RETX_MS400,
    NHU_TPOLL_RETX_MS450,
    NHU_TPOLL_RETX_MS500
  };

NhuTPollReTx uehGetPollRetxForOamVal(
    AmRlcTPollRetransmit amPlRtx
    )
{
  return pollRetxForOamVal[amPlRtx];
}
/***********************************************************
 *
 *     Func : uehGetAlphaForOamVal
 *
 *     Desc : Returns the Mapped value in RRC
 *
 *     Ret  :
 *
 *     Notes: None
 *
 *     File : ueh_utils.c
 *
 **********************************************************/
RgrPwrAlpha alphaForOamVal[ALPHA_AL1 + 1]=  
{
  RGR_PWR_ALPHA0, 
  RGR_PWR_ALPHA4, 
  RGR_PWR_ALPHA5, 
  RGR_PWR_ALPHA6, 
  RGR_PWR_ALPHA7, 
  RGR_PWR_ALPHA8, 
  RGR_PWR_ALPHA9, 
  RGR_PWR_ALPHAALL
};

RgrPwrAlpha uehGetAlphaForOamVal(
    Alpha alpha
    )
{
  return alphaForOamVal[alpha];
}
    
/***********************************************************
 *
 *     Func : uehGetPollPduForOamVal
 *
 *     Desc : Returns the Mapped value in RRC
 *
 *     Ret  :
 *
 *     Notes: None
 *
 *     File : ueh_utils.c
 *
 **********************************************************/
const  NhuPollPdu pollPduForOamVal[ AMRLCPOLLPDU_PINFINITY + 1] =
  {
    NHU_POLL_PDU_P4,
    NHU_POLL_PDU_P8,
    NHU_POLL_PDU_P16,
    NHU_POLL_PDU_P32,
    NHU_POLL_PDU_P128,
    NHU_POLL_PDU_P256,
    NHU_POLL_PDU_P384,
    NHU_POLL_PDU_INF
  };

NhuPollPdu uehGetPollPduForOamVal(
    AmRlcPollPDU amPlPdu
    )
{
  return pollPduForOamVal[amPlPdu];
}
/***********************************************************
 *
 *     Func : uehGetPollByteForOamVal
 *
 *     Desc : Returns the Mapped value in RRC
 *
 *     Ret  :
 *
 *     Notes: None
 *
 *     File : ueh_utils.c
 *
 **********************************************************/
const  NhuRlcPollByte pollByteForOamVal[ AMRLCPOLLBYTE_KBINFINITY + 1] =
  {
    NHU_POLL_BYTE_KB25,
    NHU_POLL_BYTE_KB50,
    NHU_POLL_BYTE_KB75,
    NHU_POLL_BYTE_KB100,
    NHU_POLL_BYTE_KB125,
    NHU_POLL_BYTE_KB250,
    NHU_POLL_BYTE_KB375,
    NHU_POLL_BYTE_KB500,
    NHU_POLL_BYTE_KB750,
    NHU_POLL_BYTE_KB1000,
    NHU_POLL_BYTE_KB1250,
    NHU_POLL_BYTE_KB1500,
    NHU_POLL_BYTE_KB2000,
    NHU_POLL_BYTE_KB3000,
    NHU_POLL_BYTE_INF
  };

NhuRlcPollByte uehGetPollByteForOamVal(
    AmRlcPollByte amPlbyte
    )
{
  return pollByteForOamVal[amPlbyte];
}

/***********************************************************
 *
 *     Func : uehGetMaxRetxThreForOamVal
 *
 *     Desc : Returns the Mapped value in RRC
 *
 *     Ret  :
 *
 *     Notes: None
 *
 *     File : ueh_utils.c
 *
 **********************************************************/
const  NhuMaxRetxThres maxRetxThreForOamVal[ AMRLCMAXRETXTHRESHOLD_T32 + 1 ]=
  {
    NHU_MAX_RETX_THRES_T1,
    NHU_MAX_RETX_THRES_T2,
    NHU_MAX_RETX_THRES_T3,
    NHU_MAX_RETX_THRES_T4,
    NHU_MAX_RETX_THRES_T6,
    NHU_MAX_RETX_THRES_T8,
    NHU_MAX_RETX_THRES_T16,
    NHU_MAX_RETX_THRES_T32
  };

NhuMaxRetxThres uehGetMaxRetxThreForOamVal(
    AmRlcMaxRetxThreshold maxRetxThrsld
    )
{
  return maxRetxThreForOamVal[maxRetxThrsld];
}

/***********************************************************
 *
 *     Func : uehGetTReordForOamVal
 *
 *     Desc : Returns the Mapped value in RRC
 *
 *     Ret  :
 *
 *     Notes: None
 *
 *     File : ueh_utils.c
 *
 **********************************************************/
const  NhuTReOrdering tReordForOamVal[ AMRLCTREORDERING_MS200 + 1] =
  {
    NHU_TREORD_MS0,
    NHU_TREORD_MS5,
    NHU_TREORD_MS10,
    NHU_TREORD_MS15,
    NHU_TREORD_MS20,
    NHU_TREORD_MS25,
    NHU_TREORD_MS30,
    NHU_TREORD_MS35,
    NHU_TREORD_MS40,
    NHU_TREORD_MS45,
    NHU_TREORD_MS50,
    NHU_TREORD_MS55,
    NHU_TREORD_MS60,
    NHU_TREORD_MS65,
    NHU_TREORD_MS70,
    NHU_TREORD_MS75,
    NHU_TREORD_MS80,
    NHU_TREORD_MS85,
    NHU_TREORD_MS90,
    NHU_TREORD_MS95,
    NHU_TREORD_MS100,
    NHU_TREORD_MS110,
    NHU_TREORD_MS120,
    NHU_TREORD_MS130,
    NHU_TREORD_MS140,
    NHU_TREORD_MS150,
    NHU_TREORD_MS160,
    NHU_TREORD_MS170,
    NHU_TREORD_MS180,
    NHU_TREORD_MS190,
    NHU_TREORD_MS200
  };

NhuTReOrdering uehGetTReordForOamVal(
    AmRlcTReordering amTreord
    )
{
  return tReordForOamVal[amTreord];
}

/***********************************************************
 *
 *     Func : uehGetStatProhForOamVal
 *
 *     Desc : Returns the Mapped value in RRC
 *
 *     Ret  :
 *
 *     Notes: None
 *
 *     File : ueh_utils.c
 *
 **********************************************************/
const  NhuTStatusProh statProhForOamVal[ AMRLCTSTATUSPROHIBIT_MS500 + 1] =
  {
    NHU_STATUS_PROH_MS5,
    NHU_STATUS_PROH_MS10,
    NHU_STATUS_PROH_MS15,
    NHU_STATUS_PROH_MS20,
    NHU_STATUS_PROH_MS25,
    NHU_STATUS_PROH_MS30,
    NHU_STATUS_PROH_MS35,
    NHU_STATUS_PROH_MS40,
    NHU_STATUS_PROH_MS45,
    NHU_STATUS_PROH_MS50,
    NHU_STATUS_PROH_MS55,
    NHU_STATUS_PROH_MS60,
    NHU_STATUS_PROH_MS65,
    NHU_STATUS_PROH_MS70,
    NHU_STATUS_PROH_MS75,
    NHU_STATUS_PROH_MS80,
    NHU_STATUS_PROH_MS85,
    NHU_STATUS_PROH_MS90,
    NHU_STATUS_PROH_MS95,
    NHU_STATUS_PROH_MS100,
    NHU_STATUS_PROH_MS105,
    NHU_STATUS_PROH_MS110,
    NHU_STATUS_PROH_MS115,
    NHU_STATUS_PROH_MS120,
    NHU_STATUS_PROH_MS125,
    NHU_STATUS_PROH_MS130,
    NHU_STATUS_PROH_MS135,
    NHU_STATUS_PROH_MS140,
    NHU_STATUS_PROH_MS145,
    NHU_STATUS_PROH_MS150,
    NHU_STATUS_PROH_MS155,
    NHU_STATUS_PROH_MS160,
    NHU_STATUS_PROH_MS165,
    NHU_STATUS_PROH_MS170,
    NHU_STATUS_PROH_MS175,
    NHU_STATUS_PROH_MS180,
    NHU_STATUS_PROH_MS185,
    NHU_STATUS_PROH_MS190,
    NHU_STATUS_PROH_MS195,
    NHU_STATUS_PROH_MS200,
    NHU_STATUS_PROH_MS205,
    NHU_STATUS_PROH_MS210,
    NHU_STATUS_PROH_MS215,
    NHU_STATUS_PROH_MS220,
    NHU_STATUS_PROH_MS225,
    NHU_STATUS_PROH_MS230,
    NHU_STATUS_PROH_MS235,
    NHU_STATUS_PROH_MS240,
    NHU_STATUS_PROH_MS245,
    NHU_STATUS_PROH_MS250,
    NHU_STATUS_PROH_MS300,
    NHU_STATUS_PROH_MS350,
    NHU_STATUS_PROH_MS400,
    NHU_STATUS_PROH_MS450,
    NHU_STATUS_PROH_MS500
  };

NhuTStatusProh uehGetStatProhForOamVal (
    AmRlcTStatusProhibit amStatProh
    )
{
  return statProhForOamVal[amStatProh];
}
/***********************************************************
 *
 *     Func : uehGetDiscTmrForOamVal
 *
 *     Desc : Returns the Mapped value in RRC
 *
 *     Ret  :
 *
 *     Notes: None
 *
 *     File : ueh_utils.c
 *
 **********************************************************/
const  NhuPdcpDiscardTmr discTmrForOamVal[ DISCARDTIMER_INFINITY + 1 ]=
  {
    NHU_PDCP_DISCARD_TMR_MS50,
    NHU_PDCP_DISCARD_TMR_MS100,
    NHU_PDCP_DISCARD_TMR_MS150,
    NHU_PDCP_DISCARD_TMR_MS300,
    NHU_PDCP_DISCARD_TMR_MS500,
    NHU_PDCP_DISCARD_TMR_MS750,
    NHU_PDCP_DISCARD_TMR_MS1500,
    NHU_PDCP_DISCARD_TMR_MSINF
  };

NhuPdcpDiscardTmr uehGetDiscTmrForOamVal (
    DiscardTimer disTmr
    )
{
  return discTmrForOamVal[disTmr];
}
/***********************************************************
 *
 *     Func : uehGetMaxHargTxForOamVal
 *
 *     Desc : Returns the Mapped value in RRC
 *
 *     Ret  :
 *
 *     Notes: None
 *
 *     File : ueh_utils.c
 *
 **********************************************************/
const  NhuMaxHarqTx maxHargTxForOamVal[MAXULHARQTX_N28 + 1] =
  {
    NHU_MAX_HARQ_TX_N1,
    NHU_MAX_HARQ_TX_N2,
    NHU_MAX_HARQ_TX_N3,
    NHU_MAX_HARQ_TX_N4,
    NHU_MAX_HARQ_TX_N5,
    NHU_MAX_HARQ_TX_N6,
    NHU_MAX_HARQ_TX_N7,
    NHU_MAX_HARQ_TX_N8,
    NHU_MAX_HARQ_TX_N10,
    NHU_MAX_HARQ_TX_N12,
    NHU_MAX_HARQ_TX_N16,
    NHU_MAX_HARQ_TX_N20,
    NHU_MAX_HARQ_TX_N24,
    NHU_MAX_HARQ_TX_N28
  };

NhuMaxHarqTx uehGetMaxHargTxForOamVal(
    MaxUlHarqTx mxHarqTx
    )
{
  return maxHargTxForOamVal[mxHarqTx];
}

U32 uehGetUeRespTmrInMsFromEnum[]={50,100,200,500,1000,2000,5000};
U32 uehGetUeRlfDetectionTmrInMsFromEnum[]={0,500,1000,2000,5000,10000,20000,30000,40000,50000};

U32 uehGetTimerValFrmEnum[]={50,100,200,300,500,1000,2000,3000,5000,10000,30000,60000,120000};

U16 uehGetActualDefaultPagCycle[] = {32,64,128,256};
U8  uehGetActualModiPriodCoeff[]  = {2,4,8,16};
/* maxHARQ-Tx			ENUMERATED {
 *											n1, n2, n3, n4, n5, n6, n7, n8,
 *											n10, n12, n16, n20, n24, n28,
 *											spare2, spare1} 
 */
U8  uehGetMaxUlHarqTx[] = {1, 2, 3, 4, 5, 6, 7, 8, 10, 12, 16, 20, 24, 28};

/* T-Reordering ::=					ENUMERATED {
 *									ms0, ms5, ms10, ms15, ms20, ms25, ms30, ms35,
 *									ms40, ms45, ms50, ms55, ms60, ms65, ms70,
 *									ms75, ms80, ms85, ms90, ms95, ms100, ms110,
 *									ms120, ms130, ms140, ms150, ms160, ms170,
 *									ms180, ms190, ms200, spare1}
 */
U8  uehTReordring[] = {0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, \
                       70, 75, 80, 85, 90, 95, 100, 110, 120, 130, 140, \
                       150, 160, 170, 180, 190, 200};

/* SN-FieldLength ::=		ENUMERATED {size5, size10}*/
U8 uehSnFieldLen[] = {5, 10};

/* T-StatusProhibit ::=				ENUMERATED {
 *									ms0, ms5, ms10, ms15, ms20, ms25, ms30, ms35,
 *									ms40, ms45, ms50, ms55, ms60, ms65, ms70,
 *									ms75, ms80, ms85, ms90, ms95, ms100, ms105,
 *									ms110, ms115, ms120, ms125, ms130, ms135,
 *									ms140, ms145, ms150, ms155, ms160, ms165,
 *									ms170, ms175, ms180, ms185, ms190, ms195,
 *									ms200, ms205, ms210, ms215, ms220, ms225,
 *									ms230, ms235, ms240, ms245, ms250, ms300,
 *									ms350, ms400, ms450, ms500, spare8, spare7,
 *									spare6, spare5, spare4, spare3, spare2,
 *									spare1}
 */                    
U16 uehTStatusProhibit[] = {0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55,
                            60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 
                            115, 120, 125, 130, 135, 140, 145, 150, 155, 
                            160, 165, 170, 175, 180, 185, 190, 195, 200,
                            205, 210, 215, 220, 225, 230, 235, 240, 245, 
                            250, 300, 350, 400, 450, 500};
//BS-1664
U8  uehGetCqiPeriodicity[] = {2, 5, 10, 20, 40, 80, 160, 0, 32, 64, 128, 0};
U8  uehGetSriPeriodicity[] = {5, 10, 20, 40, 80};


#if 0
/***********************************************************
 *
 *     Func : uehHdlTmrInd
 *
 *     Desc : Call Trace buffer is flushed if configured flush-period is reached.
 *
 *     Ret  :
 *
 *     Notes: None
 *
 *     File : ueh_utils.c
 *
 **********************************************************/
PUBLIC S16 uehHdlTmrInd(
 RgrTtiIndInfo *tmngInfo
)
{
  logFunc("%s", "uehHdlTmrInd");
  /* Updating SFN */
  uehGetCellCb()->sfn = tmngInfo->sfn % UEH_MAX_SFN;

  /* Akash: Call trace - start */
  /* Possible improvement: If calTrcBuf was flushed recently then let it fill 
   * For this, we will need separate timer for callTrc feature only.
   * This can be implemented later */
  if(tmngInfo->sfn % CALLTRCFLUSHPERIOD == 0)
  {
    /* flush the call trace buffer to callTrcFile */
    if(strlen(uehCb.callTrcBuf) > 0)
    {
      logInfo("%s", "CALLTRC: Flushing call trc...");
      UehFlushTrace();
    }
  }
  /* Akash: Call trace - end */

   RETVALUE(ROK);
} /* uehHdlTmrInd */

#endif
/*
*
*       Fun:  uehSmSelfPstMsg
*
*       Desc:  This function is used by  Layer to self post
*              a message
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_main.c
*
*/

PUBLIC S16 uehSmSelfPstMsg
(
Pst   *pst
)
{
   Buffer *mBuf;          /* message buffer */

   logFunc("%s", "uehSmSelfPstMsg");

   if(SGetMsg(DFLT_REGION, UEH_POOL, &mBuf) != ROK)
   {
      UEH_ERROR_SSI_EVENT_SEND_FAILED( pst, EVENTTYPE_OTHER, NOT_APPLICABLE, NOT_APPLICABLE);
      RETVALUE(RFAILED);
   } /* end of if statement */

   RETVALUE(SPstTsk(pst, mBuf));

} /* end of uehSmSelfPstMsg */
