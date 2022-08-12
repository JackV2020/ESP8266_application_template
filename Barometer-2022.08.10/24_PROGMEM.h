// ======================================================================
//  Standard web page content all in PROGMEM be carefull to change.....
//  Keep your own PROGMEM in the other tab
// ======================================================================

// https://developer.mozilla.org/en-US/docs/Web/CSS/gradient/radial-gradient
// https://www.gradientmagic.com/

// ======================================================================
// Header for web pages on Access Point interface with WiFi Manager
// ======================================================================

// for 'input[type=text] + span {' etc below see :
// https://stackoverflow.com/questions/60503161/how-to-force-title-attribute-to-show-even-when-is-input-active-clicked

static const char ap_Header[] PROGMEM = R"====(
REFRESH
<meta name="viewport" content="width=device-width,height=device-height, initial-scale=1">
<style>html{text-align: center;}
h1{color: #0F3376; padding: 2vh;}
p{font-size: 1.5rem}
input[type="text"]{text-align: center;border-radius:15px;-moz-border-radius:15px;-webkit-border-radius:15px;}
body {margin: 0; line-height: 80%; height: 100vh; background: radial-gradient(circle at 90%, #888, #aaa 20%,  #888 50%, #eee 75%, #555 75%, #eee 76%); }
#bottom {position: absolute;bottom: 0;width: 100%;}
.rnd_btn {  background-color: lightgrey; border-radius: 50%; border-width : 3; border-color: silver; color: blue; width: 100px; height: 50px; text-align: center}
input[type=text] + span {
  display: none;
}
input[type=text]:focus + span {
  display: inline;
}
input[type=password] + span {
  display: none;
}
input[type=password]:focus + span {
  display: inline;
}
</style></head><body>
<div id="main">
<br><br><br><br><p>WiFi Manager</p>
)====";

// ======================================================================
// Header for web pages on Station interface with full application
// ======================================================================

static const char sta_Header[] PROGMEM = R"====(
<html><head>
<meta name="viewport" content="width=device-width,height=device-height, initial-scale=1">
<style>html{text-align: center;}
h1{color: #0F3376; padding: 2vh;}
p{font-size: 1.5rem}
input[type="text"]{text-align: center;border-radius:15px;-moz-border-radius:15px;-webkit-border-radius:15px;}
body {margin: 0; line-height: 80%; height: 100vh; background: 
  radial-gradient(circle at 90% 60%,rgb(255,255,100) , rgb(255,255,160) 20%,  rgb(255,255,100) 30%, rgb(255,255,180) 40%,
  rgb(255,255,100) 50%, rgb(255,255,200) 60%, rgb(255,215,0, 0.5) 70%, gold 75% 3px , black 76%); }
.rnd_btn {  background-color: lightgrey; border-radius: 50%; border-width : 3; border-color: gold; color: blue; width: 100px; height: 50px; text-align: center}
input[type=text] + span {
  display: none;
}
input[type=text]:focus + span {
  display: inline;
}
input[type=password] + span {
  display: none;
}
input[type=password]:focus + span {
  display: inline;
}
</style></head><body id="theBody">
<div id="main">
<br><br><br><br>
)====";

// ======================================================================
//  No changes below
// ======================================================================
// ======================================================================
//  Web response types <<Do not change>>
// ======================================================================

// a response to the client is either plain text or html

static const char textplain[] PROGMEM = "text/plain"; //  only used for the response notFound
static const char texthtml[] PROGMEM = "text/html";


// ======================================================================
//  WiFi Manager page <<Do not change>>
// ======================================================================

// ***** (If you want support for WiFi without password you remove required from the password field)
//  <br><br><input type="password" name="sta_pwd" required id="sta_pwd" value="" placeholder="Password">
//  <br><br><input type="password" name="sta_pwd"          id="sta_pwd" value="" placeholder="Password">

static const char WiFi_Manager[] PROGMEM = R"====(
APP_VERSION
<br><br>
<form action="/WiFi_info" method="POST">
<button class="rnd_btn" style="position:fixed; top:20; left:5;">Save</button>
<button class="rnd_btn" style="position:fixed; top:100; left:5;" name="reboot" value="yes">Refresh</button>
DROPDOWNSSIDS
<br><br><input type="password" name="sta_pwd"          id="sta_pwd" value="" placeholder="Password">
<span><br><br>WiFi password :<br><br>Leave empty to not change.<br></span>
<br><input type="checkbox" onclick="toggleShowPwdField('sta_pwd')">Show Password
<br><br><input type="text" name="sta_hostname" required pattern="[a-z0-9_]{4,20}" value="STAHOSTNAME"placeholder="hostname"><span><br><br>hostname 4 to 20<br><br>lower case letters+digits+underscores</span>
<br><input type="text" id="dummy" name="dummy" style="height: 0px;width: 0px;border:none;background-color:grey;"  >
<br><br><input type="password" name="app_password" id="app_password" required placeholder="APP_PASSWORD_PLACEHOLDER">
<span><br><br>App password to Save settings :<br><br>Can be changed on System Setup page.<br></span>
<br><input type="checkbox" onclick="toggleShowPwdField('app_password')">APP_SHOW_PASSWORD_LABEL
</form>
</div></center>
<script>
function toggleShowPwdField(what) {
var x = document.getElementById(what);
if (x.type === "password")
{x.type = "text";}
else {x.type = "password";}}
</script></body></html>
)====";

// ======================================================================
//  Station interface home page with buttons <<Do not change>>
// ======================================================================

static const char sta_HomePage[] PROGMEM = R"====(
<br><br>
<button class="rnd_btn" STYLE onclick="window.location.href = '/app';" >APPNAME</button>
<br><br><br><button class="rnd_btn" onclick="window.location.href = '/json';" >JSON</button>
<br><br><br><button class="rnd_btn" onclick="window.location.href = '/log';" >Log File</button>
<br><br><br><button class="rnd_btn" onclick="window.location.href = '/setup';" >Setup</button>
</div></body></html>
)====";

