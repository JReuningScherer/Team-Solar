// This file defines functions shared by both the adjuster and LabView
// communications systems 

#include "SMEZACtrlComm.h"

/************************************/
/*      CONSTANTS AND GLOBALS       */
/************************************/

/**
 * @brief String containing all valid command keys, separated by spaces. Begins with 
 * a bell character to prevent matching of 0 length strings. 
 * @note "\a ping get set off on"
 */
const char *KNOWN_COMMANDS_PTR = "\a ping get set off on";
/**
 * @brief String containing all valid parameter keys, separated by spaces. Begins with 
 * a bell character to prevent matching of 0 length strings. 
 * @note "\a omnipresent address bill powerState"
 */
const char *KNOWN_PARAMETERS = "\a omnipresent address bill powerState";

// Global Variables for Command Processing
char cmdArgArr[MAX_ARGUMENTS+3][MAX_ARGUMENT_LENGTH+1];
char *recipAddrPtr = cmdArgArr[0];
char *sendrAddrPtr = cmdArgArr[1];
char *cmdWordPtr = cmdArgArr[2];

// Labview Globals 
char labviewRxBufferStr[MAX_COMMAND_LENGTH] = "";
char *labviewRxBuffer = labviewRxBufferStr;
char labviewResponseTxStr[MAX_COMMAND_LENGTH] = "";
char labviewPassToAdjusterStr[MAX_COMMAND_LENGTH] = "";
labview_transmission_state labviewCommunicationState = labview_transmission_state::lbvIdle;

// Adjuster Globals 
char adjusterRxBufferStr[MAX_COMMAND_LENGTH] = "";
char *adjusterRxBuffer = labviewRxBufferStr;
char adjusterCommandTxStr[MAX_COMMAND_LENGTH] = "";
char adjusterPassToLabviewStr[MAX_COMMAND_LENGTH] = "";
adjuster_transmission_state adjusterCommunicationState = adjuster_transmission_state::adjIdle;
uint32_t responseTimeoutValue = 0;

/************************************/
/*          SHARED FUNCTIONS        */
/************************************/


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
        return 2;
    }
    return 0;
}

/**
 * @brief checks for the presense of a keyword in a string of known keys. Returns 
 * the location of the keyword in the string. 
 * 
 * @param match The keyword to be matched 
 * @param KNOWN_KEYS_PTR A string containing all valid keys. The string should begin 
 * with a character not used in any valid keys (e.g. @, %) to distinguish no match 
 * from the first location. 
 * @return int8_t - Location of the keyword in the known keys string. Returns 0 
 * if no match is not found. 
 */
int8_t matchKeyWord(char *match, const char* KNOWN_KEYS_PTR){
    char* matchLocationPtr = strstr(KNOWN_KEYS_PTR,match);
    if(matchLocationPtr == NULL)
        return 0;
    return matchLocationPtr - KNOWN_KEYS_PTR;
}


/** @brief Splits a provided message string into addresses, and the arguments.

    @returns The splitMessage() function returns an integer, with the 
    value indicating number of arguments, excluding the addresses and the command
    keyword. Some values have special meaning. 
    @retval INT_MIN is returned for a command string with too many arguments. 
    @retval -1 indicates that the command keyword is missing
    @retval -2 indicates the sender address is missing
    */
