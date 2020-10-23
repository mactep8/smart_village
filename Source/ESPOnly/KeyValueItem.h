#ifndef KEY_VALUE_ITEM
#define KEY_VALUE_ITEM
#include <Arduino.h>

class KeyValueItem
{
  public:
    String Key;
    String Value;
    
    KeyValueItem()
    {
      #ifdef DEBUG
        DEBUG_PORT.println("--- KeyValueItem constructor");
      #endif DEBUG
    }

    ~KeyValueItem()
    {
      #ifdef DEBUG
        DEBUG_PORT.println("--- KeyValueItem destructor");
        DEBUG_PORT.println(Key);
      #endif DEBUG
    }
};

#endif KEY_VALUE_ITEM
