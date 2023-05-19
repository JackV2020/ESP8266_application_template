// ======================================================================
//  3 Standard web pages where you may make modifications
/*

The 3 web pages are :

void serve_setup_app (AsyncWebServerRequest * request, String mode)
void serve_json (AsyncWebServerRequest * request)
void serve_change_variable (AsyncWebServerRequest * request)

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
//  App setup, leave name as is
// ======================================================================

void serve_setup_app (AsyncWebServerRequest * request, String mode) {

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
/*
      if ( (check_app_password == app_password)                 // You entered the right app_password (it may even still be the 'default_app_password')
      or (check_app_password == String(app_password_secret) )   // You use the app_password_secret to reset the app_password to the default_app_password which is a "."
      or (app_password == String(default_app_password)) ) {     // You want to change the password from a 'default_app_password' to something else
*/
      if (check_app_password == app_password) {                 // You are allowed to save settings

// ----------------------------------------------------------------------
//  Start processing app setup page input
// ----------------------------------------------------------------------

//.....check if we are on the offset and graph page

        if (request->hasParam(F("Temperature_offset"), true))    {

          float old_Temperature_offset = Temperature_offset;
          float old_Pressure_offset = Pressure_offset;
          float old_Humidity_offset = Humidity_offset;
          int old_graphDays = graphDays;
          String old_gaugeInterval = gaugeInterval;

          String str_Temperature_offset;
          String str_Pressure_offset;
          String str_Humidity_offset;

          str_Temperature_offset  = request->getParam(F("Temperature_offset"), true)->value(); Temperature_offset = str_Temperature_offset.toFloat() ;
          str_Pressure_offset     = request->getParam(F("Pressure_offset"), true)->value();    Pressure_offset = str_Pressure_offset.toFloat() ;
          str_Humidity_offset     = request->getParam(F("Humidity_offset"), true)->value();    Humidity_offset = str_Humidity_offset.toFloat() ;
          gaugeInterval           = request->getParam(F("gaugeInterval"), true)->value();

          if (old_Temperature_offset != Temperature_offset ) {log(F("Change Temperature_offset from ")+String(old_Temperature_offset)+F(" to ")+String(Temperature_offset));};
          if (old_Pressure_offset    != Pressure_offset    ) {log(F("Change Pressure_offset from ")+String(old_Pressure_offset)+F(" to ")+String(Pressure_offset));};
          if (old_Humidity_offset    != Humidity_offset    ) {log(F("Change Humidity_offset from ")+String(old_Humidity_offset)+F(" to ")+String(Humidity_offset));};
          if (old_gaugeInterval      != gaugeInterval      ) {log(F("Change gaugeInterval from ")+String(old_gaugeInterval)+F(" to ")+gaugeInterval);};

        }

//.....check if we are on the limits page

        if (request->hasParam(F("Limit_values[0]"), true))    {
          String old_value;
          String newvalue;
          bool update = false;
          for (int i = 0; i <nr_limits; i++) {

            old_value = String(Alarm_dabs_str[Alarm_dabs[i]]);
            newvalue = request->getParam(F("Alarm_dabs[")+String(i)+F("]"), true)->value();
            Alarm_dabs[i] = newvalue.toInt() ;
            update  = ( String(Alarm_dabs_str[Alarm_dabs[i]]) != old_value );

            old_value = String(Limit_values[i],1 );                 // convert float to string 1 decimal
//            Serial.println("old_value : >"+old_value+"<");
            newvalue = request->getParam(F("Limit_values[")+String(i)+F("]"), true)->value();
            Limit_values[i] = newvalue.toFloat() ;                  // 1 decimal input is forced by web page
            update = update or ( String(Limit_values[i],1 ) != old_value );

            old_value = String(Limit_units_name_str[Limit_untis[i]]   );
            newvalue = request->getParam(F("Limit_untis[")+String(i)+F("]"), true)->value();
            Limit_untis[i] = newvalue.toInt() ;
            update = update or ( String(Limit_units_name_str[Limit_untis[i]] ) != old_value );

            if (update) {log(F("Change Alarm Limit ")+String(i+1)+F(" to '")+String(Alarm_dabs_str[Alarm_dabs[i]])+F(" ")+String(Limit_values[i],1)+F(" ")+String(Limit_units_str[Limit_untis[i]])+F("'"));};

          }

        }


// ----------------------------------------------------------------------
//  End processing app setup page input
// ----------------------------------------------------------------------

        saveAppData();

        status_txt = F("<font color=\"green\">Saved</font>");
      } else {
        status_txt = F("<font color=\"red\">Wrong Entry</font>");
      }

