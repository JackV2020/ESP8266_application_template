// ======================================================================
//  Prototypes for functions in tabs 2x
// ======================================================================

// 23_web

void serve_setup_app (AsyncWebServerRequest * request, String mode);
void serve_json (AsyncWebServerRequest * request);
void serve_change_variable (AsyncWebServerRequest * request);

// 27_files

void saveAppData();
void readAppData();

// 28_general

String getHttp(String url);
String postHttp(String url, String message);
String getHttps(String url);
String postHttps(String url, String message);
String dropmessage(String &url, String requestType, String &payload);
String dropMail(String messageSubject, String messageToSend);
