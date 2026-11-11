#pragma once

#include<Arduino.h>

//Motor
constexpr int PWMA = 13;
constexpr int AIN2 = 12;
constexpr int AIN1 = 11;
constexpr int STBY = 10;
//Motor Encoder
constexpr int Motor_A = 2;
constexpr int Motor_B = 3;
//Gimmick Encoder
constexpr int Gimmick_A = 18;
constexpr int Gimmick_B = 19;

void PinInitialize();