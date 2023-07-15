/*
*   FligtSimulatorSerialControllsProtocoll Arduino Library
*   by Dennis Gunia 07/2023
*/

#include "fsscp2.h"

FSSCP2::FSSCP2(uint16_t deviceTypeID){
    _deviceType = deviceTypeID;
    clearBuffer();

}
// sets deviceid in EEPROM
void FSSCP2::setDevUID(uint16_t devUID)
{
    EEPROM.put(EEPROM_DEVUID_DATA,devUID);
    EEPROM.write(EEPROM_DEVUID_CHECK, EEPROM_DEVUID_CHECK_BYTE);
}

// setup serial interface
void FSSCP2::initSerial()
{
    // check device UID
    _deviceID = getDevUID();
    // setup serial
    DDRD |=  (1<<PORTD1);  // set TX pin and pin 4 to output
    DDRD &= ~(1<<PORTD0);  // set RX pin to input 
    UBRR0H = (char) (SERUBBR>>8);
    UBRR0L = (char) SERUBBR & 0xFF;
    UCSR0A = 0;
    UCSR0B = 0;
    UCSR0C = 0;
    UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    unsigned char dummy;

    // disavle all interrupts and clear rx buffer
    cli();
    while (UCSR0A & (1<<RXC0)) dummy = UDR0;

    // send id and type on connect
    clearBuffer();
    appendBuffer((_deviceID >>8) & 0xFF);
    appendBuffer(_deviceID & 0xFF);
    appendBuffer((_deviceType >>8) & 0xFF);
    appendBuffer(_deviceType & 0xFF);
    sendBuffer();
    clearBuffer();
}

// gets deviceid from EEPROM and initializes EEPROM if not already done
// Default-ID is 0x0000
uint16_t FSSCP2::getDevUID()
{
    uint8_t eeprom_init = EEPROM.read(EEPROM_DEVUID_CHECK);
    if (eeprom_init != EEPROM_DEVUID_CHECK_BYTE){
        EEPROM.put(EEPROM_DEVUID_DATA, 0x0000);
        EEPROM.write(EEPROM_DEVUID_CHECK, EEPROM_DEVUID_CHECK_BYTE);
    }
    uint16_t eeprom_dev_id = 0x0000;
    EEPROM.get(0x0002, eeprom_dev_id);
    return eeprom_dev_id;
}

// this method must be called in the main loop as fast as possible
// it processes the incomming serial data
void FSSCP2::tick(){
    // check if data is available
    while (UCSR0A & _BV(RXC0)) {
        uint16_t _inByte = 0x0000 | UDR0; // UDR0;
        // byte two is payload length
        _data[_dataLoc] = _inByte;

        if (_dataLoc == 1) {
            _dataLen = _inByte + 1;
        }
        _dataLoc ++;
        _dataLen --;

        if (_dataLen <= 0){
            processMessage();
        }
        // input byte processed

    }
}

// register variable
void FSSCP2::registerVariable(uint8_t varId, uint8_t *var)
{
    _varPtr[varId] = var;
}

// send data to host without response
void FSSCP2::eventAsync(uint8_t *var[], uint8_t len)
{
    // send length
    WAIT_UART_TX;
    UDR0 = _dataLoc;
    // send data
    for (uint16_t i = 0; i < len; i++){
        WAIT_UART_TX;
        UDR0 = *(var + i);
    }
    WAIT_UART_TX;
}

// clear serial data buffer
void FSSCP2::clearBuffer(){
     
    for (uint8_t i = 0; i < CMD_LEN_MAX; i++){
        _data[i] = 0x00;
    }
    _dataLoc = 0;
    _dataLen = 2;
}

// process incomming message
void FSSCP2::processMessage(){
    uint8_t ix = 0;
    uint8_t cmd = _data[0];
    switch(cmd){
        case CMD_IDENT:
            clearBuffer();
            appendBuffer((_deviceID >>8) & 0xFF);
            appendBuffer(_deviceID & 0xFF);
            appendBuffer((_deviceType >>8) & 0xFF);
            appendBuffer(_deviceType & 0xFF);
            sendBuffer();   //send empty buffer
            break;
        case CMD_RESET:
            clearBuffer();
            sendBuffer(); 
            asm volatile ("  jmp 0");  
            break;
        case CMD_DATA_WR:
            ix = _data[2];
            *_varPtr[ix] = _data[3];
            clearBuffer();
            appendBuffer(*_varPtr[ix]);
            sendBuffer(); 
            break;
        case CMD_DATA_RD:
            ix = _data[2];
            clearBuffer();
            appendBuffer(ix);
            appendBuffer(*_varPtr[ix]);
            sendBuffer(); 
            break;
        case CMD_ID_WR:
            setDevUID(_data[3] | (_data[2] << 8));
            _deviceID = getDevUID();
            clearBuffer();
            appendBuffer((_deviceID >>8) & 0xFF);
            appendBuffer(_deviceID & 0xFF);
            sendBuffer();   //send empty buffer
            break;
        case 0xFE:  //echo for debug
            sendBuffer(); 
            break;
        default:
            sendError();
            break;
    }
    clearBuffer();
}

// transmitt buffer
void FSSCP2::sendBuffer(){
    // send length
    WAIT_UART_TX;
    UDR0 = _dataLoc;
    // send data
    for (uint16_t i = 0; i < _dataLoc; i++){
        WAIT_UART_TX;
        UDR0 = _data[i];
    }
    WAIT_UART_TX;
}

// send error code
void FSSCP2::sendError(){
    // send length
    WAIT_UART_TX;
    UDR0 = RE_ERR;
    WAIT_UART_TX;
}

// add byte to tx/rx buffer
void FSSCP2::appendBuffer(uint8_t data)
{
    _data[_dataLoc] = data;
    _dataLoc++;
}
