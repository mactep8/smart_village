#ifndef MY_CONTROLLER
#define MY_CONTROLLER

#include "EEPROMStream.h"
#include "AbstractUnit.h"
#include<Vector.h>

class MyController
{
  private:
    int _allDevCount;
    Vector<AbstractUnit*> * _sensors;
    Vector<AbstractUnit*> * _execUnits;
  public:
    MyController();
    ~MyController();
    bool setup();
    void loop();

    void SaveToStream(EEPROMStream * stream);
    void WriteDefaultConfig();
    void AddDevice(AbstractUnit * dev);
    void DeleteDevice(byte moduleID);
    AbstractUnit * GetDeviceInstance(byte deviceCode);
	  String GetDeviceListJSON(byte collection_id);
   String getModuleData(byte moduleID);
   void setModuleData(byte moduleID, String data);
};

#endif MY_CONTROLLER
