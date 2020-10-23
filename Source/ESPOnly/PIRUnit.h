#ifndef UNIT_PIR
#define UNIT_PIR

#include "SensorUnit.h"
#include "EEPROMStream.h"
#include<Vector.h>

class PIRUnit: public SensorUnit
{
  private:
    byte _pin;
    uint32_t _lastTime;
    Vector<AbstractUnit*>* _execUnits;
    long _key;
    byte _lvl;
  public:
    PIRUnit(Vector<AbstractUnit*>* execUnits);
    ~PIRUnit();

    void LoadFromStream(EEPROMStream * stream) override;
    void SaveToStream(EEPROMStream * stream) override;
    void SetParams(String prm) override;
    void Operate()override;
    String GetJSONAdd() override;
};

#endif UNIT_PIR
