#include "fsscp2.h"

FSSCP2 fsscp(0x1234);

uint8_t test1 = 033;
uint8_t test2 = 0x42;

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  fsscp.initSerial();
  fsscp.registerVariable(0x00, &test1);
  fsscp.registerVariable(0x01, &test2);
}

void loop() {
  // put your main code here, to run repeatedly:
  fsscp.tick();
}