int8_t splitMessage(char *cmdStringPtr){
    const char s[2] = " ";
    char *token;
    uint8_t nTokens = 0;

    /* get the first token */
    token = strtok(cmdStringPtr, s);

    /* walk through other tokens */
    while( token != NULL ) {
        #if(LABVIEW_DEBUG_FEEDBACK >= 3)
        Serial.print("\nTOKEN:");
        Serial.print(token);
        Serial.print("\tCNTR:");
        Serial.println(nTokens);
        dumpSplitter(); 
        #endif

        if(nTokens >= (MAX_ARGUMENTS+3))
            return INT8_MIN;

        if (nTokens == 0) {
            strcpy(cmdArgArr[nTokens], token);
        } else if (nTokens == 1) {
            strcpy(cmdArgArr[nTokens], token);
        } else {
            strcpy(cmdArgArr[nTokens], " ");
            strcat(cmdArgArr[nTokens], token);

        }

        token = strtok(NULL, s);
        nTokens++;
    }

    // Clear the rest of the cmdArgArr
    for (uint8_t i = nTokens; i < (MAX_ARGUMENTS + 3); ++i){
        strcpy(cmdArgArr[i], "");
        #if(LABVIEW_DEBUG_FEEDBACK >= 3)
        dumpSplitter(); 
        #endif

    }

    return nTokens - 3;

}



#if((LABVIEW_DEBUG_FEEDBACK != 0) || (ADJUSTER_DEBUG_FEEDBACK != 0))
void dumpSplitter(){
    Serial.println("PART\t\tPTR\tVALUE");

    Serial.print("recip:\t\t");
    Serial.print((int)(recipAddrPtr));
    Serial.print("\t");
    Serial.println(recipAddrPtr);

    Serial.print("sendr:\t\t");
    Serial.print((int)(sendrAddrPtr));
    Serial.print("\t");
    Serial.println(sendrAddrPtr);

    Serial.print("cmdWord:\t");
    Serial.print((int)(cmdWordPtr));
    Serial.print("\t");
    Serial.println(cmdWordPtr);

    for (uint8_t i = 0; i < (MAX_ARGUMENTS+3); ++i)
    {
        Serial.print("cmdArg");
        Serial.print(i);
        Serial.print(":\t");
        Serial.print((int)(cmdArgArr[i]));
        Serial.print("\t");
        Serial.println(cmdArgArr[i]);
    }
    


    return;
}
#endif



/************************************/
/*         LABVIEW FUNCTIONS        */
/************************************/


/**
 * @brief Reads USB serial connected to LabView
 * 
 * @return int8_t Always returns 0. Ff it doesn't then something is very wrong. 
 */
int8_t readLabviewRx(){
    while (Serial.available()) {
        // get the new byte:
        char inChar = (char)Serial.read();
        // if the incoming character is a newline, set a flag so the main loop can
        // do something about it:
        if (inChar == '\n') {
            labviewCommunicationState = lbvReceivingComplete;
        } else {
            // add it to recievedCommand:
            strncat(labviewRxBufferStr, &inChar, 1);
        }
    }
    return 0;
}


/** @brief Parses a provided command string into addresses and data values  

    @returns The parseCommand() function returns an unsigned integer, with the 
    value indicating the status of the parced function. 
    @retval 0 is returned for a successful parce where the address did not match.
    @retval 1 is returned for a successful parse where the address matched and the 
    command was successfully processed. 
    @retval -1 is returned for a command string that was less than the minimum 
    specified value.
 
    */
