// ======================================================================
//  Standard web pages <<Do not change>>
// ( lcl_* is local to this tab and not in *prototypes.h files )
/*

Notes on GET and PUT

    String parameter;

Check if GET parameter exists
    if(request->hasParam(F("parameter"), false))
        parameter  = request->getParam(F("parameter"), false)->value();

Check if POST parameter exists
    if(request->hasParam(F("parameter"), true))
        parameter  = request->getParam(F("parameter"), true)->value();

more  documentation : https://github.com/me-no-dev/ESPAsyncWebServer#readme

*/
// ======================================================================

// ======================================================================
//  Start --- WiFi Manager and Home Page <<Do not change >>
// ======================================================================

void serve_WiFi_mgr (AsyncWebServerRequest * request ) {

  html_response = String(ap_Header);
  html_response.replace(F("REFRESH"), F(""));
  html_response.concat(String(WiFi_Manager));
  html_response.replace(F("<body"),F("<body ondblclick=\"history.back();\""));
  html_response.replace(F("STAHOSTNAME"),sta_hostname);
  html_response.replace(F("DROPDOWNSSIDS"),drop_down_SSIDs(F("sta_ssid"),F("sta_ssid_id")));
  html_response.replace(F("APP_PASSWORD_PLACEHOLDER"),String(app_password_placeholder));
  html_response.replace(F("APP_SHOW_PASSWORD_LABEL"),String(app_show_password_label));

  html_response.replace(F("APP_VERSION"),appVersion);

//  html_response.concat(F("<div id=\"bottom\"><hr>"));
//  html_response.concat(String(appVersion));
//  html_response.concat(F("<hr></div></body></html>"));
  char* c = const_cast<char*>(html_response.c_str());
  request->send_P(200, texthtml, c);

}

