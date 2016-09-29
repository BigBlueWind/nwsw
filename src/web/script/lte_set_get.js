/////////////////// [ GET FUNCTION ] //////////////////////////////////////////////////////////////
function LteGet(tableName, paramList, cb, count) {
    var result = null;
    var query = "";
    var rand = (new Date()).valueOf();
    var sessionID = parent.sessionID;

    //GET Multiple row data
    if (paramList == null) {
        query = "key="+ tableName + "&rand=" + rand;
    }
    //GET Single row data
    else {
        query = "key="+ tableName + "&rand=" + rand + paramList.serialize();
    }

    var sTime = new Date();
    $("#cmdMsg").html("GET Command in Progress.......!");
    $("#cmdMsg").show();
    $.post(sessionID + "-LTEWebGet.fcgi", query,
           function(response) {
               $("#cmdMsg").hide();
               if (response != null && response != undefined) {
                   if (response.result == "SUCCESS") {
                       result = response.result;
                       if (response.data != "")
                           DISPLAY(tableName, response, count);
                       if (cb != null)
                           cb(tableName, response);
                         UpdateHiddenFieldsSuccess();
                   }
                   else {
                       result = response.data;
                       ChkLock(tableName);
                       if (response.data == "User Logged out due to INACTIVITY.") {
                           alert("Session Timed Out Due To INACTIVITY!!!");
                           parent.window.location.replace("lte_login.htm");
                       }
                   }
                   var eTime = new Date();
                   execTime = eTime - sTime;
                   STATUS("GET", tableName, sTime, eTime, response.result, response.data);
               }
           }
            , "json");
    return result;
}

////////////////////// [ SET FUNCTION ] ///////////////////////////////////////////////////////////

function LteSet(tableName, paramList, btnCtxt, cb)
{
    var request = "";
    var result = null;
    var sessionID = parent.sessionID;
    if ($(paramList).is("form") == true)
        request = paramList.serialize();
    else
        request = paramList;
    if (request != "") 
    {
        request = "key=" + tableName + "&" + request;
        var sTime = new Date();
        if (btnCtxt != null)
            $(btnCtxt).attr("disabled", true);
        $("#cmdMsg").html("SET Command in Progress.......!");
        $("#cmdMsg").show();
        $.post(sessionID + "-LTEWebSet.fcgi", request,
               function(response) 
               {
                   $("#cmdMsg").hide();
                   if (response != null && response != "undefined") 
                    {
                        if (response.result == "SUCCESS") 
                        {
                            result = response.result;
                            if(response != "");
                            DISPLAY(tableName, response);
                            if (cb != null)
                                cb(tableName, response);
                            UpdateHiddenFieldsSuccess();
                        }
                        else 
                        {
                            result = response.data;
                            ChkLock(tableName);
                            if (response.data == "User Logged out due to INACTIVITY.") 
                            {
                                alert("Session Timed Out Due To INACTIVITY!!!");
                               parent.window.location.replace("lte_login.htm");
                            }
                        }
                    }
                    else 
                    {
                        result = null;
                    }
                    var eTime = new Date();
                    execTime = eTime - sTime;
                    STATUS("SET", tableName, sTime, eTime, response.result, response.data);
                    if (btnCtxt != null)
                        $(btnCtxt).attr("disabled", false);
               }, "json");
    }
    
    return result;
}

//////////////////////////// [ DISPLAY ] //////////////////////////////////////////////////////////

