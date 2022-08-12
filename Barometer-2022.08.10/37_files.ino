// ======================================================================
//  Standard functions LittleFS <<Do not change>>
// ======================================================================

void setupLittleFS() {

// During inital boot the file system will be set up and formatted
// During other boots it will be mounted leaving files in place

  if (!LittleFS.begin()) {
    Serial.println(F("LittleFS mount failed\nFormatting LittleFS filesystem"));
    LittleFS.format();
    if (!LittleFS.begin()) {
      Serial.println(F("Formatting LittleFS filesystem failed"));
    }
  }
}

int LittleFSsize(char what) {
  int result;
  FSInfo fsinfo;
  LittleFS.info(fsinfo);
  switch (what) {
  case 't':    result= fsinfo.totalBytes; break ;
  case 'u':    result= fsinfo.usedBytes;  break ;
  case 'f':    result= (fsinfo.totalBytes - fsinfo.usedBytes);  break ;
  default :    result= fsinfo.totalBytes; break ;
  }
  return result;
}

// Notes on listDir :
//  The next line is in prototypes.h to be able to just call listDir("/").
//    void listDir( String path, String indent ="" , bool level0 = true ); 
//  Because of the recursive algorithm the next are global variables

uint16_t lcl_dirCount;
uint16_t lcl_fileCount;
uint32_t lcl_byteCount;

