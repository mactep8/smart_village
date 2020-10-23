#ifndef UNIT_EXEC
#define UNIT_EXEC

#include "AbstractUnit.h"
#include "EEPROMStream.h"

class ExecUnit: public AbstractUnit
{
  public:
    ExecUnit():AbstractUnit()
    {
      #ifdef DEBUG
        DEBUG_PORT.println("--- ExecUnit constructor");
      #endif
      _isSensor = false;
    }
  void LoadFromStream(EEPROMStream * stream) override;
  void SaveToStream(EEPROMStream * stream) override;
  void SetParams(String prm) override;
};

#endif UNIT_EXEC
