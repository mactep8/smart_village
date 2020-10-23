#ifndef UNIT_DHT
#define UNIT_DHT

#include "SensorUnit.h"
#include "EEPROMStream.h"
#include "DHTesp.h"

class DHTUnit: public SensorUnit
{
  private:
    DHTesp* _dht;
    byte _pin;
    uint32_t _lastTime;
    float _temp = 0;
    float _hum = 0;
  public:
    DHTUnit();
    ~DHTUnit();

    void LoadFromStream(EEPROMStream * stream) override;
    void SaveToStream(EEPROMStream * stream) override;
    void SetParams(String prm) override;
    void Operate()override;
    String GetJSONAdd() override;
};

#endif UNIT_DHT
