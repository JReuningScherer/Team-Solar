#include "SMEZAMotorControl.h"
/* SMEZA Motor Control 
 * 
 */

motor_controller_state xMotorState = mtrIdle;
motor_controller_state yMotorState = mtrIdle;

motor_direction xMotorDirection = none;
motor_direction yMotorDirection = none;

/**
 * @brief Initializes the motor control system 
 * 
 */
void motorInit(void){
    // Set up Limit Switch Pins 
    // Set up the pushbutton pin to be an input:
    pinMode(POS_X_LIMIT_PIN, INPUT);
    pinMode(NEG_X_LIMIT_PIN, INPUT);
    pinMode(POS_Y_LIMIT_PIN, INPUT);
    pinMode(NEG_Y_LIMIT_PIN, INPUT);

    // Set up Motor output pins 
    pinMode(POS_X_MOTOR_PIN, OUTPUT);
    pinMode(NEG_X_MOTOR_PIN, OUTPUT);
    pinMode(POS_Y_MOTOR_PIN, OUTPUT);
    pinMode(NEG_Y_MOTOR_PIN, OUTPUT);

}

/**
 * @brief Motor State Machine Tick
 * 
 */
void MotorState(motor_controller_state &mtrState)
{

    switch (mtrState)
    {
    case mtrIdle:
        // Motors in Brake mode 
        break;
    case mtrBeginAdjustment:
        // Check Limit switch in the direction of travel. If at limit, go back to 
        // idle

        // Set motorTimeout Value
        break;
    case mtrBusyAdjusting:
        // Run Motors based on motor
        break;
    
    default:
        mtrState = mtrIdle;
        break;
    }

}

/**
 * @brief Checks the limit switch corresponding to the provided direction of travel.
 * 
 * @param adjustmentDirection 
 * @return int8_t 
 */
int8_t checkLimit(motor_direction adjustmentDirection)
{
    int8_t limitValue = -1;
    switch (adjustmentDirection)
    {
    case posX:
        limitValue = digitalRead(POS_X_LIMIT_PIN);
        break;
    case negX:
        limitValue = digitalRead(NEG_X_LIMIT_PIN);
        break;
    case posY:
        limitValue = digitalRead(POS_Y_LIMIT_PIN);
        break;
    case negY:
        limitValue = digitalRead(NEG_Y_LIMIT_PIN);
        break;
    
    default:
        // If none of the commands matched, 
        limitValue = -1;
        break;
    }
    return limitValue;
}

/**
 * @brief This function is how other parts of the adjuster begin an adjustment. 
 * The function looks at the current state and updates the control variables 
 * and state machine accordingly. 
 * 
 * @param adjustmentDirection The direction of the adjustment
 * @param numSteps The number of steps to adjust
 */
void beginAdjustment(motor_direction adjustmentDirection, uint16_t numSteps)
{
}

// End of File
