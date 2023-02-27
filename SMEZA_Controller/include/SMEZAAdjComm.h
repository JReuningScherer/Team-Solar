#ifndef SMEZACOMM_H 
#define SMEZACOMM_H

#define DEBUG_FEEDBACK 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "HardwareSerial.h"

#include "SMEZACtrlGlobals.h"


#if(DEBUG_FEEDBACK != 0)
#include "Arduino.h"
#endif

// Define Minimum command length. Any command strings shorter than this length are 
// assumed to be errors. 
#define MIN_COMMAND_LENGTH 7
#define MAX_COMMAND_LENGTH 256
#define MAX_ARGUMENTS 10 
#define MAX_ARGUMENT_LENGTH 15

#define DRIVER_ENABLE_PIN 2


/** @brief State variable for the communications bus. 
 * @param idle No communications being sent on bus. Default state. 
 * is ready to be parsed. 
 * @param transmissionReady A transmission is ready to be sent and will be 
 * transmitted as soon as possible. 
 * @param awaitingResponse Device has finished transmitting and is waiting for a 
 * response. 
 * @param busyReceiving A transmission is currently being received. Transmission 
 * should be disabled. 
 * @param receivingComplete A transmission that was being received has finished and 
 */
enum transmission_state
{
    idle,
    transmissionReady,
    awaitingReponse,
    busyReceiving,
    receivingComplete
};

uint8_t addressMatches(char cmdString[]);

int8_t matchKeyWord(char *match, const char *KNOWN_KEYS_PTR);

int8_t readRx();

#if(DEBUG_FEEDBACK != 0)
void dumpSplitter();
#endif

void AdjCommInit();

void AdjCommState();



#endif