#include <Arduino.h>
#include "SMEZAGlobals.h"

#include "SMEZALed.h"

#include "SMEZAMotorControl.h"

#include "SMEZAComm.h"
#include "stdint.h"


void setup() {
    // put your setup code here, to run once:
    readAddressSetting();
    CommInit();
    ledInit();
}



void loop() {

    CommState();
    
    tickMotorCtrl(&xMotor);
    xMotor.motorTick();
    tickMotorCtrl(&yMotor);
    yMotor.motorTick();

    ledTick();
}