// ----------------------------------------------------------------------
//  End processing input
// ----------------------------------------------------------------------

    }
    
// Building the app setup page

    html_response = String(sta_Header);
    html_response.replace(F("<body"),F("<body ondblclick=\"window.location.href = '/setup';\""));

    switch (setup_page_nr) {
// ----------------------------------------------------------------------
//  ***** Start APP creation setup pages
// ----------------------------------------------------------------------
        case 1:
          html_response.concat(String(appSetupPage_1));
          html_response.replace(F("TMP"),String(Temperature + Temperature_offset));
          html_response.replace(F("PRS"),String(Pressure + Pressure_offset));
          html_response.replace(F("HUM"),String(Humidity + Humidity_offset));
          html_response.replace(F("TO"),String(Temperature_offset));
          html_response.replace(F("PR"),String(Pressure_offset));
          html_response.replace(F("HU"),String(Humidity_offset));
          html_response.replace(F("value=\"")+String(graphDays)+F("\">")+String(graphDays)+F(" day"),F("value=\"")+String(graphDays)+F("\" selected>")+String(graphDays)+F(" day"));
          html_response.replace(F("value=\"")+String(gaugeInterval)+F("\">")+String(gaugeInterval)+F(" second"),F("value=\"")+String(gaugeInterval)+F("\" selected>")+String(gaugeInterval)+F(" second"));
          break ;
        case 2:
          html_response.concat(String(appSetupPage_2));
          String LIMITTYPE;
          String ALARMDAB;
          for (int i = 0; i <nr_limits; i++) {  // for each limit row
// prep selection for disabled|above|below
            ALARMDAB = F("<select style=\"width: 90px;\" name=\"Alarm_dabs[");
            ALARMDAB.concat(String(i));
            ALARMDAB.concat(F("]\" >"));
            ALARMDAB.concat(F("<option value=\"0\">disabled</option><option value=\"1\">above</option><option value=\"2\">below</option></select>"));
            ALARMDAB.replace(F("value=\"")+String(Alarm_dabs[i])+F("\""),F("value=\"")+String(Alarm_dabs[i])+F("\" selected"));
// prep selection for deg C, hPa, etc.
            LIMITTYPE = F("<select style=\"width: 90px;\" name=\"Limit_untis[");
            LIMITTYPE.concat(String(i));
            LIMITTYPE.concat(F("]\" >"));
            LIMITTYPE.concat(F("<option value=\"0\">&degC (temp)</option><option value=\"1\">hPa</option><option value=\"2\">%</option><option value=\"3\">&degC (dew)</option><option value=\"4\">Humidex</option><option value=\"5\">hPa/hour</option><option value=\"6\">hPa/2hours</option></select>"));
            LIMITTYPE.replace(F("value=\"")+String(Limit_untis[i])+F("\""),F("value=\"")+String(Limit_untis[i])+F("\" selected"));

// add this row

            html_response.concat(F("<br>"));
            if (i<9) {html_response.concat(F("&nbsp;&nbsp;"));}
            html_response.concat(String(i+1));
            html_response.concat(F("&nbsp;&nbsp;"));
            html_response.concat(ALARMDAB);

            html_response.concat(F("&nbsp;&nbsp;<input type=\"text\" style=\"width: 90px;\" name=\"Limit_values["));
            html_response.concat(String(i));
            html_response.concat(F("]\" pattern=\"[-|\\][0-9]+([\\.][0-9]{1})\" title=\"exact 1 decimal\" value="));
            html_response.concat(String(Limit_values[i],1));
            html_response.concat(F(">"));

            html_response.concat(F("&nbsp;&nbsp;"));
            html_response.concat(LIMITTYPE);
            if (nr_limits < 8) {html_response.concat(F("<br>"));}

          }
          break ;
    }
