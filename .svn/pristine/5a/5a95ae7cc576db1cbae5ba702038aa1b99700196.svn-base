
function SetMapping(tableName, response) {
    if (tableName == "swPackageStatus") {
        if (parent.frames["MiddleRight"].document.getElementById("swType").innerHTML == "1")
            parent.frames["MiddleRight"].document.getElementById("swType").innerHTML = "ENB L2L3 Software";
        else
            if (parent.frames["MiddleRight"].document.getElementById("swType").innerHTML == "2")
            parent.frames["MiddleRight"].document.getElementById("swType").innerHTML = "Radiohead Software";
        else
            if (parent.frames["MiddleRight"].document.getElementById("swType").innerHTML == "3")
            parent.frames["MiddleRight"].document.getElementById("swType").innerHTML = "ENB PHY Software";
        else
            if (parent.frames["MiddleRight"].document.getElementById("swType").innerHTML == "4")
            parent.frames["MiddleRight"].document.getElementById("swType").innerHTML = "Linux";

        if (parent.frames["MiddleRight"].document.getElementById("builtOn") != null) {
            parent.frames["MiddleRight"].document.getElementById("builtOn").innerHTML = DisplayTime(parent.frames["MiddleRight"].document.getElementById("builtOn").innerHTML);
        }
    }
    else
        if (tableName == "componentStatus") {
        if (parent.frames["MiddleRight"].document.getElementById("componentType").innerHTML == "1")
            parent.frames["MiddleRight"].document.getElementById("componentType").innerHTML = "Baseband";
        else
            if (parent.frames["MiddleRight"].document.getElementById("componentType").innerHTML == "2")
            parent.frames["MiddleRight"].document.getElementById("componentType").innerHTML = "Radiohead";
        else
            if (parent.frames["MiddleRight"].document.getElementById("componentType").innerHTML == "3")
            parent.frames["MiddleRight"].document.getElementById("componentType").innerHTML = "Ethernet";
        else
            if (parent.frames["MiddleRight"].document.getElementById("componentType").innerHTML == "4")
            parent.frames["MiddleRight"].document.getElementById("componentType").innerHTML = "Temperature Sensor";
        else
            if (parent.frames["MiddleRight"].document.getElementById("componentType").innerHTML == "5")
            parent.frames["MiddleRight"].document.getElementById("componentType").innerHTML = "CPRI";
        else
            if (parent.frames["MiddleRight"].document.getElementById("componentType").innerHTML == "6")
            parent.frames["MiddleRight"].document.getElementById("componentType").innerHTML = "GPS";
    }
    else
        if (tableName == "enbStatus") {

        if (parent.frames["MiddleRight"].document.getElementById("connectedToMME").innerHTML == "1")
            $(parent.frames["MiddleRight"].document.getElementById("connectedToMME")).text("Not Connected").css("color", "red");
        else if (parent.frames["MiddleRight"].document.getElementById("connectedToMME").innerHTML == "2")
            $(parent.frames["MiddleRight"].document.getElementById("connectedToMME")).text("Connected").css("color", "green");

        if (parent.frames["MiddleRight"].document.getElementById("adminState").innerHTML == "1")
            $(parent.frames["MiddleRight"].document.getElementById("adminState")).text("Locked").css("color", "red");
        else if (parent.frames["MiddleRight"].document.getElementById("adminState").innerHTML == "2")
            $(parent.frames["MiddleRight"].document.getElementById("adminState")).text("Unlocked").css("color", "green");

        if (parent.frames["MiddleRight"].document.getElementById("operationalState").innerHTML == "1")
            $(parent.frames["MiddleRight"].document.getElementById("operationalState")).text("Disabled").css("color", "red");
        else if (parent.frames["MiddleRight"].document.getElementById("operationalState").innerHTML == "2")
            $(parent.frames["MiddleRight"].document.getElementById("operationalState")).text("Enabled").css("color", "green");

        if (parent.frames["MiddleRight"].document.getElementById("dateAndTimeSource") != null) {
            if (parent.frames["MiddleRight"].document.getElementById("dateAndTimeSource").innerHTML == "1")
                $(parent.frames["MiddleRight"].document.getElementById("dateAndTimeSource")).text("Source Un-available");
            else if (parent.frames["MiddleRight"].document.getElementById("dateAndTimeSource").innerHTML == "2")
                $(parent.frames["MiddleRight"].document.getElementById("dateAndTimeSource")).text("GPS");
            else if (parent.frames["MiddleRight"].document.getElementById("dateAndTimeSource").innerHTML == "3")
                $(parent.frames["MiddleRight"].document.getElementById("dateAndTimeSource")).text("NTP Server");
            else if (parent.frames["MiddleRight"].document.getElementById("dateAndTimeSource").innerHTML == "4")
                $(parent.frames["MiddleRight"].document.getElementById("dateAndTimeSource")).text("Configured");
            else if (parent.frames["MiddleRight"].document.getElementById("dateAndTimeSource").innerHTML == "5")
                $(parent.frames["MiddleRight"].document.getElementById("dateAndTimeSource")).text("Other");
        }

        if (parent.frames["MiddleRight"].document.getElementById("dateAndTimeSource") != null) {
            if (parent.frames["MiddleRight"].document.getElementById("loadState").innerHTML == "1")
                $(parent.frames["MiddleRight"].document.getElementById("loadState")).text("Low Load").css("color", "green");
            else if (parent.frames["MiddleRight"].document.getElementById("loadState").innerHTML == "2")
                $(parent.frames["MiddleRight"].document.getElementById("loadState")).text("Normal Load").css("color", "green");
            else if (parent.frames["MiddleRight"].document.getElementById("loadState").innerHTML == "3")
                $(parent.frames["MiddleRight"].document.getElementById("loadState")).text("Overloaded").css("color", "red");
        }
        if (parent.frames["MiddleRight"].document.getElementById("uptime") != null) {
            var temp = parent.frames["MiddleRight"].document.getElementById("uptime").innerHTML;
            var display = "";

            temp = parseInt(temp);
            var hour = 0;
            var min = 0;
            var sec = 0;
            var i = 0;
            for (i = 0; temp >= 3600; i++) {
                temp = temp - 3600;
            }
            hour = i;
            for (i = 0; temp >= 60; i++) {
                temp = temp - 60;
            }
            min = i;
            sec = temp;
            if (hour < 10)
                hour = "0" + hour;
            if (min < 10)
                min = "0" + min;
            if (sec < 10)
                sec = "0" + sec;

            display = hour + ":" + min + ":" + sec;
            parent.frames["MiddleRight"].document.getElementById("uptime").innerHTML = display;

        }


        if (parent.frames["MiddleRight"].document.getElementById("localTime") != null) {
            parent.frames["MiddleRight"].document.getElementById("localTime").innerHTML = DisplayTime(parent.frames["MiddleRight"].document.getElementById("localTime").innerHTML);

        }
    }
    else
        if (tableName == "timersConfig") {
        var i = parent.frames["MiddleRight"].document.getElementById("timerName" + response).innerHTML;
        if (i == "1")
                $(parent.frames["MiddleRight"].document.getElementById("timerName" + response)).text("WaitInitComplete");
        else
            if (i == "2")
                $(parent.frames["MiddleRight"].document.getElementById("timerName" + response)).text("HealthCheckInterval");
        else
            if (i == "3")
                $(parent.frames["MiddleRight"].document.getElementById("timerName" + response)).text("WaitHelloAck");
        else
            if (i == "4")
                $(parent.frames["MiddleRight"].document.getElementById("timerName" + response)).text("WaitExitComplete");
        else
            if (i == "5")
                $(parent.frames["MiddleRight"].document.getElementById("timerName" + response)).text("WaitBootLowerArmComplete");
        else
            if (i == "6")
                $(parent.frames["MiddleRight"].document.getElementById("timerName" + response)).text("WaitRhSwInstallRsp");
        else
            if (i == "7")
                $(parent.frames["MiddleRight"].document.getElementById("timerName" + response)).text("WaitRhActivateRsp");
        else
            if (i == "8")
                $(parent.frames["MiddleRight"].document.getElementById("timerName" + response)).text("WaitDehStatusReportPeriod");
        else
            if (i == "9")
                $(parent.frames["MiddleRight"].document.getElementById("timerName" + response)).text("WaitAllDeviceRsp");
        else
            if (i == "10")
                $(parent.frames["MiddleRight"].document.getElementById("timerName" + response)).text("WaitRhInit");
        else
            if (i == "11")
                $(parent.frames["MiddleRight"].document.getElementById("timerName" + response)).text("WaitRhDisableRsp");
        else
            if (i == "12")
                $(parent.frames["MiddleRight"].document.getElementById("timerName" + response)).text("WaitRhEnableRsp");
        else
            if (i == "13")
                $(parent.frames["MiddleRight"].document.getElementById("timerName" + response)).text("WaitRhConfigRsp");
        else
            if (i == "14")
                $(parent.frames["MiddleRight"].document.getElementById("timerName" + response)).text("WaitRhStatusRspForCih");
        else
            if (i == "15")
                $(parent.frames["MiddleRight"].document.getElementById("timerName" + response)).text("WaitRhStatusRspForSih");
        else
            if (i == "16")
                $(parent.frames["MiddleRight"].document.getElementById("timerName" + response)).text("WaitRhStatusRspForWih");
        else
            if (i == "17")
                $(parent.frames["MiddleRight"].document.getElementById("timerName" + response)).text("WaitBistComplete");
        else
            if (i == "18")
                $(parent.frames["MiddleRight"].document.getElementById("timerName" + response)).text("WaitCpriLinkup");
        else
            if (i == "19")
                $(parent.frames["MiddleRight"].document.getElementById("timerName" + response)).text("WaitTraceConfirm");
        else
            if (i == "20")
                $(parent.frames["MiddleRight"].document.getElementById("timerName" + response)).text("WaitTraceDuration");
        else
            if (i == "21")
                $(parent.frames["MiddleRight"].document.getElementById("timerName" + response)).text("WaitEnableComplete");
       else
            if (i == "22")
                $(parent.frames["MiddleRight"].document.getElementById("timerName" + response)).text("WaitDisableComplete");         
        else
            if (i == "23")
            $(parent.frames["MiddleRight"].document.getElementById("timerName" + response)).text("WaitConfigComplete");
        else
            if (i == "24")
                $(parent.frames["MiddleRight"].document.getElementById("timerName" + response)).text("WaitGpsParametersRsp");
        else
            if (i == "25")
                $(parent.frames["MiddleRight"].document.getElementById("timerName" + response)).text("WaitEmsRegRsp");
        else
            if (i == "26")
                $(parent.frames["MiddleRight"].document.getElementById("timerName" + response)).text("WaitMmeLinkEstablish");
        else
            if (i == "27")
                $(parent.frames["MiddleRight"].document.getElementById("timerName" + response)).text("WaitDeviceInitComplete");
        else
            if (i == "28")
                $(parent.frames["MiddleRight"].document.getElementById("timerName" + response)).text("WaitInitIpAddrComplete");
        else
            if (i == "29")
                $(parent.frames["MiddleRight"].document.getElementById("timerName" + response)).text("WaitCmdRsp");
        else
            if (i == "30")
                $(parent.frames["MiddleRight"].document.getElementById("timerName" + response)).text("WaitCounterCollection");
        else
            if (i == "31")
                $(parent.frames["MiddleRight"].document.getElementById("timerName" + response)).text("WaitSnmpCmdRsp");
        else
            if (i == "32")
                $(parent.frames["MiddleRight"].document.getElementById("timerName" + response)).text("WaitCounterReset");
        else
            if (i == "33")
                $(parent.frames["MiddleRight"].document.getElementById("timerName" + response)).text("WaitL2PMCounterControlProfile1");
        else
            if (i == "34")
                $(parent.frames["MiddleRight"].document.getElementById("timerName" + response)).text("WaitL2PMCounterControlProfile2");
        else
            if (i == "35")
                $(parent.frames["MiddleRight"].document.getElementById("timerName" + response)).text("WaitL2PMReportProfile1");
        else
            if (i == "36")
                $(parent.frames["MiddleRight"].document.getElementById("timerName" + response)).text("WaitL2PMReportProfile2");
        
    }
    else if (tableName == "msgTrace") {
        var i = parent.frames["MiddleRight"].document.getElementById("msgTraceLevel" + response).val();
        if (i == "1")
            $(parent.frames["MiddleRight"].document.getElementById("msgTraceLevel" + response)).val("Disable");
        else
            if (i == "2")
            $(parent.frames["MiddleRight"].document.getElementById("msgTraceLevel" + response)).val("MsgIdOnly");
        else
            if (i == "3")
            $(parent.frames["MiddleRight"].document.getElementById("msgTraceLevel" + response)).val("MsgWithParams");

        i = parent.frames["MiddleRight"].document.getElementById("msgTraceDirection" + response).val();

        if (i == "1")
            $(parent.frames["MiddleRight"].document.getElementById("msgTraceDirection" + response)).val("MsgIn");
        else
            if (i == "2")
            $(parent.frames["MiddleRight"].document.getElementById("msgTraceDirection" + response)).val("MsgOut");
        else
            if (i == "3")
            $(parent.frames["MiddleRight"].document.getElementById("msgTraceDirection" + response)).val("MsgInBothDirec");
    }
    else if (tableName == "swActivationStatus") {
        var i = parent.frames["MiddleRight"].document.getElementById("swStatus").innerHTML;
        if (i == "1")
            $(parent.frames["MiddleRight"].document.getElementById("swStatus")).html("Not Initiated").css("color", "red");
        else
            if (i == "2")
            $(parent.frames["MiddleRight"].document.getElementById("swStatus")).html("In Progress").css("color", "blue");
        if (i == "3")
            $(parent.frames["MiddleRight"].document.getElementById("swStatus")).html("Successful").css("color", "green");
        else
            if (i == "4")
            $(parent.frames["MiddleRight"].document.getElementById("swStatus")).html("Failed").css("color", "red");
    }
    else if (tableName == "traceStatus") {
        if (parent.frames["MiddleRight"].document.getElementById("traceState").innerHTML == "1")
            $(parent.frames["MiddleRight"].document.getElementById("traceState")).text("Trace ON").css("color", "green");
        else if (parent.frames["MiddleRight"].document.getElementById("traceState").innerHTML == "2")
            $(parent.frames["MiddleRight"].document.getElementById("traceState")).text("Trace OFF").css("color", "red");
       
        i = parent.frames["MiddleRight"].document.getElementById("traceDestination").innerHTML;
        switch (i) {
            case "1": $(parent.frames["MiddleRight"].document.getElementById("traceDestination")).html("Collect To File");
                break;
            case "2": $(parent.frames["MiddleRight"].document.getElementById("traceDestination")).html("Relay to Remote Host");
                break;            
        }
    }
    else if (tableName == "swInstallHistory") {
        var i = parent.frames["MiddleRight"].document.getElementById("wasSuccessful").innerHTML;
        switch (i) {
            case "0": $(parent.frames["MiddleRight"].document.getElementById("wasSuccessful")).html("Not Successful").css("color", "red");
                break;
            case "1": $(parent.frames["MiddleRight"].document.getElementById("wasSuccessful")).html("Successful").css("color", "green");
                break;

        }
    }
    else
        if (tableName == "radioHeadStatus") {
        /* C1IQMap */
        if (parent.frames["MiddleRight"].document.getElementById("c1IQMap").innerHTML == "1")
            parent.frames["MiddleRight"].document.getElementById("c1IQMap").innerHTML = "0";
        else
            if (parent.frames["MiddleRight"].document.getElementById("c1IQMap").innerHTML == "2")
            parent.frames["MiddleRight"].document.getElementById("c1IQMap").innerHTML = "1";
        else
            if (parent.frames["MiddleRight"].document.getElementById("c1IQMap").innerHTML == "3")
            parent.frames["MiddleRight"].document.getElementById("c1IQMap").innerHTML = "3";
        else
            if (parent.frames["MiddleRight"].document.getElementById("c1IQMap").innerHTML == "4")
            parent.frames["MiddleRight"].document.getElementById("c1IQMap").innerHTML = "5";
        else
            if (parent.frames["MiddleRight"].document.getElementById("c1IQMap").innerHTML == "5")
            parent.frames["MiddleRight"].document.getElementById("c1IQMap").innerHTML = "10";
        else
            if (parent.frames["MiddleRight"].document.getElementById("c1IQMap").innerHTML == "6")
            parent.frames["MiddleRight"].document.getElementById("c1IQMap").innerHTML = "15";
        else
            if (parent.frames["MiddleRight"].document.getElementById("c1IQMap").innerHTML == "7")
            parent.frames["MiddleRight"].document.getElementById("c1IQMap").innerHTML = "20";
        /* C2IQMap */
        if (parent.frames["MiddleRight"].document.getElementById("c2IQMap").innerHTML == "1")
            parent.frames["MiddleRight"].document.getElementById("c2IQMap").innerHTML = "0";
        else
            if (parent.frames["MiddleRight"].document.getElementById("c2IQMap").innerHTML == "2")
            parent.frames["MiddleRight"].document.getElementById("c2IQMap").innerHTML = "1";
        else
            if (parent.frames["MiddleRight"].document.getElementById("c2IQMap").innerHTML == "3")
            parent.frames["MiddleRight"].document.getElementById("c2IQMap").innerHTML = "3";
        else
            if (parent.frames["MiddleRight"].document.getElementById("c2IQMap").innerHTML == "4")
            parent.frames["MiddleRight"].document.getElementById("c2IQMap").innerHTML = "5";
        else
            if (parent.frames["MiddleRight"].document.getElementById("c2IQMap").innerHTML == "5")
            parent.frames["MiddleRight"].document.getElementById("c2IQMap").innerHTML = "10";
        else
            if (parent.frames["MiddleRight"].document.getElementById("c2IQMap").innerHTML == "6")
            parent.frames["MiddleRight"].document.getElementById("c2IQMap").innerHTML = "15";
        else
            if (parent.frames["MiddleRight"].document.getElementById("c2IQMap").innerHTML == "7")
            parent.frames["MiddleRight"].document.getElementById("c2IQMap").innerHTML = "20";
        /* C3IQMap */
        if (parent.frames["MiddleRight"].document.getElementById("c3IQMap").innerHTML == "1")
            parent.frames["MiddleRight"].document.getElementById("c3IQMap").innerHTML = "0";
        else
            if (parent.frames["MiddleRight"].document.getElementById("c3IQMap").innerHTML == "2")
            parent.frames["MiddleRight"].document.getElementById("c3IQMap").innerHTML = "1";
        else
            if (parent.frames["MiddleRight"].document.getElementById("c3IQMap").innerHTML == "3")
            parent.frames["MiddleRight"].document.getElementById("c3IQMap").innerHTML = "3";
        else
            if (parent.frames["MiddleRight"].document.getElementById("c3IQMap").innerHTML == "4")
            parent.frames["MiddleRight"].document.getElementById("c3IQMap").innerHTML = "5";
        else
            if (parent.frames["MiddleRight"].document.getElementById("c3IQMap").innerHTML == "5")
            parent.frames["MiddleRight"].document.getElementById("c3IQMap").innerHTML = "10";
        else
            if (parent.frames["MiddleRight"].document.getElementById("c3IQMap").innerHTML == "6")
            parent.frames["MiddleRight"].document.getElementById("c3IQMap").innerHTML = "15";
        else
            if (parent.frames["MiddleRight"].document.getElementById("c3IQMap").innerHTML == "7")
            parent.frames["MiddleRight"].document.getElementById("c3IQMap").innerHTML = "20";
        /* C4IQMap */
        if (parent.frames["MiddleRight"].document.getElementById("c4IQMap").innerHTML == "1")
            parent.frames["MiddleRight"].document.getElementById("c4IQMap").innerHTML = "0";
        else
            if (parent.frames["MiddleRight"].document.getElementById("c4IQMap").innerHTML == "2")
            parent.frames["MiddleRight"].document.getElementById("c4IQMap").innerHTML = "1";
        else
            if (parent.frames["MiddleRight"].document.getElementById("c4IQMap").innerHTML == "3")
            parent.frames["MiddleRight"].document.getElementById("c4IQMap").innerHTML = "3";
        else
            if (parent.frames["MiddleRight"].document.getElementById("c4IQMap").innerHTML == "4")
            parent.frames["MiddleRight"].document.getElementById("c4IQMap").innerHTML = "5";
        else
            if (parent.frames["MiddleRight"].document.getElementById("c4IQMap").innerHTML == "5")
            parent.frames["MiddleRight"].document.getElementById("c4IQMap").innerHTML = "10";
        else
            if (parent.frames["MiddleRight"].document.getElementById("c4IQMap").innerHTML == "6")
            parent.frames["MiddleRight"].document.getElementById("c4IQMap").innerHTML = "15";
        else
            if (parent.frames["MiddleRight"].document.getElementById("c4IQMap").innerHTML == "7")
            parent.frames["MiddleRight"].document.getElementById("c4IQMap").innerHTML = "20";

        /*MappingOf1FAStatus*/
        if (parent.frames["MiddleRight"].document.getElementById("mappingOf1FAStatus").innerHTML == "0")
            parent.frames["MiddleRight"].document.getElementById("mappingOf1FAStatus").innerHTML = "0";
        else
            if (parent.frames["MiddleRight"].document.getElementById("mappingOf1FAStatus").innerHTML == "1")
            parent.frames["MiddleRight"].document.getElementById("mappingOf1FAStatus").innerHTML = "C1";
        else
            if (parent.frames["MiddleRight"].document.getElementById("mappingOf1FAStatus").innerHTML == "2")
            parent.frames["MiddleRight"].document.getElementById("mappingOf1FAStatus").innerHTML = "C2";
        else
            if (parent.frames["MiddleRight"].document.getElementById("mappingOf1FAStatus").innerHTML == "3")
            parent.frames["MiddleRight"].document.getElementById("mappingOf1FAStatus").innerHTML = "C3";
        else
            if (parent.frames["MiddleRight"].document.getElementById("mappingOf1FAStatus").innerHTML == "4")
            parent.frames["MiddleRight"].document.getElementById("mappingOf1FAStatus").innerHTML = "C4";

        /*MappingOf2FAStatus*/
        if (parent.frames["MiddleRight"].document.getElementById("mappingOf2FAStatus").innerHTML == "0")
            parent.frames["MiddleRight"].document.getElementById("mappingOf2FAStatus").innerHTML = "0";
        else
            if (parent.frames["MiddleRight"].document.getElementById("mappingOf2FAStatus").innerHTML == "1")
            parent.frames["MiddleRight"].document.getElementById("mappingOf2FAStatus").innerHTML = "C1";
        else
            if (parent.frames["MiddleRight"].document.getElementById("mappingOf2FAStatus").innerHTML == "2")
            parent.frames["MiddleRight"].document.getElementById("mappingOf2FAStatus").innerHTML = "C2";
        else
            if (parent.frames["MiddleRight"].document.getElementById("mappingOf2FAStatus").innerHTML == "3")
            parent.frames["MiddleRight"].document.getElementById("mappingOf2FAStatus").innerHTML = "C3";
        else
            if (parent.frames["MiddleRight"].document.getElementById("mappingOf2FAStatus").innerHTML == "4")
            parent.frames["MiddleRight"].document.getElementById("mappingOf2FAStatus").innerHTML = "C4";
        /*MappingOf3FAStatus*/
        if (parent.frames["MiddleRight"].document.getElementById("mappingOf3FAStatus").innerHTML == "0")
            parent.frames["MiddleRight"].document.getElementById("mappingOf3FAStatus").innerHTML = "0";
        else
            if (parent.frames["MiddleRight"].document.getElementById("mappingOf3FAStatus").innerHTML == "1")
            parent.frames["MiddleRight"].document.getElementById("mappingOf3FAStatus").innerHTML = "C1";
        else
            if (parent.frames["MiddleRight"].document.getElementById("mappingOf3FAStatus").innerHTML == "2")
            parent.frames["MiddleRight"].document.getElementById("mappingOf3FAStatus").innerHTML = "C2";
        else
            if (parent.frames["MiddleRight"].document.getElementById("mappingOf3FAStatus").innerHTML == "3")
            parent.frames["MiddleRight"].document.getElementById("mappingOf3FAStatus").innerHTML = "C3";
        else
            if (parent.frames["MiddleRight"].document.getElementById("mappingOf3FAStatus").innerHTML == "4")
            parent.frames["MiddleRight"].document.getElementById("mappingOf3FAStatus").innerHTML = "C4";
        /*MappingOf4FAStatus*/
        if (parent.frames["MiddleRight"].document.getElementById("mappingOf4FAStatus").innerHTML == "0")
            parent.frames["MiddleRight"].document.getElementById("mappingOf4FAStatus").innerHTML = "0";
        else
            if (parent.frames["MiddleRight"].document.getElementById("mappingOf4FAStatus").innerHTML == "1")
            parent.frames["MiddleRight"].document.getElementById("mappingOf4FAStatus").innerHTML = "C1";
        else
            if (parent.frames["MiddleRight"].document.getElementById("mappingOf4FAStatus").innerHTML == "2")
            parent.frames["MiddleRight"].document.getElementById("mappingOf4FAStatus").innerHTML = "C2";
        else
            if (parent.frames["MiddleRight"].document.getElementById("mappingOf4FAStatus").innerHTML == "3")
            parent.frames["MiddleRight"].document.getElementById("mappingOf4FAStatus").innerHTML = "C3";
        else
            if (parent.frames["MiddleRight"].document.getElementById("mappingOf4FAStatus").innerHTML == "4")
            parent.frames["MiddleRight"].document.getElementById("mappingOf4FAStatus").innerHTML = "C4";

        /* fa1ChannelBandwidthStatus */
        if (parent.frames["MiddleRight"].document.getElementById("fa1ChannelBandwidthStatus").innerHTML == "1")
            parent.frames["MiddleRight"].document.getElementById("fa1ChannelBandwidthStatus").innerHTML = "0";
        else
            if (parent.frames["MiddleRight"].document.getElementById("fa1ChannelBandwidthStatus").innerHTML == "2")
            parent.frames["MiddleRight"].document.getElementById("fa1ChannelBandwidthStatus").innerHTML = "1";
        else
            if (parent.frames["MiddleRight"].document.getElementById("fa1ChannelBandwidthStatus").innerHTML == "3")
            parent.frames["MiddleRight"].document.getElementById("fa1ChannelBandwidthStatus").innerHTML = "3";
        else
            if (parent.frames["MiddleRight"].document.getElementById("fa1ChannelBandwidthStatus").innerHTML == "4")
            parent.frames["MiddleRight"].document.getElementById("fa1ChannelBandwidthStatus").innerHTML = "5";
        else
            if (parent.frames["MiddleRight"].document.getElementById("fa1ChannelBandwidthStatus").innerHTML == "5")
            parent.frames["MiddleRight"].document.getElementById("fa1ChannelBandwidthStatus").innerHTML = "10";
        else
            if (parent.frames["MiddleRight"].document.getElementById("fa1ChannelBandwidthStatus").innerHTML == "6")
            parent.frames["MiddleRight"].document.getElementById("fa1ChannelBandwidthStatus").innerHTML = "15";
        else
            if (parent.frames["MiddleRight"].document.getElementById("fa1ChannelBandwidthStatus").innerHTML == "7")
            parent.frames["MiddleRight"].document.getElementById("fa1ChannelBandwidthStatus").innerHTML = "20";
        /* fa2ChannelBandwidthStatus */
        if (parent.frames["MiddleRight"].document.getElementById("fa2ChannelBandwidthStatus").innerHTML == "1")
            parent.frames["MiddleRight"].document.getElementById("fa2ChannelBandwidthStatus").innerHTML = "0";
        else
            if (parent.frames["MiddleRight"].document.getElementById("fa2ChannelBandwidthStatus").innerHTML == "2")
            parent.frames["MiddleRight"].document.getElementById("fa2ChannelBandwidthStatus").innerHTML = "1";
        else
            if (parent.frames["MiddleRight"].document.getElementById("fa2ChannelBandwidthStatus").innerHTML == "3")
            parent.frames["MiddleRight"].document.getElementById("fa2ChannelBandwidthStatus").innerHTML = "3";
        else
            if (parent.frames["MiddleRight"].document.getElementById("fa2ChannelBandwidthStatus").innerHTML == "4")
            parent.frames["MiddleRight"].document.getElementById("fa2ChannelBandwidthStatus").innerHTML = "5";
        else
            if (parent.frames["MiddleRight"].document.getElementById("fa2ChannelBandwidthStatus").innerHTML == "5")
            parent.frames["MiddleRight"].document.getElementById("fa2ChannelBandwidthStatus").innerHTML = "10";
        else
            if (parent.frames["MiddleRight"].document.getElementById("fa2ChannelBandwidthStatus").innerHTML == "6")
            parent.frames["MiddleRight"].document.getElementById("fa2ChannelBandwidthStatus").innerHTML = "15";
        else
            if (parent.frames["MiddleRight"].document.getElementById("fa2ChannelBandwidthStatus").innerHTML == "7")
            parent.frames["MiddleRight"].document.getElementById("fa2ChannelBandwidthStatus").innerHTML = "20";
        /* fa3ChannelBandwidthStatus */
        if (parent.frames["MiddleRight"].document.getElementById("fa3ChannelBandwidthStatus").innerHTML == "1")
            parent.frames["MiddleRight"].document.getElementById("fa3ChannelBandwidthStatus").innerHTML = "0";
        else
            if (parent.frames["MiddleRight"].document.getElementById("fa3ChannelBandwidthStatus").innerHTML == "2")
            parent.frames["MiddleRight"].document.getElementById("fa3ChannelBandwidthStatus").innerHTML = "1";
        else
            if (parent.frames["MiddleRight"].document.getElementById("fa3ChannelBandwidthStatus").innerHTML == "3")
            parent.frames["MiddleRight"].document.getElementById("fa3ChannelBandwidthStatus").innerHTML = "3";
        else
            if (parent.frames["MiddleRight"].document.getElementById("fa3ChannelBandwidthStatus").innerHTML == "4")
            parent.frames["MiddleRight"].document.getElementById("fa3ChannelBandwidthStatus").innerHTML = "5";
        else
            if (parent.frames["MiddleRight"].document.getElementById("fa3ChannelBandwidthStatus").innerHTML == "5")
            parent.frames["MiddleRight"].document.getElementById("fa3ChannelBandwidthStatus").innerHTML = "10";
        else
            if (parent.frames["MiddleRight"].document.getElementById("fa3ChannelBandwidthStatus").innerHTML == "6")
            parent.frames["MiddleRight"].document.getElementById("fa3ChannelBandwidthStatus").innerHTML = "15";
        else
            if (parent.frames["MiddleRight"].document.getElementById("fa3ChannelBandwidthStatus").innerHTML == "7")
            parent.frames["MiddleRight"].document.getElementById("fa3ChannelBandwidthStatus").innerHTML = "20";
        /* fa4ChannelBandwidthStatus */
        if (parent.frames["MiddleRight"].document.getElementById("fa4ChannelBandwidthStatus").innerHTML == "1")
            parent.frames["MiddleRight"].document.getElementById("fa4ChannelBandwidthStatus").innerHTML = "0";
        else
            if (parent.frames["MiddleRight"].document.getElementById("fa4ChannelBandwidthStatus").innerHTML == "2")
            parent.frames["MiddleRight"].document.getElementById("fa4ChannelBandwidthStatus").innerHTML = "1";
        else
            if (parent.frames["MiddleRight"].document.getElementById("fa4ChannelBandwidthStatus").innerHTML == "3")
            parent.frames["MiddleRight"].document.getElementById("fa4ChannelBandwidthStatus").innerHTML = "3";
        else
            if (parent.frames["MiddleRight"].document.getElementById("fa4ChannelBandwidthStatus").innerHTML == "4")
            parent.frames["MiddleRight"].document.getElementById("fa4ChannelBandwidthStatus").innerHTML = "5";
        else
            if (parent.frames["MiddleRight"].document.getElementById("fa4ChannelBandwidthStatus").innerHTML == "5")
            parent.frames["MiddleRight"].document.getElementById("fa4ChannelBandwidthStatus").innerHTML = "10";
        else
            if (parent.frames["MiddleRight"].document.getElementById("fa4ChannelBandwidthStatus").innerHTML == "6")
            parent.frames["MiddleRight"].document.getElementById("fa4ChannelBandwidthStatus").innerHTML = "15";
        else
            if (parent.frames["MiddleRight"].document.getElementById("fa4ChannelBandwidthStatus").innerHTML == "7")
            parent.frames["MiddleRight"].document.getElementById("fa4ChannelBandwidthStatus").innerHTML = "20";
    }    
    else if (tableName == "locationStatus") {
        var i = parent.frames["MiddleRight"].document.getElementById("locationSource").innerHTML;
        switch (i) {
            case "1": $(parent.frames["MiddleRight"].document.getElementById("locationSource")).html("Configured By OAM");
                break;
            case "2": $(parent.frames["MiddleRight"].document.getElementById("locationSource")).html("GPS");
                break;
        }
        if (parent.frames["MiddleRight"].document.getElementById("latitude").innerHTML != null)
            parent.frames["MiddleRight"].document.getElementById("latitude").innerHTML = DisplayGps('latitude', parent.frames["MiddleRight"].document.getElementById("latitude").innerHTML);
        if (parent.frames["MiddleRight"].document.getElementById("longitude").innerHTML != null)
            parent.frames["MiddleRight"].document.getElementById("longitude").innerHTML = DisplayGps('longitude', parent.frames["MiddleRight"].document.getElementById("longitude").innerHTML);
    }
    else if (tableName == "activeRabStatus") {
        if (parent.frames["MiddleRight"].document.getElementById("rabSetupTime") != null) {
            parent.frames["MiddleRight"].document.getElementById("rabSetupTime").innerHTML = DisplayTimeUptoMilliSecond(parent.frames["MiddleRight"].document.getElementById("rabSetupTime").innerHTML);
        }
    }
    else if (tableName == "macGenCounters" || tableName == "macUplaneCounters" || tableName == "rlcGenCounters" || tableName == "pdcpGenCounters" || tableName == "pdcpUplaneCounters" || tableName == "gtpGenCounters" || tableName == "sctpGenCounters" || tableName == "rrcGenCounters" || tableName == "rrmCounters" || tableName == "rrcCellCounters" || tableName == "s1apGenCounters" || tableName == "s1apPeerNodeCounters") {
        if (parent.frames["MiddleRight"].document.getElementById("lastCollectedAt") != null) {
            parent.frames["MiddleRight"].document.getElementById("lastCollectedAt").innerHTML = DisplayTime(parent.frames["MiddleRight"].document.getElementById("lastCollectedAt").innerHTML);
        }
    }
    else if (tableName == "gpsStatus") {
        if (parent.frames["MiddleRight"].document.getElementById("gpsDateAndTime").innerHTML != null)
            parent.frames["MiddleRight"].document.getElementById("gpsDateAndTime").innerHTML = DisplayTime(parent.frames["MiddleRight"].document.getElementById("gpsDateAndTime").innerHTML);
        if (parent.frames["MiddleRight"].document.getElementById("gpsLatitude").innerHTML != null)
            parent.frames["MiddleRight"].document.getElementById("gpsLatitude").innerHTML = DisplayGps('latitude', parent.frames["MiddleRight"].document.getElementById("gpsLatitude").innerHTML);
        if (parent.frames["MiddleRight"].document.getElementById("gpsLongitude").innerHTML != null)
            parent.frames["MiddleRight"].document.getElementById("gpsLongitude").innerHTML = DisplayGps('longitude', parent.frames["MiddleRight"].document.getElementById("gpsLongitude").innerHTML);   
    }
}


