#include "SMEZAMotorControl.h"
/* SMEZA Motor Control 
 * 
 */

motor_controller_state xMotorState = mtrIdle;
motor_controller_state yMotorState = mtrIdle;

/**
 * @brief Initializes the motor control system 
 * 
 */
void motorInit(void){
    // Set up Limit Switch Pins 

    // Set up Motor output pins 


}

/**
 * @brief Motor State Machine Tick
 * 
 */
void MotorState(void)
{

}

/**
 * @brief Checks the limit switch corresponding to the provided direction of travel
 * 
 * @param adjustmentDirection 
 * @return uint8_t 
 */
uint8_t checkLimit(motor_direction adjustmentDirection)
{
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
