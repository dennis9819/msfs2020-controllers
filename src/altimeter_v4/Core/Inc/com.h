/*
 * com.h
 *
 *  Created on: 4 Aug 2021
 *      Author: dgunia
 */

#ifndef INC_COM_H_
#define INC_COM_H_

#include "stm32f1xx_hal.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "motor.h"

//message stack
struct MSG {
	uint8_t *txBuf;
	uint32_t *len;
};

uint8_t stack_max;
uint8_t stack_pointer;
uint8_t stack_top;
int stack[64];
void tx_stack_init();
void tx_stack_push(struct MSG *message);
void usb_data_rx(uint8_t* Buf, uint32_t *Len);
void tx_stack_process();
#endif /* INC_COM_H_ */
