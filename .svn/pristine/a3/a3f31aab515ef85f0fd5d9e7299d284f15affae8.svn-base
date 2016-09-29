//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL idft_utils.c
 *
 * @brief DOXYGEN_TO_DO
 * @author Mindspeed Technologies
 * @version $Revision: 1.14 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "IDFT.h"
#include "IDFT_Tables.h"
#include "prototypes.h"

COMPLEX16 ***mult_factors;

#ifdef DEBUG_PRINT_IDFT
FILE *file_temp;
FILE *file_mult_tables;
char temp_stringa[1024];
char temp_string1[1024];
char temp_string2[1024];
char temp_string[1024];
#endif

COMPLEX16 mult_3pt[2][2];
COMPLEX16 mult_5pt[4][4];

//-------------------------------------------------------------------------------------------
/** @brief IDFT code that simlulates the IDFT results and performace on MAP in Device
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   *input Input Array pointer
 *  @param   idftSize Size of IDFT. Can be one of the 12 sizes supported in 4G LTE
 *  @param   *output Output Array pointer
 *  @param   *exponent Exponent output pointer
 *
 *  @return   none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------------------
 *  |Name         |Name In       |Direction    |Length        |Type          |Format    |Valid Range      |Units         |
 *  |             |Standard      |             |              |              |          |                 |              |
 *  ----------------------------------------------------------------------------------------------------------------------
 *  |input        |              | Input       | idftSize     |Complex Array |(1:0:15)  |full range       |              |
 *  ----------------------------------------------------------------------------------------------------------------------
 *  |idftSize     |              | Input       | 1            |Real          |(0:16:0)  |1 of 35 possible |              |
 *  ----------------------------------------------------------------------------------------------------------------------
 *  |output       |              | Output      | idftSize     |Complex Array |(1:0:15)  |full range       |              |
 *  ----------------------------------------------------------------------------------------------------------------------
 *  |exponent     |              | Output      | 1            |Real          |(1:15:0)  |-8:8             |              |
 *  ----------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------
 *  |Block Name          |IDFT             |
 *  ----------------------------------------
 *  |Input Stream        |input            |
 *  ----------------------------------------
 *  |Output Stream       |output, exponent |
 *  ----------------------------------------
 *  |Config Parameters   |idftSize         |
 *  ----------------------------------------
 *  |Status Parameters   |none             |
 *  ----------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  IDFT code that simlulates the IDFT results and performace on MAP in Device
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *  none
 *
 *
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
void IDFT(S16 *input, U16 idftSize, S16 *output, S16 *exponent)
{
    S16 *constMultTables[6];
    S16 i, j, k, l;
    S16 num_stages_idft, num_stages_mult;
    S16 *size;
    COMPLEX24 *input_inter, *output_inter;
    S16 width, count;

    S16 size_rows[2] =
    { 0 }, product_rows;
    S16 product;

    S16 Mult_Factors[6];
    S16 num_factors = 0;

#ifdef DEBUG_PRINT_IDFT
    file_temp = fopen("Error.txt","a");
    sprintf(temp_stringa, "%d IDFT", idftSize);
#endif

    //----------------------------------------------------------------------------------------------
    // Calculate Twiddle Factors for all IDFT sizes
    //---------------------------------------------

    // 3Pt
    k = 0;
    for (i = 1; i <= 2; i++)
    {
        for (j = 1; j <= 2; j++)
        {
            mult_3pt[i - 1][j - 1].real = Twiddle3Pt[k++];
            mult_3pt[i - 1][j - 1].imag = Twiddle3Pt[k++];
        }
    }

#ifdef DEBUG_PRINT_IDFT
    file_mult_tables = fopen("Twiddle_Factors_Table.txt","w");
    fprintf(file_mult_tables, "Table: 3 Pt. IDFT\n\n");
    for (i = 0; i < 2; i++)
    for (j = 0; j < 2; j++)
    fprintf(file_mult_tables, "%d\t%d\n", mult_3pt[i][j].real, mult_3pt[i][j].imag);
    fprintf(file_mult_tables, "\n");
#endif

    // 5Pt
    k = 0;
    for (i = 1; i <= 4; i++)
    {
        for (j = 1; j <= 4; j++)
        {
            mult_5pt[i - 1][j - 1].real = Twiddle5Pt[k++];
            mult_5pt[i - 1][j - 1].imag = Twiddle5Pt[k++];
        }
    }

#ifdef DEBUG_PRINT_IDFT
    fprintf(file_mult_tables, "Table: 5 Pt. IDFT\n\n");
    for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
    fprintf(file_mult_tables, "%d\t%d\n", mult_5pt[i][j].real, mult_5pt[i][j].imag);
    fprintf(file_mult_tables, "\n");
    fclose(file_mult_tables);
#endif

    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    // Allocate the necessary constants for each size
    //-----------------------------------------------

    switch (idftSize)
    {
        case 12:
        {
            num_factors = 2;
            Mult_Factors[0] = 3;
            Mult_Factors[1] = 4;
            constMultTables[0] = Mult_4_3;
            *exponent = -2;
        }
            break;
        case 24:
        {
            num_factors = 3;
            Mult_Factors[0] = 3;
            Mult_Factors[1] = 4;
            Mult_Factors[2] = 2;
            constMultTables[0] = Mult_4_3_2;
            constMultTables[1] = Mult_2_12;
            *exponent = -3;
        }
            break;
        case 36:
        {
            num_factors = 3;
            Mult_Factors[0] = 3;
            Mult_Factors[1] = 4;
            Mult_Factors[2] = 3;
            constMultTables[0] = Mult_4_3_2;
            constMultTables[1] = Mult_3_12;
            *exponent = -3;
        }
            break;
        case 48:
        {
            num_factors = 3;
            Mult_Factors[0] = 3;
            Mult_Factors[1] = 4;
            Mult_Factors[2] = 4;
            constMultTables[0] = Mult_4_3_2;
            constMultTables[1] = Mult_4_12;
            *exponent = -3;
        }
            break;
        case 60:
        {
            num_factors = 3;
            Mult_Factors[0] = 3;
            Mult_Factors[1] = 5;
            Mult_Factors[2] = 4;
            constMultTables[0] = Mult_5_3;
            constMultTables[1] = Mult_4_15;
        }
            break;
        case 72:
        {
            num_factors = 4;
            Mult_Factors[0] = 3;
            Mult_Factors[1] = 4;
            Mult_Factors[2] = 2;
            Mult_Factors[3] = 3;
            constMultTables[0] = Mult_4_3_2;
            constMultTables[1] = Mult_2_12_2;
            constMultTables[2] = Mult_3_24;
        }
            break;
        case 96:
        {
            num_factors = 4;
            Mult_Factors[0] = 3;
            Mult_Factors[1] = 4;
            Mult_Factors[2] = 2;
            Mult_Factors[3] = 4;
            constMultTables[0] = Mult_4_3_2;
            constMultTables[1] = Mult_2_12_2;
            constMultTables[2] = Mult_4_24;
        }
            break;
        case 108:
        {
            num_factors = 4;
            Mult_Factors[0] = 3;
            Mult_Factors[1] = 4;
            Mult_Factors[2] = 3;
            Mult_Factors[3] = 3;
            constMultTables[0] = Mult_4_3_2;
            constMultTables[1] = Mult_3_12_2;
            constMultTables[2] = Mult_3_36;
        }
            break;
        case 120:
        {
            num_factors = 4;
            Mult_Factors[0] = 3;
            Mult_Factors[1] = 5;
            Mult_Factors[2] = 2;
            Mult_Factors[3] = 4;
            constMultTables[0] = Mult_5_3;
            constMultTables[1] = Mult_2_15;
            constMultTables[2] = Mult_4_30;
        }
            break;
        case 144:
        {
            num_factors = 4;
            Mult_Factors[0] = 3;
            Mult_Factors[1] = 4;
            Mult_Factors[2] = 4;
            Mult_Factors[3] = 3;
            constMultTables[0] = Mult_4_3_2;
            constMultTables[1] = Mult_4_12_2;
            constMultTables[2] = Mult_3_48;
        }
            break;
        case 180:
        {
            num_factors = 4;
            Mult_Factors[0] = 3;
            Mult_Factors[1] = 5;
            Mult_Factors[2] = 4;
            Mult_Factors[3] = 3;
            constMultTables[0] = Mult_5_3;
            constMultTables[1] = Mult_4_15_2;
            constMultTables[2] = Mult_3_60;
        }
            break;
        case 192:
        {
            num_factors = 4;
            Mult_Factors[0] = 3;
            Mult_Factors[1] = 4;
            Mult_Factors[2] = 4;
            Mult_Factors[3] = 4;
            constMultTables[0] = Mult_4_3_2;
            constMultTables[1] = Mult_4_12_2;
            constMultTables[2] = Mult_4_48;
        }
            break;
        case 216:
        {
            num_factors = 5;
            Mult_Factors[0] = 3;
            Mult_Factors[1] = 4;
            Mult_Factors[2] = 2;
            Mult_Factors[3] = 3;
            Mult_Factors[4] = 3;
            constMultTables[0] = Mult_4_3_2;
            constMultTables[1] = Mult_2_12_2;
            constMultTables[2] = Mult_3_24_2;
            constMultTables[3] = Mult_3_72;
        }
            break;
        case 240:
        {
            num_factors = 4;
            Mult_Factors[0] = 3;
            Mult_Factors[1] = 5;
            Mult_Factors[2] = 4;
            Mult_Factors[3] = 4;
            constMultTables[0] = Mult_5_3;
            constMultTables[1] = Mult_4_15_2;
            constMultTables[2] = Mult_4_60;
        }
            break;
        case 288:
        {
            num_factors = 5;
            Mult_Factors[0] = 3;
            Mult_Factors[1] = 4;
            Mult_Factors[2] = 2;
            Mult_Factors[3] = 4;
            Mult_Factors[4] = 3;
            constMultTables[0] = Mult_4_3_2;
            constMultTables[1] = Mult_2_12_2;
            constMultTables[2] = Mult_4_24_2;
            constMultTables[3] = Mult_3_96;
        }
            break;
        case 300:
        {
            num_factors = 4;
            Mult_Factors[0] = 3;
            Mult_Factors[1] = 5;
            Mult_Factors[2] = 5;
            Mult_Factors[3] = 4;
            constMultTables[0] = Mult_5_3;
            constMultTables[1] = Mult_5_15;
            constMultTables[2] = Mult_4_75;
        }
            break;
        case 324:
        {
            num_factors = 5;
            Mult_Factors[0] = 3;
            Mult_Factors[1] = 4;
            Mult_Factors[2] = 3;
            Mult_Factors[3] = 3;
            Mult_Factors[4] = 3;
            constMultTables[0] = Mult_4_3_2;
            constMultTables[1] = Mult_3_12_2;
            constMultTables[2] = Mult_3_36_2;
            constMultTables[3] = Mult_3_108;
        }
            break;
        case 360:
        {
            num_factors = 5;
            Mult_Factors[0] = 3;
            Mult_Factors[1] = 5;
            Mult_Factors[2] = 2;
            Mult_Factors[3] = 4;
            Mult_Factors[4] = 3;
            constMultTables[0] = Mult_5_3;
            constMultTables[1] = Mult_2_15;
            constMultTables[2] = Mult_4_30_2;
            constMultTables[3] = Mult_3_120;
        }
            break;
        case 384:
        {
            num_factors = 5;
            Mult_Factors[0] = 3;
            Mult_Factors[1] = 4;
            Mult_Factors[2] = 2;
            Mult_Factors[3] = 4;
            Mult_Factors[4] = 4;
            constMultTables[0] = Mult_4_3_2;
            constMultTables[1] = Mult_2_12_2;
            constMultTables[2] = Mult_4_24_2;
            constMultTables[3] = Mult_4_96;
        }
            break;
        case 432:
        {
            num_factors = 5;
            Mult_Factors[0] = 3;
            Mult_Factors[1] = 4;
            Mult_Factors[2] = 4;
            Mult_Factors[3] = 3;
            Mult_Factors[4] = 3;
            constMultTables[0] = Mult_4_3_2;
            constMultTables[1] = Mult_4_12_2;
            constMultTables[2] = Mult_3_48_2;
            constMultTables[3] = Mult_3_144;
        }
            break;
        case 480:
        {
            num_factors = 5;
            Mult_Factors[0] = 3;
            Mult_Factors[1] = 5;
            Mult_Factors[2] = 2;
            Mult_Factors[3] = 4;
            Mult_Factors[4] = 4;
            constMultTables[0] = Mult_5_3;
            constMultTables[1] = Mult_2_15;
            constMultTables[2] = Mult_4_30_2;
            constMultTables[3] = Mult_4_120;
        }
            break;
        case 540:
        {
            num_factors = 5;
            Mult_Factors[0] = 3;
            Mult_Factors[1] = 5;
            Mult_Factors[2] = 4;
            Mult_Factors[3] = 3;
            Mult_Factors[4] = 3;
            constMultTables[0] = Mult_5_3;
            constMultTables[1] = Mult_4_15_2;
            constMultTables[2] = Mult_3_60_2;
            constMultTables[3] = Mult_3_180;
        }
            break;
        case 576:
        {
            num_factors = 5;
            Mult_Factors[0] = 3;
            Mult_Factors[1] = 4;
            Mult_Factors[2] = 4;
            Mult_Factors[3] = 4;
            Mult_Factors[4] = 3;
            constMultTables[0] = Mult_4_3_2;
            constMultTables[1] = Mult_4_12_2;
            constMultTables[2] = Mult_4_48_2;
            constMultTables[3] = Mult_3_192;
        }
            break;
        case 600:
        {
            num_factors = 5;
            Mult_Factors[0] = 3;
            Mult_Factors[1] = 5;
            Mult_Factors[2] = 5;
            Mult_Factors[3] = 2;
            Mult_Factors[4] = 4;
            constMultTables[0] = Mult_5_3;
            constMultTables[1] = Mult_5_15;
            constMultTables[2] = Mult_2_75;
            constMultTables[3] = Mult_4_150;
        }
            break;
        case 648:
        {
            num_factors = 6;
            Mult_Factors[0] = 3;
            Mult_Factors[1] = 4;
            Mult_Factors[2] = 2;
            Mult_Factors[3] = 3;
            Mult_Factors[4] = 3;
            Mult_Factors[5] = 3;
            constMultTables[0] = Mult_4_3_2;
            constMultTables[1] = Mult_2_12_2;
            constMultTables[2] = Mult_3_24_2;
            constMultTables[3] = Mult_3_72_2;
            constMultTables[4] = Mult_3_216;
        }
            break;
        case 720:
        {
            num_factors = 5;
            Mult_Factors[0] = 3;
            Mult_Factors[1] = 5;
            Mult_Factors[2] = 4;
            Mult_Factors[3] = 4;
            Mult_Factors[4] = 3;
            constMultTables[0] = Mult_5_3;
            constMultTables[1] = Mult_4_15_2;
            constMultTables[2] = Mult_4_60_2;
            constMultTables[3] = Mult_3_240;
        }
            break;
        case 768:
        {
            num_factors = 5;
            Mult_Factors[0] = 3;
            Mult_Factors[1] = 4;
            Mult_Factors[2] = 4;
            Mult_Factors[3] = 4;
            Mult_Factors[4] = 4;
            constMultTables[0] = Mult_4_3_2;
            constMultTables[1] = Mult_4_12_2;
            constMultTables[2] = Mult_4_48_2;
            constMultTables[3] = Mult_4_192;
        }
            break;
        case 864:
        {
            num_factors = 6;
            Mult_Factors[0] = 3;
            Mult_Factors[1] = 4;
            Mult_Factors[2] = 4;
            Mult_Factors[3] = 2;
            Mult_Factors[4] = 3;
            Mult_Factors[5] = 3;
            constMultTables[0] = Mult_4_3_2;
            constMultTables[1] = Mult_4_12_2;
            constMultTables[2] = Mult_2_48;
            constMultTables[3] = Mult_3_96_2;
            constMultTables[4] = Mult_3_288;
        }
            break;
        case 900:
        {
            num_factors = 5;
            Mult_Factors[0] = 3;
            Mult_Factors[1] = 5;
            Mult_Factors[2] = 5;
            Mult_Factors[3] = 4;
            Mult_Factors[4] = 3;
            constMultTables[0] = Mult_5_3;
            constMultTables[1] = Mult_5_15;
            constMultTables[2] = Mult_4_75_2;
            constMultTables[3] = Mult_3_300;
        }
            break;
        case 960:
        {
            num_factors = 5;
            Mult_Factors[0] = 3;
            Mult_Factors[1] = 5;
            Mult_Factors[2] = 4;
            Mult_Factors[3] = 4;
            Mult_Factors[4] = 4;
            constMultTables[0] = Mult_5_3;
            constMultTables[1] = Mult_4_15_2;
            constMultTables[2] = Mult_4_60_2;
            constMultTables[3] = Mult_4_240;
        }
            break;
        case 972:
        {
            num_factors = 6;
            Mult_Factors[0] = 3;
            Mult_Factors[1] = 4;
            Mult_Factors[2] = 3;
            Mult_Factors[3] = 3;
            Mult_Factors[4] = 3;
            Mult_Factors[5] = 3;
            constMultTables[0] = Mult_4_3_2;
            constMultTables[1] = Mult_3_12_2;
            constMultTables[2] = Mult_3_36_2;
            constMultTables[3] = Mult_3_108_2;
            constMultTables[4] = Mult_3_324;
        }
            break;
        case 1080:
        {
            num_factors = 6;
            Mult_Factors[0] = 3;
            Mult_Factors[1] = 5;
            Mult_Factors[2] = 4;
            Mult_Factors[3] = 2;
            Mult_Factors[4] = 3;
            Mult_Factors[5] = 3;
            constMultTables[0] = Mult_5_3;
            constMultTables[1] = Mult_4_15_2;
            constMultTables[2] = Mult_2_60;
            constMultTables[3] = Mult_3_120_2;
            constMultTables[4] = Mult_3_360;
        }
            break;
        case 1152:
        {
            num_factors = 6;
            Mult_Factors[0] = 3;
            Mult_Factors[1] = 4;
            Mult_Factors[2] = 4;
            Mult_Factors[3] = 4;
            Mult_Factors[4] = 2;
            Mult_Factors[5] = 3;
            constMultTables[0] = Mult_4_3_2;
            constMultTables[1] = Mult_4_12_2;
            constMultTables[2] = Mult_4_48_2;
            constMultTables[3] = Mult_2_192;
            constMultTables[4] = Mult_3_384;
        }
            break;
        case 1200:
        {
            num_factors = 5;
            Mult_Factors[0] = 3;
            Mult_Factors[1] = 5;
            Mult_Factors[2] = 5;
            Mult_Factors[3] = 4;
            Mult_Factors[4] = 4;
            constMultTables[0] = Mult_5_3;
            constMultTables[1] = Mult_5_15;
            constMultTables[2] = Mult_4_75_2;
            constMultTables[3] = Mult_4_300;
        }
            break;
        case 1296:
        {
            num_factors = 6;
            Mult_Factors[0] = 3;
            Mult_Factors[1] = 4;
            Mult_Factors[2] = 4;
            Mult_Factors[3] = 3;
            Mult_Factors[4] = 3;
            Mult_Factors[5] = 3;
            constMultTables[0] = Mult_4_3_2;
            constMultTables[1] = Mult_4_12_2;
            constMultTables[2] = Mult_3_48_2;
            constMultTables[3] = Mult_3_144_2;
            constMultTables[4] = Mult_3_432;
        }
            break;

    }

    product = 1;
    for (i = 0; i < num_factors; i++)
        product *= Mult_Factors[i];

    if (product != idftSize)
    {
#ifdef _WIN32
        printf("Error: Product %d != IDFT Size %d\n", product, idftSize);
#endif
        return;
    }

#ifdef DEBUG_PRINT_IDFT
    sprintf(temp_string,"Mult_Tables_%d.txt", product);
    file_mult_tables = fopen(temp_string,"w");
#endif

    num_stages_idft = num_factors;
    num_stages_mult = num_stages_idft - 1;

    //----------------------------------------------------------------------------------------------
    // Create all of the multiplication tables
    //----------------------------------------

    mult_factors = (COMPLEX16***) MemAlloc(num_stages_mult * sizeof(COMPLEX16**),
                                                  CRAM_MEM, 0);
    size = (S16 *) MemAlloc(num_stages_mult * sizeof(S16), CRAM_MEM, 0);

    l = 0;
    size_rows[0] = Mult_Factors[0];
    product_rows = size_rows[0];
    for (i = 1; i < num_factors; i++, l++)
    {
        size_rows[1] = Mult_Factors[i];
        product_rows *= size_rows[1];

        size[l] = size_rows[1];
        mult_factors[l] = (COMPLEX16**) MemAlloc(size_rows[1] * sizeof(COMPLEX16*),
                                                        CRAM_MEM, 0);
        for (j = 0; j < size_rows[1]; j++)
            mult_factors[l][j] = (COMPLEX16*) MemAlloc(
                                                              size_rows[0]
                                                                  * sizeof(COMPLEX16),
                                                              CRAM_MEM, 0);

#ifdef DEBUG_PRINT_IDFT
        fprintf(file_mult_tables, "Table %d: Mult_Factors - %d x %d\n\n", l+1, size_rows[1], size_rows[0]);
#endif

        count = 0;
        for (j = 0; j < size_rows[0]; j++)
        {
            for (k = 0; k < size_rows[1]; k++)
            {
                mult_factors[l][k][j].real = constMultTables[i - 1][count++];
                mult_factors[l][k][j].imag = constMultTables[i - 1][count++];
#ifdef DEBUG_PRINT_IDFT
                fprintf(file_mult_tables, "%d\t%d\n", mult_factors[l][k][j].real, mult_factors[l][k][j].imag);
#endif
            }
        }
#ifdef DEBUG_PRINT_IDFT
        fprintf(file_mult_tables, "\n");
#endif
        size_rows[0] *= size_rows[1];
    }

#ifdef DEBUG_PRINT_IDFT
    fclose(file_mult_tables);
#endif

    //----------------------------------------------------------------------------------------------
    // Perform different IDFT sizes and multiplications
    //-------------------------------------------------

    l = 0;

    // Copy Input from 16 to 24 bits
    input_inter = (COMPLEX24*) MemAlloc(product * sizeof(COMPLEX24), CRAM_MEM, 0);
    output_inter = (COMPLEX24*) MemAlloc(product * sizeof(COMPLEX24), CRAM_MEM, 0);
    k = 0;
    for (i = 0; i < product; i++)
    {
        input_inter[i].real = ((S24) input[k++]);
        input_inter[i].imag = ((S24) input[k++]);
    }

    // IDFT Stages Loop
    for (i = 0; i < num_factors; i++)
    {
#ifdef DEBUG_PRINT_IDFT
        sprintf(temp_string1,"%s\tBreak %d", temp_stringa, i);
#endif
        BreakDownRow(input_inter, output_inter,
                     (S16) ((i == 0) ? (num_factors - 1) : (num_factors - i)),
                     (S16) ((i == 0) ? ROWS : COLUMNS), product_rows, Mult_Factors, num_factors);
#ifdef DEBUG_PRINT_IDFT
        sprintf(temp_string, "Output_%d_Stage_%d.txt", product_rows, ((l+1)*2)-1);
        file_mult_tables = fopen(temp_string,"w");

        for (j = 0; j < product; j++)
        fprintf(file_mult_tables, "%d\t%d\n", output_inter[j].real, output_inter[j].imag);
        fprintf(file_mult_tables, "\n");
        fclose(file_mult_tables);
#endif
        if (i != num_factors - 1)
        {

#ifdef DEBUG_PRINT_IDFT
            sprintf(temp_string1,"%s\tMult %d", temp_stringa, i);
#endif
            MultCosntRow(output_inter, input_inter,
                         (S16) ((i == 0) ? (num_factors - 1) : (num_factors - i - 1)),
                         product_rows, Mult_Factors, num_factors, l);

#ifdef DEBUG_PRINT_IDFT
            sprintf(temp_string, "Output_%d_Stage_%d.txt", product_rows, ((l+1)*2));
            file_mult_tables = fopen(temp_string,"w");

            for (j = 0; j < product; j++)
            fprintf(file_mult_tables, "%d\t%d\n", input_inter[j].real, input_inter[j].imag);
            fprintf(file_mult_tables, "\n");
            fclose(file_mult_tables);
#endif
            l++;
        }
    }

    FindMax(output_inter, product, &width);

    // Copy Back the outputs to the correct vectors
    //---------------------------------------------
    *exponent = width;

    k = 0;
    for (i = 0; i < product; i++)
    {
        output[k++] = (S16) output_inter[i].real;
        output[k++] = (S16) output_inter[i].imag;
    }

#ifdef DEBUG_PRINT_IDFT
    sprintf(temp_string, "Output_%d_Stage_%d.txt", product_rows, ((l+1)*2));
    file_mult_tables = fopen(temp_string,"w");

    fprintf(file_mult_tables, "%d\n", width);
    k = 0;
    for (j = 0; j < product; j++, k+=2)
    fprintf(file_mult_tables, "%d\t%d\n", output[k], output[k+1]);
    fprintf(file_mult_tables, "\n");
    fclose(file_mult_tables);
#endif

    //----------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------
    // Free Allocated memory
    //----------------------
    for (i = 0; i < num_stages_mult; i++)
    {
        for (j = 0; j < size[i]; j++)
            MemFree(mult_factors[i][j], CRAM_MEM);
    }
    for (i = 0; i < num_stages_mult; i++)
        MemFree(mult_factors[i], CRAM_MEM);

    MemFree(mult_factors, CRAM_MEM);
    MemFree(size, CRAM_MEM);
    MemFree(input_inter, CRAM_MEM);
    MemFree(output_inter, CRAM_MEM);

#ifdef DEBUG_PRINT
    printf("Completed IDFT size: %d\n", product);
#endif

#ifdef DEBUG_PRINT_IDFT
    fprintf(file_temp, "\n");
    fclose(file_temp);
#endif
    return;
}
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
// IDFT Break down operations
//---------------------------


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   COMPLEX24 *input DOXYGEN_TO_DO
 *  @param   COMPLEX24 *output DOXYGEN_TO_DO
 *  @param   stage DOXYGEN_TO_DO
 *  @param   flag DOXYGEN_TO_DO
 *  @param   product DOXYGEN_TO_DO
 *  @param   *product_array DOXYGEN_TO_DO
 *  @param   num_factors DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  -------------------------------------------------------------------------------------------------------------
 *  |Name             |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  -------------------------------------------------------------------------------------------------------------
 *  |COMPLEX24 *input |              |              |              |              |              |              |
 *  -------------------------------------------------------------------------------------------------------------
 *  |COMPLEX24 *output|              |              |              |              |              |              |
 *  -------------------------------------------------------------------------------------------------------------
 *  |stage            |              |              |              |              |              |              |
 *  -------------------------------------------------------------------------------------------------------------
 *  |flag             |              |              |              |              |              |              |
 *  -------------------------------------------------------------------------------------------------------------
 *  |product          |              |              |              |              |              |              |
 *  -------------------------------------------------------------------------------------------------------------
 *  |*product_array   |              |              |              |              |              |              |
 *  -------------------------------------------------------------------------------------------------------------
 *  |num_factors      |              |              |              |              |              |              |
 *  -------------------------------------------------------------------------------------------------------------
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
void BreakDownRow(COMPLEX24 *input, COMPLEX24 *output, S16 stage, S16 flag,
                  S16 product, S16 *product_array, S16 num_factors)
{
    int i, j, k;
    int rows, cols;
    COMPLEX24 **input_local;
    COMPLEX24 **output_local;

    // Allocate 2-D memory
    if (stage == 1 && flag == COLUMNS)
    {
        rows = product / product_array[num_factors - 1];
        cols = product_array[num_factors - 1];
    }
    else
    {
        rows = product_array[num_factors - 1];
        cols = product / product_array[num_factors - 1];
    }

    input_local = (COMPLEX24**) MemAlloc(rows * sizeof(COMPLEX24*), CRAM_MEM, 0);
    for (i = 0; i < rows; i++)
        input_local[i] = (COMPLEX24*) MemAlloc(cols * sizeof(COMPLEX24), CRAM_MEM, 0);

    output_local = (COMPLEX24**) MemAlloc(rows * sizeof(COMPLEX24*), CRAM_MEM, 0);
    for (i = 0; i < rows; i++)
        output_local[i]
            = (COMPLEX24*) MemAlloc(cols * sizeof(COMPLEX24), CRAM_MEM, 0);

    // Copy Data
    if (stage == 1 && flag == COLUMNS)
    {
        k = 0;
        for (i = 0; i < rows; i++)
            for (j = 0; j < cols; j++)
            {
                input_local[i][j].real = input[k].real;
                input_local[i][j].imag = input[k++].imag;
            }
    }
    else
    {
        k = 0;
        for (i = 0; i < cols; i++)
            for (j = 0; j < rows; j++)
            {
                input_local[j][i].real = input[k].real;
                input_local[j][i].imag = input[k++].imag;
            }
    }

    if (stage != 1)
    {
        for (i = 0; i < rows; i++)
        {
            BreakDownRow(input_local[i], output_local[i], (S16) (stage - 1), (S16) flag,
                         (S16) cols, (S16*) product_array, (S16) (num_factors - 1));
        }
    }
    else
    {
        if (cols == 2)
        {
            for (i = 0; i < rows; i++)
            {
                Idft2Pt(input_local[i], output_local[i]);
            }
        }
        else if (cols == 3)
        {
            for (i = 0; i < rows; i++)
            {
                Idft3Pt(input_local[i], output_local[i]);
            }
        }
        else if (cols == 4)
        {
            for (i = 0; i < rows; i++)
            {
                Idft4Pt(input_local[i], output_local[i]);
            }
        }
        else if (cols == 5)
        {
            for (i = 0; i < rows; i++)
            {
                Idft5Pt(input_local[i], output_local[i]);
            }
        }
    }

    // Write to output buffer
    k = 0;
    for (i = 0; i < cols; i++)
        for (j = 0; j < rows; j++)
        {
            output[k].real = output_local[j][i].real;
            output[k++].imag = output_local[j][i].imag;
        }

    // Free all the allocated memory buffers
    for (i = 0; i < rows; i++)
    {
        MemFree(input_local[i], CRAM_MEM);
        MemFree(output_local[i], CRAM_MEM);
    }
    MemFree(input_local, CRAM_MEM);
    MemFree(output_local, CRAM_MEM);
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   COMPLEX24 *input DOXYGEN_TO_DO
 *  @param   COMPLEX24 *output DOXYGEN_TO_DO
 *  @param   stage DOXYGEN_TO_DO
 *  @param   product DOXYGEN_TO_DO
 *  @param   *product_array DOXYGEN_TO_DO
 *  @param   num_factors DOXYGEN_TO_DO
 *  @param   level DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  -------------------------------------------------------------------------------------------------------------
 *  |Name             |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  -------------------------------------------------------------------------------------------------------------
 *  |COMPLEX24 *input |              |              |              |              |              |              |
 *  -------------------------------------------------------------------------------------------------------------
 *  |COMPLEX24 *output|              |              |              |              |              |              |
 *  -------------------------------------------------------------------------------------------------------------
 *  |stage            |              |              |              |              |              |              |
 *  -------------------------------------------------------------------------------------------------------------
 *  |product          |              |              |              |              |              |              |
 *  -------------------------------------------------------------------------------------------------------------
 *  |*product_array   |              |              |              |              |              |              |
 *  -------------------------------------------------------------------------------------------------------------
 *  |num_factors      |              |              |              |              |              |              |
 *  -------------------------------------------------------------------------------------------------------------
 *  |level            |              |              |              |              |              |              |
 *  -------------------------------------------------------------------------------------------------------------
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
void MultCosntRow(COMPLEX24 *input, COMPLEX24 *output, S16 stage, S16 product,
                  S16 *product_array, S16 num_factors, S16 level)
{
    int i, j, k;
    int rows, cols;
    COMPLEX24 **input_local, **output_local;
    COMPLEX64 result;

    // Allocate 2-D memory
    rows = product_array[num_factors - 1];
    cols = product / product_array[num_factors - 1];

    input_local = (COMPLEX24**) MemAlloc(rows * sizeof(COMPLEX24*), CRAM_MEM, 0);
    for (i = 0; i < rows; i++)
        input_local[i] = (COMPLEX24*) MemAlloc(cols * sizeof(COMPLEX24), CRAM_MEM, 0);

    output_local = (COMPLEX24**) MemAlloc(rows * sizeof(COMPLEX24*), CRAM_MEM, 0);
    for (i = 0; i < rows; i++)
        output_local[i]
            = (COMPLEX24*) MemAlloc(cols * sizeof(COMPLEX24), CRAM_MEM, 0);

    // Copy Data
    k = 0;
    for (i = 0; i < cols; i++)
        for (j = 0; j < rows; j++)
        {
            input_local[j][i].real = input[k].real;
            input_local[j][i].imag = input[k++].imag;
        }

    if (stage != 1)
    {
        for (i = 0; i < rows; i++)
        {
            MultCosntRow(input_local[i], output_local[i], (S16) (stage - 1), (S16) cols,
                         product_array, (S16) (num_factors - 1), (S16) level);
        }
    }
    else
    {
        for (i = 0; i < rows; i++)
            for (j = 0; j < cols; j++)
            {
                result = MultiplyComplex24(input_local[i][j], mult_factors[level][i][j]);
#ifdef DEBUG_PRINT_IDFT
                sprintf(temp_string2, "%s\t\t%d\t%d\t\t%d\t%d\t\t\t\t\t\t\t\t\t", temp_string1, input_local[i][j].real, input_local[i][j].imag, mult_factors[level][i][j].real, mult_factors[level][i][j].imag);
#endif
                ConvertComplex64to24(&result, &output_local[i][j], 1);
            }
    }

    // Write to output buffer
    k = 0;
    for (i = 0; i < cols; i++)
        for (j = 0; j < rows; j++)
        {
            output[k].real = output_local[j][i].real;
            output[k++].imag = output_local[j][i].imag;
        }

    // Free all the allocated memory buffers
    for (i = 0; i < rows; i++)
    {
        MemFree(input_local[i], CRAM_MEM);
        MemFree(output_local[i], CRAM_MEM);
    }
    MemFree(input_local, CRAM_MEM);
    MemFree(output_local, CRAM_MEM);

}
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
// IDFT Routines
//--------------


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   COMPLEX24 *input DOXYGEN_TO_DO
 *  @param   COMPLEX24 *output DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  -------------------------------------------------------------------------------------------------------------
 *  |Name             |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  -------------------------------------------------------------------------------------------------------------
 *  |COMPLEX24 *input |              |              |              |              |              |              |
 *  -------------------------------------------------------------------------------------------------------------
 *  |COMPLEX24 *output|              |              |              |              |              |              |
 *  -------------------------------------------------------------------------------------------------------------
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
void Idft2Pt(COMPLEX24 *input, COMPLEX24 *output)
{
    // 2 Pt IDFT routine
    // Input 16 bit, twiddle factors 16-bit
    // Output 24 bit

    // x(0) = X(0) + X(1)
    // x(1) = X(0) - X(1)

    S16 i;
    COMPLEX64 inter[2], input_local[2];

#ifdef DEBUG_PRINT_IDFT
    sprintf(temp_string2, "%s\t2 Pt IDFT\t%d\t%d\t\t%d\t%d\t\t\t\t\t\t\t\t\t", temp_string1, input[0].real, input[0].imag, input[1].real, input[1].imag);
#endif

    for (i = 0; i < 2; i++)
    {
        input_local[i].real = ((S64) input[i].real) << 16;
        input_local[i].imag = ((S64) input[i].imag) << 16;
    }

    inter[0] = AddComplex64(input_local[0], input_local[1]);
    inter[1] = SubtractComplex64(input_local[0], input_local[1]);

    ConvertComplex64to24(inter, output, 2);
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   COMPLEX24 *input DOXYGEN_TO_DO
 *  @param   COMPLEX24 *output DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  -------------------------------------------------------------------------------------------------------------
 *  |Name             |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  -------------------------------------------------------------------------------------------------------------
 *  |COMPLEX24 *input |              |              |              |              |              |              |
 *  -------------------------------------------------------------------------------------------------------------
 *  |COMPLEX24 *output|              |              |              |              |              |              |
 *  -------------------------------------------------------------------------------------------------------------
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
void Idft3Pt(COMPLEX24 *input, COMPLEX24 *output)
{
    // 3 Pt IDFT routine
    // Input 16 bit, twiddle factors 16-bit
    // Output 24 bit

    // x(0) = X(0) + X(1)        + X(2)
    // x(1) = X(0) + X(1)*Y(1,1) + X(2)*Y(1,2)
    // x(2) = X(0) + X(1)*Y(2,1) + X(2)*Y(2,2)

    S16 i, j;
    COMPLEX64 input_local2[4], input_local1[3];
    COMPLEX64 output_local[3];

#ifdef DEBUG_PRINT_IDFT
    sprintf(temp_string2, "%s\t3 Pt IDFT\t%d\t%d\t\t%d\t%d\t\t%d\t%d\t\t\t\t\t\t", temp_string1, input[0].real, input[0].imag, input[1].real, input[1].imag, input[2].real, input[2].imag);
#endif

    // Convert to 64 for the adds
    for (i = 0; i < 3; i++)
    {
        input_local1[i].real = ((S64) input[i].real) << 16;
        input_local1[i].imag = ((S64) input[i].imag) << 16;
    }

    // Complex Multiplies
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 2; j++)
        {
            input_local2[(i * 2 + j)] = MultiplyComplex24(input[j + 1], mult_3pt[i][j]);
        }
    }

    // 1st point
    output_local[0] = AddComplex64(input_local1[0], input_local1[1]);
    output_local[0] = AddComplex64(output_local[0], input_local1[2]);

    // 2nd Point
    output_local[1] = AddComplex64(input_local1[0], input_local2[0]);
    output_local[1] = AddComplex64(output_local[1], input_local2[1]);

    // 3rd Point
    output_local[2] = AddComplex64(input_local1[0], input_local2[2]);
    output_local[2] = AddComplex64(output_local[2], input_local2[3]);

    ConvertComplex64to24(output_local, output, 3);

    i = 0;
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   COMPLEX24 *input DOXYGEN_TO_DO
 *  @param   COMPLEX24 *output DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  -------------------------------------------------------------------------------------------------------------
 *  |Name             |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  -------------------------------------------------------------------------------------------------------------
 *  |COMPLEX24 *input |              |              |              |              |              |              |
 *  -------------------------------------------------------------------------------------------------------------
 *  |COMPLEX24 *output|              |              |              |              |              |              |
 *  -------------------------------------------------------------------------------------------------------------
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
void Idft4Pt(COMPLEX24 *input, COMPLEX24 *output)
{
    // 4 Pt IDFT routine
    // Input 16 bit, twiddle factors 16-bit
    // Output 24 bit

    // x(0).r = X(0).r + X(1).r + X(2).r + X(3).r
    // x(1).r = X(0).r - X(1).i - X(2).r + X(3).i
    // x(2).r = X(0).r - X(1).r + X(2).r - X(3).r
    // x(3).r = X(0).r + X(1).i - X(2).r - X(3).i

    // x(0).i = X(0).i + X(1).i + X(2).i + X(3).i
    // x(1).i = X(0).i + X(1).r - X(2).i - X(3).r
    // x(2).i = X(0).i - X(1).i + X(2).i - X(3).i
    // x(3).i = X(0).i - X(1).r - X(2).i + X(3).r

    S16 i;
    COMPLEX64 output_local[4], input_local[4];

#ifdef DEBUG_PRINT_IDFT
    sprintf(temp_string2, "%s\t4 Pt IDFT\t%d\t%d\t\t%d\t%d\t\t%d\t%d\t\t%d\t%d\t\t\t", temp_string1, input[0].real, input[0].imag, input[1].real, input[1].imag, input[2].real, input[2].imag, input[3].real, input[3].imag);
#endif

    for (i = 0; i < 4; i++)
    {
        input_local[i].real = ((S64) input[i].real) << 16;
        input_local[i].imag = ((S64) input[i].imag) << 16;
    }

    output_local[0].real = input_local[0].real + input_local[1].real + input_local[2].real
        + input_local[3].real;
    output_local[1].real = input_local[0].real - input_local[1].imag - input_local[2].real
        + input_local[3].imag;
    output_local[2].real = input_local[0].real - input_local[1].real + input_local[2].real
        - input_local[3].real;
    output_local[3].real = input_local[0].real + input_local[1].imag - input_local[2].real
        - input_local[3].imag;

    output_local[0].imag = input_local[0].imag + input_local[1].imag + input_local[2].imag
        + input_local[3].imag;
    output_local[1].imag = input_local[0].imag + input_local[1].real - input_local[2].imag
        - input_local[3].real;
    output_local[2].imag = input_local[0].imag - input_local[1].imag + input_local[2].imag
        - input_local[3].imag;
    output_local[3].imag = input_local[0].imag - input_local[1].real - input_local[2].imag
        + input_local[3].real;

    ConvertComplex64to24(output_local, output, 4);
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   COMPLEX24 *input DOXYGEN_TO_DO
 *  @param   COMPLEX24 *output DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  -------------------------------------------------------------------------------------------------------------
 *  |Name             |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  -------------------------------------------------------------------------------------------------------------
 *  |COMPLEX24 *input |              |              |              |              |              |              |
 *  -------------------------------------------------------------------------------------------------------------
 *  |COMPLEX24 *output|              |              |              |              |              |              |
 *  -------------------------------------------------------------------------------------------------------------
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
void Idft5Pt(COMPLEX24 *input, COMPLEX24 *output)
{
    // 5 Pt IDFT routine
    // Input 16 bit, twiddle factors 16-bit
    // Output 24 bit

    // x(0) = X(0) + X(1)        + X(2)        + X(3)        + X(4)
    // x(1) = X(0) + X(1)*Y(1,1) + X(2)*Y(1,2) + X(3)*Y(1,3) + X(4)*Y(1,4)
    // x(2) = X(0) + X(1)*Y(2,1) + X(2)*Y(2,2) + X(3)*Y(2,3) + X(4)*Y(2,4)
    // x(3) = X(0) + X(1)*Y(3,1) + X(2)*Y(3,2) + X(3)*Y(3,3) + X(4)*Y(3,4)
    // x(4) = X(0) + X(1)*Y(4,1) + X(2)*Y(4,2) + X(3)*Y(4,3) + X(4)*Y(4,4)

    S16 i, j;
    COMPLEX64 input_local2[16], input_local1[5];
    COMPLEX64 output_local[5];

#ifdef DEBUG_PRINT_IDFT
    sprintf(temp_string2, "%s\t5 Pt IDFT\t%d\t%d\t\t%d\t%d\t\t%d\t%d\t\t%d\t%d\t\t%d\t%d", temp_string1, input[0].real, input[0].imag, input[1].real, input[1].imag, input[2].real, input[2].imag, input[3].real, input[3].imag, input[4].real, input[4].imag);
#endif

    // Convert to 64 for the adds
    for (i = 0; i < 5; i++)
    {
        input_local1[i].real = ((S64) input[i].real) << 16;
        input_local1[i].imag = ((S64) input[i].imag) << 16;
    }

    // Complex Multiplies
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            input_local2[(i * 4 + j)] = MultiplyComplex24(input[j + 1], mult_5pt[i][j]);
        }
    }

    // 1st point
    output_local[0] = AddComplex64(input_local1[0], input_local1[1]);
    output_local[0] = AddComplex64(output_local[0], input_local1[2]);
    output_local[0] = AddComplex64(output_local[0], input_local1[3]);
    output_local[0] = AddComplex64(output_local[0], input_local1[4]);

    // 2nd Point
    output_local[1] = AddComplex64(input_local1[0], input_local2[0]);
    output_local[1] = AddComplex64(output_local[1], input_local2[1]);
    output_local[1] = AddComplex64(output_local[1], input_local2[2]);
    output_local[1] = AddComplex64(output_local[1], input_local2[3]);

    // 3rd Point
    output_local[2] = AddComplex64(input_local1[0], input_local2[4]);
    output_local[2] = AddComplex64(output_local[2], input_local2[5]);
    output_local[2] = AddComplex64(output_local[2], input_local2[6]);
    output_local[2] = AddComplex64(output_local[2], input_local2[7]);

    // 4th Point
    output_local[3] = AddComplex64(input_local1[0], input_local2[8]);
    output_local[3] = AddComplex64(output_local[3], input_local2[9]);
    output_local[3] = AddComplex64(output_local[3], input_local2[10]);
    output_local[3] = AddComplex64(output_local[3], input_local2[11]);

    // 5th Point
    output_local[4] = AddComplex64(input_local1[0], input_local2[12]);
    output_local[4] = AddComplex64(output_local[4], input_local2[13]);
    output_local[4] = AddComplex64(output_local[4], input_local2[14]);
    output_local[4] = AddComplex64(output_local[4], input_local2[15]);

    ConvertComplex64to24(output_local, output, 5);
}
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
// Complex Operations
//-------------------


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   COMPLEX24 a DOXYGEN_TO_DO
 *  @param   COMPLEX24 b DOXYGEN_TO_DO
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
 *  |COMPLEX24 a   |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |COMPLEX24 b   |              |              |              |              |              |              |
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
COMPLEX64 AddComplex24(COMPLEX24 a, COMPLEX24 b)
{
    COMPLEX64 c;

    c.real = (S64) a.real + (S64) b.real;
    c.imag = (S64) a.imag + (S64) b.imag;

    return c;
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   COMPLEX64 a DOXYGEN_TO_DO
 *  @param   COMPLEX64 b DOXYGEN_TO_DO
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
 *  |COMPLEX64 a   |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |COMPLEX64 b   |              |              |              |              |              |              |
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
COMPLEX64 AddComplex64(COMPLEX64 a, COMPLEX64 b)
{
    COMPLEX64 c;

    c.real = a.real + b.real;
    c.imag = a.imag + b.imag;

    return c;
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   COMPLEX24 a DOXYGEN_TO_DO
 *  @param   COMPLEX24 b DOXYGEN_TO_DO
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
 *  |COMPLEX24 a   |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |COMPLEX24 b   |              |              |              |              |              |              |
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
COMPLEX64 SubtractComplex24(COMPLEX24 a, COMPLEX24 b)
{
    COMPLEX64 c;

    c.real = (S64) a.real - (S64) b.real;
    c.imag = (S64) a.imag - (S64) b.imag;

    return c;
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   COMPLEX64 a DOXYGEN_TO_DO
 *  @param   COMPLEX64 b DOXYGEN_TO_DO
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
 *  |COMPLEX64 a   |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |COMPLEX64 b   |              |              |              |              |              |              |
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
COMPLEX64 SubtractComplex64(COMPLEX64 a, COMPLEX64 b)
{
    COMPLEX64 c;

    c.real = a.real - b.real;
    c.imag = a.imag - b.imag;

    return c;
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   COMPLEX64 *inter DOXYGEN_TO_DO
 *  @param   COMPLEX24 *output DOXYGEN_TO_DO
 *  @param   count DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  -------------------------------------------------------------------------------------------------------------
 *  |Name             |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  -------------------------------------------------------------------------------------------------------------
 *  |COMPLEX64 *inter |              |              |              |              |              |              |
 *  -------------------------------------------------------------------------------------------------------------
 *  |COMPLEX24 *output|              |              |              |              |              |              |
 *  -------------------------------------------------------------------------------------------------------------
 *  |count            |              |              |              |              |              |              |
 *  -------------------------------------------------------------------------------------------------------------
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
void ConvertComplex64to24(COMPLEX64 *inter, COMPLEX24 *output, S16 count)
{
    S16 i;
#ifdef _WIN32
    S64 temp;
    temp = 16777216;
#endif

    for (i = 0; i < count; i++)
    {
        inter[i].real >>= 16;
        inter[i].imag >>= 16;

        output[i].real = (S24) inter[i].real;
        output[i].imag = (S24) inter[i].imag;

#ifdef _WIN32
        if (inter[i].real > temp)
        printf("ERROR: Convert Overshoot%f\t%d\treal\n", (double)inter[i].real/(double)temp, i+1);
        else if (inter[i].real < -temp)
        printf("ERROR: Convert Overshoot%f\t%d\treal\n", (double)inter[i].real/(double)temp, i+1);

        if (inter[i].imag > temp)
        printf("ERROR: Convert Overshoot%f\t%d\timag\n", (double)inter[i].imag/(double)temp, i+1);
        else if (inter[i].imag < -temp)
        printf("ERROR: Convert Overshoot%f\t%d\timag\n", (double)inter[i].imag/(double)temp, i+1);
#endif

#ifdef DEBUG_PRINT_IDFT
        if (inter[i].real > temp)
        fprintf(file_temp, "%s\t\t%f\t%d\treal\n", temp_string2, (double)inter[i].real/(double)temp, i+1);
        else if (inter[i].real < -temp)
        fprintf(file_temp, "%s\t\t%f\t%d\treal\n", temp_string2, (double)inter[i].real/(double)temp, i+1);

        if (inter[i].imag > temp)
        fprintf(file_temp, "%s\t\t%f\t%d\timag\n", temp_string2, (double)inter[i].imag/(double)temp, i+1);
        else if (inter[i].imag < -temp)
        fprintf(file_temp, "%s\t\t%f\t%d\timag\n", temp_string2, (double)inter[i].imag/(double)temp, i+1);
#endif
    }
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   COMPLEX24 input DOXYGEN_TO_DO
 *  @param   COMPLEX16 twiddle DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  --------------------------------------------------------------------------------------------------------------
 *  |Name              |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  --------------------------------------------------------------------------------------------------------------
 *  |COMPLEX24 input   |              |              |              |              |              |              |
 *  --------------------------------------------------------------------------------------------------------------
 *  |COMPLEX16 twiddle |              |              |              |              |              |              |
 *  --------------------------------------------------------------------------------------------------------------
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
COMPLEX64 MultiplyComplex24(COMPLEX24 input, COMPLEX16 twiddle)
{
    S64 temp[4];
    COMPLEX64 output;

    temp[0] = (S64) input.real * (S64) twiddle.real * 2;
    temp[1] = (S64) input.imag * (S64) twiddle.imag * 2;
    temp[2] = (S64) input.real * (S64) twiddle.imag * 2;
    temp[3] = (S64) input.imag * (S64) twiddle.real * 2;

    output.real = temp[0] - temp[1];
    output.imag = temp[2] + temp[3];

    return output;
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   COMPLEX24 *input DOXYGEN_TO_DO
 *  @param   length DOXYGEN_TO_DO
 *  @param   *width DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  -------------------------------------------------------------------------------------------------------------
 *  |Name             |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  -------------------------------------------------------------------------------------------------------------
 *  |COMPLEX24 *input |              |              |              |              |              |              |
 *  -------------------------------------------------------------------------------------------------------------
 *  |length           |              |              |              |              |              |              |
 *  -------------------------------------------------------------------------------------------------------------
 *  |*width           |              |              |              |              |              |              |
 *  -------------------------------------------------------------------------------------------------------------
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
void FindMax(COMPLEX24 *input, S16 length, S16 *width)
{
    S16 i;
    S64 inter_max, inter_min, temp;
    double index;

    inter_max = -8388608;
    inter_min = 8388607;

    for (i = 0; i < length; i++)
    {
        if (input[i].real > inter_max)
            inter_max = input[i].real;

        if (input[i].imag > inter_max)
            inter_max = input[i].imag;

        if (input[i].real < inter_min)
            inter_min = input[i].real;

        if (input[i].imag < inter_min)
            inter_min = input[i].imag;
    }

    inter_max = ((inter_max > -inter_min) ? inter_max : -inter_min);

    inter_min = 8388608;
    index = ((double) inter_min / (double) inter_max);

    index = floor(log10(index) / log10(2));

    if (index > 15.0)
        index = 15.0;

    *width = (S16) index;

    for (i = 0; i < length; i++)
    {
        temp = (S64) input[i].real * 32767;
        temp <<= *width;
        temp >>= 23;
        input[i].real = (S24) temp;

        temp = (S64) input[i].imag * 32767;
        temp <<= *width;
        temp >>= 23;
        input[i].imag = (S24) temp;

#ifdef _WIN32
        if (input[i].real > 32767 || input[i].real < -32768)
        printf("ERROR: Find Max: Something wrong\n");

        if (input[i].imag > 32767 || input[i].imag < -32768)
        printf("ERROR: Find Max: Something wrong\n");
#endif
    }

    *width = 8 - *width;

    return;
}
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
// Other Useful Functions
//-----------------------


//---------------------------------------------------------------
// Rounding Function for Floating Point
//---------------------------------------------------------------


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   input DOXYGEN_TO_DO
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
 *  |input         |              |              |              |              |              |              |
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
S16 Round(double input)
{
    char temp[500];
    S16 ret_int;
    sprintf(temp, "%.0f", input);

    ret_int = atoi(temp);
    return ret_int;
}

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------


