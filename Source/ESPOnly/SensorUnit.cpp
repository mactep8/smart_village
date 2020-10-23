#include "SensorUnit.h"
#include "AbstractUnit.h"
#include "EEPROMStream.h"

void SensorUnit::LoadFromStream(EEPROMStream * stream)
{
  AbstractUnit::LoadFromStream(stream);
}

void SensorUnit::SaveToStream(EEPROMStream * stream)
{
  AbstractUnit::SaveToStream(stream);
}

void SensorUnit::SetParams(String prm)
{
  #ifdef DEBUG
    DEBUG_PORT.println("--- SensorUnit Set Params");
  #endif
  AbstractUnit::SetParams(prm);
}
