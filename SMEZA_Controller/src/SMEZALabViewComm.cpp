/* SMEZAComm.cpp 
 * Version 0.3
 * 2/8/2023
 * 
 * This is the file which defined the functions for the SMEZA Communications 
 * Protocol. 
 * 
*/

#include "SMEZALabViewComm.h"

char cmdArgArr[MAX_ARGUMENTS+3][MAX_ARGUMENT_LENGTH+1];
char *recipAddrPtr = cmdArgArr[0];
char *sendrAddrPtr = cmdArgArr[1];
char *cmdWordPtr = cmdArgArr[2];

transmission_state labViewCommunicationState = idle;

char rxBufferStr[MAX_COMMAND_LENGTH] = "";
char *rxBuffer = rxBufferStr;
char txBufferStr[MAX_COMMAND_LENGTH] = "";

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

    // Check if command is longer than MIN_COMMAND_LENGTH. If not, return 1. 
    if(strlen(cmdStringPtr) < MIN_COMMAND_LENGTH){
        labViewCommunicationState = idle;
        return -1;
    }

    // Check if recipient address matches DEVICE_ADDRESS. If not, return 0.
    if(!addressMatches(cmdStringPtr)){
        labViewCommunicationState = idle;
        return 0;
    }
    
    // Split the command string into parts at each space
    int8_t numArguments = splitCommand(cmdStringPtr);
    #if(DEBUG_FEEDBACK >= 2)
        Serial.println(numArguments);
        dumpSplitter();
    #endif



    // Create the response payload string
    char responsePayloadString[248] = "";
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
        #if(DEBUG_FEEDBACK >= 1 )
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
    char *responseStringPtr = txBufferStr;
    strcpy(responseStringPtr, sendrAddrPtr);
    strcat(responseStringPtr, " ");
    strcat(responseStringPtr, recipAddrPtr);
    strcat(responseStringPtr, " ");
    strcat(responseStringPtr, responsePayloadString);
    strcat(responseStringPtr, "\n");

    // Indicate that response string is ready for transmission
    labViewCommunicationState = transmissionReady;

    return 0;
}


/** @brief Splits a provided command string into addresses, the command keyword, 
    and the arguments.

    @returns The splitCommand() function returns an integer, with the 
    value indicating number of arguments, excluding the addresses and the command
    keyword. Some values have special meaning. 
    @retval INT_MIN is returned for a command string with too many arguments. 
    @retval -1 indicates that the command keyword is missing
    @retval -2 indicates the sender address is missing
    */
int8_t splitCommand(char *cmdStringPtr){
    const char s[2] = " ";
    char *token;
    uint8_t nTokens = 0;

    /* get the first token */
    token = strtok(cmdStringPtr, s);

    /* walk through other tokens */
    while( token != NULL ) {
        #if(DEBUG_FEEDBACK >= 3)
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
        #if(DEBUG_FEEDBACK >= 3)
        dumpSplitter(); 
        #endif

    }

    return nTokens - 3;

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

int8_t readRx(){
    while (Serial.available()) {
        // get the new byte:
        char inChar = (char)Serial.read();
        // if the incoming character is a newline, set a flag so the main loop can
        // do something about it:
        if (inChar == '\n') {
            labViewCommunicationState = receivingComplete;
        } else {
            // add it to recievedCommand:
            strncat(rxBufferStr, &inChar, 1);
        }
    }
    return 0;
}





void LabViewCommInit(){
    Serial.begin(115200);
}



#if(DEBUG_FEEDBACK >= 1)
transmission_state prevState = idle;
#endif
void LabViewCommState(){
    #if(DEBUG_FEEDBACK >= 1)
    if (labViewCommunicationState != prevState){
        Serial.print("CommunicationState: ");
        Serial.println(labViewCommunicationState);
    }
        prevState = labViewCommunicationState;

    #endif

    switch (labViewCommunicationState){
    case idle:
        if (Serial.available()){
            labViewCommunicationState = busyReceiving;
            readRx();
        }
        break;

    case busyReceiving:
        if (Serial.available()){
            readRx();
        }
        break; 

    case receivingComplete:

        #if(DEBUG_FEEDBACK >= 1)
        Serial.print("RAW: ");
        Serial.println(rxBufferStr);;
        #endif

        parseCommand(rxBuffer);

        // clear the string:
        strcpy(rxBufferStr, "");
        break;

    case transmissionReady:
        // Check that there are no current transmissions going out
        
        // Transmit response
        Serial.write(txBufferStr);
        Serial.flush();

        #if(DEBUG_FEEDBACK >= 1)
        Serial.println(txBufferStr);
        Serial.println("---");
        Serial.flush();
        #endif

        labViewCommunicationState = transmissionComplete;
        
        break;

    case transmissionComplete:
        labViewCommunicationState = idle;
        break;

    default:
        labViewCommunicationState = idle;
        break;
    }

     
}


#if(DEBUG_FEEDBACK != 0)
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