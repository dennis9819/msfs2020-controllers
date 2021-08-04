
#include <EEPROM.h>

//DEFAULT DEVICE ID
int dev_id = 0x0000; 
const int dev_type = 0x00E1; // cannot be changed in EEPROM


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

int last_a0 = 0;
int last_a1 = 0;
int last_a2 = 0;
int last_a3 = 0;
int last_a4 = 0;

const int numReadings = 10;

int readings_A0[numReadings];      // the readings from the analog input
int readIndex_A0 = 0;              // the index of the current reading
int total_A0 = 0;                  // the running total
int average_A0 = 0;                // the average

int readings_A1[numReadings];      // the readings from the analog input
int readIndex_A1 = 0;              // the index of the current reading
int total_A1 = 0;                  // the running total
int average_A1 = 0;                // the average

int readings_A2[numReadings];      // the readings from the analog input
int readIndex_A2 = 0;              // the index of the current reading
int total_A2 = 0;                  // the running total
int average_A2 = 0;                // the average

int readings_A3[numReadings];      // the readings from the analog input
int readIndex_A3 = 0;              // the index of the current reading
int total_A3 = 0;                  // the running total
int average_A3 = 0;                // the average

int readings_A4[numReadings];      // the readings from the analog input
int readIndex_A4 = 0;              // the index of the current reading
int total_A4 = 0;                  // the running total
int average_A4 = 0;                // the average

void processAnalog(){

  // subtract the last reading:
  total_A0 = total_A0 - readings_A0[readIndex_A0];
  // read from the sensor:
  readings_A0[readIndex_A0] = analogRead(A0);
  // add the reading to the total:
  total_A0 = total_A0 + readings_A0[readIndex_A0];
  // advance to the next position in the array:
  readIndex_A0 = readIndex_A0 + 1;

  // if we're at the end of the array...
  if (readIndex_A0 >= numReadings) {
    // ...wrap around to the beginning:
    readIndex_A0 = 0;
  }
  
  // calculate the average:
  average_A0 = total_A0 / numReadings;
  // send it to the computer as ASCII digits
  if (average_A0 != last_a0){
     last_a0 = average_A0;
     //Serial.println("A0 " + String(average_A0)); 
  }

  // subtract the last reading:
  total_A1 = total_A1 - readings_A1[readIndex_A1];
  // read from the sensor:
  readings_A1[readIndex_A1] = analogRead(A1);
  // add the reading to the total:
  total_A1 = total_A1 + readings_A1[readIndex_A1];
  // advance to the next position in the array:
  readIndex_A1 = readIndex_A1 + 1;

  // if we're at the end of the array...
  if (readIndex_A1 >= numReadings) {
    // ...wrap around to the beginning:
    readIndex_A1 = 0;
  }
  
  // calculate the average:
  average_A1 = total_A1 / numReadings;
  // send it to the computer as ASCII digits
  if (average_A1 != last_a1){
     last_a1 = average_A1;
     //Serial.println("A1 " + String(average_A1)); 
  }

  // subtract the last reading:
  total_A2 = total_A2 - readings_A2[readIndex_A2];
  // read from the sensor:
  readings_A2[readIndex_A2] = analogRead(A2);
  // add the reading to the total:
  total_A2 = total_A2 + readings_A2[readIndex_A2];
  // advance to the next position in the array:
  readIndex_A2 = readIndex_A2 + 1;

  // if we're at the end of the array...
  if (readIndex_A2 >= numReadings) {
    // ...wrap around to the beginning:
    readIndex_A2 = 0;
  }
  
  // calculate the average:
  average_A2 = total_A2 / numReadings;
  // send it to the computer as ASCII digits
  if (average_A2 != last_a2){
     last_a2 = average_A2;
     //Serial.println("A2 " + String(average_A2)); 
  }

  // subtract the last reading:
  total_A3 = total_A3 - readings_A3[readIndex_A3];
  // read from the sensor:
  readings_A3[readIndex_A3] = analogRead(A3);
  // add the reading to the total:
  total_A3 = total_A3 + readings_A3[readIndex_A3];
  // advance to the next position in the array:
  readIndex_A3 = readIndex_A3 + 1;

 // if we're at the end of the array...
  if (readIndex_A3 >= numReadings) {
    // ...wrap around to the beginning:
    readIndex_A3 = 0;
  }
  
 // calculate the average:
  average_A3 = total_A3 / numReadings;
  // send it to the computer as ASCII digits
  if (average_A3 != last_a3){
     last_a3 = average_A3;
     //Serial.println("A3 " + String(average_A3)); 
  }

  // subtract the last reading:
  total_A4 = total_A4 - readings_A4[readIndex_A4];
  // read from the sensor:
  readings_A4[readIndex_A4] = analogRead(A4);
  // add the reading to the total:
  total_A4 = total_A4 + readings_A4[readIndex_A4];
  // advance to the next position in the array:
  readIndex_A4 = readIndex_A4 + 1;

  // if we're at the end of the array...
  if (readIndex_A4 >= numReadings) {
    // ...wrap around to the beginning:
    readIndex_A4 = 0;
  }

  // calculate the average:
  average_A4 = total_A4 / numReadings;
  // send it to the computer as ASCII digits
  if (average_A4 != last_a4){
     last_a4 = average_A4;
     noInterrupts();
     Serial.println("A4 " + String(average_A4)); 
    interrupts();
  }
}

void loop() {
   if (stringComplete) {
    processFunc(inputString);
    inputString = "";
    stringComplete = false;
  }
  processAnalog();
}
