void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);

}

bool powerIsOn = false;

void loop() {
  // put your main code here, to run repeatedly:

  "";
  if (Serial.available()){
    String incomingMsg = Serial.readStringUntil('\n');
    delay(10);
    Serial.println(incomingMsg);
  }
  
  

}
