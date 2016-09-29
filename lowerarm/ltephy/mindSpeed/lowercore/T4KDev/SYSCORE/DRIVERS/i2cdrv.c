//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL i2cdrv.c
 *
 * @brief The I2C driver
 * @author Mindspeed Technologies
 * @version $Revision: 1.6 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "i2cdrv.h"
#include "systimers.h"

#define I2C_TIMEOUT         (10*150000)
#define EXEC(proc, retcode)	if ( (proc) == 0 ) {return (retcode);}

/** @brief This function waits for the specified FLAG, the time of
           waiting should be specified in ticks,
           150 ticks = 1 microseconds

    @param nFlagID [in] - flag id, see: I2C_xxx
    @param timeout [in] - the timeout

    @return [UINT32] status of flag */

UINT32 I2CWaitForFlag(UINT32 nFlagID, UINT32 timeout)
{
	UINT32 ts = SysTimerGetTick();

	//check if transmitted, wait until iflg is turned on
    while (!(REG32(I2C_CNTR_REG) & nFlagID) && (SysTimerGetTick() - ts) < timeout);

    return REG32(I2C_CNTR_REG) & nFlagID;
}

/** @brief This function waits for I-FLAG, the time of
           waiting is equal to 1ms

    @return [UINT32] status of I-FLAG*/

UINT32 I2CWaitForIFlag(void)
{
    UINT32 ts = SysTimerGetTick();

    while (!(REG32(I2C_CNTR_REG) & I2C_IFLG) && (SysTimerGetTick() - ts) < 150000);

    return REG32(I2C_CNTR_REG) & I2C_IFLG;
}

/** @brief This function waits for I-FLAG, the time of
           waiting should be specified in ticks,
           150 ticks = 1 microseconds

    @return [UINT32] status of I-FLAG*/

UINT32 I2CWaitForIFlagEx(UINT32 timeout)
{
	UINT32 ts = SysTimerGetTick();

	//check if transmitted, wait until iflg is turned on
    while (!(REG32(I2C_CNTR_REG) & I2C_IFLG) && (SysTimerGetTick() - ts) < timeout);

    return REG32(I2C_CNTR_REG) & I2C_IFLG;
}

/** @brief This function writes byte to be sent with using I2CStartXmit function
    @param Data [in] - byte of data
    @return [MXRC] an error code*/

MXRC I2CXmitByte(UINT8 Data)
{
    UINT32 rc = I2C_RC_OK;

    REG32(I2C_DATA_REG) = Data;
    REG32(I2C_CNTR_REG) &= ~I2C_IFLG;

    if (I2CWaitForIFlag() == 0)
        rc = I2C_RC_TIMEOUT;

    return rc;
}

/** @brief This function sends byte of data specified with
            I2CXmitByte function

    @return [MXRC] an error code */

MXRC I2CStartXmit(void)
{
    UINT32 rc = I2C_RC_OK;

    REG32(I2C_CNTR_REG) = I2C_STA;
    if (I2CWaitForIFlag() == 0)
        rc = I2C_RC_TIMEOUT;

    return rc;
}

/** @brief This function sends byte of data
    @return [MXRC] an error code */

MXRC I2CSendData(UINT8 Data)
{
    MXRC rc;

    if (FAILED(rc = I2CXmitByte(Data)))
        return rc;

    return I2CStartXmit ();
}

/** @brief This function reads byte of data

    @param pData[out] - the pointer to the storage
    @return [MXRC] an error code */

MXRC I2CReadData (UINT8 * pData)
{
    UINT32 nStat;
    _ASSERT_PTR (pData);

    if (!I2CWaitForFlag (I2C_IFLG, I2C_TIMEOUT))
        return I2C_RC_EMPTY;

    nStat = REG32(I2C_STAT_REG);

    if (nStat == I2C_DATA_RECEIVE_ACK || nStat == I2C_DATA_RECEIVE_NACK)
    {
        *pData = (UINT8)REG32(I2C_DATA_REG);
        return I2C_RC_OK;
    }

    return I2C_RC_EMPTY;
}

/** @brief This function is designed to generate reset seq.
           for the connected EEPROM device

    @return [MXRC] an error code */

