/**
 * @file    imc_err.h
 * @brief Error Logging Functions
 * @details Copyright Powerwave Technologies, Inc., 2009
 */

#ifndef IMC_ERR_H
#define IMC_ERR_H

#include "cmn_types.h"

/* used to clear the chain */
void emptyErrorChain (IO ErrChain *chain);

/* used to add/log an error */
void reportError (
  const char *format,
  ...
  );

/* used to print the errors to log and empty the chain */
void printAndEmptyErrorChain (IO ErrChain *chain);

#endif /* IMC_ERR_H */
