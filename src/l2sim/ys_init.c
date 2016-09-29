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

/************************************************************************
 
     Name:     LTE MAC Convergence Layer
  
     Type:     C source file
  
     Desc:     C source code for Entry point fucntions
  
     File:     ys_init.c
  
     Sid:      yw_init.c@@/main/3 - Thu Dec  2 03:31:41 2010
  
     Prg:      pk
  
**********************************************************************/

/** @file ys_init.c
@brief This module acts as an interface handler for upper interface and 
manages Pst and Sap related information for upper interface APIs.
*/

/* header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timers defines */
#include "cm_hash.h"       /* common hash list defines */
#include "cm_llist.h"      /* common linked list defines */
#include "cm_mblk.h"       /* memory management */
#include "cm_tkns.h"       /* common tokens */
#include "cm_lte.h"
#include "ctf.h"           /* CTF defines */
#include "lys.h"           /* layer management defines for LTE-CL */
#include "tfu.h"
#ifndef NOFILESYS
#include "cm_os.h"         /* file operations */
#endif

#if !(defined SS_CAVIUM || defined SS_4GMX_LCORE)
#include "cm_inet.h"       /* common tokens */
#endif /* SS_CAVIUM */


#ifdef YS_MSPD
#include "LtePhyL2Api.h"
//#include "mlog.h"
#include "apidefs.h"
#include "tcb.h"
//#include "lte_entry.h"
/***************SEC_CHANGE_START*****************/
#ifdef CTF_AS_SEC_ENB
//#include "spaccdrv.h"
#endif
/***************SEC_CHANGE_END*****************/

//#include "appids.h"
#include "ys_ms.h"         /* defines and macros for CL */
//#include "lte_entry.h"
#else
#include "ys.h"            /* defines and macros for CL */
#include <sys/time.h>
#endif
#include <stdlib.h>        /*  ys004.102 :186 : exit warning */
#include "ys_err.h"        /* YS error defines */
#include <stdlib.h>        /* ys004.102 : CR 7528 exit warning */

/* header/extern include files (.x) */

#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"
#include "ctf.x"           /* CTF types */
#include "lys.x"           /* layer management typedefs for CL */
#include "tfu.x"
#ifndef NOFILESYS
#include "cm_os.x"         /* file operations */
#endif

#if !(defined SS_CAVIUM || defined SS_4GMX_LCORE)
#include "cm_inet.x"       /* common tokens */
#endif /* SS_CAVIUM */


#ifdef YS_MSPD
#include "ys_ms.x"            /* typedefs for CL */
#else
#include "ys.x"            /* typedefs for CL */
#endif



#ifdef YS_MSPD
PRIVATE S16 ysMsInit ARGS ((Void));

#endif /*YS_MSPD*/

#define YS_IP_ADDR_STR_LEN        64
#define IID_LTE_MAC      28
#define IID_LTE_MAC_EX      45
#define YS_MAX_CFG_PARAM_LEN      256
#define YS_MAX_CFG_FILE_LINE_LEN  256

#ifndef NOFILESYS
U32  YS_TTI_TMR_VAL_CFG;/* ys004.102 : CR 7531 TTI configured*/
U16 YS_PORT_ENB;
U16 YS_PORT_UE;
U8  YS_IP_ADDR_ENB[YS_IP_ADDR_STR_LEN];
U8  YS_IP_ADDR_UE[YS_IP_ADDR_STR_LEN];
U8 WIRESHARK_SRVR_IP_ADDR[YS_IP_ADDR_STR_LEN]; 

PRIVATE S8 ysCfgParams[][YS_MAX_CFG_PARAM_LEN] = 
{
   "YS_IP_ADDR_ENB",
   "YS_IP_ADDR_UE",
   "YS_PORT_ENB",
   "YS_PORT_UE",
   "YS_TTI_TMR_VAL_CFG", /* ys004.102 : CR 7531 TTI configured */
   "WIRESHARK_SRVR_IP_ADDR" 
};

