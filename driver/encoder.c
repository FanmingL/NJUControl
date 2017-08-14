#include "main.h"
_encoder_st Pitch_motor_encoder=Encoder_InitVal,Roll_motor_encoder=Encoder_InitVal;
/*-----Pitch----TIM2----PA5---PA1*/
/*------Roll----TIM4---PD12---PD13*/
void Encoder_Configuration(void)
{
		GPIO_InitTypeDef gpio;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
		gpio.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
		gpio.GPIO_Mode = GPIO_Mode_AF;
		gpio.GPIO_PuPd = GPIO_PuPd_UP;
	gpio.GPIO_Mode= GPIO_Mode_IN;
		GPIO_Init(GPIOD,&gpio);
		gpio.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_1;
		GPIO_Init(GPIOA,&gpio);
	
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4);
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4);
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_TIM2);
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM2);
		
    TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);
    TIM_Cmd(TIM2,ENABLE);	
	
	   TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);
    TIM_Cmd(TIM4,ENABLE);	
}



void Encoder_Start(void)
{
    TIM2->CNT = 0x00;
		TIM4->CNT = 0x00;
		
		
}
void Refresh_Encoder(void)
{
	int TIM_TIM2;
	int TIM_TIM4;
	
	TIM_TIM2 = (short)(TIM2 -> CNT); TIM2->CNT=0;
	TIM_TIM4 = (short)(TIM4 -> CNT); TIM4->CNT=0;
/*-------------------------------------------------------------------------*/	
	Pitch_motor_encoder.T=((GetInnerLoop(Pitch_Time))/1000000.0f);
	
	Pitch_motor_encoder.encoder_temp = TIM_TIM2*PWMToDegree;
	
	Pitch_motor_encoder.speed=ANGLE_TO_RADIAN*TIM_TIM2*PWMToDegree/Pitch_motor_encoder.T;
	
	Pitch_motor_encoder.degree += Pitch_motor_encoder.encoder_temp;
/*--------------------------------------------------------------------------------------------------*/
	Roll_motor_encoder.T=((GetInnerLoop(Roll_Time))/1000000.0f);
	
	Roll_motor_encoder.encoder_temp = TIM_TIM4*PWMToDegree;
	
	Roll_motor_encoder.speed=ANGLE_TO_RADIAN*TIM_TIM4*PWMToDegree/Roll_motor_encoder.T;
	
	Roll_motor_encoder.degree += Roll_motor_encoder.encoder_temp;
	
	
	
	if (Pitch_motor_encoder.degree>360){Pitch_motor_encoder.degree-=360;Pitch_motor_encoder.circle_cnt++;}
	else if(Pitch_motor_encoder.degree<0){Pitch_motor_encoder.degree+=360;Pitch_motor_encoder.circle_cnt--;}
		if (Roll_motor_encoder.degree>360){Roll_motor_encoder.degree-=360;Roll_motor_encoder.circle_cnt++;}
	else if(Roll_motor_encoder.degree<0){Roll_motor_encoder.degree+=360;Roll_motor_encoder.circle_cnt--;}
}
