#include <EEPROM.h>
//DEFAULT DEVICE ID
int dev_id = 0x0000; 
const int dev_type = 100; // cannot be changed in EEPROM
//VARS FOR SERIAL INTERFACE
String inputString = "";  
bool stringComplete = false; 
#define CH_0 0b00111111
#define CH_1 0b00000110
#define CH_2 0b01011011
#define CH_3 0b01001111
#define CH_4 0b01100110
#define CH_5 0b01101101
#define CH_6 0b01111101
#define CH_7 0b00000111
#define CH_8 0b01111111
#define CH_9 0b01100111
#define CH_N 0b00000000
#define CH_S 0b01000000

uint8_t segChar[12] = {CH_0,CH_1,CH_2,CH_3,CH_4,CH_5,CH_6,CH_7,CH_8,CH_9,CH_S,CH_N};

uint8_t segData[32] = {CH_0,CH_1,CH_2,CH_3,CH_4,CH_5,CH_6,CH_7,
CH_8,CH_9,CH_S,CH_S,CH_S,CH_S,CH_S,CH_S,
CH_S,CH_S,CH_S,CH_S,CH_S,CH_S,CH_S,CH_S,
CH_S,CH_S,CH_S,CH_S,CH_S,CH_S,CH_S,CH_S};

uint8_t multiplexState = 0;

//BYTE5 is CATHODE

#define PIN_SDATA 3
#define PIN_SCLCK 4
#define PIN_LATCH 5
#define PIN_RESET 6
#define PIN_BUTTON 7

#define ENC1A 8
#define ENC1B 9
#define ENC2A 10
#define ENC2B 11
#define ENC3A A0
#define ENC3B A1
#define ENC4A A2
#define ENC4B A3

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

void setup() {
  Serial.begin(115200);
  pinMode(PIN_SDATA,OUTPUT);
  pinMode(PIN_SCLCK,OUTPUT);
  pinMode(PIN_LATCH,OUTPUT);
  pinMode(PIN_RESET,OUTPUT);
  pinMode(PIN_BUTTON,INPUT);
  pinMode(ENC1A,INPUT_PULLUP);
  pinMode(ENC1B,INPUT_PULLUP);
  pinMode(ENC2A,INPUT_PULLUP);
  pinMode(ENC2B,INPUT_PULLUP);
  pinMode(ENC3A,INPUT_PULLUP);
  pinMode(ENC3B,INPUT_PULLUP);
  pinMode(ENC4A,INPUT_PULLUP);
  pinMode(ENC4B,INPUT_PULLUP);
  digitalWrite(PIN_RESET,LOW);
  digitalWrite(PIN_RESET,HIGH);
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

  Serial.println("EV INIT_DONE");
}

int testCounter = 0;
int testOffset = 0;


int com1_ac_high = 118;
int com1_ac_low = 0;

int com1_stby_high = 118;
int com1_stby_low = 0;

int com2_ac_high = 118;
int com2_ac_low = 0;

int com2_stby_high = 118;
int com2_stby_low = 0;

uint8_t btn1_last = 0;
uint8_t btn2_last = 0;
uint8_t btn3_last = 0;
uint8_t btn4_last = 0;

uint8_t btn1_time = 0;
uint8_t btn2_time = 0;
uint8_t btn3_time = 0;
uint8_t btn4_time = 0;

uint8_t enc1a_last = 0;
uint8_t enc2a_last = 0;
uint8_t enc1b_last = 0;
uint8_t enc2b_last = 0;
uint8_t enc3a_last = 0;
uint8_t enc4a_last = 0;
uint8_t enc3b_last = 0;
uint8_t enc4b_last = 0;


void renderFreq(int high, int low, int offset){
  segData[offset] = 0x00;
  segData[offset+1] = segChar[(high / 100 % 10)];
  segData[offset+2] = segChar[(high / 10 % 10)];
  segData[offset+3] = segChar[(high  % 10)] | 0b10000000;
  segData[offset+4] = segChar[(low / 100 % 10)];
  segData[offset+5] = segChar[(low / 10 % 10)];
  segData[offset+6] = segChar[(low % 10)];
  segData[offset+7] = 0x00;
}

void test(){
  renderFreq(com1_ac_high,com1_ac_low,24);
  renderFreq(com1_stby_high,com1_stby_low,16);
  renderFreq(com2_ac_high,com2_ac_low,8);
  renderFreq(com2_stby_high,com2_stby_low,0);
}