// ======================================================================
//  Station interface setup page with buttons <<Do not change>>
// ======================================================================

static const char sta_setup[] PROGMEM = R"====(
<p>Setup</p><br>
<button class="rnd_btn" STYLE onclick="window.location.href = '/setup_app';" >APPNAME</button>
<br><br><br><button class="rnd_btn" onclick="window.location.href = '/setup_system';" >System</button>
<br><br><br><button class="rnd_btn" onclick="window.location.href = '/start_ota';" >ElegantOTA</button>
<br><br><br><button class="rnd_btn" onclick="window.location.href = '/WiFi_mgr';" >WiFi Setup</button>
</div></body></html>
)====";

// ======================================================================
//  Reboot info page <<Do not change>>
// ======================================================================

static const char rebootPage[] PROGMEM = R"====(
<p id="timer">We need a restart<br><br>Please wait....<br><br>30 seconds</p>
<script>
var cd = new Date().getTime()+30000;
var x = setInterval(function() {
var now = new Date().getTime();
var df = cd - now;
var sec = Math.floor(df / 1000) ;
document.getElementById("timer").innerHTML = "We need a restart<br><br>Please wait....<br><br>" + sec + " seconds";
if (df < 0) {
clearInterval(x);
window.location.href = 'http://STA_HOSTNAME/';
}
}, 1000);
</script></body></html>
)====";


// ======================================================================
//  System setup page <<Do not change>> except ##### note below
// ======================================================================

//  ***** activate/deactivate the line after TIMEZONESELECT below to add/remove droppoint functionality
// Activating this is enough. It is already everywhere in the code.

