/*
 * com.c
 *
 *  Created on: 4 Aug 2021
 *      Author: dgunia
 */

#include "com.h"
#include "instrument.h"

const uint16_t device_type = 0x0006;
const uint16_t device_id = 0x0000;
uint8_t last_com_state = 0;


//UNUSED
void heartbeatISR(){
	if (host_com_port_open != last_com_state){
		last_com_state = host_com_port_open;
		if (host_com_port_open == 0){
			//closed
		}else if (host_com_port_open == 1){
			uint8_t txBuf[] = "-HELLO\n";
			CDC_Transmit_FS((uint8_t*)txBuf, strlen(txBuf));
		}
	}
	uint8_t txBuf[] = "-ALIVE\n";
	CDC_Transmit_FS((uint8_t*)txBuf, strlen(txBuf));
}

void protoInit(){

	uint8_t *txBuf = malloc(32);
	sprintf(txBuf,"EV CONNECTED 0x%04x\n",device_type);
	CDC_Transmit_FS((uint8_t*)txBuf, strlen(txBuf));
	memset(&txBuf[0], 0, sizeof(txBuf));

	//init steppers
	homeAll();


}

void protoSetAlt(uint8_t* Buf, uint32_t *Len){
	uint32_t length = *Len -7;
	char number[length];
	long int val;
	memcpy( number, &Buf[6], length );
	sscanf(number, "%d", &val);
	inst_set_alt(val);

	//uint8_t txBuf[] = "+RT OK";
	//CDC_Transmit_FS((uint8_t*)txBuf, strlen(txBuf));
}




char RESP_INIT[]	= "INIT\n";
char RESP_ALT[]	= "S ALT";
char RESP_IDENT[]	= "GS IDENT\n";
char RESP_TYPE[]	= "GS TYPE\n";

void usb_data_rx(uint8_t* Buf, uint32_t *Len){
	if (strcmp((const char *)Buf, RESP_INIT) == 0)
	{
		protoInit();
		memset(&Buf[0], 0, *Len);
		return;
	}
	if (strcmp((const char *)Buf, RESP_IDENT) == 0)
	{
		uint8_t *txBuf = malloc(32);
		sprintf(txBuf,"+0x%04x\n+RT OK\n",0);
		CDC_Transmit_FS((uint8_t*)txBuf, strlen(txBuf));
		memset(&Buf[0], 0, *Len);
		return;
	}
	if (strcmp((const char *)Buf, RESP_TYPE) == 0)
	{
		uint8_t *txBuf = malloc(32);
		sprintf(txBuf,"+0x%04x\n+RT OK\n",device_type);
		CDC_Transmit_FS((uint8_t*)txBuf, strlen(txBuf));
		memset(&Buf[0], 0, *Len);
		return;
	}
	if (strncmp((const char *)Buf, RESP_ALT,5) == 0)
	{
		protoSetAlt(Buf,Len);
		memset(&Buf[0], 0, *Len);
		return;
	}
	uint8_t txBuf[] = "-RT INVALID_COMMAND";
	memset(&Buf[0], 0, *Len);
	CDC_Transmit_FS((uint8_t*)txBuf, strlen(txBuf));
}
