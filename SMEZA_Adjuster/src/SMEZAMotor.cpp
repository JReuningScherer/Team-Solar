#include "SMEZAMotor.h"

SMEZAMotor::SMEZAMotor(
    uint8_t posMotorPin, 
    uint8_t negMotorPin, 
    uint8_t posLimitPin, 
    uint8_t negLimitPin) : 
    POS_LIMIT_PIN(posLimitPin), NEG_LIMIT_PIN(negLimitPin),
    POS_MOTOR_PIN(posMotorPin), NEG_MOTOR_PIN(negMotorPin)
{
    state = mtrIdle;
    direction = noDirection;
    adjustmentTimeout = 0;
    brakeTimeout = 0;
    adjustmentStartTime = 0;
    adjustmentStopTime = 0;
    position = 0;

}


/**
 * @brief Tick the motor state machine 
 * 
 */
void SMEZAMotor::motorTick(){
    switch (state)
    {
    case mtrIdle:
        // Motors in off mode
        digitalWrite(POS_MOTOR_PIN, LOW);
        digitalWrite(NEG_LIMIT_PIN, LOW);
        break;
    case mtrBusyAdjusting:
        // Check if limits are ok
        if(limitIsTripped()){
            startBraking();
            state = mtrBrake;
            break;
        }
        // Check timeout
        if (millis() >= adjustmentTimeout){
            startBraking();
            state = mtrBrake;
            break;
        }
        
        if(direction == pos){
            digitalWrite(POS_MOTOR_PIN, HIGH);
            digitalWrite(NEG_MOTOR_PIN, LOW);
        }
        if(direction == neg){
            digitalWrite(POS_MOTOR_PIN, LOW);
            digitalWrite(NEG_MOTOR_PIN, HIGH);
        }

        break;
    case mtrBrake:
        // Motors in brake mode
        digitalWrite(POS_MOTOR_PIN, HIGH);
        digitalWrite(NEG_LIMIT_PIN, HIGH);
        if(millis() >= brakeTimeout){
            state = mtrIdle;
        }
        break;
    case mtrDirectionSwitchBrake:
        // Motors in brake mode
        digitalWrite(POS_MOTOR_PIN, HIGH);
        digitalWrite(NEG_LIMIT_PIN, HIGH);
        if(millis() >= brakeTimeout){
            state = mtrBusyAdjusting;
        }
        break;

    default:
        state = mtrIdle;
        break;
    }
}

/**
 * @brief Checks the limit switch corresponding to the direction of adjustment. 
 * 
 * @return int8_t. 
 * Returns 0 if the limit switch is not tripped. 
 * Returns 1 if the limit switch is tripped.
 * Returns -1 if the direction is `noDirection`.

 */
int8_t SMEZAMotor::limitIsTripped(){
    int8_t limitValue;
    switch (direction)
    {
    case pos:
        limitValue = digitalRead(POS_LIMIT_PIN);
        break;
    case neg:
        limitValue = digitalRead(NEG_LIMIT_PIN);
        break;
    default:
        // If none of the commands matched, 
        limitValue = -1;
        break;
    }
    return limitValue;
}

void SMEZAMotor::startBraking(void)
{
    adjustmentStopTime = millis();
    digitalWrite(POS_MOTOR_PIN, HIGH);
    digitalWrite(NEG_LIMIT_PIN, HIGH);
    // Update Position
    if(direction == pos){
        position += (adjustmentStopTime - adjustmentStartTime) / MOTOR_STEP_SIZE_MS;
    }
    if(direction == neg){
        position -= (adjustmentStopTime - adjustmentStartTime) / MOTOR_STEP_SIZE_MS;
    }

    brakeTimeout = adjustmentStopTime + MOTOR_BRAKE_TIME_MS;


}

/**
 * @brief Begin an adjustment. 
 * The function looks at the current state and updates the control variables 
 * and state machine accordingly. 
 * 
 * @param adjustmentDirection The direction of the adjustment
 * @param numSteps The number of steps to adjust
 */
void SMEZAMotor::beginAdjustment(motor_direction dir, uint16_t numSteps){
    // If the inputs are nonsensical, ignore them
    if(dir == noDirection) {
        return;
    }
    if(numSteps == 0){
        return;
    }
    
    // Check that the limit switch in the direction of travel is not tripped. 
    // If it is tripped, return
    if (limitIsTripped()){
        return;
    }


    if(dir == direction){
        adjustmentStartTime = millis();
        adjustmentTimeout = adjustmentStartTime + (numSteps * MOTOR_STEP_SIZE_MS);
        state = mtrBusyAdjusting;
    } else {
        startBraking();
        direction = dir;
        adjustmentStartTime = millis() + MOTOR_BRAKE_TIME_MS;
        adjustmentTimeout = adjustmentStartTime + (numSteps * MOTOR_STEP_SIZE_MS) + MOTOR_BRAKE_TIME_MS;
        state = mtrDirectionSwitchBrake;
    }




}

