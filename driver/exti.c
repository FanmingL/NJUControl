#include "main.h"


void EXTI0_IRQHandler(void)
{		 
	char temp = Get_KeyValue();
	if (temp=='D'){
		LCD_Clear(BLACK);
		BRUSH_COLOR=RED;
		LCD_DisplayString(40,30,16,"Now You Choose Stop");
		BRUSH_COLOR=BLACK;
	}
	EXTI_ClearITPendingBit(EXTI_Line0);//清除LINE2上的中断标志位 
}


