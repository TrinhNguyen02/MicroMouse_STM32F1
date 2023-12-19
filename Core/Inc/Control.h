/*
 * control.h
 *
 *  Created on: Dec 9, 2023
 *      Author: tantr
 */

#ifndef INC_CONTROL_H_
#define INC_CONTROL_H_
#include "stm32f1xx_it.h"

extern TIM_HandleTypeDef htim2;

int constrain(int  value, int min, int max);
void leftMotor(int throttle);
void rightMotor(int throttle);


#endif /* INC_CONTROL_H_ */
