// ======================================================================
//  App code 
// ( lcl_* is local to this tab and not in *prototypes.h files )
// ======================================================================

void sysinfo (String location) {

  Serial.println(F("\n+---------- sysinfo at : ")+location);

  Serial.println(F("|\n|getChipId            : ")+String(ESP.getChipId()));
  Serial.println(F("|getFlashChipId       : ")+String(ESP.getFlashChipId()));
  Serial.println(F("|getFlashChipRealSize : ")+String(ESP.getFlashChipRealSize()));
  Serial.println(F("|getFlashChipSize     : ")+String(ESP.getFlashChipSize()));
  Serial.println(F("|getFlashChipSpeed    : ")+String(ESP.getFlashChipSpeed()));
  Serial.println(F("|getFlashChipMode     : ")+String(ESP.getFlashChipMode()));

  Serial.println(F("|\n|getResetReason       : ")+String(ESP.getResetReason()));

  Serial.println(F("|\n|FS totalBytes        : ")+String(LittleFSsize('t')));
  Serial.println(F("|FS usedBytes         : ")+String(LittleFSsize('u')));
  Serial.println(F("|FS freeBytes         : ")+String(LittleFSsize('f')));

  Serial.println(F("|\n|getSketchSize        : ")+String(ESP.getSketchSize()));
  Serial.println(F("|getFreeSketchSpace   : ")+String(ESP.getFreeSketchSpace()));

  Serial.println(F("|\n|getFreeHeap          : ")+String(ESP.getFreeHeap()));
  Serial.println(F("|getHeapFragmentation : ")+String(ESP.getHeapFragmentation()));
  Serial.println(F("|getMaxFreeBlockSize  : ")+String(ESP.getMaxFreeBlockSize()));

//  Serial.println(F("|\n|html_response size   : ")+String(html_response.length()));

//  Serial.println(F("|\n|log file size        : ")+String(fileSize(file_system_log)));
//  Serial.println(F("|config file size     : ")+String(fileSize(file_config)));
//  Serial.println(F("|data file size       : ")+String(fileSize(file_appdata)));

  Serial.println(F("|\n+---------- sysinfo ----------\n"));
}

void build_sta_ssids(){

// Fill the array sta_ssids with all SSIDs we see published on WiFi
// called immediately after reboot because WiFi needs to be down.

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  Serial.println(F("Scan Network for SSID's... "));
  int n = WiFi.scanNetworks();
  
  for (int i = 0; i < n; i++)
  {
//    Serial.println(String(i)+" "+WiFi.SSID(i));
    sta_ssids[i]=WiFi.SSID(i);
  }
}

// ======================================================================
//  Build drop down list for web page
// ======================================================================

String drop_down_SSIDs(String dd_name, String dd_id){

  String result = F("<select style=\"width: 200px;\" name=\"");
  result.concat(dd_name);
  result.concat(F("\" id=\""));
  result.concat(dd_id);
  result.concat(F("\"><optgroup label=\"To refresh list, click Refresh.\">"));
  for (int i = 0; i < max_ssids; i++)
  {
    result.concat(F("<option value=\""));
    result.concat(sta_ssids[i]);
    result.concat(F("\">"));
    result.concat(sta_ssids[i]);
    result.concat(F("</option>"));
  }
  result.concat(F("</optgroup></select>"));
  result.replace(F("value=\"")+sta_ssid+F("\""),F("value=\"")+sta_ssid+F("\" selected"));
  return result;
}




//void flip_LED(String by = "") {
void flip_LED(String by) {
// Flip the onboard led and write a message
  if ( by != F("") ) {Serial.println( F("flip_LED by : ") + by );}
  digitalWrite(LED_BUILTIN, ! digitalRead(LED_BUILTIN));
}

