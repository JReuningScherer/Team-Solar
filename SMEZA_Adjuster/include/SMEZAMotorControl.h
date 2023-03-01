/* SMEZA Motor Control 
 * 
 */

#ifndef SMEZAMOTORCONTROL_H
#define SMEZAMOTORCONTROL_H

#include <Arduino.h>

#include "SMEZAGlobals.h"

// Define the motor movement increments in ms 
#define MOTOR_STEP_SIZE_MS 100

// Define the pins for the limit switches (Arduino Pins)
#define POS_X_LIMIT_PIN 12
#define NEG_X_LIMIT_PIN 13
#define POS_Y_LIMIT_PIN 14
#define NEG_Y_LIMIT_PIN 15

// Define the pins for the motor outputs (Arduino Pins)
#define POS_X_MOTOR_PIN 10
#define NEG_X_MOTOR_PIN 9
#define POS_Y_MOTOR_PIN 3
#define NEG_Y_MOTOR_PIN 11

// Define motor state machine states
enum motor_controller_state {
    mtrIdle,
    mtrBeginAdjustment,
    mtrBusyAdjusting
};


extern motor_controller_state xMotorState;
extern motor_controller_state yMotorState;

enum motor_direction
{
    posX,
    negX,
    posY,
    negY,
    none
}; 
extern motor_direction xMotorDirection;
extern motor_direction yMotorDirection;


// Initialization function
void motorInit(void);

void MotorState(motor_controller_state *mtrState);

// Check limit switch fxn
int8_t checkLimit(motor_direction adjustmentDirection);

// Begin adjustment fxn
void beginAdjustment(motor_direction adjustmentDirection, uint16_t numSteps);


#endif
// End of File
