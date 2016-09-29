
function validate(type, name, fieldName, min, max) {
    switch (type) {
        case "IsEmpty": return IsEmpty(name, fieldName);
        case "PwdValidate": return PwdValidate(name, fieldName);
        case "IpValidate": return IpValidate(name, fieldName);
        case "SubnetValidate": return SubnetValidate(name, fieldName);
        case "EmailValidate": return EmailValidate(name, fieldName);
        case "UrlValidate": return UrlValidate(name, fieldName);
        case "StrValidate": return StrValidate(name, fieldName);
        case "StrLengthValidate": return StrLengthValidate(name, fieldName, min, max);
        case "IntegerStrLengthRangeValidate": return IntegerStrLengthRangeValidate(name, fieldName, min, max);
        case "IntegerValidate": return IntegerValidate(name, fieldName, min, max);
        case "FloatValidate": return FloatValidate(name, fieldName);
        case "IntRangeChk": return IntRangeChk(name, fieldName, min, max);
        case "FloatRangeChk": return FloatRangeChk(name, fieldName, min, max);
        case "MacValidate": return MacValidate(name, fieldName);
        case "BinaryValidate": return BinaryValidate(name, fieldName);
        case "IntLengthValidate": return IntLengthValidate(name, fieldName, min, max);
        case "NameValidate": return NameValidate(name, fieldName, min, max);
        case "LocalTimeValidate": return LocalTimeValidate(name, fieldName);
        case "UTCTimeValidate": return UTCTimeValidate(name, fieldName);
        case "GpsLatitudeValidate": return GpsLatitudeValidate(name, fieldName);
        case "GpsLongitudeValidate": return GpsLongitudeValidate(name, fieldName);
        case "AlphaNumericValidate": return AlphaNumericValidate(name, fieldName, min, max)
    }                
}

//Empty Tested 
function IsEmpty(name, fieldName) {
    var str = document.getElementById(name).value;
    if (str.length == 0 || str == "null" || str == "undefined")
        return "Please enter " + fieldName + ".\n";
    else
        return "";

}

//Confirm Password Check Tested
function PwdValidate(pwd, cpwd) {
    var password = document.getElementById(pwd).value;
    var copy = document.getElementById(cpwd).value;
    if (password != copy)
        return "Confirm Password not matching with Password";
    else
        return "";
}

//IP Address Validation Tested 
function IpValidate(name, fieldName) {
    var str = document.getElementById(name).value;
    var objRegExp = /^((25[0-5]|2[0-4][0-9]|1[0-9]{2}|[0-9]{1,2})\.){3}(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[0-9]{1,2})$/;
    if (!objRegExp.test(str))
        return "Please enter valid " + fieldName + ".\n";
    else
        return "";
}

//Subnet Mask Validation Supporting All Formats Tested
function SubnetValidate(name, fieldName) {
    var mask = document.getElementById(name).value;
    var y, z;
    var str = "";
    if (IpValidate(name, fieldName) == "") {
        var x = mask.split(".");
        for (var i = 0; i < 4; i++) {
            y = parseInt(x[i], 10);
            z = "00000000" + y.toString(2);
            str = str + z.substring(z.length, z.length - 8);
        }

        var r = new RegExp("^[1]+[0]+$");
        if (!r.test(str)) {
            return "Please enter valid " + fieldName + ".\n";
        }
        else
            return "";
    }
    else
        return "Please enter valid " + fieldName + ".\n";
}

//Email Tested
function EmailValidate(name) {
    var str = document.getElementById(name).value;
    var objRegExp = /^([A-Za-z0-9_\-\.])+\@([A-Za-z0-9_\-\.])+\.([A-Za-z]{2,4})$/;
    if (!(objRegExp.test(str)))
        return "Please enter a valid email address.\n";
    return "";
}

//URL Validation Tested
function UrlValidate(name, fieldName) {
    var str = document.getElementById(name).value;
    var objRegExp = /^http:\/\/[A-Za-z0-9\.-]{3,}\.[A-Za-z]{3}$/;
    if (!(objRegExp.test(str)))
        return "Please enter valid " + fieldName + ". \n";
    return "";
}