MXRC I2CDrvResetDevice(void)
{
    UINT32 i;
    VUINT32 nTmp;

    REG32(I2C_CTRL) |= 1;

    // The algorithm of device resetting is:
    //   - Issue 9 START conditional
    //   - Send  9 data bits :  111111111
    //   - Send one START conditional
    //   - Send one STOP conditional

    //assume we have initialized the HW before
    // Issue 9 START conditional
    for (i = 0; i < 9; i++)
    {
        //put dev in master mode and tx a start condition
        REG32(I2C_CNTR_REG) = I2C_STA;
        I2CWaitForIFlagEx(I2C_TIMEOUT); //wait, but if no transition ignore
    }

    // Send  9 data bits :  111111111, 8+1 from the ack, i2c ip will clock 9 pulses for us. note bus is held hi
    REG32(I2C_DATA_REG) = 0xFF;
    // To send it, we must clear the iflg.
    REG32(I2C_CNTR_REG) = 0x0;
    I2CWaitForIFlagEx(I2C_TIMEOUT); //wait, but if no transition ignore

    // Send one START conditional
    REG32(I2C_CNTR_REG) = I2C_STA;
    I2CWaitForIFlagEx(I2C_TIMEOUT); //wait, but if no transition ignore

    //   - Send one STOP conditional
    REG32(I2C_CNTR_REG) = I2C_STP;
    REG32(I2C_CNTR_REG) = 0; //we are done!!!! clear the flag

    //undo the mux
    REG32(I2C_CTRL) &= ~1;

    return I2C_RC_OK;
}

/** @brief This function enables I2C device and setup
           dividers to the default value: 0x4d

    @return [MXRC] an error code */

MXRC I2CInitHw(void)
{
    // Power up I2C pad
    REG32(GENERIC_PAD_CTRL_REG) |= 1 << 14;
    REG32(GENERIC_PAD_CTRL_REG) &= ~(1 << 15);
    // Setup special configuration of I2C in GPIO register
    REG32(GPIO_PIN_SELECT_REG) &= ~(3 << 27);

    // Clear DATA register
    REG32(I2C_DATA_REG) = 0;

    // Clear slave address registers (probably not needed since we are always a master)
    REG32(I2C_ADDR_REG) = 0;
    REG32(I2C_XADDR_REG) = 0;

    // Clear control register
    REG32(I2C_CNTR_REG) = 0;

    // Set up clock divider for standard mode
    REG32(I2C_CCRFS_REG) = I2C_CLKDIV;

    I2CDrvResetDevice();

    // Clear DATA register
    REG32(I2C_DATA_REG) = 0;

    // Clear slave address registers (probably not needed since we are always a master)
    REG32(I2C_ADDR_REG) = 0;
    REG32(I2C_XADDR_REG) = 0;

    // Clear control register
    REG32(I2C_CNTR_REG) = 0;

    // Set up clock divider for standard mode
    REG32(I2C_CCRFS_REG) = I2C_CLKDIV;

    return I2C_RC_OK;
}

/** @brief This function enables I2C device and setup dividers
    @param pInit  [in] - init data
    @return [MXRC] an error code */

MXRC I2CInitHwEx(PI2CINIT pInit)
{
    _ASSERT_PTR(pInit);

    // Power up I2C pad
    REG32(GENERIC_PAD_CTRL_REG) |= 1 << 14;
    REG32(GENERIC_PAD_CTRL_REG) &= ~(1 << 15);
    REG32(GPIO_PIN_SELECT_REG) &= ~(3 << 27);

    // Clear DATA register
    REG32(I2C_DATA_REG) = 0;

    // Clear slave address registers (probably not needed since we are always a master)
    REG32(I2C_ADDR_REG) = pInit->addrreg;
    REG32(I2C_XADDR_REG) = pInit->xaddrreg;

    // Clear control register
    REG32(I2C_CNTR_REG) = pInit->ctrlreg;

    // Set up clock divider for standard mode
    REG32(I2C_CCRFS_REG) = pInit->clkdiv;

    if (pInit->resetseq != 0)
        I2CDrvResetDevice();

    // Clear DATA register
    REG32(I2C_DATA_REG) = 0;

    // Clear slave address registers (probably not needed since we are always a master)
    REG32(I2C_ADDR_REG) = pInit->addrreg;
    REG32(I2C_XADDR_REG) = pInit->xaddrreg;

    // Clear control register
    REG32(I2C_CNTR_REG) = pInit->ctrlreg;

    // Set up clock divider for standard mode
    REG32(I2C_CCRFS_REG) = pInit->clkdiv;

    return I2C_RC_OK;
}

