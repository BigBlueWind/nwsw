//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL initcpu.h
 *
 * @brief This file contains definition of inter-cpu API used in application
 * @author Mindspeed Technologies
 * @version $Revision: 1.12 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif


#ifndef INITCPU_H_
#define INITCPU_H_

#include "4gmx.h"
#include "syscorecfg.h"
#include "mmumgr.h"

#define RC_SC_PARAM_ERROR       DEF_USRRC(APP_MODULE, SC_LIBRARY, 1)
#define RC_SC_UNKNOWN_SECTION   DEF_USRRC(APP_MODULE, SC_LIBRARY, 2)
#define RC_SC_MMU_TABLE_ERROR   DEF_USRRC(APP_MODULE, SC_LIBRARY, 3)
#define RC_SC_MMU_TIMEOUT       DEF_USRRC(APP_MODULE, SC_LIBRARY, 4)

typedef MXRC (*ExtAppCallback) (void);

typedef struct _SYSCORE_PARAM_
{
    UINT32          Config;                 // reserved
    ExtAppCallback  app_entry [CPU_NUM];    // application entry points for each core
    MmuCfg          mmu_cfg [CPU_NUM];      // application mmu configuration entry points for each core

}SyscoreParam;

typedef struct _SYSCORE_MEM_AP_
{
    UINT32 Base;
    UINT32 Size;
    UINT32 nAccess;
    VUINT32 Status;
    UINT32 Rc;

}SYSCORE_MEM_AP;

#define SECTION_RO_CODE             1
#define SECTION_RW_CODE             2
#define SECTION_EXT_ICPU            3
#define SECTION_PRV_CORE0           4
#define SECTION_APP_HEAP            5
#define SECTION_NCNB                6

typedef struct _SECTION_INFO_
{
    UINT32 Base;
    UINT32 Size;

}SECTINFO;

#define SYSCORE_MEM_NO_ACCESS       1
#define SYSCORE_MEM_RO_ACCESS       2
#define SYSCORE_MEM_RW_ACCESS       3
#define SYSCORE_MEM_DCACHED         (1 << 16)

/** @brief This function is an entry point of syscore library
            and should be called first before usage of any system API


        @NOTE  When this function calls cpu0 or cpu1 callbacks
               the system is initialized and application may use free
               any system API commands


    @param pParam [in] - the input parameters

    @return [MXRC] an error code */

MXRC SysCoreStart(SyscoreParam * pParam);

/** @brief This function checks MMU table and returns a status for the memory page
            is D-CACHE enabled or not

    @param nAddr [in] - the page address

    @return [BOOL] the status */

BOOL SysCoreIsDCached (UINT32 nAddr);

/** @brief This function returns number of CPUs controlled by the library

    @return [UINT32] the number of CPUs */

UINT32 SysGetCpuNum (void);

/** @brief This function returns information about specific code section

    @param nID  [in] - the section ID
    @param pInfo[out]- information
    @return [MXRC] an error code */

MXRC SysCoreGetSectInfo (UINT32 nID, SECTINFO * pInfo);

/** @brief This function sets memory access to the memory region

    @param nCpuMask [in] - CPU mask
    @param Base     [in] - the base address
    @param nSize    [in] - the size in bytes
    @param nAccess  [in] - the access flags, see: 4GMX enum <MMU_AP>

    @return [MXRC] an error code */

MXRC SysCoreMemAccess (UINT32 nCpuMask, UINT32 Base, UINT32 nSize, UINT32 nAccess);

#endif /*INITCPU_H_*/

#ifdef __cplusplus
}
#endif