bool parsejson(String & json , String key, String & value){
// My own routine to parse 1 dimensional json strings
//    note that everything is returned as String
//    so when you expect something else like int or float
//    you still need to convert

// First check if the key is there
  if ( json.indexOf(key) < 0 ) { return false ; }

  value = json.substring( json.indexOf(key) + key.length() + 2);  // copy last part of json starting at the value we want
  if (value.substring(0,1) == F("\"")) {                          // this is a value between 2 "'s
    if (value.indexOf(F(" , "),1) > 0 ) {
      value = value.substring(1,value.indexOf(F(" , "),1) - 1);     //    so get the value before the " , "
    } else {
      value = value.substring(1,value.length()-2);                //    so remove "} from the end to get the value
    }  
  } else {                                                        // this is a number value without "'s
    if (value.indexOf(F("\"")) == -1 ) {                          //    this means there is no " anymore so no next fiels and this is the last field
      value = value.substring(0,value.length()-1);                //        so just remove the ending }
    } else {                                                      //    there is  still another field following so this is not the last field
      value = value.substring(0,value.indexOf(" , "));            //        so get until seperating ,
    }
  }
  return true;
}

String IpAddress2String(const IPAddress& ipAddress) {
// My own routine to get real IP address, also when not connected
// Because the standard :
//    String gateway = WiFi.gatewayIP().toString().c_str();
//    returns "(IP unset)", when it is actually "0.0.0.0"
  String result;
  result = String(ipAddress[0]);
  result.concat(F("."));
  result.concat(String(ipAddress[1]));
  result.concat(F("."));
  result.concat(String(ipAddress[2]));
  result.concat(F("."));
  result.concat(String(ipAddress[3]));
  return result;
}

String floatArrayToString(float floatarray [] ,int arraysize, int startat) {
// when saving data or sending it to a graph we need to convert arrays to strings and
// when we need to put a part of an array on a graph we often need a starting point != 0
  String result = "[";
  for ( int i = startat ; i < arraysize ; i++ ){
    result.concat(floatarray[i]);
    if (i < ( arraysize - 1)  ) { result.concat(F(",")) ; }
  }
  result.concat("]");
  return result;
}

String intArrayToString(int intarray [] ,int arraysize , int startat) {
// when saving data or sending it to a graph we need to convert arrays to strings and
// some day we may need the option to not start at index 0
  String result = "[";
  for ( int i = startat ; i < (arraysize) ; i++ ){
    result.concat(intarray[i]);
    if (i < (arraysize -1) ) { result.concat(F(",")) ; }
  }
  result.concat("]");
  return result;
}

void stringToFloatArray (String & string ,int arraysize , float values[]){
// when the string is empty,
//  this routine build an array with 'arraysize' elements filled with 0's
// when the string is not empty 
//  it expects a string with enough elements to fill the aray
  if(string.length() > 0) {
    string.replace(F("["),F(""));
    string.replace(F("]"),F(""));
    String value;
    int last;
    int index = -1;
    while (string.length() > 0 ) {
      last = string.indexOf(F(","));
      if (last == -1 ) { last = string.length(); }

      value = string.substring(0,last);
      index = index + 1;
      values[index] = value.toFloat();

      string = string.substring(last+1,string.length());
    }
  } else {
    for (int i=0 ; i < arraysize ; i++ ) {
      values[i] = 0;
    }
  }
}

void stringToIntArray (String &string ,int arraysize , int values[]){
// when the string is empty,
//  this routine build an array with 'arraysize' elements filled with 0's
// when the string is not empty 
//  it expects a string with enough elements to fill the aray
  if(string.length() > 0) {
    string.replace(F("["),F(""));
    string.replace(F("]"),F(""));
    String value;
    int last;
    int index = -1;
    while (string.length() > 0 ) {
      last = string.indexOf(F(","));
      if (last == -1 ) { last = string.length(); }

      value = string.substring(0,last);
      index = index + 1;
      values[index] = value.toInt();

      string = string.substring(last+1,string.length());
    }
  } else {
    for (int i=0 ; i < arraysize ; i++ ) {
      values[i] = 0;
    }
  }
}