UINT32 I2CDrvRead(UINT32 MfgID, PUINT8 pSrcData, UINT32 nDstAddr, UINT32 nLen)
{
    UINT8 result;
    UINT8 AddressWord;
    UINT16 i; //note len cannot exceed 64 kbytes
    //PUINT8 ptarget_address = (PUINT8)target_address;

    //put dev into master mode and transmit the start condition
    REG32(I2C_CNTR_REG) = I2C_STA;

    if (I2CWaitForIFlagEx(I2C_TIMEOUT)== 0)
    {
        REG32(I2C_CNTR_REG) = START_MASTER;
        EXEC(I2CWaitForIFlagEx(I2C_TIMEOUT), I2C_RC_TIMEOUT);
    }

    //---------------------------------------------
    //enter dummy write mode
    //this assumes a two word address ie min size eeprom is 4kBytes
    // for testing on smaller parts ( eg 2kByte) change to one word address
    //---------------------------------------------
    result = REG32(I2C_STAT_REG);

    if (result == I2C_START_TRANSMIT ||  result== I2C_REPEAT_START_TRANSMIT)
    {
        //now send the opcode and set to write
        REG32(I2C_DATA_REG) = (MfgID&0xFE)|I2C_WRITE;

        //to send it, must clear the iflg, note its ok here only to reset all flags to zero
        REG32(I2C_CNTR_REG) = 0;
    }

   EXEC(I2CWaitForIFlagEx(I2C_TIMEOUT), I2C_RC_TIMEOUT); //wait for tx to complete- no error handling here

    //check what's returned, if acked then continue with dummy write
    result = REG32(I2C_STAT_REG);

    if (result !=I2C_ADDRESS_W_ACK )
    {
        return 0;
    }

	if (MfgID == I2C_EEPROM_OPCODE)
	{
		// ****************** To test **********************************
		//write the MSB word of eeprom address
		AddressWord= (nDstAddr&0xFF00) >> 8;

		REG32(I2C_DATA_REG) = AddressWord;

		//clear the iflag, to send the second word
		REG32(I2C_CNTR_REG) = 0;

		EXEC(I2CWaitForIFlagEx(I2C_TIMEOUT), I2C_RC_TIMEOUT); //wait for tx to complete- no error handling here
		// ****************************************************
	}

    //write the LSB word of eeprom address
    AddressWord = (nDstAddr &0x000000FF);
    REG32(I2C_DATA_REG) = AddressWord;

    //clear the iflag, to send the second word
    REG32(I2C_CNTR_REG) = 0;
   EXEC(I2CWaitForIFlagEx(I2C_TIMEOUT), I2C_RC_TIMEOUT); //wait for tx to complete- no error handling here

    //---------------------------------------------
    //enter the sequential read
    //---------------------------------------------
    //send a new start condition
    REG32(I2C_CNTR_REG) = START_MASTER;

    if (I2CWaitForIFlagEx(I2C_TIMEOUT)==0)
    { //?? what do you do if timeout, do you retransmit, if so, how many times

        REG32(I2C_CNTR_REG) = START_MASTER;
       EXEC(I2CWaitForIFlagEx(I2C_TIMEOUT), I2C_RC_TIMEOUT); //wait for tx to complete- no error handling here
    }

    // now send the opcode and set to READ
    REG32(I2C_DATA_REG) = (MfgID & 0xFE) | I2C_READ;

    // to send it, must clear the iflg, note its ok here only to reset all flags to zero
    REG32(I2C_CNTR_REG) = 0;
    EXEC(I2CWaitForIFlagEx(I2C_TIMEOUT), I2C_RC_TIMEOUT);//wait for tx to complete - no error handling here

    // check if Read command was good, if not, no err handling, exit
    result = REG32(I2C_STAT_REG);

    if (result != I2C_ADDRESS_R_ACK)
    {
        return 0;
    }

    // depending on length we need to send an ack,
    // or if len=1, send no ack in prep for a stop
    if (nLen > 1)
    {
        //send an ack
        REG32(I2C_CNTR_REG) = I2C_AAK;
    }
    else if (nLen == 1)
    {
        //send NOack when the last byte is recvd
        //to send it, must clear the iflg, note its ok here only to reset all flags to zero
        REG32(I2C_CNTR_REG) = 0;
    }
    else
    {   //nLen==0
        //wait the flag, clear it and return

       EXEC(I2CWaitForIFlagEx(I2C_TIMEOUT), I2C_RC_TIMEOUT);
        REG32(I2C_CNTR_REG) = 0; //now clear it
    }

    for (i = 0; i < nLen; i++)
    {
       EXEC(I2CWaitForIFlagEx(I2C_TIMEOUT), I2C_RC_TIMEOUT);//wait for tx to complete- no error handling here

        *(VUINT8 *)(pSrcData + i) = (UINT8)REG32(I2C_DATA_REG);

        if (i == nLen-1)
        {
            // just rcvd my last byte
            //last byte received , transmit a  stop bit
            REG32(I2C_CNTR_REG) = I2C_STP;

            REG32(I2C_CNTR_REG) = 0; //we are done!!!! clear the flag
        }
        else
        {
            //not last byte
            if (i < nLen-2)
            { //more bytes to rcv, just ack the next time you rcv
                REG32(I2C_CNTR_REG) = I2C_AAK; //clear iflg and send ack bit
            }
            else
            {  //my next byte is my last byte (i = nLen -2)
                REG32(I2C_CNTR_REG) = 0;//send a no ack and clear iflg after you rcv the last byte
            }
        }

    }

    return i;
}