// ----------------------------------------------------------------------
//  ***** End APP creation setup pages
// ----------------------------------------------------------------------
    html_response.replace(F("STATUS_TXT"),status_txt);
    html_response.concat(String(SetupBottom));

    html_response.replace(F("APP_PASSWORD_PLACEHOLDER"),String(app_password_placeholder));
    html_response.replace(F("APP_SHOW_PASSWORD_LABEL"),String(app_show_password_label));

    html_response.concat(String(AppSetupPageButtons));

    html_response.concat(F("</body></html>"));

  }
  char* c = const_cast<char*>(html_response.c_str());
  request->send_P(200, texthtml, c);
}
// ======================================================================
//  Web page JSON, leave as is, only add/remove fields
// ======================================================================

void serve_json (AsyncWebServerRequest * request) {

// ---- Note this starts with some standard items below
// I may include some memory info :  https://github.com/Locoduino/MemoryUsage

  showServing(request);

  if (virtualhost == ap_IP) {
// do not allow access from the access point side
    html_response = F("Not found");
  } else {

// the barometer gauges page has an 'update' in function getData() so it gets real actual values

    if (request->hasParam(F("update"), false))    {
      readbme280();
    }

    String led;
    if (digitalRead(LED_BUILTIN)) { led = F("off") ;} else { led = F("on") ; } ;

    html_response =  F("{\"sta_hostname\":\"");               html_response.concat(sta_hostname);
    html_response.concat(F("\" , \"appVersion\":\""));        html_response.concat(String(appVersion));

    html_response.concat(F("\" , \"ap_MAC_address\":\""));    html_response.concat(WiFi.softAPmacAddress());
    html_response.concat(F("\" , \"sta_MAC_address\":\""));   html_response.concat(WiFi.macAddress());
    html_response.concat(F("\" , \"ap_IP\":\""));             html_response.concat(ap_IP);
    html_response.concat(F("\" , \"sta_IP\":\""));            html_response.concat(sta_IP);
    html_response.concat(F("\" , \"client_IP\":\""));         html_response.concat(client_IP);

    html_response.concat(F("\" , \"getChipId\":\""));         html_response.concat(String(ESP.getChipId()));

    html_response.concat(F("\" , \"boottime\":\""));          html_response.concat(boottime);
    html_response.concat(F("\" , \"loopCounter\":\""));       html_response.concat(String(loopCounter));
    html_response.concat(F("\" , \"led\":\""));               html_response.concat(led);


    html_response.concat(F("\" , \"FreeHeap\":\""));           html_response.concat(String(ESP.getFreeHeap()));
    html_response.concat(F("\" , \"HeapFragmentation\":\""));  html_response.concat(String(ESP.getHeapFragmentation()));
    html_response.concat(F("\" , \"MaxFreeBlockSize\":\""));   html_response.concat(String(ESP.getMaxFreeBlockSize()));

// disabled to reduce possible file system access due to polling domotica systems

//    html_response.concat(F("\" , \"logSize\":\""));            html_response.concat(String(fileSize(file_system_log)));
//    html_response.concat(F("\" , \"LittleFSsize_total\":\""));   html_response.concat(String(LittleFSsize('t')));
//    html_response.concat(F("\" , \"LittleFSsize_used\":\""));    html_response.concat(String(LittleFSsize('u')));
//    html_response.concat(F("\" , \"LittleFSsize_free\":\""));    html_response.concat(String(LittleFSsize('f')));

//  ***** Start APP specific fields

    html_response.concat(F("\" , \"Temperature\":\"")); html_response.concat(String(real_temperature));
    html_response.concat(F("\" , \"Pressure\":\""));    html_response.concat(String(real_pressure));
    html_response.concat(F("\" , \"Humidity\":\""));    html_response.concat(String(real_humidity));
    html_response.concat(F("\" , \"Dewpoint\":\""));    html_response.concat(String(real_dewpoint));
    html_response.concat(F("\" , \"Humidex\":\""));     html_response.concat(String(real_humidex));
    html_response.concat(F("\" , \"Pressure1h\":\""));  html_response.concat(String(hour_hpa1hour));
    html_response.concat(F("\" , \"Pressure2h\":\""));  html_response.concat(String(hour_hpa2hour));

    html_response.concat(F("\" , \"barometer_page_nr\":\""));               html_response.concat(String(barometer_page_nr));

//  ***** End APP specific fields

    html_response.concat(F("\"}"));

    html_response.concat(F("<body ondblclick=\"window.location.href = '/';\" onclick=\"window.location.href = '/json';\"></body>"));
  }
  char* c = const_cast<char*>(html_response.c_str());
  request->send_P(200, texthtml, c);
}

