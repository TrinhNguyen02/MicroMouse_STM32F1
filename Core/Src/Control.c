/*
 * control.c
 *
 *  Created on: Dec 9, 2023
 *      Author: Tan Trinh
 */


#include "main.h"
#include "Control.h"
#include "stm32f1xx_hal.h"

extern TIM_HandleTypeDef htim2;


int constrain(int value, int min, int max) {
    if (value <= min) {
        return min;
    } else if (value >= max) {
        return max;
    } else {
        return value;
    }
}

void leftMotor(int throttle){
	constrain(throttle, -100, 100);
	throttle=0.90*throttle;

	// channel B of tb6612 --> tim2 channel 2
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, SET);
	if (throttle >= 0 ){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, RESET);
	    __HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,throttle*10);
	}
	else if (throttle < 0 ){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, SET);
	    __HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,throttle*(-10));
	}

}

void rightMotor(int throttle){
	constrain(throttle, -100, 100);
	// channel A of tb6612 --> tim2 channel 1
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, SET);
	if (throttle >= 0 ){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, RESET);
	    __HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_1,throttle*10);
	}
	else if (throttle < 0 ){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, SET);
	    __HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_1,throttle*(-10));
	}
}

