/*
 * ReadSensor.h
 *
 *  Created on: Dec 9, 2023
 *      Author: tantr
 */

#ifndef INC_READSENSOR_H_
#define INC_READSENSOR_H_

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

uint32_t leftSensor;
uint32_t rightSensor;
uint32_t frontSensor;

void delay_us (uint16_t us);
double kalman(double U);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
void HCSR04_Read (void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);


#endif /* INC_READSENSOR_H_ */