static const char SetupSystem[] PROGMEM = R"====(
<p>STATUS_TXT</p>
<form action="/setup_system" method="POST">
<button class="rnd_btn" style="position:fixed; top:20; left:5" name="savesystemsetup" value="yes">Save</button>
<input type="checkbox" id="email_enabled" name="email_enabled" value="yes" M_ENACHECKED>Enable Mail
<br><br><input type="text" id="email_to" name="email_to" placeholder="Mail recipients" value=EMAIL_TO>
<span><br><br>Mail recipients :<br><br>to1@x.com(;to2@y.com(;...))</span>
<br><br><input type="text" id="smtp_server" name="smtp_server" placeholder="Mail host" value=SMTP_SERVER>
<span><br><br>Mail host :<br><br>smtp.gmail.com</span>
<br><br><input type="text" id="smtp_port" name="smtp_port" placeholder="Mail smtp port" value=SMTP_PORT>
<span><br><br>Mail smtp port :<br><br>465</span>
<br><br><input type="text" id="email_login" name="email_login" placeholder="Mail login" value=EMAIL_LOGIN>
<span><br><br>Mail login :<br><br>Sender@x.com</span>
<br><br><input type="password" name="email_password" id="email_password" placeholder="Mail password">
<span><br><br>Mail password :<br><br>Leave empty to not change.<br></span>
<br><input type="checkbox" onclick="toggleShowPwdField('email_password')">Show Password
<br><br><input type="text" id="email_link" name="email_link" placeholder="Link in mail" value=EMAIL_LINK>
<span><br><br>Browser addres for link in mail :<br><br>Leave empty for LAN access only.<br><br>When you have port forwarding<br><br>on your Internet router you may use :<br><br>http://InternetAddress:port/[app]</span>
<br><br>TIMEZONESELECT
<!-- <br><br><input type="text" id="droppoint" name="droppoint" placeholder="https://srv:443/droppoint/droppoint.php" value=DROPPOINT> -->
)====";


// ======================================================================
//  Setup pages bottom <<Do not change>>
// ======================================================================

// needs to be followed by </body></html>

// the dummy is a trick to avoid a popup when entering the last field before that which is caused by the password type of 'app_password' below

static const char SetupBottom[] PROGMEM = R"====(
<br><br><br><input type="text" id="dummy" name="dummy" style="height: 0px;width: 0px;border:none;"  >
<input type="password" name="app_password" id="app_password" required placeholder="APP_PASSWORD_PLACEHOLDER">
<span><br><br>App password to Save settings :<br><br>Can be changed on System Setup page.<br></span>
<br><input type="checkbox" onclick="toggleShowPwdField('app_password')">APP_SHOW_PASSWORD_LABEL
</form>
<script>function toggleShowPwdField(what) {
  var x = document.getElementById(what);
  if (x.type === "password") {
    x.type = "text";
  } else {
    x.type = "password";
  }
}
</script>
)====";


// ======================================================================
//  Time zone select value for system setup page
// ======================================================================

// Be carefull and think of your heap space !
// A complete list is in the comments just below........

// Only 1 value entry per timezone is supported to avoid the string getting too big
// Values below are fixed. Names between > and < are free
// Supported values with standard names are available in :

// https://github.com/esp8266/Arduino/blob/master/cores/esp8266/TZ.h

static const char timezoneselect[] PROGMEM = R"====(
<select style="width: 200px;" name="time_zone" title="Select a time zone" id="time_zone">
<optgroup label="Select a timezone">
<option value="CET-1CEST,M3.5.0,M10.5.0/3">Amsterdam (CET-1CEST)</option>
<option value="<-03>3">Buenos_Aires (<-03>3)</option>
<option value="EET-2">Cairo (EET-2)</option>
<option value="CAT-2">Cape_Town (CAT-2)</option>
<option value="AST4">Curacao (AST4)</option>
<option value="GMT0">Greenwich (GMT0)</option>
<option value="SAST-2">Johannesburg (SAST-2)</option>
<option value="MSK-3">Moscow (MSK-3)</option>
<option value="EST5EDT,M3.2.0,M11.1.0">New_York (EST5EDT)</option>
<option value="CST-8">Shanghai (CST-8)</option>
<option value="<+08>-8">Singapore (<+08>-8)</option>
<option value="JST-9">Tokyo (JST-9)</option>
<option value="<+00>0<+02>-2,M3.5.0/1,M10.5.0/3">Troll (<+00>0<+02>-2)</option>
<option value="<+09>-9">Yakutsk (<+09>-9)</option>
</optgroup></select>
)====";

// ..... the complete list

