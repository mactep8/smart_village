#ifndef UNIT_SENSOR
#define UNIT_SENSOR

#include "AbstractUnit.h"
#include "EEPROMStream.h"

class SensorUnit: public AbstractUnit
{
  public:
    SensorUnit():AbstractUnit()
    {
      #ifdef DEBUG
        DEBUG_PORT.println("--- SensorUnit constructor");
      #endif
      _isSensor = true;
    }

  void LoadFromStream(EEPROMStream * stream) override;
  void SaveToStream(EEPROMStream * stream) override;
  void SetParams(String prm) override;
    
};

#endif UNIT_SENSOR
