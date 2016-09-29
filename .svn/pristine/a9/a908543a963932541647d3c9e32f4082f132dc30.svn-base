/**
 * @file    imc_util.c
 * @brief   This file contains the functions that are of generic use.
 * @details Copyright Powerwave Technologies, Inc., 2009
 *
 * @author  Manu Sharma
 * @author  Ramprasad Chinta
 *
 *
 *@details
 *         1. added addModulo, subtractModulo functions   -  Ramprasad Chinta @n
 *         2. Added New feature -- Reading config file(imc.cfg) from user as argument for
 *         3. module Related Information(IP,port..etc)  on June 07 2010 --Madhu @n
 *         4  constructFullPath function is added on July 01 2010 --Madhu @n
 *
 */

#include "imc_util.h"
#include "bs_config.h"

/* Incase of windows we have driver names as prefixes for a path,
 * We need to remove the drive name from path
 */
#if defined _WIN32 || defined __WIN32__ || defined __EMX__ || defined __DJGPP__
/* Win32, OS/2, DOS */
  # define HAS_DEVICE(P) \
    ((((P)[0] >= 'A' && (P)[0] <= 'Z') || ((P)[0] >= 'a' && (P)[0] <= 'z')) \
     && (P)[1] EQ ':')
  # define FILESYSTEM_PREFIX_LEN(P) (HAS_DEVICE (P) ? 2 : 0)
  # define ISSLASH(C) ((C) EQ '/' || (C) EQ '\\')
#endif

#ifndef FILESYSTEM_PREFIX_LEN
  # define FILESYSTEM_PREFIX_LEN(Filename) 0
#endif

#ifndef ISSLASH
# define ISSLASH(C) ((C) EQ '/')
#endif

/**
* @param name The complete pathname
* @details 
* This function removes the directory names from the file path and returns the file name,
* as similar function exists in libgen.h
*/
UINT8 *getBasename (
    IN UINT8 *name
    )
{

  UINT8 IN *base = name += FILESYSTEM_PREFIX_LEN (name);
  SINT32 all_slashes = 1;
  UINT8 IN *p;

  for (p = name; *p; p++)
  {
    if (ISSLASH (*p))
    {
      base = p + 1;
    }
    else
      all_slashes = 0;
  }

  /* If NAME is all slashes, arrange to return `/'. */
  if (*base EQ '\0' && ISSLASH (*name) && all_slashes)
     --base;

  /* Make sure the last byte is not a slash. */
  assert (all_slashes || !ISSLASH (*(p - 1)));

  return (UINT8*) base;
}

/**
 * @param    eth        Ethernet interface
 * @param    ipAddr     IP address which will be filled
 * @param    ipNetMask  NetMask address which will be filled
 * @details: This function will get and fill the Ip address of eth0 in the ipAddr field.
 * */
TBOOL getIpAddressOfEth(
  IN UINT8     *eth,
  IO UINT8     *ipAddr,
  IO UINT8     *ipNetMask

  )
{
  int fd;
  struct ifreq ifr;
  if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) EQ -1 )
  {
    BS_ERROR_SOCK_CREATE_FAILED();
    return FALSE;
  }

  /* Get IPv4 IP address */
  ifr.ifr_addr.sa_family = AF_INET;
  /* Get IP address attached to "eth0" */
  strncpy(ifr.ifr_name, eth, IFNAMSIZ-1); /* IFNAMSIZ = 16 defined in inlude/linux/if.h*/
  /* Get IP Address */
  if( ioctl(fd, SIOCGIFADDR, &ifr) EQ -1 )
  {
    logError("%s: ioctl failed for eth=%s. Could not read IP Address! ioctl returned errno %d", __func__,eth, errno);
    close(fd);
    return FALSE;
  }
  strncpy( ipAddr, (char *) inet_ntoa( ((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr ), IFNAMSIZ);
  /*Get NetMask Address */
  if( ioctl(fd, SIOCGIFNETMASK, &ifr) EQ -1 )
    {
      logError("%s: ioctl failed for eth=%s. Could not read NetMask Address! ioctl returned errno %d", __func__,eth, errno);
      close(fd);
      return FALSE;
    }
  strncpy( ipNetMask, (char *) inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr), IFNAMSIZ);
  close(fd);

  return TRUE;
}
/**
 * @param    eth        Ethernet interface.
 * @param    ipAddr     IP address which  need to be SET.
 * @param    ipNetMask  NetMask address which need to be SET.
 * @details: This function will get and fill the Ip address of eth0 in the ipAddr field.
 * */
