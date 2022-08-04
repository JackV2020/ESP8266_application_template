// ======================================================================
//  Standard includes <<Do not change>>
// ======================================================================

// #include <time.h>            // used but include seems not needed
// #include <FS.h>              // used by LittleFS but include seems not needed
#include <LittleFS.h>           // store settings, log file and user data

#include <Ticker.h>             // used to run repeating routines with fixed interval https://www.arduino.cc/reference/en/libraries/ticker/

#include <ESP8266WiFi.h>        // https://arduino-esp8266.readthedocs.io/en/latest/esp8266WiFi/readme.html

                                // The default gateway is pinged to check if WiFi connection is ok
#include <Pinger.h>             // "IDE > Tools > Manage Libraries" : https://www.technologytourist.com/electronics/2018/05/22/ESP8266-ping-arduino-library.html
extern "C"
{
  #include <lwip/icmp.h>        // needed for icmp packet definitions for Pinger.h
}

#include <ESP_Mail_Client.h>    // https://randomnerdtutorials.com/esp8266-nodemcu-send-email-smtp-server-arduino/

#include <ESPAsyncTCP.h>        // required for ESPAsyncWebServer.h
#include <ESPAsyncWebServer.h>  // https://github.com/me-no-dev/ESPAsyncWebServer

#include <AsyncElegantOTA.h>    // https://randomnerdtutorials.com/esp8266-nodemcu-ota-over-the-air-arduino/

#include <ESP8266HTTPClient.h>  // https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266HTTPClient/src/ESP8266HTTPClient.h

#include <WiFiClientSecure.h>   // for https support

#include <FTPServer.h>          // access the filesystem and is part of library FTPClientServer https://github.com/JackV2020/FTPClientServer

// ======================================================================
//  Standard constants and variables <<Do not change>>
// ======================================================================

String boottime = "";                   // set in loop after first successful time sync

bool  demomode;                         // my standard variable to tell if we are in demo mode

// ---- WiFi AP

String ap_ssid = "SSID";                // will be : sta_hostname + access point IP address ( starts as 'newdevice xxx.yyy.zzz.qqq' )
String ap_pwd = "1234567890";           // leave as is, this is easy to remember 8-)
String ap_IP;                           // IP AP interface

// ---- WiFi STA

static const int max_ssids = 25;        // maximum number of SSIDs in selection list
String sta_ssids[max_ssids];            // during startup this array is filled with available SSID's
String sta_ssid = "SSID";               // Connect to the access point 'newdevice xxx.yyy.zzz.qqq' and browse to the
String sta_pwd = "PWD";                 // IP 'xxx.yyy.zzz.qqq' you see when connecting to 'newdevice xxx.yyy.zzz.qqq'
String sta_hostname = "newdevice";      // and enter SSID, Password and hostname for the WiFi
String sta_IP ="" ;                     // IP STA interface
String time_zone = "GMT0";              // time_zone updated on setup page

// ---- email variables with some default values. ( may be changed from the standard app setup web page )


SMTPSession smtp;                       // The SMTP Session object used for Email sending

String email_enabled = "no";            // yes : mail enabled
String email_login = "";                // account name to logon to provider
String email_password = "";             // the password top log in
String email_from = "";                 // email_from (will be set to email_login)
String email_name_from = "";            // text shown in from field in received mail (will be set to sta_hostname)
String email_to = "";                   // receiver@someprovider.domain
String smtp_server = "smtp.gmail.com";  // smtp server to send mail to
String smtp_port = "465";               // TLS Use port 465 for gmail
String droppoint ="https://veraart.thehomeserver.net:443/droppoint/droppoint.php";    // default value for my droppoint https

// ---- files

static const char file_config[]           PROGMEM = "/configuration.txt";
static const char file_system_log[]       PROGMEM = "/system_log.txt";
static const char file_appdata[]          PROGMEM = "/appdata.txt";

// ----- web

AsyncWebServer WEBserver(80);

String virtualhost;                     // IP / hostname client uses to connect
String html_response;                   // html response sent to client
String client_IP;                       // IP of client

// tell the FtpServer to use LittleFS
FTPServer ftpServer(LittleFS);

// Ticker object to run systemTickerRoutine()
Ticker systemTicker;
