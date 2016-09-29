/**
 * @file    udh_lmm.c
 * @brief This file contains the management interface functions for UDH module
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Madhu K, Bhogeswara M; 02May2011 :
 *     a) Created initial version
 */

#include "udh_includes.h"
/*
 *  Fun : udhActvInit
 *  Desc: The task initialise its UDH global variables.
 *  Ret : ROK   - ok
 *  File: udh_lm.c
 */

PUBLIC S16 udhActvInit
(
  Ent    entity,    /* Entity   */
  Inst   inst,      /* Instance */
  Region region,    /* Region   */
  Reason reason     /* reason   */
)
{
  UdhCb  *udhCb = getUdhCb();
  memset(udhCb, 0, sizeof(UdhCb));

  udhCb->uehSap.sapCfg.pst.srcProcId = SFndProcId();
  udhCb->uehSap.sapCfg.pst.srcEnt    = entity;
  udhCb->uehSap.sapCfg.pst.srcInst   = inst;
  udhCb->uehSap.sapState             = SAP_NOT_CONFIGURED;
  udhCb->uehSap.sapCfg.pst.region    = DFLT_REGION;
  udhCb->uehSap.sapCfg.pst.pool      = DFLT_POOL;
  udhCb->smSap.sapState = SAP_NOT_CONFIGURED;
  /* initialise pointers used to store UE information  */
  //udhInitUeCb();
  logInfo("%s","udhActvInit: UDH Active Init is success");
  RETVALUE(ROK);
} /* udhActvInit */

