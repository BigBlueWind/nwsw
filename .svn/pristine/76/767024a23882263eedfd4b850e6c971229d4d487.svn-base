//SCTPexports.h
#ifndef __EXPORTS__
#define __EXPORTS__

#include "turnerapidef.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <SCTP.h>

#define FORG_PORT 32
#define SCTP_INFO_MAX_ASSOC 16

#define FORG_SCTP_WAIT_TIMEOUT	WAIT_TIMEOUT

#define FORG_SCTP_ASSOC_NOT_FOUND	((UINT16)SCTP_ASSOC_NOT_FOUND)
#define FORG_SCTP_PARAMETER_PROBLEM ((UINT16)SCTP_PARAMETER_PROBLEM)
#define FORG_SCTP_INIT_FAILED	-0xFF
#define FORG_SCTP_IO_PENDING	-0x100
#define FORG_SCTP_RECV_FAILED	-0x101
#define FORG_SCTP_BUFF_SMALL	-0x102
#define FORG_SCTP_CONN_FAILED	-0x103
	
#define FORG_ACK_NOT_GOOD	-0xF00

#define ASSOC_OPT_NETWORK_EVENT		0x0001
#define ASSOC_OPT_NATIVE_STATE		0x0002
#define ASSOC_OPT_USER_DEF			0x0003

typedef void (*LPAcceptCB)(UINT associationID);

typedef struct _sctp_info{
	BOOL Inited;
	UINT AssocCount;
	UINT Assocs[SCTP_INFO_MAX_ASSOC];
}sctp_info, *psctp_info;

typedef struct _stuser_def_conn_options {
	ULONG ApiAckTimeout; 
}stuser_def_conn_options, *pstuser_def_conn_options;

/** SCTPInfo Get info about SCTP library: is library already initialized, association count, 
*				 first SCTP_INFO_MAX_ASSOC associations.   
*	@param[out]  info			- out info placed
*	@note 		
*/
void SCTPInfo(psctp_info info);

/** SCTPInit Initialize SCTP library, register SCTP instance to all local interfaces "0.0.0.0"  
*	@param[in]  AcceptCB		- Callback function to make known associationID connection up occured  
*	@return		SCTP result of sctp_initLibrary or sctp_registerInstance
*	@note 		
*/
#ifdef __cplusplus
	INT SCTPInit( LPAcceptCB AcceptCB = NULL );
#else
	INT SCTPInit( LPAcceptCB AcceptCB );
#endif

/** SCTPTerm Terminate SCTP library environment, free resources  
*	@return		SCTP result of sctp_freeLibrary 
*	@note 		
*/
INT SCTPTerm( );

/** SCTPConnect Connect to remote host.  
*	@param[in]  dest			- connection destination
*	@return		SCTP result of sctp_associate if connected successful 
*	@note 		
*/
INT SCTPConnect( UCHAR dest[SCTP_MAX_IP_LEN] );

/** SCTPDisconnect Disconnect associated connection.  
*	@param[in]  AssocId			- connection associated before
*	@return		SCTP result of sctp_shutdown 
*	@note 		
*/
INT SCTPDisconnect( UINT AssocId );

/** SCTPGetAssocOpt Get option of certain association.
*	@param[in]		AssocId - researched association id
 *	@param[in]		OptName - option name (see supported options name)
*	@param[out]		OptVal - buffer to keep returned option 
*	@param[in, out]	OptLen - length of buffer, out actually received bytes
*	@return			SCTP_SUCCESS if success
*					FORG_SCTP_ASSOC_NOT_FOUND - mentioned association does not exist (probably destroyed)
*					FORG_SCTP_PARAMETER_PROBLEM - pointed parameters does not compatible 
*	@note Supported options: 
*							ASSOC_OPT_NETWORK_EVENT - network event handler for given association
*							ASSOC_OPT_NATIVE_STATE - state of association, return SCTP_AssociationStatus structure
*/
INT SCTPGetAssocOpt( UINT AssocId, UINT OptName, PUCHAR OptVal, PUINT OptLen );

/** SCTPSetAssocOpt Set option to association
*	@param[in]		AssocId - id of interested association
*	@param[in]		OptName - option name (see supported option name)
*	@param[in]		OptVal - buffer to keep assignable option 
*	@param[in]		OptLen - length of buffer
*	@return			SCTP_SUCCESS if success
*					FORG_SCTP_ASSOC_NOT_FOUND - mentioned association does not exist (probably destroyed)
*					FORG_SCTP_PARAMETER_PROBLEM - pointed parameters does not compatible
*	@note Supported options: 
*							ASSOC_OPT_NETWORK_EVENT - network event handler for given association
*							ASSOC_OPT_NATIVE_STATE - state of association, SCTP_AssociationStatus structure
*/
INT SCTPSetAssocOpt( UINT AssocId, UINT OptName, PUCHAR OptVal, UINT OptLen );


