// ======================================================================
//  APP variables whose values you may change <<Do not change names>>
// ======================================================================

// !!!!!!!!!! W A R N I N G !!!!!!!!!!!!! >>>>>>> Do not change the names

String app_name = "Barometer";  // label on homepage button

static const char appVersion[] PROGMEM = R"====(V.2023.04.11)====";   // appVersion number yyyy.mm.dd as shown in WiFi_mgr and json page

static const int maxSize_system_log = 5000;                           // In characters and used by log() for appendFile / prependFile

static const char default_app_password[] PROGMEM = ".";               // keep this simple
String app_password = default_app_password;                           // Used to protect settings on mail setup page, startvalue is default_app_password which is a "."
static const char app_password_secret[] PROGMEM = "SecretPassword"; // When you changed the password and forgot it, this can be used to reset to default_app_password "."

static const char app_password_placeholder[] PROGMEM = "Barometer ID (required)" ; // Placeholder as info in app_password field on web setup page
static const char app_show_password_label[] PROGMEM = "Show Barometer ID" ;        // Label for show app_password checkbox on web setup page

/*
// ----- ftp emergency account further below

ftp user 1 is sta_hostname with password app_password
ftp user 2 is the emergency account

suppose the next situation :
  - your device does not get on the WiFi
  - you need to reconfigure WiFi in the Access Point homepage with the WiFi manager
  - you forgot the password
  - fact is you can not change the password on the WiFi manager page
the way to get in is :
  - start FileZilla, create a session with emergency account user and password as you set up below
  - on the transfer settings of FileZilla configure maximum number off connections to only 1
  - connect to the Access Point WiFi
  - start your ftp session
  - delete 1 file only : configuration.txt
  - leave other files in place
  - now you can use the WiFi manager with the default_app_password
  - also you need to enter the details of the fist app setup page again

To make life easier you can also change the WiFi manager. See the notes in 'void serve_WiFi_info'

*/
String ftp_emergency_usr="FtpEmergency";              
String ftp_emergency_pwd=String(app_password_secret)+"_FTP";  
