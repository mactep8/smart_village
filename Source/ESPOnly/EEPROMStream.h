#ifndef EEPROM_STREAM
#define EEPROM_STREAM
#include <EEPROM.h>
#include <Arduino.h>
#include "Devices.h"

#define SEEK_TO_FIRST -32767

#ifdef ESP8266
  //#define EEPROM_SIZE 2048
  #define EEPROM_SIZE 1024
#else
  #define EEPROM_SIZE
#endif ESP8266

class EEPROMStream
{
  private:
    int _offset;
  public:
    EEPROMStream()
    {
      #ifdef DEBUG_EEPROM
      DEBUG_PORT.println("--- EEPROMStream Constructor ------------------- ");
      #endif DEBUG_EEPROM
      _offset = 0;
      #ifdef ESP8266
        EEPROM.begin(EEPROM_SIZE);
      #endif
    }

    ~EEPROMStream()
    {
      #ifdef DEBUG_EEPROM
      DEBUG_PORT.println("--- EEPROMStream Destructor ------------------- ");
      #endif DEBUG_EEPROM
    }
    
    template<typename S> S read()
    {
      #ifdef DEBUG_EEPROM
        DEBUG_PORT.println("--- EEPROMStream->read() ------------------------- ");
      #endif DEBUG_EEPROM
      byte cc = sizeof(S);
      S val;
      byte* res = (byte*)(void*)&val;
      for (byte i = 0; i < cc; i++){
          #ifdef DEBUG_EEPROM
            byte b = EEPROM.read(_offset);
            //DEBUG_PORT.print("read byte : ");DEBUG_PORT.println(b);
          #endif DEBUG_EEPROM
          *res++ = EEPROM.read(_offset++);
      }
      #ifdef DEBUG_EEPROM
        //DEBUG_PORT.print("read value : ");DEBUG_PORT.println(val);
      #endif
      return val;
    }

    template<typename S> void read(S* val)
    {
      #ifdef DEBUG_EEPROM
        DEBUG_PORT.println("--- EEPROMStream->read(*val) ------------------------- ");
      #endif DEBUG_EEPROM
      byte cc = sizeof(S);
      byte* res = (byte*)(void*)val;
      for (byte i = 0; i < cc; i++){
          #ifdef DEBUG_EEPROM
            byte b = EEPROM.read(_offset);
            //DEBUG_PORT.print("read byte : ");DEBUG_PORT.println(b);
          #endif DEBUG_EEPROM
          *res++ = EEPROM.read(_offset++);
      }
      #ifdef DEBUG_EEPROM
        //DEBUG_PORT.print("read value : ");DEBUG_PORT.println(val);
      #endif DEBUG_EEPROM
    }

    template<typename S> void write(S* val)
    {
      #ifdef DEBUG_EEPROM
        DEBUG_PORT.println("--- EEPROMStream->write(*val) ------------------------ ");
        //DEBUG_PORT.print("write value : ");DEBUG_PORT.println(val);
      #endif DEBUG_EEPROM
      byte cc = sizeof(S);
      const byte* f = (const byte*)(const void*)val;
      for (byte i = 0; i < cc; i++)
      {
        #ifdef DEBUG_EEPROM
          byte b = *f;
          DEBUG_PORT.print("write byte : ");DEBUG_PORT.println(b);
        #endif DEBUG_EEPROM
        EEPROM.write(_offset++, *f++);
      }
      #ifdef ESP8266
      EEPROM.commit();
      #endif
    }

    template<typename S> void write(S val)
    {
      #ifdef DEBUG_EEPROM
        DEBUG_PORT.println("--- EEPROMStream->write(val) ------------------------ ");
        //DEBUG_PORT.print("write value : ");DEBUG_PORT.println(val);
      #endif
      byte cc = sizeof(S);
      const byte* f = (const byte*)(const void*)&val;
      for (byte i = 0; i < cc; i++)
      {
        #ifdef DEBUG_EEPROM
          byte b = *f;
          DEBUG_PORT.print("write byte : ");DEBUG_PORT.println(b);
        #endif DEBUG_EEPROM
        EEPROM.write(_offset++, *f++);
      }
      #ifdef ESP8266
      EEPROM.commit();
      #endif
    }

    void Seek(int offset)
    {
      if (offset<0 && offset*(-1) > _offset) _offset = 0;
      else
      _offset += offset;
    }
    
};

#endif EEPROM_STREAM
