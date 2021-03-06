/* generated by /home/travula/ltecode/nwsw/build/scripts/gen_eventnames.pl on Wed Feb 13 13:11:13 2013 */
#include "bs_tables.h"
#include "ueh_fdecl.h"
const char *uehEventName[] = {
    [EVENTTYPE_UNKNOWN] = "UNKNOWN",
    [EVENTTYPE_RRCCONNREQ] = "RRCCONNREQ",
    [EVENTTYPE_RRCCONNSETUP] = "RRCCONNSETUP",
    [EVENTTYPE_RRCCONNREJ] = "RRCCONNREJ",
    [EVENTTYPE_RRCCONNSETUPCOMPL] = "RRCCONNSETUPCOMPL",
    [EVENTTYPE_INITIALUEMSG] = "INITIALUEMSG",
    [EVENTTYPE_UECAPABILITYENQ] = "UECAPABILITYENQ",
    [EVENTTYPE_UECAPABILITYINFO] = "UECAPABILITYINFO",
    [EVENTTYPE_UECAPABILITYINFOIND] = "UECAPABILITYINFOIND",
    [EVENTTYPE_RRCCONNRECONFIG] = "RRCCONNRECONFIG",
    [EVENTTYPE_RRCCONNRECONFCOMPL] = "RRCCONNRECONFCOMPL",
    [EVENTTYPE_SECMODECOMMAND] = "SECMODECOMMAND",
    [EVENTTYPE_SECMODECOMPL] = "SECMODECOMPL",
    [EVENTTYPE_SECMODEFAIL] = "SECMODEFAIL",
    [EVENTTYPE_INITCONTEXTSETUPREQ] = "INITCONTEXTSETUPREQ",
    [EVENTTYPE_INITCONTEXTSETUPRESP] = "INITCONTEXTSETUPRESP",
    [EVENTTYPE_INITCONTEXTSETUPFAIL] = "INITCONTEXTSETUPFAIL",
    [EVENTTYPE_RRCCONNRELEASE] = "RRCCONNRELEASE",
    [EVENTTYPE_RRCREESTABREQ] = "RRCREESTABREQ",
    [EVENTTYPE_RRCREESTABRES] = "RRCREESTABRES",
    [EVENTTYPE_RRCREESTABREJ] = "RRCREESTABREJ",
    [EVENTTYPE_RRCREESTABCOM] = "RRCREESTABCOM",
    [EVENTTYPE_RRCCOUNTERCHECKREQ] = "RRCCOUNTERCHECKREQ",
    [EVENTTYPE_RRCCOUNTERCHECKRES] = "RRCCOUNTERCHECKRES",
    [EVENTTYPE_ULINFTRANSFER] = "ULINFTRANSFER",
    [EVENTTYPE_ULNASTRANSPORTMSG] = "ULNASTRANSPORTMSG",
    [EVENTTYPE_DLINFTRANSFER] = "DLINFTRANSFER",
    [EVENTTYPE_DLNASTRANSPORTMSG] = "DLNASTRANSPORTMSG",
    [EVENTTYPE_LOCATIONREPORTCTRL] = "LOCATIONREPORTCTRL",
    [EVENTTYPE_LOCATIONREPORTFAILUREIND] = "LOCATIONREPORTFAILUREIND",
    [EVENTTYPE_LOCATIONREPORTRES] = "LOCATIONREPORTRES",
    [EVENTTYPE_TRACESTART] = "TRACESTART",
    [EVENTTYPE_TRACEFAILUREIND] = "TRACEFAILUREIND",
    [EVENTTYPE_DEACTIVATETRACE] = "DEACTIVATETRACE",
    [EVENTTYPE_DEACTIVATETRACEFAILUREIND] = "DEACTIVATETRACEFAILUREIND",
    [EVENTTYPE_OUTHOTRIGGERED] = "OUTHOTRIGGERED",
    [EVENTTYPE_OUTHOCOMPLETE] = "OUTHOCOMPLETE",
    [EVENTTYPE_OUTHOFAIL] = "OUTHOFAIL",
    [EVENTTYPE_INHOREQUEST] = "INHOREQUEST",
    [EVENTTYPE_INHOCOMPLETE] = "INHOCOMPLETE",
    [EVENTTYPE_INHOFAIL] = "INHOFAIL",
    [EVENTTYPE_HONOTIFY] = "HONOTIFY",
    [EVENTTYPE_PATHSWITCHREQ] = "PATHSWITCHREQ",
    [EVENTTYPE_PATHSWITCHRES] = "PATHSWITCHRES",
    [EVENTTYPE_PATHSWITCHFAIL] = "PATHSWITCHFAIL",
    [EVENTTYPE_HOCANCEL] = "HOCANCEL",
    [EVENTTYPE_HOCANCELACK] = "HOCANCELACK",
    [EVENTTYPE_RABSETUPREQ] = "RABSETUPREQ",
    [EVENTTYPE_RABSETUPCOMPL] = "RABSETUPCOMPL",
    [EVENTTYPE_RABSETUPFAIL] = "RABSETUPFAIL",
    [EVENTTYPE_RABMODREQ] = "RABMODREQ",
    [EVENTTYPE_RABMODRES] = "RABMODRES",
    [EVENTTYPE_RABMODFAIL] = "RABMODFAIL",
    [EVENTTYPE_RABRELREQ] = "RABRELREQ",
    [EVENTTYPE_RABRELCOMPL] = "RABRELCOMPL",
    [EVENTTYPE_RABRELIND] = "RABRELIND",
    [EVENTTYPE_UECONTEXTRELREQ] = "UECONTEXTRELREQ",
    [EVENTTYPE_UECONTEXTRELCMD] = "UECONTEXTRELCMD",
    [EVENTTYPE_UECONTEXTRELCOM] = "UECONTEXTRELCOM",
    [EVENTTYPE_UECONTEXTMODREQ] = "UECONTEXTMODREQ",
    [EVENTTYPE_UECONTEXTMODRESP] = "UECONTEXTMODRESP",
    [EVENTTYPE_UECONTEXTMODFAIL] = "UECONTEXTMODFAIL",
    [EVENTTYPE_RLFAILURE] = "RLFAILURE",
    [EVENTTYPE_RLRECOVERY] = "RLRECOVERY",
    [EVENTTYPE_RRCBNDREQ] = "RRCBNDREQ",
    [EVENTTYPE_RRCCELLCFG] = "RRCCELLCFG",
    [EVENTTYPE_SCHBNDREQ] = "SCHBNDREQ",
    [EVENTTYPE_SCHCELLCONFIG] = "SCHCELLCONFIG",
    [EVENTTYPE_SCHLCHCONFIG] = "SCHLCHCONFIG",
    [EVENTTYPE_SCHLCGCONFIG] = "SCHLCGCONFIG",
    [EVENTTYPE_SCHUECONFIG] = "SCHUECONFIG",
    [EVENTTYPE_SCHUERECONFIG] = "SCHUERECONFIG",
    [EVENTTYPE_SCHUEDELETE] = "SCHUEDELETE",
    [EVENTTYPE_SCHSICONFIG] = "SCHSICONFIG",
    [EVENTTYPE_PHYBNDREQ] = "PHYBNDREQ",
    [EVENTTYPE_PHYUECONFIG] = "PHYUECONFIG",
    [EVENTTYPE_PHYCELLCONFIG] = "PHYCELLCONFIG",
    [EVENTTYPE_PHYUEDELETE] = "PHYUEDELETE",
    [EVENTTYPE_UDHBNDREQ] = "UDHBNDREQ",
    [EVENTTYPE_UDHADDRABREQ] = "UDHADDRABREQ",
    [EVENTTYPE_UDHDELRABREQ] = "UDHDELRABREQ",
    [EVENTTYPE_UDHDELUEREQ] = "UDHDELUEREQ",
    [EVENTTYPE_RRCUECAPDECODEREQ] = "RRCUECAPDECODEREQ",
    [EVENTTYPE_S1BNDREQ] = "S1BNDREQ",
    [EVENTTYPE_DLNASNONDELIVERYIND] = "DLNASNONDELIVERYIND",
    [EVENTTYPE_UEDELETED] = "UEDELETED",
    [EVENTTYPE_INTERNAL] = "INTERNAL",
    [EVENTTYPE_OPSTOPRELEASE] = "OPSTOPRELEASE",
    [EVENTTYPE_RRCLICFGCFM] = "RRCLICFGCFM",
    [EVENTTYPE_RRCERRIND] = "RRCERRIND",
    [EVENTTYPE_UERESTMREXP] = "UERESTMREXP",
    [EVENTTYPE_SRB1TMREXP] = "SRB1TMREXP",
    [EVENTTYPE_RRCCONNRELL2ACKTMREXP] = "RRCCONNRELL2ACKTMREXP",
    [EVENTTYPE_UECTXRELTMREXP] = "UECTXRELTMREXP",
    [EVENTTYPE_UECRNTIRETENTMREXP] = "UECRNTIRETENTMREXP",
    [EVENTTYPE_RRCENCCFM] = "RRCENCCFM",
    [EVENTTYPE_RRCDECCFM] = "RRCDECCFM",
    [EVENTTYPE_EGTERRIND] = "EGTERRIND",
    [EVENTTYPE_UDHERABADDCFM] = "UDHERABADDCFM",
    [EVENTTYPE_UDHUEINACTVIND] = "UDHUEINACTVIND",
    [EVENTTYPE_UDHERABDELCFM] = "UDHERABDELCFM",
    [EVENTTYPE_UECTFCFGCFM] = "UECTFCFGCFM",
    [EVENTTYPE_UDHULINACTVIND] = "UDHULINACTVIND",
    [EVENTTYPE_RRCCOUNTCFM] = "RRCCOUNTCFM",
    [EVENTTYPE_UECOUNTERCHECKRSP] = "UECOUNTERCHECKRSP",
    [EVENTTYPE_S1APHOCMD] = "S1APHOCMD",
    [EVENTTYPE_S1APHOPREPFAILED] = "S1APHOPREPFAILED",
    [EVENTTYPE_S1APENCDECCFM] = "S1APENCDECCFM",
    [EVENTTYPE_MMERELINDTMREXP] = "MMERELINDTMREXP",
    [EVENTTYPE_UESCHCFGCFM] = "UESCHCFGCFM",
    [EVENTTYPE_UENHUDATACFM] = "UENHUDATACFM",
    [EVENTTYPE_S1CLEAR] = "S1CLEAR",
    [EVENTTYPE_S1APERRIND] = "S1APERRIND",
    [EVENTTYPE_EGTPTUNNELMGMTCFM] = "EGTPTUNNELMGMTCFM",
    [EVENTTYPE_UDHDELUECFM] = "UDHDELUECFM",
    [EVENTTYPE_PAGING] = "PAGING",
    [EVENTTYPE_INTCONFTMREXP] = "INTCONFTMREXP",
    [EVENTTYPE_RRCCONNESTASUCC] = "RRCCONNESTASUCC",
    [EVENTTYPE_INITIALCTXSETUPSUCC] = "INITIALCTXSETUPSUCC",
    [EVENTTYPE_SECURITYSUCC] = "SECURITYSUCC",
    [EVENTTYPE_UECAPABILITYSUCC] = "UECAPABILITYSUCC",
    [EVENTTYPE_RRCRECONFIGSUCC] = "RRCRECONFIGSUCC",
    [EVENTTYPE_UECTXRELEASESUCC] = "UECTXRELEASESUCC",
    [EVENTTYPE_UEENTERED] = "UEENTERED",
    [EVENTTYPE_MACL2MEAS] = "MACL2MEAS",
    [EVENTTYPE_RLCL2MEAS] = "RLCL2MEAS",
    [EVENTTYPE_PDCPL2MEAS] = "PDCPL2MEAS",
    [EVENTTYPE_S1APLOCALERRIND] = "S1APLOCALERRIND",
    [EVENTTYPE_RRCLOCALERRIND] = "RRCLOCALERRIND",
    [EVENTTYPE_PHYKDFREQ] = "PHYKDFREQ",
    [EVENTTYPE_PHYKDFRSP] = "PHYKDFRSP",
    [EVENTTYPE_RESERVED1] = "RESERVED1",
    [EVENTTYPE_RESERVED2] = "RESERVED2",
    [EVENTTYPE_RESERVED3] = "RESERVED3",
    [EVENTTYPE_RESERVED4] = "RESERVED4",
    [EVENTTYPE_RESERVED5] = "RESERVED5",
    [EVENTTYPE_OTHER] = "OTHER"
};
const char *uehGetEventName(UINT8 event)
{ return uehEventName[event]; }
