#include <Arduino.h>
#include <heltec.h>
#include <oled/OLEDDisplayUi.h>
#include <WiFi.h>
#include <time.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <EEPROM.h>
#include <Stepper.h>
#include "motor.h"


const char* ssid       = "SSID";
const char* password   = "PASSWORD";
IPAddress ip;

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

extern Heltec_ESP32 Heltec;
OLEDDisplayUi ui( Heltec.display );

bool wifiConnected = false;

#define UP 1
#define DOWN 0
#define HALT -1
#define CHA 0


int currentPos = 0;       // Save and Load from EEPROM
int totalDistance = 100;  // Save and Load from EEPROM
int direction = HALT;     // 1,0,-1

bool allowMove = false;
bool isTop = false;
bool isBottom = false;
bool isMoving = false;
const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// for your motor
// motor 1 settings
#define CHA 0
#define ENA 38 // this pin must be PWM enabled pin if Arduino board is used
#define IN1 13
#define IN2 12
#define IN3 14
#define IN4 27
#define ENB 39 // this pin must be PWM enabled pin if Arduino board is used
#define CHB 1

const int CCW = 2; // do not change
const int CW  = 1; // do not change

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, IN1, IN2, IN3, IN4);

#define STEP_SIZE 10
#define MAX_STEPS 300
#define MIN_STEPS 100
#define STEPPER_PIN 24

#define UP_BTN 26
#define DOWN_BTN 32
#define STOP_BTN 33
#define HAL_SWITCH 36

#define EEPROM_SIZE 64

void LoadSettingsFromEEPROM()
{
  Serial.println("Loading settings from EEPROM"); 

  Serial.println("Settings loaded from EEPROM"); 

}

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void disconnectWifi()
{
  if (wifiConnected)
  {
  	//disconnect WiFi as it's no longer needed
  	WiFi.disconnect(true);
  	WiFi.mode(WIFI_OFF);
  }
}


// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
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
  printLocalTime();
  
  return state;
}



void setup() {
  analogReadResolution(12);
  //analogWriteResolution(10);

  
  myStepper.setSpeed(60);

 	Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/);

  if (!EEPROM.begin(EEPROM_SIZE))
  {
    Serial.println("failed to initialise EEPROM"); delay(1000000);
  }
  else
  {
    LoadSettingsFromEEPROM();
  }


  pinMode(HAL_SWITCH, INPUT_PULLUP);
  
  pinMode(UP_BTN, INPUT_PULLUP);
  pinMode(DOWN_BTN, INPUT_PULLUP);
  pinMode(STOP_BTN, INPUT_PULLUP);


  // Initialise wifi connection
  wifiConnected = connectWifi();
  if (wifiConnected)
    Serial.println(" CONNECTED");
  
  Heltec.display->clear();
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
  
  Heltec.display->drawString(64, 0, String(ssid) );
  Heltec.display->drawString(64, 15, ip.toString() );
  Heltec.display->display(); 

}

void loop() {
  //myStepper.step(100);
  //Serial.println("Moving Forward");


  uint i3 = digitalRead(STOP_BTN);
  if (!i3)
  {
    Stop();
  }

 
  uint i4 = digitalRead(HAL_SWITCH);
  if (!i4)
  {
    Serial.println("Detected Magnet");
    Stop();
  }

  uint i1 = digitalRead(UP_BTN);
  if (!i1)
  {
    // step one revolution  in one direction:
    Serial.println("clockwise");
    myStepper.step(stepsPerRevolution);
    MoveUp();
  }  

  uint i2 = digitalRead(DOWN_BTN);
  if (!i2)
  {
    // step one revolution in the other direction:
    Serial.println("counterclockwise");
    myStepper.step(-stepsPerRevolution);
    MoveDown();
  }

  //delay(500); 
 }
