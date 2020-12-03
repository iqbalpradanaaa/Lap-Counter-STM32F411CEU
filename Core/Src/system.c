/*
 * system.c
 *
 *  Created on: Nov 30, 2020
 *      Author: ACER
 */
#include "stm32f4xx_hal.h"
#include "main.h"
#include "task.h"
#include "i2c-lcd.h"
#include "hardwareinit.h"
#include "system.h"

TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart1;

void system (void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_TIM3_Init();
  MX_TIM4_Init(2);
  MX_USART1_UART_Init();

  lcd_init();
  lcd_put_cur(1,3);
  lcd_send_string ("-LAP COUNTING-");
  HAL_Delay(1000);
  lcd_put_cur(2, 1);
  lcd_send_string("3D4EA | 1110181004");
  HAL_Delay(3000);
  lcd_clear ();
  HAL_TIM_Base_Start_IT(&htim4);
  HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rProject UTS Sistem Embedded 2020",35,10);
  HAL_UART_Transmit(&huart1,(uint8_t*)"\n\r---------Lap Counting-----------",35,10);
  HAL_UART_Transmit(&huart1,(uint8_t*)"\n\r------3D4EA---1110181004--------",35,10);
}
