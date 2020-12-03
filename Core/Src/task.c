/*
 * task.c
 *
 *  Created on: oct 25, 2019
 *
 */
#include "main.h"
#include "stm32f4xx_hal.h"
#include "i2c-lcd.h"
#include "task.h"
#include "stdio.h"
#include "hardwareinit.h"

I2C_HandleTypeDef hi2c1;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
UART_HandleTypeDef huart1;

char buffer[17],buffs[13];
unsigned int runstop=0;
unsigned int ms=0,dd=0,mm=0,ulang=0;

unsigned int lapA=0,lapB=0,lapC=0,totlap=4;
int ms1,dd1,mm1,mslast1=0,ddlast1=0,mmlast1=0;
int ms2,dd2,mm2,mslast2=0,ddlast2=0,mmlast2=0;
int ms3,dd3,mm3,mslast3=0,ddlast3=0,mmlast3=0;

unsigned char bouncing	=0xFF;
unsigned char bouncing2	=0xFF;
unsigned char bouncing3	=0xFF;
unsigned char bouncing4	=0xFF;
unsigned char bouncing5	=0xFF;

uint32_t Timeout_loop 	= 0;
uint32_t Timeout_value 	= 2000;
uint32_t Timeout_loop2 	= 0;
uint32_t Timeout_value2 = 2000;
uint32_t Timeout_loop3 	= 0;
uint32_t Timeout_value3 = 2000;
uint32_t Timeout_loop4 	= 0;
uint32_t Timeout_value4 = 2000;
uint32_t Timeout_loop5 	= 0;
uint32_t Timeout_value5 = 2000;


void StartStop(void)
{
	if(HAL_GPIO_ReadPin(SS_GPIO_Port, SS_Pin)== GPIO_PIN_RESET && (Timeout_loop++<=Timeout_value)){
		bouncing=(bouncing<<1);
	}

	else if(HAL_GPIO_ReadPin(SS_GPIO_Port, SS_Pin)== GPIO_PIN_RESET && (Timeout_loop++>Timeout_value)){
		if(Timeout_loop>=Timeout_value+300){
			Timeout_loop=Timeout_value+1;
			HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rErr: StartStop_Button ",25,10);
		}
	}

	else if(HAL_GPIO_ReadPin(SS_GPIO_Port, SS_Pin)== GPIO_PIN_SET && Timeout_loop<=Timeout_value){
		bouncing = (bouncing<<1)|1;
	}
	else{
		Timeout_loop=0;
		bouncing=0xFF;
	}

	if (bouncing==0x01){
		if(runstop==0){
			HAL_TIM_Base_Start_IT(&htim3);
			runstop=1;
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\r--------Start Counting----------",35,10);
		}
		else{
			HAL_TIM_Base_Stop_IT(&htim3);
			runstop=0;
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\r---------Stop Counting----------",35,10);
		}

		Timeout_loop=0;
		bouncing=0xFF;
	}
}

void Reset(void)
{
	if(HAL_GPIO_ReadPin(RST_GPIO_Port,RST_Pin)== GPIO_PIN_RESET && (Timeout_loop2++<=Timeout_value2)){
		bouncing2=(bouncing2<<1);

	}

	else if(HAL_GPIO_ReadPin(RST_GPIO_Port,RST_Pin)== GPIO_PIN_RESET && (Timeout_loop2++>Timeout_value2)){
		if(Timeout_loop2>=Timeout_value2+300){
			Timeout_loop2=Timeout_value2+1;
			HAL_GPIO_TogglePin(LED2_GPIO_Port,LED2_Pin);
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rErr: Reset_Button ",23,10);
		}
	}

	else if(HAL_GPIO_ReadPin(RST_GPIO_Port,RST_Pin)== GPIO_PIN_SET && Timeout_loop2<=Timeout_value2){
		bouncing2 = (bouncing2<<1)|1;
	}

	else{
		Timeout_loop2=0;
		bouncing2=0xFF;
	}

	if (bouncing2==0x01){
		HAL_UART_Transmit(&huart1,(uint8_t*)"\n\r------------Reset---------------",35,10);

		HAL_TIM_Base_Stop_IT(&htim3);

		ms=0;	dd=0;	mm=0;
		ms1=0;	dd1=0;	mm1=0;
		ms2=0;	dd2=0;	mm2=0;
		ms3=0;	dd3=0;	mm3=0;

		runstop=0;
		lapA=0;	lapB=0;	lapC=0;

		mmlast1=0; ddlast1=0; mslast1=0;
		mmlast2=0; ddlast2=0; mslast2=0;
		mmlast3=0; ddlast3=0; mslast3=0;

		Timeout_loop2=0;
		bouncing2=0xFF;
	}
}

