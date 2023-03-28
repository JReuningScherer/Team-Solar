/* SMEZA Motor Control 
 * 
 */


#include "SMEZAMotorControl.h"


uint8_t adjCommandRecievedFlag = 0; 
uint8_t adjCommandAxis = 0;
motor_direction adjCommandDirection = motor_direction::noDirection;
uint16_t adjCommandNumSteps = 0;

SMEZAMotor xMotor(POS_X_MOTOR_PIN, NEG_X_MOTOR_PIN, POS_X_LIMIT_PIN, NEG_X_LIMIT_PIN, 0);
SMEZAMotor yMotor(POS_Y_MOTOR_PIN, NEG_Y_MOTOR_PIN, POS_Y_LIMIT_PIN, NEG_Y_LIMIT_PIN, 1);

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

    SMEZAMotor xMotor(POS_X_MOTOR_PIN, NEG_X_MOTOR_PIN, POS_X_LIMIT_PIN, NEG_X_LIMIT_PIN, 0);
    SMEZAMotor yMotor(POS_Y_MOTOR_PIN, NEG_Y_MOTOR_PIN, POS_Y_LIMIT_PIN, NEG_Y_LIMIT_PIN, 1);

}

void tickMotorCtrl(SMEZAMotor *mtr)
{

    
    if (adjCommandRecievedFlag && (adjCommandAxis == mtr->AXIS_INDICATOR)){
        #if(MTR_CTRL_DEBUG_FEEDBACK >= 1)
        Serial.print("\tFLAG:");
        Serial.println(adjCommandRecievedFlag);
        Serial.print("\tAXIS:");
        Serial.println(adjCommandAxis);
        Serial.print("\tDIR:");
        Serial.println(adjCommandDirection);
        Serial.print("\tSTEPS:");
        Serial.println(adjCommandNumSteps);
        Serial.print("\tPOSITION:");
        Serial.println(mtr->positionCounter);

        #endif


        switch(mtr->getMotorCurrState()){
        case motor_state::mtrIdle:
            if(adjCommandDirection == motor_direction::pos){
                mtr->motorNextState = motor_state::mtrMovPos;
            }
            if(adjCommandDirection == motor_direction::neg){
                mtr->motorNextState = motor_state::mtrMovNeg;
            }
            // Update the movement timeout
            mtr->adjTimeout = millis() + (MILLIS_PER_STEP * adjCommandNumSteps);
            adjCommandRecievedFlag = 0;
            mtr->motorFlags |= start_of_adj_flag;
            break;
        case motor_state::mtrMovPos:
            // New movement is opposite the current direction of movement 
            if(adjCommandDirection == motor_direction::neg){
                // Brake
                mtr->motorNextState = motor_state::mtrBrake;
                // SET the command received flag to be handled again on the next cycle
                adjCommandRecievedFlag = 1;
                break;
            }   
            // New Movement is in the same direction as current movement
            // Update the movement timeout
            mtr->adjTimeout = millis() + (MILLIS_PER_STEP * adjCommandNumSteps);

            // Reset the command received flag
            adjCommandRecievedFlag = 0;
            break;
        case motor_state::mtrMovNeg: 
            // New movement is opposite the current direction of movement 
            if(adjCommandDirection == motor_direction::pos){
                // Brake
                mtr->motorNextState = motor_state::mtrBrake;
                // SET the command received flag to be handled again on the next cycle
                adjCommandRecievedFlag = 1;
                break;
            }
            // New Movement is in the same direction as current movement
            // Update the movement timeout 
            mtr->adjTimeout = millis() + uint32_t(MILLIS_PER_STEP * adjCommandNumSteps);

            // Reset the command received flag
            adjCommandRecievedFlag = 0;
            break;
        case motor_state::mtrBrake:
            if(adjCommandDirection == motor_direction::pos){
                mtr->motorNextState = motor_state::mtrMovPos;
            }
            if(adjCommandDirection == motor_direction::neg){
                mtr->motorNextState = motor_state::mtrMovNeg;
            }
            // Update the movement timeout
            mtr->adjTimeout = millis() + (MILLIS_PER_STEP * adjCommandNumSteps);
            adjCommandRecievedFlag = 0;
            mtr->motorFlags |= start_of_adj_flag;
            break;
        default:
            mtr->motorNextState = motor_state::mtrIdle;
        }
        #if(MTR_CTRL_DEBUG_FEEDBACK >= 1)
        Serial.print("\tTIMEOUT:");
        Serial.println(mtr->adjTimeout);

        #endif
        return;
    }

    // If no new command is pending, then...
    switch(mtr->getMotorCurrState()){
        case motor_state::mtrMovPos:
            // If millis > timeout, then brake
            if (millis() > mtr->adjTimeout){
                mtr->motorNextState = motor_state::mtrBrake;
            }
            break;
        case motor_state::mtrMovNeg: 
            // If millis > timeout, then brake
            if (millis() > mtr->adjTimeout){
                mtr->motorNextState = motor_state::mtrBrake;
            }
            break;
        default:
            mtr->motorNextState = motor_state::mtrIdle;
        }
        return;


}

// End of File