TBOOL setIpAddressOfEth(
  IN UINT8     *eth,
  IN UINT8     *ipAddr,
  IO UINT8     *ipNetMask
  )
{
  int fd;
  struct ifreq ifr;
  struct sockaddr_in sin;

  if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) EQ -1 )
  {
    BS_ERROR_SOCK_CREATE_FAILED();
    return FALSE;
  }

  /* get interface name */
  strncpy(ifr.ifr_name, eth, IFNAMSIZ);

  /* Read interface flags */
  if (ioctl(fd, SIOCGIFFLAGS, &ifr) < 0)
  {
    logError("%s: ioctl failed for eth=%s. Could not read Interface Flags! ioctl returned errno %d", __func__,eth, errno);
    close(fd);
    return FALSE;
  }

  sin.sin_family = AF_INET;

  /* Convert IP from numbers and dots to binary notation */
  inet_aton(ipAddr, &(sin.sin_addr));
  memcpy(&ifr.ifr_addr, &sin, sizeof(struct sockaddr));
   /* Set interface IP address */
  if (ioctl(fd, SIOCSIFADDR, &ifr) < 0)
  {
    logError("%s", "Cannot set IP address. ");
    close(fd);
    return FALSE;
  }
  else
  {
    logInfo("%s","Set suceeded fo IPAddress");
  }

  /* Convert NetMask from numbers and dots to binary notation */
  inet_aton(ipNetMask,&sin.sin_addr);
  memcpy(&ifr.ifr_addr, &sin, sizeof(struct sockaddr));
  /* Set interface NetMask address */
  if (ioctl(fd, SIOCSIFNETMASK, &ifr) < 0)
  {
    logError("%s", "Cannot set Netmask address. ");
    close(fd);
    return FALSE;
  }
  else
  {
    logInfo("%s","Set succeded for Netmask");
  }
  close(fd);
  return TRUE;

}


/**
* @param str The destination string
* @param val The value to be appended to string
* @details 
* This function sets the original string to contenet at val
*/
void setString100(
    IO String100 *str,
    IN UINT8 *val
    )
{
  if(strlen(val) < sizeof(String100) -1)
    sprintf(str[0], val);
}

/**
* @param str The destination string
* @param val The value to be appended to string
* @details 
* This function appends the val to the original string
*/
void appendString100(
    IO String100 *str,
    IN UINT8 *val
    )
{
  if(strlen(val) + strlen(str[0]) < sizeof(String100) -1)
    strcat(str[0], val);
}

/**
* @param src The source string
* @param dst The destination  string
* @details 
* This function reverses the string in the source and places the reversed string in the
* destination
*/
void reverseString(
    UINT8 *src,
    UINT8 *dst
    )
{
  SINT16 i = 0, len = 0, r = 0;
  len=strlen(src);

  for(i=len-1; i>=0; i--)
  {
         dst[r] = src[i];
         r++;
  }
  dst[r] = '\0';
}

/**
* @param modId  Module ID of the receiver
* @param timeoutMsgId The Message name of timeout indicator
* @details 
* This function Waits for a message to be received within MAX_MSG_WAIT_MSEC.
* If msg is received within this time, it is returned.
* At timeout, a TIMEOUT indication msg (identified by "timeoutMsgId")
* is returned.
*
* this function wakes up after every RECV_MSG_POLL_INTERVAL_MSEC interval and checks for messages
*/
#define MAX_MSG_WAIT_MSEC (45 * 1000)

