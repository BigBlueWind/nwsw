/*---------------------------------------------------------------
 * Copyright (c) 1999,2000,2001,2002,2003                              
 * The Board of Trustees of the University of Illinois            
 * All Rights Reserved.                                           
 *--------------------------------------------------------------- 
 * Permission is hereby granted, free of charge, to any person    
 * obtaining a copy of this software (Iperf) and associated       
 * documentation files (the "Software"), to deal in the Software  
 * without restriction, including without limitation the          
 * rights to use, copy, modify, merge, publish, distribute,        
 * sublicense, and/or sell copies of the Software, and to permit     
 * persons to whom the Software is furnished to do
 * so, subject to the following conditions: 
 *
 *     
 * Redistributions of source code must retain the above 
 * copyright notice, this list of conditions and 
 * the following disclaimers. 
 *
 *     
 * Redistributions in binary form must reproduce the above 
 * copyright notice, this list of conditions and the following 
 * disclaimers in the documentation and/or other materials 
 * provided with the distribution. 
 * 
 *     
 * Neither the names of the University of Illinois, NCSA, 
 * nor the names of its contributors may be used to endorse 
 * or promote products derived from this Software without
 * specific prior written permission. 
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
 * NONINFRINGEMENT. IN NO EVENT SHALL THE CONTIBUTORS OR COPYRIGHT 
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. 
 * ________________________________________________________________
 * National Laboratory for Applied Network Research 
 * National Center for Supercomputing Applications 
 * University of Illinois at Urbana-Champaign 
 * http://www.ncsa.uiuc.edu
 * ________________________________________________________________ 
 * main.cpp
 * by Mark Gates <mgates@nlanr.net>
 * &  Ajay Tirumala <tirumala@ncsa.uiuc.edu>
 * -------------------------------------------------------------------
 * main does initialization and creates the various objects that will
 * actually run the iperf program, then waits in the Joinall().
 * -------------------------------------------------------------------
 * headers
 * uses
 *   <stdlib.h>
 *   <string.h>
 *
 *   <signal.h>
 * ------------------------------------------------------------------- */

#define HEADERS()

#include "headers.h"
#include "Condition.h"
#include "Settings.hpp"
#include "List.h"
#include "locale.h"
#include "PerfSocket.hpp"
#include "Timestamp.hpp"
#include "util.h"
#include "iperf.h"

#ifdef IPERF_SERVER
#include "Listener.hpp"
#endif

/* -------------------------------------------------------------------
 * prototypes
 * ------------------------------------------------------------------- */
// Function called at exit to clean up as much as possible
void cleanup( void );
void _gnu_getopt_init(void);
/* -------------------------------------------------------------------
 * global variables
 * ------------------------------------------------------------------- */
extern "C" {
    // Global flag to signal a user interrupt
    int sInterrupted = 0;
    // Global ID that we increment to be used 
    // as identifier for SUM reports
    int groupID = 0;
    
    /* 1003.2 says this must be 1 before any call.  */
    extern int gnu_optind;
    
    /* Formerly, initialization of gnu_getopt depended on gnu_optind==0, which
    causes problems with re-calling gnu_getopt as programs generally don't
    know that. */
    
    extern int __gnu_getopt_initialized;    
}
  nthread_t sThread;
/* -------------------------------------------------------------------
 * main()
 *      Entry point into Iperf
 *
 * sets up signal handlers
 * initialize global locks and conditions
 * parses settings from environment and command line
 * starts up server or client thread
 * waits for all threads to complete
 * ------------------------------------------------------------------- */
extern "C" int iperf( int argc, char **argv ) 
{
	sInterrupted = 0;
    // Initialize the thread subsystem
    thread_init( );

    // Initialize the interrupt handling thread to 0
    sThread = thread_zeroid();

    // perform any cleanup when quitting Iperf
    atexit( cleanup );

    // Allocate the "global" settings
    thread_Settings* ext_gSettings = new thread_Settings;

    _gnu_getopt_init();
    
    // Initialize settings to defaults
    Settings_Initialize( ext_gSettings ); 
    // read settings from environment variables
    Settings_ParseEnvironment( ext_gSettings );

    // read settings from command-line parameters
    Settings_ParseCommandLine( argc, argv, ext_gSettings );

    // Check for either having specified client or server
    if ( ext_gSettings->mThreadMode == kMode_Client 
         || ext_gSettings->mThreadMode == kMode_Listener ) {
        // initialize client(s)
        if ( ext_gSettings->mThreadMode == kMode_Client ) {
            client_init( ext_gSettings );
        }
        // No need to make a reporter thread because we don't have threads
        thread_start( ext_gSettings );
    } else {
        // neither server nor client mode was specified
        // print usage and exit

        fprintf( stderr, usage_short, argv[0], argv[0] );
        uart_printf("incorrect usage\r\n");
        return 0;
    }

    // all done!
    uart_printf("done\r\n");
    return 0;
} // end main

/* -------------------------------------------------------------------
 * Any necesary cleanup before Iperf quits. Called at program exit,
 * either by exit() or terminating main().
 * ------------------------------------------------------------------- */
void cleanup( void ) {
    // clean up the list of clients
    Iperf_destroy ( &clients );

    // shutdown the thread subsystem
    thread_destroy( );
} // end cleanup

void _gnu_getopt_init(void) {
  /* 1003.2 says this must be 1 before any call.  */
  gnu_optind = 1;
  
  /* Formerly, initialization of gnu_getopt depended on gnu_optind==0, which
  causes problems with re-calling gnu_getopt as programs generally don't
  know that. */
  
  __gnu_getopt_initialized = 0;  
}


void interrupt_iperf(void)
{
	sInterrupted = true;
}

