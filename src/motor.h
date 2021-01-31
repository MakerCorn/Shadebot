#ifndef _MOTOR_UTIL_
#define _MOTOR_UTIL_

#define UP 1
#define DOWN 0
#define HALT -1
#define CHA 0
#define CHB 1

#define STEP_SIZE 10
#define MAX_STEPS 20
#define MIN_STEPS 1

void MoveUp();
void MoveDown();
void Stop();

#endif