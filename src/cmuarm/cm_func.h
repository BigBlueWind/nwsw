/*
 * cm_func.h
 *
 *  Created on: 16-Feb-2011
 *      Author: mkata
 */

#ifndef CM_FUNC_H_
#define CM_FUNC_H_

#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include <sys/time.h>
#include "bs_tables.h"


#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timer defines */
#include "cm_tkns.h"       /* common tokens defines */
#include "cm_mblk.h"       /* common memory allocation library defines */
#include "cm_llist.h"      /* common link list  defines  */
#include "cm_hash.h"       /* common hash list  defines */
#include "cm_lte.h"        /* common LTE defines */
#include "ckw.h"           /* CKW defines */

/* extern (.x) include files */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */

#include "cm5.x"           /* common timer library */
#include "cm_tkns.x"       /* common tokens */
#include "cm_mblk.x"       /* common memory allocation */
#include "cm_llist.x"      /* common link list */
#include "cm_hash.x"       /* common hash list */
#include "cm_lte.x"        /* common LTE includes */
#include "cm_lib.x"        /* common memory allocation library */
#include "ckw.x"           /* CKW */

EXTERN  S16 rdConQ ARGS((Data data));
void fillDataAndTimeStampField(UINT8   *dataAndTimeStampField);
PUBLIC S16 SRegInfoShow(Region region,U32 *availmem);

#endif /* CM_FUNC_H_ */


