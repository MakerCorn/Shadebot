#include <Arduino.h>
#include <heltec.h>
#include <WiFi.h>

IPAddress ip;

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

void LoadSettingsFromEEPROM()
{
  Serial.println("Loading settings from EEPROM"); 

  Serial.println("Settings loaded from EEPROM"); 

}

void PrintLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}


void DisconnectWifi()
{
  	//disconnect WiFi as it's no longer needed
  	WiFi.disconnect(true);
  	WiFi.mode(WIFI_OFF);
}


// connect to wifi – returns true if successful or false if not
boolean ConnectWifi(const char* ssid, const char* password)
{
  boolean state = true;
  int i = 0;
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.printf("Connecting to %s ", ssid);

  // Wait for connection
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20){
      state = false; break;
    }
    i++;
  }
  Serial.println("");
  if (state){
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    ip = WiFi.localIP();
    Serial.println(ip.toString());
  }
  else {
    Serial.println("Connection failed.");
  }
  delay(100);
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  PrintLocalTime();
  
  return state;
}