int8_t parseCommand(char *cmdStringPtr){

    // Check if command is longer than MIN_COMMAND_LENGTH. If not, return 1. 
    if(strlen(cmdStringPtr) < MIN_COMMAND_LENGTH){
        labviewCommunicationState = lbvIdle;
        return -1;
    }

    // Check if recipient address matches DEVICE_ADDRESS. If not, return 0.
    if(!addressMatches(cmdStringPtr)){
        labviewCommunicationState = lbvIdle;
        return 0;
    }
    
    // Split the command string into parts at each space
    int8_t numArguments = splitMessage(cmdStringPtr);
    #if(LABVIEW_DEBUG_FEEDBACK >= 2)
        Serial.println(numArguments);
        dumpSplitter();
    #endif



    // Create the response payload string
    char responsePayloadString[MAX_COMMAND_LENGTH-8] = "";
    char *responsePayload = responsePayloadString;

    switch (numArguments){
    case INT8_MIN:
        // The command string had too many arguments, but addresses are (probably) 
        // intact. Inform sender of the error. 
        strcpy(responsePayload, "Error: Too many arguments. Please try again.");
        break;
    case -2: 
        // The command string only contained the recipient address. Sender address
        // is unknown, so there is nothing we can do. Return 1.
        return 1;
    case -1: 
        // The command string is missing a command keyword. Inform sender of
        // the error. 
        strcpy(responsePayload, "Error: No command keyword found. Please try again.");
        break;
    default:
        #if(LABVIEW_DEBUG_FEEDBACK >= 1 )
        Serial.print("\ncmdWordID:");
        Serial.println(matchKeyWord(cmdWordPtr, KNOWN_COMMANDS_PTR));
        #endif

        // Determine which command was sent and call associated command handler
        switch (matchKeyWord(cmdWordPtr, KNOWN_COMMANDS_PTR)){
        // PING -1- send back PING response 
        case 1:
            sprintf(responsePayload, "I got your PING!");
            break;

        // GET -6- send back value requested 
        case 6:
            // If no parameters passed, inform sender and break
            if(!numArguments) {
                sprintf(responsePayload, "Error: No parameter keyword found. Please try again.");
                goto ASSEMBLE_RESPONSE;
            }
            for (uint8_t i = 3; i < (MAX_ARGUMENTS+3); ++i){
                if(strlen(cmdArgArr[i])) {
                    // if argument length is not 0, then call the get handler
                    if(handleGet(cmdArgArr[i],responsePayload)){
                        // if get error occurs, exit the loop immedately and inform sender
                        goto ASSEMBLE_RESPONSE;
                    }
                }
                    
            }
            break;
        // SET -10- set value, then send back updated value OR send permission error
        case 10:
            // If no parameters passed, inform sender and break
            if(!numArguments) {
                sprintf(responsePayload, "Error: No parameter keyword found. Please try again.");
                goto ASSEMBLE_RESPONSE;
            }
            for (uint8_t i = 3; i < (MAX_ARGUMENTS+3); i+=2){
                if(strlen(cmdArgArr[i])) {
                    // if argument length is not 0, then call the set handler
                    switch(handleSet(cmdArgArr[i], cmdArgArr[i+1])){
                    case 1:
                        sprintf(responsePayload, "Unknown Parameter:\"%s\"", cmdArgArr[i]);
                        goto ASSEMBLE_RESPONSE;
                    case -1:
                        sprintf(responsePayload, "Forbidden Parameter:\"%s\"", cmdArgArr[i]);
                        goto ASSEMBLE_RESPONSE;
                    default:
                        // if the set handler had no issues, respond with the updated values 
                        if(handleGet(cmdArgArr[i],responsePayload)){
                            // if get error occurs, exit the loop immedately and inform sender
                            goto ASSEMBLE_RESPONSE;
                        }
                        break;
                    }

                }
                
            }
            break;

        // ADJ -14- turn system power off 
        case 14:
            powerIsOn = 0;
            sprintf(responsePayload, "Power is turning off...");
            break; 

        // ADJ -17- turn system power on 
        case 18:
            powerIsOn = 1;
            sprintf(responsePayload, "Power is turning on...");
            break; 

        // default - send back unrecognized command error
        default:
            sprintf(responsePayload, "Unknown Command:\"%s\"",cmdWordPtr);
            break;
        } // end of match Command Keyword switch-case 

        break;
    } // end of numArguments switch-case




    // Responce string..... Assemble!  
    ASSEMBLE_RESPONSE: 
    char *responseStringPtr = labviewResponseTxStr;
    strcpy(responseStringPtr, sendrAddrPtr);
    strcat(responseStringPtr, " ");
    strcat(responseStringPtr, recipAddrPtr);
    strcat(responseStringPtr, " ");
    strcat(responseStringPtr, responsePayloadString);
    strcat(responseStringPtr, "\n");

    // Indicate that response string is ready for transmission
    labviewCommunicationState = lbvTransmissionReady;

    return 0;
}


