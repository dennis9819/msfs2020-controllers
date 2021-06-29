#include <Servo.h>
#include <EEPROM.h>

//DEFAULT DEVICE ID
int dev_id = 0x0000; 
const int dev_type = 0x00E1; // cannot be changed in EEPROM

//VARS FOR SERVO
Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards
double pos = 0;    // variable to store the servo position

//VARS FOR ENCODER
unsigned long lastInterruptTime = 0;
long steps = 0;
const int PinA = 2; // Used for generating interrupts using CLK signal
const int PinB = 3; // Used for reading DT signal

//VARS FOR SERIAL INTERFACE
String inputString = "";  
bool stringComplete = false; 


//ENCODER ISR
void isr ()  {
  unsigned long interruptTime = millis();
  if (interruptTime - lastInterruptTime > 1 ) {
    if (digitalRead(PinB) == LOW)
    {
      steps ++;
    }
    else {
      steps --;
    }

    if (steps > 2){
      steps = 0;
     Serial.println("UP"); 
    }
    if (steps < -2){
      steps = 0;
     Serial.println("DOWN"); 
    }
  }
  // Keep track of when we were here last (no more than every 5ms)
  lastInterruptTime = interruptTime;
}




void setup() {
  //SETUP IO
  Serial.begin(115200);
  
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  pinMode(PinA, INPUT_PULLUP);
  pinMode(PinB, INPUT_PULLUP);
  
  //INIT EEPROM
  int eeprom_init = EEPROM.read(0x0000);
  //detect if eprom is initialized
  if (eeprom_init != 0xE3){
    EEPROM.put(0x0002, dev_id);
    EEPROM.write(0x0000, 0xE3);
  }

  //get device id from eeprom
  int eeprom_dev_id = 0x0000;
  EEPROM.get(0x0002, eeprom_dev_id);
  dev_id = eeprom_dev_id;
 
  String conv_string = String(dev_id, HEX);   
  conv_string.toUpperCase();
  Serial.println("EV CONNECTED 0x" + appendLeading(conv_string,4));
   if (eeprom_dev_id == 0x0000){
    Serial.println("EV WARN ZERO_DEV_ID");
  }

  
  //ENABLE ENCODER INT
  attachInterrupt(digitalPinToInterrupt(PinA), isr, RISING);
  
  //INIT SERVO
  myservo.write(0);
  delay(1000); 
  myservo.write(16);

  //INT DONE
  Serial.println("EV INIT_DONE");
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

//MISC FUNCTIONS


void processFunc(String call){
  //echo
  String echo = call.substring(0,(call.length() - 1));
  Serial.println(echo);

  //process
  if (call.startsWith("S1 ")){
      String val = call.substring(3,(call.length() - 1));
      int val_int = map(val.toInt(),0,100,0,32);
      myservo.write(val_int);
      Serial.println("+RT OK");
      return;
  }

  if (call.startsWith("GS IDENT")){
      String conv_string = String(dev_id, HEX); 
      conv_string.toUpperCase();
      Serial.println("+0x" + appendLeading(conv_string,4));
      Serial.println("+RT OK");
      return;
  }

  if (call.startsWith("GS TYPE")){
      String conv_string = String(dev_type, HEX);   
      conv_string.toUpperCase();
      Serial.println("+0x" + appendLeading(conv_string,4));
      Serial.println("+RT OK");
      return;
  }

 if (call.startsWith("EV RESET")){
      Serial.println("+RT OK_RESET");
      delay(1000); 
      asm volatile ("  jmp 0");  
  }

   if (call.startsWith("SS IDENT")){
      String val = call.substring(9,(call.length() - 1));   
      int val_int = val.toInt();   
      EEPROM.put(0x0002, val_int);
      Serial.println("+RT OK_RESET");
      delay(1000); 
      asm volatile ("  jmp 0");  
  }

  Serial.println("-RT INVALID_COMMAND");
}

String appendLeading(String in, int len) {
  int toAdd = (len - in.length());
  String prefix = "";
  if (toAdd > 0){
    for (int i = 0 ; i < toAdd ; i++) {
      prefix = prefix + "0" ;
    }
  }
  return prefix + in;
}


void loop() {
   if (stringComplete) {
    processFunc(inputString);
    inputString = "";
    stringComplete = false;
  }
}
