#include "SMEZAGlobals.h"
#include <Arduino.h>
#include <stdint.h>

#ifndef SMEZALED_H
#define SMEZALED_H



#define LED_PIN 5

enum led_pattern
{
    powerIsOn,
    adjustmentInProgress,
    limitIsTripped
};

#define ADJ_IN_PROGRESS_PERIOD      500
#define ADJ_IN_PROGRESS_HIGHTIME    200
#define LIMIT_TRIPPED_PERIOD        300
#define LIMIT_TRIPPED_HIGHTIME      150

uint32_t nextLightEventTime = 0; 

led_pattern ledPatternState = powerIsOn;
led_pattern prevLedPatternState = powerIsOn;

void ledInit();

void ledTick();

#endif