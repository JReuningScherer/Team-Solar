#include <Arduino.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int rawAnalog = analogRead(A6);

  int dipValues[16] = {0, 55, 152, 192, 327, 353, 401, 422, 697, 703, 714, 719, 739, 744, 752, 756};

  int minDifference = 10000;
  int dipDecimal = 0;
  for (int i = 0; i < 16; ++i){
      int difference = abs(rawAnalog - dipValues[i]);
      if (difference < minDifference)
      {
          minDifference = difference;
          dipDecimal = i;
    }

  }

  Serial.print("RAW: ");
  Serial.print(rawAnalog);
  Serial.print("\tDECIMAL: ");
  Serial.print(dipDecimal);
  Serial.print("\n");
  Serial.flush();
  delay(100);
}



