// ======================================================================
//  App variable constants etc. used by more than 1 tab
// ======================================================================
/*
Some notes

 Dewpoint formula : https://www.weerschip.nl/formules.html
 float X = log10(Humidity / 100) / log10(2.718282) + (17.62 * Temperature / (243.12 + Temperature)) ;
 Dewpoint = 243.12 * X / (17.62 - X) ;
 Arduino log() is already e-based and not 10 based so the next works just fine :
 float X = log(Humidity / 100) + (17.62 * Temperature / (243.12 + Temperature)) ;
 Dewpoint = 243.12 * X / (17.62 - X) ;

*/

// ---- Make sure we start at first setup page

int setup_page_nr = 1;

//

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme; // I2C

bool bmestatus=false; // true when communication is ok. When we can not connect to bme280 we will reboot unless we are in demo mode.

Ticker bmeTicker;

float bmeTickerInterval = 60 ;  // run bme Ticker every 60 seconds

// results read from bme280

float Temperature = 0;
float Pressure = 0;
float Humidity = 0 ;

// you may want enter an offset on a setup page

float Temperature_offset = 0 ;
float Pressure_offset = 0 ;
float Humidity_offset = 0;

// current results including offsets

float real_temperature = 0;
float real_pressure = 0;
float real_humidity = 0;
float real_dewpoint = 0;  // to which temperature do we need to drop to get dew
float real_humidex = 0;   // some index based on humidity, temperature and pressure indicating how sticke the weather feels
float hour_hpa1hour = 0;  // air pressure change per hour
float hour_hpa2hour = 0;  // air pressure change per 2 hours

// data read from bme280 is stored in 3 arrays ( without offset )

const int datahours = 7 * 24;     // we store 7 days of hourly data

float Temperatures[datahours+1];  // each has room for 7 days + 1 for the last actual value
float Pressures[datahours+1];     //    the last values are on the gauges
float Humidities[datahours+1];    //    and on the rightmost position of the graphs

float PressureMinutes[120 + 1];   // 120 minutes + actual value to calculate hPa/hour and hPa/2hours

// variables for the graphs

float graphdata[datahours+1];     // the actual datapoints can hold the 3 sensor values for Dewpoint, Humidex etc.

String gaugeInterval = "10" ;     // default gauge update interval in seconds
int graphDays = 1;                // default number of days on the line graphs

int barometer_page_nr = 0;        // there are some barometer graphs and we start on page 0 with the gauges

// variables for alarming we can configure on a setup page

const int nr_limits = 7;         // the number of limits can be increased / decreased. Setup page and code take care.

float Limit_values[nr_limits];    // the value limit
int   Limit_untis[nr_limits];     // 0=Temperature, 1=Pressure, 2=Humidity, 3=Dewpoint, 4=Humidex, 5=hPa/hour, 6=hPa/2hours
int   Alarm_dabs[nr_limits];      // 0=disabled, 1=above, 2=below
int   notify_state[nr_limits];    // 0 not in alarm
                                  // 1 alarm activated please send
                                  // 2 alarm activated send failed
                                  // 3 alarm activated send ok/disabled
                                  // 4 alarm cleared please send
                                  // 5 alarm cleared send failed
                                  // 0 alarm cleared send ok/disabled

// 2 handy text strings to translate numeric limit types to text and display units

static const char Limit_units_name_str[][12] PROGMEM = {
 "Temperature"
  ,"Pressure"
  ,"Humidity"
  ,"Dewpoint"
  ,"Humidex"
  ,"hPa/hour"
  ,"hPa/2hours"
};

static const char Limit_units_str[][12] PROGMEM = {
   "°C (temp)"
  ,"hPa"
  ,"%"
  ,"°C (dew)"
  ,"Humidex"
  ,"hPa/hour"
  ,"hPa/2hours"
};

// some strings to translate numeric alarm dabs ( = disabled, above and below 's )

static const char Alarm_dabs_str[][9] PROGMEM = {
 "disabled"
  ,"above"
  ,"below"
};

// note next deadband is currently for all types of alarms and seems to work just fine

float dead_band = 0.5;  // when in alarm value has to change this amount to safe side of limit to clear alarm
