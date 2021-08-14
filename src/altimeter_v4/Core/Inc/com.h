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

void usb_data_rx(uint8_t* Buf, uint32_t *Len);

#endif /* INC_COM_H_ */
