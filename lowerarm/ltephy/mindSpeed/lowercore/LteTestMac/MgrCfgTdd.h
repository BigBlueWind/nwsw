#ifdef AC_MSPD_TDD

#ifndef _LTE_MAC_MGR_CFG_TDD_H
#define _LTE_MAC_MGR_CFG_TDD_H



#include "osal.h"
#include "resultcodes.h"
#include "basetypes.h"
#include "LtePhyL2Api_TDD.h"
#include "LteMacTestCfgMgr.h"

RESULTCODE TestMgrProcessConfigSpDlTdd(PDLSUBFRDESC pSubFrame, U8 SpEna, U8 PdschEna);
RESULTCODE TestMgrProcessConfigUlTdd(PULSUBFRDESC pSubFrame);
void setPhichTdd(U8 PhichFactorM);
#endif /*_LTE_MAC_MGR_CFG_TDD_H*/

#endif /*AC_MSPD_TDD*/
