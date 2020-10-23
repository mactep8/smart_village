#include "RelayUnit.h"
#include "EEPROMStream.h"
#include "AbstractUnit.h"

RelayUnit::RelayUnit():ExecUnit()
{
  #ifdef DEBUG
    DEBUG_PORT.println("--- RelayUnit constructor");
  #endif
  _devCode = RFRELAY;
  _time = millis();
}

RelayUnit::~RelayUnit()
{
  
}

void RelayUnit::LoadFromStream(EEPROMStream * stream)
{
  ExecUnit::LoadFromStream(stream);
  _relayPin = stream->read<byte>();
  pinMode(_relayPin, OUTPUT);
  _state = stream->read<byte>(); // Default state
  digitalWrite(_relayPin, _state); // Set default state
  _key = stream->read<long>();
  _timeInterval = stream->read<uint32_t>();
  _timeDelaySwitch = stream->read<uint32_t>();
}

void RelayUnit::SaveToStream(EEPROMStream * stream)
{
  ExecUnit::SaveToStream(stream);
  stream->write<byte>(_relayPin);
  _state = digitalRead(_relayPin); // get current state as default
  stream->write<byte>(_state); // write current state as default
  stream->write<long>(_key);
  stream->write<uint32_t>(_timeInterval);
  stream->write<uint32_t>(_timeDelaySwitch);
}

void RelayUnit::Operate()
{
  if (_operState == OPER_STATE_ON && millis() - _time > _timeDelaySwitch)
  {
    _time = millis();
    _state = digitalRead(_relayPin);
    _state = !_state;
    digitalWrite(_relayPin, _state);
    _operState = OPER_STATE_OFF;
    return;
  }

  if (_operState == OPER_STATE_OFF && millis() - _time > (_timeInterval * 1000))
  {
    _time = millis();
    _state = digitalRead(_relayPin);
    _state = !_state;
    digitalWrite(_relayPin, _state);
    _operState = OPER_STATE_IDLE;
  }
}

void RelayUnit::Switch()
{
  if (_timeDelaySwitch > 0)
  {
    if (_operState == OPER_STATE_IDLE)
    {
      _time = millis();
      _operState = OPER_STATE_ON;
    }
    return;
  }
  
  if (millis()-_time > 500)
  {
    _time = millis();
    _state = digitalRead(_relayPin);
    _state = !_state;
    digitalWrite(_relayPin, _state);
  }
}

long RelayUnit::getRFKey()
{
  return _key;
}

String RelayUnit::GetJSONAdd()
{
  byte state = digitalRead(_relayPin);
  return ",\"Pin\":"+String(_relayPin,DEC)+",\"State\":"+String(state,DEC)+",\"Key\":"+String(_key,DEC) + ", \"StartDelay\":" + String(_timeDelaySwitch, DEC) + ", \"Interval\":" + String(_timeInterval, DEC);
}

void RelayUnit::SetParams(String prm)
{
  ExecUnit::SetParams(prm);
  #ifdef DEBUG
    DEBUG_PORT.println("--- RelayUnit try");
  #endif
  try
  {
    if (getValueByKey("pin") != "NOT FOUND") _relayPin = getValueByKey("pin").toInt();
    if (getValueByKey("key") != "NOT FOUND") _key = getValueByKey("key").toInt();
    if (getValueByKey("Key") != "NOT FOUND") _key = getValueByKey("Key").toInt();
    if (getValueByKey("State") != "NOT FOUND") 
    {
      _state = getValueByKey("State").toInt();
      digitalWrite(_relayPin, _state);
    }
    
    if (getValueByKey("StartDelay") != "NOT FOUND") _timeDelaySwitch = getValueByKey("StartDelay").toInt();
    if (getValueByKey("Interval") != "NOT FOUND") _timeInterval = getValueByKey("Interval").toInt();
    #ifdef DEBUG
      DEBUG_PORT.println("--- RelayUnit end try");
    #endif
  }
  catch(int& ErrorCode)
  {
    #ifdef DEBUG
      DEBUG_PORT.print("--- RelayUnit SetParams Exception ");DEBUG_PORT.println(ErrorCode);
      DEBUG_PORT.print("--- RelayUnit SetParams ModuleID ");DEBUG_PORT.println(_moduleID);
      for(int i=0;i<Count();i++)
      {
        DEBUG_PORT.print("--- RFSwitchUnit SetParams");DEBUG_PORT.println(getKey(i));DEBUG_PORT.print(" = ");DEBUG_PORT.println(getValue(i));
      }
    #endif
  }
}