// ======================================================================
//  serve_change_variable <<Do not change>> except ***** section
// ======================================================================

void serve_change_variable (AsyncWebServerRequest * request) {

// This function can be called from web pages to change variables
//  if (virtualhost == ap_IP) {
  if (1 == 2) {
    html_response = F("Not found");
  } else {

//  Standard for setup pages

    if (request->hasParam(F("setup_page"), false))    { // next or prev setup page
      String setup_page;
      setup_page  = request->getParam(F("setup_page"), false)->value();
// for 2 setup pages
      int nrmax = 2;
      if (setup_page == F("next") )      { if (setup_page_nr == nrmax ) {setup_page_nr = 1; } else {  setup_page_nr = setup_page_nr + 1 ;} }
      else                               { if (setup_page_nr == 1 ) {setup_page_nr = nrmax; } else {  setup_page_nr = setup_page_nr - 1 ;} }
    }

// ----------------------------------------------------------------------
//  ***** Start APP variables
// ----------------------------------------------------------------------

    int lastpage = 8; // page with 120 minus of pressure values

    if (request->hasParam(F("zoom_direction"), false))    { // more (zoom out) or less (zoom in) days on barometer graph pages
      if (barometer_page_nr != lastpage) {
        String zoom_direction;
        zoom_direction  = request->getParam(F("zoom_direction"), false)->value();
        if ( (zoom_direction == F("in") ) and (graphDays > 1 )) { graphDays = graphDays - 1 ; }
        if ( (zoom_direction == F("out") ) and (graphDays < 7 )) { graphDays = graphDays + 1 ; }
      }
    }

    if (request->hasParam(F("barometer_page"), false))    { // next or prev barometer page for pages 1 .. 8
      String barometer_page;
      barometer_page  = request->getParam(F("barometer_page"), false)->value();
      if      (barometer_page == F("prev") ) { barometer_page_nr = (barometer_page_nr - 2 + lastpage ) % lastpage + 1 ; }
      else if (barometer_page == F("next") ) { barometer_page_nr = (barometer_page_nr                ) % lastpage + 1 ; }
      else {
        barometer_page_nr = barometer_page.toInt();
      }
    }

// ----------------------------------------------------------------------
//  ***** End APP variables
// ----------------------------------------------------------------------

    html_response = F("Oke");
  }
  char* c = const_cast<char*>(html_response.c_str());
  request->send_P(200, texthtml, c);
}