Msg *recvMsgAtMod(
    IN ModuleId modId,
    IN MsgId timeoutMsgId
    )
{
  UINT16        recvMsgRetVal;
  Msg          *rcvMsg;
  Duratn      elapsed = 0;

  while(1)
  {
    rcvMsg = recvMsg( modId, &recvMsgRetVal);
    if( NULL EQ rcvMsg )
    {
      if( elapsed >= MAX_MSG_WAIT_MSEC )
      {
        MsgTimeoutInd msgTimeoutInd;
        memset( &msgTimeoutInd, 0, sizeof(MsgTimeoutInd) );
        rcvMsg = createMsg( timeoutMsgId );
        copyBytes( &msgTimeoutInd, sizeof(MsgTimeoutInd), &rcvMsg->data  );
        logInfo("TIMEOUT: %s", getMsgName(timeoutMsgId));
        break;
      }
      else
      {
        sleep_msec(RECV_MSG_POLL_INTERVAL_MSEC);
        elapsed += RECV_MSG_POLL_INTERVAL_MSEC;
      }
    }
    else
      break;
  }
  return rcvMsg;
}

/* The functionality of the below function is same as recvMsgAtMod,
 * the only difference is timeoutMsgWaitMsec is introduced so that
 * timeout duration for the expected Msg can be defined. */

Msg *recvMsgAtMod2(
    //IN ModuleId modId,
    IN MsgId    timeoutMsgId,
    IN UINT32   timeoutMsgWaitMsec
    )
{
  Msg        *rcvMsg = NULL;
  Duratn      elapsed = 0;

  while(1)
  {
    if( NULL EQ rcvMsg )
    {
      if( elapsed >= timeoutMsgWaitMsec )
      {
        MsgTimeoutInd msgTimeoutInd;
        memset( &msgTimeoutInd, 0, sizeof(MsgTimeoutInd) );
        rcvMsg = createMsg( timeoutMsgId );
        copyBytes( &msgTimeoutInd, sizeof(MsgTimeoutInd), &rcvMsg->data  );
        logInfo("TIMEOUT: %s", getMsgName(timeoutMsgId));
        break;
      }
      else
      {
        sleep_msec(RECV_MSG_POLL_INTERVAL_MSEC);
        elapsed += RECV_MSG_POLL_INTERVAL_MSEC;
      }
    }
    else
      break;
  }
  return rcvMsg;
}

/**
* @param received indicates received packet number
* @param expected indicates expected packet number
* @param modulo indicates the limit of window size
* @param result will be filled with missing packets
* @details 
* @todo chinta to add description
*/
TBOOL addModulo(
    IN UINT16 received,
    IN UINT16 expected,
    IN UINT16 modulo,
    IO UINT16 *result
    )
{
  TBOOL retVal = TRUE;

  if((received < modulo) && (expected < modulo))
  {
    *result = (received + expected) % modulo;
  }
  else
  {
    BS_ERROR_RECD_EXPECT_NOT_SAME();
    retVal = FALSE;
  }
  return retVal;
}

/**
* @param received indicates received packet number
* @param expected indicates expected packet number
* @param modulo indicates the limit of window size
* @param result will be filled with missing packets
* @details 
*
* @todo chinta to add description
*/
TBOOL subtractModulo(
    IN UINT16 received,
    IN UINT16 expected,
    IN UINT16 modulo,
    IO UINT16 *result
    )
{
  TBOOL retVal = TRUE;

  if((received < modulo) && (expected < modulo))
  {
    if(received >= expected)
    {
      *result = received - expected;
    }
    else
    {
      *result = modulo - expected + received;
    }
  }
  else
  {
    BS_ERROR_RECD_EXPECT_NOT_SAME( );
    retVal = FALSE;
  }
  return retVal;
}


/****************************************************************************************
 *   @author : Madhu
 *   Date   : 22-May-2010
 *   Description : This will take the module information from a text file as an input.
 *                 Text file will be placed in  NwSw folder.
 *                 Flag is added temporarily in Makefile, till the implementation is verified
 *                 and approved.
 *
 ***************************************************************************************/

/**
 * @param inputStr   is user Input String taken from file
 * @param tokens[]   is the array of tokens after parsing string
 * @details This function is used to Parse string for  \n=,{}  and divide it into tokens.
 */
