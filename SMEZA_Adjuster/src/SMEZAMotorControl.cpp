/* SMEZA Motor Control 
 * 
 */

#include "SMEZAMotorControl.h"

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


// End of File
