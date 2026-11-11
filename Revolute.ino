#include<math.h>
#include "PinAssign.h"
#include "DriveMotor.h"
#include "Encoder.h"



float MotorAngle = 0.0f;
float GimmickAngle = 0.0f;
int GimmickFlag = 0;
int PreviousGimmickFlag = 0;

void setup(){
    Serial.begin(115200);

    PinInitialize();//ピン配置
}

void loop(){
    MotorAngle = GetMotorAngle();
    GimmickAngle = GetGimmickAngle();
    
    if (GimmickAngle > 360.0f) {
        GimmickFlag = 3;
    } else if (GimmickAngle > 240.0f) {
        GimmickFlag = 2;
    } else if (GimmickAngle > 120.0f) {
        GimmickFlag = 1;
    } else {
        GimmickFlag = 0;
    }

if(GimmickFlag != PreviousGimmickFlag)
    {
        switch(GimmickFlag){
            case 0: 
                DriveMotor(0.0f);
                break;

            case 1:
                DriveMotor(100.0f);
                break;

            case 2:
                DriveMotor(50.0f);
                break;

            case 3:
                DriveMotor(50.0f);
                break;

            default: 
                DriveMotor(0.0f);
                break;
        }
    }
    PreviousGimmickFlag = GimmickFlag;
}