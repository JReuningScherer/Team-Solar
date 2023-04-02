// This file declares functions shared by both the adjuster and LabView
// communications systems 

#ifndef SMEZACTRLCOMM_H
#define SMEZACTRLCOMM_H

#define LABVIEW_DEBUG_FEEDBACK 0
#define ADJUSTER_DEBUG_FEEDBACK 2


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "HardwareSerial.h"

#include "SMEZACtrlGlobals.h"

#include "Arduino.h"

// Define Minimum command length parameters. 
#define MIN_COMMAND_LENGTH 7
#define MAX_COMMAND_LENGTH 81
#define MAX_ARGUMENTS 10 
#define MAX_ARGUMENT_LENGTH 15

// Define pin for tranceiver driver enable 
#define DRIVER_ENABLE_PIN 2

// Timeout in ms to wait for a response from an adjuster 
#define ADJUSTER_RESPONSE_TIMEOUT 20
// Timeout in ms for a response to complete. 
#define ADJUSTER_RESPONSE_LENGTH_TIMEOUT 20

// State machine enums
/** @brief State variable for the communications bus. 
 * @param lbvIdle No communications being sent on bus. Default state. 
 * @param lbvBusyReceiving A transmission is currently being received. Transmission 
 * should be disabled. 
 * @param lbvReceivingComplete A transmission that was being received has finished and 
 * is ready to be parsed. 
 * @param lbvTransmissionReady A transmission is ready to be sent and will be 
 * transmitted as soon as possible. 
 * @param lbvTransmissionComplete Device has finished transmitting. 
 */
enum labview_transmission_state
{
    lbvIdle,
    lbvBusyReceiving,
    lbvReceivingComplete,
    lbvTransmissionReady,
    lbvTransmissionComplete
};

extern labview_transmission_state labviewCommunicationState;

/** @brief State variable for the communications bus. 
 * @param adjIdle No communications being sent on bus. Default state. 
 * is ready to be parsed. 
 * @param adjTransmissionReady A transmission is ready to be sent and will be 
 * transmitted as soon as possible. 
 * @param awaitingResponse Device has finished transmitting and is waiting for a 
 * response. 
 * @param adjBusyReceiving A transmission is currently being received. Transmission 
 * should be disabled. 
 * @param adjReceivingComplete A transmission that was being received has finished and 
 */
enum adjuster_transmission_state
{
    adjIdle,
    adjTransmissionReady,
    adjAwaitingReponse,
    adjBusyReceiving,
    adjReceivingComplete
};

extern adjuster_transmission_state adjusterCommunicationState;

// Shared Functions
uint8_t addressMatches(char *cmdStringPtr);

int8_t matchKeyWord(char *match, const char *KNOWN_KEYS_PTR);

int8_t splitMessage(char *cmdStringPtr);

void dumpSplitter();

// Labview Commands
int8_t readLabviewRx();

int8_t parseCommand(char *cmdStringPtr);

int8_t handleGet(char *argString, char *responsePayload);

int8_t handleSet(char *paramString, char *valueString);

void LabViewCommInit();

void LabViewCommState();

// Adjuster Functions
int8_t readAdjusterRx();

int8_t parseResponse(char *respStringPtr);

void AdjCommInit();

void AdjusterCommState(void);

#endif
// End of File

