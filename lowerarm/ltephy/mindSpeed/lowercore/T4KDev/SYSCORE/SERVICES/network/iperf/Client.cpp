/*---------------------------------------------------------------
 * Copyright (c) 1999,2000,2001,2002,2003                              
 * The Board of Trustees of the University of Illinois            
 * All Rights Reserved.                                           
 *--------------------------------------------------------------- 
 * Permission is hereby granted, free of charge, to any person    
 * obtaining a copy of this software (Iperf) and associated       
 * documentation files (the "Software"), to deal in the Software  
 * without restriction, including without limitation the          
 * rights to use, copy, modify, merge, publish, distribute,        
 * sublicense, and/or sell copies of the Software, and to permit     
 * persons to whom the Software is furnished to do
 * so, subject to the following conditions: 
 *
 *     
 * Redistributions of source code must retain the above 
 * copyright notice, this list of conditions and 
 * the following disclaimers. 
 *
 *     
 * Redistributions in binary form must reproduce the above 
 * copyright notice, this list of conditions and the following 
 * disclaimers in the documentation and/or other materials 
 * provided with the distribution. 
 * 
 *     
 * Neither the names of the University of Illinois, NCSA, 
 * nor the names of its contributors may be used to endorse 
 * or promote products derived from this Software without
 * specific prior written permission. 
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
 * NONINFRINGEMENT. IN NO EVENT SHALL THE CONTIBUTORS OR COPYRIGHT 
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. 
 * ________________________________________________________________
 * National Laboratory for Applied Network Research 
 * National Center for Supercomputing Applications 
 * University of Illinois at Urbana-Champaign 
 * http://www.ncsa.uiuc.edu
 * ________________________________________________________________ 
 *
 * Client.cpp
 * by Mark Gates <mgates@nlanr.net>
 * -------------------------------------------------------------------
 * A client thread initiates a connect to the server and handles
 * sending and receiving data, then closes the socket.
 * ------------------------------------------------------------------- */

#include "headers.h"
#include "Client.hpp"
#include "Thread.h"
#include "SocketAddr.h"
#include "PerfSocket.hpp"
#include "Extractor.h"
//#include "driver/delay.h"
#include "util.h"
#include "locale.h"


/* -------------------------------------------------------------------
 * Store server hostname, optionally local hostname, and socket info.
 * ------------------------------------------------------------------- */

Client::Client( thread_Settings *inSettings ) {
    mSettings = inSettings;
    mBuf = NULL;

    // initialize buffer
    mBuf = new char[ mSettings->mBufLen ];
    pattern( mBuf, mSettings->mBufLen );
    if ( isFileInput( mSettings ) ) {
        if ( !isSTDIN( mSettings ) )
            Extractor_Initialize( mSettings->mFileName, mSettings->mBufLen, mSettings );
        else
            Extractor_InitializeFile( stdin, mSettings->mBufLen, mSettings );

        if ( !Extractor_canRead( mSettings ) ) {
            unsetFileInput( mSettings );
        }
    }

    // connect
    Connect( );

    if ( isReport( inSettings ) ) {
        ReportSettings( inSettings );
        if ( mSettings->multihdr && isMultipleReport( inSettings ) ) {
            mSettings->multihdr->report->connection.peer = mSettings->peer;
            mSettings->multihdr->report->connection.size_peer = mSettings->size_peer;
            mSettings->multihdr->report->connection.local = mSettings->local;
            SockAddr_setPortAny( &mSettings->multihdr->report->connection.local );
            mSettings->multihdr->report->connection.size_local = mSettings->size_local;
        }
    }

} // end Client

/* -------------------------------------------------------------------
 * Delete memory (hostname strings).
 * ------------------------------------------------------------------- */

Client::~Client() {
    if ( mSettings->mSock != INVALID_SOCKET ) {
        int rc = close( mSettings->mSock );
        WARN_errno( rc == SOCKET_ERROR, "close" );
        mSettings->mSock = INVALID_SOCKET;
    }
    DELETE_ARRAY( mBuf );
} // end ~Client
#ifdef IPERF_DELAY
const double kSecs_to_usecs = 1e6; 
const int    kBytes_to_Bits = 8; 
#endif
/* ------------------------------------------------------------------- 
 * Send data using the connected UDP/TCP socket, 
 * until a termination flag is reached. 
 * Does not close the socket. 
 * ------------------------------------------------------------------- */ 

