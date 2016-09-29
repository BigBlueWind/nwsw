/** @file c4ktrace.h
 *
 * @brief Trace interfaces
 * @author Mindspeed Technologies
 * @version $Revision: 1.3 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef _C4KTRACE_H
#define _C4KTRACE_H

//
// Communication
//
#define MAIL_FDX            _T("\\\\.\\mailslot\\C4KFDX")
#define PIPE_FDX            _T("\\\\.\\pipe\\C4KFDX")

//
// Process IDs
//
#define APP_RTR             17
#define APP_MAC             18
#define APP_WBE             19
#define APP_LBE				20
#define APP_LRCT			21

//
// Message IDs
//
#define MSG_UPDCFG          3       // Test config display to be updated
#define MSG_SETSTEPS        4
#define MSG_PROFILE         5
#define MSG_OVERALL         6
#define MSG_CURRENT         7
#define MSG_PASSED          8
#define MSG_PROGRESS        9
#define MSG_STOP            10      // Test finished. 0 == SUCCESS
#define MSG_FAILURE         11

typedef enum StagesEnum
{
    // WiMAX FDX, LTE FDX
    STG_INITRTR,
    STG_LOADAPP,
    STG_LOADPHYTX,
    STG_LOADPHYRX,
    STG_WAITSHM,
    STG_CREATEPHYMS,
    STG_CREATEMACMS,
    STG_CREATERRCMS,
    STG_WAITCOMM,
    STG_LOADDSP,
    STG_LOADLARM,
    STG_LOADUARM,
    STG_CREATEPHY,
    STG_CREATEMAC,
    STG_CREATERRC,
    STG_CREATECHAN,
    STG_PHYTICK,
    STG_GETSAMPLES,
    STG_MSDECODE,
    STG_MSMACSDU,
    STG_MSTXSTART,
    STG_MSPHYTXSTART,
    STG_SENDSAMPLES,
    STG_RXSDU,
    
	// WiMAX BE
    STG_LOADLIB,
    STG_TX1PUSC,
    STG_TX2PUSCSTC,
    STG_RX1PUSC,
    STG_RXTXPUSC,
    STG_RXTXMIMO,
    STG_TX0,
    STG_TX1,
    STG_TX2,
    STG_TX3,
    STG_RX0,
    STG_RX1,
    STG_BIN,
    STG_MAC,
    STG_COMPARE0I,
    STG_COMPARE0Q,
    STG_COMPARE1I,
    STG_COMPARE1Q,
    STG_COMPARE2I,
    STG_COMPARE2Q,
    STG_COMPARE3I,
    STG_COMPARE3Q,

    // LTE BE
    STG_DONE,

	// pre-RCT TTCN-3
    STG_DELIM,
	STG_PFVERDICT,
	STG_PF_LTE_RCT_6_1_TC000,
	STG_PF_LTE_RCT_6_1_TC001,
	STG_PF_LTE_RCT_6_1_TC002,
	STG_PF_LTE_RCT_6_1_TC003,
	
	//LTE pre-RCT FDX
	STG_INITMS,
	STG_DEINITMS,
	STG_RRCCR,
	STG_RRCCS,
	STG_RRCCSC,

    // Common
    STG_INFO,
    STG_CONFIG,
    STG_PASSFAIL,
    STG_STOP,

    STG_CHILD,

	// New WiMAX tests
	STG_TEST0 = STG_CHILD + 1,
	STG_TEST1 = STG_TEST0 + 20,
	STG_TEST2 = STG_TEST1 + 20,
	STG_TEST3 = STG_TEST2 + 20,
	STG_TEST4 = STG_TEST3 + 20,
	STG_TEST5 = STG_TEST4 + 20,
	STG_TEST6 = STG_TEST5 + 20,
	STG_TEST7 = STG_TEST6 + 20,
	STG_TEST8 = STG_TEST7 + 20,
	STG_TEST9 = STG_TEST8 + 20,
	STG_TEST10 = STG_TEST9 + 20,
	STG_TEST11 = STG_TEST10 + 20,
	STG_TEST12 = STG_TEST11 + 20,
	STG_TEST13 = STG_TEST12 + 20,
	STG_TEST14 = STG_TEST13 + 20,
	STG_TEST15 = STG_TEST14 + 20,
	STG_TEST16 = STG_TEST15 + 20,
	STG_TEST17 = STG_TEST16 + 20,
	STG_TEST18 = STG_TEST17 + 20,
	STG_TEST19 = STG_TEST18 + 20,

	//LTE RCT TS 36.523
	STG_INITUE,
	STG_INITENB,
	STG_PWRONUE,
	STG_SWONUE,
	STG_CONNREQ,
	STG_CONNSETUP,
	STG_CONNSETUPCOMP,
	STG_CONNREL,
	STG_CONNREQV,
    // Add new values at the end of the list (or reuse old) to keep compatibility

} STAGES_ENUM;

typedef struct MailMessage
{
    int Process;
    int Stage;
    int MsgID;
    int Progress;
    int Data[4];
} MAIL_MESSAGE;

#ifdef _MSC_VER
#if _MSC_VER <= 1200
typedef long intptr_t;
#endif
#endif

#if defined (USE_WINDOWS) || defined (WIN32)
#define C4KTRACE(stage,msgid,progress)                  C4KTrace(stage,msgid,progress)
#define C4KTRACEEX(stage,msgid,progress,d0,d1,d2,d3)    C4KTraceEx(stage,msgid,progress,d0,d1,d2,d3)
#else /* USE_WINDOWS || WIN32 */
#define C4KTRACE(stage,msgid,progress)
#define C4KTRACEEX(stage,msgid,progress,d0,d1,d2,d3)
#endif /* USE_WINDOWS || WIN32 */

#ifdef __cplusplus
extern "C" 
{
#endif

void C4KTraceInit(int appid, int usepipe);
void C4KTrace(int stage, int msgid, int progress);
void C4KTraceEx(int stage, int msgid, int progress, int data0, int data1, int data2, int data3);
void C4KTraceDone(void);

#ifdef __cplusplus
};
#endif

#endif /* _C4KTRACE_H */
