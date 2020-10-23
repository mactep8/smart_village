#ifndef MY_WEB_SERVER
#define MY_WEB_SERVER

#include <ESP8266WiFi.h>
#include "network.h"
#include "MyController.h"
#include <Vector.h>

#define MAX_LEN_REQUEST 2048
#define MAX_BUFFER_SIZE 2912

class MyWebSRV{
  private:
    WiFiServer* srv;
    MyNetwork* Network;
    MyController* Controller;

    //byte _count;
    Vector<KeyValueItem*> _items;
    
    void Load(String payload);
    String getKey(int i);
    String getValue(int i);
    int Count();
    String getValueByKey(String key);

    void fileUpload(WiFiClient wcl, String filename);
    void doResponse(String req_page, WiFiClient wcl,String payload);
    void ReplaceMarkers(WiFiClient wcl, byte* buff, int rsize);
    void doMarker(WiFiClient wcl, String marker);
    
    String CreateHTTPRespHeader(String content, int lng);
    void SendHTTPResponse(WiFiClient wcl, int lng);
    void SendCSSResponse(WiFiClient wcl, int lng);
    void SendJSResponse(WiFiClient wcl, int lng);
    void SendHTTPError(WiFiClient wcl);
    
    void sendIndexResponse(WiFiClient wcl);
    void sendWifiScan(WiFiClient wcl);
    void sendWifiReboot(WiFiClient wcl);
    void sendWifiSave(WiFiClient wcl, String payload);
    void sendWifiLoad(WiFiClient wcl);
    void sendDevList(WiFiClient wcl);
    void sendDevAdd(WiFiClient wcl, String payload);
    void sendDevDel(WiFiClient wcl, String payload);
    void sendUnitsLoad(WiFiClient wcl, String payload);
    void sendFilesList(WiFiClient wcl);
    void fileDelete(WiFiClient wcl, String payload);
    void sendOperate(WiFiClient wcl, String payload);
    void sendUpdate(WiFiClient wcl, String payload);
    void sendGetUnit(WiFiClient wcl, String payload);
  public:
    MyWebSRV(MyNetwork* aNetwork, MyController* aController);
    ~MyWebSRV();
    void doWork();
};
#endif MY_WEB_SERVER