/**
 * @Configuration file read function.
 *
 * @details
 *
 *     Function : ysInitCfg
 *
 *     This function is called suring the CL's initialization for 
 *     reading the CL configuration file
 *
 *  @param[in]  NONE
 *  @return  S16
 *     -# ROK 
 *     -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16  ysInitCfg
(
Void
)
#else
PUBLIC S16  ysInitCfg (Void)
#endif
{
   S8               line[YS_MAX_CFG_FILE_LINE_LEN];
   OsFile           *fp = NULLP;
   S8               *ret1 = NULLP;
   U32              idx = 0;
   S32              lineNo = 0;
   U8               numParams;
   S8               *lPtr;
   S8               word[YS_MAX_CFG_PARAM_LEN];
   U8               lineLen;
   Bool             inToken = TRUE;

   TRC2(ysInitCfg);

   if((fp = (OsFile*)osFopen("ys_cfg.txt","r")) == (S32)NULLP)
   {
      /* ys004.102 : Defect 117186 */
      /* Configuration file check */
      YSLOGDBGERR(0, 0, "FAILED to open the file ys_cfg.txt\n");
      exit(0);
   } /* if */
   /* ys004.102 Kworks warning fix*/
   numParams = (U8)(sizeof(ysCfgParams)/YS_MAX_CFG_PARAM_LEN);

   lineNo = 1;

   for (;;)
   {
      cmMemset((U8*)line, 0, YS_MAX_CFG_FILE_LINE_LEN);

      ret1 = osFgets(line, YS_MAX_CFG_FILE_LINE_LEN, fp);

      if (ret1 == (S8*)-1 || ret1 == NULLD) /* eof or NULL */
      {
         YS_DBG_ERR((_ysp, "End of File reached, ys_cfg.txt\n"));
         break;
      }
      /* ys004.102 Kworks warnings fix*/
      if (!(lineLen = (U8)(osStripBlanks(line))))
      {
         lineNo++;
         continue;   /* blank line */
      }

      if (*line == '#')
      {
         lineNo++;
         continue;   /* blank line */
      }

      lPtr = line;

      /* Get each of the words from the line */
      while ((lPtr = osGetWord(lPtr, word)) != NULLP)
      {
         if (word[0] == '#')
         {
            break;
         }

         if (inToken)
         {
            /* Compare the word with the list of tokens */
            for (idx = 0; idx < numParams; idx++)
            {
               if(!osStrcmp(word, ysCfgParams[idx]))
               {
                  inToken = FALSE;
                  break;
               }
               if (idx == numParams)
               {
                  YS_DBG_ERR((_ysp, " invalid configuration parameter \n"));
                  if(fp) osFclose(fp);
                  RETVALUE(RFAILED);
               }
            }/*ys004.102 Kworks warning fix*/
         }
         else
         {
            /* Now based on the index store the configuration values */
            switch(idx)
            {
               case 0: /* YS_IP_ADDR_ENB */
               {
                  osStrcpy((S8*)YS_IP_ADDR_ENB, word);
                  break;
               }
               case 1: /* YS_IP_ADDR_UE */
               {
                  osStrcpy((S8*)YS_IP_ADDR_UE, word);
                  break;
               }
               case 2: /* YS_PORT_ENB */
               {
                  YS_PORT_ENB = (U16)osStrtol(word, NULL, 10);
                  break;
               }
               case 3: /* YS_PORT_UE */
               {
                  YS_PORT_UE = (U16)osStrtol(word, NULL, 10);
                  break;
               }
               case 4:/* ys004.102 : CR 7531 TTI configured */ 
               {
                  YS_TTI_TMR_VAL_CFG = (U32)osStrtol(word, NULL, 10);
                  break;
               }
               case 5: /* added case 5 */ 
               {
                   osStrcpy((S8*)WIRESHARK_SRVR_IP_ADDR, word);
                   break;
               }


               default:
               {
                  break;
               }
            }
            inToken = TRUE;
         }

         cmMemset((U8*)word, 0, YS_MAX_CFG_PARAM_LEN);

      }
   } /* End of for loop */

   if(fp) osFclose(fp);

   RETVALUE(ROK);
}
#endif
#ifdef MSPD
//extern void SetMacInstanceId(UINT32 MacId);
extern U32 YsLiMsgHandler (Void * ysCtx, Void * msg);
static U32  ysMsgList = 0;
#endif

