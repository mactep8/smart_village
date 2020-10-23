#ifndef MY_NETWORK
#define MY_NETWORK
#include "config.h"
#include "EEPROMStream.h"

class MyNetwork{
  private:
    config_t _cfg;
    //EEPROMStream* stream;
    
    bool ValidateString(char* value);
    void LoadConfig();
    void ResetConfig();
    void SaveConfig(EEPROMStream* stream);
    void Run();
  public:
    MyNetwork();
    ~MyNetwork();
    bool isAP();
    String Scan();
    void SetConfig(
      String aSSID,
      String aSSIDkey,
      String aModuleName,
      String aIsAP
      );
    String GetConfigJSON();
};

#endif MY_NETWORK
