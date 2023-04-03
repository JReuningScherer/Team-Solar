/* SMEZA Motor Control 
 * 
 */

#ifndef SMEZAMOTORCONTROL_H
#define SMEZAMOTORCONTROL_H

#include <Arduino.h>

#include "SMEZAGlobals.h"
#include "SMEZAMotor.h"

#define MTR_CTRL_DEBUG_FEEDBACK 0

#define MILLIS_PER_STEP (uint32_t)100

// Variables for incoming command storage 
enum motor_direction
{
    pos,
    neg,
    noDirection
};


extern uint8_t adjCommandRecievedFlag;
extern uint8_t adjCommandAxis;
extern motor_direction adjCommandDirection;
extern uint32_t adjCommandNumSteps;

extern SMEZAMotor xMotor;
extern SMEZAMotor yMotor;

// Initialization function
void motorInit(void);

//void tickMotorCtrl(SMEZAMotor &mtr);

void tickMotorCtrl(SMEZAMotor *mtr);


#endif
// End of File