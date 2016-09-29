//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL gputil_fl.c
 *
 * @brief DOXYGEN_TO_DO
 * @author Mindspeed Technologies
 * @version $Revision: 1.10 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#include "typedef.h"
#include "ltephydef.h"
#include "prototypes.h"
#include "ext_phy.h"
//#include "heaps.h"

#ifdef CEVA_INTRINSICS
#include <basic_op.h>
#else
#include "basic_op_cc.h"
#endif

#include "gputil_fl.h"

//-------------------------------------------------------------------------------------------
/** @brief DOXYGEN_TO_DO
 * 
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   nrl DOXYGEN_TO_DO
 *  @param   nrh DOXYGEN_TO_DO
 *  @param   ncl DOXYGEN_TO_DO
 *  @param   nch  DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |nrl           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |nrh           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |ncl           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |nch           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
double **dmatrix(S32 nrl, S32 nrh, S32 ncl, S32 nch)
{
    S32 i;
    double **m;

    m = (double **) calloc((unsigned) (nrh - nrl + 1), sizeof(double*));
    //          if (!m) nrerror("allocation failure 1 in dmatrix()");
    m -= nrl;

    for (i = nrl; i <= nrh; i++)
    {
        m[i] = (double *) calloc((unsigned) (nch - ncl + 1), sizeof(double));
        //              if (!m[i])
        //                nrerror("allocation failure 2 in dmatrix()");
        m[i] -= ncl;
    }
    return m;
}

//-------------------------------------------------------------------------------------------
/** @brief DOXYGEN_TO_DO
 * 
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   **m DOXYGEN_TO_DO
 *  @param   nrl DOXYGEN_TO_DO
 *  @param   nrh DOXYGEN_TO_DO
 *  @param   ncl DOXYGEN_TO_DO
 *  @param   nch DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |**m           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |nrl           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |nrh           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |ncl           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |nch           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void free_dmatrix(double **m, S32 nrl, S32 nrh, S32 ncl, S32 nch)
{
    S32 i;

    for (i = nrh; i >= nrl; i--)
        free((char*) (m[i] + ncl));
    free((char*) (m + nrl));
}

//-------------------------------------------------------------------------------------------
/** @brief DOXYGEN_TO_DO
 * 
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   nl DOXYGEN_TO_DO
 *  @param   nh  DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |nl            |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |nh            |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
double *dvector(long nl, long nh)
{
    double *v;

    v = (double *) calloc((unsigned) (nh - nl + 1), sizeof(double));
    //        if (!v) nrerror("allocation failure in dvector()");
    return v - nl;
}

//-------------------------------------------------------------------------------------------
/** @brief DOXYGEN_TO_DO
 * 
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *v DOXYGEN_TO_DO
 *  @param   nl DOXYGEN_TO_DO
 *  @param   nh DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*v            |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |nl            |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |nh            |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void free_dvector(double *v, long nl, long nh)
{
    free((char*) (v + nl));
}