/**
 * @brief Task Initiation callback function. 
 *
 * @details
 *
 *     Function : ysActvInit
 *     
 *     This function is supplied as one of parameters during CL's 
 *     task registration. SSI will invoke this function once, after
 *     it creates and attaches this Tapa Task to a system task.
 *     
 *  @param[in]  Ent entity, the entity Id of this task.     
 *  @param[in]  Inst inst, the instance Id of this task.
 *  @param[in]  Region region, the region Id registered for memory 
 *              usage of this task.
 *  @param[in]  Reason reason.
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 ysActvInit
(
Ent entity,            /* entity */
Inst inst,             /* instance */
Region region,         /* region */
Reason reason          /* reason */
)
#else
PUBLIC S16 ysActvInit(entity, inst, region, reason)
Ent entity;            /* entity */
Inst inst;             /* instance */
Region region;         /* region */
Reason reason;         /* reason */
#endif
{
   S16       ret;

   TRC2(ysActvInit);

   /* Initialize the CL TskInit structure to zero */
   cmMemset ((U8 *)&ysCb, 0, sizeof(ysCb));

   /* Initialize the CL TskInit with received values */
   ysCb.ysInit.ent = entity;
   ysCb.ysInit.inst = inst;
   ysCb.ysInit.region = region;
#ifdef MSPD
   ysCb.ysInit.pool = 0;
#else
   ysCb.ysInit.pool = 3; /* Changing from 0 to 3 for performance */
#endif
   ysCb.ysInit.reason = reason;
   ysCb.ysInit.cfgDone = FALSE;
   ysCb.ysInit.acnt = FALSE;
   ysCb.ysInit.usta = FALSE;
   ysCb.ysInit.trc = FALSE;
   ysCb.trcLen = 0; 
#ifdef DEBUGP
#ifdef MSPD
   ysCb.ysInit.dbgMask = 0x0 ;
   ysCb.ysInit.dbgMask = YS_DBGMASK_FATAL;
#else
#ifdef YS_DEBUG
   ysCb.ysInit.dbgMask = 0xffffffff; 
#endif
#endif
#endif /* DEBUGP */
   ysCb.ysInit.procId = SFndProcId();

   /* Initialize CTF sap state */
   ysCb.ctfSap.sapState = LYS_NOT_CFG;

#if 0
#ifndef NOFILESYS
   if ((ysInitCfg()) != ROK)
   {
      RETVALUE (RFAILED);
   }
#endif
#endif


#ifdef  YS_WIRESHARK_LOG
  ysInitLogWireless();
#endif
#ifdef YS_MSPD
   ysMsInit();
   /* changes for new SysCore
    * Need to register a call back function over here
    * Currently passing context as null dont see a usage as of now */
//   ret = SvsrRegMsgHandler (IID_LTE_MAC, YsLiMsgHandler, NULLP);
//   ret = SvsrRegMsgHandler (IID_LTE_MAC_EX, YsLiMsgHandler, &ysMsgList);
   //SetMacInstanceId(IID_LTE_MAC);

   /* Store the post structure */
   ysCb.clPst.dstProcId = SFndProcId();
   ysCb.clPst.srcProcId = SFndProcId();
   ysCb.clPst.dstEnt = ENTTF;
   ysCb.clPst.srcEnt = ENTTF;
   ysCb.clPst.srcInst = 0;
   ysCb.clPst.dstInst = 0;
   ysCb.clPst.route = RTESPEC;
   ysCb.clPst.prior = PRIOR0;
   ysCb.clPst.region = ysCb.ysInit.region;
   ysCb.clPst.pool = ysCb.ysInit.pool;
   ysCb.clPst.selector = 1;
   ysCb.clPst.event = 0;

#if 0
/***************SEC_CHANGE_START*****************/
#ifdef CTF_AS_SEC_ENB
   /*Added SPAcc initialisation for MSPD accelerators */
   ret = SPAccDrvInit();
   if(ret == ROK)
   {
       MSPD_DBG("\nSPACC Driver Inititalised Successfully\n");
   }
#endif
/***************SEC_CHANGE_END*****************/

#endif

#endif /* YS_MSPD */


   RETVALUE(ROK);
} /* ysActvInit */
#ifdef MSPD
#define PID_MAC_PROCESSING_THREAD   30000

