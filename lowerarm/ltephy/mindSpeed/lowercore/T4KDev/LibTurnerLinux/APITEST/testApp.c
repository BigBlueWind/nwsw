/** @file testApp.c
 *
 * @brief I-CPU communication for Linux usage scenarios
 * @author Mindspeed Technologies
 * @version $Revision: 1.3 $
 *
 * COPYRIGHT(c) 2011-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */
 
#include <stdio.h>
#include <unistd.h>
#ifdef USE_GNUPOSIX
#define _GNU_SOURCE
#include <signal.h>
#endif /*USE_GNUPOSIX*/
#ifndef USE_POLL
#include <sys/select.h>
#else
#include <poll.h>
#endif /*USE_POLL*/
#include <sys/time.h>
#include <sched.h>
#include <icpu.h>
#include <ctrlmsg.h>

#define UARM 0
#define ApbTimerReg ApbTimerReg2
signed int mspdMsgQFD_g;
extern int IcpuHandle;
extern U32* volatile ApbTimerReg;
RESULTCODE InitAdi(U32 configBand, U32 configTxAnt, U32 configRxAnt, U32 configLoopback, U32 configDejitter_init); //InitAdi(7,1,1,0,0);
RESULTCODE ApiPktGen(U32 numpkt); //genapi 1 0xffffffff 0
RESULTCODE ApiPktGenOff(void); //genapi off

unsigned int recvDataFromPhysical (void* msgBuf_p)
{
    signed int mq_ret;
    mq_ret = IcpuRxDirect(msgBuf_p);
    return mq_ret;
}

#ifdef USE_RT
RESULTCODE set_realtime()
{
    struct sched_param sp;

    sp.sched_priority = sched_get_priority_max(SCHED_FIFO);
    if (sched_setscheduler(0, SCHED_FIFO, &sp) >= 0) return TRUE;
    printf("sched_setscheduler");
    return FALSE;
}
#endif /* USE_RT */

RESULTCODE InitAdi(U32 configBand, U32 configTxAnt, U32 configRxAnt, U32 configLoopback, U32 configDejitter_init)
{
	U16 ConSrcID = 0x00ff;
    printf("ADI: Band=%d, TxAnt=%d, RxAnt=%d, Loopback=%d, Dejitter_init=%d\n", configBand, configTxAnt, configRxAnt, configLoopback, configDejitter_init);

    return MsgCommand(ConSrcID, MSGS_DI | MSGS_LARM, ADI_BAND, 5, // 5 parameters send to larm
                      1, TYPE_U32, configBand,              // band
                      3, TYPE_U32, configTxAnt,             // txant
                      4, TYPE_U32, configRxAnt,             // rxant
                      5, TYPE_U32, configLoopback,          // loopback
                      2, TYPE_U32, configDejitter_init);    // init dejitter

}

RESULTCODE ApiPktGen(U32 numpkt)
{
	U16 ConSrcID = 0x00ff;
	return MsgCommand(ConSrcID, MSGS_SVSR | MSGS_LARM, 0xF80D, 3,
						2, TYPE_U32, 1,
						3, TYPE_U32, numpkt,
						4, TYPE_U32, 0);
}

RESULTCODE ApiPktGenOff(void)
{
	U16 ConSrcID = 0x00ff;
	return MsgCommand(ConSrcID, MSGS_SVSR | MSGS_LARM, 0xF80D, 1,
                                                2, TYPE_U32, 0);
                                                
}

