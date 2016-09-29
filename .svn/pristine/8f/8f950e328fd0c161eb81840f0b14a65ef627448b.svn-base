
#define HOST_NOT_FOUND   1
#define NO_ADDRESS       4
#define NO_RECOVERY      3
#define TRY_AGAIN        2

/* Description of data base entry for a single host.  */
typedef struct _hostent_t
{
  char *h_name;         /* Official name of host.  */
  char **h_aliases;     /* Alias list.  */
  int h_addrtype;       /* Host address type.  */
  int h_length;         /* Length of address.  */
  char **h_addr_list;       /* List of addresses from name server.  */
#if defined __USE_MISC || defined __USE_GNU
# define    h_addr  h_addr_list[0] /* Address, for backward compatibility.*/
#endif
} hostent;

hostent *gethostbyname (const char *_name);

/* Error status for non-reentrant lookup functions.
   We use a macro to access always the thread-specific `h_errno' variable.  */
#define h_errno (*__h_errno_location ())

/* Function to get address of global `h_errno' variable.  */
int *__h_errno_location (void);