U32 MacGetTick(void)
{
    return *(volatile U32 *)0xFE050004;
}

#if 0
PUBLIC U32 YsLiMsgHandler (Void * ysCtx, Void * msg)
{

   U32 size = 0;
   static Bool firstMsg = TRUE;
   volatile int t = MacGetTick();
   int t2;
   YsCellCb	*cellCb;
   U32       isList = ysCtx == (void *)&ysMsgList ? TRUE : FALSE;
   //uart_printf("YsLiMsgHandler %x\n", *(U32*)msg);
   /* ysCtx is not used ignore */

   if (firstMsg)
   {
      MxSetThreadPriority(MxGetCurrentThread(),0);
      firstMsg = FALSE;
   }

   MacDispatch(isList, (PTR)msg);
#ifdef MSPD_MLOG_NEW
   MLogTask(PID_CL_PHY_MSG, RESOURCE_LARM, t, t2 = MacGetTick());
#endif

   RETVALUE(SUCCESS);
}
#endif
#endif



#ifdef YS_MSPD
/**
 * @brief Task Initiation callback function.
 *
 * @details
 *
 *     Function : ysMsInit
 *
 *     This function is supplied as one of parameters during CL's
 *     task registration. SSI will invoke this function once, after
 *     it creates and attaches this Tapa Task to a system task.
 *
 *  @param[in]  Ent entity, the entity Id of this task.
 *  @param[in]  Inst inst, the instance Id of this task.
 *  @param[in]  Region region, the region Id registered for memory
 *              usage of this task.
 *  @param[in]  Reason reason.
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PRIVATE S16 ysMsInit
(
Void
)
#else
PRIVATE S16 ysMsInit()
#endif
{
   U16       idx;
   YsCellCb  cellCb;
   U16       offset;
   S16       ret;
   U8        a0, a1, a2, a3, bit, cnt;
   U16       value;

   TRC2(ysMsInit);

   offset = (U16)((PTR)&cellCb.cellHlEnt - (PTR)&cellCb);
   ret = cmHashListInit(&(ysCb.cellCfgLst),     /* hash list Cp */
            YS_MAX_CELL_HASH_BINS,        /* HL bins */
            offset,                             /* Offset of HL Entry */
            FALSE,                              /* Allow dup. keys ? */
            CM_HASH_KEYTYPE_DEF,                /* HL key type */
            ysCb.ysInit.region,                 /* Mem region for HL */
            ysCb.ysInit.pool);                  /* Mem pool for HL */

   if(ret != ROK)
   {
      YS_DBG_ERR((_ysp, "cmHashListInit: cellHLst failed \n"));
      RETVALUE(RFAILED);
   }

   /* Initialize timing Info */
   cellCb.timingInfo.subframe = -1;
   cellCb.timingInfo.sfn      = 0;

