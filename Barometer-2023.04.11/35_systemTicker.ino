// ======================================================================
//  Standard system Ticker <<Do not change>>
// ( lcl_* is local to this tab and not in *prototypes.h files )
// ======================================================================

// >>>>> lcl_getLocalTime() is called by updateTime()  just below

bool lcl_getLocalTime(struct tm * info, uint32_t maxduration) {

// get local time which is syncronised with Internet by internal ntp client

  const uint32_t interval = 100;                // 100 milliseconds delay per attempt

  uint32_t attempts = maxduration / interval;   // the number of attempts we have

  time_t now;                                   // now of type time _t :: type representing the number of seconds elapsed since 00:00 hours, Jan 1, 1970 UTC (i.e., a unix timestamp)

  while (attempts--) {
    time(&now);                                 // get the time (The value returned generally represents the number of seconds since 00:00 hours, Jan 1, 1970 UTC)
    localtime_r(&now, info);                    // The localtime function converts the time stamp pointed to by now into broken-down time info, expressed as Local time
    if (info->tm_year > (2000 - 1900)) {        // info is type tm and contains tm_year since 1900 so we check if the date is after the year 2000 ;-)
      if ( ( firstTimeUpdateAfterConnect == 0 ) and timeServerConnected ) {firstTimeUpdateAfterConnect = 1 ;}
      return true;
    }
    Serial.print(F("."));
    delay(interval);                            // <-- note delay is not supported within Ticker and causes us to do it via the loop-Ticker construction
  }
  Serial.println(F("lcl_getLocalTime error"));
  return false;
}

// >>>>> updateTime() below calls lcl_getLocalTime() just above
// >>>>> updateTime() below is called by loop when loop is triggered by systemTickerRoutine()  ( see further below )


void updateTime() {

// updateTime() runs every 1 second so if we are connected to WiFi it can set the sta_IP variable which is shown on the WiFi manager page serve_WiFi_info and serve_json

  sta_IP = IpAddress2String(WiFi.localIP());

  if (WiFi.status() == WL_CONNECTED) {
    if ( ! timeServerConnected ) {
      Serial.println(F("Contacting Time Server for time zone ")+time_zone);
      configTime(time_zone.c_str(), "time.nist.gov","0.pool.ntp.org", "1.pool.ntp.org");
      timeServerConnected = true;
      time_zone_set = true;
    }
  } else { // we have no WiFI connection and are fully depending on the internal clock which is not (yet) syncing.
    timeServerConnected = false;
    firstTimeUpdateAfterConnect = 0;
    Serial.println(F("No WiFi ==> No NTP sync, running on clock ! " )+datetime.CDT);
  }

//  if (! timeServerConnected ) { firstTimeUpdateAfterConnect = 0;}

// when the timezone is set ntp synced at least once and the internal clock runs fine

  if (time_zone_set) {
    tmstruct.tm_year = 0;
    while (! lcl_getLocalTime(&tmstruct, 5000)) {} ;

    datetime.year    = String(tmstruct.tm_year + 1900);
    datetime.month   = F("00");  datetime.month.concat(String(tmstruct.tm_mon + 1)); datetime.month  = datetime.month.substring(  datetime.month.length()  - 2 );
    datetime.day     = F("00");  datetime.day.concat(String(tmstruct.tm_mday));      datetime.day    = datetime.day.substring(    datetime.day.length()    - 2 );
    datetime.hour    = F("00");  datetime.hour.concat(String(tmstruct.tm_hour));     datetime.hour   = datetime.hour.substring(   datetime.hour.length()   - 2 );
    datetime.minute  = F("00");  datetime.minute.concat(String(tmstruct.tm_min));    datetime.minute = datetime.minute.substring( datetime.minute.length() - 2 );
    datetime.second  = F("00");  datetime.second.concat(String(tmstruct.tm_sec));    datetime.second = datetime.second.substring( datetime.second.length() - 2 );
    datetime.dayname = String(daynames[tmstruct.tm_wday]);
    datetime.CDT = datetime.dayname;
    datetime.CDT.concat(F(" "));
    datetime.CDT.concat(datetime.year);
    datetime.CDT.concat(F("-"));
    datetime.CDT.concat(datetime.month);
    datetime.CDT.concat(F("-"));
    datetime.CDT.concat(datetime.day);
    datetime.CDT.concat(F(" "));
    datetime.CDT.concat(datetime.hour);
    datetime.CDT.concat(F(":"));
    datetime.CDT.concat(datetime.minute);
    datetime.CDT.concat(F(":"));
    datetime.CDT.concat(datetime.second);
//    Serial.println(F("Local time : ")+datetime.CDT);
  } else {
    Serial.println(F("No time sync, No time zone set"));
  }
}

// ======================================================================
//  Standard System Timer systemTickerRoutine() <<Do not change>>
// ======================================================================

// Settings to run the systemTickerRoutine() routine

//**Ticker systemTicker;              // Ticker object to run systemTickerRoutine() we create further below
//**float systemTickerInterval = 1 ;  // run system Ticker every second

// first thing is to check network connection

bool lcl_startToConnect = true;

bool softAPRunning = true;
Pinger lcl_pinger;                    // object to ping default gateway to check if we have connection
int lcl_pingCount;
int lcl_max_pingCount = 300;           // after reboot or WiFi lost try to connect every 300 seconds;


// he he, finally the routine itself

void systemTickerRoutine()  {

  flip_LED();

// first thing is to check network connection

  if (lcl_startToConnect) {
    readConfig();
    if (sta_ssid == F("SSID")) {
      Serial.println(F("No SSID set yet"));
    } else {
      Serial.println(F("Connecting to : ")+sta_ssid+F(" ")+sta_pwd+F(" as ")+sta_hostname);
      WiFi.disconnect();
      WiFi.hostname(sta_hostname);
      WiFi.begin(sta_ssid, sta_pwd);
      lcl_startToConnect = false;
      lcl_pingCount = 0;

      timeServerConnected = false;
      firstTimeUpdateAfterConnect = 0;
    }
  } else {
    String gateway = IpAddress2String(WiFi.gatewayIP());
    lcl_pingCount += 1;
    if ( gateway == F("0.0.0.0")) {                     // we have no connection, stop WiFi to test
      Serial.println(F("We have no default gateway... ")+String(lcl_pingCount));
    } else {                                            // verify if we can reach gateway
      if ( lcl_pinger.Ping(WiFi.gatewayIP())) {
// we have a network connection
        lcl_pingCount = 0 ;
// fire time sync
        timerUpdateSignal = true;
      };
    }
    lcl_startToConnect = lcl_pingCount >= lcl_max_pingCount;
  }

// check if we want a reboot for some reason

  if ( rebootRequestCounter > 0) {
    rebootRequestCounter = rebootRequestCounter - 1;
    Serial.println(F("rebootRequestCounter : ")+String(rebootRequestCounter));
    if ( rebootRequestCounter == 1) {
      saveAppData();
    }
    if (rebootRequestCounter == 0) {
      log(F("Reboot request : ")+rebootRequestReason);
      ESP.restart();
    }
  }

// and run a counter to be reported by serve_json

  loopCounter += 1; 
  
  if (loopCounter == 900 && softAPRunning) { log(F("Shutdown Access Point") ) ; WiFi.softAPdisconnect(true) ; softAPRunning = false ; };

  if (loopCounter == 10000) {loopCounter = -10000 ; };

  flip_LED();

};