function DISPLAY(tableName, content, count) {
    var param = "";
    var value = "";
    if (content == null || content == undefined || content.result == false) {
        alert("ERROR IN LOADING THE TABLE DATA");
    }
    else 
    {
        if (count == null || count == undefined)
        {
            if (tableName == "swPackageStatus") {
                DISPLAY_SWPACSTATUS(content);
                return;
            }
            for (var i = 0; i < content.data.row.length; i++) {
                for (var j = 0; j < content.data.row[i].col.length; j++) {
                    param = content.data.row[i].col[j].param;
                    value = content.data.row[i].col[j].value;
                    if ($(parent.frames["MiddleRight"].document.getElementById(param)).is("td,span,div,a") == true)
                        $(parent.frames["MiddleRight"].document.getElementById(param)).html(value);
                    else if ($("#" + param).is(":radio,:checkbox")) {
                        $(parent.frames["MiddleRight"].document.getElementById(param)).val(value);
                        $(parent.frames["MiddleRight"].document.getElementById(param)).get(0).checked = (parseInt(value) > 0 ? true : false);
                        $(parent.frames["MiddleRight"].document.getElementById(param)).triggerHandler("click");
                    }
                    else
                        $(parent.frames["MiddleRight"].document.getElementById(param)).val(value);
                }
            }
        }
        else
        {
            for (var i = 0; i < count; i++) {
                for (var j = 0; j < content.data.row[i].col.length; j++) {
                    param = content.data.row[i].col[j].param;
                    value = content.data.row[i].col[j].value;
                    if ($(parent.frames["MiddleRight"].document.getElementById(param)).is("td,span,div,a") == true)
                        $(parent.frames["MiddleRight"].document.getElementById(param)).html(value);
                    else if ($("#" + param).is(":radio,:checkbox")) {
                        $(parent.frames["MiddleRight"].document.getElementById(param)).val(value);
                        $(parent.frames["MiddleRight"].document.getElementById(param)).get(0).checked = (parseInt(value) > 0 ? true : false);
                        $(parent.frames["MiddleRight"].document.getElementById(param)).triggerHandler("click");
                    }
                    else
                        $(parent.frames["MiddleRight"].document.getElementById(param)).val(value);
                }
            }
        
        
        }
    }
}

function DISPLAY_SWPACSTATUS(content) {
    for (var i = 0; i < content.data.row.length; i++) {
        if (content.data.row[i].col[5].value == "1") {
            for (var j = 0; j < content.data.row[i].col.length; j++) {
                param = content.data.row[i].col[j].param;
                value = content.data.row[i].col[j].value;
                if ($(parent.frames["MiddleRight"].document.getElementById(param)).is("td,span,div,a") == true)
                    $(parent.frames["MiddleRight"].document.getElementById(param)).html(value);
                else if ($("#" + param).is(":radio,:checkbox")) {
                    $(parent.frames["MiddleRight"].document.getElementById(param)).val(value);
                    $(parent.frames["MiddleRight"].document.getElementById(param)).get(0).checked = (parseInt(value) > 0 ? true : false);
                    $(parent.frames["MiddleRight"].document.getElementById(param)).triggerHandler("click");
                }
                else
                    $(parent.frames["MiddleRight"].document.getElementById(param)).val(value);
            }
            break;
        }        
    }
}

//////////////////////////// [ DISPLAY RESULT ] ///////////////////////////////////////////////////

function STATUS(command, tableName, sTime, eTime, result, description)
{
    addRow("resultTable", command, tableName, sTime, eTime, result, description);
    var element = parent.frames["MiddleDown"].document.getElementById("resultarea");
    var length = $(element).scrollTop();
    $(element).scrollTop(-length);
}

function addRow(tableID, command, tableName, sTime, eTime, result, description)
{
    var table = parent.frames["MiddleDown"].document.getElementById(tableID);
 //   if (table != null)
  //  {
        var rowCount = table.rows.length;
        table.className = "tblBorder";
        table.width = "1065px";
        table.border = "0";
        table.cellpadding = "0";
        table.cellspacing = "0";
        table.overflow ="auto"
        var row = table.insertRow(1);
        var cell1 = row.insertCell(0);        
        var cell2 = row.insertCell(1);
        var cell3 = row.insertCell(2);
        var cell4 = row.insertCell(3);
        var cell5 = row.insertCell(4);
        var cell6 = row.insertCell(5);
        
        if (rowCount % 2 == 1) {
            row.className = "conTblRowBg1_general"
        }
        else {
            row.className = "conTblRowBg2_general"
        }

        cell1.align = "center";
        cell1.width = "10%";
        cell1.innerHTML = command;

        cell2.align = "center";
        cell2.width = "20%";
        cell2.innerHTML = tableName;

        cell3.align = "center";
        cell3.width = "9%";
        var hrs = sTime.getHours();
        var min = sTime.getMinutes();
        var sec = sTime.getSeconds();
        if (hrs < 10)
            var hrs = "0" + hrs;
        if (min < 10)
            var min = "0" + min;
        if (sec < 10)
            var sec = "0" + sec;
            
        cell3.innerHTML = hrs + ":" + min + ":" + sec ;

        cell4.align = "center";
        cell4.width = "11%";
         hrs = eTime.getHours();
         min = eTime.getMinutes();
         sec = eTime.getSeconds();
        if (hrs < 10)
            var hrs = "0" + hrs;
        if (min < 10)
            var min = "0" + min;
        if (sec < 10)
            var sec = "0" + sec;

        cell4.innerHTML = hrs + ":" + min + ":" + sec;
       // cell4.innerHTML = eTime.getHours() + ":" + eTime.getMinutes() + ":" + eTime.getSeconds();

        cell5.align = "center";
        cell5.width = "13%";
        if (result == "SUCCESS")
        {
            cell5.innerHTML ="<font color=\"green\">"+result+"</font>" ;
        }
        else 
        {
            cell5.innerHTML = "<font color=\"red\">" + result + "</font>";
            cell6.align = "center";
            cell6.width = "37%";
            cell6.innerHTML = description;
        }
 //   }
}