//String Validation Tested
function StrValidate(name, charSet, fieldName) {
    var reg = null;
    var str = document.getElementById(name).value;
    if ((charSet != "" && charSet != null))
        reg = charSet;
    var regExp = new RegExp(reg);
    if (!regExp.test(str)) {
        return "Please enter valid " + fieldName + ". \n";
    }
    return "";
}
//String Length Tested
function StrLengthValidate(name, fieldname, min, max) {
    var length = document.getElementById(name).value.length;
    var msg;
    if ((msg = IsEmpty(name, fieldname)) != "") {
        return msg;
    }
    else {
        if (length >= min && length <= max)
            return "";
        else {
            if (min == max)
                return "Please enter " + fieldname + " of length " + min + " characters.\n";
            else
                return "Please enter " + fieldname + " of length between " + min + " and " + max + " characters.\n";
        }
    }
}

//Integer Validation Tested
function IntegerValidate(name, fieldName) {
    var str = document.getElementById(name).value;
    var objRegExp = /^[-+]?[0-9]+$/;
    if (!objRegExp.test(str))
        return "Please enter a valid Integer for " + fieldName + ".\n";
    return "";
}

// Float Validation Tested
function FloatValidate(name, fieldName) {
    var str = document.getElementById(name).value;
    var objRegExp = /^[-+]?[0-9]+(\.[0-9]+)?$/;
    if (!objRegExp.test(str))
        return "Please enter a valid Float Value for " + fieldName + ".\n";
    return "";
}

// Integer String Length and Range Validation Tested
function IntegerStrLengthRangeValidate(name, fieldName, min, max) {
    var str = document.getElementById(name).value;
    if (str.length < min || str.length > max) 
    {
        if (min != max)
            return "Please enter between " + min + "and " + max + " Integer Characters for " + fieldName + ".\n";
        else
            return "Please enter " + min + " Integer Characters for "  + fieldName + ".\n";
    }
    if (isNaN(str) == true) {
        return "Please enter a valid Value for " + fieldName + ".\n";
    }        
    if (parseInt(str, 10) == 0) {
        return "Please enter a valid Value for " + fieldName + ".\n";
    }
    return "";
}

// Range Validation for Integer Tested
function IntRangeChk(name, fieldName, min, max ) {
    var value = document.getElementById(name).value;
    var checkValue = parseInt(value, 10);
    var str;
    if ((str = IntegerValidate(name, fieldName)) == "") 
    {        
        if ( checkValue < min || checkValue > max ) 
        {
            if (min != max)
                return "Please enter a Integer Value between " + min + " to " + max + " for " + fieldName + ". \n";
            else
                return "Please enter a Integer Value " + min + " for " + fieldName + ". \n";
        }
        else
            return "";
    }
    else
        return str;
}

// Range Validation for Float Tested
function FloatRangeChk(name, min, max, fieldName) {
    var value = document.getElementById(name).value;
    var str;
    if ((str = FloatValidate(name, fieldName)) == "") {
        checkValue = parseFloat(value);
        if (((min = parseFloat(min)) && checkValue < min) || ((max = parseFloat(max)) && checkValue > max))
            return "Please enter a Float Value between " + min + " to " + max + " for " + fieldName + ". \n";
        else
            return "";
    }
    else
        return str;
}

// MAC Address validation
function MacValidate(name, fieldName) {
    var str = document.getElementById(name).value;
    var objRegExp = /^([0-9a-f]{2}([:-]|$)){6}$|([0-9a-f]{4}([.]|$)){3}$/i;
    if (!objRegExp.test(str))
        return "Please enter valid " + fieldName + ".\n";
    else
        return "";
}

// Binary Validation
function BinaryValidate(name, fieldName, length) {
    var str = document.getElementById(name).value;
    var objRegExp = /^[0-1]{5}$/;
    if (!objRegExp.test(str))
        return "Please enter valid " + fieldName + ".\n";
    else
        return "";
}

// Integer Length Validation
function IntLengthValidate(name, fieldName, minlength, maxlength) {
    var str = document.getElementById(name).value;
    if (str.length >= minlength && str.length <= maxlength) {
        var objRegExp = /^[0-9]+$/;
        if (!objRegExp.test(str))
            return "Please enter valid " + fieldName + ".\n";
        else
            return "";
    }
    else {
        if (minlength != maxlength)
            return "Please enter a Integer Value of length " + minlength + " to " + maxlength + " for " + fieldName + ". \n";
        else
            return "Please enter a Integer Value of length " + minlength + " for " + fieldName + ". \n";
    }

}