UINT32 I2CDrvWrite(UINT32 MfgID, PUINT8 pSrcData, UINT32 nDstAddr, UINT32 nLen)
{
	UINT32 status = 0, i = 0, nPage = 0, nTry = 0;

	i = 0;

	while (i < nLen)
	{
		// -----------------------
		// START WRITE OPERATION
		// -----------------------

		nTry = 0;

		while (nTry < I2C_MAX_SET_MASTER)
		{

          		REG32(I2C_CNTR_REG) = I2C_STA;

                 	if (I2CWaitForIFlagEx(I2C_TIMEOUT)== 0)
          	       	{
                 		REG32(I2C_CNTR_REG) = START_MASTER;
                 		EXEC(I2CWaitForIFlagEx(I2C_TIMEOUT), I2C_RC_TIMEOUT); //one more try, then move on
          	       	}

          		status = REG32(I2C_STAT_REG);

          		if (status == I2C_START_TRANSMIT || status == I2C_REPEAT_START_TRANSMIT)
          		{
                 		//now send the opcode and set to write
          	       		REG32(I2C_DATA_REG) = (MfgID & 0xFE) | I2C_WRITE;

                 		//to send it, must clear the iflg, note its ok here only to reset all flags to zero
          	       		REG32(I2C_CNTR_REG) &= ~I2C_IFLG;
                 	}
          		else
          		{
          			nTry ++;
          			continue;
          		}


                	EXEC(I2CWaitForIFlagEx(I2C_TIMEOUT), I2C_RC_TIMEOUT); //wait for tx to complete

          	       	//check what's returned, if acked then continue with dummy write
                 	status = REG32(I2C_STAT_REG);

          	       	if (status != I2C_ADDRESS_W_ACK)
          		{
          			nTry ++;
          		}
          		else
			{
          			break;
			}

			//delay (1);
		}

		if (nTry >= I2C_MAX_SET_MASTER)
		{
			return I2C_RC_SET_MASTER_ERROR;
		}

		// -----------------------
		// ADDRESS WRITE OPERATION
		// -----------------------

		if (MfgID == I2C_EEPROM_OPCODE)
		{
			// The MSB byte of addr

       		REG32(I2C_DATA_REG) = (nDstAddr & 0xFF00) >> 8;

	       	//clear the iflag, to send the second word
       		REG32(I2C_CNTR_REG) &= ~I2C_IFLG;

	       	EXEC(I2CWaitForIFlagEx(I2C_TIMEOUT), I2C_RC_TIMEOUT); //wait for tx to complete

       		//check what's returned, if acked then continue with dummy write
	       	status = REG32(I2C_STAT_REG);

		}

		// The LSB byte of address
		REG32(I2C_DATA_REG) = (nDstAddr & 0x00FF);

       		//clear the iflag, to send the second word
	       	REG32(I2C_CNTR_REG) &= ~I2C_IFLG;

       		EXEC(I2CWaitForIFlagEx(I2C_TIMEOUT), I2C_RC_TIMEOUT); //wait for tx to complete

	       	//check what's returned, if acked then continue with dummy write
       		status = REG32(I2C_STAT_REG);

		nPage = 0;

		while (nPage < I2C_PAGE_SIZE && i < nLen)
		{
			// -----------------------
			// DATA WRITE OPERATION
			// -----------------------
        		REG32(I2C_DATA_REG) = (UINT8)pSrcData [i];

	   		//clear the iflag, to send
      			REG32(I2C_CNTR_REG) &= ~I2C_IFLG;

      			EXEC(I2CWaitForIFlagEx(I2C_TIMEOUT), I2C_RC_TIMEOUT); //wait for tx to complete

			nPage ++;
			i ++;
		}

		REG32(I2C_CNTR_REG) = I2C_STP; // I2C_STP

		nDstAddr += nPage;
	}

	return i;
}