void listDir( String path, String indent , bool level0)
{
  if (level0) {
    Serial.printf_P(PSTR("\nListing contents for '%s'...\n"),path);
    lcl_dirCount = 0; lcl_fileCount=0; lcl_byteCount=0;
  }
  Dir dir = LittleFS.openDir(path);
  while (dir.next())
  {
    if (dir.isDirectory())
    {
      ++lcl_dirCount;
      Serial.printf_P(PSTR("%s%s [Dir]\n"), indent.c_str(), dir.fileName().c_str());
      listDir( path + dir.fileName() + "/",indent + "  ", false);
    } else {
      ++lcl_fileCount;
      Serial.printf_P(PSTR("%s%-25s"), indent.c_str(), dir.fileName().c_str());

      struct tm * tmstruct;
      File file = dir.openFile("r");

      time_t cr = file.getCreationTime();
      tmstruct = localtime(&cr);
      Serial.printf("  # Created : %d-%02d-%02d %02d:%02d:%02d", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);

      time_t lw = file.getLastWrite();
      tmstruct = localtime(&lw);
      Serial.printf(" # Changed : %d-%02d-%02d %02d:%02d:%02d   (%ld Bytes)\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec, (uint32_t)dir.fileSize());
      lcl_byteCount = lcl_byteCount + (uint32_t)dir.fileSize();
      file.close();
    }
  }
  if (level0) {
    Serial.printf_P(PSTR("Total of %d files, %d subdirs, %d bytes ( FS used %d of %d bytes)\n\n"), lcl_fileCount, lcl_dirCount, lcl_byteCount,LittleFSsize('u'),LittleFSsize('t'));
  }
}

int fileSize(const char * file_name) {

  Dir home = LittleFS.openDir("/");

  int filesize = 0;

  while (home.next()) {
    File file = home.openFile("r");

    String fileName = "/";
    fileName.concat(home.fileName());

    if ( fileName == String(file_name) ) { filesize = file.size(); }

    file.close();
  }
  return filesize;
}

void readFile(String path, String & data ) {

  File file = LittleFS.open(path, "r");
  if (!file) {
    Serial.println(F("Failed to read ")+path);
    return;
  }

  String single_character;
  data ="";
  while (file.available()) {
    single_character = file.read();
    data.concat(single_character);
  }
  file.close();
}

bool writeFile(String path, String & message) {

  File file = LittleFS.open(path, "w");
  if (!file) {
    Serial.println(F("Failed to open for write ")+path);
    return false;
  }
  if (!file.print(message)) {
    sysinfo(F("writeFile error"));
    Serial.println(F("Failed to write path ")+path);
    Serial.println(F("Failed to write data ")+message);
    listDir("/");
    return false;
    file.close();
  }
  file.close();

  return true;
}

void deleteFile(String path) {
  if (!LittleFS.remove(path)) {
    Serial.println(F("Failed to delete ")+path);
  }
}

void renameFile(String path1, String path2) {
  if (!LittleFS.rename(path1, path2)) {
    Serial.println(F("Failed to rename ")+ path1 +F(" to ")+ path2);
  }
}

String contentOfFile(String path) {

  String data ="";

  File file = LittleFS.open(path, "r");
  if (!file) {
    Serial.println(F("Failed to open for head ")+path);
  return data;
  };

  String single_character;
  while (file.available()) {
    single_character = file.read();
    data.concat(single_character);
  }
  file.close();

  return data;
}

void appendFile(String path, String message, String seperator, int maxSize) {

  if (maxSize == 0 ) {
    File file = LittleFS.open(path, "a");
    if (!file) {
      Serial.println(F("Failed to open for append ")+path);
      writeFile(path, message);
      return;
   }
    if (!file.print(message+seperator)) {
      Serial.println(F("Failed to append ")+message);
    }
    file.close();
  } else {

    String data = "";

    File file = LittleFS.open(path, "r");
    if (file) {

      String single_character;
      while (file.available()) {
        single_character = file.read();
        data.concat(single_character);
        if ( (maxSize > 0) and (data.length() > maxSize ) ) {data.remove(0,data.indexOf(seperator)+seperator.length());}; // remove first line
        }
      file.close();
      }

    data.concat(message);
    data.concat(seperator);
    if (data.length() > maxSize ) {data.remove(0,data.indexOf(seperator)+seperator.length());}; // remove first line

    if (writeFile(F("/tempfile.tmp"),data)) {
        renameFile(F("/tempfile.tmp"),path);
    }
  }

}

void prependFile(String path, String message, String seperator, int maxSize ) {

  String data = message + seperator;

  File file = LittleFS.open(path, "r");
  if (file) {

    String single_character;
    while (file.available()) {
      single_character = file.read();
      if (maxSize == 0 ) {
        data.concat(single_character);
      } else {
        if (data.length() < maxSize ) {
          data.concat(single_character);
        }
      }
    }
    file.close();
  }

  if (data.length() == maxSize ) {
    data.remove(data.lastIndexOf(seperator),data.length()); // remove last incomplete line
  };

  if (writeFile(F("/tempfile.tmp"),data)) {
      renameFile(F("/tempfile.tmp"),path);
  }

  return;
}

// ======================================================================
//  Standard functions Using LittleFS functions above <<Do not change>>
// ======================================================================

void log(String line){

  Serial.println(F("Logging : ")+datetime.CDT +" "+line);

// we log in html format so we can send the log file without modifications to a web page

//  line.replace(F(" "),F("&nbsp;"));
  line.replace(F("\n"),F("<br>"));
  line.replace(F("°C"),F("&degC"));

//  appendFile(String(file_system_log), datetime.CDT +F(" ")+line,F("<br>"), maxSize_system_log );
  prependFile(String(file_system_log), datetime.CDT +F(" ")+line,F("<br>"), maxSize_system_log );
}

void saveConfig(){
/*
  Serial.println(F("--------------------------"));
  Serial.println(F("Save config"));

  String encoded=sencdec(sta_pwd, key0 , key1, key2);
  String decoded=sencdec(encoded, key0 , key1, key2); 
  Serial.println(F("password : ->")+sta_pwd+F("<-"));
  Serial.println(F("encoded  : ->")+encoded+F("<-"));
  Serial.println(F("decoded  : ->")+decoded+F("<-"));
*/

  String config = F("{\"sta_ssid\":\"");          config.concat(sta_ssid);
//  config.concat(F("\" , \"sta_pwd\":\""));        config.concat(sencdec(sta_pwd, key0 , key1, key2));
  config.concat(F("\" , \"sta_pwd\":\""));        config.concat(sencdec(sta_pwd));
  config.concat(F("\" , \"sta_hostname\":\""));   config.concat(sta_hostname);
  config.concat(F("\" , \"time_zone\":\""));      config.concat(time_zone);
  config.concat(F("\" , \"email_enabled\":\""));  config.concat(email_enabled);
  config.concat(F("\" , \"email_login\":\""));    config.concat(email_login);
//  config.concat(F("\" , \"email_password\":\"")); config.concat(sencdec(email_password, key0 , key1, key2));
  config.concat(F("\" , \"email_password\":\"")); config.concat(sencdec(email_password));
  config.concat(F("\" , \"email_to\":\""));       config.concat(email_to);
  config.concat(F("\" , \"email_link\":\""));    config.concat(email_link);
  config.concat(F("\" , \"smtp_server\":\""));    config.concat(smtp_server);
  config.concat(F("\" , \"smtp_port\":\""));      config.concat(smtp_port);
//  config.concat(F("\" , \"app_password\":\""));   config.concat(sencdec(app_password, key0 , key1, key2));
  config.concat(F("\" , \"app_password\":\""));   config.concat(sencdec(app_password));
  config.concat(F("\" , \"droppoint\":\""));      config.concat(droppoint);
  config.concat(F("\"}"));

  writeFile(String(file_config),config);

  Serial.println(F("Saved config data"));

}

void readConfig(){

  String config = "";
  readFile(String(file_config),config);

  if ( config != "" ) { // after initial boot we have nothing stored yet so we stick with default values

    parsejson(config,F("sta_ssid"),sta_ssid);
    parsejson(config,F("sta_pwd"),sta_pwd);
    sta_pwd = sencdec(sta_pwd);
    parsejson(config,F("sta_hostname"),sta_hostname);
    parsejson(config,F("time_zone"),time_zone);
    parsejson(config,F("email_enabled"),email_enabled);
    parsejson(config,F("email_login"),email_login);
    parsejson(config,F("email_password"),email_password);
    email_password = sencdec(email_password);
    parsejson(config,F("email_to"),email_to);
    parsejson(config,F("email_link"),email_link);
    parsejson(config,F("smtp_server"),smtp_server);
    parsejson(config,F("smtp_port"),smtp_port);
    parsejson(config,F("app_password"),app_password);
    app_password = sencdec(app_password);
    parsejson(config,F("droppoint"),droppoint);
  }
}
