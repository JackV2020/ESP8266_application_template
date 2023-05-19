// ======================================================================
//  App web pages
// ( lcl_* is local to this tab and not in *prototypes.h files )
// ======================================================================
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
//  Just a test to show some media files
// ======================================================================

void serve_testmedia(AsyncWebServerRequest * request) {

  showServing(request);

  html_response = F("<html><style>body {background-color:black;}</style></body><center><h1><font style=\"color:red\">Media Test</font></h1><img src=\"testgif\"><br><img width=\"300\" height=\"300\" src=\"testjpg\"></center></body></html>");

  char* c = const_cast<char*>(html_response.c_str());
  request->send_P(200, texthtml, c);
//body {margin: 0; line-height: 80%; height: 100vh; background: radial-gradient(circle at 90%, #888, #aaa 20%,  #888 50%, #eee 75%, #555 75%, #eee 76%); }
  
}
// ======================================================================
//  Barometer web pages
// ======================================================================

// First some handy routines to prepare data

void lcl_PrepGraphGata(float input[datahours+1], float input_offset) {
// process the complete array. Another step will cut the right part for the number of days
  for (int i = 0 ; i < (datahours+1) ; i++ ) {
    graphdata[i] = input[i] + input_offset ;
  }
}

void lcl_PrepDewpoints() {
// process the complete array. Another step will cut the right part for the number of days
  for (int i = 0 ; i < (datahours+1) ; i++ ) {
    if (Humidities[i] > 0 ) {
      float X = log(( Humidities[i] + Humidity_offset) / 100) + (17.62 * ( Temperatures[i] + Temperature_offset ) / (243.12 + Temperatures[i]+ Temperature_offset )) ;
      graphdata[i] = 243.12 * X / (17.62 - X) ;
    } else {
      graphdata[i] = 0;
    }
  }
}

void lcl_PrepPressureChanges(int hours) {
// prep data for hPa/hour or hPa/2hours
// process the complete array. Another step will cut the right part for the number of days

// last results are based by last 60 and 120 minutes.
  if (hours == 1) {   // results for change per hour
    graphdata[datahours] = hour_hpa1hour ;
  } else {            // results for change per 2 hours
    graphdata[datahours] = hour_hpa2hour ;
  }
// we fill this array at the end and work our way back to index 0

  for (int i = datahours - 1 ; i >=0 ; i-- ) {
    if (i >= hours ) { graphdata[i] = Pressures[i] - Pressures[i-hours]; }  // substract value 1 hour or 2 hours back
    else {graphdata[i] = 0 ; }
  }
}


void lcl_PrepHumidex() {
// https://en.m.wikipedia.org/wiki/Humidex
// process the complete array. Another step will cut the right part for the number of days
  for (int i = 0 ; i < (datahours+1) ; i++ ) {
    if (Humidities[i] == 0) {
      graphdata[i] = 0;
    } else {
      float X = log(( Humidities[i] + Humidity_offset) / 100) + (17.62 * ( Temperatures[i] + Temperature_offset ) / (243.12 + Temperatures[i]+ Temperature_offset )) ;
      float DewPoint = 243.12 * X / (17.62 - X) ;
      graphdata[i] = Temperatures[i] + Temperature_offset + ( 6.11 * pow(2.71828, ( 5417.7530 * ( ( 1.0 / 273.16 ) - ( 1.0 / (273.15 + DewPoint) ) ) ) ) - 10.0 ) * 5.0 / 9.0 ;
    }
  }
}

void lcl_PrepPressureMinutes() {
// process last part of graphdata array. Another step will cut the right part for the number of minutes
  for (int i = 0 ; i < 121 ; i++ ) {
//    Serial.println(String(i)+" "+String(i+48));
    graphdata[i+48] = PressureMinutes[i] + Pressure_offset ;
  }
}

