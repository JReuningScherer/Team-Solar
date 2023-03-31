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
    if (incomingMsg.equals("ON")){
      powerIsOn = true;
    } else if (incomingMsg.equals("OFF")) {
      powerIsOn = false;
    } else if (incomingMsg.equals("ESTOP")) {
      powerIsOn = false;
    }
  }
   
  if(powerIsOn){
    digitalWrite(13,HIGH);
    digitalWrite(12,HIGH);
  } else {
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
  }
  
  

}
