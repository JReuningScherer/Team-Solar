
#ifndef SMEZAGLOBALS_H
#define SMEZAGLOBALS_H

#include <stdint.h>

// Define Pin Constants 

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

// Define system variables 
extern int omnipresent;
extern const char DEVICE_ADDRESS[4];
extern const char BROADCAST_ADDRESS[4];
extern const char BILL[27];

#endif
//Full file