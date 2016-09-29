//-------------------------------------------------------------------------------------------
/** @file UMTSNwlCrc.c
 *
 * @brief extract and calculate crc result based on decoder output
 * @author Mindspeed Technologies
 * @version $Revision: 1.2.4.2 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include <stdio.h> 
#include <string.h>

#include "UMTSNwlCrc.h"




//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  extract the crc value from decoder output.

 Function ProGetCRCOfTBBlock

 @param    pDataIn =  the address of input data.
 @param    sTBSize =  the bit numbers without crc.
 @param    byCRCLen =  the bit num of crc.
 @return    crc value.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
U32 ProGetCRCOfTBBlock(U8 *pDataIn, U16 sTBSize, U8 byCRCLen)
{
	U32 dwCrc = 0;
	U32 idx;
	U8 *pTemp = NULL;
	pTemp = pDataIn + sTBSize + byCRCLen -1;
        
	for(idx=0;idx<byCRCLen;idx++)
	{
		if(1 == *pTemp)
        {
        	dwCrc <<= 1;
        	dwCrc += 1;
        }
		else
        {
        	dwCrc <<= 1;
        }

    	pTemp --;
	}
	        
	return dwCrc;
	
} 


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  put bit (stored in byte) data conject to byte

 Function ConjBitToByte

 @param    pbitIn =  the address of bit data.
 @param    pByteOut =  the address of combined byte data.
 @param    wBitLen =  the bit num of data.
 @return    the byte number of combined output data.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
U32 ConjBitToByte(U8 *pbitIn, U8 *pByteOut, U32 wBitLen)
{
	U32 idx;
       U32 dByteNum;
	U32 dResBitNum;	
       
  
	U8 *p1 = NULL;
	U8 *p2 = NULL;

	p2 = pByteOut;

        
	if((NULL == pbitIn) || (NULL == pByteOut))
	{	
	    printf( "ConjBitToByte Addr err" );
	}

	dResBitNum = wBitLen%8;
	dByteNum = (0 == dResBitNum)?(wBitLen/8):(wBitLen/8+1);

	for(idx = 0;idx<wBitLen;idx++)
	{
		p1 = pbitIn+idx;
            p2 = (pByteOut+(idx>>3));
		
		*p2 <<= 1;

    	       if(1 == *p1)
		{
		    *p2 += 1;
		}
	}
	
    return dByteNum;         
}



/* ==============  =======================================
function :calc crcr 16
input: ptr: the addr of data stream which will be deliver
	len: the byte num of data stream which will be deliver	
output : the crc 
=======================================================*/
U32 cal_crc16(U8 *ptr, U8 len)
{
	U8 i;
	U32 crc=0;
	while(len--!=0)
        {
		for(i=0x80; i!=0; i/=2)
		{
			if((crc&0x8000)!=0) {crc*=2; crc^=0x1021;} /* 余式CRC 乘以2 再求CRC */
			else crc*=2;
			if((*ptr&i)!=0) crc^=0x1021; /* 再加上本位的CRC */
		}
		ptr++;
	}
	return(crc);
}




//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  calculate the crc , the unit is byte

 Function Bit_Calc_crc16

 @param    ptr =  the addr of data stream which will be deliver.
 @param    dLen =  the bit num of data stream which will be deliver.
 @return    the crc value.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
U32 Bit_Calc_crc16(U8 *ptr,  U32 dLen)
{
	U32 crc=0;
    
	while(dLen !=0 )
        {
		if((crc&0x8000)!=0) {crc*=2; crc^=0x1021;} /* 余式CRC 乘以2 再求CRC */
		else crc*=2;
		if((*ptr)!=0) crc^=0x1021; /* 再加上本位的CRC */
    	ptr++;
        dLen --;
	}
    
	return(crc);
    	
}
