/*
 * motor.c
 *
 *  Created on: 3 Aug 2021
 *      Author: dgunia
 */
#include "motor.h"
#include "com.h"

int stepper_1_change = 0;
int stepper_2_change = 0;
int stepper_3_change = 0;
int stepper_4_change = 0;

int stepper_1_state = 0;
int stepper_2_state = 0;
int stepper_3_state = 0;
int stepper_4_state = 0;


int dly_step = 10;

int homing = 0;

void moveStepper(int id, int steps) {
	switch(id){
		case 0: stepper_1_change += steps; break;
		case 1: stepper_2_change += steps; break;
		case 2: stepper_3_change += steps; break;
		case 3: stepper_4_change += steps; break;
	}
}

void stepperServiceRoutine(){
	//HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
	if (homing > 0){
		homeISR();
	}else{
		doStepISR();
	}

}

void doStepISR(){
	if (stepper_1_change > 0){
		doStep(M1_A1_GPIO_Port, M1_A1_Pin, M1_A2_Pin, M1_B1_Pin, M1_B2_Pin, stepper_1_state);
		stepper_1_change --;
		stepper_1_state ++;
		if (stepper_1_state == 8){stepper_1_state = 0;}
	}
	if (stepper_1_change < 0){
		doStep(M1_A1_GPIO_Port, M1_A1_Pin, M1_A2_Pin, M1_B1_Pin, M1_B2_Pin, stepper_1_state);
		stepper_1_change ++;
		stepper_1_state --;
		if (stepper_1_state < 0){stepper_1_state = 7;}
	}

	if (stepper_2_change > 0){
		doStep(M2_A1_GPIO_Port, M2_A1_Pin, M2_A2_Pin, M2_B1_Pin, M2_B2_Pin, stepper_2_state);
		stepper_2_change --;
		stepper_2_state ++;
		if (stepper_2_state == 8){stepper_2_state = 0;}
	}
	if (stepper_2_change < 0){
		doStep(M2_A1_GPIO_Port, M2_A1_Pin, M2_A2_Pin, M2_B1_Pin, M2_B2_Pin, stepper_2_state);
		stepper_2_change ++;
		stepper_2_state --;
		if (stepper_2_state < 0){stepper_2_state = 7;}
	}

	if (stepper_3_change > 0){
		doStep(M3_A1_GPIO_Port, M3_A1_Pin,M3_A2_Pin, M3_B1_Pin, M3_B2_Pin, stepper_3_state);
		stepper_3_change --;
		stepper_3_state ++;
		if (stepper_3_state == 8){stepper_3_state = 0;}
	}
	if (stepper_3_change < 0){
		doStep(M3_A1_GPIO_Port, M3_A1_Pin, M3_A2_Pin, M3_B1_Pin, M3_B2_Pin, stepper_3_state);
		stepper_3_change ++;
		stepper_3_state --;
		if (stepper_3_state < 0){stepper_3_state = 7;}
	}

	if (stepper_4_change > 0){
		doStep(M4_A1_GPIO_Port, M4_A1_Pin, M4_A2_Pin, M4_B1_Pin, M4_B2_Pin, stepper_4_state);
		stepper_4_change --;
		stepper_4_state ++;
		if (stepper_4_state == 8){stepper_4_state = 0;}
	}
	if (stepper_4_change < 0){
		doStep(M4_A1_GPIO_Port, M4_A1_Pin, M4_A2_Pin, M4_B1_Pin, M4_B2_Pin, stepper_4_state);
		stepper_4_change ++;
		stepper_4_state --;
		if (stepper_4_state < 0){stepper_4_state = 7;}
	}

}

