#include "AbstractUnit.h"
#include "Devices.h"
#include <Arduino.h>

AbstractUnit::AbstractUnit()
{
  #ifdef DEBUG
    DEBUG_PORT.println("--- AbstractUnit constructor");
  #endif
  _isSensor = false;
  _devCode = NO_DEVICE;
  _moduleID = 0;

  _items = new Vector<KeyValueItem*>();
}

AbstractUnit::~AbstractUnit()
{
  // KeyValueItem* _items[MAX_COLLECTION_COUNT];
  /*for(int i=0;i<MAX_COLLECTION_COUNT;i++)
  {
    delete _items[i];
  }*/
}

bool AbstractUnit::GetIsSensor()
{
  return _isSensor;
}

byte AbstractUnit::GetModuleID()
{
  return _moduleID;
}

byte AbstractUnit::GetDeviceCode()
{
  return _devCode;
}

void AbstractUnit::LoadFromStream(EEPROMStream * stream)
{
  _moduleID = stream->read<byte>();
  #ifdef DEBUG
    DEBUG_PORT.print("--- AbstractUnit LoadFromStream _ModuleID = ");DEBUG_PORT.println(_moduleID);
  #endif
}

void AbstractUnit::Operate()
{
  
}

void AbstractUnit::SaveToStream(EEPROMStream * stream)
{
  stream->write<byte>(_devCode);
  stream->write<byte>(_moduleID);
  #ifdef DEBUG
    DEBUG_PORT.print("--- AbstractUnit SaveToStream _deviceCode = ");DEBUG_PORT.println(_devCode);
    DEBUG_PORT.print("--- AbstractUnit SaveToStream _ModuleID = ");DEBUG_PORT.println(_moduleID);
  #endif
}

String AbstractUnit::GetJSONAdd()
{
  return "";
}

void AbstractUnit::SetParams(String prm)
{
  #ifdef DEBUG
    DEBUG_PORT.print("--- AbstractUnit Set Params prm = ");DEBUG_PORT.println(prm);
  #endif
  Load(prm);
  try
  {
    if (getValueByKey("moduleid") != "NOT FOUND") _moduleID = getValueByKey("moduleid").toInt();
  }
  catch(int& ErrorCode)
  {
    #ifdef DEBUG
      DEBUG_PORT.print("--- AbstractUnit SetParams Exception ");DEBUG_PORT.println(ErrorCode);
      DEBUG_PORT.print("--- AbstractUnit SetParams old ModuleID ");DEBUG_PORT.println(_moduleID);
      DEBUG_PORT.print("--- AbstractUnit SetParams new ModuleID ");DEBUG_PORT.println(getValueByKey("ModuleID"));
    #endif
  }
}

String AbstractUnit::GetJSON()
{
  String s = "{\"DevCode\":"+String(_devCode, DEC)+",\"ModuleID\":"+String(_moduleID, DEC)+GetJSONAdd()+"}";
  return s;
}

void AbstractUnit::Load(String payload)
{
  _items = new Vector<KeyValueItem*>();
  #ifdef DEBUG
    DEBUG_PORT.print("--- AbstractUnit KeyValueCollection Load = ");DEBUG_PORT.println(payload);
  #endif DEBUG

  String p = payload;
  int n = -1;
  n = p.indexOf(";");
  while (n>=0)
  {
    String prm = p.substring(0,n);
    int d = prm.indexOf("=");
    
    KeyValueItem* itm = new KeyValueItem();
    try
    {
    itm->Key = prm.substring(0,d);
    itm->Value = prm.substring(d+1,prm.length());
    _items->push_back(itm);
    }
    catch(int& ErrorCode)
    {
      #ifdef DEBUG
        DEBUG_PORT.print("--- AbstractUnit Load Exception ");DEBUG_PORT.println(ErrorCode);
        DEBUG_PORT.print("--- AbstractUnit prm = ");DEBUG_PORT.println(prm);
      #endif
    }
    p = p.substring(n+1,p.length());
    if (p == "") n = -1; else n = p.indexOf(";");
    
  }
  #ifdef DEBUG
    DEBUG_PORT.print("Count = ");DEBUG_PORT.println(Count());
  #endif DEBUG
}

int AbstractUnit::Count()
{
  return _items->size();
}

String AbstractUnit::getKey(int i)
{
  KeyValueItem* itm = _items->get(i);
  #ifdef DEBUG
    DEBUG_PORT.print("getKey = ");DEBUG_PORT.println(itm->Key);
  #endif DEBUG
  
  return itm->Key;
}
String AbstractUnit::getValue(int i)
{
  KeyValueItem* itm = _items->get(i);
  #ifdef DEBUG
    DEBUG_PORT.print("getValue = ");DEBUG_PORT.println(itm->Value);
  #endif DEBUG
  return itm->Value;
}

String AbstractUnit::getValueByKey(String key)
{
  String res = "NOT FOUND";
  for(int i=0;i<_items->size();i++)
  {
    String k = getKey(i);
    if (k == key) res = getValue(i);
  }
  return res;
}