// Name validation
function NameValidate(name, fieldName, minlength, maxlength) {
    var str = document.getElementById(name).value;
    if (str.length >= minlength && str.length <= maxlength) {
        var objRegExp = /^[a-zA-Z][a-zA-Z0-9-]*$/;
        if (!objRegExp.test(str))
            return "Please enter valid " + fieldName + ".\n";
        else
            return "";
    }
    else {
        if (minlength != maxlength)
            return "Please enter "+ fieldName +" of length between " + minlength + " to " + maxlength + ". \n";
        else
            return "Please enter " + fieldName + "  of length " + minlength + ". \n";
    }
}

// Alpha Numeric validation
function AlphaNumericValidate(name, fieldName, minlength, maxlength) {
    var str = document.getElementById(name).value;
    if (str.length >= minlength && str.length <= maxlength) {
        var objRegExp = /^[a-zA-Z][a-zA-Z0-9]*$/;
        if (!objRegExp.test(str))
            return "Please enter valid " + fieldName + ".\n";
        else
            return "";
    }
    else {
        if (minlength != maxlength)
            return "Please enter " + fieldName + " of length between " + minlength + " to " + maxlength + ". \n";
        else
            return "Please enter " + fieldName + "  of length " + minlength + ". \n";
    }
}

// Local Time Validation
function LocalTimeValidate(name, fieldName) 
{
    var str = document.getElementById(name).value;
    
    if (str == "auto-detect")
        return "";
    
    var year = 0;
    var month = 0; // YYYYMMDDHHNNSSTTT
    var day = 0;
    var hour = 0;
    var min = 0;
    var sec = 0;
    var msec = 0;
    if (str.length != 17) {
        return "Please enter valid " + fieldName + ".\n"; 
    }
    year  = parseInt(str.slice(0, 4), 10);
    month = parseInt(str.slice(4, 6), 10);
    day   = parseInt(str.slice(6, 8), 10);
    hour  = parseInt(str.slice(8, 10), 10);
    min   = parseInt(str.slice(10, 12), 10);
    sec   = parseInt(str.slice(12, 14), 10);
    msec  = parseInt(str.slice(14, 17), 10);
    if (year <= 0 || year >= 10000) {
        return "Please enter valid " + fieldName + ".\n";
    }
    else if (month <= 0 || month >= 13) {
        return "Please enter valid " + fieldName + ".\n";
    }
    else if (day <= 0 || day >= 32) {
        return "Please enter valid " + fieldName + ".\n";
    }
    else if (hour >= 24 || hour < 0) {
        return "Please enter valid " + fieldName + ".\n";
    }
    else if (min >= 60 || min < 0) {
        return "Please enter valid " + fieldName + ".\n";
    }
    else if (sec >= 60 || sec < 0) {
        return "Please enter valid " + fieldName + ".\n";
    }
    else if (msec >= 1000 || msec < 0) {
        return "Please enter valid " + fieldName + ".\n";
    }
    else if (year % 4 != 0 && month == 2 && day >= 29) {
        return "Please enter valid " + fieldName + ".\n";
    }
    else
        return "";
}

// UTC Time Validation
function UTCTimeValidate(name, fieldName) {
    var str = document.getElementById(name).value;
    if (str == "auto-detect")
        return "";
        
    var hour = 0; // HHNN
    var min = 0;

    if (str.length != 5 )
        return "Please enter valid " + fieldName + ".\n";
    
    hour = parseInt(str.slice(0, 3), 10);
    min = parseInt(str.slice(3, 5), 10);


    if (hour >= 14 || hour <= -13)
        return "Please enter valid " + fieldName + ".\n";
    else if (min >= 60 || min < 0)
        return "Please enter valid " + fieldName + ".\n";
    else
        return "";
}

