/** @file unified_diag.h
 *
 * @brief Unified Diagnostics module
 * @author Mindspeed Technologies
 * @version $Revision: 1.6 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef UNIFIED_DIAG_H_
#define UNIFIED_DIAG_H_

#include "4gmx.h"
#include "appids.h"
#include "supervisor.h"
#include "apimgr.h"
#include "network/network.h"


//codes of error
#define RC_UNIDIAG_UNSUP_API    DEF_USRRC(APP_MODULE, UNIDIAG, 1)
#define RC_UNIDIAG_UNSUP_TYPE    DEF_USRRC(APP_MODULE, UNIDIAG, 2)
#define RC_UNIDIAG_ERROR        DEF_USRRC(APP_MODULE, UNIDIAG, 3)
#define RC_UNIDIAG_NO_PARAM        DEF_USRRC(APP_MODULE, UNIDIAG, 4)
#define RC_UNIDIAG_PARAM_ERROR        DEF_USRRC(APP_MODULE, UNIDIAG, 5)
#define RC_UNIDIAG_PARAM_LEN_ERROR        DEF_USRRC(APP_MODULE, UNIDIAG, 6)
#define RC_UNIDIAG_NO_NETWORK_DEST        DEF_USRRC(APP_MODULE, UNIDIAG, 7)
#define RC_UNIDIAG_NO_NETWORK_SOCKET        DEF_USRRC(APP_MODULE, UNIDIAG, 8)


//API UD
#define PAR_API_MESSAGE_REQUEST   0xE100
#define PAR_API_MESSAGE_TX     0xE102
#define PAR_API_MESSAGE_RX     0xE103

#define API_TX_MASK (1<<0)
#define API_RX_MASK (1<<1)
#define API_MESSAGE_MASK (1<<0)

//redirection path
#define PAR_UNIDIAG_INTERFACE           0xEF00
#define PAR_UNIDIAG_DEST_IPADDR   0xEF01
#define PAR_UNIDIAG_DEST_PORT           0xEF02
#define PAR_UNIDIAG_DEST_MACADDR     0xEF03

#define UNIDIAG_INTF_DEFAULT_CMD_INTF     0
#define UNIDIAG_INTF_PCIE   1
#define UNIDIAG_INTF_ETH    2
#define UNIDIAG_INTF_SRIO   3

typedef struct tagEXTRAUDHEADER {
        UINT16 seqnum;
        UINT16 fragment_num;
        UINT32 timestamp;
    } EXTRAUDHEADER; 

typedef struct tUniDiagContxt {
    UINT32 StreamEnableMask;
    UINT32 API_Message_Option;
    UINT32 UD_interface;

    net_socket soc;
    void  (*transmit)(UINT16 nID, LPVOID pData, UINT16 nDataSize);
    void  (*transmitex)(UINT16 nID, LPVOID pData, UINT16 nDataSize);
    UINT16 seqnum;

}UNIDIAGCONTXT,*PUNIDIAGCONTXT;


MXRC UniDiagInit (void);
void UniDiagStreamingAPI(UINT16 nID, ApiHeader* pApi);
MXRC UniDiagSetAPIMessageOption(UINT32 option);
MXRC UniDiagSetStreamEnableMask(UINT32 mask);
UINT32 UniDiagGetStreamEnableMask(void);
MXRC UniDiagSetStreamOverUDP(UINT32 ip, UINT16 port, UINT8* mac);


#endif /*UNIFIED_DIAG_H_*/
