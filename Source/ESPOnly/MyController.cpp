#include "MyController.h"
#include <Arduino.h>
#include "AbstractUnit.h"
#include "PIRUnit.h"
#include "DHTUnit.h"
#include "RFSwitch.h"
#include "RelayUnit.h"

void MyController::loop()
{
  for (int i = 0;i<_sensors->size();i++) 
  {
    AbstractUnit * dev = _sensors->get(i);
    dev->Operate();
    delay(0);
  }

  for (int i = 0;i<_execUnits->size();i++) 
  {
    AbstractUnit * dev = _execUnits->get(i);
    dev->Operate();
    delay(0);
  }
}

AbstractUnit * MyController::GetDeviceInstance(byte deviceCode)
{
  switch(deviceCode)
  {
    case RF433: return new RFSwitchUnit(_execUnits);break;
    case RFRELAY: return new RelayUnit();break;
    case DHTSENSOR: return new DHTUnit();break;
    case PIRSENSOR: return new PIRUnit(_execUnits);break;
    default: return NULL;
  }
}

void MyController::SaveToStream(EEPROMStream * stream)
{
  #ifdef DEBUG
    DEBUG_PORT.println("--- MyController SaveToStream ------------ ");
    DEBUG_PORT.print("_allDevCount = ");DEBUG_PORT.println(_allDevCount);
  #endif
  stream->write<int>(_allDevCount);

  #ifdef DEBUG
    DEBUG_PORT.println("Writting sensors.");
  #endif
  for (int i = 0;i<_sensors->size();i++)
  {
    AbstractUnit * d = _sensors->get(i);
    d->SaveToStream(stream);
  }

  #ifdef DEBUG
    DEBUG_PORT.println("Writting exec units.");
  #endif
  for (int i = 0;i<_execUnits->size();i++)
  {
    AbstractUnit * d = _execUnits->get(i);
    d->SaveToStream(stream);
  }
}

bool MyController::setup()
{
  #ifdef DEBUG
    DEBUG_PORT.println("--- MyController setup ------------------------- ");
    //WriteDefaultConfig();
  #endif
  // Прочитать конфигурацию и настроить датчики
  EEPROMStream * rom = new EEPROMStream();
  rom->Seek(EEPROM_POSITION);
  _allDevCount = rom->read<int>();
  #ifdef DEBUG
    DEBUG_PORT.print("--- MyController allDevCount = ");DEBUG_PORT.println(_allDevCount);
  #endif
  if (_allDevCount<0 || _allDevCount>MAX_DEVICES) 
  {
    delete rom;
    #ifdef DEBUG
      DEBUG_PORT.println("--- MyController setup break 1 ----------------- ");
    #endif
    return false;
  }

  for (int i = 0; i < _allDevCount; i++)
  {
    byte devCode = rom->read<byte>();
    #ifdef DEBUG
      DEBUG_PORT.print("--- MyController setup devCode = ");DEBUG_PORT.println(devCode);
    #endif
    AbstractUnit * dev = GetDeviceInstance(devCode);
    if (dev == NULL) 
    {
      delete rom;
      #ifdef DEBUG
        DEBUG_PORT.println("--- MyController setup break 2 ----------------- ");
      #endif
      return false;
    }

    if (dev->GetIsSensor()) _sensors->push_back(dev);
    else _execUnits->push_back(dev);
    dev->LoadFromStream(rom);
  }

  delete rom;
  #ifdef DEBUG
    DEBUG_PORT.println("--- MyController setup end --------------------- ");
  #endif
  return true;
}


String MyController::GetDeviceListJSON(byte collection_id)
{
  #ifdef DEBUG
      DEBUG_PORT.println("--- MyController GetDeviceListJSON ------------------- ");
  #endif
  String res = "[";
  if (collection_id & 1)
  for (int i = 0;i<_sensors->size();i++)
  {
    AbstractUnit * dev = _sensors->get(i);
	res += dev->GetJSON();
	if (i<_sensors->size()-1) res += ",";
  }

  #ifdef DEBUG
      DEBUG_PORT.print("collection_id = ");DEBUG_PORT.println(collection_id);
      DEBUG_PORT.print("collection_id & 1 = ");DEBUG_PORT.println((collection_id & 1));
      DEBUG_PORT.print("((collection_id>>1) & 1) = ");DEBUG_PORT.println(((collection_id>>1) & 1));
      DEBUG_PORT.print("_sensors->Count() = ");DEBUG_PORT.println(_sensors->size());
      DEBUG_PORT.print("_execUnits->Count() = ");DEBUG_PORT.println(_execUnits->size());
  #endif
  
  if (
	((collection_id & 1) && _sensors->size()>0) 
	&& 
	( ((collection_id>>1) & 1) && _execUnits->size()>0 )
	) res += ",";

  if ((collection_id>>1) & 1)
  for (int i = 0;i<_execUnits->size();i++)
  {
    AbstractUnit * dev = _execUnits->get(i);
    res += dev->GetJSON();
	if (i<_execUnits->size()-1) res += ",";
  }
  res += "]";
  return res;
}

void MyController::AddDevice(AbstractUnit * dev)
{
  if (dev->GetIsSensor()) _sensors->push_back(dev);
  else _execUnits->push_back(dev);
  _allDevCount = _sensors->size() + _execUnits->size();
}