void ssr1(void)
{
	if(HAL_GPIO_ReadPin(SSR1_GPIO_Port,SSR1_Pin)== GPIO_PIN_RESET && (Timeout_loop3++<=Timeout_value3)){
		bouncing3=(bouncing3<<1);

	}

	else if(HAL_GPIO_ReadPin(SSR1_GPIO_Port,SSR1_Pin)== GPIO_PIN_RESET && (Timeout_loop3++>Timeout_value3)){
		if(Timeout_loop3>=Timeout_value3+300){
			Timeout_loop3=Timeout_value3+1;
			HAL_GPIO_TogglePin(LED3_GPIO_Port,LED3_Pin);
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rErr: Sensor 1 ",18,10);
		}
	}

	else if(HAL_GPIO_ReadPin(SSR1_GPIO_Port,SSR1_Pin)== GPIO_PIN_SET && Timeout_loop3<=Timeout_value3){
		bouncing3 = (bouncing3<<1)|1;
	}

	else{
		Timeout_loop3=0;
		bouncing3=0xFF;
	}

	if (bouncing3==0x01){

		mm1=(mm-mmlast1);
		dd1=(dd-ddlast1);
		ms1=(ms-mslast1);

		if(ms1<0){
			dd1=dd1-1;
			ms1=ms1+60;
		}
		if(dd1<0){
			mm1=mm1-1;
			dd1=dd1+60;
		}

		if(mm1<0) mm1=0;
		if(dd1<0) dd1=0;
		if(ms1<0) ms1=0;

		mmlast1=mm;
		ddlast1=dd;
		mslast1=ms;

		if(runstop==1)	lapA++;
		else 			lapA=0;

		if(lapA<totlap){
			sprintf(buffs,"%d = %d%d:%d%d:%d%d",lapA,(mm1/10),(mm1%10),(dd1/10),(dd1%10),(ms1/10),(ms1%10));
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rTrack A Lap ",14,10);
			HAL_UART_Transmit(&huart1,(uint8_t*)buffs,sizeof(buffs),10);
		}
		else if (lapA==totlap)
		{
			lapA=totlap;
			sprintf(buffs,"%d = %d%d:%d%d:%d%d",lapA,(mm1/10),(mm1%10),(dd1/10),(dd1%10),(ms1/10),(ms1%10));
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rTrack A Lap ",14,10);
			HAL_UART_Transmit(&huart1,(uint8_t*)buffs,sizeof(buffs),10);
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\r-------Track A FINISH-----------",35,10);
		}
		else HAL_UART_Transmit(&huart1,(uint8_t*)"\n\r-------Track A FINISH-----------",35,10);

		Timeout_loop3=0;
		bouncing3=0xFF;

	}
}

