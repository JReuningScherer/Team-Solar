/* SMEZA Motor Control 
 * 
 */

#ifndef SMEZAMOTORCONTROL_H
#define SMEZAMOTORCONTROL_H

#include "SMEZAGlobals.h"

// Define the motor movement increments in ms 
#define MOTOR_STEP_SIZE_MS 100

// Define the pins for the limit switches 

// Define the pins for the motor outputs 

// Define motor state machine states
enum motor_controller_state {
    mtrIdle,
    mtrBeginAdjustment,
    mtrBusyAdjusting,
    mtrStop
};

enum motor_direction
{
    posX,
    negX,
    posY,
    negY
};

extern motor_controller_state xMotorState;
extern motor_controller_state yMotorState;

// Initialization function
void motorInit(void);

void MotorState(void);

// Check limit switch fxn
uint8_t checkLimit(motor_direction adjustmentDirection);

// Begin adjustment fxn
void beginAdjustment(motor_direction adjustmentDirection, uint16_t numSteps);


#endif
// End of File