function SetAlarmMapping(data) {
          switch (data) {
            case "1": return "MME_LINK_DOWN";
                break;
            case "2": return "ALL_MME_LINK_DOWN";
                break;           
            case "21": return "REQ_TIMED_OUT";
                break;
            case "22": return "UNKNOWN_DEST_ADDRESS";
                break;
            case "23": return "NO_ROUTE_TO_TARGET";
                break;
            case "24": return "INTERFACE_INACTIVE";
                break;
            case "25": return "ARP_FAILURE";
                break;
            case "26": return "DNS_RESOLUTION_FAIL";
                break;
            case "27": return "SCTP_ASSOCIATION_FAIL";
                break;
            case "41": return "RH_CLOCK_FAIL";
                break;
            case "42": return "RH_HIGH_TEMPERATURE";
                break;
            case "43": return "PRH_LOW_TEMPERATURE";
                break;
            case "44": return "RH_HIGH_VOLTAGE";
                break;
            case "45": return "RH_LOW_VOLTAGE";
                break;
            case "46": return "RH_INTERNAL_ERROR";
                break;
            case "47": return "RH_OVERPOWER";
                break;
            case "48": return "RH_VSWR_FAIL";
                break;
            case "49": return "RH_LOW_POWER";
                break;
            case "50": return "RH_LOW_GAIN";
                break;
            case "51": return "RH_DISABLED";
                break;        
            case "52": return "RH_SHUTDOWN";
                break;
            case "53": return "RH_INA_FAILED";
                break;    
            case "54": return "RH_PATH_ERROR";
                break;
            case "61": return "BB_CLOCK_FAIL";
                break;
            case "62": return "BB_OVERPOWER";
                break;
            case "63": return "BB_HIGH_TEMPERATURE";
                break;
            case "64": return "CPRI_DOWN";
                break;
            case "65": return "GPS_RESPONSE_FAIL";
                break;
            case "66": return "ETHERNET_DOWN";
                break;            
            case "81": return "SW_CRASH";
                break;
            case "82": return "HEALTH_CHECK_FAIL";
                break;
            case "83": return "QUEUE_SIZE_EXCEEDED";
                break;
            case "84": return "CONFIG_CUSTOM_ERROR";
                break;
            case "85": return "S1AP_GUARD_TIME_EXPIRE";
                break;
            case "86": return "S1AP_UNKNOWN_PLMNID";
                break;
            case "101": return "MAX_LIMIT_REACHED";
                break;
            case "102": return "CPU_OVERLOAD";
                break;
            case "103": return "MEMORY_OVERLOAD";
                break;    
        }
    }

