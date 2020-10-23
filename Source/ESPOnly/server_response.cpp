#include "server.h"
#include "AbstractUnit.h"
#include "MyController.h"
#include <FS.h>

void MyWebSRV::sendIndexResponse(WiFiClient wcl)
{
  File f = SPIFFS.open("/index.html", "r");
  if (!f) {
    #ifdef DEBUG
    DEBUG_PORT.println("file open failed");  //  "открыть файл не удалось"
    #endif DEBUG
    SendHTTPResponse(wcl, 0);
  }
  else {
    #ifdef DEBUG
    DEBUG_PORT.print("file opened ");DEBUG_PORT.println(f.size());
    #endif DEBUG
    SendHTTPResponse(wcl, f.size());
    wcl.write(f, f.size());
    f.close();
  }
}

// WiFi config pages
void MyWebSRV::sendWifiScan(WiFiClient wcl)
{
  #ifdef DEBUG
  DEBUG_PORT.print("WifiScan send json ");
  #endif DEBUG
  String payload = Network->Scan();
  String s = "";
  s += F("HTTP/1.1 200 OK\n");
  s += F("Content-Type: application/x-www-form-urlencoded\n"); // text/plain
  s += F("Content-Length: ");
  s += String(payload.length());
  s += F("\n");
  s += F("Connnection: close\n");
  wcl.println(s);
  wcl.println(payload);
  #ifdef DEBUG
  DEBUG_PORT.println(payload);
  #endif DEBUG
}

void MyWebSRV::sendWifiReboot(WiFiClient wcl)
{
  SendHTTPResponse(wcl, 0);
  ESP.restart();
}

void MyWebSRV::sendWifiLoad(WiFiClient wcl)
{
  #ifdef DEBUG
  DEBUG_PORT.print("WifiLoad send json ");
  #endif DEBUG
  String payload = Network->GetConfigJSON();
  String s = "";
  s += F("HTTP/1.1 200 OK\n");
  s += F("Content-Type: application/x-www-form-urlencoded\n"); // text/plain
  s += F("Content-Length: ");
  s += String(payload.length());
  s += F("\n");
  s += F("Connnection: close\n");
  wcl.println(s);
  wcl.println(payload);
  #ifdef DEBUG
  DEBUG_PORT.println(payload);
  #endif DEBUG
}

void MyWebSRV::sendWifiSave(WiFiClient wcl, String payload)
{
  String aSSID = "";
  String aSSIDkey = "";
  String aModuleName = "";
  String aIsAP = "";
  
  String p = payload;
  int n = -1;
  n = p.indexOf("\n");
  while (n>=0)
  {
    String prm = p.substring(0,n);
    int d = prm.indexOf(":");
    String key = prm.substring(0,d);
    String val = prm.substring(d+1,prm.length());
    if (key == "SSID") aSSID = val;
    else
    if (key == "SSIDkey") aSSIDkey = val;
    else
    if (key == "MOD") aModuleName = val;
    else
    if (key == "AP") aIsAP = val;
    #ifdef DEBUG
      DEBUG_PORT.print("prm = ");DEBUG_PORT.println(prm);
      DEBUG_PORT.print("key = ");DEBUG_PORT.println(key);
      DEBUG_PORT.print("val = ");DEBUG_PORT.println(val);
    #endif DEBUG
    p = p.substring(n+1,p.length());
    if (p == "") n = -1; else n = p.indexOf("\n");
  }
  Network->SetConfig(
      aSSID,
      aSSIDkey,
      aModuleName,
      aIsAP
      );
  wcl.println("ok");
}

// Device collection
void MyWebSRV::sendDevList(WiFiClient wcl)
{
  #ifdef DEBUG
      DEBUG_PORT.println("MyWebSRV::sendDevList");
    #endif DEBUG
  wcl.println(Controller->GetDeviceListJSON(3));
}

void MyWebSRV::sendDevAdd(WiFiClient wcl, String payload)
{
  #ifdef DEBUG
    DEBUG_PORT.print("sendDevAdd payload=");DEBUG_PORT.println(payload);
  #endif DEBUG
  long s_time = millis();
  String prm;
  byte devcode=255;
  
  // extract device configuration
  Load(payload);
  devcode = getValueByKey("devcode").toInt();
  prm = getValueByKey("params");
  
  // create device
  AbstractUnit * dev = Controller->GetDeviceInstance(devcode);
  #ifdef DEBUG
    DEBUG_PORT.print("sendDevAdd GetDeviceInstance devcode = ");DEBUG_PORT.println(devcode);
  #endif DEBUG
  try
  {
    dev->SetParams(prm);
  }
  catch(int& ErrorCode)
  {
    #ifdef DEBUG
      //DEBUG_PORT.print("--- AbstractUnit sendDevAdd Exception ");DEBUG_PORT.println(ErrorCode);
      DEBUG_PORT.print("--- AbstractUnit prm = ");DEBUG_PORT.println(prm);
    #endif
  }
  #ifdef DEBUG
    DEBUG_PORT.println("sendDevAdd SetParams");
    //DEBUG_PORT.print("moduleid = ");DEBUG_PORT.println(moduleid);
    //DEBUG_PORT.print("pin = ");DEBUG_PORT.println(pin);
    //DEBUG_PORT.print("key = ");DEBUG_PORT.println(vkey);
  #endif DEBUG
  // device created
  
  // Add device to controller
  Controller->AddDevice(dev);
  EEPROMStream * rom = new EEPROMStream();
  rom->Seek(EEPROM_POSITION);
  Controller->SaveToStream(rom);
  delete rom;
  wcl.println("[ok]");
  
}