int main(int argc, char *argv[])
{
#ifndef USE_POLL
    fd_set readfds;
#else
    nfds_t nfds = 0;
#endif /* USE_POLL */
    signed int highestFD = 0;
    unsigned char   msgBuf[16384] = {0};
    signed int retval = 0;
    unsigned int discardBytes = 0;
	U32 tt = 0, ttmin=0xffffffff, ttmax=0, ttavg=0, counterTTI=1000, TTImin=0xffffffff, TTImax=0, TTIavg=0, prevTTI=150000, currTTI=150000;
	int checkpoint_tt = 0;
#ifndef USE_POLL
	static struct timeval timeout;
#else
#ifdef USE_GNUPOSIX
    struct timespec timeout_ts;
    //sigset_t sigmask;
#else
	static int timeout;
#endif /* USE_GNUPOSIX */
	struct pollfd pollFD, *pfd;
#endif /* USE_POLL */
#ifdef USE_RT
	if (set_realtime()) printf("realtime priority set\n");
#endif /* USE_RT */
    InitSettings(argc, argv);
    DiagInit();
    if (SUCCESS != MsgDispatcherInit()) printf("MsgDispatcherInit Failed!!!\n");
    IcpuInit(UARM);
    mspdMsgQFD_g = IcpuHandle;
    printf("MAC:ICPU FD=%d\n",mspdMsgQFD_g);
     if (argc > 2) {
	if (SUCCESS != InitAdi(7,1,1,0,0)) printf("ADI Init Failed!!!\n");
	usleep(3000000);
	}
#ifdef USE_RT
	printf("\nTimer granularity test initialization\n");
	tt = (U32) *ApbTimerReg;
	for ( checkpoint_tt=10; checkpoint_tt >0; checkpoint_tt-- )
	{
		printf("Checkpoint interval: %u us\n",((U32)*ApbTimerReg - tt)/150);
		tt = (U32) *ApbTimerReg;
		usleep(3);
	}
	printf("\nTight Loop Check\n");
	U32 tttmax=0, tttmin=0xffffffff, tttavg=0, deltattt=0, prevttt=0, currttt=0;
	for ( checkpoint_tt=1000; checkpoint_tt >0; checkpoint_tt-- )
	{

	// just 2 tt get in row 1000 times, see timing.
	prevttt = (U32) *ApbTimerReg;
	currttt = (U32) *ApbTimerReg;
	deltattt = currttt - prevttt;
		
		if (deltattt < tttmin) tttmin = deltattt;
		if (deltattt > tttmax) tttmax = deltattt;
		if (tttavg == 0) tttavg = deltattt;
			else tttavg = (tttavg + deltattt + 1)/2;
	}
		printf("tttmax: %u ticks, tttmin: %u ticks, tttavg: %u ticks\n",tttmax,tttmin,tttavg);

	printf("\nUsleep(50) Loop Check\n");
	tttmax=0; tttmin=0xffffffff; tttavg=0; deltattt=0; prevttt=0; currttt=0;
        for ( checkpoint_tt=1000; checkpoint_tt >0; checkpoint_tt-- )
        {
        prevttt = (U32) *ApbTimerReg;
	usleep(50);
        currttt = (U32) *ApbTimerReg;
        deltattt = currttt - prevttt;

                if (deltattt < tttmin) tttmin = deltattt;
                if (deltattt > tttmax) tttmax = deltattt;
                if (tttavg == 0) tttavg = deltattt;
                        else tttavg = (tttavg + deltattt + 1)/2;
        }
                printf("DevMax: %u us, DevMin: %u us, DevAvg: %u us\n\n",tttmax/150-50,tttmin/150-50,tttavg/150-50);



#endif /* USE_RT */
    discardBytes = recvDataFromPhysical((void *) msgBuf);
    printf("Discarding Bytes = %d\n",discardBytes );
	if (argc > 1) counterTTI = (U32)strtoul(argv[1], NULL, 0);
	if (SUCCESS != ApiPktGen(counterTTI)) printf("ApiPktGen Failed!!!\n");
	 else printf("ApiPktGen started successefully for %u TTIs\n",counterTTI);
#ifndef USE_POLL
    timeout.tv_sec=0;
    timeout.tv_usec=0;
    printf("testApp: select() call will be used to wait. Wait time is:%6d.%6d\n",timeout.tv_sec,timeout.tv_usec);
#else
#ifdef USE_GNUPOSIX
    timeout_ts.tv_sec = 0;
    timeout_ts.tv_nsec = 0;
    printf("testApp: ppoll() call will be used to wait with infinite time.\n");
#else
    timeout = -1;
    printf("testApp: poll() call will be used to wait with timeout: %d us.\n", timeout);
#endif /* USE_GNUPOSIX */
    pfd = &pollFD;
#endif /* USE_POLL */
	
    while(1)
    {
#ifndef USE_POLL
        FD_ZERO(&readfds);
        FD_SET(mspdMsgQFD_g, &readfds);
        if (mspdMsgQFD_g > highestFD) 
            highestFD = mspdMsgQFD_g;
#else
	if (mspdMsgQFD_g > highestFD)
	{
	   highestFD = mspdMsgQFD_g;
	   pollFD.fd = mspdMsgQFD_g;
	   pollFD.events = POLLIN;
	   pollFD.revents = 0;
	   nfds = highestFD + 1;
	}
#endif /* USE_POLL */

#ifdef USE_RT 
#endif /* USE_RT */

#ifndef USE_POLL
#ifndef USE_GNUPOSIX
        if(0 != (retval = select(highestFD + 1, &readfds, NULL, NULL, NULL)))
#else
	if(0 != (retval = pselect(highestFD + 1, &readfds, NULL, NULL, NULL, NULL)))
#endif /* USE_GNUPOSIX */
#else
#ifndef USE_GNUPOSIX
	retval = poll(pfd, nfds, timeout);
#else
	retval = ppoll(pfd, nfds, NULL, NULL);
#endif /* USE_GNUPOSIX */
	if(0 != retval)
#endif /* USE_POLL */
        {
#ifndef USE_POLL
            if (FD_ISSET(mspdMsgQFD_g, &readfds))
#else
	    if ((pollFD.revents & POLLIN) == POLLIN)
#endif /* USE_POLL */
	    { 
                discardBytes = recvDataFromPhysical((void *) msgBuf);
		if (discardBytes>0) {
			if ( *(U16*)(msgBuf+16) == 3 ) 
		{

		U32 deltatt, deltaTTI;
		prevTTI = currTTI;
		currTTI = (U32)*ApbTimerReg;
		deltatt = (currTTI - *(U32*)(msgBuf+20));
		if (prevTTI == 150000) deltaTTI = prevTTI;
		else deltaTTI = currTTI-prevTTI;
		if (deltatt < ttmin) ttmin = deltatt;
		if (deltatt > ttmax) ttmax = deltatt;
		if (ttavg == 0) ttavg = deltatt;
			else ttavg = (ttavg + deltatt + 1)/2;
		if (deltaTTI < TTImin) TTImin = deltaTTI;
		if (deltaTTI > TTImax) TTImax = deltaTTI;
		if (TTIavg == 0) TTIavg = deltaTTI;
			else TTIavg = (TTIavg + deltaTTI + 1)/2;
				
		if (ttmax > 100 * 150) { ApiPktGenOff();
		printf("Threshold reached at TTI: %u\nttmax: %u us, ttmin: %u us, ttavg: %u us, lastdeltatt: %u us\n",counterTTI,ttmax/150,ttmin/150,ttavg/150,deltatt/150);
		printf("TTI times - TTImax: %u, TTImin: %u, TTIavg; %u, TTIlast:%u\n",TTImax/150,TTImin/150,TTIavg/150,deltaTTI/150);
		break;
		 }
		if (counterTTI < 2) {
		printf("ttmax: %u us, ttmin: %u us, ttavg: %u us, lastdeltatt: %u us\n",ttmax/150,ttmin/150,ttavg/150,deltatt/150);
		printf("TTI times - TTImax: %u, TTImin: %u, TTIavg; %u, TTIlast:%u\n",TTImax/150,TTImin/150,TTIavg/150,deltaTTI/150);
                break;
                }
		else  counterTTI--; 

		}
		}
				
            }
        }
    }

    return 0;
}