//////////////////////////// [ HANDLING SPECIAL VALUES ] ///////////////////////////////////////////////////

function EnableInput(chkbox, input, specialValue, hidden) 
{
    if ($("#" + chkbox).is(':checked') == true) 
    {
        if ($("#" + input).is("input:text") == true) {
            $("#" + input).attr("readonly", false).val($("#" + hidden).val());
        }
        else {
            $("#" + input).attr("readonly", true);
        }
        $("#" + input).css("background-color", "white");
    }
    else 
    {        
        if ($("#" + input).is("input:text") == true)
            $("#" + input).val(specialValue).attr("readonly",true);
        else
            $("#" + input).attr("disabled", true);
        $("#" + input).css("background-color", "#E6E6E6"); 
    }
}


//////////////////////////// [ HANDLING UPDATE OF MULTI ROW TABLES ] ///////////////////////////////////////////////////

var tableId = null;
function updateTable(tableName, id) 
{
    tableId = id;
    LteGet(tableName, null, StatusTableDisplay);
}

function StatusTableDisplay(tableName, d) {
    delRow(tableId);
    if (d.data == "") {
        addRowStatus(tableId, null, null);
    }
    else {
        for (i = 0; i < d.data.row.length; i++) {
            addRowStatus(tableId, i + 1, d.data.row[i]);
        }
    }
}
function delRow(tableID) {
    var table = document.getElementById(tableID);
    var rowCount = table.rows.length;
    for (var i = 0, j = 0; j < rowCount; j++) {
        table.deleteRow(i);
    }
    rowCount = 0;
}


//////////////////////////// [ HANDLING UPDATE OF MULTI COLUMN  TABLES ] ///////////////////////////////////////////////////

function UpdateMultiColTable(tableName, id) {
    tableId = id;
    LteGet(tableName, null, StatusMultiColTableDisplay);
}

function StatusMultiColTableDisplay(tableName, d) {
    var divcount = delCol("maintable", 1);
    delCol(tableName + "Table", 3);
    var div = document.getElementById("maindiv");
    div.style.width = parseInt(div.style.width) - (divcount * 150);
    if (d.data == "") {
        addRowStatus(tableId, null, null);
    }
    else {
        for (i = 0; i < d.data.row.length; i++) {
            addRowMultiColStatus(tableId, i + 1, d.data.row[i]);
        }
    }
}

function delCol(tableID, nocols) {
    var table = document.getElementById(tableID);
    var rowCount = table.rows.length;
    var delCellCount = 0;
    for (var i = 0; i < rowCount; i++) {
        var colCount = table.rows[i].cells.length;
        for (var j = colCount; j > nocols; j--) {
            table.rows[i].deleteCell(j - 1);
            if (i == 0)
                delCellCount++;
        }
    }
    return delCellCount;
}

/////////////////////////////////////////// [ DISABLE HOT KEYS ] //////////////////////////////////////////

