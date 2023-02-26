/* SMEZALabViewComm.h
 * Version 0.3
 * 2/8/2023
 * 
 * This is the header-file for SMEZALabViewComm.cpp. It defines the functions used for the 
 * SMEZA Communications Protocol.
 * 
 */

#ifndef SMEZALABVIEWCOMM_H 
#define SMEZALABVIEWCOMM_H

#define DEBUG_FEEDBACK 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "HardwareSerial.h"

#include "SMEZACtrlGlobals.h"

//#if(DEBUG_FEEDBACK != 0)
#include "Arduino.h"
//#endif

// Define Minimum command length. Any command strings shorter than this length are 
// assumed to be errors. 
#define MIN_COMMAND_LENGTH 7
#define MAX_ARGUMENTS 10 
#define MAX_ARGUMENT_LENGTH 15


/** @brief State variable for the communications bus. 
 * @param idle No communications being sent on bus. Default state. 
 * @param busyReceiving A transmission is currently being received. Transmission 
 * should be disabled. 
 * @param receivingComplete A transmission that was being received has finished and 
 * is ready to be parsed. 
 * @param busyTransmitting A transmission is being sent by this device. 
 * @param transmissionComplete Device has finished transmitting. 
 */
enum transmission_state
{
    idle,
    busyReceiving,
    receivingComplete,
    transmissionReady,
    transmissionComplete
};

uint8_t addressMatches(char cmdString[]);

int8_t matchKeyWord(char *match, const char *KNOWN_KEYS_PTR);

int8_t parseCommand(char *cmdStringPtr);

int8_t splitCommand(char *cmdStringPtr);

int8_t handleGet(char *paramString, char *responsePayload);

int8_t handleSet(char *paramString, char *valueString);

int8_t readRx();

#if(DEBUG_FEEDBACK != 0)
void dumpSplitter();
#endif

void LabViewCommInit();

void LabViewCommState();


#endif
// Full File