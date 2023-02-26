#include <Arduino.h>
#include "SMEZACtrlGlobals.h"

#include "SMEZALabViewComm.h""
#include "stdint.h"


void setup() {
    // put your setup code here, to run once:
    CommInit();
}



void loop() {

    CommState();

}

