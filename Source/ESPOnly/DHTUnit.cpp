#include "DHTUnit.h"
#include "EEPROMStream.h"
#include "Devices.h"

DHTUnit::DHTUnit():SensorUnit()
{
  #ifdef DEBUG
    DEBUG_PORT.println("--- DHTUnit constructor");
  #endif
  _dht = new DHTesp();
  _lastTime = millis();
  _devCode = DHTSENSOR;
}

DHTUnit::~DHTUnit()
{
  delete _dht;
}

void DHTUnit::LoadFromStream(EEPROMStream * stream)
{
  SensorUnit::LoadFromStream(stream);
  _pin = stream->read<byte>();
  _dht->setup(_pin, DHTesp::DHT22); // Connect DHT sensor to GPIO pin
  _lastTime = millis();
  _temp = _dht->getTemperature();
  _hum = _dht->getHumidity();
  if (String(_temp) == "nan") _temp = 0;
  if (String(_hum) == "nan") _hum = 0;
}

void DHTUnit::SaveToStream(EEPROMStream * stream)
{
  SensorUnit::SaveToStream(stream);
  stream->write<byte>(_pin);
}

void DHTUnit::SetParams(String prm)
{
  SensorUnit::SetParams(prm);
  try
  {
    byte pin = getValueByKey("pin").toInt();
    if (pin != _pin)
    {
      _pin = pin;
      _dht->setup(_pin, DHTesp::DHT22); // Connect DHT sensor to GPIO pin
    }
  }
  catch(int& ErrorCode)
  {
    #ifdef DEBUG
      DEBUG_PORT.print("--- DHTUnit SetParams Exception ");DEBUG_PORT.println(ErrorCode);
      DEBUG_PORT.print("--- DHTUnit SetParams ModuleID ");DEBUG_PORT.println(_moduleID);
    #endif
  }
}

void DHTUnit::Operate()
{
  
  if (millis() - _lastTime<60000) return;
  #ifdef DEBUG
    DEBUG_PORT.println("--- DHTUnit Operate ");
  #endif
  _lastTime = millis();
  _temp = _dht->getTemperature();
  _hum = _dht->getHumidity();
  if (String(_temp) == "nan") _temp = 0;
  if (String(_hum) == "nan") _hum = 0;
  #ifdef DEBUG
    DEBUG_PORT.print("_temp = ");DEBUG_PORT.println(_temp);
    DEBUG_PORT.print("_hum = ");DEBUG_PORT.println(_hum);
  #endif
}

String DHTUnit::GetJSONAdd()
{
  #ifdef DEBUG
    DEBUG_PORT.println("--- DHTUnit get JSON");
  #endif
  char tempStr[10];
  dtostrf(_temp, 3, 3, tempStr); 
  char humStr[10];
  dtostrf(_hum, 3, 3, humStr); 
  #ifdef DEBUG
    DEBUG_PORT.print("_temp = ");DEBUG_PORT.println(_temp);
    DEBUG_PORT.print("_hum = ");DEBUG_PORT.println(_hum);
    DEBUG_PORT.println(",\"Pin\":"+String(_pin, DEC) + ",\"Temp\":"+String(_temp) + ",\"Hum\":"+String(_hum));
  #endif
  return ",\"Pin\":"+String(_pin, DEC) + ",\"Temp\":"+String(tempStr) + ",\"Hum\":"+String(humStr);
}
