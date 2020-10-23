#ifndef MY_CONFIG
#define MY_CONFIG

#define MIN_STR_LEN 8
#define MAX_STR_LEN 17

#define SSID_DEFAULT "APConfig"
#define PASS_DEFAULT "password"
#define MNAME_DEFAULT "ModuleX"

struct config_t{
  char ssid[MAX_STR_LEN] = SSID_DEFAULT;
  char pass[MAX_STR_LEN] = PASS_DEFAULT;
  char moduleName[MAX_STR_LEN] = MNAME_DEFAULT;
  bool APMode = true;
};

#endif MY_CONFIG