void serve_barometer(AsyncWebServerRequest * request) {

  showServing(request);

//  if (virtualhost == ap_IP) {
  if (1 == 2) {
    html_response = F("Not found");
  } else {

    readbme280();

    html_response = String(sta_Header);

    if (barometer_page_nr == 0 ) {
//      html_response = html_response + String(appShowGauges);
      html_response.concat(String(appShowGauges));
      html_response.replace(F("<body"),F("<body ondblclick=\"window.location.href = '/';\""));
      html_response.replace(F("GAUGEINTERVAL"),gaugeInterval);
      html_response.replace(F("TEMPERATURE"),String(Temperature+Temperature_offset));
      html_response.replace(F("DEGREE"),String(char(176)));
      html_response.replace(F("PRESSURE"),String(Pressure+Pressure_offset));
      html_response.replace(F("HUMIDITY"),String(Humidity+Humidity_offset));
    } else {
//      html_response = html_response + String(appShowChart);
      html_response.concat(String(appShowChart));
//      html_response.replace(F("BTHIS"), String(barometer_page_nr));
      if (barometer_page_nr == 8) {
          html_response.replace(F("STATUS_TXT"),F("Last 120 minutes"));
      } else {
        if (graphDays == 1) {
          html_response.replace(F("STATUS_TXT"),F("Last day"));
        } else {
          html_response.replace(F("STATUS_TXT"),F("Last ")+String( graphDays)+F(" days"));
        }
      }
      html_response.replace(F("CHARTID"),F("Chart1"));
      html_response.replace(F("DATETIME.CDT"),datetime.CDT);

      int step;
      int steps;
      String scale="";
      String skips;
      if (barometer_page_nr == 8) {
        for (int i = 0; i < 12 ; i++ ) {
          scale.concat(String(10*i));
          scale.concat(F(",,,,,,,,,,"));
        }
        scale.concat(F("120"));
      } else {
        switch (graphDays) {
          case 1:    step =  3; skips =F(",,,"); break ;
          case 2:    step =  6; skips =F(",,,,,,"); break ;
          case 3://    step = 12; steps =  6 ; skips =F(",,,,,,,,,,,,"); break ;
          case 4://    step = 12; steps =  8 ; skips =F(",,,,,,,,,,,,"); break ;
          case 5://    step = 12; steps = 10 ; skips =F(",,,,,,,,,,,,"); break ;
          case 6://    step = 12; steps = 12 ; skips =F(",,,,,,,,,,,,"); break ;
          case 7:    step = 12; skips =F(",,,,,,,,,,,,"); break ;
        }
        steps = graphDays * 24  / step ;
        scale = String((tmstruct.tm_hour + 1 ) % 24 );
        for (int i = 1 ; i <= steps ; i++ ) {
          scale = String((tmstruct.tm_hour + 1 - step * i + 240 ) % 24 ) + skips + scale;
        }
      }

      html_response.replace(F("XVALUES"),scale);

      switch (barometer_page_nr) {
        case 1 :
          lcl_PrepGraphGata(Temperatures, Temperature_offset) ;
          html_response.replace(F("LABEL"),F("<a href=\"https://en.m.wikipedia.org/wiki/Temperature\">Temperature &degC</a>"));
          html_response.replace(F("DATA"),floatArrayToString(graphdata, datahours+1, (7 - graphDays) * 24 ) );
        break;
        case 2 :
          lcl_PrepGraphGata(Pressures, Pressure_offset) ;
          html_response.replace(F("LABEL"),F("<a href=\"https://en.m.wikipedia.org/wiki/Atmospheric_pressure#Surface_pressure\">Pressure hPa</a>"));
          html_response.replace(F("DATA"),floatArrayToString(graphdata, datahours+1, (7 - graphDays) * 24 ) );
        case 3 :
          lcl_PrepGraphGata(Humidities, Humidity_offset) ;
          html_response.replace(F("LABEL"),F("<a href=\"https://en.m.wikipedia.org/wiki/Humidity#Relative_humidity\">Relative Humidity %</a>"));
          html_response.replace(F("DATA"),floatArrayToString(graphdata, datahours+1, (7 - graphDays) * 24 ) );
        break;
        case 4 :
          lcl_PrepDewpoints() ;
          html_response.replace(F("LABEL"),F("<a href=\"https://en.m.wikipedia.org/wiki/Dew_point\">Dewpoint &degC</a>"));
          html_response.replace(F("DATA"),floatArrayToString(graphdata, datahours+1, (7 - graphDays) * 24 ) );
        break;
        case 5 :
          lcl_PrepHumidex() ;
          html_response.replace(F("LABEL"),F("<a href=\"https://en.m.wikipedia.org/wiki/Humidex\">Humidex</a>"));
          html_response.replace(F("DATA"),floatArrayToString(graphdata, datahours+1, (7 - graphDays) * 24 ) );
        break;
        case 6 :
          lcl_PrepPressureChanges(1) ;
          html_response.replace(F("LABEL"),F("<a href=\"https://www.worldstormcentral.co/law%20of%20storms/secret%20law%20of%20storms.html\">hPa / hour</a>"));
          html_response.replace(F("DATA"),floatArrayToString(graphdata, datahours+1, (7 - graphDays) * 24 ) );
        break;
        case 7 :
          lcl_PrepPressureChanges(2) ;
          html_response.replace(F("LABEL"),F("<a href=\"https://www.worldstormcentral.co/law%20of%20storms/secret%20law%20of%20storms.html\">hPa / 2 hours</a>"));
          html_response.replace(F("DATA"),floatArrayToString(graphdata, datahours+1, (7 - graphDays) * 24 ) );
        break;
        case 8 :
          lcl_PrepPressureMinutes();
//          Serial.println(floatArrayToString(graphdata, datahours+1, datahours+1-121));
//          Serial.println(floatArrayToString(PressureMinutes, 121,0));
//          Serial.println(String(graphdata[48]) + "  "+String(graphdata[168]));
          html_response.replace(F("LABEL"),F("<a href=\"https://en.m.wikipedia.org/wiki/Atmospheric_pressure#Surface_pressure\">Pressure hPa</a>"));
          html_response.replace(F("DATA"),floatArrayToString(graphdata, datahours+1, datahours+1-121));
//          html_response.replace(F("DATA"),floatArrayToString(PressureMinutes, 121,0));
        break;
      }
    }

    html_response = html_response + F("</body></html>");
  }
  char* c = const_cast<char*>(html_response.c_str());
  request->send_P(200, texthtml, c);

}