int8_t handleGet(char *argString, char *responsePayload){
    
    // see if the argument string matches any of the things in KNOWN_PARAMETERS
    // temporary storage for the string responce
    char paramValue[246] = "";
    // use a switch-case statement to get the correct parameter value 
    switch (matchKeyWord(argString, KNOWN_PARAMETERS))
    {
    case 1: // omnipresent
        sprintf(paramValue, "%d ", omnipresent);
        break;
    case 13: // address
        sprintf(paramValue, "%s ", DEVICE_ADDRESS);
        break;
    case 21: //bill
        sprintf(paramValue, "%s ", BILL);
        break;
    case 26: //powerState
        sprintf(paramValue, "%d ", powerIsOn);
        break;
    default:
        sprintf(responsePayload, "Unknown Parameter:\"%s\"", argString);
        return 1;
    }

    // strcat the value to responsePayload
    strcat(responsePayload, paramValue);

    return 0;
}


int8_t handleSet(char *paramString, char *valueString){
    // Match against known parameters
    // switch case time
    // for those that are valid, update the value
    // for those that are forbidden, return -1
    // for those that are unknown, return 1
    switch (matchKeyWord(paramString, KNOWN_PARAMETERS))
    {
    case 1: // "omnipresent" - valid, int
        omnipresent = atoi(valueString);
        break;
    case 13: // "address" - forbidden
        return -1;
    case 21: // "bill" - forbidden
        return -1;
    case 26: // "powerState" - forbidden
        return -1;
    default:
        return 1;
    }

    return 0;
}


void LabViewCommInit(){
    Serial.begin(115200);
}


#if(LABVIEW_DEBUG_FEEDBACK >= 1)
labview_transmission_state prevState = lbvIdle;
#endif
void LabViewCommState(){
    #if(LABVIEW_DEBUG_FEEDBACK >= 1)
    if (labviewCommunicationState != prevState){
        Serial.print("CommunicationState: ");
        Serial.println(labviewCommunicationState);
    }
        prevState = labviewCommunicationState;

    #endif

    switch (labviewCommunicationState)
    {
    case labview_transmission_state::lbvIdle:
        if (Serial.available()){
            labviewCommunicationState = lbvBusyReceiving;
            readLabviewRx();
        }
        if(strlen(adjusterPassToLabviewStr) != 0){
            labviewCommunicationState = lbvTransmissionReady;
        }
        break;

    case labview_transmission_state::lbvBusyReceiving:
        if (Serial.available()){
            readLabviewRx();
        }
        break; 

    case labview_transmission_state::lbvReceivingComplete:

        #if(LABVIEW_DEBUG_FEEDBACK >= 1)
        Serial.print("RAW: ");
        Serial.println(labviewRxBufferStr);;
        #endif

        parseCommand(labviewRxBuffer);

        // clear the string:
        strcpy(labviewRxBufferStr, "");
        break;

    case labview_transmission_state::lbvTransmissionReady:
        // Check that there are no current transmissions going out
        
        // Transmit response
        Serial.write(labviewResponseTxStr);
        Serial.flush();

        #if(LABVIEW_DEBUG_FEEDBACK >= 1)
        //Serial.println(labviewResponseTxStr);
        Serial.println("---");
        Serial.flush();
        #endif

        labviewCommunicationState = lbvTransmissionComplete;
        
        break;

    case labview_transmission_state::lbvTransmissionComplete:
        labviewCommunicationState = lbvIdle;
        break;

    default:
        labviewCommunicationState = lbvIdle;
        break;
    }

}



/************************************/
/*         ADJUSTER FUNCTIONS       */
/************************************/


/**
 * @brief Reads USB serial connected to LabView
 * 
 * @return int8_t Always returns 0. Ff it doesn't then something is very wrong. 
 */