function DisableRefresh(e)
{
    if (!e)
        var e = window.event;    
    var keycode = e.keyCode;
    if (e.which)
        keycode = e.which;

    var src = e.srcElement;
    if (e.target)
        src = e.target;

    // 116 = F5
    if (e.keyCode == 17) {
        alert("Due to security reasons, HOT KEYS are disabled !!!");
        return false;
    }
    if (116 == keycode || (e.altKey == true) || 118 == keycode || 112 == keycode || 114 == keycode || 115 == keycode || 122 == keycode || 27 == keycode || 19 == keycode) {
        // Firefox and other non IE browsers 
        if (e.preventDefault) {
            e.preventDefault();
            e.stopPropagation();
            alert("Due to security reasons, HOT KEYS are disabled !!!");
        }
        // Internet Explorer
        else if (e.keyCode || (e.altKey == true) || (e.ctrlKey == true)) {
            if (e.keyCode == 112) {
                document.onhelp = function() { return (false); }
                window.onhelp = function() { return (false); }
            }
            event.returnValue = false;
            e.keyCode = 0;
            alert("Due to security reasons, HOT KEYS are disabled !!!");
            }
        return false;
    }
}

//////////////////////////////////////// [ CHECK ENODEB STATUS ] //////////////////////////////////////

function ChkLock(tableName)
{
    var query = "";
    var rand = (new Date()).valueOf();
    var sessionID = parent.sessionID;
    $(':text, :password, select').each(function() {
        $(this).attr('disabled', false);
    });
    $(':checkbox').each(function() {
        $(this).attr('disabled', false);
    }); 
    query = "key=enbStatus&rand=" + rand;

    $.post(sessionID + "-LTEWebGet.fcgi", query,
           function(response) {
               if (response != null || response != undefined) {
                   if (response.result == "SUCCESS") {
                       if (parent.frames["MiddleRight"].ChkSpecialValues != null)
                           parent.frames["MiddleRight"].ChkSpecialValues();
                       if (response.data.row[0].col[5].value == 2) {
                           parent.frames["Top"].isLocked = "unlocked";
                           if (parent.frames["MiddleRight"].LockAttributes != null)
                                parent.frames["MiddleRight"].LockAttributes();                           
                       }
                       else
                       if (response.data.row[0].col[5].value == 1) {
                           parent.frames["Top"].isLocked = "locked";
                       }
                   }
               }
           }
           , "json");
}

/////////////////////////////////////// [ UPDATE HIDDEN FIELDS ON SET SUCCESS ] ///////////////////////////

function UpdateHiddenFieldsSuccess() {
    $(':text, :password,:hidden, select').each(function() {
        $('#' + this.id + 'Hidden').val($(this).val());
    });
} 

//////////////////////////////////////// [ LOCKING SELECT BOX ITEMS ] //////////////////////////////////////

function FreezeList(item) {
   if (item != null && item != undefined) {
       $(item).each(function(i) {
           if ( document.getElementById(this.id + "hidden") != null ) {
               $(this).val(document.getElementById(this.id + "hidden").value);
           }
           else {
               $(parent.frames["MiddleRight"].document.body).append("<input type='hidden' id='" + this.id + "hidden" + "' />");
               document.getElementById(this.id + "hidden").value=$(this).val();
           }
       });
   }
}

function SelectBoxReload(item){
   if (item != null && item != undefined) {
       $(item).each(function(i) {
           if (document.getElementById(this.id + "hidden") != null) {
               document.getElementById(this.id + "hidden").value = $(this).val();
           }               
       });
   }
}

/////////////////////////// [ADD ENUM VALUES TO SELECT BOX] ////////////////////////////////////////////////
function addOption(selectbox, text, value) {
   var optn = document.createElement("OPTION");
   optn.text = text;
   optn.value = value;
   selectbox.options.add(optn);
}

////////////////////////// [CREATE CIRCLE USING JAVASCRIPT 2D DRAWING] ////////////////////////////////////
function CreateCircle(id, color) {
    var graphics = new jsGraphics(document.getElementById(id));

    //Create jsColor object 
    switch (color) {
        case "1": var col = new jsColor("#FFD700");
            break;
        case "2": var col = new jsColor("red");
            break;
        case "3": var col = new jsColor("green");
            break;
    } 
    var point = new jsPoint(50,50);
    //Draw filled circle with pt1 as center point and radius 30.
    graphics.fillCircle(col, point, 5);
}

    
