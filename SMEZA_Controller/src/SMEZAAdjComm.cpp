
#include "SMEZAAdjComm.h"

char cmdArgArr[MAX_ARGUMENTS+3][MAX_ARGUMENT_LENGTH+1];
char *recipAddrPtr = cmdArgArr[0];
char *sendrAddrPtr = cmdArgArr[1];
char *cmdWordPtr = cmdArgArr[2];

transmission_state adjCommunicationState = idle;

char rxBufferStr[256] = "";
char *rxBuffer = rxBufferStr;
char txBufferStr[256] = "";




/** @fn uint8_t addressMatches(char cmdString[])
    @brief Determines if the recipient address of the command string matches the 
    address of the unit. Recipient addresses are case sensitive and must be 3 
    characters long. 

    @returns The addressMatches() function returns an unsigned integer, with the 
    value indicating whether the recipient address matches. 0 is returned if the 
    address does NOT match. 1 is returned if the address matches. 
    */
uint8_t addressMatches(char *cmdStringPtr){
    if (!strncmp(DEVICE_ADDRESS,cmdStringPtr,3)){
        return 1;
    }
    if (!strncmp(BROADCAST_ADDRESS,cmdStringPtr,3)){
        return 1;
    }
    return 0;
}


int8_t matchKeyWord(char *match, const char* KNOWN_KEYS_PTR){
    char* matchLocationPtr = strstr(KNOWN_KEYS_PTR,match);
    if(matchLocationPtr == NULL)
        return 0;
    return matchLocationPtr - KNOWN_KEYS_PTR;
}


/** @brief Parses a provided command string. 

    @returns The parseCommand() function returns an unsigned integer, with the 
    value indicating the status of the parced function. 
    @retval 0 is returned for a successful parce where the address did not match.
    @retval 1 is returned for a successful parse where the address matched and the 
    command was successfully processed. 
    @retval -1 is returned for a command string that was less than the minimum 
    specified value.
 
    */
int8_t parseCommand(char *cmdStringPtr){

    // Check if command is longer than MIN_COMMAND_LENGTH. If not, return -1. 
    if(strlen(cmdStringPtr) < MIN_COMMAND_LENGTH){
        adjCommunicationState = idle;
        return -1;
    }

    // Check if recipient address matches DEVICE_ADDRESS (ctr). If not, relay to 
    // the LabView interface 
    if(!addressMatches(cmdStringPtr)){
        adjCommunicationState = idle;
        return 0;
    }

    /**
     * @todo add parsing of responses for internal commands 
     * 
     */

}


int8_t readRx(){
    while (Serial.available()) {
        // get the new byte:
        char inChar = (char)Serial.read();
        // if the incoming character is a newline, set a flag so the main loop can
        // do something about it:
        if (inChar == '\n') {
            adjCommunicationState = receivingComplete;
        } else {
            // add it to recievedCommand:
            strncat(rxBufferStr, &inChar, 1);
        }
    }
    return 0;
}






void AdjCommInit(){

    #if(DEBUG_FEEDBACK != 0)
    Serial.begin(115200);
    #endif

    Serial1.begin(9600, SERIAL_8N1);

    pinMode(DRIVER_ENABLE_PIN, OUTPUT);
}




