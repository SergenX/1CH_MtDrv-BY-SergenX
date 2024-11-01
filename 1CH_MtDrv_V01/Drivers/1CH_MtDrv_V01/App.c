/*
 * App.c
 *
 *  Created on: Oct 27, 2024
 *      Author: Sergen
 */

#include "App.h"
#include "../../Core/Inc/tim.h"
#include "../../Core/Inc/i2c.h"
#include "../../Core/Inc/dma.h"
#include "../../Core/Inc/gpio.h"
#include "../../Core/Inc/adc.h"
#include "../../Core/Inc/usart.h"
#include "../../Core/Inc/main.h"
#include <stdint.h>
#define MOTOR_360_MAX 1460 //360 Pulse
#define MOTOR_RPM 40
#define T_ADDR       0
#define I_ADDR       1
#define DEVICE_ADDR  2
#define MASTER_ID    3
uint8_t  PARAMETERS[4];
uint8_t RS485_RxData[8];
uint32_t ADC_PARAMS[3];
uint32_t TIMER;
volatile uint32_t ENCODER_PULSE;
uint32_t PREV_ENCODER_PULSE = 0;
uint32_t PREV_TIME;
uint32_t TASK;

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
void MEMORY_SET(){
	uint8_t PARAMS[4] = {85,60,64,01};
	HAL_I2C_Mem_Write(&hi2c1, 0x50 << 1, 0x10, I2C_MEMADD_SIZE_8BIT, PARAMS, 4, HAL_MAX_DELAY);
}

void System_Init(){
	HAL_I2C_Mem_Read(&hi2c1, 0x50 << 1, 0x10, I2C_MEMADD_SIZE_8BIT, PARAMETERS, 4, HAL_MAX_DELAY);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1); HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2); HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
	TIM1->CCR1 = 0;    TIM1->CCR2 = 0;
	HAL_ADCEx_Calibration_Start(&hadc);
	HAL_TIM_Encoder_Start_IT(&htim2,TIM_CHANNEL_ALL);
}

static inline void Motor_Control(uint8_t Rotation,uint8_t Acceleration , uint8_t Brake){
 if(Brake != 1){
	 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12,GPIO_PIN_RESET); //Release Brake
  switch(Rotation){
   case 1:
       TIM1->CCR2 = map(Acceleration,0,100,0,986);
   break;
   case 0:
	   TIM1->CCR2 = 0; TIM1->CCR1 = map(Acceleration,0,100,0,986);
   break;
  default:
	 TIM1->CCR1 = 0;  TIM1->CCR2 = 0;
   return;
  }
 }
 else{
	   TIM1->CCR1 = 0;  TIM1->CCR2 = 0;
	   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12,GPIO_PIN_SET);
	   return;
   }
}
union {
	uint16_t RPM;
	uint8_t  RPM_BUFF[2];
}RPM_C;

static inline void POWER_Control(){
    HAL_ADC_Start_DMA(&hadc,ADC_PARAMS,3); //Index : 0 - Current 1 - Voltage 2 - Temp
	float VBUS_CURRENT = ((float)ADC_PARAMS[0] * 60.0f) / 4095.0f;
	float VBUS_VOLTAGE = ((float)ADC_PARAMS[1] * 31.0f) / 4095.0f;
	//float TEMP         = ((float)ADC_PARAMS[2] * 85.0f) / 4095.0f;
	if(VBUS_CURRENT > PARAMETERS[I_ADDR] - 1){
		Motor_Control(0, 0, 0);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15,GPIO_PIN_SET);
	}
	if(VBUS_VOLTAGE < 14.0f){
	    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15,GPIO_PIN_SET);
	}
//  if(TEMP >= 85.0f - 1){ //MAX TEMP (1 DEGREES TOLERANCE)
//	  Motor_Control(M_RESET_VALUE,M_RESET_VALUE,M_RESET_VALUE);
//  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
    	HAL_UART_Receive_DMA(&huart1,RS485_RxData,8);
     if(RS485_RxData[0] == PARAMETERS[MASTER_ID] && RS485_RxData[1] == PARAMETERS[DEVICE_ADDR]){
  		 uint8_t  RS485_TxData[10] = {PARAMETERS[DEVICE_ADDR],PARAMETERS[MASTER_ID],RS485_RxData[3],RS485_RxData[4],map(ADC_PARAMS[0],0,4096,0,60),map(ADC_PARAMS[1],0,4096,0,31),RPM_C.RPM_BUFF[0],RPM_C.RPM_BUFF[1],0xFF,0xFF};
  		 uint8_t CRC8_TX = RS485_TxData[2] + RS485_TxData[3] + RS485_TxData[4] + RS485_TxData[5] %256;
  		 RS485_TxData[8] = CRC8_TX;
  		 HAL_UART_Transmit_IT(&huart1,RS485_TxData,10);
  		 uint8_t CRC8_RX = RS485_RxData[2] + RS485_RxData[3] + RS485_RxData[4] + RS485_RxData[5] %256;
        if(RS485_RxData[6] == CRC8_RX && RS485_RxData[7] == 0xFF) {
         Motor_Control(RS485_RxData[2],RS485_RxData[3],RS485_RxData[4]);
        }
        else if(RS485_RxData[6] == CRC8_RX && RS485_RxData[7] == 0xFF && RS485_RxData[4] == 0x01){
         Motor_Control(0,0,0);
        }
        else{
         Motor_Control(0,0,0);
        }
    }
 }
}

//void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
//	ENCODER_PULSE = __HAL_TIM_GET_COUNTER(htim);
//}
void MtDrv_Main(){
    System_Init();
    //MEMORY_SET();
	while(1){
		TIMER = HAL_GetTick();
		if(TIMER - PREV_TIME >= 1){
		       if(TASK++ >= 150){
		 		 POWER_Control(); //Current - Temp Control (Temp Closed)
		       }
			   if(TASK >= 1000){
				 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15,GPIO_PIN_RESET); HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_14);
				 ENCODER_PULSE = __HAL_TIM_GET_COUNTER(&htim2);
		            int32_t PULSE_DIFF = (int32_t)(ENCODER_PULSE - PREV_ENCODER_PULSE);
		            if (PULSE_DIFF < 0) {
		            	PULSE_DIFF += 0xFFFF; //Overflow Detecting
		            }
		            PREV_ENCODER_PULSE = ENCODER_PULSE;

		            //RPM = encoder time difference * 60 (full 1 minute pulse) / (1 tur 360 maximum pulse counter)
		            RPM_C.RPM = (PULSE_DIFF * 60) / 1460;
				 TASK = 0;
			   }
			   PREV_TIME = TIMER;
		}
	  HAL_UART_Receive_DMA(&huart1,RS485_RxData,8);
	}
}