function SetMultiRowMapping(tableName, param, value) {
    if (tableName == "ledStatus") {
        if (param == "ledId") {
            switch (value) {
                case "1": return "LED_POWER";
                    break;
                case "2": return "LED_GPS";
                    break;
                case "3": return "LED_EMS_REGISTRATION";
                    break;
                case "4": return "LED_ENB_OPERATIONAL";
                    break;
            }
        }
    }
    if (tableName == "moduleStatus") {
        if (param == "isRunning") {
            if (value == "1") {
                return "<font color=\"green\">YES</font>";
            }
            else if (value == "0") {
                return "<font color=\"red\">NO</font>";
            }
        }
        else if (param == "restartReason") {
            switch (value) {
                case "1": return "<font color=\"red\">OPERATOR_COMMAND</font>";
                    break;
                case "2": return "<font color=\"red\">CRASH</font>";
                    break;
                case "3": return "<font color=\"red\">HEALTHCHK_FAIL</font>";
                    break;
                case "4": return "<font color=\"black\">-</font>";
                    break;
            }
        }
    }
    else if (tableName == "fileUploadStatus") {
        if (param == "fileUploadCommandReceived") {
            switch (value) {
                case "1": return "None";
                    break;
                case "2": return "Upload";
                    break;
                case "3": return "Upload Cancel";
                    break;
            }
        }
        else if (param == "uploadStatus") {
            switch (value) {
                case "1": return "<font color=\"blue\">Not Initiated</font>";
                    break;
                case "2": return "<font color=\"orange\">Ongoing</font>";
                    break;
                case "3": return "<font color=\"green\">Successful</font>";
                    break;
                case "4": return "<font color=\"red\">Failed</font>";
                    break;
                case "5": return "<font color=\"blue\">Cancelled</font>";
                    break;
            }
        }
    }
    else if (tableName == "fileDownloadStatus") {
        if (param == "fileDownloadCommandReceived") {
            switch (value) {
                case "1": return "Instant Download";
                    break;
                case "2": return "Scheduled Download";
                    break;
                case "3": return "Modified Download Schedule" ;
                    break;
				case "4": return "Download Cancelled";
                    break;
                case "5": return "none";
                    break;
            }
        }
        else if (param == "downloadStatus") {
            switch (value) {
                case "1": return "<font color=\"blue\">Not Initiated</font>";
                    break;
                case "2": return "<font color=\"orange\">Ongoing</font>";
                    break;
                case "3": return "<font color=\"green\">Successful</font>";
                    break;
                case "4": return "<font color=\"red\">Failed</font>";
                    break;
                case "5": return "<font color=\"blue\">Cancelled</font>";
                    break;
            }
        }
    }
    else if (tableName == "componentStatus") {
        if (param == "componentType") {
            switch (value) {
                case "1": return "Baseband"
                    break;
                case "2": return "Radiohead";
                    break;
                case "3": return "Ethernet";
                    break;
                case "4": return "Temperature Sensor";
                    break;
                case "5": return "CPRI";
                    break;
                case "6": return "GPS";
                    break;
                case "7": return "CLOCK";
                    break;    
            }
        }
        if (param == "componentOperationalState") {
            switch (value) {
                case "1": return "<font color=\"green\">Up</font>";
                    break;
                case "2": return "<font color=\"red\">Down</font>";
                    break;
                case "3": return "<font color=\"orange\">Unknown</font>";
                    break;    
                case "4": return "<font color=\"blue\">NotMonitored</font>";
                    break;    
            }
        }
    }
    else if (tableName == "swPackageStatus") {
        if (param == "swType") {
            switch (value) {
                case "1": return "ENB L2L3 Software";
                    break;                
                case "2": return "RH Software";
                    break;                
            }
        }
        else if (param == "isRunning") {
            switch (value) {
                case "1": return "<font color=\"green\">YES</font>";
                    break;
                case "0": return "<font color=\"red\">NO</font>";
                    break;
            }
        }
    }
    else if (tableName == "fileEventHistory") {
        if (param == "fileStatus") {
            switch (value) {
                case "1": return "Not Applicable";
                    break;
                case "2": return "File Opened";
                    break;
                case "3": return "File Closed";
                    break;
                case "4": return "File Deleted";
                    break;
                case "5": return "File Download";
                    break;
                case "6": return "File Upload";
                    break;
                case "7": return "File Upload and Delete";
                    break;
                case "8": return "File Auto Upload and Delete";
                    break;
                case "9": return "File Saved To Persistent Memory";
                    break;
            }
        }
        else if (param == "wasSuccessful") {
            switch (value) {
                case "1": return "<font color=\"green\">YES</font>";
                    break;
                case "0": return "<font color=\"red\">NO</font>";
                    break;
            }
        }
    }
    else if (tableName == "activeAlarmStatus") {
        if (param == "alarmType") {
            switch (value) {
                case "1": return "Other";
                    break;
                case "2": return "Communications";
                    break;
                case "3": return "Quality of Service";
                    break;
                case "4": return "Processing Error";
                    break;               
                case "5": return "Equipment";
                    break;
                case "6": return "Environmental";
                    break;
                case "7": return "Integrity Violation";
                    break;
                case "8": return "Operation Violation";
                    break;
                case "9": return "Physical Violation";
                    break;    
                case "10": return "Security Service/Mechanism";
                    break;                
                case "11": return "Time Domain Violation";
                    break;
            }
        }
        else if (param == "alarmSeverity") {
            switch (value) {
                case "1": return "<font color=\"green\">Cleared</font>";
                    break;
                case "2": return "<font color=\"blue\">Indeterminate</font>";
                    break;
                case "3": return "<font color=\"red\">Critical</font>";
                    break;
                case "4": return "<font color=\"orange\">Major</font>";
                    break;
                case "5": return "<font color=\"yellow\">Minor</font>";
                    break;
                case "6": return "<font color=\"navy\">Warning</font>";
                    break;
            }
        }
    }
    else if (tableName == "alarmHistory") {
        if (param == "alarmType") {
            switch (value) {
                case "1": return "Other";
                    break;
                case "2": return "Communications";
                    break;
                case "3": return "Quality of Service";
                    break;
                case "4": return "Processing Error";
                    break;
                case "5": return "Equipment";
                    break;
                case "6": return "Environmental";
                    break;
                case "7": return "Integrity Violation";
                    break;
                case "8": return "Operation Violation";
                    break;
                case "9": return "Physical Violation";
                    break;
                case "10": return "Security Service/Mechanism";
                    break;
                case "11": return "Time Domain Violation";
                    break;
            }
        }
        else if (param == "alarmSeverity") {
            switch (value) {
                case "1": return "<font color=\"green\">Cleared</font>";
                    break;
                case "2": return "<font color=\"blue\">Indeterminate</font>";
                    break;
                case "3": return "<font color=\"red\">Critical</font>";
                    break;
                case "4": return "<font color=\"orange\">Major</font>";
                    break;
                case "5": return "<font color=\"yellow\">Minor</font>";
                    break;
                case "6": return "<font color=\"navy\">Warning</font>";
                    break;
            }
        }
        else if (param == "alarmNotificationType") {
            switch (value) {
                case "1": return "Occured";
                    break;
                case "2": return "Cleared";
                    break;
            }
        }
    }
    else if (tableName == "swInstallHistory") {
        if (param == "wasSuccessful") {
            if (value == "0")
                return "<font color=\"red\">FALSE</font>";
            else if (value == "1")
                return "<font color=\"green\">TRUE</font>";
        }
    }
    else if (tableName == "activeUeStatus") {
        if (param == "ueCategory") {
            switch (value) {
                case "1": return "CAT1";
                    break;
                case "2": return "CAT2";
                    break;
                case "3": return "CAT3";
                    break;
                case "4": return "CAT4";
                    break;
                case "5": return "CAT5";
                    break;
                case "6": return "<font color=\"red\">Unknown</font>";
                    break;
            }
        }
        else if (param == "ueAccessStratumRelease") {
            switch (value) {
                case "0": return "Rel8";
                    break;
                case "1": return "spare7";
                    break;
                case "2": return "spare6";
                    break;
                case "3": return "spare5";
                    break;
                case "4": return "spare4";
                    break;
                case "5": return "spare3";
                    break;
                case "6": return "spare2";
                    break;
                case "7": return "spare1";
                    break;                
            }
        }
        else if (param == "activeIntProtAlgo") {
            switch (value) {
                case "0": return "128EIA1";
                    break;
                case "1": return "128EIA2";
                    break;
                case "2": return "<font color=\"red\">Null</font>";
                    break;
                case "3": return "<font color=\"red\">Inactive</font>";
                    break;
            }
        }
        else if (param == "activeCiphAlgo") {
            switch (value) {
                case "0": return "EEA0"
                    break;
                case "1": return "128EEA1";
                    break;
                case "2": return "128EEA2";
                    break;
                case "3": return "<font color=\"red\">Null</font>";
                    break;
                case "4": return "<font color=\"red\">Inactive</font>";
                    break;
            }
        }
    }
    else if (tableName == "ueEventHistory") {
        if (param == "eventType") {
          switch (value) {
            case "0": return "Unknown";
                    break;
            case "1": return "RRC Connection Request";
                    break;
            case "2": return "RRC Connection Setup";
                    break;
            case "3": return "RRC Connection Reject";
                    break;
            case "4": return "RRC Connection Setup Complete";
                    break;
            case "5": return "Initial UE Message";
                    break;
            case "6": return "UE Capability Enquiry";
                    break;    
            case "7": return "UE Capability Information";
                    break;
            case "8": return "UE Capability Information Indication";
                    break;                                  
            case "9": return "RRC Connection Re-Configuration";
                    break;
            case "10": return "RRC Connection Re-Configuration Complete";
                    break;
            case "11": return "Security Mode Command";
                    break;
            case "12": return "Security Mode Complete";
                    break;
            case "13": return "Security Mode Failure";
                    break;
            case "14": return "Initial Context Setup Request";
                    break;
            case "15": return "Initial Context Setup Response";
                    break;
            case "16": return "Initial Context Setup Failure";
                    break;
            case "17": return "RRC Connection Release";
                    break;
            case "18": return "RRC Re-Establishment Request";
                    break;
            case "19": return "RRC Re-Establishment Response";
                    break;
            case "20": return "RRC Re-Establishment Reject";
                    break;
            case "21": return "RRC Re-Establishment Complete";
                    break;
            case "22": return "RRC Counter Check Request";
                    break;
            case "23": return "RRC Counter Check Response";
                    break;
            case "24": return "UL Information Transfer";
                    break;
            case "25": return "UL NAS Transport Message";
                    break;
            case "26": return "DL Information Transfer";
                    break;
            case "27": return "DL NAS Transport Message";
                    break;
            case "28": return "Location Report Control";
                    break;
            case "29": return "Location Report Failure Indication";
                    break;
             case "30": return "Location Report Response";
                    break;
            case "31": return "Trace Start";
                    break;
            case "32": return "Trace Failure Indication";
                    break;
            case "33": return "Deactivate Trace";
                    break;
            case "34": return "Deactivate Trace Failure Indication";
                    break;
            case "35": return "Out Handover Triggered";
                    break;
            case "36": return "Out Handover Complete";
                    break;
            case "37": return "Out Handover Fail";
                    break;
            case "38": return "In Handover Request";
                    break;
            case "39": return "In Handover Complete";
                    break;
            case "40": return "In Handover Fail";
                    break;
            case "41": return "Handover Notify";
                    break;
            case "42": return "Path Switch Request";
                    break;
            case "43": return "Path Switch Response";
                    break;
            case "44": return "Path Switch Fail";
                    break;
            case "45": return "Handover Cancel";
                    break;
            case "46": return "Handover Cancel Ack";
                    break;
            case "47": return "RAB Setup Request";
                    break;
            case "48": return "RAB Setup Completed";
                    break;
            case "49": return "RAB Setup Fail";
                    break;
            case "50": return "RAB Modify Request";
                    break;
            case "51": return "RAB Modify Response";
                    break;
            case "52": return "RAB Modify Fail";
                    break;
            case "53": return "RAB Release Request";
                    break;
            case "54": return "RAB Release Complete";
                    break;
            case "55": return "RAB Release Indication";
                    break;
            case "56": return "UE Context Release Request";
                    break;
            case "57": return "UE Context Release Command";
                    break;
            case "58": return "UE Context Release Complete";
                    break;
            case "59": return "UE Context Modify Request";
                    break;
            case "60": return "UE Context Modify Response";
                    break;
            case "61": return "UE Context Modify Fail";
                    break;
            case "62": return "Radio Link Failure";
                    break;
            case "63": return "Radio Link Recovery";
                    break;
            case "64": return "RRC Bind Request";
                    break;
            case "65": return "RRC Cell Configuration";
                    break;
            case "66": return "SCH Bind Request";
                    break;
            case "67": return "SCH Cell Configuration";
                    break;
            case "68": return "SCH Logical Channel Configuration";
                    break;
            case "69": return "SCH Logical Channel Group Configuration";
                    break;
            case "70": return "SCH UE Configuration";
                    break;
            case "71": return "SCH UE Re-Configuration";
                    break;
            case "72": return "SCH UE Delete";
                    break;
            case "73": return "SCH Si Configuration";
                    break;
            case "74": return "PHY Band Request";
                    break;
            case "75": return "PHY UE Configuration";
                    break;
            case "76": return "PHY Cell Configuartion";
                    break;
            case "77": return "PHY UE Delete";
                    break;
            case "78": return "UDH Bind Request";
                    break;
            case "79": return "UDH Add RAB Request";
                    break;
            case "80": return "UDH Delete RAB Request";
                    break;
            case "81": return "UDH Delete UE Request";
                    break;
            case "82": return "RRC UE Capability Decode Request";
                    break;
            case "83": return "S1 Bind Request";
                    break;
            case "84": return "DL NAS Non-Delivery Request Indication";
                    break;
            case "85": return "UE Context Deleted";
                    break;
            case "86": return "Internal";
                    break;
            case "87": return "Opstop Release";
                    break;
            case "88": return "RRC LI Configuration Confirmed";
                    break;
            case "89": return "RRC Error Indication";
                    break;
            case "90": return "UE Response Timer Expired";
                    break;
            case "91": return "SRB1 Timer Expired";
                    break;
            case "92": return "RRC Connection Release L2 ACK timer Expired"; 
                    break;
            case "93": return "UE Context release Timer Expired";
                    break;
            case "94": return "UE CRNTI Retention Timer Expired"; 
                    break;
            case "95": return "RRC Encode Confirmed";   
                    break;
            case "96": return "RRC Decode Confirmed"; 
                    break;
            case "97": return "EGTP Error Indication";  
                    break;
            case "98": return "UDH ERAB Addition Confirmed";
                    break;
            case "99": return "UDH UE Event Indication"; 
                    break;
            case "100": return "UDH ERAB Deletion Confirmed";  
                    break;
            case "101": return "UE CTF Configuration Confirmed"; 
                    break;
            case "102": return "UDH UL Inactivity Indication"; 
                    break;
            case "103": return "RRC Counter Confirmed";  
                    break;
            case "104": return "UE Counter Chek response";  
                    break;
            case "105": return "S1AP Handover Command"; 
                    break;
            case "106": return "S1AP Handover Preparation Failed"; 
                    break;
            case "107": return "S1AP Encodind & Deconding Confirmation"; 
                    break;
            case "108": return "MME Release Indication timer Expired"; 
                    break;
            case "109": return "UE Schedule Configuration Confirmation"; 
                    break;
            case "110": return "UE to RRC data confirmation"; 
                    break;
            case "111": return "S1 reset";
                     break;
            case "112": return "S1AP error Indication"; 
                    break;
            case "113": return "EGTP Tunnel Management Confirmation";
                     break;
            case "114": return "UDH Delete UE Confirmation";
                break;
            case "115": return "Paging";
                break;
            case "116": return "Initial Confirmation Timer Expiry";
                break;
            case "117": return "RRC Connection Establishment Success";
                break;
            case "118": return "Initial Context Setup Success";
                break;
            case "119": return "Security Success";
                break;
            case "120": return "UE Capability Success";
                break;
            case "121": return "RRC Reconfiguration Success";
                break;
            case "122": return "UE Context Release Success";
                break;
            case "123": return "UE Entered";
                break;
            case "124": return "MAC L2 Measurements";
                break;
            case "125": return "RLC L2 Measurements";
                break;
            case "126": return "PDCP L2 Measurements";
                break;
            case "127": return "S1AP Local Error Indication";
                break;
            case "128": return "RRC Local Error Indication";
                break;
            default: return "Other";
                break;
          }
        }
        else if (param == "eventTimestamp") {
            return DisplayTimeUptoMilliSecond(value);
        }
    }
    else if (tableName == "rabHistory") {
        if (param == "rabReleaseCause") {
            switch (value) {
                case "0": return "Not Applicable";
                    break;
                case "1": return "Unspecified";
                    break;
                case "2": return "UE Context Released";
                    break;                
            }        
        }
    }
    if (tableName == "activeAlarmStatus" || tableName == "alarmHistory") {
        if (param == "alarmCause") {
            switch (value) {
                case "1": return "MME Link Down";
                    break;
                case "2": return "All MME Links Down";
                    break;
                case "3": return "EMS Registration Confirmation Not Received";
                    break;
                case "21": return "Request Timed-Out";
                    break;
                case "22": return "Unknown Destination Address";
                    break;
                case "23": return "No Route To Target";
                    break;
                case "24": return "Interface Inactive To Target";
                    break;
                case "25": return "ARP Failure";
                    break;
                case "26": return "Unable To Resolve DNS Name";
                    break;
                case "27": return "SCTP Association Failed";
                    break;
                case "28": return "Set IP Address Failed";
                    break;
                case "41": return "RH Clock Failed";
                    break;
                case "42": return "RH High Temperature";
                    break;
                case "43": return "RH Low Temperature";
                    break;
                case "44": return "RH High Voltage";
                    break;
                case "45": return "RH Low Voltage";
                    break;
                case "46": return "RH Comman Internal Error";
                    break;
                case "47": return "RH Over Power";
                    break;
                case "48": return "VSWR Fail";
                    break;
                case "49": return "Low Power";
                    break;
                case "50": return "Low Gain";
                    break;
                case "51": return "RH Disable";
                    break;
                case "52": return "RH Shutdown";
                    break;
                case "53": return "lNAFail";
                    break;
                case "54": return "RH Path Internal Error";
                    break;
                case "61": return "Clock Fail";
                    break;
                case "62": return "Overpower";
                    break;
                case "63": return "High Temperature";
                    break;
                case "64": return "CPRI Link Down";
                    break;
                case "65": return "GPS Receiver Not Responding";
                    break;
                case "66": return "Back-Haul Ethernet Device Down";
                    break;
                case "67": return "Debug Ethernet Device Down";
                    break;
                case "68": return "Self-Test Failed";
                    break;
                case "81": return "SW Crash";
                    break;
                case "82": return "No Response To Health Check";
                    break;
                case "83": return "Queue Size Exceeded";
                    break;
                case "84": return "Configuration/Customisation Error";
                    break;
                case "85": return "S1AP Guard Time Expired";
                    break;
                case "86": return "S1AP:Radio Network Unspecified";
                    break;
                case "87": return "S1AP:Radio Network Tx2 Reloc overall timer expired";
                    break;
                case "88": return "S1AP:Radio Network Successful Handover";
                    break;
                case "89": return "S1AP:Radio Network Release of Eutran Generated Reason";
                    break;
                case "90": return "S1AP:Radio Network Handover Cancelled";
                    break;
                case "91": return "S1AP:Radio Network Partial Handover";
                    break;
                case "92": return "S1AP:Radio Network HO Failure in Target EPC ENB or Target System";
                    break;
                case "93": return "S1AP:Radio Network  HO Target Did Not Allow";
                    break;
                case "94": return "S1AP:Radio Network T s1 Reloc overall Expired";
                    break;
                case "95": return "S1AP:Radio Network T s1 Reloc Prep Expired";
                    break;
                case "96": return "S1AP:Radio Network Cell Not Available";
                    break;
                case "97": return "S1AP:Radio Network Unknown Target ID";
                    break;
                case "98": return "S1AP:Radio Network No Radio Resources Available At Target Cell";
                    break;
                case "99": return "S1AP:Radio Network Unknown MME UE S1AP ID";
                    break;
                case "100": return "S1AP:Radio Network Unknown ENB UE S1AP ID";
                    break;
                case "101": return "S1AP:Radio Network Unknown Pair UE S1AP ID";
                    break;
                case "102": return "S1AP:Radio Network Handover Is Desirable For Radio Reason";
                    break;
                case "103": return "S1AP:Radio Network Time Critical Handover";
                    break;
                case "104": return "S1AP:Radio Network Resource Optimisation Handover";
                    break;
                case "105": return "S1AP:Radio Network Reduce Load In Serving Cell";
                    break;
                case "106": return "S1AP:Radio Network User Inactivity";
                    break;
                case "107": return "S1AP:Radio Network Radio Connection Lost With UE.";
                    break;
                case "108": return "S1AP:Radio Network Load Balancing TAU Required";
                    break;
                case "109": return "S1AP:Radio Network CS Fallback was Triggered";
                    break;
                case "110": return "S1AP:Radio Network UE Not Available For PS Service";
                    break;
                case "111": return "S1AP:Radio Network Radio Resource Not Available. ";
                    break;
                case "112": return "S1AP:Radio Network Failure In Radio Interface Procedure";
                    break;
                case "113": return "S1AP:Radio Network Invalid Qos Combination";
                    break;
                case "114": return "S1AP:Radio Network Inter RAT Redirection";
                    break;
                case "115": return "S1AP:Radio Network Interaction With Other Procedure";
                    break;
                case "116": return "S1AP:Radio Network Unknown E-RAB ID";
                    break;
                case "117": return "S1AP:Radio Network Multiple Unknown E-RAB ID Instances";
                    break;
                case "118": return "S1AP:Radio Network Encryption and/or Integrity Protection Alog Not Supported";
                    break;
                case "119": return "S1AP:Radio Network S1 Intra System Handover Triggered";
                    break;
                case "120": return "S1AP:Radio Network S1 Inter System Handover Triggered";
                    break;
                case "121": return "S1AP:Radio Network X2 Handover Triggered";
                    break;
                case "122": return "S1AP:Radio Network Redirection Towards 1xrtt";
                    break;
                case "123": return "S1AP:Radio Network QCI Value Not Supported";
                    break;
                case "124": return "S1AP:Radio Network Invalid CSG ID";
                    break;
                case "125": return "S1AP:NAS Normal Release";
                    break;
                case "126": return "S1AP:NAS Authentication Failure";
                    break;
                case "127": return "S1AP:NAS Detach";
                    break;
                case "128": return "S1AP:NAS Value Not Specified";
                    break;
                case "129": return "S1AP:NAS CSG Subscription Expiry";
                    break;
                case "130": return "S1AP:Transport Resource Unavailable";
                    break;
                case "131": return "S1AP:Transport Value Not Specified";
                    break;
                case "132": return "S1AP:Protocol Transfer Syntax Error";
                    break;
                case "133": return "S1AP:Protocol Abstract Syntax Error Reject";
                    break;
                case "134": return "S1AP:Protocol Abstract Syntax Error Ignore And Notify";
                    break;
                case "135": return "S1AP:Protocol Message Not Compatible With Receiver State";
                    break;
                case "136": return "S1AP:Protocol Semantic Error";
                    break;
                case "137": return "S1AP:Protocol Abstract Syntax Error Falsely Constructed Message";
                    break;
                case "138": return "S1AP:Protocol Value Not Specified";
                    break;
                case "139": return "S1AP:Miscellaneous Control Processing Overload";
                    break;
                case "140": return "S1AP:Miscellaneous Resources Not Enough For User Plane Processing";
                    break;
                case "141": return "S1AP:Miscellaneous Hardware Failure";
                    break;
                case "142": return "S1AP:Miscellaneous OM Intervention";
                    break;
                case "143": return "S1AP:Miscellaneous Value Not Specifie";
                    break;
                case "144": return "S1AP:Miscellaneous Unknown PLMN ID";
                    break;
                case "201": return "Max Concurrent Limit Reached";
                    break;
                case "202": return "Overload";
                    break;
                case "203": return "Memory Overload";
                    break;
                case "500": return "Other";
                    break;
            }
        }
    }
    else if (tableName == "logConfig") {
        if (param == "moduleId") {
            switch (value) {
                case "MODID_CIH": return "1";
                    break;
                case "MODID_SIH": return "2";
                    break;
                case "MODID_WIH": return "3";
                    break;
                case "MODID_BSM": return "4";
                    break;
                case "MODID_FTH": return "5";
                    break;
                case "MODID_UEH": return "6";
                    break;
                case "MODID_PRM": return "7";
                    break;
                case "MODID_DEH": return "8";
                    break;
                case "MODID_LTH": return "9";
                    break;
                case "MODID_PMH": return "10";
                    break;
                case "MODID_EGTP": return "11";
                    break;
                case "MODID_TUCL": return "12";
                    break;
                case "MODID_PDCP": return "13";
                    break;
                case "MODID_RLC": return "14";
                    break;
                case "MODID_MAC": return "15";
                    break;
                case "MODID_RRC": return "16";
                    break;
                case "MODID_SCTP": return "17";
                    break;
                case "MODID_S1AP": return "18";
                    break;
                case "MODID_UDH": return "19";
                    break;
            }
        }
    }
    else if (tableName == "counterCollectionStatus") {
        if (param == "profileId") {
            switch (value) {
                case "1": return "PROFILE1";
                    break;
                case "2": return "PROFILE2";
                    break;
            }
        }
        else if (param == "isRunning") {
            switch (value) {
                case "1": return "TRUE";
                    break;
                case "0": return "FALSE";
                    break;
            }
        }
        else if (param == "measurementDuration") {
            switch (value) {
                case "1": return "2mins";
                    break;
                case "2": return "15mins";
                    break;
                case "3": return "30mins";
                    break;
                case "4": return "1hr";
                    break;
                case "5": return "2hr";
                    break;
                case "6": return "Infinite";
                    break;
            }
        }
        else if (param == "reportingDuration") {
            switch (value) {
                case "1": return "2mins";
                    break;
                case "2": return "15mins";
                    break;
                case "3": return "30mins";
                    break;
                case "4": return "1hr";
                    break;
                case "5": return "2hr";
                    break;
            }
        }
    }
    else if (tableName == "counterConfig") {
        if (param == "profileId") {
            switch (value) {
                case "1": return "PROFILE1";
                    break;
                case "2": return "PROFILE2";
                    break;
            }
        }
    }     
}