function GpsLatitudeValidate(latitude, latitudeFieldName ) {
    var gpsLatitude = document.getElementById(latitude).value;
    if (gpsLatitude == "" || gpsLatitude == null)
        return "Please enter valid" + latitudeFieldName;
    var gpsArrayLatitude = gpsLatitude.split(".", 2);
    var latdegrees, latmins, latsecs, latpole;
       
    if (gpsLatitude.length < 11 || gpsArrayLatitude[1].length != 7)
        return "Please enter valid " + latitudeFieldName + ".\n";     
    
    /*LATIDUDE VALIDATION*/
    if (gpsArrayLatitude[0].length == 4) {
        latdegrees = parseInt(gpsArrayLatitude[0].substr(0, 2), 10);
        latmins = parseInt(gpsArrayLatitude[0].substr(2), 10);
    }
    else
    if (gpsArrayLatitude[0].length == 3) {
        latdegrees = parseInt(gpsArrayLatitude[0].substr(0, 1), 10);
        latmins = parseInt(gpsArrayLatitude[0].substr(1), 10);
    }
    else {
        return "Please enter valid " + latitudeFieldName + ".\n";
    }

    if (gpsArrayLatitude[1].length == 7) {
        latsecs = parseInt(gpsArrayLatitude[1].substr(0, 6), 10);
        latpole = gpsArrayLatitude[1].substr(6, 6);
    }
    else {
        return "Please enter valid " + latitudeFieldName + ".\n";
    }
    
    
    if (latdegrees < 0 || latdegrees > 89)
        return "Please enter valid " + latitudeFieldName + " Degree value between 0 to 89.\n";
    if (latmins < 0 || latmins > 59)
        return "Please enter valid " + latitudeFieldName + " Minutes value between 0 to 59.\n";
    if (latsecs < 0 || latsecs > 999999)
        return "Please enter valid " + latitudeFieldName + " Precision value between 0 to 999999 .\n";
    if (latpole != "N" && latpole != "S")
        return "Please enter valid " + latitudeFieldName + " Direction N/S.\n";

    return "";
}

function GpsLongitudeValidate(longitude, longitudeFieldName) {
    var gpsLongitude = document.getElementById(longitude).value;
    if (gpsLongitude == "" || gpsLongitude == null)
        return "Please enter valid" + longitudeFieldName;
    var gpsArrayLongitude = gpsLongitude.split(".", 2);
    var longdegrees, longmins, longsecs, longpole;
    /*LONGITUDE VALIDATION*/

    if (gpsLongitude.length < 11 || gpsArrayLongitude[1].length != 7)
        return "Please enter valid " + longitudeFieldName + " .\n";
        
    if (gpsArrayLongitude[0].length == 5) {
        longdegrees = parseInt(gpsArrayLongitude[0].substr(0, 3), 10);
        longmins = parseInt(gpsArrayLongitude[0].substr(3), 10);               
    }
    else
    if (gpsArrayLongitude[0].length == 4) {
        longdegrees = parseInt(gpsArrayLongitude[0].substr(0, 2), 10);
        longmins = parseInt(gpsArrayLongitude[0].substr(2), 10);
    }
    else
    if (gpsArrayLongitude[0].length == 3) {
        longdegrees = parseInt(gpsArrayLongitude[0].substr(0, 1), 10);
        longmins = parseInt(gpsArrayLongitude[0].substr(1), 10);
    }
    else
    {
        return "Please enter valid " + longitudeFieldName + ".\n";
    }
    
    if (gpsArrayLongitude[1].length == 7) {
        longsecs = parseInt(gpsArrayLongitude[1].substr(0, 6), 10);
        longpole = gpsArrayLongitude[1].substr(6, 6);
    }
    else {
        return "Please enter valid " + longitudeFieldName + ".\n";
    }
    //longsecs = parseInt(gpsArrayLongitude[1].substr(0, 6));
    //longpole = gpsArrayLongitude[1].substr(6, 6);

    if (longdegrees < 0 || longdegrees > 179)
        return "Please enter valid " + longitudeFieldName + " Degree value between 0 to 179.\n";
    if (longmins < 0 || longmins > 59)
        return "Please enter valid " + longitudeFieldName + " Minutes value between 0 to 59.\n";
    if (longsecs < 0 || longsecs > 999999)
        return "Please enter valid " + longitudeFieldName + " Precision value between 0 to 999999 .\n";
    if (longpole != "E" && longpole != "W")
        return "Please enter valid " + longitudeFieldName + " Direction E/W.\n";
    
    return "";
}