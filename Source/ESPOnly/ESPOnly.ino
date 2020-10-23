#include "Devices.h"
#include "network.h"
#include "server.h"
#include "MyController.h"
//#include <ESP8266mDNS.h>

MyNetwork* wnet;
MyWebSRV* ws;
MyController* Controller;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, led_OFF);
  delay(1500);
  #ifdef DEBUG
  DEBUG_PORT.begin(DEBUG_RATE);
  DEBUG_PORT.println("Starting...");
  #endif DEBUG

  wnet = new MyNetwork();

  Controller = new MyController();
  if (!Controller->setup())
  {
    #ifdef DEBUG
      DEBUG_PORT.println("--- Bad setup ---------------------------------- ");
    #endif
    Controller->WriteDefaultConfig();
    #ifdef DEBUG
      DEBUG_PORT.println("--- Retry setup -------------------------------- ");
    #endif
    Controller->setup();
  }

  ws = new MyWebSRV(wnet, Controller);
  digitalWrite(LED_BUILTIN, led_ON);
}

void loop()
{
  /*if (!wnet->isAP())
  {
    MDNS.update();
  }*/
  #ifdef DEBUG
      //DEBUG_PORT.println("--- Retry loop -------------------------------- ");
    #endif
  Controller->loop();
  ws->doWork();
}
