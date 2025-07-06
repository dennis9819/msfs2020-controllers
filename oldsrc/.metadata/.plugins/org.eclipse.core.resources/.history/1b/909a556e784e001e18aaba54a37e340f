/*
 * instrument.c
 *
 *  Created on: 4 Aug 2021
 *      Author: dgunia
 */

#include "motor.h"

float 		INST_QNH_VAL = 28.0;
long int 	INST_ALT_VAL = 0;

long int	STEPS_100_CURR = 0;
long int	STEPS_010_CURR = 0;
long int	STEPS_001_CURR = 0;

const double step_calib_001 = 4.11632758333;
const double step_calib_010 = 0.411632758333;
const double step_calib_100 = 0.0411632758333;

inst_set_qnh(float val){
	INST_QNH_VAL = val;
}

inst_set_alt(long int val){
	INST_ALT_VAL = val;


	long int req_steps_001 = val * step_calib_001;
	long int req_steps_010 = val * step_calib_010;
	long int req_steps_100 = val * step_calib_100;

	moveStepper(0,req_steps_001-STEPS_001_CURR);
	moveStepper(2,(req_steps_010-STEPS_010_CURR) * (-1));
	moveStepper(3,(req_steps_100-STEPS_100_CURR) * (-1));

	STEPS_001_CURR = req_steps_001;
	STEPS_010_CURR = req_steps_010;
	STEPS_100_CURR = req_steps_100;

	printf("debug");
}