void serve_WiFi_info (AsyncWebServerRequest * request, String mode) {

// individual page but alse also called from serve_home()

  showServing(request);

  if (mode == F("save")) {     // When data posted from access point home page

// Check for valid app password

    String check_app_password="";

    if ( (request->hasParam(F("app_password"), true) ) )    { check_app_password = request->getParam(F("app_password"), true)->value(); }

    if  ( (check_app_password == app_password)                    // You entered the right app_password to get in

// you could enable one or more of the next to have extra functionality to get in

//      or (check_app_password == String(app_password_secret)  )   // You use the app_password_secret to get in
//      or (check_app_password == String(ftp_emergency_pwd)    )   // You use the ftp_emergency_pwd to get in
// next line allowes to get in when 'app_password' is still the 'default_app_password' and first check failed because you 
// typed something else than the current 'app_password'. Line is needed to enable password change from 'default_app_password' to something else.
//      or (app_password       == String(default_app_password) )   // Previous checks failed, so you get in when actual password is still 'default_app_password'
        
    ) {                 // You got in some way

      String reboot="no";
      if (request->hasParam(F("reboot"), true)) { reboot = request->getParam(F("reboot"), true)->value(); }

      html_response = String(ap_Header);
      html_response.replace(F("REFRESH"), F(""));

      if ( reboot == F("yes") ) {                  // We have a reboot request from the AP home page to refresh the SSID list

        html_response.concat(String(rebootPage));
        html_response.replace(F("STA_HOSTNAME"),virtualhost);

        rebootRequestCounter = 5;
        rebootRequestReason = F("Refresh SSID list");
      } else {

        if ( ( false )

// you could enable one or more of the next to have extra functionality to reset / change the app_password

//          or ( check_app_password == String(app_password_secret) )    // You use the app_password_secret to reset the app_password to the default_app_password
//          or ( check_app_password == String(ftp_emergency_pwd)   )    // You use the ftp_emergency_pwd to reset the app_password to the default_app_password

          ) {
          app_password = default_app_password ; 
          log(F("WiFi Manager change APP password to default from IP : ")+client_IP);
          saveConfig();

// you got here via (check_app_password == app_password) or via (app_password == default_app_password) and could....
// enable the pasword to be changed from 'default_app_password' to something else
//        } else { 
//          if ((check_app_password != app_password)) {  // You want to change the password from a 'default_app_password' to something else
//            app_password = String(check_app_password) ; 
//            log(F("WiFi Manager change APP default password to something else from IP : ")+client_IP);
//            saveConfig();
//          }
        }

        String old_value;

        rebootRequestReason="";

        old_value = sta_ssid;
        if (request->hasParam(F("sta_ssid"), true))   { sta_ssid  = request->getParam(F("sta_ssid"), true)->value(); }
        if (old_value   != sta_ssid  ) {
          rebootRequestReason.concat(F(" SSID"));
        }

        old_value = sta_pwd;
        if (request->hasParam(F("sta_pwd"), true))   { sta_pwd  = request->getParam(F("sta_pwd"), true)->value(); }
        if (sta_pwd == "") {sta_pwd = old_value; }
        if (old_value   != sta_pwd  ) {
          rebootRequestReason.concat(F(" password"));
        }

        old_value = sta_hostname;
        if (request->hasParam(F("sta_hostname"), true))   { sta_hostname  = request->getParam(F("sta_hostname"), true)->value(); }
        if (old_value   != sta_hostname  ) {
          rebootRequestReason.concat(F(" hostname"));
        }

        if (rebootRequestReason == "") {              // nothing changed so we go back to the WiFi manager page to ask for input
          html_response=R"===(<html><head><meta http-equiv="refresh" content="0; url='/WiFi_mgr'" /></head></html>)===";
        } else {
          rebootRequestReason=F("WiFi change :")+rebootRequestReason;
          saveConfig();
          if (virtualhost != ap_IP ) {
            html_response.concat(String(rebootPage));
            html_response.replace(F("STA_HOSTNAME"),sta_hostname); // for redirect from rebootPage to right sta_hostname
            html_response.concat(F("<p>When I can't connect...."));
          }
          html_response.concat(F("<p>Please connect to WiFi :<br><br>\""));
          html_response.concat(sta_hostname);
          html_response.concat(F(" "));
          html_response.concat(ap_IP);
          html_response.concat(F("\"<br><br> and browse to<br><br><a href=\"http://"));
          html_response.concat(ap_IP);
          html_response.concat(F("/\">http://"));
          html_response.concat(ap_IP);

          rebootRequestCounter = 5;
        }
      }
    } else {
      html_response=R"===(<html><head><meta http-equiv="refresh" content="0; url='/WiFi_mgr'" /></head></html>)===";
    }
  } else {                  // when AP home page detects WiFi connection
    html_response = String(ap_Header);
    html_response.replace(F("REFRESH"), F("<meta http-equiv=\"refresh\" content=\"60\" />")); // 60 second refresh option
    html_response.concat(F("<p>You can browse to "));
    html_response.concat(sta_hostname);
    html_response.concat(F("<br><br>on your WiFi "));
    html_response.concat(sta_ssid);
    html_response.concat(F("<br><br>on :<br><br><a href=\"http://"));
    html_response.concat(sta_IP);
    html_response.concat(F("/\">http://"));
    html_response.concat(sta_IP);
    html_response.concat(F("/</a><br><br>and maybe on :<br><br><a href=\"http://"));
    html_response.concat(sta_hostname);
    html_response.concat(F("/\">http://"));
    html_response.concat(sta_hostname);
  }
  html_response.concat(F("</a></p></center><div id=\"bottom\"><hr>"));
  html_response.concat(String(appVersion));
  html_response.concat(F("<hr></div></body></html>"));
  char* c = const_cast<char*>(html_response.c_str());
  request->send_P(200, texthtml, c);
}

