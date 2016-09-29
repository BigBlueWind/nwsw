/**
 * @file    deh_ethernet.c
 * @brief This file is contains code get detail from Ethernet
 * @details Copyright Powerwave Technologies, Inc., 2011
 *
 * @author kgummad
 * @date   29-07-2011
 * @details 1)Ethernet supported added in DEH.
 *
 */

#include "deh.h"
#include "lte_bb_platform.h"

/**
 * @param   linkNum   contains ethernet link
 * @details This function will check whether ethernet link is active or not.
 */
TBOOL dehIsEthernetLinkActive(
  EthernetLink linkNum
  )
{
  EthReturnCode response;
  if( linkNum EQ ETHERNETLINK_BACK_HAUL )
  {
    response = isLinkActive( linkNum );
  }
  if( linkNum EQ ETHERNETLINK_DEBUG_PORT )
  {
	response = Switch_isLinkActive( linkNum );
  }
  if( ETHRETURNCODE_STATUS_LINK_ACTIVE EQ response)
  {
	logInfo("%s Ethernet device is Active and Link is Up", ethernetLinkName( linkNum ));
	return TRUE;
  }
  return FALSE;
}

/**
 * @param   linkNum   contains ethernet link
 * @details This function will return the error string of ethernet link.
 */
UINT8* dehEthernetLinkErrorString(
  EthernetLink linkNum
  )
{
  EthReturnCode response;
  if( linkNum EQ ETHERNETLINK_BACK_HAUL )
  {
    response = isLinkActive( linkNum );
  }
  if( linkNum EQ ETHERNETLINK_DEBUG_PORT )
  {
	response = Switch_isLinkActive( linkNum );
  }

  if(ETHRETURNCODE_ERR_INVALID_PARAMETER EQ response )
  {
    logError("%s Invalid Ethernet Link", ethernetLinkName( linkNum ));
    return "Link Invalid";
  }
  else if(ETHRETURNCODE_STATUS_LINK_INACTIVE EQ response )
  {
    logInfo("%s Ethernet device is Active but Link is Down", ethernetLinkName( linkNum ));
    return "Link Inactive";
  }
  else if(ETHRETURNCODE_STATUS_SUCCESS EQ response )
  {
    logInfo("%s Ethernet device is Active but Link is Down", ethernetLinkName( linkNum ));
    return "Link Inactive";
  }
  logError("Unable to read %s Ethernet status", ethernetLinkName( linkNum ));
  return "Link Down";
}

/**
 * @param   linkNum   contains ethernet link
 * @details This function will return the name of corresponding ethernet link.
 */
UINT8* ethernetLinkName(
  EthernetLink linkNum
  )
{
  switch(linkNum)
  {
    case ETHERNETLINK_BACK_HAUL:
    	return "BACK HAUL";
    case ETHERNETLINK_DEBUG_PORT:
    	return "DEBUG PORT";
    case ETHERNETLINK_WIFI_PORT:
    	return "WIFI PORT";
    case ETHERNETLINK_LK_PORT:
    	return "LK PORT";
    case ETHERNETLINK_MAX_PORT:
    	return "MAX PORT";
  }
  return NULL;
}

/**
 * @param   ethernetAddr   Ethernet address
 * @param   portNum        Port number
 * @details This function will GET the MAC address of the requested port
 */
TBOOL dehGetMacAddress(
  IO UINT8 *ethernetAddr,
  IO UINT8  portNum
  )
{
  struct ether_addr macAddress;

  clearBytes( &macAddress, sizeof(struct ether_addr) );

  if(SETGETMACADDRESSRETURNCODE_SUCCESS NOTEQ getMacAddress( &macAddress, portNum))
  {
    BS_ERROR_INTERNAL_ERROR_OCCURED("Failed to GET MacAddress using Platform API");
    return FALSE;
  }
  snprintf(ethernetAddr, MAX_SIZE_ENBADDRESSES_BACKHAULPORTMACADDRESS + 1 , "%02x:%02x:%02x:%02x:%02x:%02x", macAddress.ether_addr_octet[0],
		  macAddress.ether_addr_octet[1],macAddress.ether_addr_octet[2],macAddress.ether_addr_octet[3],macAddress.ether_addr_octet[4],
		  macAddress.ether_addr_octet[5]);
  return TRUE;
}

