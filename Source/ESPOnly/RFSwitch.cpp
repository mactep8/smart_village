#include "AbstractUnit.h"
#include "RFSwitch.h"
#include "Devices.h"
#include "EEPROMStream.h"
#include "RelayUnit.h"

RFSwitchUnit::RFSwitchUnit(Vector<AbstractUnit*>* execUnits):SensorUnit()
{
  #ifdef DEBUG
    DEBUG_PORT.println("--- RFSwitchUnit constructor");
  #endif
  _devCode = RF433;
  _execUnits = execUnits;
  _lastTime = millis();
  _enabled = false;
  _mySwitch = new RCSwitch();
}

void RFSwitchUnit::LoadFromStream(EEPROMStream * stream)
{
  SensorUnit::LoadFromStream(stream);
  _pin = stream->read<byte>();
  int n = digitalPinToInterrupt(_pin);
  if (n != NOT_AN_INTERRUPT) {_mySwitch->enableReceive(n); _enabled = true;}
  #ifdef DEBUG
    DEBUG_PORT.print("--- RF433Device _pin = ");DEBUG_PORT.println(_pin);
    DEBUG_PORT.print("--- RF433Device NOT_AN_INTERRUPT = ");DEBUG_PORT.println(NOT_AN_INTERRUPT);
    DEBUG_PORT.print("--- RF433Device n = ");DEBUG_PORT.println(n);
  #endif
}

void RFSwitchUnit::SaveToStream(EEPROMStream * stream)
{
  SensorUnit::SaveToStream(stream);
  stream->write<byte>(_pin);
}

void RFSwitchUnit::Operate()
{
  #ifdef DEBUG
    //DEBUG_PORT.println("--- RF433Device Operate _lastKey = ");
  #endif
  if (_mySwitch->available()) {
    _lastKey = _mySwitch->getReceivedValue();
    #ifdef DEBUG
    DEBUG_PORT.print("--- RFSwitchUnit Operate _lastKey = ");DEBUG_PORT.println(_lastKey);
    #endif
    _mySwitch->resetAvailable();
    if (millis() - _lastTime < 200) return; // антидребезг
    _lastTime = millis();
    for (int i = 0;i<_execUnits->size();i++) 
    {
      AbstractUnit* dev = _execUnits->get(i);
      #ifdef DEBUG
        DEBUG_PORT.print("--- RFSwitchUnit dev->GetDeviceCode() = ");DEBUG_PORT.println(dev->GetDeviceCode());
        DEBUG_PORT.print("--- RFSwitchUnit RFRELAY = ");DEBUG_PORT.println(RFRELAY);
        if (dev->GetDeviceCode() == RFRELAY)
        {
          DEBUG_PORT.print("--- RFSwitchUnit ((RelayUnit *)dev)->getRFKey() = ");DEBUG_PORT.println(((RelayUnit *)dev)->getRFKey());
        }
      #endif
      if (dev->GetDeviceCode() == RFRELAY && ((RelayUnit *)dev)->getRFKey() == _lastKey) ((RelayUnit *)dev)->Switch();
    }
  }
}

String RFSwitchUnit::GetJSONAdd()
{
  return ",\"Pin\":"+String(_pin, DEC) + ",\"LastKey\":"+String(_lastKey, DEC);
}

void RFSwitchUnit::SetParams(String prm)
{
  #ifdef DEBUG
    DEBUG_PORT.println("--- RFSwitchUnit Set Params");
    DEBUG_PORT.print("--- RFSwitchUnit _pin = ");DEBUG_PORT.println(_pin);
  #endif
  SensorUnit::SetParams(prm);
  try
  {
    byte pin = getValueByKey("pin").toInt();
    #ifdef DEBUG
      DEBUG_PORT.print("--- RFSwitchUnit pin = ");DEBUG_PORT.println(pin);
    #endif
    if (pin != _pin)
    {
      if (_enabled) _mySwitch->disableReceive();
      _enabled = false;
      _pin = pin;
      int n = digitalPinToInterrupt(_pin);
      #ifdef DEBUG
        DEBUG_PORT.print("--- RFSwitchUnit n = ");DEBUG_PORT.println(n);
        DEBUG_PORT.print("--- RFSwitchUnit NOT_AN_INTERRUPT = ");DEBUG_PORT.println(NOT_AN_INTERRUPT);
      #endif
      if (n != NOT_AN_INTERRUPT) {_mySwitch->enableReceive(n);_enabled = true;}
    }
  }
  catch(int& ErrorCode)
  {
    #ifdef DEBUG
      DEBUG_PORT.print("--- RFSwitchUnit SetParams Exception ");DEBUG_PORT.println(ErrorCode);
      DEBUG_PORT.print("--- RFSwitchUnit SetParams ModuleID ");DEBUG_PORT.println(_moduleID);
    #endif
  }
}