UINT32 I2CDrvWriteBytes(UINT8 DeviceAddress, UINT8 RegisterAddress, PUINT8 pData, UINT16 RequestedLength)
{

    UINT8 result;
    UINT16 i=0; //note len cannot exceed 64 kbytes
    //PUINT8 ptarget_address = (PUINT8)target_address;

    //put dev into master mode and transmit the start condition
    REG32(I2C_CNTR_REG) = I2C_STA;

    if (I2CWaitForIFlagEx(I2C_TIMEOUT)== 0)
    {
        REG32(I2C_CNTR_REG) = START_MASTER;
        EXEC(I2CWaitForIFlagEx(I2C_TIMEOUT), I2C_RC_TIMEOUT);
    }

    //---------------------------------------------
    //enter dummy write mode
    //this assumes a one byte address i.e. small device, not an EEPROM
    //---------------------------------------------
    result = REG32(I2C_STAT_REG);

    if (result == I2C_START_TRANSMIT ||  result== I2C_REPEAT_START_TRANSMIT)
    {
        //now send the opcode and set to write
        REG32(I2C_DATA_REG) = (DeviceAddress&0xFE)|I2C_WRITE;

        //to send it, must clear the iflg, note its ok here only to reset all flags to zero
        REG32(I2C_CNTR_REG) = 0;
    }

    EXEC(I2CWaitForIFlagEx(I2C_TIMEOUT), I2C_RC_TIMEOUT); //wait for tx to complete- no error handling here

    //check what's returned, if acked then continue with dummy write
    result = REG32(I2C_STAT_REG);

    if (result !=I2C_ADDRESS_W_ACK )
    {
        return 0;
    }

    //write the Reqister address to start write operation from
    REG32(I2C_DATA_REG) = RegisterAddress;

    //clear the iflag, to send the register address
    REG32(I2C_CNTR_REG) = 0;
    EXEC(I2CWaitForIFlagEx(I2C_TIMEOUT), I2C_RC_TIMEOUT); //wait for tx to complete- no error handling here

    for (i=0; i<RequestedLength; i++)
    {
        // -----------------------
        // DATA WRITE OPERATION
        // -----------------------
        REG32(I2C_DATA_REG) = (UINT8)pData[i];

        //clear the iflag, to send
        REG32(I2C_CNTR_REG) &= ~I2C_IFLG;

        //wait for tx to complete
        EXEC(I2CWaitForIFlagEx(I2C_TIMEOUT), I2C_RC_TIMEOUT);
    }

    REG32(I2C_CNTR_REG) = I2C_STP; // I2C_STP

    MxDelayTicks(3*150000);

    return ((UINT32)i);
}

UINT8 I2CGetStatus(void)
{
	return (UINT8)REG32(I2C_STAT_REG);
}

MXRC I2CStopXmit(void)
{
	MXRC rc = I2C_RC_OK;
	UINT32 ts;

	REG32(I2C_CNTR_REG) &= ~I2C_IFLG;
	REG32(I2C_CNTR_REG) = I2C_STP;

	ts = SysTimerGetTick();
	while ((SysTimerGetTick() - ts) < 1500000);

	REG32(I2C_CNTR_REG) &= ~I2C_IFLG;
	REG32(I2C_CNTR_REG) = I2C_STP;

	ts = SysTimerGetTick();
	while ((SysTimerGetTick() - ts) < 1500000);

	if (I2CGetStatus() != 0xF8)
		return I2C_RC_STATUS_ERROR;

	if (REG32(I2C_CNTR_REG) & I2C_IFLG)
		return I2C_RC_ERROR;

	if (REG32(I2C_CNTR_REG) & I2C_STP)
		return I2C_RC_ERROR;

	if (REG32(I2C_CNTR_REG) & I2C_STA)
			return I2C_RC_ERROR;

	return rc;
}

void I2CSetBit(UINT32 nBit)
{
	REG32(I2C_CNTR_REG) |= nBit;
}

void I2CClearBit(UINT32 nBit)
{
	REG32(I2C_CNTR_REG) &= ~nBit;
}