UINT8 parseCmdString(
  IO UINT8           *inputStr,
  IO DataFromFileStr tokens[]
  )
{
  UINT8 *word;
  UINT32 index = 0;
  UINT8 *delim   = " \n=,{}";

  clearBytes(tokens,sizeof(tokens));
  word = strtok(inputStr, delim);
  if(word NOTEQ NULL)
  {
    strcpy(tokens[index++],word);
  }

  word = strtok(NULL, delim);
  while(word NOTEQ NULL)
  {
    strcpy(tokens[index++],word);
    word = strtok(NULL,delim);
  }
  return index;
}

/**
 * @param   picoBsHomePath   Configuration file path given by user
 * @details This function populates the framework context.
 *           Module identifier and the destination port address of a module
 *           are taken from configuration file.
 */
void initImcContext(
  IN UINT8*   fullFilePath
)
{
  ImcContext*      imcContext = NULL;
  TBOOL            isError = FALSE;

  imcContext = getGlobalImcContext();
  memset(imcContext, 0, sizeof(ImcContext));

  readImcCfgFile(fullFilePath, imcContext, &isError);

  if ( isError EQ TRUE )
  {
    logFatalErrorAndExit(EXIT_CONFIG_READ_FAIL, "Reading %s failed",fullFilePath);
  }
  msgCntrsRunning = TRUE;
}

/* @param picoBsHomePath : This is the config path directory where configurations are stored
 * @param filename       : This is the configuration file name
 * @param fullFilePath   : Output where full directory along with cfg filename appended.
 * @details   This function will append the file directory path to the file name
 *            and return the full configuration file path.
 * */
void constructFullPath(
  IN UINT8   *configFilePath,
  IN UINT8   *filename,
  IO UINT8    fullFilePath[]
  )
{
  String512  file;
  memset ( &file, 0, sizeof(file));

  strncpy( file, filename, strlen(filename));


  if ( strlen(configFilePath) EQ 0 )
  {
    logError("%s", "Environment variable LTEBSCONFIGDIR is not set; set this to the folder containing configuration files and restart system. example $export LTEBSCONFIGDIR=/home/picobs/rel1-2-3/config/");
   /* strcpy( fullFilePath, DEFAULT_CFG_FILE_DIR ); */
  }
  else
  {
    strcpy( fullFilePath, configFilePath );
    strcat( fullFilePath, "/");
  }
  strcat( fullFilePath, file);
}

/* @param number : This is the number to be converted to string
 * @param str    : This is the string in which converted string will be stored
 * @details   This function convert the integer to string
 * */
void itoa(
    IO SINT8 number,
    IO SINT8 str[]
    )
{
     int i, sign;

     if ((sign = number) < 0)  /* record sign */
         number = -number;          /* make n positive */
     i = 0;
     do {       /* generate digits in reverse order */
         str[i++] = number % 10 + '0';   /* get next digit */
     } while ((number /= 10) > 0);     /* delete it */
     if (sign < 0)
         str[i++] = '-';
     str[i] = '\0';
     reverse(str);
}


/* @param str    : This is the string in which reversed string will be stored
 * @details   This function reverses the string
 * */
void reverse(
    IO SINT8 str[]
    )
{
     int i, j;
     char c;

     for (i = 0, j = strlen(str)-1; i<j; i++, j--) {
         c = str[i];
         str[i] = str[j];
         str[j] = c;
     }
}

void imcReplaceByte(
    IO String50 str,
    IN SINT8     from ,
    IN SINT8     to
    )
{
  UINT8 i;

  for (i=0;i<=strlen(str);i++)
  {
    if(str[i] EQ from)
      str[i] = to;
  }
}

/* Used to get the Lapsed Time on usage of gettimeofday() function */
void getLapsedTime(
  IN struct timeval  *first,
  IO struct timeval  *second,
  IO struct timeval  *lapsed
)
{
  if (first->tv_usec > second->tv_usec)
  {
    second->tv_usec += 1000000;
    second->tv_sec--;
  }
  lapsed->tv_usec = second->tv_usec - first->tv_usec;
  lapsed->tv_sec = second->tv_sec - first->tv_sec;
}