function SetMultiColMapping(tableName, param, value) {
    if (tableName == "ethernetCounters") {
        if (param == "ifSpeed") {
            switch (value) {
                case "1": return "Undetectable";
                    break;
                case "2": return "10 Mbps";
                    break;
                case "3": return "100 Mbps";
                    break;
                case "4": return "1 Gbps";
                    break;
                case "5": return "2.5 Gbps";
                    break;
                case "6": return "10 Gbps";
                    break;
            }
        }
        if (param == "ifAdminStatus") {
            switch (value) {
                case "1": return "<font color=\"green\">Up</font>";
                    break;
                case "2": return "<font color=\"red\">Down</font>";
                    break;
                case "3": return "Testing";
                    break;
            }
        }
        if (param == "ifOperStatus") {
            switch (value) {
                case "1": return "<font color=\"green\">Up</font>";
                    break;
                case "2": return "<font color=\"red\">Down</font>";
                    break;
                case "3": return "Testing";
                    break;
                case "4": return "Unknown";
                    break;
                case "5": return "Dormant";
                    break;
                case "6": return "Not Present";
                    break;
                case "7": return "Lower Layer Down";
                    break;
            }
        }
        if (param == "ifType") {
          value = parseInt(value);
          if( value == 1)
          {
            return "OTHER";
          }
          if( value == 2)
          {
            return "REGULAR1822";
          }
          if( value == 3)
          {
            return "HDH1822";
          }
          if( value == 4)
          {
            return "DDNX25";
          }
          if( value == 5)
          {
            return "RFC877X25";
          }
          if( value == 6)
          {
            return "ETHERNETCSMACD";
          }
          if( value == 7)
          {
            return "ISO88023CSMACD";
          }
          if( value == 8)
          {
            return "ISO88024TOKENBUS";
          }
          if( value == 9)
          {
            return "ISO88025TOKENRING";
          }
          if( value == 10)
          {
            return "ISO88026MAN";
          }
          if( value == 11)
          {
            return "STARLAN";
          }
          if( value == 12)
          {
            return "PROTEON10MBIT";
          }
          if( value == 13)
          {
            return "PROTEON80MBIT";
          }
          if( value == 14)
          {
            return "HYPERCHANNEL";
          }
          if( value == 15)
          {
            return "FDDI";
          }
          if( value == 16)
          {
            return "LAPB";
          }
          if( value == 17)
          {
            return "SDLC";
          }
          if( value == 18)
          {
            return "DS1";
          }
          if( value == 19)
          {
            return "E1";
          }
          if( value == 20)
          {
            return "BASICISDN";
          }
          if( value == 21)
          {
            return "PRIMARYISDN";
          }
          if( value == 22)
          {
            return "PROPPOINTTOPOINTSERIAL";
          }
          if( value == 23)
          {
            return "PPP";
          }
          if( value == 24)
          {
            return "SOFTWARELOOPBACK";
          }
          if( value == 25)
          {
            return "EON";
          }
          if( value == 26)
          {
            return "ETHERNET3MBIT";
          }
          if( value == 27)
          {
            return "NSIP";
          }
          if( value == 28)
          {
            return "SLIP";
          }
          if( value == 29)
          {
            return "ULTRA";
          }
          if( value == 30)
          {
            return "DS3";
          }
          if( value == 31)
          {
            return "SIP";
          }
          if( value == 32)
          {
            return "FRAMERELAY";
          }
          if( value == 33)
          {
            return "RS232";
          }
          if( value == 34)
          {
            return "PARA";
          }
          if( value == 35)
          {
            return "ARCNET";
          }
          if( value == 36)
          {
            return "ARCNETPLUS";
          }
          if( value == 37)
          {
            return "ATM";
          }
          if( value == 38)
          {
            return "MIOX25";
          }
          if( value == 39)
          {
            return "SONET";
          }
          if( value == 40)
          {
            return "X25PLE";
          }
          if( value == 41)
          {
            return "ISO88022LLC";
          }
          if( value == 42)
          {
            return "LOCALTALK";
          }
          if( value == 43)
          {
            return "SMDSDXI";
          }
          if( value == 44)
          {
            return "FRAMERELAYSERVICE";
          }
          if( value == 45)
          {
            return "V35";
          }
          if( value == 46)
          {
            return "HSSI";
          }
          if( value == 47)
          {
            return "HIPPI";
          }
          if( value == 48)
          {
            return "MODEM";
          }
          if( value == 49)
          {
            return "AAL5";
          }
          if( value == 50)
          {
            return "SONETPATH";
          }
          if( value == 51)
          {
            return "SONETVT";
          }
          if( value == 52)
          {
            return "SMDSICIP";
          }
          if( value == 53)
          {
            return "PROPVIRTUAL";
          }
          if( value == 54)
          {
            return "PROPMULTIPLEXOR";
          }
          if( value == 55)
          {
            return "IEEE80212";
          }
          if( value == 56)
          {
            return "FIBRECHANNEL";
          }
          if( value == 57)
          {
            return "HIPPIINTERFACE";
          }
          if( value == 58)
          {
            return "FRAMERELAYINTERCONNECT";
          }
          if( value == 59)
          {
            return "AFLANE8023";
          }
          if( value == 60)
          {
            return "AFLANE8025";
          }
          if( value == 61)
          {
            return "CCTEMUL";
          }
          if( value == 62)
          {
            return "FASTETHER";
          }
          if( value == 63)
          {
            return "ISDN";
          }
          if( value == 64)
          {
            return "V11";
          }
          if( value == 65)
          {
            return "V36";
          }
          if( value == 66)
          {
            return "G703AT64K";
          }
          if( value == 67)
          {
            return "G703AT2MB";
          }
          if( value == 68)
          {
            return "QLLC";
          }
          if( value == 69)
          {
            return "FASTETHERFX";
          }
          if( value == 70)
          {
            return "CHANNEL";
          }
          if( value == 71)
          {
            return "IEEE80211";
          }
          if( value == 72)
          {
            return "IBM370PARCHAN";
          }
          if( value == 73)
          {
            return "ESCON";
          }
          if( value == 74)
          {
            return "DLSW";
          }
          if( value == 75)
          {
            return "ISDNS";
          }
          if( value == 76)
          {
            return "ISDNU";
          }
          if( value == 77)
          {
            return "LAPD";
          }
          if( value == 78)
          {
            return "IPSWITCH";
          }
          if( value == 79)
          {
            return "RSRB";
          }
          if( value == 80)
          {
            return "ATMLOGICAL";
          }
          if( value == 81)
          {
            return "DS0";
          }
          if( value == 82)
          {
            return "DS0BUNDLE";
          }
          if( value == 83)
          {
            return "BSC";
          }
          if( value == 84)
          {
            return "ASYNC";
          }
          if( value == 85)
          {
            return "CNR";
          }
          if( value == 86)
          {
            return "ISO88025DTR";
          }
          if( value == 87)
          {
            return "EPLRS";
          }
          if( value == 88)
          {
            return "ARAP";
          }
          if( value == 89)
          {
            return "PROPCNLS";
          }
          if( value == 90)
          {
            return "HOSTPAD";
          }
          if( value == 91)
          {
            return "TERMPAD";
          }
          if( value == 92)
          {
            return "FRAMERELAYMPI";
          }
          if( value == 93)
          {
            return "X213";
          }
          if( value == 94)
          {
            return "ADSL";
          }
          if( value == 95)
          {
            return "RADSL";
          }
          if( value == 96)
          {
            return "SDSL";
          }
          if( value == 97)
          {
            return "VDSL";
          }
          if( value == 98)
          {
            return "ISO88025CRFPINT";
          }
          if( value == 99)
          {
            return "MYRINET";
          }
          if( value == 100)
          {
            return "VOICEEM";
          }
          if( value == 101)
          {
            return "VOICEFXO";
          }
          if( value == 102)
          {
            return "VOICEFXS";
          }
          if( value == 103)
          {
            return "VOICEENCAP";
          }
          if( value == 104)
          {
            return "VOICEOVERIP";
          }
          if( value == 105)
          {
            return "ATMDXI";
          }
          if( value == 106)
          {
            return "ATMFUNI";
          }
          if( value == 107)
          {
            return "ATMIMA";
          }
          if( value == 108)
          {
            return "PPPMULTILINKBUNDLE";
          }
          if( value == 109)
          {
            return "IPOVERCDLC";
          }
          if( value == 110)
          {
            return "IPOVERCLAW";
          }
          if( value == 111)
          {
            return "STACKTOSTACK";
          }
          if( value == 112)
          {
            return "VIRTUALIPADDRESS";
          }
          if( value == 113)
          {
            return "MPC";
          }
          if( value == 114)
          {
            return "IPOVERATM";
          }
          if( value == 115)
          {
            return "ISO88025FIBER";
          }
          if( value == 116)
          {
            return "TDLC";
          }
          if( value == 117)
          {
            return "GIGABITETHERNET";
          }
          if( value == 118)
          {
            return "HDLC";
          }
          if( value == 119)
          {
            return "LAPF";
          }
          if( value == 120)
          {
            return "V37";
          }
          if( value == 121)
          {
            return "X25MLP";
          }
          if( value == 122)
          {
            return "X25HUNTGROUP";
          }
          if( value == 123)
          {
            return "TRANSPHDLC";
          }
          if( value == 124)
          {
            return "INTERLEAVE";
          }
          if( value == 125)
          {
            return "FAST";
          }
          if( value == 126)
          {
            return "IP";
          }
          if( value == 127)
          {
            return "DOCSCABLEMACLAYER";
          }
          if( value == 128)
          {
            return "DOCSCABLEDOWNSTREAM";
          }
          if( value == 129)
          {
            return "DOCSCABLEUPSTREAM";
          }
          if( value == 130)
          {
            return "A12MPPSWITCH";
          }
          if( value == 131)
          {
            return "TUNNEL";
          }
          if( value == 132)
          {
            return "COFFEE";
          }
          if( value == 133)
          {
            return "CES";
          }
          if( value == 134)
          {
            return "ATMSUBINTERFACE";
          }
          if( value == 135)
          {
            return "L2VLAN";
          }
          if( value == 136)
          {
            return "L3IPVLAN";
          }
          if( value == 137)
          {
            return "L3IPXVLAN";
          }
          if( value == 138)
          {
            return "DIGITALPOWERLINE";
          }
          if( value == 139)
          {
            return "MEDIAMAILOVERIP";
          }
          if( value == 140)
          {
            return "DTM";
          }
          if( value == 141)
          {
            return "DCN";
          }
          if( value == 142)
          {
            return "IPFORWARD";
          }
          if( value == 143)
          {
            return "MSDSL";
          }
          if( value == 144)
          {
            return "IEEE1394";
          }
          if( value == 145)
          {
            return "IF_GSN";
          }
          if( value == 146)
          {
            return "DVBRCCMACLAYER";
          }
          if( value == 147)
          {
            return "DVBRCCDOWNSTREAM";
          }
          if( value == 148)
          {
            return "DVBRCCUPSTREAM";
          }
          if( value == 149)
          {
            return "ATMVIRTUAL";
          }
          if( value == 150)
          {
            return "MPLSTUNNEL";
          }
          if( value == 151)
          {
            return "SRP";
          }
          if( value == 152)
          {
            return "VOICEOVERATM";
          }
          if( value == 153)
          {
            return "VOICEOVERFRAMERELAY";
          }
          if( value == 154)
          {
            return "IDSL";
          }
          if( value == 155)
          {
            return "COMPOSITELINK";
          }
          if( value == 156)
          {
            return "SS7SIGLINK";
          }
          if( value == 157)
          {
            return "PROPWIRELESSP2P";
          }
          if( value == 158)
          {
            return "FRFORWARD";
          }
          if( value == 159)
          {
            return "RFC1483";
          }
          if( value == 160)
          {
            return "USB";
          }
          if( value == 161)
          {
            return "IEEE8023ADLAG";
          }
          if( value == 162)
          {
            return "BGPPOLICYACCOUNTING";
          }
          if( value == 163)
          {
            return "FRF16MFRBUNDLE";
          }
          if( value == 164)
          {
            return "H323GATEKEEPER";
          }
          if( value == 165)
          {
            return "H323PROXY";
          }
          if( value == 166)
          {
            return "MPLS";
          }
          if( value == 167)
          {
            return "MFSIGLINK";
          }
          if( value == 168)
          {
            return "HDSL2";
          }
          if( value == 169)
          {
            return "SHDSL";
          }
          if( value == 170)
          {
            return "DS1FDL";
          }
          if( value == 171)
          {
            return "POS";
          }
          if( value == 172)
          {
            return "DVBASIIN";
          }
          if( value == 173)
          {
            return "DVBASIOUT";
          }
          if( value == 174)
          {
            return "PLC";
          }
          if( value == 175)
          {
            return "NFAS";
          }
          if( value == 176)
          {
            return "TR008";
          }
          if( value == 177)
          {
            return "GR303RDT";
          }
          if( value == 178)
          {
            return "GR303IDT";
          }
          if( value == 179)
          {
            return "ISUP";
          }
          if( value == 180)
          {
            return "PROPDOCSWIRELESSMACLAYER";
          }
          if( value == 181)
          {
            return "PROPDOCSWIRELESSDOWNSTREAM";
          }
          if( value == 182)
          {
            return "PROPDOCSWIRELESSUPSTREAM";
          }
          if( value == 183)
          {
            return "HIPERLAN2";
          }
          if( value == 184)
          {
            return "PROPBWAP2MP";
          }
          if( value == 185)
          {
            return "SONETOVERHEADCHANNEL";
          }
          if( value == 186)
          {
            return "DIGITALWRAPPEROVERHEADCHANNEL";
          }
          if( value == 187)
          {
            return "AAL2";
          }
          if( value == 188)
          {
            return "RADIOMAC";
          }
          if( value == 189)
          {
            return "ATMRADIO";
          }
          if( value == 190)
          {
            return "IMT";
          }
          if( value == 191)
          {
            return "MVL";
          }
          if( value == 192)
          {
            return "REACHDSL";
          }
          if( value == 193)
          {
            return "FRDLCIENDPT";
          }
          if( value == 194)
          {
            return "ATMVCIENDPT";
          }
          if( value == 195)
          {
            return "OPTICALCHANNEL";
          }
          if( value == 196)
          {
            return "OPTICALTRANSPORT";
          }
          if( value == 197)
          {
            return "PROPATM";
          }
          if( value == 198)
          {
            return "VOICEOVERCABLE";
          }
          if( value == 199)
          {
            return "INFINIBAND";
          }
          if( value == 200)
          {
            return "TELINK";
          }
          if( value == 201)
          {
            return "Q2931";
          }
          if( value == 202)
          {
            return "VIRTUALTG";
          }
          if( value == 203)
          {
            return "SIPTG";
          }
          if( value == 204)
          {
            return "SIPSIG";
          }
          if( value == 205)
          {
            return "DOCSCABLEUPSTREAMCHANNEL";
          }
          if( value == 206)
          {
            return "ECONET";
          }
          if( value == 207)
          {
            return "PON155";
          }
          if( value == 208)
          {
            return "PON622";
          }
          if( value == 209)
          {
            return "BRIDGE";
          }
          if( value == 210)
          {
            return "LINEGROUP";
          }
          if( value == 211)
          {
            return "VOICEEMFGD";
          }
          if( value == 212)
          {
            return "VOICEFGDEANA";
          }
          if( value == 213)
          {
            return "VOICEDID";
          }
          if( value == 214)
          {
            return "MPEGTRANSPORT";
          }
          if( value == 215)
          {
            return "SIXTOFOUR";
          }
          if( value == 216)
          {
            return "GTP";
          }
          if( value == 217)
          {
            return "PDNETHERLOOP1";
          }
          if( value == 218)
          {
            return "PDNETHERLOOP2";
          }
          if( value == 219)
          {
            return "OPTICALCHANNELGROUP";
          }
          if( value == 220)
          {
            return "HOMEPNA";
          }
          if( value == 221)
          {
            return "GFP";
          }
          if( value == 222)
          {
            return "CISCOISLVLAN";
          }
          if( value == 223)
          {
            return "ACTELISMETALOOP";
          }
          if( value == 224)
          {
            return "FCIPLINK";
          }
          if( value == 225)
          {
            return "RPR";
          }
          if( value == 226)
          {
            return "QAM";
          }
          if( value == 227)
          {
            return "LMP";
          }
          if( value == 228)
          {
            return "CBLVECTASTAR";
          }
          if( value == 229)
          {
            return "DOCSCABLEMCMTSDOWNSTREAM";
          }
          if( value == 230)
          {
            return "ADSL2";
          }
          if( value == 231)
          {
            return "MACSECCONTROLLEDIF";
          }
          if( value == 232)
          {
            return "MACSECUNCONTROLLEDIF";
          }
          if( value == 233)
          {
            return "AVICIOPTICALETHER";
          }
          if( value == 234)
          {
            return "ATMBOND";
          }
          if( value == 235)
          {
            return "VOICEFGDOS";
          }
          if( value == 236)
          {
            return "MOCAVERSION1";
          }
          if( value == 237)
          {
            return "IEEE80216WMAN";
          }
          if( value == 238)
          {
            return "ADSL2PLUS";
          }
          if( value == 239)
          {
            return "DVBRCSMACLAYER";
          }
          if( value == 240)
          {
            return "DVBTDM";
          }
          if( value == 241)
          {
            return "DVBRCSTDMA";
          }
          if( value == 242)
          {
            return "X86LAPS";
          }
          if( value == 243)
          {
            return "WWANPP";
          }
          if( value == 244)
          {
            return "WWANPP2";
          }
          if( value == 245)
          {
            return "VOICEEBS";
          }
          if( value == 246)
          {
            return "IFPWTYPE";
          }
          if( value == 247)
          {
            return "ILAN";
          }
          if( value == 248)
          {
            return "PIP";
          }
          if( value == 249)
          {
            return "ALUELP";
          }
          if( value == 250)
          {
            return "GPON";
          }
          if( value == 251)
          {
            return "VDSL2";
          }
      }
      if (param == "lastCollectedAt") {
          return DisplayTime(value);
      }  
    }
    else if (tableName == "ipCounters") {
        if (param == "ipSystemStatsIPVersion") {
            switch (value) {
                case "0": return "Unknown";
                    break;
                case "1": return "IPv4";
                    break;
                case "2": return "IPv6";
                    break;
            }
        }
    }
    return value;
}

