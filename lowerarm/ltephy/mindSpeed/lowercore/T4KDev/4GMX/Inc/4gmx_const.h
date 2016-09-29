//-------------------------------------------------------------------------------------------
/** @file 4gmx_const.h
 *
 * @brief The file contains macros, modules ID and error codes of 4GMX system
 * @author Mindspeed Technologies
 * @version $Revision: 1.17 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#ifndef _4GMX_CONST_H_
#define _4GMX_CONST_H_

#include "4gmx_config.h"
#include "4gmx_compiler.h"

/************************************************************************/
/*          The system's parameters                                     */
/************************************************************************/
#define MX_HIGH_VERSION      0x02
#define MX_LOW_VERSION       0x02


#ifdef __cplusplus
extern "C"
{
#endif
    void MxCreateObjLog(UINT32 procAddr, HANDLE h);
#ifdef __cplusplus
}
#endif



#ifdef MX_CREATE_OBJ_LOG_ENABLED

#define DEF_RADDR UINT32 _mx_ret_addr__ = MX_GET_RADDR

#define MX_CREATE_OBJECT_LOG(h)	MxCreateObjLog(_mx_ret_addr__, h)

#else

#define DEF_RADDR

#define MX_CREATE_OBJECT_LOG(h)

#endif


/************************************************************************/
/*         Debug macros                                                 */
/************************************************************************/

/*******************************************************************/
#ifdef _MX_DEBUG    /* Debug build */
/*******************************************************************/

    #define DEF_ASSERT_LR() UINT32 __mx_assert_ret_addr__ = MX_GET_RADDR

    #ifdef __cplusplus
    extern "C"
    {
    #endif
        void MxAssertHandler(UINT32 rc, const char *filename, UINT32 nRow, UINT32 lr);
    #ifdef __cplusplus
    }
    #endif

    #ifdef _APP_ASSERT_HANDLER
        #define _ASSERT(x)  if ((x) == 0) { MxAssertHandler ((x), __FILE__, __LINE__, ~0);while (1);}
        #define _ASSERT_LR(x)  if ((x) == 0) { MxAssertHandler ((x), __FILE__, __LINE__, __mx_assert_ret_addr__);while (1);}
        #define _ASSERT_RC(x) if ((x) != MX_OK) { MxAssertHandler ((x), __FILE__, __LINE__, ~0);while (1);}
        #define _ASSERT_RC_LR(x) if ((x) != MX_OK) { MxAssertHandler ((x), __FILE__, __LINE__, __mx_assert_ret_addr__);while (1);}
    #else
        #define _ASSERT(x)  if ((x) == 0) { while (1);}
        #define _ASSERT_RC(x) if ((x) != MX_OK) { while (1);}
    #endif

/*******************************************************************/
#else /* Release build */
/*******************************************************************/

    #define DEF_ASSERT_LR()

    #define _ASSERT(x)          ((void)0);
    #define _ASSERT_LR(x)       ((void)0);
    #define _ASSERT_RC(x)       ((void)0);
    #define _ASSERT_RC_LR(x)    ((void)0);

#endif /*_MX_DEBUG*/

#define _ASSERT_PTR_LR(x)   _ASSERT_LR((x) != NULL)
#define _ASSERT_PTR(x)   _ASSERT((x) != NULL)
#define _ASSERT_HANDLE_LR(x) _ASSERT_LR(HANDLE_NOT_NULL((x)))
#define _ASSERT_HANDLE(x) _ASSERT(HANDLE_NOT_NULL((x)))
#define _ASSERT_HANDLE_TYPE_LR(x, type) _ASSERT_LR(HANDLE_GET_TYPE((x)) == (type))
#define _ASSERT_HANDLE_TYPE(x, type) _ASSERT(HANDLE_GET_TYPE((x)) == (type))
/************************************************************************/
/*           The macro of error definition                              */
/************************************************************************/

#define MX_OK                     0

#define DEF_MXRC(mod, err)        (((mod) << 8) | (err) )
#define DEF_EXMXRC(app, mod, err) (((app) << 16)| ((mod) << 8) | (err) )
#define DEF_USRRC(app, mod, err)  (((app) << 16)| ((mod) << 8) | (err) )
#define GET_ERR(err)              ((err      ) & 0xFF)
#define GET_MOD(err)              ((err >>  8) & 0xFF)
#define GET_APP(err)              ((err >> 16) & 0xFF)

#define FAILED(err)               ((err) & 0xFF)
#define SUCCESSFUL(err)              (!((err) & 0xFF))

/************************************************************************/
/*           The list of modules                                        */
/*           MXM_xxx - MX Module                                        */
/************************************************************************/
enum
{

    MXM_KERNEL = 1,
    MXM_MKERNEL,
    MXM_THREAD,
    MXM_MTHREAD,
    MXM_SEMA,
    MXM_EVENT,
    MXM_TIMER,
    MXM_HEAP,
    MXM_PART,
    MXM_QUEUE,
    MXM_DEBUG_TRACE,
    MXM_IC,
    MXM_RHEAP,
    MXM_ARM,
    MXM_ARMMMU,
    MXM_SERV,
    MXM_TRACER
};

