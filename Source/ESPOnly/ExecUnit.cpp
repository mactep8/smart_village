#include "ExecUnit.h"
#include "AbstractUnit.h"
#include "EEPROMStream.h"

void ExecUnit::LoadFromStream(EEPROMStream * stream)
{
  AbstractUnit::LoadFromStream(stream);
}

void ExecUnit::SaveToStream(EEPROMStream * stream)
{
  AbstractUnit::SaveToStream(stream);
}

void ExecUnit::SetParams(String prm)
{
  AbstractUnit::SetParams(prm);
}