/* Display time upto seconds */
function DisplayTime(temp) {
    if (temp == "-" || temp == null) {
        return temp;
    }
    var year = temp.slice(0, 4);
    var month = temp.slice(4, 6);
    var date = temp.slice(6, 8);
    var time = temp.slice(8, 10) + ":" + temp.slice(10, 12) + ":" + temp.slice(12, 14);
    switch (month) {
        case "01": month = "Jan";
            break;
        case "02": month = "Feb";
            break;
        case "03": month = "Mar";
            break;
        case "04": month = "Apr";
            break;
        case "05": month = "May";
            break;
        case "06": month = "Jun";
            break;
        case "07": month = "Jul";
            break;
        case "08": month = "Aug";
            break;
        case "09": month = "Sep";
            break;
        case "10": month = "Oct";
            break;
        case "11": month = "Nov";
            break;
        case "12": month = "Dec";
            break;
    }
    temp = month + " " + date + " " + time + " " + year;
    return temp;
}

/* Display time upto milliseconds */
function DisplayTimeUptoMilliSecond(temp) {
    if (temp == "-" || temp == null) {
        return temp;
    }
    var year = temp.slice(0, 4);
    var month = temp.slice(4, 6);
    var date = temp.slice(6, 8);
    var time = temp.slice(8, 10) + ":" + temp.slice(10, 12) + ":" + temp.slice(12, 14) + "." + temp.slice(14, 17);
    switch (month) {
        case "01": month = "Jan";
            break;
        case "02": month = "Feb";
            break;
        case "03": month = "Mar";
            break;
        case "04": month = "Apr";
            break;
        case "05": month = "May";
            break;
        case "06": month = "Jun";
            break;
        case "07": month = "Jul";
            break;
        case "08": month = "Aug";
            break;
        case "09": month = "Sep";
            break;
        case "10": month = "Oct";
            break;
        case "11": month = "Nov";
            break;
        case "12": month = "Dec";
            break;
    }
    temp = month + " " + date + " " + time + " " + year;
    return temp;
}



