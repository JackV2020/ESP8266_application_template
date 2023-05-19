// ======================================================================
//  App code 
// ( lcl_* is local to this tab and not in *prototypes.h files )
// ======================================================================

void bmeTickerRoutine()
{
  readbme280();
}

// readbme280 needs to shift the data 1 hour in the temperature, humidity and pressure arrays when the hour changes.
// but not after the first hour change after reboot.
// ( think of a number of reboots within the hour and data being shifted 1 hour after every reboot )
int lcl_lastupdatehour = -1;      // so start with lcl_lastupdatehour at -1
int lcl_lastupdateminute = -1;    // the array with 120 minutes data needs an update every minute and we force the first one

float lcl_pressureChange(float floatarray [] ,int arraysize, int startat = 0) {
  float min; float max; int minindex; int maxindex;
  float result ;
  min = floatarray[startat]; minindex = startat ;
  max = floatarray[startat]; maxindex = startat ;
  for ( int i = startat + 1 ; i < arraysize ; i++ ){
    if (floatarray[i] < min ) {min = floatarray[i] ; minindex = i ;}
    if (floatarray[i] > max ) {max = floatarray[i] ; maxindex = i ;}
  }
  if ( maxindex > minindex ) { result = max - min ; } else { result = min - max ; }
  return result;
}