String sencdec(String input, char key0, char key1, char key2) {
/*
sencdec : s(ymmetric) enc(ryption) dec(ryption) is where encryption and decryption use the same algorithm and keys.

Parameters :
  input : the string to be encrypted / decrypted
  key0  : a value from 0..127
  key1  : another value from 0..127
  key2  : another value from 0..127
 
Algorithm in short :
  Characters in input are XoR-ed with per character changing keys key0, key1 and key2.

Notes:
  1) It uses 3 keys. Why 3 ?
  One key would give a max of 127 different values for the encryption algorithm.
  Two give 127 x 127 and 3 give 127 x 127 x 127 different values for the algorithm.
  The keys are used one by one to encrypt the characters of the input which may
  be unusual for an encryption algorithm I think and may be less easy to crack.
  2) Want more /less keys ?
  Feel free to change the code. Encryption/decryption speed will stay the same.
  3) Communicate with other systems ?
  When data stays in the system you could generate keys based on a hardware property like MAC.
  When you use this encryption method between systems, both need the same algorithm and keys.
  For this you would choose numbers for the keys yourself and use them on both sides.
 
Example which generates keys :

void setup() {
 
  Serial.begin(115200);
  delay(1000);
 
  Serial.println(F("\n---------------------------------\n"));
 
  String password=F("my secret password") ,encoded, decoded;

  char key_0, key_1, key_2;
 
  Serial.println(F("password : ->")+password+F("<-"));

// Generate the keys based on the chip id of the ESP

  key_0 = ESP.getChipId()%124;  // max is 128 however
  key_1 = ESP.getChipId()%121;  // use higher numbers to
  key_2 = ESP.getChipId()%126;  // have more possibilities

  encoded=sencdec(password, key_0, key_1, key_2);
 
  Serial.println(F("encoded  : ->")+encoded+F("<-"));

// You simply decode in the same way :

  decoded=sencdec(encoded, key_0, key_1, key_2);
 
  Serial.println(F("decoded  : ->")+decoded+F("<-"));
 
  Serial.println(F("\n---------------------------------\n"));
}

*/

  String result="", tempStr;

// Buffer to convert 1 string character of 'input' (length 2 bytes byte0=value, byte1=0) to char

  char temp[2];

// Know that C++ uses a null character ( binary value 00000000 ) at the end of a string to mark
// the end of a string and that the encryption uses the XoR function which is ^ to encrypt.
// The keys can be anything from 0 to 127 and an XoR with a character in the not encoded input
// having the same value as a key would generate a null character meaning the end of a string
// which we can not put somewhere in the encoded string because then it would seem to end there.
// So we add 128 to the keys to generate the real encryption keys enc0, enc1 and enc2.

  char enc0, enc1, enc2;
  enc0 = key0 + 128;
  enc1 = key1 + 128;
  enc2 = key2 + 128;

  for (int i = 0 ; i < input.length(); i++ ) {
    tempStr = input.substring(i,i+1);   // get a string length 1 from input string
    tempStr.toCharArray(temp, 2);       // turn it to a character array
    switch (i % 3) {
      case 0: temp[0] = temp[0] ^ enc0; break;  // XoR the character with key1
      case 1: temp[0] = temp[0] ^ enc1; break;  // XoR the character with key2
      case 2: temp[0] = temp[0] ^ enc2; break;  // XoR the character with key3
    }
    result.concat(String(temp[0]));     // append it to the result
  }

  return result;
}

