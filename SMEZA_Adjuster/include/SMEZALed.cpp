#include "SMEZALed.h"

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