void readbme280() {

// read bme280 or work with dummy data when in demo mode
  if (bmestatus or demomode ) {

    if ( demomode ) {
      Temperature = 25.25 + float(random(-300,300)) / 100.0 ;
      Pressure    = 1018.12 + float(random(-300,300)) / 100.0 ;
      Humidity    = 44.25 + float(random(-300,300)) / 100.0 ;
    } else {
      Temperature = bme.readTemperature();
      Pressure    = bme.readPressure() / 100.0F;
      Humidity    = bme.readHumidity();
    }

    Temperatures[datahours]  = Temperature;
    Pressures[datahours]     = Pressure;
    Humidities[datahours]    = Humidity;

    if ( (firstTimeUpdateAfterConnect > 0 ) and ( lcl_lastupdatehour != tmstruct.tm_hour ) ) {  // As soon as we have the right time and we have a change of hour......
      if (lcl_lastupdatehour > -1 ) {                   // only during real hour changes we shift data ( so skip this immediately after reboot.)
        for (int i = 0 ; i < datahours ; i++) {
          Temperatures[i] = Temperatures[i+1];
          Pressures[i]    = Pressures[i+1];
          Humidities[i]   = Humidities[i+1];
        }
        saveAppData();
      }
      lcl_lastupdatehour = tmstruct.tm_hour;
    }

    real_temperature = Temperature + Temperature_offset;
    real_pressure = Pressure + Pressure_offset;
    real_humidity = Humidity + Humidity_offset;

// bookkeeping pressure / minute and calculate hPa/hour and hPa/2hours

// note that this array is initialised at real_pressure at boot time so it will only produce the right values after 2 hours of uptime

    if ( lcl_lastupdateminute == -1 ) {
        for ( int i = 0; i < 121 ; i++ ) {
          PressureMinutes[i] = Pressure;
        }
        if (firstTimeUpdateAfterConnect > 0 ) { lcl_lastupdateminute = tmstruct.tm_min; }
    } else {
      if ( lcl_lastupdateminute != tmstruct.tm_min ) {
//        String toprint = "";
        lcl_lastupdateminute = tmstruct.tm_min;
        for ( int i = 0; i < 120 ; i++ ) {
          PressureMinutes[i] = PressureMinutes[i+1];
//        toprint= toprint + " ["+String(i)+"] : "+String(PressureMinutes[i]);
        }
        PressureMinutes[120] = Pressure ;
//        toprint= toprint + " [120] : "+String(PressureMinutes[120]);
//        Serial.println(toprint);
      }

    }

//    hour_hpa1hour = PressureMinutes[120] - PressureMinutes[60]; //  60 minutes ago
//    hour_hpa2hour = PressureMinutes[120] - PressureMinutes[0];  // 120 minutes ago
    hour_hpa1hour = lcl_pressureChange(PressureMinutes,121,60); // start at minute 60
    hour_hpa2hour = lcl_pressureChange(PressureMinutes,121,0);  // start at minute  0

// dewpoint and humidex

    float X = log(real_humidity / 100) + (17.62 * real_temperature / (243.12 + real_temperature)) ;
    real_dewpoint = 243.12 * X / (17.62 - X) ;
    real_humidex = real_temperature + ( 6.11 * pow(2.71828, ( 5417.7530 * ( ( 1.0 / 273.16 ) - ( 1.0 / (273.15 + real_dewpoint) ) ) ) ) - 10.0 ) * 5.0 / 9.0 ;


// bookkeeping alarms

    for ( int i=0; i < nr_limits ; i++ ){
      switch (Alarm_dabs[i]) {
        case 0: // disabled
          notify_state[i] = 0;
        break;
        case 1: // above
          switch (notify_state[i]) {
            case 0: // not in alarm but we may have to go into alarm
              switch (Limit_untis[i]) {
                case 0: if ( real_temperature > Limit_values[i] ) { notify_state[i] = 1 ; } break; // ask to send alarm set notification
                case 1: if ( real_pressure    > Limit_values[i] ) { notify_state[i] = 1 ; } break;
                case 2: if ( real_humidity    > Limit_values[i] ) { notify_state[i] = 1 ; } break;
                case 3: if ( real_dewpoint    > Limit_values[i] ) { notify_state[i] = 1 ; } break;
                case 4: if ( real_humidex     > Limit_values[i] ) { notify_state[i] = 1 ; } break;
                case 5: if ( hour_hpa1hour    > Limit_values[i] ) { notify_state[i] = 1 ; } break;
                case 6: if ( hour_hpa2hour    > Limit_values[i] ) { notify_state[i] = 1 ; } break;
              }
            break ;
            case 2: notify_state[i] = 1 ; break ; // 2 :: alarm set notification sending failed so retry by setting to 1
            case 3: // in alarm state, notification succesfull but we may need to clear
              switch (Limit_untis[i]) {
                case 0: if ( real_temperature < Limit_values[i] - dead_band ) { notify_state[i] = 4 ; } break; // ask to send alarm clear notification
                case 1: if ( real_pressure    < Limit_values[i] - dead_band ) { notify_state[i] = 4 ; } break;
                case 2: if ( real_humidity    < Limit_values[i] - dead_band ) { notify_state[i] = 4 ; } break;
                case 3: if ( real_dewpoint    < Limit_values[i] - dead_band ) { notify_state[i] = 4 ; } break;
                case 4: if ( real_humidex     < Limit_values[i] - dead_band ) { notify_state[i] = 4 ; } break;
                case 5: if ( hour_hpa1hour    < Limit_values[i] - dead_band ) { notify_state[i] = 4 ; } break;
                case 6: if ( hour_hpa2hour    < Limit_values[i] - dead_band ) { notify_state[i] = 4 ; } break;
              }
            break ;
            case 5: notify_state[i] = 4 ; break ; // 5 :: alarm clear notification sending failed so retry by setting to 4
            default : break ;
          }
        break;
        case 2: // below
          switch (notify_state[i]) {
            case 0: // not in alarm but we may have to go into alarm
              switch (Limit_untis[i]) {
                case 0: if ( real_temperature < Limit_values[i] ) { notify_state[i] = 1 ; } break; // ask to send alarm set notification
                case 1: if ( real_pressure    < Limit_values[i] ) { notify_state[i] = 1 ; } break;
                case 2: if ( real_humidity    < Limit_values[i] ) { notify_state[i] = 1 ; } break;
                case 3: if ( real_dewpoint    < Limit_values[i] ) { notify_state[i] = 1 ; } break;
                case 4: if ( real_humidex     < Limit_values[i] ) { notify_state[i] = 1 ; } break;
                case 5: if ( hour_hpa1hour    < Limit_values[i] ) { notify_state[i] = 1 ; } break;
                case 6: if ( hour_hpa2hour    < Limit_values[i] ) { notify_state[i] = 1 ; } break;
              }
            break ;
            case 2: notify_state[i] = 1 ; break ; // 2 :: alarm set notification sending failed so retry by setting to 1
            case 3: // in alarm state, notification succesfull but we may need to clear
              switch (Limit_untis[i]) {
                case 0: if ( real_temperature > Limit_values[i] + dead_band ) { notify_state[i] = 4 ; } break; // ask to send alarm clear notification
                case 1: if ( real_pressure    > Limit_values[i] + dead_band ) { notify_state[i] = 4 ; } break;
                case 2: if ( real_humidity    > Limit_values[i] + dead_band ) { notify_state[i] = 4 ; } break;
                case 3: if ( real_dewpoint    > Limit_values[i] + dead_band ) { notify_state[i] = 4 ; } break;
                case 4: if ( real_humidex     > Limit_values[i] + dead_band ) { notify_state[i] = 4 ; } break;
                case 5: if ( hour_hpa1hour    > Limit_values[i] + dead_band ) { notify_state[i] = 4 ; } break;
                case 6: if ( hour_hpa2hour    > Limit_values[i] + dead_band ) { notify_state[i] = 4 ; } break;
              }
            break ;
            case 5: notify_state[i] = 4 ; break ; // 5 :: alarm clear notification sending failed so retry by setting to 4
            default : break ;
          }
        break;
      }
    }

  }

  if ( ( (Humidity == 0 ) or (Humidity == 100 ) or (String(Humidity) == F("nan") ) ) and (rebootRequestCounter == 0) ) {
    rebootRequestCounter = 60 ;
    rebootRequestReason = F("BME280 error");
    bmestatus = false;
  }

}
