/*
*   FligtSimulatorSerialControllsProtocoll Arduino Library
*   by Dennis Gunia 07/2023
*/

#ifndef FSSCP2_h
#define FSSCP2_h

#include "Arduino.h"
#include <EEPROM.h>

// define serial params
#define F_CPU 16000000L
#define BAUD_RATE 19200
#define SERUBBR (F_CPU / 16 / BAUD_RATE) - 1
#define _BV(bit) (1<<(bit))
#define WAIT_UART_TX while( ! (UCSR0A & (1<<UDRE0)) )

// define EEPROM memory allocs
#define EEPROM_DEVUID_DATA 0x0002
#define EEPROM_DEVUID_CHECK 0x0000
// define check constant to verify that EEPROM data is already set
// at least once
#define EEPROM_DEVUID_CHECK_BYTE 0xE3

// define proto version
#define PROTOCOL_VERSION 0x02

// define commands
#define CMD_IDENT 0x01
#define CMD_RESET 0x3F
#define CMD_DATA_WR 0x02
#define CMD_DATA_RD 0x82
#define CMD_ID_WR 0x84

// define responses
#define RE_ERR 0xFF


// max message length (bytes)
#define CMD_LEN_MAX 255

// FligtSimulatorSerialControllsProtocoll V2 Class
class FSSCP2
{
    public:
        FSSCP2 (uint16_t deviceTypeID);
        void setDevUID(uint16_t devUID);
        void initSerial();
        uint16_t getDevUID();
        void tick();
        void registerVariable(uint8_t varId, uint8_t* var);
        void eventAsync(uint8_t* var[], uint8_t len);
    private:
       uint16_t _deviceType;
       uint16_t _deviceID; 
       uint8_t _data[CMD_LEN_MAX];
       uint16_t _dataLoc;
       uint16_t _dataLen;
       uint8_t* _varPtr[256];   //pointer to variable
       void processMessage();
       void clearBuffer();
       void sendBuffer();
       void sendError();
       void appendBuffer(uint8_t data);
};
#endif
