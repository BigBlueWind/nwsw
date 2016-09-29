#include "netdb.h"

hostent hostent_t;

//
hostent *gethostbyname (const char *_name)
{
    return ((hostent *)&hostent_t);
}

int *__h_errno_location (void)
{
  return 0;
}