/** SCTPSend Send buffer of length.  
*	@param[in]  associationID			- connection associated before
*	@param[in]	buffer					- buffer have to be send
*	@param[in]	length					- length of buffer
*	@return		SCTP result of sctp_send
*	@note 		
*/
INT SCTPSend( UINT associationID, UCHAR *buffer, UINT length );

/** SCTPReceive receive data has been answered to us
*	@param[in]		associationID			- connection associated before
*	@param[in]		buffer					- buffer was been allocated to receive data
*	@param[in, out]	length					- length of buffer, out actually received bytes
*	@param[in]		async_event				- event for async receiving, 
*												return immediately (async_event signals then data received)
*	@return			SCTP result of sctp_receive
*	@note 		
*/
#ifdef __cplusplus
	INT SCTPReceive( UINT associationID, UCHAR *buffer, UINT* length, HANDLE async_event = NULL );
#else
	INT SCTPReceive( UINT associationID, UCHAR *buffer, UINT* length, HANDLE async_event );
#endif

/** SCTPQueue put back message to the internal queue back
*	@param[in]		associationID			- connection associated before
*	@param[in]		buffer					- data to put 
*	@param[in]		length					- length of buffer
*	@return			SCTP_SUCCESS if success
*	@note 			message in buffer can be received later by Receive when previous all messages in queue will be gotten
*/
INT SCTPQueue( UINT associationID, UCHAR* buffer, UINT length );

/** Wait wait while some data arrived in answer
*	@param[in] associationID			- association which data waiting
*	@param[in] time						- time (in milliseconds) to wait
*	@return	   SCTP_SUCCESS, FORG_SCTP_WAIT_TIMEOUT (if no data arrived by period)
*	@note 		
*/
INT SCTPWait( UINT associationID, ULONG time );

/** SCTPSelect get length of data can be received on association
*	@param[in]	associationID			- association which data looking
*	@param[out] length					- length of data looking
*	@return	   SCTP_SUCCESS if success
*	@note 		
*/
INT SCTPSelect( UINT associationID, UINT* length );

/** SCTPAccept permits an incoming connection
*	@return	   positive value equal new association created, otherwise error
*	@note 		
*/
INT SCTPAccept( );

/** SCTPGetParamApiCmd return command param identified by nNum
*	@param[in]	cmd			- command pointer which param requested
*	@param[in]	nNum		- requested param index 0 beginning 
*								API_LAST_PARAM cause this function return memory after last param  
*	@return	   pointer to param structure ApiParam
*	@note 		ParamData[0] in return value contain first 4 bytes of data (not pointer to data) 
*				be sure param with nNum index exist else assert occured
*/
PC4kApiParam SCTPGetParamApiCmd(IN PC4kApiHeader cmd, IN UINT16 nNum);

/** SetParamApiCmd place param in command 
*	@param[in, out]	cmd		- command pointer where to put
*	@param[in]		param	- param which placing. param->ParamID index in cmd where to put this
*								if param->ParamID == API_LAST_PARAM function append parameter  
*	@return	   pointer to param structure ApiParam, have been placed 
*	@note 		ParamData[0] in param parameter must contain pointer to data should be copied to command
*				be sure param->ParamID has right value
*/
PC4kApiParam SCTPSetParamApiCmd (IN OUT PC4kApiHeader cmd, IN PC4kApiParam param);

/** SCTPRecvApiCmd receive API command or part of command (in ext. API case)
*	@param[in]	assocID		- connection associated before
*	@param[in]	cmd			- command pointer where ACK placed
*								cmd->Length interprets as in parameter, maximum data can be placed in cmd
*	@return	   SCTP_SUCCESS - OK, 
*				FORG_SCTP_ASSOC_NOT_FOUND - assocID invalid value 
*				FORG_SCTP_WAIT_TIMEOUT - period for wait command elapsed
*				FORG_ACK_NOT_GOOD - ACK contain error code in 0 parameter
*				FORG_SCTP_RECV_FAILED - unexpected behavior 
*	@note 		Receive error code checking only in case of usual command otherwise 
*				Important, that in case of extended API this function just return the command (or part of the command) without checking
*/
INT SCTPRecvApiCmd(UINT assocID, C4kApiHeader* cmd);

/** SCTPSendApiCmd send API command 
*	@param[in]	assocID		- connection associated before
*	@param[in]	cmd			- command pointer which will be sent
*								usual API command must be fully constructed before this call
*								extended API command payload must be placed in memory immediately after header (pointed by cmd)
*								cmd->Length interprets as length of all useful data started from cmd, payload length 
*	@return		SCTP_SUCCESS if ok.
*	@note		In case of extended API command this functions send portion of payload and 
*					RecvApiCmd will be sequentially called on each command chunk except of the last.
*/
INT SCTPSendApiCmd(UINT assocID, C4kApiHeader* cmd);

#ifdef __cplusplus
}
#endif

#endif //__EXPORTS__