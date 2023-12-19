/*
 * ReadSensor.c
 *
 *  Created on: Dec 9, 2023
 *      Author: Tan Trinh
 */
#include "main.h"
#include "ReadSensor.h"
#include "stm32f1xx_hal.h"
#include "Control.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

uint32_t IC_Val1 = 0;
uint32_t IC_Val2 = 0;
uint32_t IC_Val3 = 0;
uint32_t IC_Val1_ = 0;
uint32_t IC_Val2_ = 0;
uint32_t IC_Val3_ = 0;
uint8_t Is_First_Captured_1 = 0;  // is the first value captured ?
uint8_t Is_First_Captured_2 = 0;  // is the first value captured ?
uint8_t Is_First_Captured_3 = 0;  // is the first value captured ?

uint32_t leftSensor;
uint32_t rightSensor;
uint32_t frontSensor;




uint32_t dif1 = 0;
uint32_t dif2 = 0;
uint32_t dif3 = 0;


#define TRIG_PIN GPIO_PIN_15
#define TRIG_PORT GPIOB






void delay_us (uint16_t us)
{
	__HAL_TIM_SET_COUNTER(&htim1,0);  // set the counter value a 0
	while (__HAL_TIM_GET_COUNTER(&htim1) < (us-1));  // wait for the counter to reach the us input in the parameter
}

double kalman(double U){
  static const double R = 40;
  static const double H = 1.00;
  static double Q = 10;
  static double P = 0;
  static double U_hat = 0;
  static double K = 0;
  K = P*H/(H*P*H+R);
  U_hat += + K*(U-H*U_hat);
  P = (1-K*H)*P+Q;
  return U_hat;
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{


	 if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1 )  // if the interrupt source is channel1
	{

		if (Is_First_Captured_1==0) // if the first value is not captured
		{
			IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); 	// read the first value
			Is_First_Captured_1 = 1;  // set the first captured as true
			// Now change the polarity to falling edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
		}

		else if (Is_First_Captured_1==1)   // if the first is already captured
		{
			IC_Val1_ = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);  // read second value

			leftSensor = (IC_Val1_ - IC_Val1)* .34/2;
			leftSensor = constrain(leftSensor, -500, 500);
			dif1 =(IC_Val1_ - IC_Val1);
			Is_First_Captured_1 = 0; // set it back to false

			// set polarity to rising edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(&htim1, TIM_IT_CC1);
		}
	}
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)  // if the interrupt source is channel1
	{
		if (Is_First_Captured_2==0) // if the first value is not captured
		{
			IC_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2); 	// read the first value
			Is_First_Captured_2 = 1;  // set the first captured as true
			// Now change the polarity to falling edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_FALLING);

		}

		else if (Is_First_Captured_2==1)   // if the first is already captured
		{
			IC_Val2_ = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);  // read second value

			frontSensor = (IC_Val2_ - IC_Val2) * .34/2;
			frontSensor = constrain(frontSensor, -500, 500);
			dif2 =(IC_Val2_ - IC_Val2);
			Is_First_Captured_2 = 0; // set it back to false

			// set polarity to rising edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(&htim1, TIM_IT_CC2);
		}

	}
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)  // if the interrupt source is channel1
	{
		if (Is_First_Captured_3==0) // if the first value is not captured
		{
			IC_Val3 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3); 	// read the first value
			Is_First_Captured_3 = 1;  // set the first captured as true
			// Now change the polarity to falling edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_FALLING);

		}

		else if (Is_First_Captured_3==1)   // if the first is already captured
		{
			IC_Val3_ = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);  // read second value

			rightSensor = (IC_Val3_ - IC_Val3) * .34/2;
			rightSensor = constrain(rightSensor, -500, 500);
			dif3 =(IC_Val3_ - IC_Val3);
			Is_First_Captured_3 = 0; // set it back to false

			// set polarity to rising edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(&htim1, TIM_IT_CC3);
		}

	}

}





void HCSR04_Read (void)
{
	HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_SET);  // pull the TRIG pin HIGH
	delay_us(10);  // wait for 10 us
	HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);  // pull the TRIG pin low
	__HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC1);
	__HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC2);
	__HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC3);
}


uint8_t preiodRead = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
 if(htim->Instance == htim2.Instance)
 {
	 if(preiodRead >= 5){
		 HCSR04_Read();
		 preiodRead = 0;
	 }
	 preiodRead++;

 }

}
