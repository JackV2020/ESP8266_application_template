// ======================================================================
//  Standard save and read for APP data <<Do not change function names>>
// ======================================================================

void saveAppData(){   // !!!!! W A R N I N G !!!!! Do not change function name
/*
  called by 
    systemTickerRoutine() in cased we need a reboot,
    serve_app_setup to save settings
    your functions when needed
*/
  String appdata=  F("{\"Temperatures\":\"");           appdata.concat(floatArrayToString(Temperatures,datahours+1,0)) ;
  appdata.concat(F("\" , \"Pressures\":\""));           appdata.concat(floatArrayToString(Pressures,datahours+1,0));
  appdata.concat(F("\" , \"Humidities\":\""));          appdata.concat(floatArrayToString(Humidities,datahours+1,0));
  appdata.concat(F("\" , \"Temperature_offset\":\""));  appdata.concat(String(Temperature_offset));
  appdata.concat(F("\" , \"Pressure_offset\":\""));     appdata.concat(String(Pressure_offset));
  appdata.concat(F("\" , \"Humidity_offset\":\""));     appdata.concat(String(Humidity_offset));
  appdata.concat(F("\" , \"graphDays\":\""));           appdata.concat(String(graphDays));
  appdata.concat(F("\" , \"gaugeInterval\":\""));       appdata.concat(gaugeInterval);
  appdata.concat(F("\" , \"Limit_values\":\""));        appdata.concat(floatArrayToString(Limit_values,nr_limits,0));
  appdata.concat(F("\" , \"Limit_untis\":\""));         appdata.concat(intArrayToString(Limit_untis,nr_limits,0));
  appdata.concat(F("\" , \"Alarm_dabs\":\""));          appdata.concat(intArrayToString(Alarm_dabs,nr_limits,0));
  appdata.concat(F("\"}"));

  writeFile(String(file_appdata),appdata);

  Serial.println(F("Saved app data"));

}

void readAppData(){   // !!!!! W A R N I N G !!!!!  Do not change function name

// watch it !!!! called by setup() after a reboot

  String appdata = "" ;
  readFile(String(file_appdata),appdata);

  if ( appdata == "") { // after initial boot we have nothing stored yet so we enter some default values
    for ( int i = 0; i <datahours ; i++ ) {
      Temperatures[i] = 20.1;   // degrees Celcius
      Pressures[i]    = 1020.1; // hPa
      Humidities[i]   = 40.2;   // %
    }

    for ( int i = 0; i <nr_limits ; i++ ) {
      Limit_values[i] = 0;
      Limit_untis[i]  = 0;
      Alarm_dabs[i]   = 0;
      notify_state[i] = 0;
    }

  } else { // we read some data back which we saved before so let's unpack it

    String dataarray;   // there are arrays in the saved data
    String datastring;  // and also some strings

    parsejson(appdata , F("Temperatures"), dataarray);
    stringToFloatArray(dataarray, datahours+1, Temperatures );

    parsejson(appdata , F("Pressures"), dataarray);
    stringToFloatArray(dataarray, datahours+1, Pressures );

    parsejson(appdata , F("Humidities"), dataarray);
    stringToFloatArray(dataarray, datahours+1, Humidities );

    parsejson(appdata , F("Temperature_offset"), datastring);
    Temperature_offset = datastring.toFloat();

    parsejson(appdata , F("Pressure_offset"), datastring);
    Pressure_offset = datastring.toFloat();

    parsejson(appdata , F("Humidity_offset"), datastring);
    Humidity_offset = datastring.toFloat();

    parsejson(appdata , F("graphDays"), datastring);
    graphDays = datastring.toInt();

    parsejson(appdata , F("gaugeInterval"), gaugeInterval);

    parsejson(appdata , F("Limit_values"), dataarray);
    stringToFloatArray(dataarray, nr_limits, Limit_values );

    parsejson(appdata , F("Limit_untis"), dataarray);
    stringToIntArray(dataarray, nr_limits, Limit_untis );

    parsejson(appdata , F("Alarm_dabs"), dataarray);
    stringToIntArray(dataarray, nr_limits, Alarm_dabs );

    for ( int i = 0; i <nr_limits ; i++ ) {
      notify_state[i] = 0;
    }

  }
}
