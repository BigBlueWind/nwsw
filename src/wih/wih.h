/**
 * @file    wih.h
 * @brief API for Web Interface Handler (Abbrev: WIH)
 * @details Copyright Powerwave Technologies, Inc., 2009
 *
 * Author         : Tara Shankar Maji
 * Date           : 13-09-2010
 * Change History : 13-09-10 Initial version created.
 */

/**
 * WIH = Web Interface Handler
 */

#ifndef __WIH_H__
#define __WIH_H__

#include <time.h>
#include <pthread.h>
#include <dirent.h>

extern "C"
{
#include "all_cfg.h"
#include "print_tlv.h"
#include "start_indication.h"

}

/////////////////////// DEFINITIONS /////////////////////
#define MAX_SCRIPT_LENGTH       144
#define MAX_REQ_PAGE_LENGTH     128
#define RES_BUFF_LEN		128
#define MAX_PARAM_NAME_LENGTH   64
#define MAX_VALUE_LENGTH        256
#define MAX_TABLE_NAME_LENGTH   128
#define MAX_REQ_PARAMS          255
#define MAX_QUERY_STR_LENGTH    1024
#define MAX_WBSVR_STARTUP_STR   640
#define NULL_CONTEXT		0
#define MAX_WEBCRAFT_USERS              8 
#define WEBCRAFT_INACTIVITY_TIME        1800  // In seconds (30 mins = 1800 sec)
#define WEBCRAFT_CMD_TIMEOUT_VAL	10 // In seconds
#define WIH_INSTANCE_ID   	OAMIF_WIH
#define FIELDNAME_KEY   	"key"
#define FIELDNAME_ROW   	"webGuiRow"
#define KEY_LENGTH      	8
#define ROW_LENGTH      	16

#define DOC_ROOT	"/tmp/web/html/"

/*///////////// Basic pages used internally ///////////////*/
#define INDEX_PAGE       	"index.htm"
#define CONFIG_INDEX_PAGE	"configuration_index.htm"
#define MONITOR_INDEX_PAGE	"monitor_index.htm"
#define COMMANDS_INDEX_PAGE	"commands_index.htm"
#define TOP_PAGE       		"top.htm"
#define BOTTOM_PAGE      	"bottom.htm"
#define CONFIG_PAGE      	"config.htm"
#define MONITOR_PAGE     	"monitor.htm"
#define COMMAND_PAGE     	"commands.htm"
#define RESULT_PAGE      	"result.htm"
#define HELP_PAGE           "help.htm"

/*/////////////// Command Pages //////////////*/
#define ENB_CMD_PAGE		"enbadmin_command.htm"
#define TRACE_CMD_PAGE		"tracecontrol_command.htm"
#define DWNLD_CMD_PAGE		"filedownload_command.htm"
#define UPLD_CMD_PAGE		"fileupload_command.htm"
#define SWINST_CMD_PAGE		"swinstall_command.htm"
#define SWACTV_CMD_PAGE		"swactivation_command.htm"
#define CELLADMIN_CMD_PAGE    "celladmin_command.htm"
#define COUNTERCTRL_CMD_PAGE    "countercontrol_command.htm"

/*/////////////// Config Pages //////////////*/
#define GEN_DETAIL_PAGE		"general_details.htm"
#define ENBREG_CFG_PAGE		"enbregistration_config.htm" 
#define ENB_CFG_PAGE		"enodeb_config.htm"
#define CELL_CFG_PAGE		"cell_config.htm"
#define SCHDL_CFG_PAGE		"scheduler_config.htm"
#define PHY_CFG_PAGE		"phy_config.htm"
#define SIB1_CFG_PAGE		"sysinfo1_config.htm"
#define SIB2_CFG_PAGE		"sysinfo2_config.htm"
#define SIB3_CFG_PAGE		"sysinfo3_config.htm"
#define LOG_CFG_PAGE		"log_config.htm"
#define RADIOHD_CFG_PAGE  	"radiohead_config.htm"
#define TRC_CFG_PAGE		"trace_config.htm"
#define QOS_CFG_PAGE		"qos_config.htm"
#define PMCOUNTERLOG_CFG_PAGE	"pmcounterlog_config.htm"
#define HO_CFG_PAGE		    "eutrahoprofile_config.htm"
#define NEIG_CFG_PAGE		"eutraneighcell_config.htm"
#define MME_CFG_PAGE		"mme_config.htm"
#define MSGTRC_CFG_PAGE		"msgtrace_config.htm"
#define TIMER_CFG_PAGE		"timers_config.htm"
#define EVTTHR_CFG_PAGE		"eventthresholds_config.htm"
#define ENBADDR_CFG_PAGE	"enbaddresses_config.htm"
#define SRSDEDICATED_CFG_PAGE "srsdedicated_config.htm"
#define SECURITY_CFG_PAGE   "security_config.htm"
#define UEEVENTHISTORYLOG_CFG_PAGE   "ueeventhistorylog_config.htm"
#define COUNTER_CFG_PAGE        "counter_config.htm"

