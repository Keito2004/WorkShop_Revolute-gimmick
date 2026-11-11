#include "Encoder.h"

volatile long MotorEncCount = 0;
volatile long GimmickEncCount = 0;

void MotorEncCounter_A(){
    if(digitalRead(Motor_A) == 1){
        if(digitalRead(Motor_B) == 1){
            MotorEncCount++;
        } else{
            MotorEncCount--;
        }   
    }else{
        if(digitalRead(Motor_B) == 0){
            MotorEncCount++;
        }else{
            MotorEncCount--;
        }
    }
}

void MotorEncCounter_B(){
    if(digitalRead(Motor_B) == 1){
        if(digitalRead(Motor_A) == 0){
            MotorEncCount++;
        }else{
            MotorEncCount--;
        }
    }else{
        if(digitalRead(Motor_A) == 1){
            MotorEncCount++;
        }else{
            MotorEncCount--;
        }
    }
}

void GimmickEncCounter_A(){
    if(digitalRead(Gimmick_A) == 1){
        if(digitalRead(Gimmick_B) == 1){
            GimmickEncCount++;
        } else{
            GimmickEncCount--;
        }   
    }else{
        if(digitalRead(Gimmick_B) == 0){
            GimmickEncCount++;
        }else{
            GimmickEncCount--;
        }
    }
}

void GimmickEncCounter_B(){
    if(digitalRead(Gimmick_B) == 1){
        if(digitalRead(Gimmick_A) == 0){
            GimmickEncCount++;
        } else{
            GimmickEncCount--;
        }   
    }else{
        if(digitalRead(Gimmick_A) == 1){
            GimmickEncCount++;
        }else{
            GimmickEncCount--;
        }
    }
}

float GetMotorAngle() {
  long count;
  noInterrupts();
  count = MotorEncCount;
  interrupts();

  return count * 360.0f / 1496.0f;
}

float GetGimmickAngle() {
  long count;
  noInterrupts();
  count = GimmickEncCount;
  interrupts();

  constexpr float countsPerRev = 1496.0f;
  return count * 360.0f / countsPerRev;
}
