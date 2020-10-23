#include "network.h"
#include "EEPROMStream.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266NetBIOS.h>

MyNetwork::MyNetwork()
{
  #ifdef DEBUG
  DEBUG_PORT.println("Network initialize...");
  #endif DEBUG
  #ifdef JMP_RESET_WIFI
  pinMode(JMP_RESET_WIFI, INPUT_CFG);
  if (digitalRead(JMP_RESET_WIFI) == RESET_LEVEL)
  {
    #ifdef DEBUG
    DEBUG_PORT.println("--- MyNetwork Reset config ------------------- ");
    #endif
    EEPROMStream* stream = new EEPROMStream();
    ResetConfig();
    SaveConfig(stream);
    delete stream;
  }
  #endif JMP_RESET_WIFI
  
  LoadConfig();
  Run();
}

MyNetwork::~MyNetwork()
{
  //delete stream;
}

String MyNetwork::Scan()
{
  #ifdef DEBUG
  DEBUG_PORT.println("Scanning WiFi...");
  #endif DEBUG
  int n = WiFi.scanNetworks();
  String s = "";
  for (int i=0;i<n;i++)
  {
    s+="{\"key\" : \"" + WiFi.SSID(i) + "\"}";
    if (i<n-1) s+=",";
    #ifdef DEBUG
    DEBUG_PORT.println(WiFi.SSID(i));
    #endif DEBUG
  }
  s = "[" + s + "]";
  return s;
}

void MyNetwork::Run()
{
  #ifdef DEBUG
  DEBUG_PORT.println("Network running...");
  #endif DEBUG
  if (_cfg.APMode) {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(_cfg.ssid, _cfg.pass);
    #ifdef DEBUG
    DEBUG_PORT.println("WiFi AP " + String(_cfg.ssid) + "/" + String(_cfg.pass) + " started.");
    #endif DEBUG
  }
  else{
    WiFi.mode(WIFI_STA);
    #ifdef DEBUG
    DEBUG_PORT.println("WiFi begin to " + String(_cfg.ssid) + "/" + String(_cfg.pass));
    #endif DEBUG
    WiFi.begin(_cfg.ssid, _cfg.pass);
    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      #ifdef DEBUG
      DEBUG_PORT.print(".");
      #endif DEBUG
    }
    #ifdef DEBUG
    DEBUG_PORT.println("");
    DEBUG_PORT.println("WiFi connected.");
    #endif DEBUG
    
    if (MDNS.begin(_cfg.moduleName)) {
      MDNS.addService("http", "tcp", 80);
    }
    NBNS.begin(_cfg.moduleName);

    #ifdef DEBUG
    DEBUG_PORT.println("NS name: " + String(_cfg.moduleName));
    #endif DEBUG
  }
}

bool MyNetwork::ValidateString(char* value)
{
  bool valid = true;
  
  //Check to see if the string is an acceptable length
  if((strlen(value) < MIN_STR_LEN) && (strlen(value) >= MAX_STR_LEN)) {
    valid = false;
  }
  else {
    //Check each character in the string to make sure it is alphanumeric or space
    for(uint8_t i = 0; i < strlen(value); i++)
      if(!isAlphaNumeric(value[i]))
        if(!isSpace(value[i]))
          valid = false;
  }
  return valid;
}

void MyNetwork::LoadConfig()
{
  #ifdef DEBUG
  DEBUG_PORT.println("Loading network configuration...");
  #endif DEBUG
  EEPROMStream* stream = new EEPROMStream();
  //stream->Seek(SEEK_TO_FIRST);
  stream->read<config_t>(&_cfg);
  
  bool eepromValid = true;
  eepromValid &= ValidateString(_cfg.ssid);
  eepromValid &= ValidateString(_cfg.pass);
  eepromValid &= ValidateString(_cfg.moduleName);
  if(!eepromValid) {
    ResetConfig();
    SaveConfig(stream);
  }

  delete stream;
  #ifdef DEBUG
  DEBUG_PORT.println("Network configuration loaded.");
  #endif DEBUG
}

void MyNetwork::ResetConfig()
{
  #ifdef DEBUG
  DEBUG_PORT.println("Reset network configuration...");
  #endif DEBUG
  sprintf(_cfg.ssid, "%s %06X", SSID_DEFAULT, ESP.getChipId());
  sprintf(_cfg.pass, PASS_DEFAULT);
  sprintf(_cfg.moduleName, MNAME_DEFAULT);
  _cfg.APMode = true;
}

void MyNetwork::SaveConfig(EEPROMStream* stream)
{
  #ifdef DEBUG
  DEBUG_PORT.println("Save network configuration...");
  #endif DEBUG
  stream->Seek(SEEK_TO_FIRST);
  stream->write<config_t>(&_cfg);
}

bool MyNetwork::isAP()
{
  return _cfg.APMode;
}

void MyNetwork::SetConfig(
      String aSSID,
      String aSSIDkey,
      String aModuleName,
      String aIsAP
      )
{
  #ifdef DEBUG
  DEBUG_PORT.println("Set network configuration...");
  DEBUG_PORT.println("SSID: " + aSSID);
  DEBUG_PORT.println("key: " + aSSIDkey);
  DEBUG_PORT.println("ModuleName: " + aModuleName);
  DEBUG_PORT.println("AP: " + aIsAP);
  #endif DEBUG
  aSSID.toCharArray(_cfg.ssid,MAX_STR_LEN);
  aSSIDkey.toCharArray(_cfg.pass,MAX_STR_LEN);;
  aModuleName.toCharArray(_cfg.moduleName,MAX_STR_LEN);;
  _cfg.APMode = (aIsAP == "Y");
  EEPROMStream* stream = new EEPROMStream();
  SaveConfig(stream);
  delete stream;
}

String MyNetwork::GetConfigJSON()
{
  String ap = "false";
  if (_cfg.APMode) ap = "true";
  String json = "{\"SSID\":\"" + String(_cfg.ssid) + "\","+
          "\"SSIDkey\":\"" + String(_cfg.pass) + "\","+
          "\"mName\":\"" + String(_cfg.moduleName) + "\"," +
          "\"APEnable\":" + ap +
          "}";
  #ifdef DEBUG
  DEBUG_PORT.println("Get network JSON configuration...");
  DEBUG_PORT.println(json);
  #endif DEBUG
  return json;
}
