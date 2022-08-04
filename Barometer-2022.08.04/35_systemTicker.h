// ======================================================================
//  Standard system Ticker <<Do not change>>
// ======================================================================
// ======================================================================
//  Time synchronisation part
// ======================================================================

// ***** interesting variables below : dt datetime , bool timerUpdateSignal, bool timeServerConnected, int firstTimeUpdateAfterConnect, tm tmstruct

struct dt                       // structure for variable datetime below which holds this data
{
  String year;
  String month;
  String day;
  String hour;      // used in readbme280 to check if the hour changed and data needs to move
  String minute;    // used in readbme280 to check if the minute changed and data needs to move
  String second;
  String dayname;   // one of daynames[][10] below
  String CDT;       // dayname year-month-day hour:minute:second  <- used on the web pages with the graphs
};

// Next is used to fill datetime.dayname

static const char daynames[][10] PROGMEM = {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
// for Dutch ;-) static const char daynames[][10] PROGMEM = {"Zondag","Maandag","Dinsdag","Woensdag","Donderdag","Vrijdag","Zaterdag"};

// the dt structure fields like datetime.CDT can be used anywhere in code

struct dt datetime;

// Next true when connected, false when not connected

bool timeServerConnected = false ;

// next is set to 1 after connect ( values : 0, 1, max 2, checked by main loop for 1 and set to 2 to log and send notification on first time sync after (re-)connect )

int firstTimeUpdateAfterConnect = 0 ;


bool time_zone_set = false;     // is set to true after first call to updateTime() while WiFi is connected so we set time zone only once

struct tm tmstruct {};          // contains int for : tm_sec, tm_min, tm_hour, tm_mday, tm_mon, tm_year ( years since 1900 ) ,tm_wday ( days since sunday 0,.6) , tm_yday (0..365), tm_isdst ( <0=?, 0=no, >0=yes)

// ======================================================================
//  system ticker part
// ======================================================================

float systemTickerInterval = 1 ;  // run system Ticker every second

// Settings to run the timeTickerRoutine() routine

bool timerUpdateSignal = false; // set by systemTickerRoutine() each second and checked and reset by main loop which calls updateTime() when signaled

// check if we want a reboot for some reason

int rebootRequestCounter = 0;     // set to higher value by : WiFi setup page ; app setup page ;
String rebootRequestReason;       // when rebootRequestCounter set to higher value, this needs to be set to reason
// and run a counter to be reported by serve_json

int loopCounter = 0;              // loops between -10000 and +10000
