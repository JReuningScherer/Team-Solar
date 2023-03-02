/* SMEZA Motor Control 
 * 
 */

#ifndef SMEZAMOTORCONTROL_H
#define SMEZAMOTORCONTROL_H

#include <Arduino.h>

#include "SMEZAGlobals.h"
#include "SMEZAMotor.h"


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

extern SMEZAMotor xMotor(POS_X_MOTOR_PIN, NEG_X_MOTOR_PIN, POS_X_LIMIT_PIN, NEG_X_LIMIT_PIN);
extern SMEZAMotor yMotor(POS_Y_MOTOR_PIN, NEG_Y_MOTOR_PIN, POS_Y_LIMIT_PIN, NEG_Y_LIMIT_PIN);

// Initialization function
void motorInit(void);

#endif
// End of File
