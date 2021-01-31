#ifndef _SHADEBOT_UTIL_ 
#define _SHADEBOT_UTIL_

#define EEPROM_SIZE 64

void LoadSettingsFromEEPROM();
void PrintLocalTime();
void DisconnectWifi();
boolean ConnectWifi(const char*, const char*);

#endif