#include "PIRUnit.h"
#include "EEPROMStream.h"
#include "Devices.h"

PIRUnit::PIRUnit(Vector<AbstractUnit*>* execUnits):SensorUnit()
{
  #ifdef DEBUG
    DEBUG_PORT.println("--- PIRUnit constructor");
  #endif
  _lastTime = 0;
  _devCode = PIRSENSOR;
  _execUnits = execUnits;
  _lvl = 0;
}

PIRUnit::~PIRUnit()
{}

void PIRUnit::LoadFromStream(EEPROMStream * stream)
{
  SensorUnit::LoadFromStream(stream);
  _pin = stream->read<byte>();
  pinMode(_pin, INPUT);
  _lvl = digitalRead(_pin);
  _key = stream->read<long>();
  _lastTime = 0;
}

void PIRUnit::SaveToStream(EEPROMStream * stream)
{
  SensorUnit::SaveToStream(stream);
  stream->write<byte>(_pin);
}

void PIRUnit::SetParams(String prm)
{
  SensorUnit::SetParams(prm);
  try
  {
    byte pin = getValueByKey("Pin").toInt();
    if (pin != _pin)
    {
      _pin = pin;
    }
    _key = getValueByKey("Key").toInt();
  }
  catch(int& ErrorCode)
  {
    #ifdef DEBUG
      DEBUG_PORT.print("--- PIRUnit SetParams Exception ");DEBUG_PORT.println(ErrorCode);
      DEBUG_PORT.print("--- PIRUnit SetParams ModuleID ");DEBUG_PORT.println(_moduleID);
    #endif
  }
}

void PIRUnit::Operate()
{
  byte lvl = digitalRead(_pin);
  if (lvl == HIGH && lvl != _lvl)
  {
    _lastTime = millis();
    for (int i = 0;i<_execUnits->size();i++) 
    {
      AbstractUnit* dev = _execUnits->get(i);
//      if (dev->GetDeviceCode() == RFRELAY && ((RelayUnit *)dev)->getRFKey() == _lastKey) ((RelayUnit *)dev)->Switch();
    }
  }

  _lvl = lvl;
  if (_lastTime > 0 && millis() - _lastTime > 90000)
  {
    _lastTime = 0;
    for (int i = 0;i<_execUnits->size();i++) 
    {
      AbstractUnit* dev = _execUnits->get(i);
//      if (dev->GetDeviceCode() == RFRELAY && ((RelayUnit *)dev)->getRFKey() == _lastKey) ((RelayUnit *)dev)->Switch();
    }
  }
}

String PIRUnit::GetJSONAdd()
{
  
}
