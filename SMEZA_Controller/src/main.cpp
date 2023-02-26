#include <Arduino.h>
#include "SMEZACtrlGlobals.h"

#include "SMEZALabViewComm.h"
#include "stdint.h"

void setup() {
    // put your setup code here, to run once:
    LabViewCommInit();

    // This should be in an init function but I'm too impatient rn for that 
    pinMode(13,OUTPUT);
    pinMode(12,OUTPUT);


}



void loop() {

    // This should probably be in a function or something
    if(powerIsOn){
        digitalWrite(13,HIGH);
        digitalWrite(12,HIGH);
    } else {
        digitalWrite(12, LOW);
        digitalWrite(13, LOW);
    }

    // Tick State Machines 
    LabViewCommState();
}

