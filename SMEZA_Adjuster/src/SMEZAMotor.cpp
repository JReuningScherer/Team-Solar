#include "SMEZAMotor.h"

SMEZAMotor::SMEZAMotor(
    uint8_t posMotorPin, 
    uint8_t negMotorPin, 
    uint8_t posLimitPin, 
    uint8_t negLimitPin) : 
    POS_LIMIT_PIN(posLimitPin), NEG_LIMIT_PIN(negLimitPin),
    POS_MOTOR_PIN(posMotorPin), NEG_MOTOR_PIN(negMotorPin)
{
    motorCurrState = mtrIdle;
    motorNextState = mtrIdle;
    motorFlags = 0; 
    adjustmentStartTime = 0;
    adjustmentStopTime = 0;
    adjTimeout = 1111;
    positionCounter = 0;
    positionSign = 1;
}

motor_state SMEZAMotor::getMotorCurrState(void)
{
    return motorCurrState;
}

void SMEZAMotor::motorTick(void)
{
    motorCurrState = motorNextState;

    if(motorCurrState == motor_state::mtrIdle){
            digitalWrite(POS_MOTOR_PIN, LOW);
            digitalWrite(NEG_MOTOR_PIN, LOW);
            
    } 
    if(motorCurrState == motor_state::mtrMovPos) {
            digitalWrite(POS_MOTOR_PIN, HIGH);
            digitalWrite(NEG_MOTOR_PIN, LOW);
            positionSign = 1;
            if (digitalRead(POS_LIMIT_PIN)){
                motorCurrState = motor_state::mtrBrake;
                motorNextState = motor_state::mtrBrake;
                motorFlags = motorFlags | start_of_brake_flag;
            }
            if (motorFlags & start_of_adj_flag) {
                adjustmentStartTime = millis();
                motorFlags = motorFlags & !(start_of_adj_flag);
            }
            
    }
    if(motorCurrState == motor_state::mtrMovNeg) {
            digitalWrite(POS_MOTOR_PIN, LOW);
            digitalWrite(NEG_MOTOR_PIN, HIGH);
            positionSign = -1;
            if (digitalRead(NEG_LIMIT_PIN)){
                motorCurrState = motor_state::mtrBrake;
                motorNextState = motor_state::mtrBrake;

                motorFlags = motorFlags | start_of_brake_flag;
            }
            if (motorFlags & start_of_adj_flag) {
                adjustmentStartTime = millis();
                motorFlags = motorFlags & !(start_of_adj_flag);
            
            }
    }
    if(motorCurrState == motor_state::mtrBrake) {
            digitalWrite(POS_MOTOR_PIN, HIGH);
            digitalWrite(NEG_MOTOR_PIN, HIGH);
            if (start_of_brake_flag) {
                adjustmentStopTime = millis();
                        Serial.print("\tSTART:");
                        Serial.println(this->adjustmentStartTime);
                        Serial.print("\tSTOP:");
                        Serial.println(this->adjustmentStopTime);
                        Serial.print("\tDIFF:");
                        Serial.println((adjustmentStopTime - adjustmentStartTime));
                        Serial.print("\tSIGNED:");
                        Serial.println(positionSign*(adjustmentStopTime - adjustmentStartTime));
                positionCounter = positionCounter + positionSign*int32_t(adjustmentStopTime - adjustmentStartTime);
                motorFlags = motorFlags & !(start_of_brake_flag);
            }
            
    }
}
