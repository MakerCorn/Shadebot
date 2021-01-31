#include <Arduino.h>
#include <heltec.h>
#include <oled/OLEDDisplayUi.h>
#include <time.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <EEPROM.h>
#include <Stepper.h>

#include "config.h"
#include "motor.h"
#include "util.h"

extern Heltec_ESP32 Heltec;
extern IPAddress ip;

OLEDDisplayUi ui( Heltec.display );

bool wifiConnected;

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, IN1, IN2, IN3, IN4);

void setup() {
  analogReadResolution(12);
 
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
  wifiConnected = ConnectWifi(ssid, password);
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

 }
