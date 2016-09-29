/** Powerwave Defined 'Debug' Counters 
 *  - Dont include any header files here so that it can be used "as is" in lowerarm and upperarm.
 *  - Dont use CounterId type - that type is already used by OAM
 *  - All types have 'D'  in their name to distinguish them from general counters.
 *  -- example: DCounterId, DStats, ...
 */
#ifndef __CTR_ID_H
#define __CTR_ID_H
typedef enum { 

   CTRID_MAC_NUM_CRCERR_RX = 1, 
   CTRID_MAC_NUM_RACH_RX, 
   CTRID_MAC_NUM_RAR_TX, 
   CTRID_MAC_NUM_MSG3_TX, 
   CTRID_MAC_NUM_SDU_TX,           /* MAC -> PHY */
   CTRID_MAC_NUM_SDU_RX,           /* PHY -> MAC */
   CTRID_MAC_NUM_HARQ_FAIL,

   CTRID_RLC_NUM_SDU_RX,    /* from PDCP */ 
   CTRID_RLC_NUM_SDU_TX,    /* to PDPC */ 
   CTRID_RLC_NUM_SDU_DROP,  /* dropped SDU */ 

   CTRID_PDCP_NUM_SDU_RX,   /* from GTPU */ 
   CTRID_PDCP_NUM_SDU_TX,   /* to GTPU */ 
   CTRID_PDCP_NUM_SDU_DROP, /* dropped SDU */ 

   CTRID_ICPU_NUM_PKTS_RX, 
   CTRID_ICPU_NUM_PKTS_TX, 

   CTRID_MAC_DL_RATE, 
   CTRID_MAC_UL_RATE, 

   CTRID_RLC_DL_RATE, 
   CTRID_RLC_UL_RATE, 

   CTRID_PDCP_DL_RATE, 
   CTRID_PDCP_UL_RATE, 

   CTRID_TFU_DL_RATE, 
   CTRID_TFU_UL_RATE,
   CTRID_GTPU_QUEUE_LEN,
   CTRID_UDH_QUEUE_LEN,
   CTRID_TUCL_QUEUE_LEN

} DCounterId; 

typedef struct s_counter {
  DCounterId       id; 
  unsigned int    val; 
} DCounter; 

typedef enum {
  STCMD_START = 1,
  STCMD_STOP
} DStatsCmd;

typedef struct s_DStatsCfgReq { 
   DStatsCmd  cmd;
   unsigned int interval_ms; /* reporting interval */
} DStatsCfgReq; 

typedef struct s_DStatsInd { 
   /* Sfn time at which the report was generated */ 
   unsigned int n; /* number of elements present in the array below */ 
   DCounter counterList[25];
} DStatsInd;

typedef DStatsCfgReq             MsgPmhTfuDStatsCfgReq; /* MSG_TFU_PMH_STATS_CFG_REQ */
typedef DStatsCfgReq             MsgPmhUdhDStatsCfgReq;
typedef DStatsInd                MsgTfuPmhDStatsInd; /* MSG_TFU_PMH_STATS_IND */
typedef DStatsInd                MsgUdhPmhDStatsInd; 

/** Another method of definining counters **/
#if 0  //To Be Discussed
/* Where is the counter measured */

typedef enum { 
  CTRLOC_ICPU = 1,
  CTRLOC_IMC,
  CTRLOC_MEMMGR, /* memory manager */
  CTRLOC_UEH,
  CTRLOC_UDH,
  CTRLOC_PDCP,
  CTRLOC_MAC,
  CTRLOC_RLC,
  CTRLOC_PHY,

  /* Add new elements above this line */
  CTRLOC_MAX

} SCounterEnt;

/* At which point in the entity is the counter collected */
typedef enum {
  CTRPO_USAP = 1, /* upper sap */
  CTRPO_LSAP,     /* lower sap */
  CTRPO_CSAP,     /* control sap */
  CTRPO_LMSAP,    /* layer management / oam sap */
  CTRPO_RECVQ,     /* receive q */
  CTRPO_GEN,      /* somewhere inside */

  /* Add new elements above this line */
  CTRLPO_MAX
} SCounterCollectionPoint;

typedef struct s_SCounterLocation {
  SCounterEnt             ent;
  SCounterCollectionPoint point;
} SCounterLocation;

/* What type of value is it */
typedef enum {
  CTRTYPE_TX_RATE = 1,
  CTRTYPE_RX_RATE, 
  CTRTYPE_PKTS_TX,
  CTRTYPE_PKTS_RX,
  CTRTYPE_PKTS_DROP,
  CTRTYPE_VAL,       /* example to get current value of Q-length, buffer-length */
  CTRTYPE_MAXVAL_RX, /* Max value received */
  CTRTYPE_MAXVAL_TX, /* Max value sent */
  CTRTYPE_MAXVAL,    /* Max value. use where direction is not applicable/meaningful. example Maximum Q length */

  CTRTYPE_MAX /* add new types above */

} SCountedQuantity;

typedef int SCounterId;
typedef struct s_SCounterType {
  SCounterId        id;
  SCounterLocation  location;
  SCountedQuantity  qty;
  char             *desc;
} SCounterType;


typedef struct s_SCounter {
  SCounterId       id;
  unsigned int     value;
} SCounter;

/* countersDefTab:
CounterType counterDefTab[] = {
 { 1, { CTRLOC_ICPU, CTRPO_GEN  }, CTRTYPE_TX_RATE,    "TX Rate at ICPU" },
 { 2, { CTRLOC_ICPU, CTRPO_GEN  }, CTRTYPE_RX_RATE,    "RX Rate at ICPU" },
 { 3, { CTRLOC_MAC,  CTRPO_USAP }, CTRTYPE_PKTS_TX,    "Pkts sent MAC to RLC" },
 { 4, { CTRLOC_RLC,  CTRPO_LSAP }, CTRTYPE_PKTS_RX,    "Pkts recd by RLC from MAC" },
 { 5, { CTRLOC_PDCP, CTRPO_USAP }, CTRTYPE_MAXVAL_RX,  "Max DL SDU len at PDCP" },
 { 6, { CTRLOC_PDCP, CTRPO_USAP }, CTRTYPE_MAXVAL_TX,  "Max UL SDU len at PDCP" },
 { 7, { CTRLOC_PDCP, CTRPO_RECVQ}, CTRTYPE_MAXVAL,     "Max Q length at PDCP" },
 ...
}
*/
#endif
#endif