void ssr2(void)
{
	if(HAL_GPIO_ReadPin(SSR2_GPIO_Port,SSR2_Pin)== GPIO_PIN_RESET && (Timeout_loop4++<=Timeout_value4)){
		bouncing4=(bouncing4<<1);

	}

	else if(HAL_GPIO_ReadPin(SSR2_GPIO_Port,SSR2_Pin)== GPIO_PIN_RESET && (Timeout_loop4++>Timeout_value4)){
		if(Timeout_loop4>=Timeout_value4+300){
			Timeout_loop4=Timeout_value4+1;
			HAL_GPIO_TogglePin(LED4_GPIO_Port,LED4_Pin);
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rErr: Sensor 2 ",18,10);
		}
	}

	else if(HAL_GPIO_ReadPin(SSR2_GPIO_Port,SSR2_Pin)== GPIO_PIN_SET && Timeout_loop4<=Timeout_value4){
		bouncing4 = (bouncing4<<1)|1;
	}

	else{
		Timeout_loop4=0;
		bouncing4=0xFF;
	}

	if (bouncing4==0x01){

		mm2=(mm-mmlast2);
		dd2=(dd-ddlast2);
		ms2=(ms-mslast2);

		if(ms2<0){
			dd2=dd2-1;
			ms2=ms2+60;
		}
		if(dd2<0){
			mm2=mm2-1;
			dd2=dd2+60;
		}

		if(mm2<0) mm2=0;
		if(dd2<0) dd2=0;
		if(ms2<0) ms2=0;

		mmlast2=mm;
		ddlast2=dd;
		mslast2=ms;

		if(runstop==1)	lapB++;
		else 			lapB=0;

		if(lapB<totlap){
			sprintf(buffs,"%d = %d%d:%d%d:%d%d",lapB,(mm2/10),(mm2%10),(dd2/10),(dd2%10),(ms2/10),(ms2%10));
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rTrack B Lap:",14,10);
			HAL_UART_Transmit(&huart1,(uint8_t*)buffs,sizeof(buffs),10);
		}
		else if (lapB==totlap)
		{
			lapB=totlap;
			sprintf(buffs,"%d = %d%d:%d%d:%d%d",lapB,(mm2/10),(mm2%10),(dd2/10),(dd2%10),(ms2/10),(ms2%10));
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rTrack B Lap:",14,10);
			HAL_UART_Transmit(&huart1,(uint8_t*)buffs,sizeof(buffs),10);
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\r-------Track B FINISH-----------",35,10);
		}
		else HAL_UART_Transmit(&huart1,(uint8_t*)"\n\r-------Track B FINISH-----------",35,10);

		Timeout_loop4=0;
		bouncing4=0xFF;
	}
}

void ssr3(void)
{
	if(HAL_GPIO_ReadPin(SSR3_GPIO_Port,SSR3_Pin)== GPIO_PIN_RESET && (Timeout_loop5++<=Timeout_value5)){
		bouncing5=(bouncing5<<1);

	}

	else if(HAL_GPIO_ReadPin(SSR3_GPIO_Port,SSR3_Pin)== GPIO_PIN_RESET && (Timeout_loop5++>Timeout_value5)){
		if(Timeout_loop5>=Timeout_value5+300){
			Timeout_loop5=Timeout_value5+1;
			HAL_GPIO_TogglePin(LED5_GPIO_Port,LED5_Pin);
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rErr: Sensor 3 ",18,10);
		}
	}

	else if(HAL_GPIO_ReadPin(SSR3_GPIO_Port,SSR3_Pin)== GPIO_PIN_SET && Timeout_loop5<=Timeout_value5){
		bouncing5 = (bouncing5<<1)|1;
	}

	else{
		Timeout_loop5=0;
		bouncing5=0xFF;
	}

	if (bouncing5==0x01){

		mm3=(mm-mmlast3);
		dd3=(dd-ddlast3);
		ms3=(ms-mslast3);

		if(ms3<0){
			dd3=dd3-1;
			ms3=ms3+60;
		}
		if(dd3<0){
			mm3=mm3-1;
			dd3=dd3+60;
		}

		if(mm3<0) mm3=0;
		if(dd3<0) dd3=0;
		if(ms3<0) ms3=0;

		mmlast3=mm;
		ddlast3=dd;
		mslast3=ms;

		if(runstop==1)	lapC++;
		else 			lapC=0;

		if(lapC<totlap){
			sprintf(buffs,"%d = %d%d:%d%d:%d%d",lapC,(mm3/10),(mm3%10),(dd3/10),(dd3%10),(ms3/10),(ms3%10));
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rTrack C Lap:",14,10);
			HAL_UART_Transmit(&huart1,(uint8_t*)buffs,sizeof(buffs),10);
		}
		else if (lapC==totlap)
		{
			lapC=totlap;
			sprintf(buffs,"%d = %d%d:%d%d:%d%d",lapC,(mm3/10),(mm3%10),(dd3/10),(dd3%10),(ms3/10),(ms3%10));
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rTrack C Lap:",14,10);
			HAL_UART_Transmit(&huart1,(uint8_t*)buffs,sizeof(buffs),10);
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\r-------Track C FINISH-----------",35,10);
		}
		else HAL_UART_Transmit(&huart1,(uint8_t*)"\n\r-------Track C FINISH-----------",35,10);

		Timeout_loop5=0;
		bouncing5=0xFF;
	}
}

