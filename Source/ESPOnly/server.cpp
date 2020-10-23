#include "server.h"
#include <Arduino.h>
#include <FS.h>

void MyWebSRV::doResponse(String req_page, WiFiClient wcl,String payload)
{
  req_page.toLowerCase();
  #ifdef DEBUG
  DEBUG_PORT.print("REQ page = ");DEBUG_PORT.println(req_page);
  DEBUG_PORT.print("payload = ");DEBUG_PORT.println(payload);
  #endif DEBUG

  if (req_page.length()==0 || req_page.indexOf("index.htm")>=0) sendIndexResponse(wcl);
  else if (req_page == "wifiscan") sendWifiScan(wcl);
  else if (req_page == "wifireboot") sendWifiReboot(wcl);
  else if (req_page == "wifisave") sendWifiSave(wcl, payload);
  else if (req_page == "wifiload") sendWifiLoad(wcl);
  else if (req_page == "devload") sendDevList(wcl);
  else if (req_page == "devadd") sendDevAdd(wcl, payload);
  else if (req_page == "devdel") sendDevDel(wcl, payload);
  else if (req_page == "ctrlload") sendUnitsLoad(wcl, payload);
  else if (req_page == "files") sendFilesList(wcl);
  else if (req_page == "deletefile") fileDelete(wcl, payload);
  else if (req_page == "operate") sendOperate(wcl, payload);
  else if (req_page == "update") sendUpdate(wcl, payload);
  else if (req_page == "getunit") sendGetUnit(wcl, payload);
  else{
    #ifdef DEBUG
    DEBUG_PORT.print("Extension = ");DEBUG_PORT.println(req_page.substring(req_page.length()-4,req_page.length()));
    #endif DEBUG
    
    File f = SPIFFS.open("/" + req_page, "r");
    if (!f) {
      #ifdef DEBUG
      DEBUG_PORT.println("file open failed");  //  "открыть файл не удалось"
      #endif DEBUG
      if (req_page.substring(req_page.length()-4,req_page.length())==".css") SendCSSResponse(wcl, 0);
      else if (req_page.substring(req_page.length()-3,req_page.length())==".js") SendJSResponse(wcl, 0);
      else
      SendHTTPResponse(wcl, 0);
    }
    else {
      #ifdef DEBUG
      DEBUG_PORT.print("file opened ");  //  "открыть файл удалось"
      DEBUG_PORT.println(f.size());
      #endif DEBUG
      //wcl.write(f, f.size());
      if (req_page.substring(req_page.length()-4,req_page.length())==".css") SendCSSResponse(wcl, f.size());
      else if (req_page.substring(req_page.length()-3,req_page.length())==".js") SendJSResponse(wcl, f.size());
      else SendHTTPResponse(wcl, f.size());
      byte* buff = (byte*)malloc(MAX_BUFFER_SIZE);
      int rsize = MAX_BUFFER_SIZE;
      while(f.available()) {
        rsize = f.read(buff, MAX_BUFFER_SIZE);
        ReplaceMarkers(wcl, buff, rsize);
      }
      free(buff);
      f.close();
      //delete &f;
    }
  }
}

void MyWebSRV::doWork()
{
  bool data_finished = false;
  bool is_post = false;
  bool in_payload = false;
  String page_name = "";
  uint32_t contentLen = 0;
  uint32_t payload_len = 0;
  String payload = "";
  WiFiClient wcl = srv->available();
  if (wcl){
    uint32_t start_time = millis();
    String req_line = "";
    bool blank_line = false;
    // крутимся пока не вычитаем все данные или не отвалимся по таймауту
    while (wcl.connected() && !data_finished && (millis() - start_time < 60000))
    {
      if (wcl.available()) {
        char c = wcl.read();
        #ifdef DEBUG
        DEBUG_PORT.print(c);
        #endif DEBUG
        
        if (!in_payload){
          if (req_line.length() < MAX_LEN_REQUEST && c!='\r' && c!='\n') {
            req_line += c;
            blank_line = false;
          }
          
          if (blank_line){
            in_payload = true;
          }
          
          if (c == '\n' && !blank_line)// анализизуем строку запроса
          {
            int http_head = req_line.indexOf(" HTTP");
            if (http_head >= 0){
              // это первая строка
              if (req_line.indexOf("GET /")>=0) {page_name = req_line.substring(5, http_head); data_finished = true;}
              if (req_line.indexOf("POST /")>=0) {page_name = req_line.substring(6, http_head); is_post = true;}
            }
  
            if (req_line.indexOf("Content-Length")>=0) {
              contentLen = req_line.substring(16, (req_line.length())).toInt();
              #ifdef DEBUG
              DEBUG_PORT.print("Content-Length = ");DEBUG_PORT.println(contentLen);
              #endif DEBUG
            }
  
            req_line = "";
            blank_line = true;
          } // анализизуем строку запроса
        } // !in_payload
        else
        {
          payload_len++;
          if (c == '\n' && req_line == "")
          {
            req_line = "";
          }
          else if (c == '\n' && req_line == "{")
          {
            req_line = "";
          }
          else if (c == '}') {
            data_finished = true;
            payload = req_line;
          }
          else req_line+=c;
          /*#ifdef DEBUG
          DEBUG_PORT.print("req_line = ");DEBUG_PORT.println(req_line);
          DEBUG_PORT.print("payload_len = ");DEBUG_PORT.println(payload_len);
          #endif DEBUG*/
          if (payload_len > contentLen) data_finished = true;
          
        }
      } // есть данные
    } // while

    wcl.flush();
    while (wcl.connected() && wcl.available() && (millis() - start_time < 60000))
    {
      if (wcl.available()) {
        char c = wcl.read();
      }
    }
    
    // Если не свалились
    if (wcl.connected() && data_finished) {
      doResponse(page_name, wcl, payload);
    }
    wcl.flush();
    wcl.stop();
  }
}

