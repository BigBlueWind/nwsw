/**
 * @file    imc_err.c
 * @brief This file contains functions for error logging
 * @details Copyright Powerwave Technologies, Inc., 2009
 */

#include <stdarg.h>
#include "log.h"
#include "imc_module.h"
#include "imc_err.h"

/** 
 * @param   Pointer to Error Chain.
 * @details
 * API function.
 * Applications should call this to clear/empty errorChain
 * Application should not do memset/clearBytes as this is more optimal.
 */

void emptyErrorChain (IO ErrChain *chain)
{
  chain->index = 0; /* Mark that chain has no entries */
}

/** 
 * @param   Pointer to Error Chain.
 * @details 
 * This function sets the bytes in ErrorChain to 0.
 * Applications should use emptyErrorChain().
 */
void clearErrorChain (IO ErrChain *chain)
{
  memset(chain, 0, sizeof(ErrChain));
  emptyErrorChain(chain);
}

/** 
 * @param   Pointer to Error Chain.
 * @details 
 * API function.
 * This function prints the errors in the ErrorChain to logFile.
 * Applications should typically call this when a procedure is complete.
 * Example:
 *  handleMsg(|)
 *            |-->calls doA(|);
 *            |             |-->calls doB(|);
 *            |             |             |-->calls doC();
 *            |             |             |  encounters error E1, adds to errorChain;
 *            |             |             |<--returns;
 *            |             | reports its error E2;
 *            |             | fills errorChain;
 *            |             |<--returns;
 *            | reports its error E3;
 *            | fills errorChain;
 *            | <-- returns;
 *  printAndEmptyErrChain(); -> prints E1, E2, E3
 */
 
void printAndEmptyErrorChain (IO ErrChain *chain)
{
  Index i;
  for(i=0;i<chain->index;i++)
  {
    logError("chain# %d:%s", i, chain->buf[i]);
  }
  emptyErrorChain(chain);
}

/** 
 * @param   Pointer to Error Chain.
 * @details
 * API function.
 * This function should be used by applications to log an error.
 */
void reportError(
  const char *format,
  ...
  )
{
  va_list args;
  ErrChain *errChain = &(getModuleContext()->errChain);
  va_start(args, format);
  
  if(errChain->index >= MAX_NO_ERR)
  {
    logError("ERROR CHAIN IS FULL (capacity=%d, index=%d)", MAX_NO_ERR, errChain->index);
    printAndEmptyErrorChain (errChain);
  }
  /* collect the error, dont print as yet */
  vsnprintf(errChain->buf[errChain->index],MAX_ERR_LEN,format,args);
  errChain->index +=1;
  va_end(args);
}

/* ** CHANGE HISTORY (Date, Who, What) ******************60
 * 05Oct, Manu, Added Error Chain Handling
 */
