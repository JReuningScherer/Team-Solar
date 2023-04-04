#include "SMEZAGlobals.h"

#define ADDRESS_PIN_14 A6
#define ADDRESS_PIN_58 A7
#define ADDRESS_PIN_9 7

int omnipresent = 0;
char DEVICE_ADDRESS[4] = "";
const char BROADCAST_ADDRESS[4] = "all";
char BILL[27] = "THE_SUN_IS_A_DEADLY_LAZER!";

uint16_t readDipGroup(uint8_t pinNumber){
    uint16_t rawAnalog = analogRead(pinNumber);
    const uint16_t dipStateAdcValues[16] = {0, 697, 327, 739, 152, 714, 401, 752, 55, 703, 353, 744, 192, 719, 422, 756};

    // Find the known ADC value that best matches the observed ADC reading
    uint16_t minDifference = 0xFFFF;
    uint16_t dipDecimal = 0;
    for (uint8_t i = 0; i < 16; ++i){
        uint16_t difference = abs(rawAnalog - dipStateAdcValues[i]);
        if (difference < minDifference)
        {
            minDifference = difference;
            dipDecimal = i;
        }
    }
    sprintf(DEVICE_ADDRESS, "%d ", dipDecimal);
    strcat(BILL, DEVICE_ADDRESS);
    return dipDecimal;
}

void readAddressSetting(){
    uint16_t devAddrInt = readDipGroup(ADDRESS_PIN_14);
    devAddrInt += (readDipGroup(ADDRESS_PIN_58) << 4);
    devAddrInt += (digitalRead(ADDRESS_PIN_9) << 8);


    sprintf(DEVICE_ADDRESS, "%03d", devAddrInt);
    if(atoi(DEVICE_ADDRESS) != (int)devAddrInt){
        readAddressSetting();
    }
}
