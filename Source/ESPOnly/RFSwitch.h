#ifndef UNIT_RFSWITCH
#define UNIT_RFSWITCH

#include "AbstractUnit.h"
#include "SensorUnit.h"
#include "Devices.h"
#include "EEPROMStream.h"
#include <RCSwitch.h>
#include<Vector.h>

class RFSwitchUnit: public SensorUnit
{
  private:
    RCSwitch * _mySwitch;
    byte _pin;
    long _lastKey;
    uint32_t _lastTime;
    Vector<AbstractUnit*>* _execUnits;
    bool _enabled;
  public:
    RFSwitchUnit(Vector<AbstractUnit*>* execUnits);
    

    ~RFSwitchUnit()
    {
      delete _mySwitch;
    }

    void LoadFromStream(EEPROMStream * stream) override;
    void SaveToStream(EEPROMStream * stream) override;

    void Operate()override;
    String GetJSONAdd() override;
    void SetParams(String prm) override;
};

#endif UNIT_RFSWITCH