/*
static const char timezoneselect[] PROGMEM = R"====(
<select style="width: 200px;" name="time_zone" title="Select a time zone" id="time_zone">
<optgroup label="Select a timezone">
<option value="HST10HDT,M3.2.0,M11.1.0">Adak (HST10HDT)</option>
<option value="ACST-9:30ACDT,M10.1.0,M4.1.0/3">Adelaide (ACST-9:30ACDT)</option>
<option value="CET-1">Algiers (CET-1)</option>
<option value="<+06>-6">Almaty (<+06>-6)</option>
<option value="EET-2EEST,M3.5.4/24,M10.5.5/1">Amman (EET-2EEST)</option>
<option value="CET-1CEST,M3.5.0,M10.5.0/3">Amsterdam (CET-1CEST)</option>
<option value="AKST9AKDT,M3.2.0,M11.1.0">Anchorage (AKST9AKDT)</option>
<option value="<+13>-13<+14>,M9.5.0/3,M4.1.0/4">Apia (<+13>-13<+14>)</option>
<option value="<-04>4<-03>,M10.1.0/0,M3.4.0/0">Asuncion (<-04>4<-03>)</option>
<option value="EET-2EEST,M3.5.0/3,M10.5.0/4">Athens (EET-2EEST)</option>
<option value="NZST-12NZDT,M9.5.0,M4.1.0/3">Auckland (NZST-12NZDT)</option>
<option value="<+07>-7">Bangkok (<+07>-7)</option>
<option value="EET-2EEST,M3.5.0/0,M10.5.0/0">Beirut (EET-2EEST)</option>
<option value="<+11>-11">Bougainville (<+11>-11)</option>
<option value="AEST-10">Brisbane (AEST-10)</option>
<option value="<-03>3">Buenos_Aires (<-03>3)</option>
<option value="EET-2">Cairo (EET-2)</option>
<option value="CAT-2">Cape_Town (CAT-2)</option>
<option value="<-01>1">Cape_Verde (<-01>1)</option>
<option value="<-04>4">Caracas (<-04>4)</option>
<option value="<+01>-1">Casablanca (<+01>-1)</option>
<option value="<+1245>-12:45<+1345>,M9.5.0/2:45,M4.1.0/3:45">Chatham (<+1245>-12:45<+1345>)</option>
<option value="CST6CDT,M3.2.0,M11.1.0">Chicago (CST6CDT)</option>
<option value="MST7MDT,M4.1.0,M10.5.0">Chihuahua (MST7MDT)</option>
<option value="EET-2EEST,M3.5.0,M10.5.0/3">Chisinau (EET-2EEST)</option>
<option value="<+0630>-6:30">Cocos (<+0630>-6:30)</option>
<option value="<+0530>-5:30">Colombo (<+0530>-5:30)</option>
<option value="CST6">Costa_Rica (CST6)</option>
<option value="AST4">Curacao (AST4)</option>
<option value="EET-2EEST,M3.5.5/0,M10.5.5/0">Damascus (EET-2EEST)</option>
<option value="EAT-3">Dar_es_Salaam (EAT-3)</option>
<option value="ACST-9:30">Darwin (ACST-9:30)</option>
<option value="MST7MDT,M3.2.0,M11.1.0">Denver (MST7MDT)</option>
<option value="IST-1GMT0,M10.5.0,M3.5.0/1">Dublin (IST-1GMT0)</option>
<option value="<-06>6<-05>,M9.1.6/22,M4.1.6/22">Easter (<-06>6<-05>)</option>
<option value="<+13>-13">Enderbury (<+13>-13)</option>
<option value="<+0845>-8:45">Eucla (<+0845>-8:45)</option>
<option value="<+12>-12<+13>,M11.2.0,M1.2.3/99">Fiji (<+12>-12<+13>)</option>
<option value="<-06>6">Galapagos (<-06>6)</option>
<option value="<-09>9">Gambier (<-09>9)</option>
<option value="EET-2EEST,M3.4.4/48,M10.4.4/49">Gaza (EET-2EEST)</option>
<option value="<-03>3<-02>,M3.5.0/-2,M10.5.0/-1">Godthab (<-03>3<-02>)</option>
<option value="GMT0">Greenwich (GMT0)</option>
<option value="ChST-10">Guam (ChST-10)</option>
<option value="AST4ADT,M3.2.0,M11.1.0">Halifax (AST4ADT)</option>
<option value="CST5CDT,M3.2.0/0,M11.1.0/1">Havana (CST5CDT)</option>
<option value="HKT-8">Hong_Kong (HKT-8)</option>
<option value="HST10">Honolulu (HST10)</option>
<option value="WIB-7">Jakarta (WIB-7)</option>
<option value="EST5">Jamaica (EST5)</option>
<option value="WIT-9">Jayapura (WIT-9)</option>
<option value="IST-2IDT,M3.4.4/26,M10.5.0">Jerusalem (IST-2IDT)</option>
<option value="SAST-2">Johannesburg (SAST-2)</option>
<option value="<+0430>-4:30">Kabul (<+0430>-4:30)</option>
<option value="<+12>-12">Kamchatka (<+12>-12)</option>
<option value="<+0545>-5:45">Kathmandu (<+0545>-5:45)</option>
<option value="WAT-1">Kinshasa (WAT-1)</option>
<option value="<+14>-14">Kiritimati (<+14>-14)</option>
<option value="IST-5:30">Kolkata (IST-5:30)</option>
<option value="<-05>5">Lima (<-05>5)</option>
<option value="WET0WEST,M3.5.0/1,M10.5.0">Lisbon (WET0WEST)</option>
<option value="GMT0BST,M3.5.0/1,M10.5.0">London (GMT0BST)</option>
<option value="<+1030>-10:30<+11>-11,M10.1.0,M4.1.0">Lord_Howe (<+1030>-10:30<+11>-11)</option>
<option value="PST8PDT,M3.2.0,M11.1.0">Los_Angeles (PST8PDT)</option>
<option value="WITA-8">Makassar (WITA-8)</option>
<option value="<-0930>9:30">Marquesas (<-0930>9:30)</option>
<option value="<+04>-4">Mauritius (<+04>-4)</option>
<option value="AEST-10AEDT,M10.1.0,M4.1.0/3">Melbourne (AEST-10AEDT)</option>
<option value="CST6CDT,M4.1.0,M10.5.0">Mexico_City (CST6CDT)</option>
<option value="<-03>3<-02>,M3.2.0,M11.1.0">Miquelon (<-03>3<-02>)</option>
<option value="MSK-3">Moscow (MSK-3)</option>
<option value="EST5EDT,M3.2.0,M11.1.0">New_York (EST5EDT)</option>
<option value="<-11>11">Niue (<-11>11)</option>
<option value="<+11>-11<+12>,M10.1.0,M4.1.0/3">Norfolk (<+11>-11<+12>)</option>
<option value="SST11">Pago_Pago (SST11)</option>
<option value="AWST-8">Perth (AWST-8)</option>
<option value="MST7">Phoenix (MST7)</option>
<option value="<-08>8">Pitcairn (<-08>8)</option>
<option value="<+10>-10">Port_Moresby (<+10>-10)</option>
<option value="KST-9">Pyongyang (KST-9)</option>
<option value="<+03>-3">Qatar (<+03>-3)</option>
<option value="<-04>4<-03>,M9.1.6/24,M4.1.6/24">Santiago (<-04>4<-03>)</option>
<option value="<-01>1<+00>,M3.5.0/0,M10.5.0/1">Scoresbysund (<-01>1<+00>)</option>
<option value="CST-8">Shanghai (CST-8)</option>
<option value="<+08>-8">Singapore (<+08>-8)</option>
<option value="<-02>2">South_Georgia (<-02>2)</option>
<option value="NST3:30NDT,M3.2.0,M11.1.0">St_Johns (NST3:30NDT)</option>
<option value="<-10>10">Tahiti (<-10>10)</option>
<option value="<+05>-5">Tashkent (<+05>-5)</option>
<option value="<+0330>-3:30<+0430>,J79/24,J263/24">Tehran (<+0330>-3:30<+0430>)</option>
<option value="JST-9">Tokyo (JST-9)</option>
<option value="<+00>0<+02>-2,M3.5.0/1,M10.5.0/3">Troll (<+00>0<+02>-2)</option>
<option value="<+09>-9">Yakutsk (<+09>-9)</option>
</optgroup></select>
)====";
*/