void Client::Run( void ) {
    struct UDP_datagram* mBuf_UDP = (struct UDP_datagram*) mBuf; 
    long currLen = 0;
#ifdef IPERF_DELAY
    int delay_target = 0; 
    int delay = 0; 
    int adjust = 0; 
#endif
    
    // Indicates if the stream is readable 
    bool canRead = true, mMode_Time = isModeTime( mSettings ); 

    // setup termination variables
    if ( mMode_Time ) {
        mEndTime.setnow();
        mEndTime.add( mSettings->mAmount / 100.0 );
    }
#ifdef IPERF_DELAY
    if ( isUDP( mSettings ) ) {
        // Due to the UDP timestamps etc, included 
        // reduce the read size by an amount 
        // equal to the header size

        // compute delay for bandwidth restriction, constrained to [0,1] seconds 
        delay_target = (int) ( mSettings->mBufLen * ((kSecs_to_usecs * kBytes_to_Bits) 
                                                     / mSettings->mUDPRate) ); 
        if ( delay_target < 0  || 
             delay_target > (int) 1 * kSecs_to_usecs ) {
            fprintf( stderr, warn_delay_large, delay_target / kSecs_to_usecs ); 
            delay_target = (int) kSecs_to_usecs * 1; 
        }
    }
#endif

    // InitReport handles Barrier for multiple Streams
    ReportStruct *reportstruct = NULL;

#ifdef IPERF_REPORT
    mSettings->reporthdr = InitReport( mSettings );
#endif
    
    reportstruct = new ReportStruct;
    reportstruct->packetID = 0;

    lastPacketTime.setnow();
    
    do {

        // Test case: drop 17 packets and send 2 out-of-order: 
        // sequence 51, 52, 70, 53, 54, 71, 72 
        //switch( datagramID ) { 
        //  case 53: datagramID = 70; break; 
        //  case 71: datagramID = 53; break; 
        //  case 55: datagramID = 71; break; 
        //  default: break; 
        //} 
        _gettimeofday( &(reportstruct->packetTime), NULL );  

        if ( isUDP( mSettings ) ) {
            // store datagram ID into buffer 
            mBuf_UDP->id      = htonl(reportstruct->packetID); 
            reportstruct->packetID++;
            mBuf_UDP->tv_sec  = htonl( reportstruct->packetTime.tv_sec ); 
            mBuf_UDP->tv_usec = htonl( reportstruct->packetTime.tv_usec );


#ifdef IPERF_DELAY
            // delay between writes 
            // make an adjustment for how long the last loop iteration took 
            // TODO this doesn't work well in certain cases, like 2 parallel streams 
            adjust = delay_target + lastPacketTime.subUsec( reportstruct->packetTime );
            lastPacketTime.set( reportstruct->packetTime.tv_sec,
                                reportstruct->packetTime.tv_usec );

            if ( adjust > 0  ||  delay > 0 ) {
                delay += adjust; 
            }
#endif
        }

        // perform write 
        currLen = _write( mSettings->mSock, mBuf, mSettings->mBufLen );
        if ( currLen < 0 && errno != ENOBUFS ) {
            WARN_errno( currLen < 0, "write2" ); 
            break; 
        }

        reportstruct->packetLen = currLen;
#ifdef IPERF_REPORT
        // report packets 
        ReportPacket( mSettings->reporthdr, reportstruct );
#endif
#ifdef IPERF_DELAY
        if ( delay > 0 ) {
            _delay_loop( delay );
        }
#endif
        if ( !mMode_Time ) {
            /* mAmount may be unsigned, so don't let it underflow! */
            if( mSettings->mAmount >= currLen ) {
                mSettings->mAmount -= currLen;
            } else {
                mSettings->mAmount = 0;
            }
        }

    } while ( ! (sInterrupted  || 
                 (mMode_Time   &&  mEndTime.before( reportstruct->packetTime ))  || 
                 (!mMode_Time  &&  0 >= mSettings->mAmount)) && canRead ); 


    // stop timing
    _gettimeofday( &(reportstruct->packetTime), NULL );  
#ifdef IPERF_REPORT
    CloseReport( mSettings->reporthdr, reportstruct );
#endif

    if ( isUDP( mSettings ) && !sInterrupted ) {
        // send a final terminating datagram 
        // Don't count in the mTotalLen. The server counts this one, 
        // but didn't count our first datagram, so we're even now. 
        // The negative datagram ID signifies termination to the server. 
    
        // store datagram ID into buffer 
        mBuf_UDP->id      = htonl( -(reportstruct->packetID)  ); 
        mBuf_UDP->tv_sec  = htonl( reportstruct->packetTime.tv_sec ); 
        mBuf_UDP->tv_usec = htonl( reportstruct->packetTime.tv_usec ); 

        if ( isMulticast( mSettings ) ) {
            _write( mSettings->mSock, mBuf, mSettings->mBufLen );
        } else {
            write_UDP_FIN( ); 
        }
    }
    DELETE_PTR( reportstruct );
#ifdef IPERF_REPORT
    EndReport( mSettings->reporthdr );
#endif
} 
// end Run