void MyController::setModuleData(byte moduleID, String data)
{
  bool res = false;
  AbstractUnit* md;
  int cnt = _execUnits->size();
  int i = 0;
  
  while(i<cnt && !res)
  {
    md = _execUnits->get(i);
    if (md->GetModuleID() == moduleID) {
      md->SetParams(data);
      res = true;
    }
    i++;
  }
}

String MyController::getModuleData(byte moduleID)
{
  #ifdef DEBUG
    DEBUG_PORT.println("--- MyController getModuleData ------------------- ");
    DEBUG_PORT.print("--- MyController input moduleID = ");DEBUG_PORT.println(moduleID);
  #endif
  String res = "";
  AbstractUnit* md;
  int cnt = _sensors->size();
  int i = 0;
  while(i<cnt && res=="")
  {
    md = _sensors->get(i);
    #ifdef DEBUG
      DEBUG_PORT.print("--- MyController find moduleID = ");DEBUG_PORT.println(md->GetModuleID());
    #endif
    if (md->GetModuleID() == moduleID) {
      //res = md->getData();
      #ifdef DEBUG
        DEBUG_PORT.println("--- getModuleData get sensor json");
      #endif
      res = md->GetJSON();
    }
    i++;
  }

  if (res != "") return res;
  cnt = _execUnits->size();
  i = 0;
  while(i<cnt && res=="")
  {
    md = _execUnits->get(i);
    #ifdef DEBUG
      DEBUG_PORT.print("--- MyController find moduleID = ");DEBUG_PORT.println(md->GetModuleID());
    #endif
    if (md->GetModuleID() == moduleID) {
      //res = md->getData();
      #ifdef DEBUG
        DEBUG_PORT.println("--- getModuleData get execUnit json");
      #endif
      res = md->GetJSON();
    }
    i++;
  }
}

void MyController::DeleteDevice(byte moduleID)
{
  #ifdef DEBUG
      DEBUG_PORT.println("--- MyController DeleteDevice ------------------- ");
      DEBUG_PORT.print("--- MyController input moduleID = ");DEBUG_PORT.println(moduleID);
    #endif
  AbstractUnit* md;
  bool flag = false;
  int cnt = _sensors->size();
  int i = 0;
  while(i<cnt && !flag)
  {
    md = _sensors->get(i);
    #ifdef DEBUG
      DEBUG_PORT.print("--- MyController moduleID = ");DEBUG_PORT.println(md->GetModuleID());
    #endif
    if (md->GetModuleID() == moduleID) {
      delete (_sensors->get(i));
      _sensors->erase(i); 
      flag = true;
      #ifdef DEBUG
        DEBUG_PORT.println("--- MyController Deleted");
      #endif
    }
    i++;
  }

  if (flag) 
  {
    _allDevCount = _sensors->size() + _execUnits->size();
    return;
  }

  cnt = _execUnits->size();
  i = 0;
  while(i<cnt && !flag)
  {
    md = _execUnits->get(i);
    #ifdef DEBUG
      DEBUG_PORT.print("--- MyController moduleID = ");DEBUG_PORT.println(md->GetModuleID());
    #endif
    if (md->GetModuleID() == moduleID) {
      delete (_execUnits->get(i));
      _execUnits->erase(i); 
      flag = true;
      #ifdef DEBUG
        DEBUG_PORT.println("--- MyController Deleted");
      #endif
    }
    i++;
  }
  _allDevCount = _sensors->size() + _execUnits->size();
}

void MyController::WriteDefaultConfig()
{
  #ifdef DEBUG
    DEBUG_PORT.println("--- MyController WriteDefaultConfig ------------ ");
  #endif
  _allDevCount = 0;
  _sensors->clear();
  _execUnits->clear();
  
  EEPROMStream * rom = new EEPROMStream();
  #ifdef DEBUG
    DEBUG_PORT.println("--- MyController SaveToStream ------------------ ");
  #endif
  rom->Seek(EEPROM_POSITION);
  SaveToStream(rom);
  delete rom;
  #ifdef DEBUG
    DEBUG_PORT.println("--- MyController WriteDefaultConfig end -------- ");
  #endif
}

MyController::MyController()
{
  #ifdef DEBUG
    DEBUG_PORT.println("--- MyController Constructor ------------------- ");
    //WriteDefaultConfig();
  #endif
  _sensors = new (Vector<AbstractUnit*>);
  _execUnits = new (Vector<AbstractUnit*>);
  #ifdef DEBUG_RESET_CONFIG
    DEBUG_PORT.println("--- MyController Reset config ------------------- ");
    WriteDefaultConfig();
  #endif

  #ifdef JMP_RESET_DEVLIST
  pinMode(JMP_RESET_DEVLIST, INPUT_CFG);
  if (digitalRead(JMP_RESET_DEVLIST) == RESET_LEVEL)
  {
    #ifdef DEBUG_RESET_CONFIG
    DEBUG_PORT.println("--- MyController Reset config ------------------- ");
    #endif
    WriteDefaultConfig();
  }
  #endif JMP_RESET_DEVLIST
}

MyController::~MyController()
{
  #ifdef DEBUG
    DEBUG_PORT.println("--- MyController Destructor -------------------- ");
  #endif

  delete _sensors;
  delete _execUnits;
}
