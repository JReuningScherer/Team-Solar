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
    switch (adjustmentDirection)
    {
    case posX:
        /* code */
        break;
    
    default:
        // If none of the commands matched, 
        return -1;
        break;
    }
    return 0;
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
