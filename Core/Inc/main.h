#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

void Error_Handler(void);

#define LED1_Pin GPIO_PIN_3
#define LED1_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_4
#define LED2_GPIO_Port GPIOA
#define LED3_Pin GPIO_PIN_5
#define LED3_GPIO_Port GPIOA
#define LED4_Pin GPIO_PIN_6
#define LED4_GPIO_Port GPIOA
#define LED5_Pin GPIO_PIN_7
#define LED5_GPIO_Port GPIOA
#define SSR1_Pin GPIO_PIN_12
#define SSR1_GPIO_Port GPIOB
#define SSR2_Pin GPIO_PIN_13
#define SSR2_GPIO_Port GPIOB
#define SSR3_Pin GPIO_PIN_14
#define SSR3_GPIO_Port GPIOB
#define RST_Pin GPIO_PIN_15
#define RST_GPIO_Port GPIOB
#define SS_Pin GPIO_PIN_8
#define SS_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
