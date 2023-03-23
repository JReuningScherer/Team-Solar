/* SMEZA Motor Class 
 * 
 */

#ifndef SMEZAMOTOR_H
#define SMEZAMOTOR_H

#include <stdint.h>
#include "Arduino.h"

#include "SMEZAGlobals.h"

enum motor_state
{
    mtrIdle,
    mtrMovPos,
    mtrMovNeg,
    mtrBrake
};

// bits orders lsb to msb
// 1st bit is start of Adj flag 
// 2nd bit is start of brake flag
#define start_of_adj_flag 0x01
#define start_of_brake_flag 0x02

class SMEZAMotor
{
    private:
    // Define the pins for the limit switches (Arduino Pins)
    const uint8_t POS_LIMIT_PIN;
    const uint8_t NEG_LIMIT_PIN;

    // Define the pins for the motor outputs (Arduino Pins)
    const uint8_t POS_MOTOR_PIN;
    const uint8_t NEG_MOTOR_PIN;

    motor_state motorCurrState = motor_state::mtrIdle;
    uint32_t adjustmentStartTime;
    uint32_t adjustmentStopTime;
    int32_t positionSign;

public:
    motor_state motorNextState = motor_state::mtrIdle;
    uint8_t motorFlags; 
    int32_t positionCounter;
    uint32_t adjTimeout;

    SMEZAMotor(
        uint8_t posMotorPin,
        uint8_t negMotorPin,
        uint8_t posLimitPin,
        uint8_t negLimitPin);

    motor_state getMotorCurrState(void);

    void motorTick(void);
};

#endif