void Client::InitiateServer() {
    if ( !isCompat( mSettings ) ) {
        int currLen;
        client_hdr* temp_hdr;
        if ( isUDP( mSettings ) ) {
            UDP_datagram *UDPhdr = (UDP_datagram *)mBuf;
            temp_hdr = (client_hdr*)(UDPhdr + 1);
        } else {
            temp_hdr = (client_hdr*)mBuf;
        }
        Settings_GenerateClientHdr( mSettings, temp_hdr );
        if ( !isUDP( mSettings ) ) {
            currLen = _write( mSettings->mSock, mBuf, sizeof(client_hdr));
            if ( currLen < 0 ) {
                WARN_errno( currLen < 0, "write1" );
            }
        }
    }
}

/* -------------------------------------------------------------------
 * Setup a socket connected to a server.
 * If inLocalhost is not null, bind to that address, specifying
 * which outgoing interface to use.
 * ------------------------------------------------------------------- */

void Client::Connect( ) {
    int rc;
    SockAddr_remoteAddr( mSettings );

    assert( mSettings->mHost != NULL );

    // create an internet socket
    int type = ( isUDP( mSettings )  ?  SOCK_DGRAM : SOCK_STREAM);

    int domain = (SockAddr_isIPv6( &mSettings->peer ) ? 
#ifdef HAVE_IPV6
                  AF_INET6
#else
                  AF_INET
#endif
                  : AF_INET);

    int proto = ( isUDP( mSettings )  ?  IPPROTO_UDP : IPPROTO_TCP);

    mSettings->mSock = socket( domain, type, proto );
    WARN_errno( mSettings->mSock == INVALID_SOCKET, "socket" );

    SetSocketOptions( mSettings );
    
    // connect socket
    rc = connect( mSettings->mSock, (sockaddr*) &mSettings->peer,
                  SockAddr_get_sizeof_sockaddr( &mSettings->peer ));

    FAIL_errno( rc == SOCKET_ERROR, "connect", mSettings );

    getsockname( mSettings->mSock, (sockaddr*) &mSettings->local,
                 &mSettings->size_local );
    getpeername( mSettings->mSock, (sockaddr*) &mSettings->peer,
                 &mSettings->size_peer );
} // end Connect

/* ------------------------------------------------------------------- 
 * Send a datagram on the socket. The datagram's contents should signify 
 * a FIN to the application. Keep re-transmitting until an 
 * acknowledgement datagram is received. 
 * ------------------------------------------------------------------- */ 

void Client::write_UDP_FIN( ) {
    int rc; 
    
#ifdef IPERF_HAVE_SELECT
    fd_set readSet; 
    timeval timeout;
#endif

    int count = 0; 
    while ( count < 10 ) {
        count++; 

        // write data 
        _write( mSettings->mSock, mBuf, mSettings->mBufLen );

#ifdef IPERF_HAVE_SELECT
        // wait until the socket is readable, or our timeout expires 
        FD_ZERO( &readSet );
        FD_SET( mSettings->mSock, &readSet ); 
        timeout.tv_sec  = 0; 
        timeout.tv_usec = 250000; // quarter second, 250 ms 

        rc = select( mSettings->mSock+1, &readSet, NULL, NULL, &timeout ); 
        FAIL_errno( rc == SOCKET_ERROR, "select", mSettings ); 

        if ( rc == 0 ) {
            // select timed out 
            continue; 
        } else {
#else
            _delay(100 * 1000);
#endif
            // socket ready to read 
            rc = read( mSettings->mSock, mBuf, mSettings->mBufLen ); 
            WARN_errno( rc < 0, "read" );
    	    if ( rc < 0 ) {
                break;
            } else if ( rc >= (int) (sizeof(UDP_datagram) + sizeof(server_hdr)) ) { 
               ReportServerUDP( mSettings, (server_hdr*) ((UDP_datagram*)mBuf + 1) );
            }
            return;
#ifdef IPERF_HAVE_SELECT
        }
#endif
    } 
    fprintf( stderr, warn_no_ack, mSettings->mSock, count );
} 
// end write_UDP_FIN 

int Client::_connect(int soc, sockaddr *addr, socklen_t addrlen)
{
    if(!isUDP(mSettings))
    {
        return connect(soc, addr, addrlen);
    }
    return 0;
}


int Client::_write(int soc, char * buff, unsigned int len)
{
    if(isUDP(mSettings))
    {
        send(soc, buff, len, 2);
        return len;
    }
    else
    {
        send(soc, buff, len, 2);
        return len;
    }
}