void homeISR(){
	//stepper1 100
	if (homing == 1){
		if (stepper_1_change == 0){
			printf("home s1");
			if(HAL_GPIO_ReadPin(HAL_1_GPIO_Port, HAL_1_Pin)){
				doStep(M1_A1_GPIO_Port, M1_A1_Pin, M1_A2_Pin, M1_B1_Pin, M1_B2_Pin, stepper_1_state);
				stepper_1_state --;
				if (stepper_1_state < 0){stepper_1_state = 7;}
			}else{
				homing ++;
				moveStepper(0,-160);
			}
		}else{
			doStepISR();

		}
	}
	if (homing == 2){
		//stepper1
		if (stepper_1_change == 0){
			moveStepper(1,500);
			homing ++;
		}else{
			doStepISR();

		}
	}

	//stepper2 QNH
	if (homing == 3){
		if (stepper_2_change == 0){
			printf("home s2");
			if(HAL_GPIO_ReadPin(HAL_2_GPIO_Port, HAL_2_Pin)){
				doStep(M2_A1_GPIO_Port, M2_A1_Pin, M2_A2_Pin, M2_B1_Pin, M2_B2_Pin, stepper_2_state);
				stepper_2_state --;
				if (stepper_2_state < 0){stepper_2_state = 7;}
			}else{
				homing ++;
				moveStepper(1,-5450);
			}
		}else{
			doStepISR();

		}
	}
	if (homing == 4){
		//stepper2
		if (stepper_2_change == 0){
			moveStepper(2,500);
			homing ++;
		}else{
			doStepISR();

		}
	}

	//stepper3 1k
	if (homing == 5){
		if (stepper_3_change == 0){
			printf("home s3");
			if(HAL_GPIO_ReadPin(HAL_3_GPIO_Port, HAL_3_Pin)){
				doStep(M3_A1_GPIO_Port, M3_A1_Pin,M3_A2_Pin, M3_B1_Pin, M3_B2_Pin, stepper_3_state);
				stepper_3_state --;
				if (stepper_3_state < 0){stepper_3_state = 7;}
			}else{
				homing ++;
				moveStepper(2,0);
			}
		}else{
			doStepISR();

		}
	}
	if (homing == 6){
		//stepper2
		if (stepper_3_change == 0){
			moveStepper(3,-500);
			homing ++;
		}else{
			doStepISR();

		}
	}
	//stepper4 10k
	if (homing == 7){
		if (stepper_4_change == 0){
			printf("home s3");
			if(HAL_GPIO_ReadPin(HAL_4_GPIO_Port, HAL_4_Pin)){
				doStep(M4_A1_GPIO_Port, M4_A1_Pin, M4_A2_Pin, M4_B1_Pin, M4_B2_Pin, stepper_4_state);
				stepper_4_state ++;
				if (stepper_4_state == 8){stepper_4_state = 0;}
			}else{
				homing ++;
				moveStepper(3,200);
			}
		}else{
			doStepISR();

		}
	}
	if (homing == 8){
		//stepper2
		if (stepper_4_change == 0){
			uint8_t txBuf[] = "+INIT\n";
			struct MSG msg;
			msg.txBuf = txBuf;
			msg.len = strlen(txBuf);
			tx_stack_push(&msg);
			homing = 0;
		}else{
			doStepISR();

		}
	}
}



void homeAll(){
	moveStepper(0,500);
	//stepper_2_change = 600;
	//stepper_3_change = 600;
	//stepper_4_change = 600;
	homing = 1;
	//while(homing > 0){
		//wait
	//}
}

void doStep(GPIO_TypeDef *PORT, uint16_t  PIN1, uint16_t  PIN2, uint16_t  PIN3, uint16_t  PIN4, int step){
	switch(step){
		case 0:
			HAL_GPIO_WritePin(PORT, PIN1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(PORT, PIN2, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(PORT, PIN3, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(PORT, PIN4, GPIO_PIN_RESET);
			break;
		case 1:
			HAL_GPIO_WritePin(PORT, PIN1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(PORT, PIN2, GPIO_PIN_SET);
			HAL_GPIO_WritePin(PORT, PIN3, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(PORT, PIN4, GPIO_PIN_RESET);
			break;
		case 2:
			HAL_GPIO_WritePin(PORT, PIN1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(PORT, PIN2, GPIO_PIN_SET);
			HAL_GPIO_WritePin(PORT, PIN3, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(PORT, PIN4, GPIO_PIN_RESET);
			break;
		case 3:
			HAL_GPIO_WritePin(PORT, PIN1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(PORT, PIN2, GPIO_PIN_SET);
			HAL_GPIO_WritePin(PORT, PIN3, GPIO_PIN_SET);
			HAL_GPIO_WritePin(PORT, PIN4, GPIO_PIN_RESET);
			break;
		case 4:
			HAL_GPIO_WritePin(PORT, PIN1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(PORT, PIN2, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(PORT, PIN3, GPIO_PIN_SET);
			HAL_GPIO_WritePin(PORT, PIN4, GPIO_PIN_RESET);
			break;
		case 5:
			HAL_GPIO_WritePin(PORT, PIN1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(PORT, PIN2, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(PORT, PIN3, GPIO_PIN_SET);
			HAL_GPIO_WritePin(PORT, PIN4, GPIO_PIN_SET);
			break;
		case 6:
			HAL_GPIO_WritePin(PORT, PIN1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(PORT, PIN2, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(PORT, PIN3, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(PORT, PIN4, GPIO_PIN_SET);
			break;
		case 7:
			HAL_GPIO_WritePin(PORT, PIN1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(PORT, PIN2, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(PORT, PIN3, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(PORT, PIN4, GPIO_PIN_SET);
			break;
		}
}