function SpecialUsedAttributes(attribute, action) {
    if (attribute == 'acBarrMoSigEnabledChkb' && action == 'disable') {
        $('#acBarrMoSigEnabled').val('0');
        $('#acBarrMoSigEnabledtext').val('FALSE');
        $('#acBarrMoSigEnabledtextHidden').val('FALSE');
        $('#acBarrMoSigFactor,#acBarrMoSigTime').css('background-color', '#E6E6E6');
        FreezeList($('#acBarrMoSigFactor,#acBarrMoSigTime,#acBarrMoSigSpecialAC'));
        $('#acBarrMoSigEnabledtext,#acBarrMoSigSpecialAC').attr('readonly', true).css('background-color', '#E6E6E6');
    }
    else
        if (attribute == 'acBarrMoDataEnabledChkb' && action == 'disable') {
        $('#acBarrMoDataEnabled').val('0');
        $('#acBarrMoDataEnabledtext').val('FALSE');
        $('#acBarrMoDataEnabledtextHidden').val('FALSE');
        $('#acBarrMoDataFactor,#acBarrMoDataTime').css('background-color', '#E6E6E6');
        FreezeList($('#acBarrMoDataFactor,#acBarrMoDataTime,#acBarrMoDataSpecialAC'));
        $('#acBarrMoDataEnabledtext,#acBarrMoDataSpecialAC').attr('readonly', true).css('background-color', '#E6E6E6');
    }
    else
        if (attribute == 'acBarrMoSigEnabledChkb' && action == 'enable') {
        $('#acBarrMoSigEnabled').val('1');
        $('#acBarrMoSigEnabledtext').val('TRUE').css('background-color', 'white');
        $('#acBarrMoSigEnabledtextHidden').val('TRUE');
        $('#acBarrMoSigSpecialAC').attr('readonly', false).css('background-color', 'white');
        $('#acBarrMoSigFactor,#acBarrMoSigTime').css('background-color', 'white');
    }
    else
        if (attribute == 'acBarrMoDataEnabledChkb' && action == 'enable') {
        $('#acBarrMoDataEnabled').val('1');
        $('#acBarrMoDataEnabledtext').val('TRUE').css('background-color', 'white');
        $('#acBarrMoDataEnabledtextHidden').val('TRUE');
        $('#acBarrMoDataSpecialAC').attr('readonly', false).css('background-color', 'white');
        $('#acBarrMoDataFactor,#acBarrMoDataTime').css('background-color', 'white');
    }

    if (action == 'disable') {
        $('#acBarrMoSigFactor,#acBarrMoSigTime,,#acBarrMoSigEnabledtext,#acBarrMoSigSpecialAC').attr('title', 'Use MO Signaling to Modify').simpletooltipChkBox('acBarrMoSigEnabledChkb');
        $('#acBarrMoDataFactor,#acBarrMoDataTime,#acBarrMoDataEnabledtext,#acBarrMoDataSpecialAC').attr('title', 'Use MO Data to Modify').simpletooltipChkBox('acBarrMoDataEnabledChkb');

    }
}




