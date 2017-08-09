#include "main.h"


void EXTI0_IRQHandler(void)
{		 
	char temp = Get_KeyValue();
	EXTI_ClearITPendingBit(EXTI_Line0);
	if (temp=='D'){
		LCD_Clear(BLACK);
		BRUSH_COLOR=RED;
		LCD_DisplayString(40,30,16,"Now You Choose Stop");
		LCD_DisplayString(40,60,16,"Press A To Reset");
		BRUSH_COLOR=BLACK;
	}else if(temp=='A'){
		__set_FAULTMASK(1);
		NVIC_SystemReset();
	}
	//清除LINE2上的中断标志位 
}


