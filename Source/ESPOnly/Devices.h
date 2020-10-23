#ifndef MY_DEVICES
#define MY_DEVICES
#include <Arduino.h>

#define SONOFF_CH4PRO
//#define FREE_JMP

#define DEBUG
#ifdef DEBUG
  #define DEBUG_EEPROM
  #define DEBUG_PORT Serial
  #define DEBUG_RATE 115200
  //Controller debug
  //#define DEBUG_RESET_CONFIG
#endif DEBUG

// Controller
#define EEPROM_POSITION 64
#define MAX_DEVICES 10

#define NO_DEVICE 255
#define RF433 1
#define RFRELAY 2
#define DHTSENSOR 3
#define PIRSENSOR 4
#define DHTSERVO 5

#ifdef  SONOFF_CH4PRO
  #define JMP_RESET_DEVLIST 14
  #define JMP_RESET_WIFI 10
  #define RESET_LEVEL LOW
  #define INPUT_CFG INPUT
#endif  SONOFF_CH4PRO

#ifdef  FREE_JMP
  #define JMP_RESET_DEVLIST 14
  #define JMP_RESET_WIFI 10
  #define RESET_LEVEL LOW
  #define INPUT_CFG INPUT_PULLUP
#endif  FREE_JMP
//#define led_started LED_BUILTIN
#define led_ON LOW
#define led_OFF HIGH
#endif MY_DEVICES