void lcd(void)
{
	if(lapA<totlap)
	{
		lcd_send_cmd(0x80);
		sprintf(buffer,"STOPWATCH= %d%d:%d%d:%d%d",(mm/10),(mm%10),(dd/10),(dd%10),(ms1/10),(ms1%10));
		lcd_send_string(buffer);
		lcd_send_cmd(0xc0);
		sprintf(buffer,"Track A%d = %d%d:%d%d:%d%d",(lapA),(mm1/10),(mm1%10),(dd1/10),(dd1%10),(ms1/10),(ms1%10));
		lcd_send_string(buffer);

	}
	else if (lapA==totlap)
	{
		lcd_send_cmd(0xc0);
		sprintf(buffer,"A4 FINISH  %d%d:%d%d:%d%d",(mm1/10),(mm1%10),(dd1/10),(dd1%10),(ms1/10),(ms1%10));
		lcd_send_string(buffer);
	}

	if(lapB<totlap)
	{
		lcd_send_cmd(0x80);
		sprintf(buffer,"STOPWATCH= %d%d:%d%d:%d%d",(mm/10),(mm%10),(dd/10),(dd%10),(ms/10),(ms%10));
		lcd_send_string(buffer);
		lcd_send_cmd(0x94);
		sprintf(buffer,"Track B%d = %d%d:%d%d:%d%d",(lapB),(mm2/10),(mm2%10),(dd2/10),(dd2%10),(ms2/10),(ms2%10));
		lcd_send_string(buffer);

	}
	else if (lapB==totlap)
	{
		lcd_send_cmd(0x94);
		sprintf(buffer,"B4 FINISH  %d%d:%d%d:%d%d",(mm2/10),(mm2%10),(dd2/10),(dd2%10),(ms2/10),(ms2%10));
		lcd_send_string(buffer);
	}

	if(lapC<totlap)
	{
		lcd_send_cmd(0x80);
		sprintf(buffer,"STOPWATCH= %d%d:%d%d:%d%d",(mm/10),(mm%10),(dd/10),(dd%10),(ms/10),(ms%10));
		lcd_send_string(buffer);
		lcd_send_cmd(0xd4);
		sprintf(buffer,"Track C%d = %d%d:%d%d:%d%d",(lapC),(mm3/10),(mm3%10),(dd3/10),(dd3%10),(ms3/10),(ms3%10));
		lcd_send_string(buffer);
	}
	else if (lapC==totlap)
	{
		lcd_send_cmd(0xd4);
		sprintf(buffer,"C4 FINISH  %d%d:%d%d:%d%d",(mm3/10),(mm3%10),(dd3/10),(dd3%10),(ms3/10),(ms3%10));
		lcd_send_string(buffer);
	}
	if(lapA>=totlap && lapB>=totlap && lapC>=totlap)HAL_TIM_Base_Stop_IT(&htim3);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM3){
		ms++;
		if(ms>=60){
			dd++;
			ms=0;
		}
		if(dd>=60){
			mm++;
			dd=0;
		}
		if(mm>=100){
			ms=0;
			dd=0;
			mm=0;
		}
	}

	if(htim->Instance==TIM4){
		StartStop();
		Reset();
		ssr1();
		ssr2();
		ssr3();
		ulang++;
		if(ulang>=75){
			lcd();
			ulang=0;
		}
	}
}
