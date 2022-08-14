// ======================================================================
//  Standard setup and loop <<Do not change>>
// ======================================================================
void std_setup() {
  
// ---- Start serial connection

  Serial.begin(115200);

  delay(1000); // allow hardware init to finisch before we start

  Serial.println(F("\n\n---------- Start up Begin\n"));

// ---- Enable blinking of LED and show we are starting up

  pinMode(LED_BUILTIN, OUTPUT);
  for (int i = 10; i >= 0; i--) {
    delay(200);
    flip_LED(F("startup ")+String(i));
  }

// ---- First we setup the file system

  Serial.println(F("\nMount LittleFS"));
  setupLittleFS();
  Serial.println("");
  log(F("--------------------"));
  log(F("Reboot reason : ")+ESP.getResetReason());

// ---- Read configuration and app data
  
  listDir("/");
  readConfig();
  readAppData();

  demomode =  (sta_hostname == F("newdevice"))
//           or (sta_hostname == F("demomode"))
           or (sta_hostname == F("test")) ;
           
// ---- Show system info at startup

  sysinfo(F("Startup"));

// ---- Scanning for SSIDs needs to be done while network is down so.....
// ---- First we build a list with available SSID's which we need on the WiFi manager page

  build_sta_ssids();

// ---- Start Access Point, get IP, change ap_ssid to include hostname and IP, restart Access Point

  WiFi.softAP(ap_ssid, ap_pwd);
  ap_IP = IpAddress2String(WiFi.softAPIP());
  ap_ssid = sta_hostname + " " + ap_IP;
  WiFi.softAP(ap_ssid, ap_pwd);

// ----- Start the standard system Ticker

  systemTicker.attach(systemTickerInterval, systemTickerRoutine);

// ---- Mail client stuff ***** 

  /** Enable the debug via Serial port
   * none debug or 0
   * basic debug or 1
  */
  smtp.debug(0);

  /* Set the callback function to get the sending results */
// I use none so see the examples if you want one. (like File > Examples > ESP Mail Client > SMTP > Custom Client )
// maybe put the next in your 20_setup_loop
//  smtp.callback(smtpCallback);

// setup the ftp server with username and password

// 1 ftp user
//  ftpServer.begin(sta_hostname,app_password);                                            
// 2 ftp users
  ftpServer.begin(sta_hostname,app_password,ftp_emergency_usr,ftp_emergency_pwd);  

// enable write access for ftp user 2 which is default disabled
  ftpServer.FTPUser2WriteAccess = true; // allow second user to save/delete/rename files

// ---- Webserver stuff

  WEBserver.begin();

  WEBserver.onNotFound(notFound);                                                                                           // activated when wrong page entered in url

  AsyncElegantOTA.begin(&WEBserver,sta_hostname.c_str(),app_password.c_str());                                              // Start ElegantOTA on url http://any_ip/update

  WEBserver.on("/start_ota",    HTTP_GET, [](AsyncWebServerRequest *request){ serve_start_ota(  request ); } );             // redirects to /update after giving message and waiting 10 seconds

  WEBserver.on("/",             HTTP_GET, [](AsyncWebServerRequest *request){ serve_home(      request, F("normal")); } );  // uses virtual host to show AP or STA home page

  WEBserver.on("/WiFi_mgr",     HTTP_GET, [](AsyncWebServerRequest *request){ serve_WiFi_mgr(  request); } );               // from / on AP interface|setup_system on STA 

  WEBserver.on("/WiFi_info",    HTTP_POST,[](AsyncWebServerRequest *request){ serve_WiFi_info( request, F("save")); } );    // from / on AP interface|WiFi_mgr AP-STA

  WEBserver.on("/json",         HTTP_GET, [](AsyncWebServerRequest *request){ serve_json(      request); } );               // from / on STA interface

  WEBserver.on("/log",          HTTP_GET, [](AsyncWebServerRequest *request){ serve_log(       request); } );               // from / on STA interface

  WEBserver.on("/setup",        HTTP_GET, [](AsyncWebServerRequest *request){ serve_setup(       request); } );             // from / on STA interface
  WEBserver.on("/setup_app",    HTTP_GET, [](AsyncWebServerRequest *request){ serve_setup_app( request, F("setup")); } );   // from /setup        on STA interface
  WEBserver.on("/setup_app",    HTTP_POST,[](AsyncWebServerRequest *request){ serve_setup_app( request, F("data")); } );    // from /setup_app    on STA interface
  WEBserver.on("/setup_system", HTTP_GET, [](AsyncWebServerRequest *request){ serve_setup_system( request, F("setup")); } );// from /setup        on STA interface
  WEBserver.on("/setup_system", HTTP_POST,[](AsyncWebServerRequest *request){ serve_setup_system( request, F("data")); } ); // from /setup_system on STA interface

  WEBserver.on("/change_variable", HTTP_GET, [](AsyncWebServerRequest *request){ serve_change_variable( request ); } );  // change internal APP variable values

  Serial.println(F("\n---------- Start up End\n"));

}

void std_loop() {
  
// ---- Call updateTime() for each timerUpdateSignal set by the timeTicker.

  if (timerUpdateSignal) { timerUpdateSignal = false;   updateTime(); }

// ---- Send notification after First time sync after WiFi (re-)connect

  if ( firstTimeUpdateAfterConnect == 1 ) {           // firstTimeUpdateAfterConnect is set to 1 after first successful time update after (re-)connecting to WiFi

    firstTimeUpdateAfterConnect = 2 ;                 // block this part so it skipped in the loop

    if (boottime == "" ){boottime = datetime.CDT ;};  // note boottime, reported on json page

// ***** Create notification

    String notification = F("Time sync");

    String status_txt = sendMail(F("Time sync"), datetime.CDT + " " + notification);

    log( notification+" ; "+status_txt);

    listDir("/");

  }
}
