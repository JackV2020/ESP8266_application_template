// ======================================================================
//  Standard prototypes  <<Do not change>>
// ======================================================================

// 10_setup_loop

void app_setup();
void app_loop();

// 30_setup_loop

void std_setup();
void std_loop();

// 33_app

void build_sta_ssids();
String drop_down_SSIDs(String dd_name, String dd_id);
void flip_LED(String by = "");
bool parsejson(String & json , String key, String & value);
String IpAddress2String(const IPAddress& ipAddress);
void sysinfo (String location);
String floatArrayToString(float floatarray [] ,int arraysize, int startat);
String intArrayToString(int intarray [] ,int arraysize , int startat);
void stringToFloatArray (String & string ,int arraysize , float values[]);
void stringToIntArray (String &string ,int arraysize , int values[]);

// The next enables us to call sencdec("something to encrypt") without keys
// This is actually used in 37_files by save and read config to encrypt passwords
// So leave the construction in place but feel free to modify the key values.
// Note on sencdec keys values :
//  value for a key is from 1 to 127 ( 0000.0001 .. 0111.1111 )
//
//  ESP.getChipId() gives a unique number for each ESP chip
//  so a more or less 'big' random number which we need to handle
//  some way. Max value after % is 128
//  
String sencdec(String input,
            char key0 = ESP.getChipId() % 126,
            char key1 = ESP.getChipId() % 122,
            char key2 = ESP.getChipId() % 123);

String sendMail(String messageSubject, String messageToSend);
void smtpCallback(SMTP_Status status);

// 33_web

void serve_WiFi_mgr (AsyncWebServerRequest * request );
void serve_WiFi_info (AsyncWebServerRequest * request, String mode);
void serve_home (AsyncWebServerRequest * request, String mode);
void showServing(AsyncWebServerRequest *request);
void notFound(AsyncWebServerRequest *request);
void serve_setup(AsyncWebServerRequest *request);
void serve_start_ota(AsyncWebServerRequest *request);
void serve_log (AsyncWebServerRequest * request);
void serve_setup_system (AsyncWebServerRequest * request, String mode);

// 35_systemTicker

void updateTime();
void systemTickerRoutine();

// 37_files

void setupLittleFS();
bool writeFile(String path, String & message);
void readFile(String path, String & data );
void deleteFile(String path);
void renameFile(String path1, String path2);
void appendFile(String path, String message, String seperator, int maxSize = 0 );
void prependFile(String path, String message, String seperator, int maxSize = 0 );
void log(String line);
void saveConfig();
void readConfig();
String contentOfFile(String path);

//
