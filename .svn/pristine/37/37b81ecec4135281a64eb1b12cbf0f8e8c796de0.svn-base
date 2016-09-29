/**
 * @file    start_indication.h
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains the START_IND message structure
 * @author : Ramprasad Reddy
 * Date   : 26 May 2011
 */

#ifndef START_INDICATION_H_
#define START_INDICATION_H_

#include "basic_types.h" /* UINT16, UINT8  */
#include "bs_modules.h" /* ModuleId */
#include "msg_ids.h" /* MsgId  */

typedef struct s_MsgStartInd
{
  UINT32 svnVersion;
}MsgStartInd;

void sendStartIndToPrm(
    IN ModuleId moduleId,
    IN MsgId    msgId
    );
UINT32 getSvnVersion(IN ModuleId moduleId);


#endif /* START_INDICATION_H_ */