#if 0
   /* initialize SR Configuration DB */
   for(idx = 0; idx < YS_NUM_SR_CFG; idx++)
   {
      /* idx value can not go beyond 255. No loss of precission */
      ysSrCfgDb[idx].srCfgIndex = idx;

      if(idx <= 4)
      {
         ysSrCfgDb[idx].srPeriod = 5;
         ysSrCfgDb[idx].srSfOffSet = idx;
      }
      else if (idx >= 5 && idx <= 14)
      {
         ysSrCfgDb[idx].srPeriod = 10;
         ysSrCfgDb[idx].srSfOffSet = idx - 5;
      }
      else if (idx >= 15 && idx <= 34)
      {
         ysSrCfgDb[idx].srPeriod = 20;
         ysSrCfgDb[idx].srSfOffSet = idx - 15;
      }
      else if (idx >= 35 && idx <= 74)
      {
         ysSrCfgDb[idx].srPeriod = 40;
         ysSrCfgDb[idx].srSfOffSet = idx - 35;
      }
      else if (idx >= 75 && idx <= 154)
      {
         ysSrCfgDb[idx].srPeriod = 80;
         ysSrCfgDb[idx].srSfOffSet = idx - 75;
      }
      else if (idx >=155 && idx <=156)
      {
         ysSrCfgDb[idx].srPeriod = 2;
         ysSrCfgDb[idx].srSfOffSet = idx - 155;
      }
      else /*-- if (idx == 157) -- Last entry --*/
      {
         ysSrCfgDb[idx].srPeriod = 1;
         ysSrCfgDb[idx].srSfOffSet = idx - 157;
      }
   }

   /* initialize SRS Configuration DB */
   /* 3GPP Spec 36.213 Table 8.2-1 */
   for(idx = 0; idx < YS_NUM_SRS_CFG; idx++)
   {
      ysSrsCfgDb[idx].srsCfgIndex = idx;

      if(idx <= 1)
      {
         ysSrsCfgDb[idx].srsPeriod = 2;
         ysSrsCfgDb[idx].srsSfOffSet = idx;
      }
      else if (idx >= 2 && idx <= 6)
      {
         ysSrsCfgDb[idx].srsPeriod = 5;
         ysSrsCfgDb[idx].srsSfOffSet = idx - 2;
      }
      else if (idx >= 7 && idx <= 16)
      {
         ysSrsCfgDb[idx].srsPeriod = 10;
         ysSrsCfgDb[idx].srsSfOffSet = idx - 7;
      }
      else if (idx >= 17 && idx <= 36)
      {
         ysSrsCfgDb[idx].srsPeriod = 20;
         ysSrsCfgDb[idx].srsSfOffSet = idx - 17;
      }
      else if (idx >= 37 && idx <= 76)
      {
         ysSrsCfgDb[idx].srsPeriod = 40;
         ysSrsCfgDb[idx].srsSfOffSet = idx - 37;
      }
      else if (idx >= 77 && idx <= 156)
      {
         ysSrsCfgDb[idx].srsPeriod = 80;
         ysSrsCfgDb[idx].srsSfOffSet = idx - 77;
      }
      else if (idx >= 157 && idx <= 316)
      {
         ysSrsCfgDb[idx].srsPeriod = 160;
         ysSrsCfgDb[idx].srsSfOffSet = idx - 157;
      }
      else /*-- if (idx >= 317 && idx <= 636) Single Entry */
      {
         ysSrsCfgDb[idx].srsPeriod = 320;
         ysSrsCfgDb[idx].srsSfOffSet = idx - 317;
      }
   }

   /* initialize Cqi-Pmi Configuration DB */
   /* 3GPP Spec 36.213 7.2.2-1A */
   for(idx = 0; idx < YS_NUM_CQI_CFG; idx++)
   {
      ysCqiPmiCfgDb[idx].cqiPmiCfgIndex = idx;

      if(idx >= 0 && idx <= 2)
      {
         ysCqiPmiCfgDb[idx].cqiPeriod = 2;
         ysCqiPmiCfgDb[idx].cqiSfOffSet = idx;
      }
      else if (idx >= 2 && idx <= 6)
      {
         ysCqiPmiCfgDb[idx].cqiPeriod = 5;
         ysCqiPmiCfgDb[idx].cqiSfOffSet = idx - 2;
      }
      else if (idx >= 7 && idx <= 16)
      {
         ysCqiPmiCfgDb[idx].cqiPeriod = 10;
         ysCqiPmiCfgDb[idx].cqiSfOffSet = idx - 7;
      }
      else if (idx >= 17 && idx <= 36)
      {
         ysCqiPmiCfgDb[idx].cqiPeriod = 20;
         ysCqiPmiCfgDb[idx].cqiSfOffSet = idx - 17;
      }
      else if (idx >= 37 && idx <= 76)
      {
         ysCqiPmiCfgDb[idx].cqiPeriod = 40;
         ysCqiPmiCfgDb[idx].cqiSfOffSet = idx - 37;
      }
      else if (idx >= 77 && idx <= 156)
      {
         ysCqiPmiCfgDb[idx].cqiPeriod = 80;
         ysCqiPmiCfgDb[idx].cqiSfOffSet = idx - 77;
      }
      else if (idx >= 157 && idx <= 316)
      {
         ysCqiPmiCfgDb[idx].cqiPeriod = 160;
         ysCqiPmiCfgDb[idx].cqiSfOffSet = idx - 157;
      }
      else if (idx == 317)
      {
         ysCqiPmiCfgDb[idx].cqiPeriod = 0;
         ysCqiPmiCfgDb[idx].cqiSfOffSet = 0;
      }
      else if (idx >= 318 && idx <= 349)
      {
         ysCqiPmiCfgDb[idx].cqiPeriod = 32;
         ysCqiPmiCfgDb[idx].cqiSfOffSet = idx - 318;
      }
      else if (idx >= 350 && idx <= 413)
      {
         ysCqiPmiCfgDb[idx].cqiPeriod = 64;
         ysCqiPmiCfgDb[idx].cqiSfOffSet = idx - 350;
      }
      else if (idx >= 414 && idx <= 541)
      {
         ysCqiPmiCfgDb[idx].cqiPeriod = 128;
         ysCqiPmiCfgDb[idx].cqiSfOffSet = idx - 414;
      }

   }

   /* Compute and fill the cqiTo20BitMap array
   */
   for(cnt =0; cnt < 16; cnt++)
   {
      a0 = cnt & 0x00;
      a1 = cnt & 0x02;
      a2 = cnt & 0x04;
      a3 = cnt & 0x08;
      value = 0;
      bit = 0;
      for(idx =0; idx < 20; idx++)
      {
         bit = (a0 * MSeq[idx][0] + a1 * MSeq[idx][1] + a2 * MSeq[idx][2] + \
               a3 * MSeq[idx][3]) % 2;
         value = (value << 1) | bit;
      } /* end of inner for loop */
      cqiTo20BitMap[cnt][1] = value;
   } /* end of outer for loop */
#endif

   RETVALUE(ROK);
} /* ysMsInit */


#endif /* YS_MSPD */

/********************************************************************30**
  
         End of file:     yw_init.c@@/main/3 - Thu Dec  2 03:31:41 2010
  
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
----------- -------- ---- -----------------------------------------------
/main/2      ---     sgm        1. eNodeB 1.2 release
/main/2   ys003.102  sgm        1.Fix for CID:1622-02-01 DefectId:ccpu00115333
/main/2   ys004.102  ms         1.configuration file check.                                   
                                2.TTI configured.
                     pkd        3.Kworks warnings fix.
                     pkd        4.Cell Delete functionality CRID:ccpu00117556
/main/2   ys005.102  psb        1.TIC_ID :ccpu00117545 Fixed warning for gcc compilation on CentOS
*********************************************************************91*/
