/*
 * task.h
 *
 *  Created on: Oct 25, 2019
 *       Author: aji
 */

#ifndef TASK_H_
#define TASK_H_

void StartStop(void);
void Reset(void);
void ssr1(void);
void ssr2(void);
void ssr3(void);
void lcd(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#endif /* TASK_H_ */
