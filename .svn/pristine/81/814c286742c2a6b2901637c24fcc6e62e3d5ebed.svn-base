/**
 * @file    emd_sim.c
 * @brief This modules is responsible for registering the EnodeB and setting the EnodeB Identifier.
 * @details Copyright Powerwave Technologies, Inc., 2011
 *
 * @author : Krupakar G
 * Date   :
 * Description :
 */

#include "all_cmn.h"
#include "imc_proc.h"
#include "log.h"
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/stat.h>
#include "bs_tables.h"
#define SERVERPORT 162
#define BUFSIZE 500
#define OIDSIZE 35

TBOOL readSimulatorConfigFile();

typedef struct EmsContext
{
  UINT8 version[MAX_SIZE_SWPACKAGESTATUS_VERSION + 1];
} EmsContext ;

EmsContext g_emsContext;

#define logEmsInfo(format, values)   if ( isLoggingEnabled EQ TRUE )  logInfo(format, values);

#define logEmsError(format, values)  if ( isLoggingEnabled EQ TRUE )  logError(format, values);


int main(int argc, char *argv[])
{
	int sockfd, read=0, addrlen, flag, i;
	TBOOL isConfigReadSuccess = FALSE;
	TBOOL isLoggingEnabled = TRUE;
	struct sockaddr_in server, client;
	FILE *emsagentconfigfd= NULL;
	char clientip[20];
	int clientport = 0;
	unsigned char recvbuf[BUFSIZE];
	unsigned id = 1000;
	int setsuccess = 0;
	struct stat filestatus;

	char registrationOID[OIDSIZE];
	char enodebidOID[OIDSIZE];
	char agentipport[50];
	char enodebid[10];

	char *sendregisbuf[14]={"snmpset", "-v", "1", "-c", "aussie", "", "",
				"i", "1", "", "i", "","on", (char *) NULL};

	snprintf( registrationOID, OIDSIZE, ".1.3.6.1.4.1.9344.501.%d.1.1.6.0", ENODEBCONFIG );
	snprintf( enodebidOID, OIDSIZE, ".1.3.6.1.4.1.9344.501.%d.1.1.1.0", ENODEBCONFIG );
	sendregisbuf[6] = registrationOID;
	sendregisbuf[9] = enodebidOID;

	clearBytes( &g_emsContext, sizeof(EmsContext));

	/* Reading simulatorsConfig.cfg file to get version of EMS Simulator */
	if ( FALSE EQ readSimulatorConfigFile())
        {
	  isConfigReadSuccess = FALSE;
        }
	else
	{
	  isConfigReadSuccess = TRUE;
	}

        /* Checking for option --version |--log-on | --log-off */
	if(argc > 1)
	{
	  /* Version Info */
	  if( 0 EQ strncmp( argv[1], "--version", sizeof("--version")))
	  {
	    if(isConfigReadSuccess EQ TRUE)
	    {
             // printf("EMS Simulator Version: %s\n", g_emsContext.version);
              exit(0);
	    }
	    else
	    {
	      printf("[ERR: Cannot read simulator configuration file. EMS Simulator version unknown.]\n");
	      exit(0);
	    }
	  }
	  else if( 0 EQ strncmp( argv[1], "--log-off", sizeof("--log-off")))
	  {
	    /* Disable Logging */
            isLoggingEnabled = FALSE;
            sendregisbuf[12] = "off";
	  }
	  else if( 0 EQ strncmp( argv[1], "--log-on", sizeof("--log-on")))
          {
            /* Disable Logging */
            isLoggingEnabled = TRUE;
          }
	  else if( 0 EQ strncmp( argv[1], "--help", sizeof("--help")))
	  {
	    /* Display Help Information */
            printf("Available Options:\n");
            printf("1. ./ems_sim.exe              : Start EMS Simulator.\n");
            printf("2. ./ems_sim.exe --version    : Get EMS Simulator version.\n");
            printf("3. ./ems_sim.exe --log-on     : Start EMS Simulator with Logging ENABLED.\n");
            printf("4. ./ems_sim.exe --log-off    : Start EMS Simulator with Logging DISABLED.\n");
            exit(0);
	  }
	  else
	  {
	    printf("ERR: Unknown Option\n");
	    printf("Try --help to get more information.\n");
	    exit(0);
	  }
	}

	/* Initialise Logging */
	if( isLoggingEnabled EQ TRUE )
	{
	  logInit( "ems_sim.exe", NULL );
	}

        /* Printing EMS Simulator Startup banner */
	logInfo("================= EMS SIMULATOR LOG ==============", NULL);
//	fprintf(log,"================= EMS SIMULATOR LOG ============== \n");
	if(isConfigReadSuccess EQ TRUE)
        {
	  logInfo("EMS Simulator Version: %s", g_emsContext.version);
  //        printf("EMS Simulator Version: %s\n", g_emsContext.version);
        }
        else
        {
          logInfo("EMS Simulator Version: UNKNOWN. [ERR: Simulator configuration file missing.]", NULL);
    //      printf("EMS Simulator Version: UNKNOWN. [ERR: Simulator configuration file missing.]\n");
        }
	if( isLoggingEnabled EQ TRUE )
        {
	  logInfo("Logging: ENABLED", NULL);
      //    printf("Logging: ENABLED\n");
        }
	else
	{
	  logInfo("Logging: DISABLED", NULL);
//	  printf("Logging: DISABLED\n");
	}

	if( (sockfd=socket( AF_INET, SOCK_DGRAM, 0)) == -1)
        {
          logError("Socket Failure :%d.", errno);
   	  printf("Socket Failure :%d.\n",errno);
          exit(0);
        }

	/* Checking snmpset executable file */
	if( -1 EQ stat("snmpset", &filestatus))
	{
	  logError("'snmpset' executable missing :%d. Please check 'snmpset' file is present.", errno);
  // 	  printf("'snmpset' executable missing :%d. Please check 'snmpset' file is present.\n",errno);
          exit(0);
	}
	/* Checking for executable permissions on snmpset file */
	if( !(filestatus.st_mode & S_IXUSR) )
	{
	  logError("%s", "Executable permissions on 'snmpset' missing. Please check 'snmpset' file permissions.");
   	//  printf("Executable permissions on 'snmpset' missing. Please check 'snmpset' file permissions.\n");
          exit(0);
	}

        /* Configuring EMS server */
	bzero( (void *) &server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVERPORT);
	server.sin_addr.s_addr = INADDR_ANY;

	if( bind(sockfd, (struct sockaddr *) &server, sizeof(server)) == -1)
	{
		logError("Port already in Use. Bind Failure : %d.", errno);
	//	printf("Port already in Use. Bind Failure : %d.\n", errno);
		exit(0);
	}

        /* Reading EnodeB Agent Port Configuration File */
	if( (emsagentconfigfd = fopen("enodebagentport.cfg","r")) EQ NULL )
	{
		logError("Cannot Open ENodeB Agent Port Configuration File.", NULL);
	//	printf("[WARNING: Cannot Open ENodeB Agent Port Configuration File. Default port numbers will be used.]\n");
	}
	else
	{
		logInfo("Reading ENodeB Agent Port Configuration File Success", NULL);
		//printf("Reading ENodeB Agent Port Configuration File Success\n");
	}


	addrlen = sizeof(struct sockaddr);
	logInfo("SNMP Server waiting for TRAPS on Port 162 ....", NULL);
	//printf("SNMP Server waiting for TRAPS on Port 162 ....\n");
	while(1)
	{
		flag=0; clientport = 0;
		if( (read = recvfrom(sockfd, recvbuf, BUFSIZE, 0, (struct sockaddr *) &client, &addrlen )) < 0)
		{
			logError("Receive Error.", NULL);
			recvbuf[0]='\0';
		}
		else
		{
			recvbuf[read]='\0';
			recvbuf[14]= 0xa2;

			/* Checking for the required trap*/
			for (i=0; i <= read - 13; i++)
			{
				if(recvbuf[i] EQ 0x2b && recvbuf[i+1] EQ 0x06 && recvbuf[i+2] EQ 0x01 && recvbuf[i+3] EQ 0x04 && recvbuf[i+4] EQ 0x01
					&& recvbuf[i+5] EQ 0xc9 && recvbuf[i+6] EQ 0x00 && recvbuf[i+7] EQ 0x83 && recvbuf[i+8] EQ 0x75
					&& recvbuf[i+9] EQ 0x83 && recvbuf[i+10] EQ 0x75 && recvbuf[i+11] EQ 0x01 && recvbuf[i+12] EQ 0x11)
				{
					flag=1;
					recvbuf[14]= 0xa2;

					logInfo( "Received GET Request Message From:", NULL);
					logInfo( "ENodeB IP Address:  %s", inet_ntoa(client.sin_addr));
					logInfo( "ENodeB Port Number: %d", ntohs(client.sin_port));
	//				printf( "Received GET Request Message From:\n");
    //                                    printf( "ENodeB IP Address:  %s\n", inet_ntoa(client.sin_addr));
      //                                  printf( "ENodeB Port Number: %d\n", ntohs(client.sin_port));
					/* Sending the response for the trap */
					if ( sendto(sockfd, recvbuf, read, 0, (struct sockaddr *) &client,(socklen_t) addrlen ) EQ read )
					{
						logInfo("Response for GET Request Sent Successfully on PORT: %d.", ntohs(client.sin_port));
					}
					else
					{
						logError("Error in Sending Response to GET Request.", NULL);
					}
					client.sin_port=htons(161);
			
					/* Checking the ENodeB Agent Config File for registered ENodeBs */
					if( emsagentconfigfd NOTEQ NULL)
					{
						rewind(emsagentconfigfd);
						while( fscanf( emsagentconfigfd, "%s %d", clientip, &clientport) NOTEQ EOF )
						{
							if( strcmp( clientip, inet_ntoa(client.sin_addr))EQ 0 )
							{
								client.sin_port = htons(clientport);
								break;
							}
						}

					}
					break;
				}
			}
			if(flag EQ 1)
			{
				snprintf(agentipport, sizeof(agentipport), "%s:%d", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
				snprintf(enodebid, sizeof(agentipport), "%d", id);
				sendregisbuf[5] = agentipport;
				sendregisbuf[11] = enodebid;
				/* Setting the ENodeB Registration to TRUE and ENodeB Identifier*/
				if( (setsuccess= fork()) EQ 0 )
				{
					if( execv("snmpset", sendregisbuf) EQ -1)
					{
						logError("SNMP SET for ENodeB Registration and ENodeB Id Failed.", NULL);
				//		printf("SNMP SET for ENodeB Registration and ENodeB Id Failed.\n");
					}
				}
				else
				if( setsuccess > 0 )
				{
					logInfo("Setting ENodeB Id: %d Setting ENodeB Registration: TRUE", id);
					logInfo("SNMP SET for ENodeB Id and ENodeB Registration Sent to Port: %d.", ntohs(client.sin_port));
				//	printf("Setting ENodeB Id: %d \nSetting ENodeB Registration: TRUE\n", id );
				//	printf("SNMP SET for ENodeB Id and ENodeB Registration Sent to Port: %d.\n", ntohs(client.sin_port) );
					id++;
					sleep(1);
				}
				else
				{
					logInfo("SNMP SET for ENodeB Registration and ENodeB Id Failed.", NULL);
				//	printf("SNMP SET for ENodeB Registration and ENodeB Id Failed.\n");
				}
			}
			setsuccess = 0;
			memset(recvbuf,'\0',BUFSIZE);
			read = 0;
		}
	}
}

/**
 * @details This function reads the simulatorConfig file to get EMS simulator version
 */

TBOOL readSimulatorConfigFile( )
{
  config_t          cfg;
  config_setting_t  *table;
  UINT8             fullFilePath[MAX_BUFFER_SIZE];
  const char        *version;

  clearBytes( &fullFilePath, sizeof(fullFilePath));

  config_init(&cfg);
  snprintf( fullFilePath, MAX_BUFFER_SIZE, "%s", "../simulators/simulatorConfig.cfg" );

  /* Read the file. If there is an error, report it and exit. */
  if(!config_read_file(&cfg, fullFilePath))
  {
     fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
            config_error_line(&cfg), config_error_text(&cfg));
    config_destroy(&cfg);
    return FALSE;
  }
  /* Reading IP addresses of RH and ENodeB and Version of RH-SIM running */
  table = config_lookup(&cfg, "emsSimulatorConfig");
  config_setting_lookup_string(table, "version",  &version );
  strncpy(g_emsContext.version, version , MAX_SIZE_SWPACKAGESTATUS_VERSION );
  config_destroy(&cfg);
  return TRUE;
}
