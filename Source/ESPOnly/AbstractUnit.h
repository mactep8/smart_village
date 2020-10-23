#ifndef UNIT_ABSTRACT
#define UNIT_ABSTRACT

#include <Arduino.h>
#include "EEPROMStream.h"
#include "KeyValueItem.h"
#include <Vector.h>

#define MAX_COLLECTION_COUNT 6

class AbstractUnit
{
  private:
    //byte _count;
    Vector<KeyValueItem*>* _items;

    void Load(String payload);
  protected:
    byte _devCode;
    byte _moduleID;
    bool _isSensor;
    
    String getValueByKey(String key);
    String getKey(int i);
    String getValue(int i);
    int Count();
  public:
    AbstractUnit();
    ~AbstractUnit();
    bool GetIsSensor();
    byte GetDeviceCode();
    byte GetModuleID();
    void virtual LoadFromStream(EEPROMStream * stream);
    void virtual SaveToStream(EEPROMStream * stream);

    void virtual Operate();
  
    String GetJSON();
    String virtual GetJSONAdd();
    void virtual SetParams(String prm);
};

#endif UNIT_ABSTRACT
