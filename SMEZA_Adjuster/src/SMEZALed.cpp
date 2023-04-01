//e

#include "SMEZALed.h"

uint32_t nextLedLowTime = 0; 
uint32_t nextLedCycleTime = 0; 

led_pattern ledPatternState = powerIsOn;
led_pattern prevLedPatternState = powerIsOn;

void ledInit(){
    pinMode(LED_PIN, OUTPUT);
}

void ledTick(){
    if(digitalRead(POS_X_LIMIT_PIN) || digitalRead(NEG_X_LIMIT_PIN) || digitalRead(POS_Y_LIMIT_PIN) || digitalRead(NEG_Y_LIMIT_PIN)){
        ledPatternState = led_pattern::limitIsTripped;
    } else if((xMotor.getMotorCurrState() != motor_state::mtrIdle) || (yMotor.getMotorCurrState() != motor_state::mtrIdle)){
        ledPatternState = led_pattern::adjustmentInProgress;
    } else {
        ledPatternState = led_pattern::powerIsOn;
    }

    switch (ledPatternState)
    {
    case led_pattern::powerIsOn:
        analogWrite(LED_PIN, 128);
        break;
    case led_pattern::adjustmentInProgress:
        if(millis() >= nextLedCycleTime){
            digitalWrite(LED_PIN, HIGH);
            nextLedCycleTime = millis() + ADJ_IN_PROGRESS_PERIOD;
            nextLedLowTime= millis() + ADJ_IN_PROGRESS_HIGHTIME;
        }
        if(millis() >= nextLedLowTime){
            digitalWrite(LED_PIN, LOW);
        }
        break;
    case led_pattern::limitIsTripped:
        if(millis() >= nextLedCycleTime){
            digitalWrite(LED_PIN, HIGH);
            nextLedCycleTime = millis() + LIMIT_TRIPPED_PERIOD;
            nextLedLowTime= millis() + LIMIT_TRIPPED_HIGHTIME;
        }
        if(millis() >= nextLedLowTime){
            digitalWrite(LED_PIN, LOW);
        }
        break;
    default:
        ledPatternState = led_pattern::powerIsOn;
        break;
    }

    prevLedPatternState = ledPatternState;
}