String sendMail(String messageSubject, String messageToSend) {

//  Serial.println("messageToSend : "+messageToSend);

  if ( email_enabled != F("yes") ) {
    return F("Mail disabled!");
  } else {

    ESP_Mail_Session session;

    /* Set the session config */

    session.time.ntp_server = F("pool.ntp.org,time.nist.gov");
    session.time.gmt_offset = 0;
    session.time.day_light_offset = 0;
//    session.time.timezone_env_string = "JST-9"; // for Tokyo
    
    session.time.timezone_env_string = time_zone; // for Tokyo
    
    /* Set the session config */
    session.server.host_name = smtp_server;
    session.server.port = smtp_port.toInt();
    session.login.email = email_login;
    session.login.password = email_password;
    session.login.user_domain = F("");

    /* Declare the message class */
    SMTP_Message message;

    /* Set the message headers */
    message.sender.name = sta_hostname;
    message.sender.email = email_login;
    message.subject = messageSubject;

    /* Add all recipients */
    String email_toes = email_to;
    email_toes.concat(F(";"));
    email_toes.replace(F(","),F(";"));

    String email_to1;

    while (email_toes.length() > 0 ){
      email_to1 = email_toes.substring(0,email_toes.indexOf(F(";")));
      email_toes= email_toes.substring(1+email_toes.indexOf(F(";")));
      message.addRecipient(F("You"), email_to1);
    }

    /*Send HTML message*/
    String htmlMsg = F("<div style=\"color:lime;\"><a title=\"Click to browse to the IP address\"  href=\"");
    if (email_link =="") { htmlMsg.concat(F("http://")+sta_IP+F("/")); }
    else { htmlMsg.concat(email_link); }
    htmlMsg.concat (F("\"><h2>")+sta_hostname+F("</h2></a></div>"));
    htmlMsg.concat(messageToSend);
    htmlMsg.replace(F("°C"),F("&degC"));        // ***** you may want more replacements
    Serial.println("Sendmail:"+htmlMsg);
    message.html.content = htmlMsg.c_str();
    message.text.charSet = F("us-ascii");
    message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

    /*
    //Send raw text message
    String textMsg = "Hello World! - Sent from ESP board";
    message.text.content = textMsg.c_str();
    message.text.charSet = "us-ascii";
    message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

    message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
    message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;
    */

    /* Set the custom message header */
    //message.addHeader("Message-ID: <abcde.fghij@gmail.com>");

    /* Connect to server with the session config */
    if (!smtp.connect(&session)) {
      return F("Can't connect!");
    }

    /* Start sending Email and close the session */
    if (!MailClient.sendMail(&smtp, &message)) {
//    Serial.println("Error sending Email, " + smtp.errorReason());
      return F("Error sending mail!");
    }

    return F("Message sent!");
  }
}

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status)
{
    /* Print the current status */
    Serial.println(status.info());

    /* Print the sending result */
    if (status.success())
    {
        // ESP_MAIL_PRINTF used in the examples is for format printing via debug Serial port
        // that works for all supported Arduino platform SDKs e.g. AVR, SAMD, ESP32 and ESP8266.
        // In ESP32 and ESP32, you can use Serial.printf directly.

        Serial.println("----------------");
        ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount());
        ESP_MAIL_PRINTF("Message sent failed: %d\n", status.failedCount());
        Serial.println("----------------\n");
        struct tm dt;

        for (size_t i = 0; i < smtp.sendingResult.size(); i++)
        {
            /* Get the result item */
            SMTP_Result result = smtp.sendingResult.getItem(i);

            // In case, ESP32, ESP8266 and SAMD device, the timestamp get from result.timestamp should be valid if
            // your device time was synched with NTP server.
            // Other devices may show invalid timestamp as the device time was not set i.e. it will show Jan 1, 1970.
            // You can call smtp.setSystemTime(xxx) to set device time manually. Where xxx is timestamp (seconds since Jan 1, 1970)
            time_t ts = (time_t)result.timestamp;
            localtime_r(&ts, &dt);

            ESP_MAIL_PRINTF("Message No: %d\n", i + 1);
            ESP_MAIL_PRINTF("Status: %s\n", result.completed ? "success" : "failed");
            ESP_MAIL_PRINTF("Date/Time: %d/%d/%d %d:%d:%d\n", dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday, dt.tm_hour, dt.tm_min, dt.tm_sec);
            ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients.c_str());
            ESP_MAIL_PRINTF("Subject: %s\n", result.subject.c_str());
        }
        Serial.println("----------------\n");

        // You need to clear sending result as the memory usage will grow up.
        smtp.sendingResult.clear();
    }
}
