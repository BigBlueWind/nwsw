#ifndef LTE_PTFM_MISH_H
#define LTE_PTFM_MISH_H

#define OK        0
#define ERROR    -1


typedef enum e_Ad9548ReturnCode
{
    AD9548RETURNCODE_ERR_SPI_DRV_OPEN = 40,
    AD9548RETURNCODE_STATUS_UNSTABLE,
    AD9548RETURNCODE_STATUS_STABLE,
    AD9548RETURNCODE_STATUS_NOT_LOCKED,
    AD9548RETURNCODE_STATUS_LOCKED,
    AD9548RETURNCODE_GPS_SIGNAL_SOURCE,
    AD9548RETURNCODE_CLK_SOURCE_2
}Ad9548ReturnCode;

typedef enum e_CdceReturnCode
{
    CDCERETURNCODE_ERR_SPI_DRV_OPEN = 55,
    CDCERETURNCODE_STATUS_NOT_LOCKED,
    CDCERETURNCODE_STATUS_LOCKED
}CdceReturnCode;

typedef enum e_EthernetLink
{
    ETHERNETLINK_BACK_HAUL, 
    ETHERNETLINK_DEBUG_PORT, 
    ETHERNETLINK_WIFI_PORT, 
    ETHERNETLINK_LK_PORT,
    ETHERNETLINK_MAX_PORT
}EthernetLink;


typedef enum e_EthReturnCode
{
    ETHRETURNCODE_ERR_MDIO_DRV_OPEN = 60,
    ETHRETURNCODE_ERR_INVALID_PARAMETER,
    ETHRETURNCODE_STATUS_SUCCESS,
    ETHRETURNCODE_STATUS_LINK_INACTIVE,
    ETHRETURNCODE_STATUS_LINK_ACTIVE,
}EthReturnCode;


/* extern declarations  */
extern Ad9548ReturnCode             isSystemClockStable           (void);
extern Ad9548ReturnCode             isFrequencyLocked             (void);
extern Ad9548ReturnCode             activeReferenceToAd9548       (void);
extern CdceReturnCode               isPllLocked                   (void);
extern EthReturnCode                isLinkActive                  (EthernetLink linkNum);
extern EthReturnCode                getReceiveErrorCounterVal     (EthernetLink linkNum, UINT32 * errorCount);

#endif /* LTE_PTFM_MISH_H */
