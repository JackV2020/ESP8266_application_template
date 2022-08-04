// ======================================================================
//  Toolbox with functions, do not remove, just add general functions
// ======================================================================
/*

List of functions :

String getHttp(String url);
String postHttp(String url, String message);
String getHttps(String url);
String postHttps(String url, String message);
String dropmessage(String &url, String requestType, String &payload);
String dropMail(String messageSubject, String messageToSend);

*/

String getHttp(String url) {

// Get a complete webpage as answer and return it as a string

  String answer = F("Error");
  WiFiClient client;
  HTTPClient http;
  if (http.begin(client, url)) {
    int httpCode = http.GET();    // start connection and send HTTP header
    if (httpCode > 0) {           // HTTP header has been send and Server response header has been handled
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        answer = http.getString();
      }
    } else {                      // httpCode will be negative on error
      Serial.println(F("[getHttp] GET... failed, error: ")+ http.errorToString(httpCode));
    }
    http.end();
  } else {
    Serial.println(F("[getHttp] Unable to connect"));
  };
  return answer;
}

String postHttp(String url, String message) {

// Post message to an url and return answer as a string

  String answer = F("Error");
  WiFiClient client;
  HTTPClient http;
  if (http.begin(client, url)) {
//    http.addHeader("F(Content-Type"), F("application/json"));
    http.addHeader(F("Content-Type"), F("application/x-www-form-urlencoded"));
    int httpCode = http.POST(message);
    if (httpCode > 0) {                 // HTTP header has been send and Server response header has been handled
      if (httpCode == HTTP_CODE_OK) {
        answer = http.getString();
//        Serial.println("received payload:\n<<");
//        Serial.println(payload);
//        Serial.println(">>");
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }
  return answer;
}

String getHttps(String url) {

// Get a complete webpage as answer and return it as a string

  String answer = F("Error");
  WiFiClientSecure secureclient;
  // a magig line below which makes it trust all certificates
  // gold for self signed certificates ;-)
  secureclient.setInsecure();
  HTTPClient httpsclient;

//  client.connect(url, httpsPort);

  if (httpsclient.begin(secureclient, url)) {
    int httpCode = httpsclient.GET();    // start connection and send HTTP header
    if (httpCode > 0) {           // HTTP header has been send and Server response header has been handled
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        answer = httpsclient.getString();
      }
    } else {                      // httpCode will be negative on error
      Serial.println(F("[getHttps] GET... failed, error: ")+ httpsclient.errorToString(httpCode));
    }
    httpsclient.end();
  } else {
    Serial.println(F("[getHttps] Unable to connect"));
  };

  Serial.println(F("[getHttps] result for ")+url+F(" : ====(")+answer+F(")===="));

  return answer;
}

String postHttps(String url, String message) {

// Post message to an url and return answer as a string

  String answer = F("Error");
  WiFiClientSecure secureclient;
  // a magig line below which makes it trust all certificates
  // gold for self signed certificates ;-)
  secureclient.setInsecure();
  HTTPClient httpsclient;

//    Serial.print("[HTTP] begin...\n");

  if ( httpsclient.begin(secureclient, url) ) {
    httpsclient.addHeader(F("Content-Type"), F("application/x-www-form-urlencoded"));
    int httpCode = httpsclient.POST(message);
    if (httpCode > 0) {

//      Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      if (httpCode == HTTP_CODE_OK) {
        answer = httpsclient.getString();
//        Serial.println("received answer:\n<<");
//        Serial.println(answer);
//        Serial.println(">>");
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", httpsclient.errorToString(httpCode).c_str());
    }

    httpsclient.end();
  } else {
    Serial.println(F("[getHttps] Unable to connect"));
  }
  return answer;
}


// ======================================================================
//  dropmessage routine <<Do not change>>
// ======================================================================

String dropmessage(String &url, String requestType, String &payload) {

// File to be created in droppoint inbox : mail.<uniquenumber>
// content : [{"requestType":"mail","payload":"the payload you send"}]

// note that submit is with a small s, a big S means for the the php that the request comes from a web page and results in a different response.

  String droppointdata = F("formSubmit=submit&requestType=");
  droppointdata.concat(requestType);
  droppointdata.concat(F("&payload="));
  droppointdata.concat(payload);
  String poststatus ="Error";
// use http or https
// poststatus = postHttp( url, droppointdata);
   poststatus = postHttps( url, droppointdata);
//  Serial.println(">> poststatus >>"+poststatus);
  return poststatus;
}
// ======================================================================
//  dropmMail routine <<Do not change>>
// ======================================================================

String dropMail(String messageSubject, String messageToSend) {

  static const char payloadTemplate[] PROGMEM = R"====({'m_ena':'M_ENA' , 'm_ssl':'M_SSL' , 'a_id':'A_ID' , 'm_prt':'M_PRT' , 'm_to':'M_TO' , 'm_usr':'M_USR' , 'm_hst':'M_HST' , 'm_pwd':'M_PWD' , 'm_dp':'M_DP' , 'm_su':'M_SU' , 'm_msg':'M_MSG'})====";

  if ( ( email_enabled != F("yes") ) or ( demomode ) ) {
    return F("Mail disabled!");
  } else {

    String payload;
    payload = String(payloadTemplate);

    payload.replace(F("M_ENA"),email_enabled);
    payload.replace(F("M_SSL"),F("yes"));
    payload.replace(F("A_ID") ,sta_hostname);
    payload.replace(F("M_PRT"),smtp_port);
    payload.replace(F("M_TO") ,email_to);
    payload.replace(F("M_USR"),email_login);
    payload.replace(F("M_HST"),smtp_server);
    payload.replace(F("M_PWD"),email_password);
    payload.replace(F("M_DP") ,droppoint);

    payload.replace(F("M_SU") ,messageSubject);
    payload.replace(F("M_MSG"),messageToSend);

    String poststatus;
    poststatus = dropmessage(droppoint,"mail",payload);
//  Serial.println(">> poststatus >>"+poststatus);
    if (poststatus != F("Error")) {
      poststatus = F("Message sent!");
    }
    return poststatus;
  }
}
