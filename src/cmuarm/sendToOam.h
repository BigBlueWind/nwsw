/**
 * @file     sendToOam.h
 * @details  Copyright Powerwave Technologies, Inc., 2011
 * @brief
 *
 * @author    Jyotirling Ugaji
 * @date:     22/12/2011
 * @details:  Initial Version
 **/

#ifndef   TOOAM_H
#define   TOOAM_H

#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
//#include "cm_tkns.h"       /* common tokens */
//#include "cm_mblk.h"       /* Memory Link List */
//#include "leg.h"           /* Event structure file */

/* header include files (.x) */
#include"gen.x"           /* general layer */
#include"ssi.x"           /* system services */

PUBLIC S16 sendEventToOam (
  Ent   srcEnt,
  Ent   dstEnt,
  Event event,
  MsgLen   len,
  U8    *data);

#endif /*TOOAM_H*/

U32 getProcIdByEntityId(Ent entityId );
