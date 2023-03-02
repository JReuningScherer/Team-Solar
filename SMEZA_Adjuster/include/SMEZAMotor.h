/* SMEZA Motor Class 
 * 
 */

#ifndef SMEZAMOTOR_H
#define SMEZAMOTOR_H

#include <Arduino.h>

// Define the motor movement increments in ms 
#define MOTOR_STEP_SIZE_MS 100
#define MOTOR_BRAKE_TIME_MS 50


// Define motor state machine states
enum motor_controller_state {
    mtrIdle,
    mtrBusyAdjusting,
    mtrBrake,
    mtrDirectionSwitchBrake
};

enum motor_direction
{
    pos,
    neg,
    noDirection
}; 


class SMEZAMotor
{
private:
    // Define the pins for the limit switches (Arduino Pins)
    const uint8_t POS_LIMIT_PIN;
    const uint8_t NEG_LIMIT_PIN;

    // Define the pins for the motor outputs (Arduino Pins)
    const uint8_t POS_MOTOR_PIN;
    const uint8_t NEG_MOTOR_PIN;


    uint16_t adjustmentTimeout;
    uint16_t brakeTimeout;

    uint16_t adjustmentStartTime;
    uint16_t adjustmentStopTime;

    int8_t limitIsTripped(void);

    void startBraking(void);

public:
    motor_controller_state state;
    motor_direction direction;
    int16_t position;

    SMEZAMotor(
        uint8_t posMotorPin,
        uint8_t negMotorPin,
        uint8_t posLimitPin,
        uint8_t negLimitPin);

    void motorTick();
    void beginAdjustment(motor_direction dir, uint16_t numSteps);
};

#endif
// End of File
