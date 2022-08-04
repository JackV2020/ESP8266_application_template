// ======================================================================
//  Prototypes for functions in tabs 2x
// ======================================================================

// 37_files

// For a directory listing I would need to call 
//    listDir("/","",true to get a directory listing
// The next enables me to call listDir("/")

void listDir( String path, String indent ="" , bool level0 = true );

// 13_app

void bmeTickerRoutine();
void readbme280();

// 13_web

void serve_testmedia(AsyncWebServerRequest *request);
void serve_barometer(AsyncWebServerRequest * request);
