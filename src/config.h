#ifndef _CONFIG_
#define _CONFIG_

const char* ssid       = "SSID";
const char* password   = "PASSWORD";

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution

#define ENA 38 // this pin must be PWM enabled pin if Arduino board is used
#define IN1 13
#define IN2 12
#define IN3 14
#define IN4 27
#define ENB 39 // this pin must be PWM enabled pin if Arduino board is used

#define UP_BTN 26
#define DOWN_BTN 32
#define STOP_BTN 33

#define HAL_SWITCH 36

#endif