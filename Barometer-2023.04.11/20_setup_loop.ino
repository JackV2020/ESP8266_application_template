// ======================================================================
//  Your  setup and loop sections
// ======================================================================

void app_setup(){

//  demomode =  demomode or (sta_hostname == F("barometerburo"));

  Serial.println(F("\n ----- Start app setup"));

// WATCH IT !! Do not change "/app" because this is activated from the app button on the standard home page.
// You can change the routine name which in this case is serve_barometer and it located in 13_web
 
  WEBserver.on("/app", HTTP_GET, [](AsyncWebServerRequest *request){ serve_barometer( request ); } );
  
// Just some test urls to show how this works. You need to create the content via FTP and enter the urls in your browser like http://barometer/testgif

  WEBserver.on("/testjpg", HTTP_GET, [](AsyncWebServerRequest *request){ request->send(LittleFS, "/media/test.jpg", "image/jpg"); });

  WEBserver.on("/testgif", HTTP_GET, [](AsyncWebServerRequest *request){ request->send(LittleFS, "/media/test.gif", "image/gif"); });

  WEBserver.on("/testmedia", HTTP_GET, [](AsyncWebServerRequest *request){ serve_testmedia( request ); } );

// send a file

  WEBserver.on("/sudokusolver", HTTP_ANY, [](AsyncWebServerRequest *request){ request->send(LittleFS, "/SudokuSolver.html");});

// ----- Start the Ticker

  bmestatus = bme.begin(0x76);
  readbme280();
  bmeTicker.attach(bmeTickerInterval, bmeTickerRoutine);

  Serial.println(F("----- End app setup\n"));

}

// ======================================================================
//  Your  loop sections
// ======================================================================

void app_loop(){

// ======================================================================
//  FTP
// ======================================================================

// this is all you need to handle FTP, make sure to call handleFTP() frequently

  ftpServer.handleFTP();
  
// Example for some logging / printing

  if (ftpServer.FTPaction != "") {
    if (ftpServer.FTPaction.substring(0,8) == F("FTP User")) {
      log(ftpServer.FTPaction);
    } else {
      Serial.println(ftpServer.FTPaction);
    }
    ftpServer.FTPaction ="";
  }

// ======================================================================
//  app
// ======================================================================

// We can not send notofications before WiFi is ok and time is synchronised
// FirstTimeUpdateAfterConnect is set to 1 after first successful time update after connecting to WiFi

  if (firstTimeUpdateAfterConnect > 0 ) {

    for (int i = 0; i < nr_limits ; i++ ) { // for each alarm on the alarms setup page

      if ( ( notify_state[i] == 1) or ( notify_state[i] == 4) ) {     // 1::alarm reached ; 4::alarm cleared ( set in readbme280 )

//        sysinfo("Alarm handling"); // show some info during testing

        String limit_type;
        float alarm_value;

        String status_txt ;

        limit_type = String(Limit_units_name_str[Limit_untis[i]])  ;  // variables are application specific so created in 11_vars.h 

        switch (Limit_untis[i]) {
            case 0: alarm_value = real_temperature; break ;
            case 1: alarm_value = real_pressure   ; break ;
            case 2: alarm_value = real_humidity   ; break ;
            case 3: alarm_value = real_dewpoint   ; break ;
            case 4: alarm_value = real_humidex    ; break ;
            case 5: alarm_value = hour_hpa1hour   ; break ;
            case 6: alarm_value = hour_hpa2hour   ; break ;
            default: break;
        }
        String notification = limit_type + F(" Alarm STATE : ") + String(alarm_value)     + F(" ") + String(Limit_units_str[Limit_untis[i]])
                               + F(" ; limit : ") + String(Limit_values[i]) + F(" ") + String(Limit_units_str[Limit_untis[i]]) ;
        switch (notify_state[i]) {
          case 1:
            notification.replace(F("STATE"),F("reached"));
            if (demomode) {
              status_txt = F("Mail disabled!");
            } else {
              status_txt = sendMail(limit_type+ F(" Alarm reached"), datetime.CDT + F(" ") + notification);
            }
            log( notification+" ; "+status_txt);
            if ( ( status_txt == F("Message sent!")) or ( status_txt == F("Mail disabled!")) ) {
              notify_state[i] = 3;
            } else {
              notify_state[i] = 2;
            } break ;
          case 4:
            notification.replace(F("STATE"),F("cleared "));
            if (demomode) {
              status_txt = F("Mail disabled!");
            } else {
              status_txt = sendMail(limit_type+ F(" Alarm cleared"), datetime.CDT + F(" ") +notification);
            }
            log( notification+" ; "+status_txt);
            if ( ( status_txt == F("Message sent!")) or ( status_txt == F("Mail disabled!")) ) {
              notify_state[i] = 0;
            } else {
              notify_state[i] = 5;
            } break ;
          default: break;
        }
      }
    }
  }


}