void serve_home (AsyncWebServerRequest * request, String mode) {

  showServing(request);

  if ( (WiFi.status() != WL_CONNECTED)     // We have no STA connection so show AP home page with WiFi manager
      or (mode == F("WiFi_mgr")           ) ) { // We use the 'WiFi setup' button
// We could use a redirect from this home page to the WiFi_mgr page like :
//    html_response=R"===(<html><head><meta http-equiv="refresh" content="0; url='/WiFi_mgr'" /></head></html>)===";
//    char* c = const_cast<char*>(html_response.c_str());
//    request->send_P(200, texthtml, c);
// but that results in the next anyway so lets just do that :
      serve_WiFi_mgr ( request );
  } else {
    if (virtualhost == ap_IP) {               // We have a WiFi connection and are on the AP interface so show the WiFi page
// A same redirect remark like above is valid for the WiFi_info page so no redirect but just :
      serve_WiFi_info( request, F("show"));
    } else {                                  // We have a WiFi connection and are on the STA interface so show the APP home page
      html_response = String(sta_Header);
      html_response.concat(String(sta_HomePage));
      html_response.replace("STYLE",F("style=\"background-color:Chartreuse\""));
      html_response.replace("APPNAME",app_name);
      char* c = const_cast<char*>(html_response.c_str());
      request->send_P(200, texthtml, c);
    }
  }
}

// ======================================================================
//  Show info on page being served <<Do not change>>
// ======================================================================

void showServing(AsyncWebServerRequest *request){
  String print_line;
  flip_LED(request->url());
  virtualhost = request->host().c_str() ;
  client_IP = IpAddress2String(request->client()->remoteIP());
  print_line=F("Serving http://"); print_line.concat(virtualhost); print_line.concat(request->url()); print_line.concat((" for ")); print_line.concat(client_IP);
  Serial.println(print_line);
}

// ======================================================================
//  Show message for not found pages <<Do not change>>
// ======================================================================

void notFound(AsyncWebServerRequest *request) {
  showServing(request);
  flip_LED(request->url()); // revert extra flip led for not found pages
  request->send(404, textplain, F("Not found"));
}

// ======================================================================
//  Handle On The Air code load <<Do not change>>
// ======================================================================

void serve_setup(AsyncWebServerRequest *request) {  // This is the standard setup page with 3 buttons

  showServing(request);

  if (virtualhost == ap_IP) {

    html_response = F("Not found");
  } else {
    String status_txt = F("Setup");
    html_response = String(sta_Header);
    html_response.replace(F("<body"),F("<body ondblclick=\"window.location.href = '/';\""));
    html_response.concat(sta_setup);
//    html_response.replace("STYLE",F("style=\"background-color:Chartreuse\""));
    html_response.replace("STYLE","");
    html_response.replace("APPNAME",app_name);
  }
  char* c = const_cast<char*>(html_response.c_str());
  request->send_P(200, texthtml, c);
  html_response="";
}

// ======================================================================
//  Handle On The Air code load <<Do not change>>
// ======================================================================

void serve_start_ota(AsyncWebServerRequest *request) {  // This is an automatic redirect to the real ElegantOTA which runs on web page /update

  showServing(request);

//  if (virtualhost == ap_IP) {

// virtualhost check to not allow access from the access point side
// is not needed because we start OTA with username and password in setup like :
//  AsyncElegantOTA.begin(&server,sta_hostname.c_str(),app_password.c_str()); // Start ElegantOTA on url http://any_ip/update
// so we are safe.

//    html_response = F("Not found");
//  } else {
    log( F("Start ElegantOTA"));
    flip_LED(request->url()); // revert extra flip led for not found pages
    html_response = String(sta_Header);
    html_response.replace(F("<body"),F("<body ondblclick=\"window.location.href = '/';\""));
    html_response.concat(F("<html><head><meta http-equiv=\"refresh\" content=\"5; url=/update\"/></head><body><br><br><br>Starting ElegantOTA update<br><br>in a few seconds....<br><br>Username = '"));
    html_response.concat(sta_hostname);
    html_response.concat(F("'<br><br>Password = 'Setup password'<br><br>You may need to refresh<br><br>next empty page....</body></html>"));
//  }
  char* c = const_cast<char*>(html_response.c_str());
  request->send_P(200, texthtml, c);
  html_response="";
}

