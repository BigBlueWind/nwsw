#include "iperf.h"
#include "argv_parser.h"

char *g_ppsArgv[MAX_IPERF_COUNT_PARAMS];

//*****************************************************************************
//
//! \addtogroup argv_parser_api
//! @{
//
//*****************************************************************************
//*****************************************************************************
//
// External declaration for the functions and variables.
//
//*****************************************************************************

//*****************************************************************************
//
//!  Code for parsing literal string which represents IPERF
//!  command lineuse into argv array.
//!
//!  \param  string  pointer to the string contains params for IPERF
//!
//!  \return none    count of arguments in string
//!
//!  \brief          code for parsing literal string which represents IPERF
//!                  command lineuse into argv array.
//
//*****************************************************************************
int 
ConvertStringToArgv(unsigned char *string)
{
    // BE CAREFULL - for now we change the string content directly
    int iArgc;

    //
    //first element are empty in string array - we emulate program name
    //which usually should be at first place in argv[0] element
    //
    iArgc = 1;

    //
    // argv array for ipref, we clear first element
    //
    g_ppsArgv[0] = 0;

    while(1) 
    {
        //
        // We assume we receive command string from HCI tester
        // we go across space and any letter
        // If empty string - we exit
        //
        if(0 == *string)
        {
            break;
        }

        //
        // wee need this peace of code in case the string with parameters
        // begins with space or multiple spaces
        //
        while(*string && (' ' == *string))
        {
            string++;
        }

        //
        // we add parameter into Argv and zero next element
        //
        g_ppsArgv[iArgc++] = (char *)string;
        g_ppsArgv[iArgc] = 0;

        //
        // find space between parameter in order to split them
        //
        while(*string && (' ' !=*string))
        {
            string++;
        }

        //
        // we found end of string - exit
        //
        if(0 == *string)
        {
            break;
        }

        //
        // here we put 0 instead of space symbol
        //
        if(*string)
        {
            //
            // Null terminate and advance to next char
            //
            *string++ = 0;
        }
    }
    return(iArgc);
} 

void
vs_iperf_set_param(unsigned char *outBuff)
{
    int i;

    i = ConvertStringToArgv (outBuff);
    iperf(i, g_ppsArgv);
}



//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