void MyWebSRV::sendDevDel(WiFiClient wcl, String payload)
{
  byte moduleid=255;
  long s_time = millis();
  // extract device
  String p = payload;
  int n = -1;
  n = p.indexOf("\n");
  while (n>=0)
  {
    String prm = p.substring(0,n);
    int d = prm.indexOf(":");
    String key = prm.substring(0,d);
    String val = prm.substring(d+1,prm.length());

    if (key == "moduleid") moduleid = val.toInt();

    p = p.substring(n+1,p.length());
    if (p == "") n = -1; else n = p.indexOf("\n");
  }

  Controller->DeleteDevice(moduleid);
  EEPROMStream * rom = new EEPROMStream();
  rom->Seek(EEPROM_POSITION);
  Controller->SaveToStream(rom);
  delete rom;
  wcl.println("[ok]");
}

void MyWebSRV::sendUnitsLoad(WiFiClient wcl, String payload)
{
  byte tabpage=255;
  long s_time = millis();
  #ifdef DEBUG
    DEBUG_PORT.println(" *** sendUnitsLoad ***");
  #endif DEBUG
  // extract collection id
  String p = payload;
  int n = -1;
  n = p.indexOf("\n");
  while (n>=0)
  {
    String prm = p.substring(0,n);
    int d = prm.indexOf(":");
    String key = prm.substring(0,d);
    String val = prm.substring(d+1,prm.length());

    if (key == "tabpage") tabpage = val.toInt();

    p = p.substring(n+1,p.length());
    if (p == "") n = -1; else n = p.indexOf("\n");
  }
  // end extract collection id
  #ifdef DEBUG
    DEBUG_PORT.print("tabpage = ");DEBUG_PORT.println(tabpage);
  #endif DEBUG
  wcl.println(Controller->GetDeviceListJSON(tabpage));
}

void MyWebSRV::sendOperate(WiFiClient wcl, String payload)
{
  byte id = 255;
  String data="";
  //long s_time = millis();
  #ifdef DEBUG
    DEBUG_PORT.println(" *** sendOperate ***");
  #endif DEBUG
  // extract collection id
  String p = payload;
  if (p == "")
  {
    SendHTTPError(wcl);
    return;
  }
  int n = -1;
  n = p.indexOf("\n");
  while (n>=0)
  {
    String prm = p.substring(0,n);
    int d = prm.indexOf(":");
    String key = prm.substring(0,d);
    String val = prm.substring(d+1,prm.length());

    if (key == "moduleid") id = val.toInt();
    if (key == "data") data = val;
    p = p.substring(n+1,p.length());
    if (p == "") n = -1; else n = p.indexOf("\n");
  }

  Controller->setModuleData(id, data);
  
  String resp = Controller->getModuleData(id);
  SendHTTPResponse(wcl, resp.length());
  wcl.println(resp);
}

void MyWebSRV::sendUpdate(WiFiClient wcl, String payload)
{
  byte id = 255;
  String data="";
  //long s_time = millis();
  #ifdef DEBUG
    DEBUG_PORT.println(" *** sendUpdate ***");
  #endif DEBUG
  // extract collection id
  String p = payload;
  if (p == "")
  {
    SendHTTPError(wcl);
    return;
  }
  int n = -1;
  n = p.indexOf("\n");
  while (n>=0)
  {
    String prm = p.substring(0,n);
    int d = prm.indexOf(":");
    String key = prm.substring(0,d);
    String val = prm.substring(d+1,prm.length());

    if (key == "moduleid") id = val.toInt();
    if (key == "data") data = val;
    
    p = p.substring(n+1,p.length());
    if (p == "") n = -1; else n = p.indexOf("\n");
  }

  Controller->setModuleData(id, data);
  EEPROMStream * rom = new EEPROMStream();
  rom->Seek(EEPROM_POSITION);
  Controller->SaveToStream(rom);
  delete rom;
  SendHTTPResponse(wcl, 0);
}

void MyWebSRV::sendGetUnit(WiFiClient wcl, String payload)
{
  #ifdef DEBUG
    DEBUG_PORT.println(" *** sendGetUnit ***");
  #endif DEBUG
  byte id = 255;
  // extract collection id
  String p = payload;
  if (p == "")
  {
    SendHTTPError(wcl);
    return;
  }
  int n = -1;
  n = p.indexOf("\n");
  while (n>=0)
  {
    String prm = p.substring(0,n);
    int d = prm.indexOf(":");
    String key = prm.substring(0,d);
    String val = prm.substring(d+1,prm.length());

    if (key == "moduleid") id = val.toInt();

    p = p.substring(n+1,p.length());
    if (p == "") n = -1; else n = p.indexOf("\n");
  }
  String resp = Controller->getModuleData(id);
  SendHTTPResponse(wcl, resp.length());
  #ifdef DEBUG
    DEBUG_PORT.print("resp = ");DEBUG_PORT.println(resp);
  #endif DEBUG
  wcl.println(resp);
}
