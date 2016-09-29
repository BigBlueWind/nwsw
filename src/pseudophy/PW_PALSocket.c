/******************************************************************************
* Copyright: (C) 2010 Polaris Networks. All Rights Reserved.                  *
*    No part of this source code may be reproduced, transmitted, transcribed  *
*    or translated into any language without the written permission of        *
*    Polaris Networks or its suppliers or affiliate companies. To obtain this *
*    permission write to the attention of Legal counsel, Polaris Networks,    *
*    75 Robbins Road, MA 02421 United States.                                 *
******************************************************************************/

#include <string.h>
#include <stdlib.h>
#ifdef LINUX
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include "PW_PALDefinition.h"
#include "PW_PALSocket.h"
#include "MemoryDebug.h"
#include "Logger.h"
#include "envdep.h"

#define __sModuleName__ "SOKT"

static char         s_arrcUeAgentIp[20];
int                 iUeCount = 1;
int                 iServicePort = SERVICEPORT;
#define PAL_MAX_CFG_FILENAME_LEN    200

extern S16 msArgc;              /* argc */
extern S8 **msArgv;            /* argv */
int _SKReadCfgFile()
{
  char         *rootDir = NULL;
  char         filename[PAL_MAX_CFG_FILENAME_LEN];
  int          argc;
  char         **argv;
  FILE         *fp = NULL;

  argc = msArgc;
  argv = msArgv;

  rootDir = argv[2];
  if ( !rootDir )
  {
    fprintf( stderr, "Environment variable --rootDir is not set !");
    exit(EXIT_FAILURE);
  }

  sprintf( filename,"%s/config/PW_PAL.cfg", rootDir );
  fp = fopen(filename, "r");
    if(fp != NULL)
    {
        char readStr[100];
        while(fgets(readStr, sizeof(readStr), fp))
        {
            char str1[20], str2[20];
            
            sscanf(readStr, "%s = %[^;]", str1, str2);
            if(strcmp(str1, "UE_AGENT_IP") == 0)
            {
                strcpy(s_arrcUeAgentIp, str2);
                continue;
            }
            if(strcmp(str1, "UE_COUNT") == 0)
            {
                iUeCount = atoi(str2);
                if(iUeCount > MAX_UE_COUNT)
                {
                    iUeCount = MAX_UE_COUNT;
                }
                continue;
            }
            if(strcmp(str1, "UE_AGENT_PORT") == 0)
            {
                iServicePort = atoi(str2);
                continue;
            }
        }
        fclose(fp);
        return PN_SUCCESS;
    }
    else
    {
        return PN_FAILURE;
    }
}

int SKInitializeSocketModule(int *piSockFd)
{
    struct sockaddr_in sockaddress;
    int iSockaddrLen = 0;
    int iYes = 1;
    int count;

    if(_SKReadCfgFile() != PN_SUCCESS)
    {
        LOG_ERROR(ELOG_MESSAGE, "Unable to read Pseudo PHY configuration file");
        return PN_FAILURE;
    }

    for(count = 0; count < iUeCount; count++)
    {
        piSockFd[count] = socket(AF_INET, SOCK_DGRAM, 0);
        if ( piSockFd[count] < 0 )
        {
            return PN_FAILURE;
        }

        memset ((char*) &sockaddress, 0, sizeof(sockaddress));
        sockaddress.sin_family = AF_INET;
        sockaddress.sin_port = htons(iServicePort + count);
        sockaddress.sin_addr.s_addr = htonl(INADDR_ANY);

        iSockaddrLen = sizeof ( struct sockaddr_in );

        if(bind (piSockFd[count], ( struct sockaddr* )&sockaddress,
            iSockaddrLen) < 0)
        {
            return PN_FAILURE;
        }

        if(setsockopt(piSockFd[count], SOL_SOCKET, SO_REUSEADDR,
            &iYes, sizeof (iYes)) < 0)
        {
            return PN_FAILURE;
        }
    }
    return PN_SUCCESS;

}

int SKReadFromSocket(char *pcRxBuffer, int iSockFd)
{
    struct sockaddr_in sClientAddr;
    unsigned int uiAddrLen = 0;
    int iLen = PW_MAX_MSG_LENGTH;
    int iBytesRead = 0;

    memset (&sClientAddr, 0, sizeof(struct sockaddr_in));
    uiAddrLen = sizeof(struct sockaddr_in);

    if (iSockFd != 0)
    {
        iBytesRead = recvfrom(iSockFd, pcRxBuffer, iLen, 0,
            (struct sockaddr *)&sClientAddr, &uiAddrLen);
        if (iBytesRead > 0)
        {
            pcRxBuffer[iBytesRead] = '\0';
        }
    }

    return iBytesRead;
}

int SKWriteOverSocket(unsigned char* pucMsg, unsigned int uiMsgLen, int iSockFd,
                      unsigned int uiPort)
{    
    struct sockaddr_in sPalAddr;    
    UTILMemset(&sPalAddr, 0, sizeof(sPalAddr));    
    sPalAddr.sin_family      = AF_INET;    
    sPalAddr.sin_port        = htons(uiPort);    
    sPalAddr.sin_addr.s_addr = inet_addr(s_arrcUeAgentIp);
    if (sendto(iSockFd, pucMsg, uiMsgLen, 0,         
        (const struct sockaddr *)&sPalAddr, sizeof(sPalAddr)) == -1)    
    {        
        LOG_ERROR(ELOG_MESSAGE, "Unable to send packet to UE PHY");        
        return PN_FAILURE;    
    }
    return PN_SUCCESS;}


int SKCloseSocketModule(int *piSockFd)
{
    int count;    
    for(count = 0; count < iUeCount; count++)    
    { 
        close(piSockFd[count]);
        piSockFd[count] = -1;
    }
  return PN_SUCCESS;
}