/*void MyWebSRV::doWork()
{
  String payload = "";
  bool in_payload=false;
  bool req_read = false;
  String request = "";
  WiFiClient wcl = srv->available();
  if (wcl){
    uitn32_t start_time = millis();
    bool is_post = false;
    uitn32_t contentLen=0;
    #ifdef DEBUG
    DEBUG_PORT.println("------------- Begin request ----------------------");
    #endif DEBUG
    request = "";
    String pg = "";
    bool blank_line = true;
    bool isRequest = false;
    while (wcl.connected()) {
      if (wcl.available()) {
        char c = wcl.read();
          #ifdef DEBUG
          DEBUG_PORT.print(c);
          #endif DEBUG
        if (request.length() < MAX_LEN_REQUEST && c!='\r' && c!='\n') {
          request += c;
          blank_line = false;
        }
        else if (c == '\n') {
          if (in_payload && request!="}")
          {
            payload += request;
            payload += "\n";
          }
          else
          {
            int e = request.indexOf(" HTTP");
            if (request.indexOf("GET /")>=0 && e>0) {pg = request.substring(5, e); isRequest = true;}
            if (request.indexOf("POST /")>=0 && e>0) {pg = request.substring(6, e); isRequest = true; is_post = true;}
            //if (isRequest && pg.indexOf("uploadfile&")>=0) {fileUpload(wcl, pg.substring(pg.indexOf("&")+1,pg.length()));break;}
            //if (is_post && request.indexOf("Content-Length")>=0) contentLen = line.substring(16, (line.length()-1)).toInt();
          }
          //if (is_post && contentLen>0 && blank_line) break;
          if (request == "{") {payload = ""; in_payload = true;}
          if (request == "}") {in_payload = false;}
          request = "";
          blank_line = true;
          req_read = true;
          
          //break;
        }
      }
      else if (req_read || !blank_line) {
        #ifdef DEBUG
        DEBUG_PORT.println(request);
        #endif DEBUG
        if (isRequest)doResponse(pg, wcl, payload);
        
        break;
      }
    }
    delay(10); // time to browser receive data
    #ifdef DEBUG
    DEBUG_PORT.println("------------- End request ------------------------");
    #endif DEBUG
    wcl.flush();
    wcl.stop();
  }
}*/

