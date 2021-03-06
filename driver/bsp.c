#include "main.h"
 void All_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);    
	LED_Configuration();																//LED初始化
	TIM5_Configuration();																//定时器初始化
	TIM6_Configuration();																//TIM6初始化
	Usart2_Init(115200);																//串口2初始化
	Usart1_Init(115200);
	PWM_Configuration();																//PWM初始化
	///Adc_Init();																					//ADC初始化
	GPIO_Configuration();																//GPIO初始化
	Key_Configuration();																//按键

	Para_ResetToFactorySetup();													//从flash读取保存的参数
	PID_Para_Init();																		//PID初始化
	I2c_Soft_Init();																		//iic初始化
	Delay_ms(100);																			//mpu6050初始化之前必要延时
	MPU6050_Init(20); 																	//mpu6050初始化，低通频率20Hz
	Delay_ms(100);																			//磁力计初始化前的延时
	ak8975_ok = !(ANO_AK8975_Run());										//磁力计初始化
	delay_ms(100);
	LCD_Init();
	LCD_Clear(BLACK);
	BRUSH_COLOR=WHITE;	//画笔颜色
	BACK_COLOR=BLACK;  //背景色 
		Encoder_Configuration();
//	OV7670_Configuration();
	InnerLoopInit();																		//内环周期初始化
	TIM6_Start();																				//ControlLoop开始
	Encoder_Start();
	Mode_Task();																				//选择模式
	EXTI_Configuration();																//按键中断初始化
}



