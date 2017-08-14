#include "main.h"


void Key_Configuration(void)
{
	GPIO_InitTypeDef gpio;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	gpio.GPIO_Mode = GPIO_Mode_OUT; 
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Pin = GPIO_Pin_6;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD,&gpio);
	
	gpio.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_15|GPIO_Pin_14|GPIO_Pin_13|GPIO_Pin_3;
	GPIO_Init(GPIOB,&gpio);

	gpio.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_Init(GPIOC,&gpio);
	
	
	}
void EXTI_Configuration(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB,EXTI_PinSource0);//PA8对中断线8
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  EXTI_InitStructure.EXTI_Trigger =   EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//使能按键所在的外部中断通道
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//抢占优先级0 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					//子优先级0 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 



}
static const char Key_Mem[4][4]={{'1','2','3','A'},
																		{'4','5','6','B'},
																		{'7','8','9','C'},
																		{'X','0','L','D'}};	
char Get_KeyValue(void)
{
			int col=0,row=0;
			Key_Mode1();
			delay_us(600);
			if (Key1_Val||Key2_Val||Key3_Val||Key4_Val){
			if (Key1_Val){
				row=0;
			}else if (Key2_Val){
				row=1;
			}else if (Key3_Val){
				row=2;
			}else if (Key4_Val){
				row=3;
			}
			Key_Mode2();
			delay_us(600);
			if (Key5_Val||Key6_Val||Key7_Val||Key8_Val){
			if (Key5_Val){
				col=0;
			}else if (Key6_Val){
				col=1;
			}else if (Key7_Val){
				col=2;
			}else if (Key8_Val){
				col=3;
			}
					return Key_Mem[row][col];
			}else{
			return 0;
			}
		}else{
			return 0;
		}
}
#define String_LENGTH 10
int step=1;
char Info_String[String_LENGTH][30]={{"Task1"},{"Task2"},{"Task3"},{"Task4"},{"Task5"},{"Task6"},{"Task7"},{"Press 8 and 2 To choose"},{"Press 5 To enter"},{"Press B To Calibrate"}};


