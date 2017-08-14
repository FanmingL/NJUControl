#include "main.h"
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef gpio;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	gpio.GPIO_Mode = GPIO_Mode_OUT; 
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_12;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB,&gpio);
	
	gpio.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_0|GPIO_Pin_1;
	GPIO_Init(GPIOC,&gpio);
	
	Motor_Enable();
	Pitch_ON();
	Roll_ON();
}
