#include<Arduino.h>
#include<math.h>

#include "DriveMotor.h"
#include "Encoder.h"
#include "PinAssign.h"


constexpr float TravelPerRev = 100.0f; // 出力軸1回転の降下量[mm]
constexpr float DownSign = 1.0f;       // 降下時に角度が減るなら-1
constexpr float Kp = 2.0f;             
constexpr float AngleTolerance = 2.0f;
constexpr int MaxPWM = 150;

static float targetAngle = 0.0f;
static bool moving = false;

void DriveMotor(float length) {
  if (moving || length <= 0.0f) {
    return;
  }

  float rotation = length * 360.0f / TravelPerRev;
  targetAngle = GetMotorAngle() + DownSign * rotation;
  moving = true;
}

void UpdateMotor() {
  if (!moving) {
    return;
  }

  float error = targetAngle - GetMotorAngle();

  if (fabs(error) <= AngleTolerance) {
    SetMotorPWM(0);
    moving = false;
    return;
  }

  int pwm = (int)constrain(Kp * error, -MaxPWM, MaxPWM);
  SetMotorPWM(pwm);
}

void SetMotorPWM(int pwm) {
  pwm = constrain(pwm, -255, 255);

  if (pwm == 0) {
    // ショートブレーキ
    digitalWrite(STBY, HIGH);
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, LOW);
    analogWrite(PWMA, 0);
    return;
  }

  // 方向を設定する間はブレーキ
  analogWrite(PWMA, 0);

  if (pwm > 0) {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
  } else {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
  }

  digitalWrite(STBY, HIGH);
  analogWrite(PWMA, abs(pwm));
}