void Mode_Task(void)
{
	char temp,temp2;
	int j=0;
	int i=0;
	char buffer[256];
	for (i=1;i<String_LENGTH+1;i++){LCD_DisplayString(40,i*30,16,(u8*)Info_String[i-1]);}
	BACK_COLOR=RED;
	LCD_DisplayString(40,30*step,16,(u8*)Info_String[step-1]);
	while (1){
		BACK_COLOR=BLACK;  //背景色 
		
		temp=Get_KeyValue();
		if (temp){
			if (temp=='8'){
				step++;
				step=LIMIT(step,1,7);
			}else if (temp=='2'){
				step--;
				step=LIMIT(step,1,7);
			}else if (temp=='B')
			{
			mpu6050.Acc_CALIBRATE = 1;		
			mpu6050.Gyro_CALIBRATE = 1;	
			LCD_Clear(BLACK);
				LCD_DisplayString(50,50,16,"Calibrating...");
				LCD_DisplayString(50,80,16,"Do not Touch it...");
				
				delay_ms(5000);
					AppParamSave();
				LCD_Clear(BLACK);
				
			}else if (temp=='C'){
				ResetAngle();
			
			}else if (temp=='0'){
			LCD_Clear(BLACK);
			LCD_DisplayString(30,30,16,"Camera Calibrate...");
			LCD_DisplayString(30,60,16,"Please put the ball on No.1 ");
			LCD_DisplayString(30,90,16,"When You Make It Please Enter X ");
				delay_ms(200);
				for (j=0;j<9;j++){
			while (1){
				temp2 = Get_KeyValue();
				if (temp2=='0')break;
			}
			if (temp2=='0'){
				TargetPosition[j][0]=My_x;
				TargetPosition[j][1]=My_y;
				sprintf(buffer,"No %d is (%.2f,%.2f)",j+1,TargetPosition[j][0],TargetPosition[j][1]);
				LCD_DisplayString(30,120,16,(u8*)buffer);
				delay_ms(300);
				AppParamSave();
			}
			
			}
				LCD_Clear(BLACK);
			
			sprintf(buffer,"(%.2f,%.2f)",TargetPosition[0][0],TargetPosition[0][1]);
				LCD_DisplayString(30,30,16,(u8*)buffer);
			sprintf(buffer,"(%.2f,%.2f)",TargetPosition[1][0],TargetPosition[1][1]);
				LCD_DisplayString(30,90,16,(u8*)buffer);
			sprintf(buffer,"(%.2f,%.2f)",TargetPosition[2][0],TargetPosition[2][1]);
				LCD_DisplayString(30,150,16,(u8*)buffer);
			sprintf(buffer,"(%.2f,%.2f)",TargetPosition[3][0],TargetPosition[3][1]);
				LCD_DisplayString(30,210,16,(u8*)buffer);
			sprintf(buffer,"(%.2f,%.2f)",TargetPosition[4][0],TargetPosition[4][1]);
				LCD_DisplayString(60,30,16,(u8*)buffer);
			sprintf(buffer,"(%.2f,%.2f)",TargetPosition[5][0],TargetPosition[5][1]);
				LCD_DisplayString(60,90,16,(u8*)buffer);
			sprintf(buffer,"(%.2f,%.2f)",TargetPosition[6][0],TargetPosition[6][1]);
				LCD_DisplayString(60,150,16,(u8*)buffer);
			sprintf(buffer,"(%.2f,%.2f)",TargetPosition[7][0],TargetPosition[7][1]);
				LCD_DisplayString(60,210,16,(u8*)buffer);
			sprintf(buffer,"(%.2f,%.2f)",TargetPosition[8][0],TargetPosition[8][1]);
				LCD_DisplayString(90,30,16,(u8*)buffer);
			sprintf(buffer,"(%.2f,%.2f)",TargetPosition[9][0],TargetPosition[9][1]);
				LCD_DisplayString(90,90,16,(u8*)buffer);			
				delay_ms(300);
			}
			while (Get_KeyValue()=='0')
			{
			break;
				LCD_Clear(BLACK);
			}
		for (i=1;i<String_LENGTH+1;i++){
			LCD_DisplayString(40,i*30,16,(u8*)Info_String[i-1]);
		}
			BACK_COLOR=RED;
			LCD_DisplayString(40,step*30,16,(u8*)Info_String[step-1]);
			BACK_COLOR=BLACK;
		if (temp=='5'){
	
			LCD_Clear(BLACK);
			BRUSH_COLOR=RED;
			LCD_DisplayString(40,30,16,"Now You Choose ");
			LCD_DisplayString(160,30,16,(u8*)Info_String[step-1]);
			LCD_DisplayString(40,60,16,"Press D To Choose Stop");
			LCD_DisplayString(40,90,16,"Press A To Restart");
			BRUSH_COLOR=WHITE;
			delay_ms(2000);
			LCD_Clear(BLACK);
			if ((enum PendulumMode)step==Task6){
				LCD_Clear(BLACK);
				LCD_DisplayString(30,30,16,"Please choose A B C D");
				LCD_DisplayString(30,60,16,"Please choose A");
				LCD_DisplayString(30,90,16,"Please choose B");
				LCD_DisplayString(30,120,16,"Please choose C");
				LCD_DisplayString(30,150,16,"Please choose D");
				
				for (i=0;i<4;i++){
				while (	1)	
				{
					temp2 = Get_KeyValue();
					if (temp2){
					
						if (temp2=='1'){ Task6_Buffer[i]=1;}
						if (temp2=='2'){ Task6_Buffer[i]=2;}
						if (temp2=='3'){ Task6_Buffer[i]=3;}
						if (temp2=='4'){ Task6_Buffer[i]=4;}
						if (temp2=='5'){ Task6_Buffer[i]=5;}
						if (temp2=='6'){ Task6_Buffer[i]=6;}
						if (temp2=='7'){ Task6_Buffer[i]=7;}
						if (temp2=='8'){ Task6_Buffer[i]=8;}
						if (temp2=='9'){ Task6_Buffer[i]=9;}
						
					sprintf(buffer,"A is %d                ",Task6_Buffer[i]);
					LCD_DisplayString(30,2*30+30*i,16,(u8*)buffer);
						delay_ms(400);
						break;
					}
				
				}

				}
				LCD_Clear(BLACK);
				while (1){
					LCD_DisplayString(30,30,16,"Please choose '5' to start");
					temp2 = Get_KeyValue();
					if (temp2){
						LCD_Clear(BLACK);
						break;
					}
					delay_ms(30);
				}
				
			
			}
			NS =(enum PendulumMode)step;
			
			break;
		}
		

		delay_ms(200);
		}
	delay_ms(30);
	}




}



