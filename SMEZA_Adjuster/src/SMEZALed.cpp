//e

#include "SMEZALed.h"

uint32_t nextLightEventTime = 0; 

led_pattern ledPatternState = powerIsOn;
led_pattern prevLedPatternState = powerIsOn;

void ledInit(){
    pinMode(LED_PIN, OUTPUT);
}

void ledTick(){
    switch (ledPatternState)
    {
    case led_pattern::powerIsOn:
        digitalWrite(LED_PIN, HIGH);
        break;
    
    default:
        ledPatternState = led_pattern::powerIsOn;
        break;
    }

    prevLedPatternState = ledPatternState;
}
