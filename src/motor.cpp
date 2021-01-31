#include <heltec.h>
#include "motor.h"

int currentPos = 0;       // Save and Load from EEPROM
int totalDistance = 100;  // Save and Load from EEPROM
int direction = HALT;     // 1,0,-1

bool allowMove = false;
bool isTop = false;
bool isBottom = false;
bool isMoving = false;

void MoveUp()
{
    Serial.println("Move Up");
}

void MoveDown()
{
    Serial.println("Move Down");
}

void Stop()
{
    Serial.println("Stop");
}
