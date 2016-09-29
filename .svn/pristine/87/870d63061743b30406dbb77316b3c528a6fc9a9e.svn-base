//-------------------------------------------------------------------------------------------
/** @file lte_bs_phy_main.c
 *
 * @brief This file defines the SAP point to the LTE BS PHY from L2
 * @author Mindspeed Technologies
 * @version $Revision: 1.17 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#include "typedef.h"
#include "ltephydef.h"
#include "prototypes.h"
#include "ext_phy.h"

//-------------------------------------------------------------------------------------------
// LteBsPhyMain()
//
/** @brief This function provides the interface between L2 and PHY layers for
 *         an LTE Base Station
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param phyInstance Defines the particular PHY that is going to perform the
 *                     processing of the messages and input samples
 *  @param sigOut      It is a pointer to the memory that contains the Tx IQ 
 *                     Samples that constitute the output from processing an
 *                     LTE sub-frame
 *  @param sigIn       It is a pointer to the memory that contains the Rx IQ
 *                     Samples that correspond to the input for a Rx LTE 
 *                     subframe process
 *  @param inputMessage
 *                     Pointer to the input LTE L2-PHY API Message 
 *  @param inputMessageSize
 *                     Input LTE L2-PHY message size in long words
 *  @param outputMessage
 *                     Pointer to the output LTE L2-PHY API Message
 *  @param outputMessageSize
 *                     Pointer to Output LTE L2-PHY message size in long words
 *
 *  @return <B>status</B><BR>     A value of zero indicate success a value of 1 indicates
 *                     failure (We can expand to indicate the specific failure
 *                     reason)
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -----------------------------------------------------------------------------------------------------		
 *  |Name              |Name in  |Direction |Length |Type        |Data Format|Valid Range |Units        |	
 *  |                  |standard |          |       |            |           |            |             |
 *  -----------------------------------------------------------------------------------------------------	
 *  |phyInstance       |         |Input     |1      |unsigned int|           |[0:2]       |             |		
 *  -----------------------------------------------------------------------------------------------------		
 *  |sigOut            |         |pOutput   |1      |complex     |(1:1:14)   |(0:2^32)    |		|		
 *  -----------------------------------------------------------------------------------------------------		
 *  |sigIn             |         |pInput    |1      |complex     |(1:1:14)   |(0,2^32)    |		|		
 *  -----------------------------------------------------------------------------------------------------		
 *  |inputMessage      |         |pInput    |1      |unsigned int|(1:31:0)   |(0,2^32)    |		|		
 *  -----------------------------------------------------------------------------------------------------
 *  |inputMessageSize  |         |Input     |1      |unsigned int|(1:31:0)   |(0,TBD]     |Bytes	|
 *  -----------------------------------------------------------------------------------------------------
 *  |outputMessage     |         |pOutput   |1      |unsigned int|(1:31:0)   |(0,2^32)    |		|
 *  -----------------------------------------------------------------------------------------------------
 *  |outputMessageSize |         |Output    |1      |unsigned int|(1:31:0)   |(0,TBD]     |Bytes	|
 *  -----------------------------------------------------------------------------------------------------
 *  |status            |         |Output    |1      |unsigned int|(1:31:0)   |[0,2^32)    |		|
 *  -----------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |LteBsPhyMain                |
 *  ---------------------------------------------------
 *  |Input Streams       |sigIn,inputMessage          |
 *  ---------------------------------------------------
 *  |Output Streams      |sigOut,outputMessage        |
 *  ---------------------------------------------------
 *  |Config Parameters   |phyInstance                 |
 *  ---------------------------------------------------
 *  |Status Parameters   |status                      |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 *  This function takes an LTE L2-PHY API command and a set of IQ samples and
 *  then produces LTE L2-PHY acks and indications according to the API 
 *  definition contained in MSA-E81613. It also takes a set of input IQ samples
 *  and performs the receive subframe processing on them according to 3GPP spec
 *  36.211, 36.212 and 36.213 and also performs the transmit subframe processing
 *  from the data contained in the TXSDU and generates a set of output IQ samples
 *  that the sigOut pointer indicates. The output samples can be concatenated in
 *  the case of MIMO configurations on a per antenma basis
 *  The LTE L2-LTE APY messages are carried using the Mindspeed generic message
 *  format described in MSA-81479 using MessageType 0x0002 (Message for PHY)
 *  and MessageID 0x7000 Foreign API Messe
 *
 * <5> Cycle Count Formula:
 * TBA
 *
 * <6> References:
 * MSA-81749 and MSA-81613
 * 
 *  \ingroup group_lte_api
 *
 **/
//-------------------------------------------------------------------------------------------
U32 LteBsPhyMain(U32 phyInstance, S16* sigOut, S16* sigIn, U32* inputMessage, U32 inputMessageSize,
                 U32* outputMessage, U32* outputMessageSize)
{

    U32 status = 0;
    // Verify that phyInstance is within bounds else return
    if (phyInstance > MAXNUMPHYINSTANCES)
    {
        return (FAILURE);
    }
    // Now check the incoming message
    if (inputMessage)
    {
        if (LteParseMsg(phyInstance, inputMessage, inputMessageSize, outputMessage,
                        outputMessageSize))
        {
            return (FAILURE);
        }
    }
    if (sigOut)
    {
        // Perform Lte Tx subframe process
        LteBsTx(phyInstance, sigOut, outputMessage, outputMessageSize);
    }
    if (sigIn)
    {
        // Perform Lte Rx subframe process
        LteBsRx(phyInstance, sigIn, outputMessage, outputMessageSize);
    }
    return (status);
}