/**
 * @param   ethernetAddr   Ethernet address
 * @param   portNum        Port number
 * @details This function will SET the MAC address of the requested port
 */
TBOOL dehSetMacAddress(
  IO UINT8  *ethernetAddr,
  IO UINT8   portNum
  )
{
  if(SETGETMACADDRESSRETURNCODE_SUCCESS NOTEQ setMacAddress( ethernetAddr, portNum))
  {
    BS_ERROR_INTERNAL_ERROR_OCCURED("Failed to SET MacAddress using Platform API");
    return FALSE;
  }
  return TRUE;
}

/**
 * @param   DehContext             dehContext
 * @param   MsgBsmDehSetIpAddrReq  setIpAddrReq
 * @param   MsgDehBsmSetIpAddrRsp  setIpAddrRsp
 * @details This function will SET and update the eNodeB BackHaul and Debug IP Addresses
 */

void  setAndUpdateEnodebIpAddr(
  IO DehContext             *dehContext,
  IO MsgBsmDehSetIpAddrReq  *setIpAddrReq,
  IO MsgDehBsmSetIpAddrRsp  *setIpAddrRsp
   )
{
  TBOOL        isSuccess;

  /* Setting of BACKHAUL IP Address Configuration */
  if (IPADDR_STATIC EQ setIpAddrReq->ipAddrSetReq[INTERFACE_BACKHAUL].typeOfIpAddr)
  {
    isSuccess = setIpAddressOfEth(BACKHAUL_PORT, setIpAddrReq->ipAddrSetReq[INTERFACE_BACKHAUL].ipAddress,
    		                    setIpAddrReq->ipAddrSetReq[INTERFACE_BACKHAUL].netMask);
    if (TRUE EQ isSuccess)
    {
      logInfo("%s,","Setting of Static Ip Address succeeded");
    }
    else
	{
      setIpAddrRsp->ipAddrSetRsp[INTERFACE_BACKHAUL].errDesc.errorNum = BS_ERR_SET_IP_ADDR_FAILED;
      strncpy(setIpAddrRsp->ipAddrSetRsp[INTERFACE_BACKHAUL].errDesc.failReason,"Setting of Static IP Address failed",sizeof(String256));
      logError("%s","Setting of Static Ip Address failed");
	}
    strncpy(setIpAddrRsp->ipAddrSetRsp[INTERFACE_BACKHAUL].ipAddress,setIpAddrReq->ipAddrSetReq[INTERFACE_BACKHAUL].ipAddress,sizeof(IpAddrStr));
    strncpy(setIpAddrRsp->ipAddrSetRsp[INTERFACE_BACKHAUL].netMask,setIpAddrReq->ipAddrSetReq[INTERFACE_BACKHAUL].netMask,sizeof(IpAddrStr));
    setIpAddrRsp->ipAddrSetRsp[INTERFACE_BACKHAUL].isSetSucc = isSuccess;
  }
  else
  {
	if(( FALSE EQ dehContext->isBackHaulIpRecieved) || ((strncmp(dehContext->backhaulIpAddr, "169", 3 )) EQ 0))
    {
      strncpy(setIpAddrRsp->ipAddrSetRsp[INTERFACE_BACKHAUL].ipAddress ,setIpAddrReq->ipAddrSetReq[INTERFACE_BACKHAUL].ipAddress,sizeof(IpAddrStr));
      strncpy(setIpAddrRsp->ipAddrSetRsp[INTERFACE_BACKHAUL].netMask ,setIpAddrReq->ipAddrSetReq[INTERFACE_BACKHAUL].netMask,sizeof(IpAddrStr));
      setIpAddrRsp->ipAddrSetRsp[INTERFACE_BACKHAUL].isSetSucc = FALSE;
      setIpAddrRsp->ipAddrSetRsp[INTERFACE_BACKHAUL].errDesc.errorNum = BS_ERR_SET_IP_ADDR_FAILED;
      strncpy(setIpAddrRsp->ipAddrSetRsp[INTERFACE_BACKHAUL].errDesc.failReason,"Setting of DHCP IP Address failed",sizeof(String256));
      logError("%s","Setting of DHCP Ip Address failed");
    }
    else
    {
      strncpy(setIpAddrRsp->ipAddrSetRsp[INTERFACE_BACKHAUL].ipAddress,dehContext->backhaulIpAddr,sizeof(IpAddrStr));
      strncpy(setIpAddrRsp->ipAddrSetRsp[INTERFACE_BACKHAUL].netMask,dehContext->backhaulNetMaskAddr,sizeof(IpAddrStr));
      setIpAddrRsp->ipAddrSetRsp[INTERFACE_BACKHAUL].isSetSucc = TRUE;
      logInfo("%s,","Setting of DHCP Ip Address succeeded");
    }
 }

  /* Debug Static IP Address configuration */
  isSuccess = setIpAddressOfEth(DEBUG_PORT,setIpAddrReq->ipAddrSetReq[INTERFACE_DEBUG].ipAddress,
		                      setIpAddrReq->ipAddrSetReq[INTERFACE_DEBUG].netMask);
  if (TRUE EQ isSuccess)
  {
    logInfo("%s,","Setting of Static Ip Address succeeded");
    strncpy(dehContext->debugIpAddr      , setIpAddrReq->ipAddrSetReq[INTERFACE_DEBUG].ipAddress,sizeof(IpAddrStr));
    strncpy(dehContext->debugNetMaskAddr , setIpAddrReq->ipAddrSetReq[INTERFACE_DEBUG].netMask,sizeof(IpAddrStr));
  }
  else
  {
    setIpAddrRsp->ipAddrSetRsp[INTERFACE_DEBUG].errDesc.errorNum = BS_ERR_SET_IP_ADDR_FAILED;
    strncpy(setIpAddrRsp->ipAddrSetRsp[INTERFACE_DEBUG].errDesc.failReason,"Setting of Static Ip Address failed",sizeof(String256));
    logError("%s","Setting of Static Ip Address failed");
  }
  strncpy(setIpAddrRsp->ipAddrSetRsp[INTERFACE_DEBUG].ipAddress,setIpAddrReq->ipAddrSetReq[INTERFACE_DEBUG].ipAddress,sizeof(IpAddrStr));
  strncpy(setIpAddrRsp->ipAddrSetRsp[INTERFACE_DEBUG].netMask,setIpAddrReq->ipAddrSetReq[INTERFACE_DEBUG].netMask,sizeof(IpAddrStr));
  setIpAddrRsp->ipAddrSetRsp[INTERFACE_DEBUG].isSetSucc = isSuccess;

  setIpAddrRsp->ipAddrSetRsp[INTERFACE_BACKHAUL].interfaceType = setIpAddrReq->ipAddrSetReq[INTERFACE_BACKHAUL].interfaceType;
  setIpAddrRsp->ipAddrSetRsp[INTERFACE_BACKHAUL].typeOfIpAddr  = setIpAddrReq->ipAddrSetReq[INTERFACE_BACKHAUL].typeOfIpAddr;
  setIpAddrRsp->ipAddrSetRsp[INTERFACE_DEBUG].interfaceType    = setIpAddrReq->ipAddrSetReq[INTERFACE_DEBUG].interfaceType;
  setIpAddrRsp->ipAddrSetRsp[INTERFACE_DEBUG].typeOfIpAddr     = setIpAddrReq->ipAddrSetReq[INTERFACE_DEBUG].typeOfIpAddr;

}

