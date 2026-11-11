#include"PinAssign.h"

void MotorEncCounter_A();
void MotorEncCounter_B();
void GimmickEncCounter_A();
void GimmickEncCounter_B();

void PinInitialize(){
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(PWMA, OUTPUT);

    pinMode(Motor_A, INPUT);
    pinMode(Motor_B, INPUT);

    attachInterrupt(digitalPinToInterrupt(AIN1),MotorEncCounter_A, CHANGE);
    attachInterrupt(digitalPinToInterrupt(AIN2),MotorEncCounter_B, CHANGE);

    pinMode(Gimmick_A, INPUT);
    pinMode(Gimmick_B, INPUT);
    
    attachInterrupt(digitalPinToInterrupt(Gimmick_A),GimmickEncCounter_A, CHANGE);
    attachInterrupt(digitalPinToInterrupt(Gimmick_B),GimmickEncCounter_B, CHANGE);
}