function DisplayGps(type, temp) {
    if (temp == "-" || temp == null) {
        return temp;
    }
    var gpsData = "";
    var gpsArray = temp.split(".", 2);
    if (type == "longitude") {
        if (gpsArray[0].length == 5) {
            gpsData = gpsArray[0].substr(0, 3) + '&#186;' + '&nbsp;&nbsp;' + gpsArray[0].substr(3) + '.';
        }
        else
        if (gpsArray[0].length == 4) {
            gpsData = gpsArray[0].substr(0, 2) + '&#186;' + '&nbsp;&nbsp;' + gpsArray[0].substr(2) + '.';
        }
        else
        if (gpsArray[0].length == 3) {
            gpsData = gpsArray[0].substr(0, 1) + '&#186;' + '&nbsp;&nbsp;' + gpsArray[0].substr(1) + '.';
        }
        gpsData = gpsData + gpsArray[1].substr(0, 6) + '&#39;' + '&nbsp;&nbsp;' + gpsArray[1].substr(6, 6);
    }
    else if (type == "latitude") {
        if (gpsArray[0].length == 4) {
            gpsData = gpsArray[0].substr(0, 2) + '&#186;' + '&nbsp;&nbsp;' + gpsArray[0].substr(2) + '.';
        }
        else
        if (gpsArray[0].length == 3) {
            gpsData = gpsArray[0].substr(0, 1) + '&#186;' + '&nbsp;&nbsp;' + gpsArray[0].substr(1) + '.';
        }
        gpsData = gpsData + gpsArray[1].substr(0, 6) + '&#39;' + '&nbsp;&nbsp;' + gpsArray[1].substr(6, 6);
    }
    return gpsData;
}