#ifndef NULL
#define NULL  (void *)0
#endif /*NULL*/
#ifndef TRUE
#define TRUE  1
#endif /*TRUE*/
#ifndef FALSE
#define FALSE  0
#endif /*FALSE*/
#ifndef CUR_CPU
#define CUR_CPU  0xFFFFFFFF
#endif
#ifndef ROUND
#define ROUND(x, dx)  (((x) + ((dx) - 1) ) & ~((dx) - 1))
#endif

#ifndef INP
#define INP  // It's just for the parameter description
            // (it's used in functions declaration)
#endif

#ifndef OUTP
#define OUTP // It's just for the parameter description
            // (it's used in functions declaration)
#endif

#define MXREG32(x)      (*(volatile unsigned int *)(x))
#define MXREG16(x)      (*(volatile unsigned short*)(x))
#define MXREG8(x)       (*(volatile unsigned char*)(x))


/************************************************************************/
/*   The macros of driver definition table                              */
/************************************************************************/
#ifdef MX_DRV_FRAMEWORK_ENABLED

#define BEGIN_DRV_TABLE(x) MXDRV x [] = {
#define DRIVER(driver, init) { (init),  driver##_init, driver##_config, driver##_read, driver##_write, NULL },

#ifdef MX_NAMING_ENABLED
#define DRIVER_NAME(driver, init, name) { (init),  driver##_init, driver##_config, driver##_read, driver##_write, name },
#endif /*MX_NAMING_ENABLED */

#define END_DRV_TABLE()   { 0,   NULL,   NULL,    NULL,   NULL }};
#endif /*MX_DRV_FRAMEWORK_ENABLED */

#define GETARRSIZE(arr)     (sizeof(arr))
#define GETARRCOUNT(arr)    (sizeof(arr) / sizeof (arr [0]))
#define GETARRELMSIZE(arr)  (sizeof (arr [0]))
/************************************************************************/
/*           The handle control macros                                  */
/************************************************************************/
#define HANDLE_INIT(h, cpu, type, index) \
(h).CPU = (cpu); \
(h).Type =(type); \
(h).aws_1 =1; \
(h).Res =0; \
(h).Index =(index);

#define HANDLE_NOT_NULL(h)    (h.aws_1 != 0)
#define HANDLE_NULL(h)        (h.aws_1 == 0)
#define HANDLE_GET_CPU(h)     (h.CPU)
#define HANDLE_GET_TYPE(h)    ((OBJTYPE)h.Type)
#define HANDLE_GET_INDEX(h)   (h.Index)
#define IS_HANDLE_EQUAL(x,y)  ((*(PUINT32)&(x)) == (*(PUINT32)&(y)))
#define HANDLE_TOINT(x)       (*(UINT32*)&(x))
#define INT_TOHANDLE(x)       (*(HANDLE*)&(x))

#define HANDLE_IDX_SIZE       8
#define HANDLE_AWS_1_SIZE     1
#define HANDLE_RES_SIZE       11
#define HANDLE_TYPE_SIZE      4
#define HANDLE_CPU_SIZE       8
#define HANDLE_IDX_MASK       0xFF
#define HANDLE_AWS_1_MASK     0x1
#define HANDLE_RES_MASK       0x7FF
#define HANDLE_TYPE_MASK      0x0F
#define HANDLE_CPU_MASK       0xFF
#define HANDLE_IDX_OFFS       0

#define HANDLE_AWS_1_OFFS     (HANDLE_IDX_SIZE)
#define HANDLE_RES_OFFS       (HANDLE_IDX_SIZE + HANDLE_AWS_1_SIZE)
#define HANDLE_TYPE_OFFS      (HANDLE_IDX_SIZE + HANDLE_AWS_1_SIZE + HANDLE_RES_SIZE)
#define HANDLE_CPU_OFFS       (HANDLE_IDX_SIZE + HANDLE_AWS_1_SIZE + HANDLE_RES_SIZE + HANDLE_TYPE_SIZE)

#define SET_HANDLE_IDX(h, idx)   ((UINT32)(h) |= (((idx)& HANDLE_IDX_MASK) << HANDLE_IDX_OFFS))
#define SET_HANDLE_RES(h, res)   ((UINT32)(h) |= (((res)& HANDLE_RES_MASK)<< HANDLE_RES_OFFS))
#define SET_HANDLE_ALW_1(h)      ((UINT32)(h) |= HANDLE_AWS_1_MASK << HANDLE_AWS_1_OFFS)
#define SET_HANDLE_TYPE(h, type) ((UINT32)(h) |= (((type)& HANDLE_TYPE_MASK) << HANDLE_TYPE_OFFS))
#define SET_HANDLE_CPU(h, cpu)   ((UINT32)(h) |= (((cpu) & HANDLE_CPU_MASK) << HANDLE_CPU_OFFS))
#define GET_HANDLE_IDX(h)        ((UINT32)(h) & HANDLE_IDX_MASK)
#define GET_HANDLE_ALW_1(h)      (((UINT32)(h) >> HANDLE_AWS_1_OFFS) & HANDLE_AWS_1_MASK)
#define GET_HANDLE_RES(h)        (((UINT32)(h) >> HANDLE_RES_OFFS) & HANDLE_RES_MASK)
#define GET_HANDLE_TYPE(h)       (((UINT32)(h) >> HANDLE_TYPE_OFFS) & HANDLE_TYPE_MASK)
#define GET_HANDLE_CPU(h)        (((UINT32)(h) >> HANDLE_CPU_OFFS) & HANDLE_CPU_MASK)

#endif /*_4GMX_CONST_H_*/
