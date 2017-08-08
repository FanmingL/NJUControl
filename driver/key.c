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
static const char Key_Mem[4][4]={{'1','2','3','A'},
																		{'4','5','6','B'},
																		{'7','8','9','C'},
																		{'X','0','L','D'}};	
char Get_KeyValue(void)
{
			int col=0,row=0;
			Key_Mode1();
			delay_us(100);
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
			delay_us(100);
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
#define String_LENGTH 9
char Info_String[String_LENGTH][30]={{"Task1"},{"Task2"},{"Task3"},{"Task4"},{"Task5"},{"Task6"},{"Stop"},{"Press 8 and 2 To choose"},{"Press 5 To enter"}};
void Mode_Task(void)
{
	char temp;
	static int step=1;
	int i=0;
	for (i=1;i<String_LENGTH+1;i++){LCD_DisplayString(40,i*30,16,(u8*)Info_String[i-1]);}
	BACK_COLOR=RED;
	LCD_DisplayString(40,30*step,16,(u8*)Info_String[step-1]);
	while (1){
		BACK_COLOR=BLACK;  //±³¾°É« 
		
		temp=Get_KeyValue();
		if (temp){
			if (temp=='8'){
				step++;
				step=LIMIT(step,1,7);
			}else if (temp=='2'){
				step--;
				step=LIMIT(step,1,7);
			}
		for (i=1;i<String_LENGTH+1;i++){
			LCD_DisplayString(40,i*30,16,(u8*)Info_String[i-1]);
		}
			BACK_COLOR=RED;
			LCD_DisplayString(40,step*30,16,(u8*)Info_String[step-1]);
			BACK_COLOR=BLACK;
			delay_ms(300);
		if (temp=='5'){
			if (step!=0)
			{NS = (enum PendulumMode)step;}
			else
			{NS = Stop;}
			LCD_Clear(BLACK);
			BRUSH_COLOR=RED;
			LCD_DisplayString(40,30,16,"Now You Choose ");
			LCD_DisplayString(160,30,16,(u8*)Info_String[step-1]);
			BRUSH_COLOR=WHITE;
			break;
		}
		}
	
	delay_ms(30);
	}




}

