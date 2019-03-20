#ifndef MOTOR_H
#define MOTOR_H

#include "stm32f0xx_hal.h"
#include "gpio.h"
#include "adc.h"

#define MOTOR_F_LOW      HAL_GPIO_WritePin(GPIOC, MOTOR_F_Pin, GPIO_PIN_RESET);
#define MOTOR_F_HIGH     HAL_GPIO_WritePin(GPIOC, MOTOR_F_Pin, GPIO_PIN_SET);

#define MOTOR_B_LOW    HAL_GPIO_WritePin(GPIOC, MOTOR_B_Pin, GPIO_PIN_RESET);
#define MOTOR_B_HIGH   HAL_GPIO_WritePin(GPIOC, MOTOR_B_Pin, GPIO_PIN_SET);


#define MOTOR_ELECURDELAY      200
#define MOTOR_ELECUROVER       100        /////��ת������С
#define MOTOR_ELECUROVERCOUNT  5
#define MOTOR_MOTORFORCELIMIT  2625000    /////������ӳ�
#define MOTOR_RETURNHALFCOUNT  600        ////����ӳ�



void MotorBackward(void);
void MotorForward(void);
void MotorStop(void);
void DoorClose(void);
void DoorOpen(void);
#endif
