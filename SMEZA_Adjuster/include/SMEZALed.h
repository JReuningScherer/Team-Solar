#include <Arduino.h>
#include <stdint.h>
#include "SMEZAMotorControl.h"

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
#define LIMIT_TRIPPED_PERIOD        200
#define LIMIT_TRIPPED_HIGHTIME      100

void ledInit();

void ledTick();

#endif