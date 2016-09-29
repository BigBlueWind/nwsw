/**
 * @file    lueh.h
 * @brief API for User Equipment Handler (Abbrev: UEH)
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author         : Amar Ramteke
 * Date           : 14th Sept 2010
 * Change History :
 */


/**********************************************************************

     Name:     LTE-eNodeB APP
  
     Type:    
  
     Desc:     

     Ret :     ROK - success
               RFAILED - failure

     File:     lueh.h

     
**********************************************************************/

#ifndef __LUEHH__
#define __LUEHH__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Interface Events */
#define EVTLUEHCFGREQ    1       /* Config request */
#define EVTLUEHCFGCFM    2       /* Config Confirm */
#define EVTLUEHCNTRLREQ  3       /* Control Request */
#define EVTLUEHCNTRLCFM  4       /* Control Confirm */
#define EVTLUEHSTAIND    5       /* Status Indication */

#ifdef LTE_L2_MEAS
#define EVTLUEHMACL2MEASCFM  6
#define EVTLUEHRLCL2MEASCFM  7
#define EVTLUEHPDCPL2MEASCFM 8
#endif
#define EUEHXXX 0
   


/**********************************************************************
 cause field values in cmAlarm
**********************************************************************/
#define LUEH_CAUSE_NHU_SAP_BOUND          (LCM_CAUSE_LYR_SPECIFIC + 1)  
#define LUEH_CAUSE_CTF_SAP_BOUND          (LCM_CAUSE_LYR_SPECIFIC + 2)  
#define LUEH_CAUSE_RGR_SAP_BOUND          (LCM_CAUSE_LYR_SPECIFIC + 3)  
#define LUEH_CAUSE_SZT_SAP_BOUND          (LCM_CAUSE_LYR_SPECIFIC + 4)  
#define LUEH_CAUSE_UDH_SAP_BOUND          (LCM_CAUSE_LYR_SPECIFIC + 5)  

#define LUEH_EVENT_CELL_CFG_SUCCESS       (LCM_CAUSE_LYR_SPECIFIC + 1)  
#define LUEH_EVENT_CELL_CFG_FAILED        (LCM_CAUSE_LYR_SPECIFIC + 2)  
#define LUEH_EVENT_S1CON_SET_SUCCESS       (LCM_CAUSE_LYR_SPECIFIC + 3)  
#define LUEH_EVENT_S1CON_SET_FAIL        (LCM_CAUSE_LYR_SPECIFIC + 4)  



   
/*----------------- State Of SAP ------------------*/

#define LUEH_SAP_UNBOUND             1
#define LUEH_SAP_BINDING             2
#define LUEH_SAP_BOUND               3

#define ELUEHXXX            0

#define STUEHGEN            1
#define STUEHRGRSAP         2
#define STUEHNHUSAP         3
#define STUEHCTFSAP         4
#define STUEHSZTSAP         5
#define STUEHUDHSAP         6
#define STUEHCELLCFG        7
#define STUEHS1CONSETUP     8
#define STUEHPRMSTARTIND    9

#define STUEHDRBCFGDONE     99 /* DRB configuration is completed */

/* operations */
#define LUEH_CELLCONFIG  1

#define LUEH_USTA_DGNVAL_SAP  0
#define LUEH_USTA_DGNVAL_CELLUEID  1

#define LUEH_CAUSE_CELL_UP  100
/* operations */
#define LUEH_CELLCONFIG  1

#define ELUEHXXX         0
#define ERRLUEH          0

#define   ELUEH001      (ERRLUEH +    1)
#define   ELUEH002      (ERRLUEH +    2)
#define   ELUEH003      (ERRLUEH +    3)
#define   ELUEH004      (ERRLUEH +    4)
#define   ELUEH005      (ERRLUEH +    5)
#define   ELUEH006      (ERRLUEH +    6)
#define   ELUEH007      (ERRLUEH +    7)
#define   ELUEH008      (ERRLUEH +    8)
#define   ELUEH009      (ERRLUEH +    9)
#define   ELUEH010      (ERRLUEH +    10)
#define   ELUEH011      (ERRLUEH +    11)
#define   ELUEH012      (ERRLUEH +    12)
#define   ELUEH013      (ERRLUEH +    13)
#define   ELUEH014      (ERRLUEH +    14)
#define   ELUEH015      (ERRLUEH +    15)


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __LUEHH__ */

/********************************************************************

        Revision history:

**********************************************************************/

