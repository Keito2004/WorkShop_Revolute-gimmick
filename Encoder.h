#pragma once

#include<Arduino.h>
#include"PinAssign.h"

extern volatile long MotorEncCount;
extern volatile long GimmickEncCount;

void MotorEncCounter_A();
void MotorEncCounter_B();
void GimmickEncCounter_A();
void GimmickEncCounter_B();

float GetMotorAngle();
float GetGimmickAngle();

