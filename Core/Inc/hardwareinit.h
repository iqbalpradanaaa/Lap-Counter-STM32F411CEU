/*
 * hardwareinit.h
 *
 *  Created on: Nov 30, 2020
 *      Author: ACER
 */

#ifndef INC_HARDWAREINIT_H_
#define INC_HARDWAREINIT_H_


void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_I2C1_Init(void);
void MX_TIM3_Init(void);
void MX_TIM4_Init(uint32_t Periode);
void MX_USART1_UART_Init(void);

#endif /* INC_HARDWAREINIT_H_ */