/*void MyWebSRV::doWork()
{
  String payload = "";
  bool in_payload=false;
  bool req_read = false;
  String request = "";
  WiFiClient wcl = srv->available();
  if (wcl){
    #ifdef DEBUG
    DEBUG_PORT.println("------------- Begin request ----------------------");
    #endif DEBUG
    request = "";
    String pg = "";
    bool blank_line = true;
    bool isRequest = false;
    while (wcl.connected()) {
      if (wcl.available()) {
        char c = wcl.read();
        #ifdef DEBUG
        DEBUG_PORT.print(c);
        #endif DEBUG
        if (request.length() < MAX_LEN_REQUEST && c!='\r' && c!='\n') {
          request += c;
          blank_line = false;
        }
        else if (c == '\n') {
          #ifdef DEBUG
          DEBUG_PORT.print("request = ");DEBUG_PORT.println(request);
          #endif DEBUG
          if (in_payload && request!="}")
          {
            payload += request;
            payload += "\n";
          }
          else
          {
            int e = request.indexOf(" HTTP");
            if (request.indexOf("GET /")>=0 && e>0) {pg = request.substring(5, e); isRequest = true;}
            if (request.indexOf("POST /")>=0 && e>0) {pg = request.substring(6, e); isRequest = true;}
            if (isRequest && pg.indexOf("uploadfile&")>=0) {fileUpload(wcl, pg.substring(pg.indexOf("&")+1,pg.length()));break;}
          }
          if (request == "{") {payload = ""; in_payload = true;}
          if (request == "}") {in_payload = false;}
          request = "";
          blank_line = true;
          req_read = true;
          
          //break;
        }
      }
      else if (req_read || !blank_line) {
        #ifdef DEBUG
        DEBUG_PORT.println(request);
        #endif DEBUG
        if (isRequest) {
          #ifdef DEBUG
          DEBUG_PORT.println("------------- doResponse ------------------------");
          DEBUG_PORT.print("payload = ");DEBUG_PORT.println(payload);
          #endif DEBUG          
          doResponse(pg, wcl, payload);
          #ifdef DEBUG
          DEBUG_PORT.println("------------- End doResponse ------------------------");
          #endif DEBUG
        }
        break;
      }
    }
    #ifdef DEBUG
    DEBUG_PORT.println("------------- post while ----------------------");
    #endif DEBUG
    delay(10); // time to browser receive data
    #ifdef DEBUG
    DEBUG_PORT.println("------------- End request ------------------------");
    #endif DEBUG
    wcl.stop();
  }
}*/

void MyWebSRV::fileUpload(WiFiClient wcl, String filename) 
{
  #ifdef DEBUG
    DEBUG_PORT.println("fileUpload");
    DEBUG_PORT.println(filename);
  #endif DEBUG
  int contentLen = 0;

  while (wcl.connected()) {
    if (wcl.available()) {
      String line = wcl.readStringUntil('\n');
      #ifdef DEBUG
        DEBUG_PORT.println(line);
      #endif DEBUG
      if(line.startsWith("Content-Length")) {
        contentLen = line.substring(16, (line.length()-1)).toInt();
      }
      #ifdef DEBUG
        DEBUG_PORT.print("Content-Length = ");DEBUG_PORT.println(contentLen);
      #endif DEBUG
      if(line.length() == 1 && line[0] == '\r') {

        String path = "/" + filename;
        File file = SPIFFS.open(path, "w+");

        if(file) {
          int i = 0;
          while (i < contentLen) {
            file.write(wcl.read());
            i++;
          }
          file.close();
        }
        
        delay(10);
        wcl.println("DONE");

        break;
      } 
    }
  }
}

void MyWebSRV::sendFilesList(WiFiClient wcl)
{
  Dir dir = SPIFFS.openDir("/");
  boolean isFirst = true;
  String fileName = "";
  int fileSize = 0;
  wcl.print("[");
  while (dir.next()) {
    fileName = dir.fileName();
    File f = dir.openFile("r");
    fileSize = f.size();
    f.close();
    if (!isFirst) wcl.print(","); else isFirst = false;
    wcl.print("{\"fileName\":\""+fileName+"\",\"fileSize\":"+String(fileSize, DEC)+"}");
  }
  wcl.print("]");
}

void MyWebSRV::fileDelete(WiFiClient wcl, String payload)
{
  String filename = "";
  long s_time = millis();
  String p = payload;
  int n = -1;
  n = p.indexOf("\n");
  while (n>=0)
  {
    String prm = p.substring(0,n);
    int d = prm.indexOf(":");
    String key = prm.substring(0,d);
    String val = prm.substring(d+1,prm.length());

    if (key == "fileName") filename = val;

    p = p.substring(n+1,p.length());
    if (p == "") n = -1; else n = p.indexOf("\n");
  }
  
  SPIFFS.remove(filename);
  wcl.println("DONE");
}

MyWebSRV::MyWebSRV(MyNetwork* aNetwork, MyController* aController)
{
  Network = aNetwork;
  Controller = aController;
  SPIFFS.begin();
  srv = new WiFiServer(80);
  srv->begin();

  // KeyValueItem* _items[MAX_COLLECTION_COUNT];
  /*for(int i=0;i<MAX_COLLECTION_COUNT;i++)
  {
    _items[i] = new KeyValueItem();
  }*/
}

MyWebSRV::~MyWebSRV()
{
  /*for(int i=0;i<MAX_COLLECTION_COUNT;i++)
  {
    delete _items[i];
  }*/
  delete srv;
}