/*/////////////// Status Pages ///////////////*/
#define UPLD_STS_PAGE		"fileupload_status.htm"
#define DWNLD_STS_PAGE		"filedownload_status.htm"
#define SWACT_STS_PAGE   	"swactivation_status.htm"
#define SWPKG_STS_PAGE 		"swpackage_status.htm"
#define COMP_STS_PAGE  		"component_status.htm"
#define ENB_STS_PAGE     	"enb_status.htm"
#define MODULE_STS_PAGE		"module_status.htm"
#define RDH_STS_PAGE		"radiohead_status.htm"
#define TRC_STS_PAGE		"trace_status.htm"
#define FEVTHIS_STS_PAGE	"fileeventhistory_status.htm"
#define SWINST_STS_PAGE		"swinstallhistory_status.htm"
#define ACTUE_STS_PAGE		"activeue_status.htm"
#define ACTRAB_STS_PAGE		"activerab_status.htm"
#define UEEVTHIS_STS_PAGE	"ueeventhistory_status.htm"
#define RABHIS_STS_PAGE		"rabhistory_status.htm"
#define PMLOG_STS_PAGE		"pmlogfile_status.htm"
#define MODLOG_STS_PAGE		"modulelogfile_status.htm"
#define ACTALM_STS_PAGE		"activealarm_status.htm"
#define ALMHIS_STS_PAGE		"alarmhistory_status.htm"
#define LOCATION_STS_PAGE	"location_status.htm"
#define LED_STS_PAGE	               "led_status.htm"
#define GPS_STS_PAGE	               "gps_status.htm"
#define MACGENCOUNTERS_STS_PAGE    	   "macgencounters_status.htm"
#define MACUPLANECOUNTERS_STS_PAGE     "macuplanecounters_status.htm"
#define RLCGENCOUNTERS_STS_PAGE    	   "rlcgencounters_status.htm"
#define PDCPGENCOUNTERS_STS_PAGE   	   "pdcpgencounters_status.htm"
#define PDCPUPLANECOUNTERS_STS_PAGE    "pdcpuplanecounters_status.htm"
#define GTPGENCOUNTERS_STS_PAGE    	   "gtpgencounters_status.htm"
#define SCTPGENCOUNTERS_STS_PAGE   	   "sctpgencounters_status.htm"
#define S1APGENCOUNTERS_STS_PAGE  	   "s1apgencounters_status.htm"
#define S1APPEERNODECOUNTERS_STS_PAGE  "s1appeernodecounters_status.htm"
#define RRCGENCOUNTERS_STS_PAGE 	   "rrcgencounters_status.htm"
#define RRCCELLCOUNTERS_STS_PAGE  	   "rrccellcounters_status.htm"
#define RRMCOUNTERS_STS_PAGE    	   "rrmcounters_status.htm"
#define ETHERNETCOUNTERS_STS_PAGE      "ethernetcounters_status.htm"
#define IPCOUNTERS_STS_PAGE    	       "ipcounters_status.htm"
#define CELL_STS_PAGE                "cell_status.htm"
#define L2COUNTERS_STS_PAGE             "l2counters_status.htm"
#define COUNTERCOLLECTIONSTATUS_STS_PAGE "countercollection_status.htm"


///////////////////// TYPE DEFINITIONS ///////////////////
typedef unsigned long ContextID;

#endif /* __WIH_H__ */
