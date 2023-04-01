#include <Arduino.h>
#include <string.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(57600);

  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);

}

// number of ms between sending the provided message
#define SPAM_PERIOD 100

unsigned long nextTxTime = 0;

char usbRxBufferStr[81] = "";
char spamStr[81] = "";

char uartRxBufferStr[81] = "";
bool newUsbTxReady = false;

void loop() {
    // Send the stored string (spamStr) every [SPAM_PERIOD] ms
    if (millis()>=nextTxTime && digitalRead(2)){
        nextTxTime += SPAM_PERIOD;
        Serial1.write(spamStr);
        Serial1.flush();
    }

    // If there is new incoming string from the USB, store it in the USB buffer. 
    while (Serial.available()) {
        // get the new byte:
        char inChar = (char)Serial.read();
        strncat(usbRxBufferStr, &inChar, 1);
        // if the incoming character is a newline, stop receiving and 
        // update the spammed command:
        if (inChar == '\n') {
            strcpy(spamStr, usbRxBufferStr);
            strcpy(usbRxBufferStr, "");
            
        } 
    }
/*
    while (Serial1.available()) {
        // get the new byte:
        char inChar = (char)Serial1.read();
        strncat(uartRxBufferStr, &inChar, 1);
        // if the incoming character is a newline, stop receiving and update :
        if (inChar == '\n') {
            newUsbTxReady = true;
        } 
    }

    if(newUsbTxReady && !(Serial1.available())){
        newUsbTxReady = false;
        Serial1.write(uartRxBufferStr);
        Serial1.flush();
    }
  */

}