// ======================================================================
//  Web page show logging <<Do not change>>
// ======================================================================

void serve_log (AsyncWebServerRequest * request) {

  showServing(request);
  if (virtualhost == ap_IP) {
// do not allow access from the access point side
    html_response = F("Not found");
  } else {
    html_response = F("<html><body ondblclick=\"window.location.href = '/';\" onclick=\"window.location.href = '/log';\">");
    html_response.concat(contentOfFile(String(file_system_log)));
    html_response.concat(F("</body></html>"));
  }
  char* c = const_cast<char*>(html_response.c_str());
  request->send_P(200, texthtml, c);
}

// ======================================================================
//  System setup <<Do not change>>
// ======================================================================

void serve_setup_system (AsyncWebServerRequest * request, String mode) {

/*
This page is called from the home page and is used to manage the APP configuration.
- email settings
- hostname
- timezone
- etc. on your pages

These pages are available for everyone on your WiFi so it is protected by app_password which is initially default_app_password
The password is shown nowhere but can be bypassed by what you define in app_password_secret and the password will be reset to default_app_password

*/
  showServing(request);

  String status_txt = F("Setup");
  String saveappsetup = "";
  if (virtualhost == ap_IP) {
    html_response = F("Not found");
  } else {
    if (mode == F("data")) {

// ----------------------------------------------------------------------
//  ***** activate to show what comes while saving. Nice during debugging
// ----------------------------------------------------------------------

// Start Show all input from POST

      if (1 == 2) {
        Serial.println(F("URL : ")+request->url());
        int params = request->params();
        Serial.printf("%d params sent in\n", params);
        for (int i = 0; i < params; i++)
        {
          AsyncWebParameter *p = request->getParam(i);
          Serial.printf("_POST[%s]: %s", p->name().c_str(), p->value().c_str());
          Serial.println(F("\n"));
        }
      }

// End Show all input from POST

      String check_app_password ="";

// Check for valid app password

      check_app_password = request->getParam(F("app_password"), true)->value();

      if ( (check_app_password == app_password)                 // You entered the right app_password (it may even still be the 'default_app_password')
      or (check_app_password == String(app_password_secret) )   // You use the app_password_secret to reset the app_password to the default_app_password which is a "."
      or (app_password == String(default_app_password)) ) {     // You want to change the password from a 'default_app_password' to something else

// ----------------------------------------------------------------------
//  Start processing system setup page input
// ----------------------------------------------------------------------

        if ( check_app_password == String(app_password_secret)) {
          app_password = default_app_password ; 
          log(F("Change APP password to default from IP : ")+client_IP);
        } else { 
          if ((check_app_password != app_password)) {
            app_password = String(check_app_password) ; 
            log(F("Change APP password to something else from IP : ")+client_IP);
          }
        }

        String old_value;

// next is not needed because we get here via the POST of savesystemsetup
//        if (request->hasParam(F("savesystemsetup"), true))    { savesystemsetup  = request->getParam(F("savesystemsetup"), true)->value(); }

        old_value = email_to;
        if (request->hasParam(F("email_to"), true))       { email_to      = request->getParam(F("email_to"), true)->value(); }
        if (old_value       != email_to      ) {log(F("Change email recipient from ")+old_value+F(" to ")+email_to);};

// tricky one because a checkbox is not in the POSTed request when it is not checked....
        old_value = email_enabled;
        if (request->hasParam(F("email_enabled"), true))   { email_enabled = "yes" ; } else { email_enabled = "no" ;}
        if (old_value  != email_enabled ) {log(F("Change email enabled from ")+old_value+F(" to ")+email_enabled);};

        old_value = smtp_server;
        if (request->hasParam(F("smtp_server"), true))     { smtp_server   = request->getParam(F("smtp_server"), true)->value(); }
        if (old_value    != smtp_server   ) {log(F("Change smtp server from ")+old_value+F(" to ")+smtp_server);};

        old_value = smtp_port;
        if (request->hasParam(F("smtp_port"), true))       { smtp_port     = request->getParam(F("smtp_port"), true)->value(); }
        if (old_value      != smtp_port     ) {log(F("Change smtp port from ")+old_value+F(" to ")+smtp_port);};

        old_value = email_login;
        if (request->hasParam(F("email_login"), true))     { email_login   = request->getParam(F("email_login"), true)->value(); }
        if (old_value    != email_login   ) {log(F("Change email login from ")+old_value+F(" to ")+email_login);};

        old_value = email_password;
        String new_email_password ="";
        if (request->hasParam(F("email_password"), true))  { new_email_password = request->getParam(F("email_password"), true)->value(); }
        if (new_email_password != "" ) {email_password = new_email_password ; }
        if (old_value != email_password) {log(F("Change email password to something else"));};

        old_value = email_link;
        if (request->hasParam(F("email_link"), true))     { email_link = request->getParam(F("email_link"), true)->value(); }
        if (old_value    != email_link   ) {log(F("Change email_link from ")+old_value+F(" to ")+email_link);};

// ***** when you want to use the dropppint service, dropppint is already part of saved and read config data ;-)
// no need to remove this.

        old_value = droppoint;
        if (request->hasParam(F("droppoint"), true))     { droppoint = request->getParam(F("droppoint"), true)->value(); }
        if (old_value    != droppoint   ) {log(F("Change droppoint from ")+old_value+F(" to ")+droppoint);};

        old_value = time_zone;
        if (request->hasParam(F("time_zone"), true))      { time_zone     = request->getParam(F("time_zone"), true)->value(); }
        if (old_value      != time_zone     ) {
          timeServerConnected = false;
          time_zone_set = false;
          log(F("Change time zone from ")+old_value+F(" to ")+time_zone);
        };

        saveConfig();

        status_txt = F("<font color=\"green\">Saved</font>");
      } else {
        status_txt = F("<font color=\"red\">Wrong Entry</font>");
      }

// ----------------------------------------------------------------------
//  End processing input
// ----------------------------------------------------------------------

    }
    
// Building the system setup page

    html_response = String(sta_Header);
    html_response.replace(F("<body"),F("<body ondblclick=\"window.location.href = '/setup';\""));

    html_response.concat(String(SetupSystem));
    html_response.replace(F("EMAIL_TO"),email_to);
    if (email_enabled == F("yes") ) { html_response.replace(F("M_ENACHECKED"),F("checked")); }
                               else {html_response.replace(F("M_ENACHECKED"),"");  }
    html_response.replace(F("SMTP_SERVER"),smtp_server);
    html_response.replace(F("SMTP_PORT"),smtp_port);
    html_response.replace(F("EMAIL_LOGIN"),email_login);
    html_response.replace(F("DROPPOINT"),droppoint);
    html_response.replace(F("EMAIL_LINK"),email_link);
    html_response.replace(F("STA_HOSTNAME"),sta_hostname);
    html_response.replace(F("TIMEZONESELECT"),String(timezoneselect));
    html_response.replace(F("value=\"")+time_zone+F("\""),F("value=\"")+time_zone+F("\" selected"));
    html_response.replace(F("STATUS_TXT"),status_txt);

    html_response.concat(String(SetupBottom));
    html_response.replace(F("APP_PASSWORD_PLACEHOLDER"),String(app_password_placeholder));
    html_response.replace(F("APP_SHOW_PASSWORD_LABEL"),String(app_show_password_label));

    html_response.concat(F("</body></html>"));

  }
  char* c = const_cast<char*>(html_response.c_str());
  request->send_P(200, texthtml, c);
}
