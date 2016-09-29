/**
 * @file    imc_sig.c
 * @brief This file contains signal handling routines for OS signals
 * @details Copyright Powerwave Technologies, Inc., 2009
 *
 * @author  Manu Sharma
 *
 */

#define _GNU_SOURCE  /*This macro is needed for strsignal() function */
#include <string.h>
#include <wait.h>
#include "imc_sig.h"
#include "imc.h"
#include <execinfo.h> /* GNU extensions: backtrace() */

#define MAX_FUNC_DEPTH 10 /* Max function depth that is expected */
void showBacktrace(void)
{
  void  *buffer[MAX_FUNC_DEPTH];
  int    numFunc;
  char **funcNames = NULL;
  
  numFunc = backtrace(buffer, MAX_FUNC_DEPTH);
  if (numFunc EQ MAX_FUNC_DEPTH)
    logError("backtrace truncated, backtrace buffer is of size %d, increase MAX_FUNC_DEPTH to avoid this error", MAX_FUNC_DEPTH);
 
  funcNames = backtrace_symbols(buffer, numFunc); /* funcNames is malloced */
  if (funcNames NOTEQ NULL)
  {
    logInfo("%s", "Backtrace-->");
    while(numFunc>0) 
    {
      logInfo("%s", funcNames[numFunc-1]); 
      numFunc--;
    }
    free(funcNames);
  }
}

void showImcMsgCntrs()
{
  logSendMsgCntrs();
  logSendMsgErrCntrs();
  logRcvdMsgCntrs();
  logRcvdMsgErrCntrs();   
}

/**
 * @param   signum Signal received by the process from OS
 * @details This function prints the received signal, and the process stops execution.
 * This is the last function executed when a signal is received.
 */
void sigHandler (int signum)
{
  int   status;
  pid_t pid;
  logInfo("Received signal=%s(%d)", strsignal(signum), signum);
   switch(signum)
   {
     case SIGUSR1:
       /* This shows message counters status on IMC */
       if(TRUE EQ msgCntrsRunning)
         showImcMsgCntrs();
       /* This is just an example of how backtrace can be printed to log.
        * As it stands, its pratical utility is very limited.
        */
       /* showBacktrace();*/
       break;
     case SIGUSR2:
       /* This signal will be called for Physhutdown automatically
        * when base station wants to stop 
        */
       createAndSendMsg(MSG_IMC_PRM_PHYSHUTDOWN_CMD,NULL,0);
       break;
     case SIGCHLD:
         pid = waitpid( 0, &status, (WNOHANG | WUNTRACED | WCONTINUED ) );
         logInfo("PID : %d has sent SIGCHLD.. Child process has terminated/stopped", pid );
    	 break;   /* 02 Dec 2010 */
     default:
       logInfo("%s", "EXITING");
       fflush(NULL); /* flush log files */
       exit(EXIT_FAILURE);
   }
}

/**
 * @details This function waits for a child to exit.
 *          It returns immediately if no child signal is to be claimed.
 */
pid_t waitForChld( pid_t pidChld)
{
  int status;
  pid_t pid;

  logInfo("Waiting for child with PID=%d", (int)pidChld );
  pid = waitpid( 0, &status, (WNOHANG | WUNTRACED | WCONTINUED ));

  return pid;
}

/**
 * @details This function sends the provided signal to the process.
 *      return's negative values on error, if PID ==0, then the process which called kill
 *      will be killed, take care not to call kill on pid=0
 */
SINT32 killChld( pid_t pid, SINT32 signum)
{
  if( 0 EQ pid )
  {
    logError("%s","Process ID ZERO entered, this will kill the caller process");
    return -2;
  }

  logInfo("Sending signal=%s(%d) for child with PID=%d", strsignal(signum), signum, (int)pid );

  return(  kill( pid, signum ) );
}

/**
 * @details This function registers sigHandler as the default function to be
 *          invoked when a signal is received by a process.
 */
void setSigActions(void)
{
  struct sigaction sa;
  memset (&sa, 0, sizeof (sa));
  sa.sa_handler = &sigHandler;
  sigaction (SIGUSR1, &sa, NULL);
  sigaction (SIGUSR2, &sa, NULL);
  sigaction (SIGINT,  &sa, NULL);
  sigaction (SIGTERM, &sa, NULL);
  sigaction (SIGTSTP, &sa, NULL);
  sigaction (SIGXCPU, &sa, NULL); /* CPU Limit exceeded */
  sigaction (SIGXFSZ, &sa, NULL); /* File size limit exceeded */
  /* Do not handle SIGALM here.
   * It is used by timer.
   * See imc_timer.c
   */

  /* For sig Child Let OS handle,
   * This prevents zombie creation
   * Manu: 29-Oct: ignoring causes system() to fail
   */
  sigaction (SIGCHLD, &sa, NULL);

}

