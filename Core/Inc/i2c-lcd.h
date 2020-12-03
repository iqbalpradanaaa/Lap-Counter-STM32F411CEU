/*
 * i2c-lcd.h
 *
 *  Created on: Nov 14, 2020
 *      Author: Abstract
 */

#ifndef I2C_LCD_H_
#define I2C_LCD_H_

#include "stm32f4xx_hal.h"

void lcd_init (void);   // initialize lcd

void lcd_send_cmd (char cmd);  // send command to the lcd

void lcd_send_data (char data);  // send data to the lcd

void lcd_send_string (char *str);  // send string to the lcd

void lcd_clear (void);

#endif /* I2C_LCD_H_ */