void MyWebSRV::SendHTTPResponse(WiFiClient wcl, int lng) 
{
  #ifdef DEBUG
  DEBUG_PORT.println("HTTP Response");
  #endif DEBUG
  wcl.println(CreateHTTPRespHeader("text/html", lng));
}

String MyWebSRV::CreateHTTPRespHeader(String content, int lng) {
  String s = "";
  s += F("HTTP/1.1 200 OK\n");
  s += F("Content-Type: ");
  s += content;
  s += F("\n");
/*  s += F("Content-Length: ");
  s += String(lng);
  s += F("\n");*/
  s += F("Connnection: close\n");
  return s;
}

void MyWebSRV::SendHTTPError(WiFiClient wcl)
{
  #ifdef DEBUG
  DEBUG_PORT.println("HTTP Error");
  #endif DEBUG
  String s = "";
  s += F("HTTP/1.1 500 Payload Error\n");
  s += F("Content-Type: ");
  s += "text/html";
  s += F("\n");
/*  s += F("Content-Length: ");
  s += String(lng);
  s += F("\n");*/
  s += F("Connnection: close\n");
  wcl.println(s);
}

void MyWebSRV::ReplaceMarkers(WiFiClient wcl, byte* buff, int rsize)
{
  int i = 0;
  while (i<rsize)
  {
    if (i<rsize-1 && char(buff[i])=='[' && char(buff[i+1])=='#')
    {
      int j = i+2;
      String marker = "";
      while (j<rsize-1 && char(buff[j])!='#' && char(buff[j+1])!=']'){marker+=char(buff[j]); j++; }
      if (j<rsize-1 && char(buff[j])=='#' && char(buff[j+1])==']') {doMarker(wcl, marker); i = j + 1;}
      else wcl.print(char(buff[i]));
    }
    else wcl.print(char(buff[i]));
    i++;
  }
}

void MyWebSRV::doMarker(WiFiClient wcl, String marker)
{
  #ifdef DEBUG
  DEBUG_PORT.print("marker=");DEBUG_PORT.println(marker);
  #endif DEBUG
  if(marker ==  "MY_MAC") {
    wcl.print(WiFi.macAddress());
    #ifdef DEBUG
      DEBUG_PORT.print("MY_MAC=");DEBUG_PORT.println(WiFi.macAddress());
    #endif DEBUG  
  }
  else
  if(marker ==  "MY_LOCAL_IP") {
    wcl.print(WiFi.localIP());
    #ifdef DEBUG
      DEBUG_PORT.print("MY_LOCAL_IP=");DEBUG_PORT.println(WiFi.localIP());
    #endif DEBUG
  }
}

void MyWebSRV::SendCSSResponse(WiFiClient wcl, int lng)
{
  #ifdef DEBUG
  DEBUG_PORT.println("CSS Response");
  #endif DEBUG
  wcl.println(CreateHTTPRespHeader("text/css", lng));
}

void MyWebSRV::SendJSResponse(WiFiClient wcl, int lng)
{
  #ifdef DEBUG
  DEBUG_PORT.println("JavaScript Response");
  #endif DEBUG
  wcl.println(CreateHTTPRespHeader("application/javascript", lng));
}

void MyWebSRV::Load(String payload)
{
  #ifdef DEBUG
    DEBUG_PORT.println("--- KeyValueCollection Load");
  #endif DEBUG
  for(int i=0;i<_items.size();i++)
    delete (_items[i]);
  _items.clear();
  String p = payload;
  int n = -1;
  n = p.indexOf("\n");
  while (n>=0)
  {
    String prm = p.substring(0,n);
    int d = prm.indexOf(":");
    KeyValueItem* itm = new KeyValueItem();
    /*#ifdef DEBUG
      DEBUG_PORT.print("Item = ");DEBUG_PORT.println(_count);
    #endif DEBUG*/
    itm->Key = prm.substring(0,d);
    itm->Value = prm.substring(d+1,prm.length());
    _items.push_back(itm);

    p = p.substring(n+1,p.length());
    if (p == "") n = -1; else n = p.indexOf("\n");
    
  }
  #ifdef DEBUG
    DEBUG_PORT.print("Count = ");DEBUG_PORT.println(Count());
  #endif DEBUG
}

int MyWebSRV::Count()
{
  return _items.size();
}

String MyWebSRV::getKey(int i)
{
  return _items[i]->Key;
}
String MyWebSRV::getValue(int i)
{
  return _items[i]->Value;
}

String MyWebSRV::getValueByKey(String key)
{
  String res = "NOT FOUND";
  for(int i=0;i<_items.size();i++)
  {
    String k = getKey(i);
    if (k == key) res = getValue(i);
  }
  return res;
}