int8_t readAdjusterRx(){
    while (Serial1.available()) {
        // get the new byte:
        char inChar = (char)Serial1.read();
        // if the incoming character is a newline, set a flag so the main loop can
        // do something about it:
        if (inChar == '\n') {
            adjusterCommunicationState = adjReceivingComplete;
        } else {
            // add it to recievedCommand:
            strncat(adjusterRxBufferStr, &inChar, 1);
        }
    }
    return 0;
}


/** @brief Parses a provided command string from LabView. 

    @returns The parseCommand() function returns an unsigned integer, with the 
    value indicating the status of the parced function. 
    @retval 0 is returned for a successful parce where the address did not match.
    @retval 1 is returned for a successful parse where the address matched and the 
    command was successfully processed. 
    @retval -1 is returned for a command string that was less than the minimum 
    specified value.
 
    */
int8_t parseResponse(char *respStringPtr){

    // Check if command is longer than MIN_COMMAND_LENGTH. If not, return -1. 
    if(strlen(respStringPtr) < MIN_COMMAND_LENGTH){
        adjusterCommunicationState = adjIdle;
        return -1;
    }

    // Check if recipient address matches DEVICE_ADDRESS (ctr). If not, relay to 
    // the LabView interface 
    if(!addressMatches(respStringPtr)){
        adjusterCommunicationState = adjIdle;
        return 0;
    }

    /**
     * @todo add parsing of responses for internal commands 
     * 
     */
    return 0;
}


void AdjCommInit(){

    #if(ADJUSTER_DEBUG_FEEDBACK != 0)
    Serial.begin(115200);
    #endif

    Serial1.begin(9600, SERIAL_8N1);

    pinMode(DRIVER_ENABLE_PIN, OUTPUT);
}


void AdjusterCommState(void){
    switch (adjusterCommunicationState)
    {
    case adjuster_transmission_state::adjIdle:
        // DriverEnable = FALSE
        digitalWrite(DRIVER_ENABLE_PIN, LOW);

        if (Serial1.available()){
            readAdjusterRx();
            adjusterCommunicationState = adjBusyReceiving;
            break;
        }

        if (strlen(labviewPassToAdjusterStr) != 0){
            adjusterCommunicationState = adjTransmissionReady;
        }

        break;
    case adjuster_transmission_state::adjTransmissionReady:
/*         if(Serial1.available()){
            break;
        } */
        // DriverEnable = TRUE
        digitalWrite(DRIVER_ENABLE_PIN, HIGH);
        /* See if there is a labview passthrough command. If so, send it. If not, 
        send the 
         */
        if (strlen(adjusterPassToLabviewStr) != 0){
            Serial1.write(labviewPassToAdjusterStr);
            strcpy(labviewPassToAdjusterStr, "");
        } else {
            Serial1.write(adjusterCommandTxStr);
        }


        Serial1.flush();

        responseTimeoutValue = millis() + ADJUSTER_RESPONSE_TIMEOUT;

        digitalWrite(DRIVER_ENABLE_PIN, LOW);
        
        adjusterCommunicationState = adjAwaitingReponse;
        break;
    case adjuster_transmission_state::adjAwaitingReponse:
        // DriverEnable = FALSE
        digitalWrite(DRIVER_ENABLE_PIN, LOW);

        if (Serial1.available()){
            readAdjusterRx();
            adjusterCommunicationState = adjBusyReceiving;
        }

        if (millis() >= responseTimeoutValue){
            adjusterCommunicationState = adjIdle;
        }
        break;
    case adjuster_transmission_state::adjBusyReceiving:
        // DriverEnable = FALSE
        digitalWrite(DRIVER_ENABLE_PIN, LOW);

        if (Serial1.available()){
            readAdjusterRx();
        }
        break;
    case adjuster_transmission_state::adjReceivingComplete:
        // DriverEnable = FALSE
        digitalWrite(DRIVER_ENABLE_PIN, LOW);

        // TO DO - Implement Processing of commands

        adjusterCommunicationState = adjIdle;

        break;
    
    default:
        adjusterCommunicationState = adjuster_transmission_state::adjIdle;
        break;
    }
}