void multiPlex (){

    uint8_t segBuffer [5] = {0x00,0x00,0x00,0x00,0x00};
    // get current data
    
    segBuffer[0] = segData[multiplexState];
    segBuffer[1] = segData[multiplexState + 8];
    segBuffer[2] = segData[multiplexState + 16];
    segBuffer[3] = segData[multiplexState + 24];
    segBuffer[4] = (1<<multiplexState);
    multiplexState ++;
    if (multiplexState >= 8){
      multiplexState = 0;
    }
    
    //Serial.println(multiplexState);
    // shift out
    shiftOut(PIN_SDATA,PIN_SCLCK,segBuffer[0]);
    shiftOut(PIN_SDATA,PIN_SCLCK,segBuffer[1]);
    shiftOut(PIN_SDATA,PIN_SCLCK,segBuffer[2]);
    shiftOut(PIN_SDATA,PIN_SCLCK,segBuffer[3]);
    shiftOut(PIN_SDATA,PIN_SCLCK,segBuffer[4]);
    digitalWrite(PIN_LATCH,HIGH);
    digitalWrite(PIN_LATCH,LOW);
    digitalWrite(PIN_RESET,LOW);
    digitalWrite(PIN_RESET,HIGH);
    if (segBuffer[4] == 0b00000001){
      int data = digitalRead(PIN_BUTTON) ;
      if(data != btn2_last){
        btn2_last = data;
        if (data == 1){
          int com1_ac_high_tmp = com1_ac_high;
          int com1_ac_low_tmp = com1_ac_low;
  
          com1_ac_high = com1_stby_high;
          com1_ac_low = com1_stby_low;
          
          com1_stby_high = com1_ac_high_tmp;
          com1_stby_low = com1_ac_low_tmp;
          Serial.println("EV F1SL " + String(com1_stby_low, DEC));
          Serial.println("EV F1SH " + String(com1_stby_high, DEC));
          Serial.println("EV F1AL " + String(com1_ac_low, DEC));
          Serial.println("EV F1AH " + String(com1_ac_high, DEC));
        }
      }
    }else if (segBuffer[4] == 0b00000010){
      int data = digitalRead(PIN_BUTTON) ;
      if(data != btn1_last){
        btn1_last = data;
        if (data == 1){
          int com2_ac_high_tmp = com2_ac_high;
          int com2_ac_low_tmp = com2_ac_low;
  
          com2_ac_high = com2_stby_high;
          com2_ac_low = com2_stby_low;
          
          com2_stby_high = com2_ac_high_tmp;
          com2_stby_low = com2_ac_low_tmp;
          Serial.println("EV F2SL " + String(com2_stby_low, DEC));
          Serial.println("EV F2SH " + String(com2_stby_high, DEC));
          Serial.println("EV F2AL " + String(com2_ac_low, DEC));
          Serial.println("EV F2AH " + String(com2_ac_high, DEC));
        }
      }
    }
}

// the heart of the program
void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  // This shifts 8 bits out MSB first,
  //on the rising edge of the clock,
  //clock idles low
  //internal function setup
  int i=0;
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);
  //clear everything out just in case to
  //prepare shift register for bit shifting
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);
  //for each bit in the byte myDataOut&#xFFFD;
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights.
  for (i=7; i>=0; i--)  {
  digitalWrite(myClockPin, 0);
  //if the value passed to myDataOut and a bitmask result
  // true then... so if we are at i=6 and our value is
  // %11010100 it would the code compares it to %01000000
  // and proceeds to set pinState to 1.
  if ( myDataOut & (1<<i) ) {
    pinState= 1;
  }
  else {
    pinState= 0;
  }
  //Sets the pin to HIGH or LOW depending on pinState
  digitalWrite(myDataPin, pinState);
  //register shifts bits on upstroke of clock pin
  digitalWrite(myClockPin, 1);
  //zero the data pin after shift to prevent bleed through
  digitalWrite(myDataPin, 0);
  }
  //stop shifting
  digitalWrite(myClockPin, 0);
}

void readEncoder(){
  int data = digitalRead(ENC1A) ;
    if(data != enc1a_last){
      enc1a_last = data;
        if (data == digitalRead(ENC1B)){
          com1_stby_low -= 50;
        }else{
          com1_stby_low += 50;
        }
        if (com1_stby_low >= 1000){com1_stby_low -= 1000;}
        if (com1_stby_low < 0){com1_stby_low += 1000;}
        Serial.println("EV F1SL " + String(com1_stby_low, DEC));
      
  }
  data = digitalRead(ENC2A) ;
    if(data != enc2a_last){
      enc2a_last = data;
        if (data == digitalRead(ENC2B)){
          com1_stby_high -= 1;
        }else{
          com1_stby_high += 1;
        }
        if (com1_stby_high >= 1000){com1_stby_high -= 1000;}
        if (com1_stby_high < 0){com1_stby_high += 1000;}
        Serial.println("EV F1SH " + String(com1_stby_high, DEC));
  }

  data = digitalRead(ENC3A) ;
    if(data != enc3a_last){
      enc3a_last = data;
        if (data == digitalRead(ENC3B)){
          com2_stby_low -= 50;
        }else{
          com2_stby_low += 50;
        }
        if (com2_stby_low >= 1000){com2_stby_low -= 1000;}
        if (com2_stby_low < 0){com2_stby_low += 1000;}
        Serial.println("EV F2SL " + String(com2_stby_low, DEC));
      
  }
  data = digitalRead(ENC4A) ;
    if(data != enc4a_last){
      enc4a_last = data;
        if (data == digitalRead(ENC4B)){
          com2_stby_high -= 1;
        }else{
          com2_stby_high += 1;
        }
        if (com2_stby_high >= 1000){com2_stby_high -= 1000;}
        if (com2_stby_high < 0){com2_stby_high += 1000;}
        Serial.println("EV F2SH " + String(com2_stby_high, DEC));
      
  }
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

void loop() {
  // put your main code here, to run repeatedly:
  test();
  multiPlex();
  testCounter ++;
  readEncoder();
   if (stringComplete) {
    processFunc(inputString);
    inputString = "";
    stringComplete = false;
  }
}
