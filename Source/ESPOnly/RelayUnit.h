#ifndef UNIT_RFRELAY
#define UNIT_RFRELAY
#include "EEPROMStream.h"
#include "ExecUnit.h"

#define OPER_STATE_IDLE 0
#define OPER_STATE_ON   1
#define OPER_STATE_OFF  2
class RelayUnit: public ExecUnit
{
  private:
    byte _relayPin;
    byte _state;            // default relay state
    long _key;              // RF Button code
    uint32_t _time;         // Last switch time
    uint32_t _timeInterval; // Time to autoswitch
    uint32_t _timeDelaySwitch; // Time to start switch
    uint8_t  _operState;
  public:
    RelayUnit();
    ~RelayUnit();
    void LoadFromStream(EEPROMStream * stream) override;
    void SaveToStream(EEPROMStream * stream) override;

    void Switch();
    long getRFKey();
    String GetJSONAdd() override;
    void SetParams(String prm) override;
    void Operate() override;
};

#endif UNIT_RFRELAY
